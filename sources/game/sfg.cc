//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Implementation of sequence form classes
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

#include "sfg.h"
#include "sfstrat.h"
#include "base/garray.imp"
#include "game/gnarray.imp"
#include "base/grarray.imp"
#include "base/glist.imp"
#include "game/behav.h"

//----------------------------------------------------
// Sfg: Constructors, Destructors, Operators
//----------------------------------------------------


Sfg::Sfg(const gbtEfgSupport &S)
  : m_efg(S.GetGame()), efsupp(S), seq(m_efg.NumPlayers()),
    isetFlag(S.GetGame().NumInfosets()),
    isetRow(S.GetGame().NumInfosets()), infosets(m_efg.NumPlayers())
{ 
  int i;
  gArray<gbtEfgInfoset> zero(m_efg.NumPlayers());
  gArray<int> one(m_efg.NumPlayers());

  gbtEfgSupport support(m_efg);

  for(i=1;i<=m_efg.NumPlayers();i++) {
    seq[i]=1;
    zero[i]=0;
    one[i]=1;
  }

  isetFlag = 0;
  isetRow = 0;

  GetSequenceDims(m_efg.GetRoot());

  isetFlag = 0;

  gIndexOdometer index(seq);

  SF = new gNArray<gArray<gNumber> *>(seq);
  while (index.Turn()) {
    (*SF)[index.CurrentIndices()] = new gArray<gNumber>(m_efg.NumPlayers());
    for(i=1;i<=m_efg.NumPlayers();i++)
      (*(*SF)[index.CurrentIndices()])[i]=(gNumber)0;
  } 

  E = new gArray<gRectArray<gNumber> *> (m_efg.NumPlayers());
  for(i=1;i<=m_efg.NumPlayers();i++) {
    (*E)[i] = new gRectArray<gNumber>(infosets[i].Length()+1,seq[i]);
    for(int j = (*(*E)[i]).MinRow();j<=(*(*E)[i]).MaxRow();j++)
      for(int k = (*(*E)[i]).MinCol();k<=(*(*E)[i]).MaxCol();k++)
	(*(*E)[i])(j,k)=(gNumber)0;
    (*(*E)[i])(1,1)=(gNumber)1;
  } 

  sequences = new gArray<SFSequenceSet *>(m_efg.NumPlayers());
  for (i=1;i<=m_efg.NumPlayers();i++) {
    (*sequences)[i] = new SFSequenceSet( m_efg.GetPlayer(i) );
  }

  gArray<Sequence *> parent(m_efg.NumPlayers());
  for(i=1;i<=m_efg.NumPlayers();i++)
    parent[i] = (((*sequences)[i])->GetSFSequenceSet())[1];

  MakeSequenceForm(m_efg.GetRoot(),(gNumber)1,one,zero,parent);
}

Sfg::~Sfg()
{
  gIndexOdometer index(seq);

  while (index.Turn()) 
    delete (*SF)[index.CurrentIndices()];
  delete SF;

  int i;

  for(i=1;i<=m_efg.NumPlayers();i++)
    delete (*E)[i];
  delete E;

  for(i=1;i<=m_efg.NumPlayers();i++)
    delete (*sequences)[i];
  delete sequences;
}

void 
Sfg::MakeSequenceForm(const gbtEfgNode &n, gNumber prob,gArray<int>seq, 
		      gArray<gbtEfgInfoset> iset, gArray<Sequence *> parent) 
{ 
  int i,pl;

  if (!n.GetOutcome().IsNull()) {
    for(pl = 1;pl<=seq.Length();pl++)
      (*(*SF)[seq])[pl] += prob * n.GetOutcome().GetPayoff(m_efg.GetPlayer(pl));
  }
  if (!n.GetInfoset().IsNull()) {
    if (n.GetPlayer().IsChance()) {
      for(i=1;i<=n.NumChildren();i++)
	MakeSequenceForm(n.GetChild(i),
		     prob * n.GetInfoset().GetChanceProb(i), seq,iset,parent);
    }
    else {
      int pl = n.GetPlayer().GetId();
      iset[pl]=n.GetInfoset();
      int isetnum = iset[pl].GetId();
      gArray<int> snew(seq);
      snew[pl]=1;
      for(i=1;i<isetnum;i++)
	if(isetRow(pl,i)) 
	  snew[pl]+=efsupp.NumActions(pl,i);

      (*(*E)[pl])(isetRow(pl,isetnum),seq[pl]) = (gNumber)1;
      Sequence *myparent(parent[pl]);

      bool flag = false;
      if(!isetFlag(pl,isetnum)) {   // on first visit to iset, create new sequences
	isetFlag(pl,isetnum)=1;
	flag =true;
      }
      for(i=1;i<=n.NumChildren();i++) {
	if(efsupp.Contains(n.GetInfoset().GetAction(i))) {
	  snew[pl]+=1;
	  if(flag) {
	    Sequence* child;
	    child = new Sequence(n.GetPlayer(), n.GetInfoset().GetAction(i), 
				 myparent,snew[pl]);
	    parent[pl]=child;
	    ((*sequences)[pl])->AddSequence(child);
	    
	  }

	  (*(*E)[pl])(isetRow(pl,isetnum),snew[pl]) = -(gNumber)1;
	  MakeSequenceForm(n.GetChild(i),prob,snew,iset,parent);
	}
      }
    }
    
  }
}

void Sfg::GetSequenceDims(const gbtEfgNode &n) 
{ 
  int i;

  if (!n.GetInfoset().IsNull()) {
    if (n.GetPlayer().IsChance()) {
      for(i=1;i<=n.NumChildren();i++)
	GetSequenceDims(n.GetChild(i));
    }
    else {
      int pl = n.GetPlayer().GetId();
      int isetnum = n.GetInfoset().GetId();
    
      bool flag = false;
      if(!isetFlag(pl,isetnum)) {   // on first visit to iset, create new sequences
	infosets[pl].Append(n.GetInfoset());
	isetFlag(pl,isetnum)=1;
	isetRow(pl,isetnum)=infosets[pl].Length()+1;
	flag =true;
      }
      for(i=1;i<=n.NumChildren();i++) {
	if(efsupp.Contains(n.GetInfoset().GetAction(i))) {
	  if(flag) {
	    seq[pl]++;
	  }
	  GetSequenceDims(n.GetChild(i));
	}
      }
    }
  }
}

void Sfg::Dump(gOutput& out) const
{
  gIndexOdometer index(seq);

  out << "\nseq: " << seq;

  out << "\nSequence Form: \n";
  while (index.Turn()) {
    out << "\nrow " << index.CurrentIndices() << ": " << (*(*SF)[index.CurrentIndices()]);
  } 
  
  out << "\nConstraint matrices: \n";
  for(int i=1;i<=m_efg.NumPlayers();i++) 
    out << "\nPlayer " << i << ":\n " << (*(*E)[i]);
}

int Sfg::TotalNumSequences() const 
{
  int tot=0;
  for(int i=1;i<=seq.Length();i++)
    tot+=seq[i];
  return tot;
}

int Sfg::NumPlayerInfosets() const 
{
  int tot=0;
  for(int i=1;i<=infosets.Length();i++)
    tot+=infosets[i].Length();
  return tot;
}

int Sfg::InfosetRowNumber(int pl, int j) const 
{
  if(j==1) return 0;
  int isetnum = (*sequences)[pl]->Find(j)->GetInfoset().GetId();
  return isetRow(pl,isetnum);
}

int Sfg::ActionNumber(int pl, int j) const
{
  if(j==1) return 0;
  return efsupp.GetIndex(GetAction(pl,j));
}

gbtEfgInfoset Sfg::GetInfoset(int pl, int j) const 
{
  if(j==1) return 0;
  return (*sequences)[pl]->Find(j)->GetInfoset();
}

gbtEfgAction Sfg::GetAction(int pl, int j) const
{
  if(j==1) return 0;
  return (*sequences)[pl]->Find(j)->GetAction();
}

BehavProfile<gNumber> Sfg::ToBehav(const gPVector<double> &x) const
{
  BehavProfile<gNumber> b(efsupp);

  b = (gNumber) 0;

  Sequence *sij;
  const Sequence *parent;
  gNumber value;

  int i,j;
  for(i=1;i<=m_efg.NumPlayers();i++)
    for(j=2;j<=seq[i];j++) {
      sij = ((*sequences)[i]->GetSFSequenceSet())[j];
      int sn = sij->GetNumber();
      parent = sij->Parent();

      // gout << "\ni,j,sn,iset,act: " << i << " " << j << " " << sn << " ";
      // gout << sij->GetInfoset()->GetNumber() << " " << sij->GetAction()->GetNumber();

      if(x(i, parent->GetNumber())>(double)0)
	value = (gNumber)(x(i,sn)/x(i,parent->GetNumber()));
      else
	value = (gNumber)0;

      b(i,sij->GetInfoset().GetId(),efsupp.GetIndex(sij->GetAction()))= value;
    }
  return b;
}

gNumber Sfg::Payoff(const gArray<int> & index,int pl) const 
{
  return Payoffs(index)[pl];
}


template class gNArray<gArray<gNumber> *>;
template class gArray<gRectArray<gNumber> *>;
#ifndef __BCC55__
template gOutput &operator<<(gOutput &, const gArray<gNumber> &);
#endif // __BCC55__
template class gArray<gList<gbtEfgInfoset> >;
template gOutput &operator<<(gOutput &, const gArray<gList<gbtEfgInfoset> > &);
#ifndef __BCC55__
template gOutput &operator<<(gOutput &, const gList<gbtEfgInfoset> &);
#endif // __BCC55__
