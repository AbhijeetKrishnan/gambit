//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Implementation of API for outcomes
//
// This file is part of Gambit
// Copyright (c) 2003, The Gambit Project
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

//
// This file contains the implementation of the API for outcomes.
// Hopefully, the concept of "outcome" will be shared by extensive and
// normal forms very soon.
//

// Declaration of game API
#include "efg.h"
#include "nfg.h"

// Declaration of internal game classes
#include "efgint.h"
#include "nfgint.h"

//----------------------------------------------------------------------
//           struct gbt_efg_outcome_rep: Member functions
//----------------------------------------------------------------------

gbt_efg_outcome_rep::gbt_efg_outcome_rep(gbt_efg_game_rep *p_efg, int p_id)
  : m_id(p_id), m_efg(p_efg), m_deleted(false), 
    m_payoffs(p_efg->players.Length()),
    m_doublePayoffs(p_efg->players.Length()),
    m_refCount(0)
{
  for (int i = 1; i <= m_payoffs.Length(); i++) {
    m_payoffs[i] = 0;
    m_doublePayoffs[i] = 0.0;
  }
}

//----------------------------------------------------------------------
//               class gbtEfgOutcome: Member functions
//----------------------------------------------------------------------

gbtEfgOutcome::gbtEfgOutcome(void)
  : rep(0)
{ }

gbtEfgOutcome::gbtEfgOutcome(gbt_efg_outcome_rep *p_rep)
  : rep(p_rep)
{
  if (rep) {
    rep->m_refCount++;
    rep->m_efg->m_refCount++;
  }
}

gbtEfgOutcome::gbtEfgOutcome(const gbtEfgOutcome &p_outcome)
  : rep(p_outcome.rep)
{
  if (rep) {
    rep->m_refCount++;
    rep->m_efg->m_refCount++;
  }
}

gbtEfgOutcome::~gbtEfgOutcome()
{
  if (rep) {
    if (--rep->m_refCount == 0 && rep->m_deleted) {
      delete rep;
    }
    else if (--rep->m_efg->m_refCount == 0) {
      delete rep->m_efg;
    }
  }
}

gbtEfgOutcome &gbtEfgOutcome::operator=(const gbtEfgOutcome &p_outcome)
{
  if (this == &p_outcome) {
    return *this;
  }

  if (rep) {
    if (--rep->m_refCount == 0 && rep->m_deleted) {
      delete rep;
    }
    else if (--rep->m_efg->m_refCount == 0) {
      delete rep->m_efg;
    }
  }

  if ((rep = p_outcome.rep) != 0) {
    rep->m_refCount++;
    rep->m_efg->m_refCount++;
  }
  return *this;
}

bool gbtEfgOutcome::operator==(const gbtEfgOutcome &p_outcome) const
{
  return (rep == p_outcome.rep);
} 

bool gbtEfgOutcome::operator!=(const gbtEfgOutcome &p_outcome) const
{
  return (rep != p_outcome.rep);
} 

bool gbtEfgOutcome::IsNull(void) const
{
  if (rep->m_deleted) {
    throw gbtGameObjectDeleted();
  }
  else {
    return (rep == 0);
  }
}

bool gbtEfgOutcome::IsDeleted(void) const
{
  return (rep && rep->m_deleted);
}

gbtEfgGame gbtEfgOutcome::GetGame(void) const
{
  if (rep && rep->m_deleted) {
    throw gbtGameObjectDeleted();
  }
  else {
    return (rep) ? rep->m_efg : 0;
  }
}

gText gbtEfgOutcome::GetLabel(void) const
{
  if (rep && rep->m_deleted) {
    throw gbtGameObjectDeleted();
  }
  else if (rep) {
    return rep->m_label;
  }
  else {
    return "";
  }
}

void gbtEfgOutcome::SetLabel(const gText &p_label) 
{
  if (rep && rep->m_deleted) {
    throw gbtGameObjectDeleted();
  }
  else if (rep) {
    rep->m_label = p_label;
  }
}

gArray<gNumber> gbtEfgOutcome::GetPayoff(void) const
{
  if (rep && rep->m_deleted) {
    throw gbtGameObjectDeleted();
  }
  else if (!rep) {
    return gArray<gNumber>();
  }
  else {
    return rep->m_payoffs;
  }
}

gNumber gbtEfgOutcome::GetPayoff(const gbtEfgPlayer &p_player) const
{
  if (rep && rep->m_deleted) {
    throw gbtGameObjectDeleted();
  }
  else if (!rep || p_player.IsNull()) {
    return gNumber(0);
  }

  return rep->m_payoffs[p_player.GetId()];
}

void gbtEfgOutcome::SetPayoff(const gbtEfgPlayer &p_player,
			      const gNumber &p_value)
{
  if (rep && rep->m_deleted) {
    throw gbtGameObjectDeleted();
  }
  else if (!rep || p_player.IsNull()) {
    return;
  }

  rep->m_payoffs[p_player.GetId()] = p_value;
  rep->m_doublePayoffs[p_player.GetId()] = p_value;
  // FIXME: tell extensive form to update cached profile values!
}

void gbtEfgOutcome::DeleteOutcome(void)
{
  if (rep && rep->m_deleted) {
    throw gbtGameObjectDeleted();
  }
  else if (rep) {
    rep->m_efg->DeleteOutcome(rep);
  }
}

gOutput &operator<<(gOutput &p_stream, const gbtEfgOutcome &)
{ 
  return p_stream;
}

//----------------------------------------------------------------------
//                gbt_nfg_outcome_rep: Declaration
//----------------------------------------------------------------------

gbt_nfg_outcome_rep::gbt_nfg_outcome_rep(gbt_nfg_game_rep *p_nfg, int p_id)
  : m_id(p_id), m_nfg(p_nfg), m_deleted(false), 
    m_payoffs(p_nfg->m_players.Length()),
    m_doublePayoffs(p_nfg->m_players.Length()),
    m_refCount(0)
{
  for (int i = 1; i <= m_payoffs.Length(); i++) {
    m_payoffs[i] = 0;
    m_doublePayoffs[i] = 0.0;
  }
}

gbtNfgOutcome::gbtNfgOutcome(void)
  : rep(0)
{ }

gbtNfgOutcome::gbtNfgOutcome(gbt_nfg_outcome_rep *p_rep)
  : rep(p_rep)
{
  if (rep) {
    rep->m_refCount++;
    rep->m_nfg->m_refCount++;
  }
}

gbtNfgOutcome::gbtNfgOutcome(const gbtNfgOutcome &p_outcome)
  : rep(p_outcome.rep)
{
  if (rep) {
    rep->m_refCount++;
    rep->m_nfg->m_refCount++;
  }
}

gbtNfgOutcome::~gbtNfgOutcome()
{
  if (rep) {
    if (--rep->m_refCount == 0 && rep->m_deleted) {
      delete rep;
    }
    else if (--rep->m_nfg->m_refCount == 0) {
      delete rep->m_nfg;
    }
  }
}

gbtNfgOutcome &gbtNfgOutcome::operator=(const gbtNfgOutcome &p_outcome)
{
  if (this == &p_outcome) {
    return *this;
  }

  if (rep) {
    if (--rep->m_refCount == 0 && rep->m_deleted) {
      delete rep;
    }
    else if (--rep->m_nfg->m_refCount == 0) {
      delete rep->m_nfg;
    }
  }

  if ((rep = p_outcome.rep) != 0) {
    rep->m_refCount++;
    rep->m_nfg->m_refCount++;
  }
  return *this;
}

bool gbtNfgOutcome::operator==(const gbtNfgOutcome &p_outcome) const
{
  return (rep == p_outcome.rep);
} 

bool gbtNfgOutcome::operator!=(const gbtNfgOutcome &p_outcome) const
{
  return (rep != p_outcome.rep);
} 

int gbtNfgOutcome::GetId(void) const
{
  return (rep) ? rep->m_id : 0;
}

bool gbtNfgOutcome::IsNull(void) const
{
  if (rep->m_deleted) {
    throw gbtGameObjectDeleted();
  }
  else {
    return (rep == 0);
  }
}

bool gbtNfgOutcome::IsDeleted(void) const
{
  return (rep && rep->m_deleted);
}

gbtNfgGame gbtNfgOutcome::GetGame(void) const
{
  if (rep && rep->m_deleted) {
    throw gbtGameObjectDeleted();
  }
  else {
    return (rep) ? rep->m_nfg : 0;
  }
}

gText gbtNfgOutcome::GetLabel(void) const
{
  if (rep && rep->m_deleted) {
    throw gbtGameObjectDeleted();
  }
  else if (rep) {
    return rep->m_label;
  }
  else {
    return "";
  }
}

void gbtNfgOutcome::SetLabel(const gText &p_label)
{
  if (rep && rep->m_deleted) {
    throw gbtGameObjectDeleted();
  }
  else if (rep) {
    rep->m_label = p_label;
  }
}

gNumber gbtNfgOutcome::GetPayoff(const gbtNfgPlayer &p_player) const
{
  if (rep && rep->m_deleted) {
    throw gbtGameObjectDeleted();
  }
  else if (rep && p_player.rep) {
    return rep->m_payoffs[p_player.rep->m_id];
  }
  else {
    return 0;
  }
}

gArray<gNumber> gbtNfgOutcome::GetPayoff(void) const
{
  if (rep && rep->m_deleted) {
    throw gbtGameObjectDeleted();
  }
  else if (rep) {
    return rep->m_payoffs;
  }
  else {
    return gArray<gNumber>();
  }
}

double gbtNfgOutcome::GetPayoffDouble(int p_player) const
{
  if (rep && rep->m_deleted) {
    throw gbtGameObjectDeleted();
  }
  else if (rep) {
    return rep->m_payoffs[p_player];
  }
  else {
    return 0;
  }
}

void gbtNfgOutcome::SetPayoff(const gbtNfgPlayer &p_player,
			      const gNumber &p_value)
{
  if (rep && rep->m_deleted) {
    throw gbtGameObjectDeleted();
  }
  else if (rep && p_player.rep) {
    rep->m_payoffs[p_player.rep->m_id] = p_value;
    rep->m_doublePayoffs[p_player.rep->m_id] = (double) p_value;
    // FIXME: tell game to update cached values
  }
}

void gbtNfgOutcome::DeleteOutcome(void)
{
  if (rep && rep->m_deleted) {
    throw gbtGameObjectDeleted();
  }
  else if (rep) {
    rep->m_nfg->DeleteOutcome(rep);
  }
}

gOutput &operator<<(gOutput &p_stream, const gbtNfgOutcome &)
{ 
  return p_stream;
}

