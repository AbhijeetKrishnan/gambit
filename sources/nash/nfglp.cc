//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Instantiation of algorithm to compute mixed strategy equilibria
// of constant sum normal form games via linear programming
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

#include "nfglp.imp"
#include "math/rational.h"

template class gbtNfgNashLp<double>;
template gbtList<gbtMixedProfile<double> >
gbtNashLpNfg(const gbtGame &, const double &);

template class gbtNfgNashLp<gbtRational>;
template gbtList<gbtMixedProfile<gbtRational> >
gbtNashLpNfg(const gbtGame &, const gbtRational &);
