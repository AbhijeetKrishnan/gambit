//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Dialog for editing properties of behavior profiles
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

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif  // WX_PRECOMP
#include "nash/behavsol.h"
#include "dleditbehav.h"
#include "numberedit.h"

//-------------------------------------------------------------------------
//                 class dialogEditBehav: Member functions
//-------------------------------------------------------------------------

const int GBT_INFOSET_TREE = 2001;
const int GBT_PROB_GRID = 2002;

BEGIN_EVENT_TABLE(dialogEditBehav, wxDialog)
  EVT_TREE_ITEM_COLLAPSING(GBT_INFOSET_TREE, dialogEditBehav::OnItemCollapsing)
  EVT_TREE_SEL_CHANGING(GBT_INFOSET_TREE, dialogEditBehav::OnSelChanging)
  EVT_TREE_SEL_CHANGED(GBT_INFOSET_TREE, dialogEditBehav::OnSelChanged)
  EVT_BUTTON(wxID_OK, dialogEditBehav::OnOK)
END_EVENT_TABLE()

dialogEditBehav::dialogEditBehav(wxWindow *p_parent,
				 const BehavSolution &p_profile)
  : wxDialog(p_parent, -1, _("Behavior profile properties"),
	     wxDefaultPosition),
    m_profile(p_profile), m_lastInfoset(0), m_map(gbtEfgInfoset())
{
  SetAutoLayout(true);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  nameSizer->Add(new wxStaticText(this, wxID_STATIC, _("Profile name")),
		 0, wxALL, 5);
  m_profileName = new wxTextCtrl(this, -1, 
				 wxString::Format(wxT("%s"),
						  (char *) p_profile.GetLabel()));
  nameSizer->Add(m_profileName, 1, wxALL | wxEXPAND, 5);
  topSizer->Add(nameSizer, 0, wxALL | wxEXPAND, 5);

  wxBoxSizer *editSizer = new wxBoxSizer(wxHORIZONTAL);
  m_infosetTree = new wxTreeCtrl(this, GBT_INFOSET_TREE,
				 wxDefaultPosition, wxSize(200, 200));
  m_infosetTree->AddRoot(wxString::Format(wxT("%s"),
					  (char *) p_profile.GetLabel()));

  for (int pl = 1; ; pl++) {
    if (p_profile.GetGame().GetPlayer(pl)->NumInfosets() > 0) {
      m_lastInfoset = p_profile.GetGame().GetPlayer(pl)->GetInfoset(1);
      break;
    }
  }
  wxTreeItemId firstID;

  for (int pl = 1; pl <= p_profile.GetGame().NumPlayers(); pl++) {
    gbtEfgPlayer player = p_profile.GetGame().GetPlayer(pl);
    wxTreeItemId id;
    if (player->GetLabel() != "") {
      id = m_infosetTree->AppendItem(m_infosetTree->GetRootItem(),
				     wxString::Format(wxT("%s"),
						      (char *) player->GetLabel()));
    }
    else {
      id = m_infosetTree->AppendItem(m_infosetTree->GetRootItem(),
				     wxString::Format(wxT("Player %d"), pl));
    }
    m_infosetTree->SetItemBold(id, true);
    for (int iset = 1; iset <= player->NumInfosets(); iset++) {
      gbtEfgInfoset infoset = player->GetInfoset(iset);
      wxTreeItemId isetID;
      if (infoset.GetLabel() != "") {
	isetID = m_infosetTree->AppendItem(id,
					   wxString::Format(wxT("%s"),
							    (char *) infoset.GetLabel()));
      }
      else {
	isetID = m_infosetTree->AppendItem(id,
					   wxString::Format(wxT("(%d,%d)"),
							    pl, iset));
      }
      m_map.Define(isetID, player->GetInfoset(iset));
      if (player->GetInfoset(iset) == m_lastInfoset) {
	firstID = isetID;
      }
    }
    m_infosetTree->Expand(id);
  }
  m_infosetTree->Expand(m_infosetTree->GetRootItem());
  editSizer->Add(m_infosetTree, 1, wxALL | wxEXPAND, 5);

  m_probGrid = new wxGrid(this, GBT_PROB_GRID,
			  wxDefaultPosition, wxDefaultSize);
  m_probGrid->CreateGrid(m_lastInfoset.NumActions(), 1);
  m_probGrid->SetDefaultCellAlignment(wxALIGN_CENTER, wxALIGN_CENTER);
  m_probGrid->SetDefaultEditor(new gbtNumberEditor);
  m_probGrid->SetLabelValue(wxHORIZONTAL, _("Probability"), 0);
  for (int act = 1; act <= m_lastInfoset.NumActions(); act++) {
    m_probGrid->SetLabelValue(wxVERTICAL,
			      wxString::Format(wxT("%s"),
					       (char *) m_lastInfoset.GetAction(act)->GetLabel()),
			      act - 1);
    m_probGrid->SetCellValue(wxString::Format(wxT("%s"),
					      (char *) ToText(p_profile(m_lastInfoset.GetAction(act)))),
			     act - 1, 0);
    if (act % 2 == 0) {
      m_probGrid->SetCellBackgroundColour(act - 1, 0, wxColour(200, 200, 200));
    }
    else {
      m_probGrid->SetCellBackgroundColour(act - 1, 0, wxColour(225, 225, 225));
    }
  }
  m_probGrid->SetMargins(0, 0);
  m_probGrid->SetSize(wxSize(m_probGrid->GetRowLabelSize() + 
			     m_probGrid->GetColSize(0), 200));
  editSizer->Add(m_probGrid, 0, wxALL, 5);
  topSizer->Add(editSizer, 1, wxEXPAND | wxALL, 5);

  wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
  wxButton *okButton = new wxButton(this, wxID_OK, _("OK"));
  okButton->SetDefault();
  buttonSizer->Add(okButton, 0, wxALL, 5);
  buttonSizer->Add(new wxButton(this, wxID_CANCEL, _("Cancel")), 0, wxALL, 5);
  //  buttonSizer->Add(new wxButton(this, wxID_HELP, _("Help")), 0, wxALL, 5);
  topSizer->Add(buttonSizer, 0, wxCENTER | wxALL, 5);

  // We wait and do this until the end, since this emits a
  // selection event, and the grid must be set up properly before
  // that happens
  m_infosetTree->SelectItem(firstID);
  m_probGrid->AdjustScrollbars();

  SetSizer(topSizer);
  topSizer->Fit(this);
  topSizer->SetSizeHints(this);
  Layout();
  CenterOnParent();
}

dialogEditBehav::~dialogEditBehav()
{ }

void dialogEditBehav::OnItemCollapsing(wxTreeEvent &p_event)
{
  if (p_event.GetItem() == m_infosetTree->GetRootItem()) {
    p_event.Veto();
  }
}

void dialogEditBehav::OnSelChanging(wxTreeEvent &p_event)
{
  if (!m_map.IsDefined(p_event.GetItem())) {
    p_event.Veto();
  }
}

void dialogEditBehav::OnSelChanged(wxTreeEvent &p_event)
{
  if (m_probGrid->IsCellEditControlEnabled()) {
    m_probGrid->SaveEditControlValue();
    m_probGrid->HideCellEditControl();
  }

  if (!m_lastInfoset.IsNull()) {
    for (int act = 1; act <= m_lastInfoset.NumActions(); act++) {
      m_profile.SetActionProb(m_lastInfoset.GetAction(act),
			      ToNumber(gbtText(m_probGrid->GetCellValue(act - 1, 0).mb_str())));
    }
  }

  m_lastInfoset = m_map.Lookup(p_event.GetItem());

  if (!m_lastInfoset.IsNull()) {
    if (m_lastInfoset.NumActions() < m_probGrid->GetRows()) {
      m_probGrid->DeleteRows(0,
			     m_probGrid->GetRows() - m_lastInfoset.NumActions());
    }
    else if (m_lastInfoset.NumActions() > m_probGrid->GetRows()) {
      m_probGrid->InsertRows(0,
			     m_lastInfoset.NumActions() - m_probGrid->GetRows());
    }

    for (int act = 1; act <= m_lastInfoset.NumActions(); act++) {
      m_probGrid->SetLabelValue(wxVERTICAL,
				wxString::Format(wxT("%s"),
						 (char *) m_lastInfoset.GetAction(act)->GetLabel()),
				act - 1);
      m_probGrid->SetCellValue(wxString::Format(wxT("%s"),
						(char *) ToText(m_profile(m_lastInfoset.GetAction(act)))),
			       act - 1, 0);
      if (act % 2 == 0) {
	m_probGrid->SetCellBackgroundColour(act - 1, 0, wxColour(200, 200, 200));
      }
      else {
	m_probGrid->SetCellBackgroundColour(act - 1, 0, wxColour(225, 225, 225));
      }
    }
  }
  else {
    m_probGrid->DeleteRows(m_probGrid->GetRows());
  }
}

void dialogEditBehav::OnOK(wxCommandEvent &p_event)
{
  if (m_probGrid->IsCellEditControlEnabled()) {
    m_probGrid->SaveEditControlValue();
    m_probGrid->DisableCellEditControl();
  }

  gbtEfgInfoset infoset = m_map.Lookup(m_infosetTree->GetSelection());

  if (infoset.IsNull()) {
    for (int pl = 1; ; pl++) {
      if (m_profile.GetGame().GetPlayer(pl)->NumInfosets() > 0) {
	m_lastInfoset = m_profile.GetGame().GetPlayer(pl)->GetInfoset(1);
	break;
      }
    }
  }

  for (int act = 1; act <= infoset.NumActions(); act++) {
    m_profile.SetActionProb(infoset.GetAction(act),
			    ToNumber(gbtText(m_probGrid->GetCellValue(act - 1, 0).mb_str())));
  }

  p_event.Skip();
}

const BehavSolution &dialogEditBehav::GetProfile(void) const
{
  m_profile.SetLabel(gbtText(m_profileName->GetValue().mb_str()));
  return m_profile;
}

#include "base/gmap.imp"

gbtOutput &operator<<(gbtOutput &p_output, wxTreeItemId)
{ return p_output; }

template class gbtBaseMap<wxTreeItemId, gbtEfgInfoset>;
template class gbtOrdMap<wxTreeItemId, gbtEfgInfoset>;
