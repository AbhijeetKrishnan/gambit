//
// $Source$
// $Revision$
// $Date$
//
// DESCRIPTION:
// Implementation of normal form support palette window
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
#include "nfgsupport.h"

const int idSTRATEGYTREE = 8003;

class widgetStrategyTree : public wxTreeCtrl {
private:
  NfgSupportWindow *m_parent;
  wxMenu *m_menu;

  void OnRightClick(wxTreeEvent &);
  void OnMiddleClick(wxTreeEvent &);
  void OnKeypress(wxTreeEvent &);

public:
  widgetStrategyTree(NfgSupportWindow *p_parent);

  DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(widgetStrategyTree, wxTreeCtrl)
  EVT_TREE_KEY_DOWN(idSTRATEGYTREE, widgetStrategyTree::OnKeypress)
  EVT_TREE_ITEM_MIDDLE_CLICK(idSTRATEGYTREE, widgetStrategyTree::OnMiddleClick)
  EVT_TREE_ITEM_RIGHT_CLICK(idSTRATEGYTREE, widgetStrategyTree::OnRightClick)
END_EVENT_TABLE()

widgetStrategyTree::widgetStrategyTree(NfgSupportWindow *p_parent)
  : wxTreeCtrl(p_parent, idSTRATEGYTREE), m_parent(p_parent)
{ 
  m_menu = new wxMenu;
  m_menu->Append(GBT_NFG_MENU_SUPPORT_DUPLICATE, "Duplicate support",
		 "Duplicate this support");
  m_menu->Append(GBT_NFG_MENU_SUPPORT_DELETE, "Delete support",
		 "Delete this support");
}

void widgetStrategyTree::OnRightClick(wxTreeEvent &p_event)
{
  // Cannot delete the "full support"
  m_menu->Enable(GBT_NFG_MENU_SUPPORT_DELETE, (m_parent->GetSupport() > 0));
  PopupMenu(m_menu, p_event.GetPoint());
}

void widgetStrategyTree::OnKeypress(wxTreeEvent &p_event)
{
  if (m_parent->GetSupport() == 0) {
    return;
  }
  if (p_event.GetCode() == WXK_SPACE) {
    m_parent->ToggleItem(GetSelection());
  }
}

void widgetStrategyTree::OnMiddleClick(wxTreeEvent &p_event)
{
  if (m_parent->GetSupport() == 0) {
    return;
  }
  m_parent->ToggleItem(p_event.GetItem());
}



const int idSUPPORTLISTCHOICE = 8000;
const int idSUPPORTPREVBUTTON = 8001;
const int idSUPPORTNEXTBUTTON = 8002;

BEGIN_EVENT_TABLE(NfgSupportWindow, wxPanel)
  EVT_CHOICE(idSUPPORTLISTCHOICE, NfgSupportWindow::OnSupportList)
  EVT_BUTTON(idSUPPORTPREVBUTTON, NfgSupportWindow::OnSupportPrev)
  EVT_BUTTON(idSUPPORTNEXTBUTTON, NfgSupportWindow::OnSupportNext)
  EVT_TREE_ITEM_COLLAPSING(idSTRATEGYTREE, NfgSupportWindow::OnTreeItemCollapse)
END_EVENT_TABLE()

NfgSupportWindow::NfgSupportWindow(gbtGameDocument *p_doc, wxWindow *p_parent)
  : wxPanel(p_parent, -1, wxDefaultPosition, wxDefaultSize),
    gbtGameView(p_doc), m_map(gbtNfgStrategy())
{
  SetAutoLayout(true);

  m_supportList = new wxChoice(this, idSUPPORTLISTCHOICE,
			       wxDefaultPosition, wxDefaultSize,
			       0, 0);
  m_prevButton = new wxButton(this, idSUPPORTPREVBUTTON, "<-",
			      wxDefaultPosition, wxSize(30, 30));
  m_nextButton = new wxButton(this, idSUPPORTNEXTBUTTON, "->",
			      wxDefaultPosition, wxSize(30, 30));
  m_strategyTree = new widgetStrategyTree(this);

  wxBoxSizer *selectSizer = new wxBoxSizer(wxHORIZONTAL);
  selectSizer->Add(m_prevButton, 0, wxALL, 5);
  selectSizer->Add(m_supportList, 1, wxALL | wxEXPAND, 5);
  selectSizer->Add(m_nextButton, 0, wxALL, 5);

  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(selectSizer, 0, wxEXPAND, 5);
  topSizer->Add(m_strategyTree, 1, wxEXPAND, 5);
  
  SetSizer(topSizer);
  topSizer->Fit(this);
  topSizer->SetSizeHints(this);

  Layout();

  Show(true);
}

void NfgSupportWindow::OnUpdate(gbtGameView *)
{
  m_supportList->Clear();

  const gList<gbtNfgSupport *> &supports = m_doc->AllNfgSupports();

  for (int i = 1; i <= supports.Length(); i++) {
    m_supportList->Append((char *)
			  (ToText(i) + ": " + supports[i]->GetName()));
  }

  int supportIndex = supports.Find(m_doc->GetNfgSupport());
  m_supportList->SetSelection(supportIndex - 1);
  m_prevButton->Enable((supportIndex > 1) ? true : false);
  m_nextButton->Enable((supportIndex < supports.Length()) ? true : false);

  m_strategyTree->DeleteAllItems();

  m_strategyTree->AddRoot((char *) m_doc->GetNfgSupport()->GetName());
  for (int pl = 1; pl <= m_doc->GetNfg().NumPlayers(); pl++) {
    gbtNfgPlayer player = m_doc->GetNfg().GetPlayer(pl);

    wxTreeItemId id = m_strategyTree->AppendItem(m_strategyTree->GetRootItem(),
					       (char *) player.GetLabel());
    
    for (int st = 1; st <= m_doc->GetNfg().NumStrats(pl); st++) {
      gbtNfgStrategy strategy = m_doc->GetNfg().GetPlayer(pl).GetStrategy(st);

      wxTreeItemId stratID = m_strategyTree->AppendItem(id, 
						       (char *) strategy.GetLabel());
      if (m_doc->GetNfgSupport()->Contains(strategy)) {
	m_strategyTree->SetItemTextColour(stratID, *wxBLACK);
      }
      else {
	m_strategyTree->SetItemTextColour(stratID, *wxLIGHT_GREY);
      }
      m_map.Define(stratID, strategy);
    }
    m_strategyTree->Expand(id);
  }
  m_strategyTree->Expand(m_strategyTree->GetRootItem());
  Layout();
}

void NfgSupportWindow::OnSupportList(wxCommandEvent &p_event)
{
  m_doc->SetNfgSupport(p_event.GetSelection() + 1);
}

void NfgSupportWindow::OnSupportPrev(wxCommandEvent &)
{
  m_doc->SetNfgSupport(m_supportList->GetSelection());
}

void NfgSupportWindow::OnSupportNext(wxCommandEvent &)
{
  m_doc->SetNfgSupport(m_supportList->GetSelection() + 2);
}

void NfgSupportWindow::OnTreeItemCollapse(wxTreeEvent &p_event)
{
  if (p_event.GetItem() == m_strategyTree->GetRootItem()) {
    p_event.Veto();
  }
}

void NfgSupportWindow::ToggleItem(wxTreeItemId p_id)
{
  gbtNfgStrategy strategy = m_map(p_id);
  if (strategy.IsNull()) {
    return;
  }

  if (m_doc->GetNfgSupport()->Contains(strategy) &&
      m_doc->GetNfgSupport()->NumStrats(strategy.GetPlayer()) > 1) {
    m_doc->GetNfgSupport()->RemoveStrategy(strategy);
    m_strategyTree->SetItemTextColour(p_id, *wxLIGHT_GREY);
  }
  else {
    m_doc->GetNfgSupport()->AddStrategy(strategy);
    m_strategyTree->SetItemTextColour(p_id, *wxBLACK);
  }

  m_doc->SetNfgSupport(m_supportList->GetSelection() + 1);
}

//-------------------------------------------------------------------------
//                    class gbtNfgSupportFrame
//-------------------------------------------------------------------------

BEGIN_EVENT_TABLE(gbtNfgSupportFrame, wxFrame)
  EVT_MENU(wxID_CLOSE, gbtNfgSupportFrame::Close)
  EVT_CLOSE(gbtNfgSupportFrame::OnClose)
END_EVENT_TABLE()

gbtNfgSupportFrame::gbtNfgSupportFrame(gbtGameDocument *p_doc, 
				       wxWindow *p_parent)
  : wxFrame(p_parent, -1, "", wxDefaultPosition, wxSize(300, 300)),
    gbtGameView(p_doc)
{
  m_panel = new NfgSupportWindow(p_doc, this);

  wxMenu *fileMenu = new wxMenu;
  fileMenu->Append(wxID_CLOSE, "&Close", "Close this window");

  wxMenu *editMenu = new wxMenu;

  wxMenu *viewMenu = new wxMenu;

  wxMenu *formatMenu = new wxMenu;

  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(fileMenu, "&File");
  menuBar->Append(editMenu, "&Edit");
  menuBar->Append(viewMenu, "&View");
  menuBar->Append(formatMenu, "&Format");
  SetMenuBar(menuBar);

  Show(false);
}

gbtNfgSupportFrame::~gbtNfgSupportFrame()
{ }

void gbtNfgSupportFrame::OnClose(wxCloseEvent &p_event)
{
  m_doc->SetShowNfgSupports(false);
  // Frame is now hidden; leave it that way, don't actually close
  p_event.Veto();
}

void gbtNfgSupportFrame::OnUpdate(gbtGameView *p_sender)
{
  if (m_doc->ShowNfgSupports()) {
    m_panel->OnUpdate(p_sender);

    if (m_doc->GetFilename() != "") {
      SetTitle(wxString::Format("Gambit - Supports: [%s] %s", 
				m_doc->GetFilename().c_str(), 
				(char *) m_doc->GetNfg().GetTitle()));
    }
    else {
      SetTitle(wxString::Format("Gambit - Supports: %s",
				(char *) m_doc->GetNfg().GetTitle()));
    }
  }
  Show(m_doc->ShowNfgSupports());
}

#include "base/gmap.imp"

static gOutput &operator<<(gOutput &p_output, wxTreeItemId)
{ return p_output; }

template class gBaseMap<wxTreeItemId, gbtNfgStrategy>;
template class gOrdMap<wxTreeItemId, gbtNfgStrategy>;
