//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Algorithm to compute pure strategy equilibria on normal forms
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

#include "nfgpure.h"

#include "base/base.h"
#include "game/nfg.h"
#include "game/nfgiter.h"
#include "game/nfgciter.h"

gbtList<MixedSolution> gbtNfgNashEnumPure::Solve(const gbtNfgSupport &p_support,
					       gbtStatus &p_status)
{
  gbtNfgGame nfg = p_support.GetGame();
  gbtList<MixedSolution> solutions;
  gbtNfgContIterator citer(p_support);

  int ncont = 1;
  for (int pl = 1; pl <= nfg.NumPlayers(); pl++) {
    ncont *= p_support.NumStrats(pl);
  }

  int contNumber = 1;
  try {
    do  {
      p_status.Get();
      p_status.SetProgress((double) contNumber / (double) ncont);

      bool flag = true;
      gbtNfgIterator niter(citer);
    
      for (int pl = 1; flag && pl <= nfg.NumPlayers(); pl++)  {
	gbtNumber current = citer.GetPayoff(nfg.GetPlayer(pl));
	for (int i = 1; i <= p_support.NumStrats(pl); i++)  {
	  niter.Next(pl);
	  if (niter.GetPayoff(nfg.GetPlayer(pl)) > current)  {
	    flag = false;
	    break;
	  }
	}
      }
      
      if (flag)  {
	gbtMixedProfile<gbtNumber> temp(p_support.GetGame());
	((gbtVector<gbtNumber> &) temp).operator=(gbtNumber(0));
	MixedSolution soln(temp, "EnumPure[NFG]");
	for (int pl = 1; pl <= p_support.GetGame().NumPlayers(); pl++) {
	  soln.SetStrategyProb(citer.GetProfile().GetStrategy(pl), 1);
	}
	solutions.Append(soln);
      }
      contNumber++;
    }  while ((m_stopAfter == 0 || solutions.Length() < m_stopAfter) &&
	      citer.NextContingency());
  }
  catch (gbtSignalBreak &) {
    // catch exception; return list of computed equilibria (if any)
  }

  return solutions;
}
