//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Normal form iterator class
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

#ifndef NFGITER_H
#define NFGITER_H

template <class T> class gbtArray;

class gbtNfgContingency;
#include "nfg.h"
#include "nfstrat.h"
class gbtNfgContIterator;

//
// This class is useful for iterating around the normal form.
//

class NfgIter    {
private:
  gbtNfgSupport support;
  gbtNfgGame m_nfg;
  gbtArray<int> current_strat;
  gbtNfgContingency profile;

public:
  NfgIter(gbtNfgGame);
  NfgIter(const gbtNfgSupport &s);
  NfgIter(const NfgIter &);
  NfgIter(const gbtNfgContIterator &);
  ~NfgIter();

  NfgIter &operator=(const NfgIter &);

  void First(void);
  int Next(int p);
  int Set(int p, int s);

  void Get(gbtArray<int> &t) const;
  void Set(const gbtArray<int> &t);

  gbtNfgOutcome GetOutcome(void) const;
  void SetOutcome(gbtNfgOutcome);

  gbtNumber GetPayoff(const gbtNfgPlayer &p_player) const 
    { return profile.GetPayoff(p_player); }

  const gbtNfgSupport &Support(void) const { return support; }
};

#endif   // NFGITER_H
