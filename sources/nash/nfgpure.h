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

#ifndef NFGPURE_H
#define NFGPURE_H

#include "base/base.h"
#include "base/gstatus.h"
#include "nfgalgorithm.h"

class gbtNfgNashEnumPure : public gbtNfgNashAlgorithm {
private:
  int m_stopAfter;

public:
  gbtNfgNashEnumPure(void) : m_stopAfter(0) { }
  virtual ~gbtNfgNashEnumPure() { }

  int StopAfter(void) const { return m_stopAfter; }
  void SetStopAfter(int p_stopAfter) { m_stopAfter = p_stopAfter; }

  gbtText GetAlgorithm(void) const { return "EnumPure"; }
  gbtMixedNashSet Solve(const gbtNfgGame &, gbtStatus &);
};

#endif // NFGPURE_H
