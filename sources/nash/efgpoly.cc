//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Enumerates all Nash equilibria of a game, via gbtPolyUni equations
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

#include "game/efgensup.h"
#include "efgpoly.imp"

//---------------------------------------------------------------------------
//                        EfgPolEnumParams: member functions
//---------------------------------------------------------------------------

EfgPolEnumParams::EfgPolEnumParams(void)
  : stopAfter(0)
{ }

//---------------------------------------------------------------------------
//                    EfgPolEnum: nontemplate functions
//---------------------------------------------------------------------------

template class EfgPolEnumModule<gbtDouble>;

int EfgPolEnum(const gbtEfgSupport &support, const EfgPolEnumParams &params,
	       gbtList<BehavSolution> &solutions, gbtStatus &p_status,
	       long &nevals, double &time, bool &is_singular)
{
  EfgPolEnumModule<gbtDouble> module(support, params);
  module.EfgPolEnum(p_status);
  nevals = module.NumEvals();
  time = module.Time();
  solutions = module.GetSolutions();
  is_singular = module.IsSingular();

  return 1;
}

BehavSolution PolishEquilibrium(const gbtEfgSupport &support, 
				const BehavSolution &sol, 
				bool &is_singular)
{
  EfgPolEnumParams params;
  EfgPolEnumModule<gbtDouble> module(support, params);
  gbtVector<gbtDouble> vec = module.SolVarsFromBehavProfile(sol.Profile());
  module.PolishKnownRoot(vec);
  return module.ReturnPolishedSolution(vec);
}

//=======================================================================
//                    class gbtEfgNashEnumPoly
//=======================================================================

gbtEfgNashEnumPoly::gbtEfgNashEnumPoly(void)
  : m_stopAfter(0)
{ }

gbtList<BehavSolution> gbtEfgNashEnumPoly::Solve(const gbtEfgSupport &p_support,
					       gbtStatus &p_status)
{
  p_status.SetProgress(0.0);
  p_status << "Step 1 of 2: Enumerating supports";
  gbtList<gbtEfgSupport> supports = PossibleNashSubsupports(p_support,
							    p_status);

  p_status.SetProgress(0.0);
  p_status << "Step 2 of 2: Computing equilibria";

  gbtList<gbtEfgSupport> singularSupports;
  gbtList<BehavSolution> solutions;

  try {
    for (int i = 1; (i <= supports.Length() &&
		     (m_stopAfter == 0 || m_stopAfter > solutions.Length()));
	 i++) {
      p_status.Get();
      p_status.SetProgress((double) (i-1) / (double) supports.Length());
      long newevals = 0;
      double newtime = 0.0;
      gbtList<BehavSolution> newsolns;
      bool is_singular = false;

      EfgPolEnumParams params;
      params.stopAfter = 0;
      EfgPolEnum(supports[i], params, newsolns, p_status, 
		 newevals, newtime, is_singular);
      for (int j = 1; j <= newsolns.Length(); j++) {
	if (newsolns[j].IsANFNash()) {
	  solutions += newsolns[j];
	}
      }

      if (is_singular) { 
	singularSupports += supports[i];
      }
    }
  }
  catch (gbtSignalBreak &) {
    // catch exception; return list of computed equilibria (if any)
  }

  return solutions;
}
