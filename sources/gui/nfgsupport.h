//
// $Source$
// $Revision$
// $Date$
//
// DESCRIPTION:
// Declaration of normal form support palette window
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

#ifndef NFGSUPPORT_H
#define NFGSUPPORT_H

#include "wx/treectrl.h"
#include "nfgshow.h"
#include "base/gmap.h"

class NfgSupportWindow : public wxPanel, public gbtGameView {
private:
  wxChoice *m_supportList;
  wxButton *m_prevButton, *m_nextButton;
  wxTreeCtrl *m_strategyTree;
  gbtOrdMap<wxTreeItemId, gbtNfgStrategy> m_map;

  void OnSupportList(wxCommandEvent &);
  void OnSupportPrev(wxCommandEvent &);
  void OnSupportNext(wxCommandEvent &);

  void OnTreeItemCollapse(wxTreeEvent &);

  // Overriding view members
  bool IsEfgView(void) const { return false; }
  bool IsNfgView(void) const { return true; }

public:
  NfgSupportWindow(gbtGameDocument *p_doc, wxWindow *p_parent);
  virtual ~NfgSupportWindow() { }

  int GetSupport(void) const { return m_supportList->GetSelection(); }
  void ToggleItem(wxTreeItemId);

  void OnUpdate(gbtGameView *);

  DECLARE_EVENT_TABLE()
};

class gbtNfgSupportFrame : public wxFrame, public gbtGameView {
private:
  NfgSupportWindow *m_panel;

  // Event handlers
  void OnClose(wxCloseEvent &);
  
  // Overriding view members
  bool IsEfgView(void) const { return false; }
  bool IsNfgView(void) const { return true; }
  void OnUpdate(gbtGameView *);

public:
  gbtNfgSupportFrame(gbtGameDocument *p_doc, wxWindow *p_parent);
  virtual ~gbtNfgSupportFrame();

  DECLARE_EVENT_TABLE()
};

#endif  // NFGSUPPORT_H
