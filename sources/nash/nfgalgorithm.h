//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Declaration of abstract class to compute Nash equilibria on nfgs
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

#ifndef NFGALGORITHM_H
#define NFGALGORITHM_H

#include "game/game.h"
#include "game/mixed.h"

//
// This class is intended to replace MixedSolution and the usage of
// gbtList<MixedSolution> throughout Gambit.
// Currently simply a glorified list of profiles, it should evolve
// to permit representation of equilibrium components, etc., as appropriate
//
class gbtMixedNashSet : public gbtList<gbtMixedProfile<gbtNumber> > {
};

class gbtNfgNashAlgorithm {
public:
  virtual ~gbtNfgNashAlgorithm() { }

  virtual gbtText GetAlgorithm(void) const = 0;
  virtual gbtMixedNashSet Solve(const gbtNfgGame &, gbtStatus &) = 0;
};

#endif  // NFGALGORITHM_H
