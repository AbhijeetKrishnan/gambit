//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Interface to player representation classes
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

#ifndef PLAYER_H
#define PLAYER_H

//
// The extensive form and normal form player classes are both included
// in this header file in the hopes of an eventual unification of the
// two.
//

struct gbt_efg_player_rep;
class gbtEfgGame;
class gbtEfgInfoset;

class gbtEfgPlayer {
friend class gbtEfgGame;
protected:
  struct gbt_efg_player_rep *rep;

public:
  gbtEfgPlayer(void);
  gbtEfgPlayer(gbt_efg_player_rep *);
  gbtEfgPlayer(const gbtEfgPlayer &);
  ~gbtEfgPlayer();

  gbtEfgPlayer &operator=(const gbtEfgPlayer &);

  bool operator==(const gbtEfgPlayer &) const;
  bool operator!=(const gbtEfgPlayer &) const;

  bool IsNull(void) const;
  bool IsDeleted(void) const;

  gbtEfgGame GetGame(void) const;
  gText GetLabel(void) const;
  void SetLabel(const gText &);
  int GetId(void) const;

  bool IsChance(void) const;

  int NumInfosets(void) const;
  gbtEfgInfoset NewInfoset(int p_actions);
  gbtEfgInfoset GetInfoset(int p_index) const;
};


struct gbt_nfg_player_rep;
class gbtNfgGame;
class gbtNfgStrategy;

class gbtNfgPlayer {
friend class gbtNfgGame;
friend class gbtNfgOutcome;
protected:
  struct gbt_nfg_player_rep *rep;

public:
  gbtNfgPlayer(void);
  gbtNfgPlayer(gbt_nfg_player_rep *);
  gbtNfgPlayer(const gbtNfgPlayer &);
  ~gbtNfgPlayer();

  gbtNfgPlayer &operator=(const gbtNfgPlayer &);

  bool operator==(const gbtNfgPlayer &) const;
  bool operator!=(const gbtNfgPlayer &) const;

  bool IsNull(void) const;
  bool IsDeleted(void) const;

  gbtNfgGame GetGame(void) const;
  gText GetLabel(void) const;
  void SetLabel(const gText &);
  int GetId(void) const;

  int NumStrategies(void) const;
  gbtNfgStrategy GetStrategy(int) const;
};

#endif  // PLAYER_H
