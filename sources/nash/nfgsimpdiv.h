//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Compute Nash equilibria via simplicial subdivision on the normal form
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

#ifndef NFGSIMPDIV_H
#define NFGSIMPDIV_H

#include "nfgalgorithm.h"

template <class T> class gbtNfgNashSimpdiv : public gbtNfgNashAlgorithm {
private:
  int m_nRestarts, m_leashLength;

  int t, ibar;
  T pay,d,maxz,bestz,mingrid;

  T Simplex(gbtMixedProfile<T> &);
  T getlabel(gbtMixedProfile<T> &yy, gbtArray<int> &, gbtPVector<T> &);
  void update(gbtRectArray<int> &, gbtRectArray<int> &, gbtPVector<T> &,
	      const gbtPVector<int> &, int j, int i);
  void getY(gbtMixedProfile<T> &x, gbtPVector<T> &, 
	    const gbtPVector<int> &, const gbtPVector<int> &, 
	    const gbtPVector<T> &, const gbtRectArray<int> &, int k);
  void getnexty(gbtMixedProfile<T> &x, const gbtRectArray<int> &,
		const gbtPVector<int> &, int i);
  int get_c(int j, int h, int nstrats, const gbtPVector<int> &);
  int get_b(int j, int h, int nstrats, const gbtPVector<int> &);
  
public:
  gbtNfgNashSimpdiv(void);
  virtual ~gbtNfgNashSimpdiv();

  int NumRestarts(void) const { return m_nRestarts; }
  void SetNumRestarts(int p_nRestarts) { m_nRestarts = p_nRestarts; }

  int LeashLength(void) const { return m_leashLength; }
  void SetLeashLength(int p_leashLength) { m_leashLength = p_leashLength; }

  gbtText GetAlgorithm(void) const { return "Simpdiv[NFG]"; }
  gbtList<MixedSolution> Solve(const gbtNfgSupport &, gbtStatus &);
};

#endif  // NFGSIMPDIV_H
