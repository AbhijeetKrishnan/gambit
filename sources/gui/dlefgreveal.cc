//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Dialog for revealing actions to players
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

#include "game/game.h"
#include "dlefgreveal.h"


//=========================================================================
//                  dialogInfosetReveal: Member functions
//=========================================================================

dialogInfosetReveal::dialogInfosetReveal(wxWindow *p_parent,
					 const gbtGame &p_efg)
  : wxDialog(p_parent, -1, _("Reveal move"), wxDefaultPosition), m_efg(p_efg)
{
  SetAutoLayout(true);

  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

  wxStaticBoxSizer *playerBox =
    new wxStaticBoxSizer(new wxStaticBox(this, wxID_STATIC,
					 _("Reveal to players")),
			 wxHORIZONTAL);

  wxFlexGridSizer *gridSizer = new wxFlexGridSizer(wxVERTICAL,
						   (m_efg->NumPlayers() <= 2) ?
						   2 : 3);
  m_players = new wxCheckBox *[m_efg->NumPlayers()];
  for (int pl = 1; pl <= m_efg->NumPlayers(); pl++) {
    gbtGamePlayer player = m_efg->GetPlayer(pl);
    if (player->GetLabel() != "") {
      m_players[pl-1] = new wxCheckBox(this, -1,
				       wxString::Format(wxT("%s"),
							(char *) player->GetLabel()));
    }
    else {
      m_players[pl-1] = new wxCheckBox(this, -1, 
				       wxString::Format(_("Player %d"), pl));
    }
    m_players[pl-1]->SetValue(1);
    gridSizer->Add(m_players[pl-1], 1, wxALL | wxEXPAND, 0);
  }
  playerBox->Add(gridSizer, 1, wxALL, 5);
  topSizer->Add(playerBox, 1, wxALL | wxCENTER, 5);

  wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
  wxButton *okButton = new wxButton(this, wxID_OK, _("OK"));
  okButton->SetDefault();
  buttonSizer->Add(okButton, 0, wxALL, 5);
  buttonSizer->Add(new wxButton(this, wxID_CANCEL, _("Cancel")), 0, wxALL, 5);
  //  buttonSizer->Add(new wxButton(this, wxID_HELP, _("Help")), 0, wxALL, 5);
  topSizer->Add(buttonSizer, 0, wxCENTER | wxALL, 5);

  SetSizer(topSizer);
  topSizer->Fit(this);
  topSizer->SetSizeHints(this);
  Layout();
  CenterOnParent();
}

dialogInfosetReveal::~dialogInfosetReveal()
{
  delete [] m_players;
}

bool dialogInfosetReveal::IsPlayerSelected(int p_index) const
{
  return m_players[p_index-1]->GetValue();
}
