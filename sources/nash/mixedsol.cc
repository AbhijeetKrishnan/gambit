//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Solution class for mixed strategies
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

#include "math/gmath.h"
#include "mixedsol.h"
#include "game/nfdom.h"
#include "nfgpoly.h"

//----------------------------------------------------
// Constructors, Destructor, Constructive Operators
//----------------------------------------------------

MixedSolution::MixedSolution(const gbtMixedProfile<double> &p_profile,
			     const gbtText &p_creator)
  : m_profile(gbtNfgSupport(p_profile.GetGame())), m_precision(GBT_PREC_DOUBLE),
    m_support(p_profile.Support()), 
    m_creator(p_creator), m_Nash(), m_Perfect(), m_Proper(), 
    m_liapValue(), m_epsilon(0.0), m_qreLambda(-1), m_qreValue(-1),
    m_revision(p_profile.GetGame().RevisionNumber())
{
  gEpsilon(m_epsilon);
  for (int pl = 1; pl <= GetGame().NumPlayers(); pl++) {
    for (int st = 1; st <= GetGame().NumStrats(pl); st++) {
      int index = p_profile.Support().GetIndex(GetGame().GetPlayer(pl)->GetStrategy(st));
      if (index > 0)
	m_profile(pl, st) = p_profile(pl, index);
      else
	m_profile(pl, st) = gbtNumber(0.0);
    }
  }
}

MixedSolution::MixedSolution(const gbtMixedProfile<gbtRational> &p_profile,
			     const gbtText &p_creator)
  : m_profile(gbtNfgSupport(p_profile.GetGame())), m_precision(GBT_PREC_RATIONAL),
    m_support(p_profile.Support()),
    m_creator(p_creator), m_Nash(), m_Perfect(), m_Proper(), 
    m_liapValue(), m_qreLambda(-1), m_qreValue(-1),
    m_revision(p_profile.GetGame().RevisionNumber())
{
  gEpsilon(m_epsilon);
  for (int pl = 1; pl <= GetGame().NumPlayers(); pl++) {
    for (int st = 1; st <= GetGame().NumStrats(pl); st++) {
      int index = p_profile.Support().GetIndex(GetGame().GetPlayer(pl)->GetStrategy(st));
      if (index > 0)
	m_profile(pl, st) = p_profile(pl, index);
      else
	m_profile(pl, st) = gbtNumber(0);
    }
  }
}

MixedSolution::MixedSolution(const gbtMixedProfile<gbtNumber> &p_profile,
			     const gbtText &p_creator)
  : m_profile(gbtNfgSupport(p_profile.GetGame())), m_precision(GBT_PREC_RATIONAL),
    m_support(p_profile.Support()),
    m_creator(p_creator), m_Nash(), m_Perfect(), m_Proper(), 
    m_liapValue(), m_qreLambda(-1), m_qreValue(-1),
    m_revision(p_profile.GetGame().RevisionNumber())
{
  for (int pl = 1; pl <= GetGame().NumPlayers(); pl++) {
    for (int st = 1; st <= GetGame().NumStrats(pl); st++) {
      int index = p_profile.Support().GetIndex(GetGame().GetPlayer(pl)->GetStrategy(st));
      if (index > 0)
	m_profile(pl, st) = p_profile(pl, index);
      else
	m_profile(pl, st) = gbtNumber(0);
    }
  }
  LevelPrecision();
  if (m_profile[1].Precision() == GBT_PREC_DOUBLE)
    m_epsilon = 0.0;
  else
    m_epsilon = 0;
  gEpsilon(m_epsilon);
}

#if GBT_WITH_MP_FLOAT
//
// Since gbtNumber does not (yet) incorporate multiple-precision,
// cast the entries of profile down to machine double.
//
MixedSolution::MixedSolution(const gbtMixedProfile<gbtMPFloat> &p_profile,
			     const gbtText &p_creator)
  : m_profile(gbtNfgSupport(p_profile.GetGame())),
    m_precision(GBT_PREC_DOUBLE),
    m_support(p_profile.Support()), 
    m_creator(p_creator), m_Nash(), m_Perfect(), m_Proper(), 
    m_liapValue(), m_epsilon(0.0), m_qreLambda(-1), m_qreValue(-1),
    m_revision(p_profile.GetGame().RevisionNumber())
{
  gEpsilon(m_epsilon);
  for (int pl = 1; pl <= GetGame().NumPlayers(); pl++) {
    for (int st = 1; st <= GetGame().NumStrats(pl); st++) {
      int index = p_profile.Support().GetIndex(GetGame().GetPlayer(pl).GetStrategy(st));
      if (index > 0)
	m_profile(pl, st) = (double) p_profile(pl, index);
      else
	m_profile(pl, st) = gbtNumber(0.0);
    }
  }
}
#endif // GBT_WITH_MP_FLOAT

MixedSolution::MixedSolution(const MixedSolution &p_solution)
  : m_profile(p_solution.m_profile), m_precision(p_solution.m_precision),
    m_support(p_solution.m_support), m_creator(p_solution.m_creator), 
    m_Nash(p_solution.m_Nash), m_Perfect(p_solution.m_Perfect),
    m_Proper(p_solution.m_Proper), 
    m_liapValue(p_solution.m_liapValue), 
    m_epsilon(p_solution.m_epsilon),
    m_qreLambda(p_solution.m_qreLambda),
    m_qreValue(p_solution.m_qreValue), 
    m_name(p_solution.m_name), 
    m_revision(p_solution.m_revision)
{ }

MixedSolution::~MixedSolution()
{ }

MixedSolution &MixedSolution::operator=(const MixedSolution &p_solution)
{
  if (this != &p_solution)  {
    m_profile = p_solution.m_profile;
    m_precision = p_solution.m_precision;
    m_support = p_solution.m_support;
    m_creator = p_solution.m_creator;
    m_Nash = p_solution.m_Nash;
    m_Perfect = p_solution.m_Perfect;
    m_Proper = p_solution.m_Proper;
    m_liapValue = p_solution.m_liapValue;
    m_epsilon = p_solution.m_epsilon;
    m_qreLambda = p_solution.m_qreLambda;
    m_qreValue = p_solution.m_qreValue;
    m_name = p_solution.m_name;
    m_revision = p_solution.m_revision;
  }
  return *this;
}


//-----------------------------
// Private member functions
//-----------------------------

gbtTriState MixedSolution::GetNash(void) const
{
  gbtTriState answer;
  if(IsComplete())
    answer = (m_profile.MaxRegret() <= m_epsilon) ? GBT_TRISTATE_TRUE : GBT_TRISTATE_FALSE;
  else
    answer =  GBT_TRISTATE_FALSE;
  if (answer == GBT_TRISTATE_FALSE) {
    m_Perfect.Set(GBT_TRISTATE_FALSE);
    m_Proper.Set(GBT_TRISTATE_FALSE);
  }
  return answer;
}

gbtTriState MixedSolution::GetPerfect(void) const
{
  if (IsNash()) {
    gbtNullOutput gnull;
    if (IsMixedDominated(m_profile,false,m_precision,gnull)) {
      return GBT_TRISTATE_FALSE;
    }
    else if (GetGame().NumPlayers()==2) {
      return GBT_TRISTATE_TRUE;
    }
  }
  return GBT_TRISTATE_UNKNOWN;
}

gbtTriState MixedSolution::GetProper(void) const
{
  return GBT_TRISTATE_UNKNOWN;
}

void MixedSolution::LevelPrecision(void)
{
  m_precision = GBT_PREC_RATIONAL;
  m_epsilon = 0;
  for (int pl = 1; m_precision == GBT_PREC_RATIONAL && pl <= GetGame().NumPlayers();
       pl++) {
    gbtNfgPlayer player = GetGame().GetPlayer(pl);
    for (int st = 1; (m_precision == GBT_PREC_RATIONAL && 
		      st <= player->NumStrategies()); st++) {
      if (m_profile(pl, st).Precision() == GBT_PREC_DOUBLE) {
	m_precision = GBT_PREC_DOUBLE;
	m_epsilon = 0.0;
	gEpsilon(m_epsilon);
      }
    }
  }

  if (m_precision == GBT_PREC_DOUBLE) {
    for (int pl = 1; pl <= GetGame().NumPlayers(); pl++) {
      gbtNfgPlayer player = GetGame().GetPlayer(pl);
      for (int st = 1; st <= player->NumStrategies(); st++) 
	m_profile(pl, st) = (double) m_profile(pl, st);
    }
  }
}

//------------------------
// Operator overloading
//------------------------

bool MixedSolution::Equals(const gbtMixedProfile<double> &p_profile) const
{ 
  gbtNumber eps(m_epsilon);
  gEpsilon(eps, 4);   // this should be a function of m_epsilon

  int i = p_profile.First();
  while (i <= p_profile.Length()) {
    if (abs(m_profile[i] - (gbtNumber) p_profile[i]) > eps) 
      break;
    i++;
  }
  return (i > p_profile.Length());
}

bool MixedSolution::operator==(const MixedSolution &p_solution) const
{ return (m_profile == p_solution.m_profile); }

void MixedSolution::SetStrategyProb(gbtNfgAction p_strategy,
				    const gbtNumber &p_value)
{ 
  Invalidate();
  m_profile(p_strategy.GetPlayer()->GetId(), p_strategy.GetId()) = p_value;
  if (p_value.Precision() != m_precision) {
    LevelPrecision();
  }
}

const gbtNumber &MixedSolution::operator()(gbtNfgAction p_strategy) const
{
  gbtNfgPlayer player = p_strategy.GetPlayer();
  return m_profile(player->GetId(), p_strategy.GetId()); 
}

MixedSolution &MixedSolution::operator+=(const MixedSolution &p_solution)
{
  Invalidate();
  m_profile += p_solution.m_profile;
  if (m_precision == GBT_PREC_RATIONAL && p_solution.m_precision == GBT_PREC_DOUBLE)
    m_precision = GBT_PREC_DOUBLE;
  return *this;
}

MixedSolution &MixedSolution::operator-=(const MixedSolution &p_solution)
{
  Invalidate();
  m_profile -= p_solution.m_profile; 
  if (m_precision == GBT_PREC_RATIONAL && p_solution.m_precision == GBT_PREC_DOUBLE)
    m_precision = GBT_PREC_DOUBLE;
  return *this;
}

MixedSolution &MixedSolution::operator*=(const gbtNumber &p_constant)
{ 
  Invalidate(); 
  m_profile *= p_constant;
  if (m_precision == GBT_PREC_RATIONAL && p_constant.Precision() == GBT_PREC_DOUBLE)
    m_precision = GBT_PREC_DOUBLE;
  return *this; 
}


//-----------------------
// General data access
//-----------------------

bool MixedSolution::IsComplete(void) const
{ 
  for (int pl = 1; pl <= GetGame().NumPlayers(); pl++) {
    gbtNumber sum = -1;
    for (int st = 1; st <= GetGame().NumStrats(pl); st++) 
      sum += m_profile(pl, st);
    if (sum > m_epsilon || sum < -m_epsilon) 
      return false;
  }
  return true;
}

const gbtTriState &MixedSolution::IsNash(void) const
{
  CheckIsValid();
  if(!m_Nash.Checked())
    m_Nash.Set(GetNash());
  return m_Nash.Answer();
}

MixedSolution MixedSolution::PolishEq(void) const
{
  bool is_singular = false;
  return PolishEquilibrium(m_support,*this,is_singular);
}

const gbtTriState &MixedSolution::IsPerfect(bool p_compute /*=false*/) const
{
  CheckIsValid();
  if (!m_Perfect.Checked() && p_compute) {
    m_Perfect.Set(GetPerfect());
  }
  return m_Perfect.Answer();
}

const gbtTriState &MixedSolution::IsProper(void) const
{
  CheckIsValid();
  if(!m_Proper.Checked())
    m_Proper.Set(GetProper());
  return m_Proper.Answer();
}

const gbtNumber &MixedSolution::GetLiapValue(void) const 
{ 
  CheckIsValid();
  if(!m_liapValue.Checked())
    m_liapValue.Set(m_profile.LiapValue());
  return m_liapValue.Answer();
}

void MixedSolution::Invalidate(void) const
{
  m_support = gbtNfgSupport(m_profile.GetGame());
  m_creator = "User";
  m_Nash.Invalidate();
  m_Perfect.Invalidate();
  m_Proper.Invalidate();
  m_liapValue.Invalidate();
  m_qreLambda = -1;
  m_qreValue = -1;
  m_revision = GetGame().RevisionNumber();
}

//---------------------
// Payoff computation
//---------------------

gbtNumber MixedSolution::GetPayoff(gbtNfgPlayer p_player) const
{
  return m_profile.Payoff(p_player->GetId());
}

gbtNumber MixedSolution::GetStrategyValue(gbtNfgAction p_strategy) const
{
  return m_profile.Payoff(p_strategy.GetPlayer()->GetId(), p_strategy); 
}


//----------
// Output
//----------

void MixedSolution::Dump(gbtOutput &p_file) const
{
  m_profile.Dump(p_file);
  DumpInfo(p_file);
}

void MixedSolution::DumpInfo(gbtOutput &p_file) const
{
  p_file << " Creator:" << GetCreator();
  p_file << " IsNash:" << IsNash();
  p_file << " IsPerfect:" << IsPerfect();
  p_file << " IsProper:" << IsProper();
  p_file << " LiapValue:" << GetLiapValue();
  if (m_creator == "Qre[NFG]" || m_creator == "QreGrid[NFG]") {
    p_file << " QreLambda:" << m_qreLambda;
    p_file << " QreValue:" << m_qreValue;
  }
}

gbtOutput &operator<<(gbtOutput &p_file, const MixedSolution &p_solution)
{ 
  p_solution.Dump(p_file);
  return p_file;
}

#include "base/glist.imp"
template class gbtList<MixedSolution>;
