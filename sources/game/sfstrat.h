//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Interface to sequence form strategy classes
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

#ifndef SFSTRAT_H
#define SFSTRAT_H

#include "base/base.h"
#include "game/efg.h"

/*
class Efg;
class Sfg;
class Action;
*/

struct Sequence {
friend class Sfg;
friend class FullEfg;
friend class SFSequenceSet;
private:
  int number;
  gbtText name;
  gbtEfgPlayer player;
  gbtEfgAction action;
  const Sequence *parent;
  
  Sequence(const gbtEfgPlayer &pl, const gbtEfgAction &a,
	   const Sequence *p, int n) 
    : number(n), player(pl), action(a), parent(p) { }
  ~Sequence() { }
public:
  const gbtText &GetName(void) const   { return name; }
  void SetName(const gbtText &s)       { name = s; }
  
  gbtList<gbtEfgAction> History(void) const;
  int GetNumber(void) const        { return number; }
  gbtEfgAction GetAction(void) const  {return action; }
  gbtEfgInfoset GetInfoset(void) const 
  { if (!action.IsNull()) return action.GetInfoset(); else return 0; }
  gbtEfgPlayer Player(void) const  { return player; }
  const Sequence *Parent(void) const   { return parent; }
  void Dump(gbtOutput &) const;
};

class SFSequenceSet {
protected:
  gbtEfgPlayer efp;
  gbtBlock <Sequence *> sequences;
  
public:
  SFSequenceSet(const SFSequenceSet &s); 
  SFSequenceSet(const gbtEfgPlayer &);
  
  SFSequenceSet &operator=(const SFSequenceSet &s); 
  bool operator==(const SFSequenceSet &s);

  virtual ~SFSequenceSet();

  // Append a sequence to the SFSequenceSet
  void AddSequence(Sequence *s);

  // Removes a sequence pointer. Returns true if the sequence was successfully
  // removed, false otherwise.
  bool RemoveSequence( Sequence *s ); 
  Sequence * Find(int j);

  // Number of sequences in the SFSequenceSet
  int NumSequences(void) const;

  //  return the entire sequence set in a const gbtArray
  const gbtBlock<Sequence *> &GetSFSequenceSet(void) const;
};


class SFSupport {
protected:
  const Sfg *bsfg;
  gbtArray <SFSequenceSet *> sups;
  
public:
  SFSupport(const Sfg &);
  SFSupport(const SFSupport &s); 
  virtual ~SFSupport();
  SFSupport &operator=(const SFSupport &s);

  bool operator==(const SFSupport &s) const;
  bool operator!=(const SFSupport &s) const;

  const Sfg &Game(void) const   { return *bsfg; }
  
  const gbtBlock<Sequence *> &Sequences(int pl) const;

  int NumSequences(int pl) const;
  const gbtArray<int> NumSequences(void) const;
  int TotalNumSequences(void) const;

  void AddSequence(Sequence *);
  bool RemoveSequence(Sequence *);
  
  bool IsSubset(const SFSupport &s) const;

  // returns the index of the sequence in the support if it exists,
  // otherwise returns zero
  int Find(Sequence *) const; 

  void Dump(gbtOutput &) const;
};

class SequenceProfile   {
  friend class Sfg;
private:
  long index;
  gbtArray<Sequence *> profile;
  
public:
  SequenceProfile(const Sfg &);
  SequenceProfile(const SequenceProfile &p);

  ~SequenceProfile();
  
  SequenceProfile &operator=(const SequenceProfile &);
  
  bool IsValid(void) const; 
  
  long GetIndex(void) const;
  
  Sequence *const operator[](int p) const;
  Sequence *const Get(int p) const;
  void Set(int p, Sequence  *const s);
};


gbtOutput &operator<<(gbtOutput &f, const Sequence &);
gbtOutput &operator<<(gbtOutput &f, const SFSupport &);

#endif    // SFSTRAT_H
