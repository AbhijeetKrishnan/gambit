//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Compute Nash equilibria via Lyapunov function minimization
//
// This file is part of Gambit
// Copyright (c) 2002, The Gambit Project
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//

#include "efgliap.h"
#include "math/gmatrix.h"
#include "numerical/gfuncmin.h"

class EFLiapFunc : public gbtC1Function<double>  {
private:
  mutable long _nevals;
  gbtEfgGame m_efg;
  mutable gbtBehavProfile<double> _p;

  double Value(const gbtVector<double> &x) const;
  bool Gradient(const gbtVector<double> &, gbtVector<double> &) const;

public:
  EFLiapFunc(const gbtEfgGame &, const gbtBehavProfile<double> &);
  virtual ~EFLiapFunc();
    
  long NumEvals(void) const  { return _nevals; }
};


EFLiapFunc::EFLiapFunc(const gbtEfgGame &p_efg,
		       const gbtBehavProfile<double> &start)
  : _nevals(0L), m_efg(p_efg), _p(start)
{ }

EFLiapFunc::~EFLiapFunc()
{ }


double EFLiapFunc::Value(const gbtVector<double> &v) const
{
  _nevals++;
  ((gbtVector<double> &) _p).operator=(v);
    //_p = v;
  // Don't impose penalties in Lyapunov function; avoid this as
  // we go out of the feasible set in numerically computing the
  // derivative of the function.
  return _p.LiapValue(false);
}

//
// This function projects a gradient into the plane of the simplex.
// (Actually, it works by computing the projection of 'x' onto the
// vector perpendicular to the plane, then subtracting to compute the
// component parallel to the plane.)  Also imposes binding nonnegativity
// constraints as appropriate.
//
static void Project(gbtVector<double> &grad, const gbtVector<double> &x,
		    const gbtArray<int> &lengths)
{
  int index = 1;
  for (int part = 1; part <= lengths.Length(); part++)  {
    double avg = 0.0;
    int nactive = 0;
    int j;
    for (j = 1; j <= lengths[part]; j++, index++)  {
      // If x[index] is small, assume nonnegativity is binding.
      // On the other hand, if gradient suggests that the minimizing
      // direction is towards the interior, let it go (relax constraint).
      if (x[index] > 1.0e-7 || grad[index] < 0.0) {
	avg += grad[index];
	nactive++;
      }
    }
    avg /= (double) nactive;
    index -= lengths[part];
    for (j = 1; j <= lengths[part]; j++, index++)  {
      if (x[index] > 1.0e-7 || grad[index] < 0.0) {
	grad[index] -= avg;
      }
      else {
	grad[index] = 0.0;
      }
    }
  }
}

bool EFLiapFunc::Gradient(const gbtVector<double> &x,
			  gbtVector<double> &grad) const
{
  const double DELTA = .00001;

  ((gbtVector<double> &) _p).operator=(x);
  for (int i = 1; i <= x.Length(); i++) {
    _p[i] += DELTA;
    double value = Value(_p.GetDPVector());
    _p[i] -= 2.0 * DELTA;
    value -= Value(_p.GetDPVector());
    _p[i] += DELTA;
    grad[i] = value / (2.0 * DELTA);
  }

  // Project for constraints
  Project(grad, x, _p.GetPVector().Lengths());

  return true;
}

static void PickRandomProfile(gbtBehavProfile<double> &p)
{
  double sum, tmp;

  for (int pl = 1; pl <= p.GetGame()->NumPlayers(); pl++)  {
    for (int iset = 1; iset <= p.GetGame()->GetPlayer(pl)->NumInfosets();
	 iset++)  {
      sum = 0.0;
      int act;
    
      for (act = 1; act < p.Support().NumActions(pl, iset); act++)  {
	do
	  tmp = Uniform();
	while (tmp + sum > 1.0);
	p(pl, iset, act) = tmp;
	sum += tmp;
      }
  
// with truncation, this is unnecessary
      p(pl, iset, act) = 1.0 - sum;
    }
  }
}

gbtEfgNashLiap::gbtEfgNashLiap(void)
  : m_stopAfter(1), m_numTries(10), m_maxits1(100), m_maxitsN(20),
    m_tol1(2.0e-10), m_tolN(1.0e-10)
{ }

gbtList<BehavSolution> gbtEfgNashLiap::Solve(const gbtEfgSupport &p_support,
					   gbtStatus &p_status)
{
  static const double ALPHA = .00000001;

  gbtBehavProfile<double> p(p_support);
  EFLiapFunc F(p_support.GetTree(), p);

  // if starting vector not interior, perturb it towards centroid
  int kk = 0;
  for (int kk = 1; kk <= p.Length() && p[kk] > ALPHA; kk++);
  if (kk <= p.Length()) {
    gbtBehavProfile<double> c(p_support);
    for (int k = 1; k <= p.Length(); k++) {
      p[k] = c[k]*ALPHA + p[k]*(1.0-ALPHA);
    }
  }

  gbtMatrix<double> xi(p.Length(), p.Length());

  gbtList<BehavSolution> solutions;
  
  try {
    for (int i = 1; (m_numTries == 0 || i <= m_numTries) &&
	   (m_stopAfter == 0 || solutions.Length() < m_stopAfter); 
	 i++)   {
      p_status.Get();
      p_status.SetProgress((double) i / (double) m_numTries,
			   gbtText("Attempt ") + ToText(i) + 
			   gbtText(", equilibria so far: ") +
			   ToText(solutions.Length())); 
      gbtConjugatePRMinimizer minimizer(p.Length());
      gbtVector<double> gradient(p.Length()), dx(p.Length());
      double fval;
      minimizer.Set(F, p.GetDPVector(), fval, gradient, .01, .0001);

      try {
	for (int iter = 1; iter <= m_maxitsN; iter++) {
	  if (iter % 20 == 0) {
	    p_status.Get();
	  }
	  
	  if (!minimizer.Iterate(F, p.GetDPVector(), fval, gradient, dx)) {
	    break;
	  }

	  if (sqrt(gradient.NormSquared()) < .001 &&
	      fval < 1.0e-8) {
	    solutions.Append(BehavSolution(p, "Liap[EFG]"));
	    break;
	  }
	}
      }
      catch (gbtFuncMinException &) { }
      PickRandomProfile(p);
    }
  }
  catch (gbtSignalBreak &) {
    // Just stop and return any solutions found so far
  }
  // Any other exceptions propagate out, assuming something Real Bad happened

  return solutions;
}
