//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Dialog for viewing and editing properties of a node
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

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif  // WX_PRECOMP
#include "game/efg.h"
#include "dleditnode.h"

//======================================================================
//                      class dialogEditNode
//======================================================================

dialogEditNode::dialogEditNode(wxWindow *p_parent, gbtEfgNode p_node)
  : wxDialog(p_parent, -1, _("Node properties"), wxDefaultPosition), 
    m_node(p_node)
{
  SetAutoLayout(true);

  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer *labelSizer = new wxBoxSizer(wxHORIZONTAL);
  labelSizer->Add(new wxStaticText(this, wxID_STATIC, _("Node label")),
		  0, wxALL | wxCENTER, 5);
  m_nodeName = new wxTextCtrl(this, -1,
			      wxString::Format(wxT("%s"),
					       (const char *) m_node.GetLabel()));
  labelSizer->Add(m_nodeName, 1, wxALL | wxCENTER | wxEXPAND, 5);
  topSizer->Add(labelSizer, 0, wxALL | wxEXPAND, 5);

  wxBoxSizer *infosetSizer = new wxBoxSizer(wxHORIZONTAL);
  infosetSizer->Add(new wxStaticText(this, wxID_STATIC, _("Information set")),
		    0, wxALL | wxCENTER, 5);
  m_infoset = new wxChoice(this, -1);
  if (p_node.NumChildren() > 0) {
    m_infoset->Append(_("New information set"));
    int selection = 0;
    for (int pl = 1; pl <= p_node.GetGame().NumPlayers(); pl++) {
      for (gbtEfgInfosetIterator infoset(p_node.GetGame().GetPlayer(pl));
	   !infoset.End(); infoset++) {
	if (!(*infoset).IsChanceInfoset() &&
	    (*infoset).NumActions() == p_node.NumChildren()) {
	  m_infosetList.Append(*infoset);
	  m_infoset->Append(wxString::Format(wxT("Player %d, Infoset %d"),
					     (*infoset).GetPlayer().GetId(),
					     (*infoset).GetId()));
	  if (*infoset == p_node.GetInfoset()) {
	    selection = m_infoset->GetCount() - 1;
	  }
	}
      }
    }
    m_infoset->SetSelection(selection);
  }
  else {
    m_infoset->Append(_("(none)"));
    m_infoset->SetSelection(0);
    m_infoset->Enable(false);
  }
  infosetSizer->Add(m_infoset, 1, wxALL | wxEXPAND, 5);
  topSizer->Add(infosetSizer, 0, wxALL | wxEXPAND, 5);

  wxBoxSizer *subgameSizer = new wxBoxSizer(wxVERTICAL);
  if (p_node.GetParent().IsNull()) {
    subgameSizer->Add(new wxStaticText(this, wxID_STATIC,
				       _("This is the root node of the tree")),
		      0, wxALL | wxCENTER, 5);
  }
  else if (p_node.IsSubgameRoot()) {
    subgameSizer->Add(new wxStaticText(this, wxID_STATIC,
				       _("This is the root of a proper subgame")),
		      0, wxALL | wxCENTER, 5);
    m_markedSubgame = new wxCheckBox(this, -1, _("Subgame is marked"));
    m_markedSubgame->SetValue(p_node.GetSubgameRoot() == p_node);
    subgameSizer->Add(m_markedSubgame, 0, wxALL | wxCENTER, 0);
  }
  topSizer->Add(subgameSizer, 0, wxALL | wxCENTER, 5);

  wxBoxSizer *outcomeSizer = new wxBoxSizer(wxHORIZONTAL);
  outcomeSizer->Add(new wxStaticText(this, wxID_STATIC, _("Outcome")),
		    0, wxALL | wxCENTER, 5);
  m_outcome = new wxChoice(this, -1);
  m_outcome->Append(_("(null)"));
  m_outcome->SetSelection(0);
  gbtEfgGame efg = p_node.GetGame();
  for (int outc = 1; outc <= efg.NumOutcomes(); outc++) {
    gbtEfgOutcome outcome = efg.GetOutcome(outc);
    gText item = ToText(outc) + ": " + outcome.GetLabel();
    if (item == "") {
      item = gText("Outcome") + ToText(outc);
    }

    item += (" (" + ToText(outcome.GetPayoff(efg.GetPlayer(1))) + ", " +
	     ToText(outcome.GetPayoff(efg.GetPlayer(2))));
    if (efg.NumPlayers() > 2) {
      item += ", " + ToText(outcome.GetPayoff(efg.GetPlayer(3)));
      if (efg.NumPlayers() > 3) {
	item += ",...)";
      }
      else {
	item += ")";
      }
    }
    else {
      item += ")";
    }

    m_outcome->Append(wxString::Format(wxT("%s"), (const char *) item));
    if (m_node.GetOutcome() == outcome) {
      m_outcome->SetSelection(outc);
    }
  }
  outcomeSizer->Add(m_outcome, 1, wxALL | wxEXPAND, 5);
  topSizer->Add(outcomeSizer, 0, wxALL | wxEXPAND, 5);

  wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
  wxButton *okButton = new wxButton(this, wxID_OK, _("OK"));
  okButton->SetDefault();
  buttonSizer->Add(okButton, 0, wxALL, 5);
  buttonSizer->Add(new wxButton(this, wxID_CANCEL, _("Cancel")), 0, wxALL, 5);
  //  buttonSizer->Add(new wxButton(this, wxID_HELP, _("Help")), 0, wxALL, 5);
  topSizer->Add(buttonSizer, 0, wxALL | wxCENTER, 5);

  SetSizer(topSizer);
  topSizer->Fit(this);
  topSizer->SetSizeHints(this);

  Layout();
  CenterOnParent();
}

gbtEfgInfoset dialogEditNode::GetInfoset(void) const
{
  if (m_infoset->GetSelection() == 0) {
    return 0;
  }
  else {
    return m_infosetList[m_infoset->GetSelection()];
  }
}
