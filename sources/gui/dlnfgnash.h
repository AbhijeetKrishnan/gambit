//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Dialog for selecting algorithms to compute Nash equilibria
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

#ifndef DLNFGNASH_H
#define DLNFGNASH_H

#include "wx/treectrl.h"
#include "base/gmap.h"
#include "nash/nfgalgorithm.h"

class gbtPanelNfgNashAlgorithm;

class dialogNfgNash : public wxDialog {
private:
  wxTreeCtrl *m_algorithmTree;
  wxPanel *m_currentPanel;
  wxBoxSizer *m_algPanelSizer;
  gbtOrdMap<wxTreeItemId, gbtPanelNfgNashAlgorithm *> m_algorithms;

  // Private auxiliary functions
  int LoadAlgorithms(const gbtNfgGame &);

  // Event handlers
  void OnSelectionChanging(wxTreeEvent &);
  void OnItemCollapsing(wxTreeEvent &);

public:
  // Lifecycle
  dialogNfgNash(wxWindow *, const gbtNfgGame &);

  // Data access (only valid when ShowModal() returns with wxID_OK)
  gbtNfgNashAlgorithm *GetAlgorithm(void) const;

  DECLARE_EVENT_TABLE()
};



#endif   // DLNFGNASH_H
