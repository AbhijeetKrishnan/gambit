//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Enumerate undominated subsupports of a support
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

#include "libgambit/libgambit.h"

class StrategyCursorForSupport;

// We build a series of functions of increasing complexity.  The
// final one, which is our goal, is the undominated support function.
// We begin by simply enumerating all subsupports.

void AllSubsupportsRECURSIVE(const gbtNfgSupport &s,
			     gbtNfgSupport *sact,
			     StrategyCursorForSupport *c,
			     gbtList<gbtNfgSupport> &list);

gbtList<gbtNfgSupport> AllSubsupports(const gbtNfgSupport &S);

// Subsupports of a given support are _valid_ if each agent has an action.

void AllValidSubsupportsRECURSIVE(const gbtNfgSupport &s,
				  gbtNfgSupport *sact,
				  StrategyCursorForSupport *c,
				  gbtList<gbtNfgSupport> &list);

gbtList<gbtNfgSupport> AllValidSubsupports(const gbtNfgSupport &S);

// The following routines combine to return all supports that do not 
// exhibit particular type of domination.  This was a prototype for 
// PossibleNashSubsupports, and displays the methods used there,
// but it does NOT do exactly what is advertised with respect to 
// weak domination.  This is because the recursion may eliminate
// a strategy that is weakly dominated at some stage of the truncation
// process, when, after more truncations, it might be no longer weakly
// dominated, and thus part of an allowed subsupport.

void AllUndominatedSubsupportsRECURSIVE(const gbtNfgSupport &s,
					gbtNfgSupport *sact,
					StrategyCursorForSupport *c,
					bool strong,
					gbtList<gbtNfgSupport> &list);
  
gbtList<gbtNfgSupport> AllUndominatedSubsupports(const gbtNfgSupport &S,
					       bool strong,
					       bool conditional);

// The following two routines combine to produce all subsupports that could
// host the path of a behavioral Nash equilibrium.  These are subsupports
// that have no strategy, at an active infoset, that is weakly dominated by
// another active strategy, either in the conditional sense (for any active
// node in the infoset) or the unconditional sense.  In addition we 
// check for domination by strategys that are inactive, but whose activation
// would not activate any currently inactive infosets, so that the
// subsupport resulting from activation is consistent, in the sense
// of having active strategys at all active infosets, and not at other
// infosets.

void PossibleNashSubsupportsRECURSIVE(const gbtNfgSupport &s,
				      gbtNfgSupport *sact,
				      StrategyCursorForSupport *c,
				      gbtList<gbtNfgSupport> &list);

gbtList<gbtNfgSupport> SortSupportsBySize(gbtList<gbtNfgSupport> &);
  
gbtList<gbtNfgSupport> PossibleNashSubsupports(const gbtNfgSupport &S);

///////////////// Utility Cursor Class /////////////////////

class StrategyCursorForSupport {
protected:
  const gbtNfgSupport *support;
  int pl;
  int strat;

public:
  //Constructors and dtor
  StrategyCursorForSupport(const gbtNfgSupport &S);
  StrategyCursorForSupport(const StrategyCursorForSupport &s);
  ~StrategyCursorForSupport();

  // Operators
  StrategyCursorForSupport &operator =(const StrategyCursorForSupport &);
  bool                    operator==(const StrategyCursorForSupport &) const;
  bool                    operator!=(const StrategyCursorForSupport &) const;

  // Manipulation
  bool GoToNext();

  // Information
  Gambit::GameStrategy GetStrategy() const;
  int StrategyIndex() const;
  Gambit::GamePlayer GetPlayer() const;
  int PlayerIndex() const;

  bool IsLast() const;
  bool IsSubsequentTo(const Gambit::GameStrategy &) const;
};

