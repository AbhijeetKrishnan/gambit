//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Compute dominated mixed strategies on normal forms
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

#include "base/gstream.h"
#include "base/gstatus.h"
#include "numerical/lpsolve.h"
#include "game.h"
#include "nfgiter.h"
#include "nfgciter.h"
#include "nfstrat.h"
#include "nfdom.h"

#include "nfdommix.imp"

gbtNfgSupport gbtNfgSupport::MixedUndominated(bool strong, gbtPrecision precision,
				      const gbtArray<int> &players,
				      gbtOutput &tracefile,
				      gbtStatus &status) const
{
  gbtNfgSupport newS(*this);
  
  if (precision == GBT_PREC_RATIONAL) {
    for (int i = 1; i <= players.Length(); i++)   {
      status.Get();
      int pl = players[i];
      ComputeMixedDominated(*this, newS, pl, strong,
			    (gbtRational)0, tracefile, status);
    }
  }
  else if (precision == GBT_PREC_DOUBLE) {
    for (int i = 1; i <= players.Length(); i++)   {
      status.Get();
      int pl = players[i];
      ComputeMixedDominated(*this, newS, pl, strong,
			    (double)0, tracefile, status);
    }
  }

  tracefile << "\n";
  return newS;
}

bool IsMixedDominated(const gbtNfgSupport &S, gbtGameStrategy str,
		      bool strong, gbtPrecision precision,
		      gbtOutput &tracefile)
{
  bool ret = false;
  if (precision == GBT_PREC_RATIONAL) {
    ret = IsMixedDominated(S, str, strong, (gbtRational)0, tracefile);
  }
  else if (precision == GBT_PREC_DOUBLE) { 
    ret = IsMixedDominated(S, str, strong, (double)0, tracefile);
  }
  return ret;
}

bool IsMixedDominated(const gbtMixedProfile<gbtNumber> &sol,
		 bool strong, gbtPrecision precision, gbtOutput &tracefile)
{
  bool ret = false;
  int n = sol.GetGame()->NumPlayers();
  int i=1;

  while (i<=n && !ret) {
    ret = IsMixedDominated(sol,i,strong, precision, tracefile);
    i++;
  }
  return ret;
}

bool IsMixedDominated(const gbtMixedProfile<gbtNumber> &sol, int pl,
		 bool strong, gbtPrecision precision, gbtOutput &tracefile)
{
  bool ret = false;

  if(precision == GBT_PREC_RATIONAL) {
    gbtMixedProfile<gbtRational> p(sol.Support());
    for (int i = 1; i <= p.Length(); i++)
      p[i] = sol[i];
    ret =  IsMixedDominated(p, pl, strong, tracefile);
  }
  else if (precision == GBT_PREC_DOUBLE) {
    gbtMixedProfile<double> p(sol.Support());
    for (int i = 1; i <= p.Length(); i++)
      p[i] = sol[i];
    
    ret =  IsMixedDominated(p, pl, strong, tracefile);
  }
  
  return ret;
}

// Note: junk is dummy arg so the functions can be templated. 
// There is probably a cleaner way to do this.  
#ifndef __BCC55__
template bool 
ComputeMixedDominated(const gbtNfgSupport &S, gbtNfgSupport &R,int pl, bool strong, 
		      gbtRational junk, gbtOutput &tracefile, gbtStatus &status);
template bool 
ComputeMixedDominated(const gbtNfgSupport &S, gbtNfgSupport &R,int pl, bool strong, 
		      double junk, gbtOutput &tracefile, gbtStatus &status);

template bool
IsMixedDominated(const gbtNfgSupport &S, gbtGameStrategy str,
		 bool strong, gbtRational junk, gbtOutput &tracefile);
template bool
IsMixedDominated(const gbtNfgSupport &S, gbtGameStrategy str,
		 bool strong, double junk, gbtOutput &tracefile);

template bool 
IsMixedDominated(const gbtMixedProfile<gbtRational> &pr, int pl,
		 bool strong, gbtOutput &tracefile);

template bool 
IsMixedDominated(const gbtMixedProfile<double> &pr, int pl,
		 bool strong, gbtOutput &tracefile);
#endif  // __BCC55__
