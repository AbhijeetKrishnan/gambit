//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Interface to class representing extensive form viewing window
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

#ifndef EFGSHOW_H
#define EFGSHOW_H

#include <wx/printdlg.h>
#include "gamedoc.h"

class gbtTreeView;

class gbtEfgFrame : public wxFrame, public gbtGameView {
private:
  gbtTreeView *m_treeWindow;

  wxPageSetupData m_pageSetupData;
  wxPrintData m_printData;

  void MakeMenus(void);
  void MakeToolbar(void);
  
  // Event handlers
  void OnCloseWindow(wxCloseEvent &);
  void OnFocus(wxFocusEvent &);

  // Menu event handlers
  void OnFileNew(wxCommandEvent &);
  void OnFileOpen(wxCommandEvent &);
  void OnFileSave(wxCommandEvent &);
  void OnFileExportBMP(wxCommandEvent &);
  void OnFileExportJPEG(wxCommandEvent &);
  void OnFileExportPNG(wxCommandEvent &);
  void OnFileExportPS(wxCommandEvent &);
  void OnFilePageSetup(wxCommandEvent &);
  void OnFilePrintPreview(wxCommandEvent &);
  void OnFilePrint(wxCommandEvent &);
  void OnFileExit(wxCommandEvent &);
  void OnFileMRUFile(wxCommandEvent &);

  void OnEditCut(wxCommandEvent &);
  void OnEditCopy(wxCommandEvent &);
  void OnEditPaste(wxCommandEvent &);
  
  void OnEditInsert(wxCommandEvent &);
  void OnEditDelete(wxCommandEvent &);
  void OnEditReveal(wxCommandEvent &);

  void OnEditMove(wxCommandEvent &);
  void OnEditNode(wxCommandEvent &);
  void OnEditGame(wxCommandEvent &);

  void OnViewNfgReduced(wxCommandEvent &);
  void OnViewProfiles(wxCommandEvent &);
  void OnViewCursor(wxCommandEvent &);
  void OnViewOutcomes(wxCommandEvent &);
  void OnViewSupports(wxCommandEvent &);
  void OnViewZoomIn(wxCommandEvent &);
  void OnViewZoomOut(wxCommandEvent &);
  void OnViewSupportReachable(wxCommandEvent &);

  void OnFormatDisplayLayout(wxCommandEvent &);
  void OnFormatDisplayLegend(wxCommandEvent &);
  void OnFormatDisplayColors(wxCommandEvent &);
  void OnFormatDisplayDecimals(wxCommandEvent &);
  void OnFormatFontsAboveNode(wxCommandEvent &);
  void OnFormatFontsBelowNode(wxCommandEvent &);
  void OnFormatFontsAfterNode(wxCommandEvent &);
  void OnFormatFontsAboveBranch(wxCommandEvent &);
  void OnFormatFontsBelowBranch(wxCommandEvent &);

  void OnToolsDominance(wxCommandEvent &);
  void OnToolsEquilibrium(wxCommandEvent &);
  void OnToolsQre(wxCommandEvent &);
  void OnToolsNormalAgent(wxCommandEvent &);

  void OnHelpAbout(wxCommandEvent &);

  void OnSupportDuplicate(wxCommandEvent &);
  void OnSupportDelete(wxCommandEvent &);

  void OnProfilesNew(wxCommandEvent &);
  void OnProfilesDuplicate(wxCommandEvent &);
  void OnProfilesDelete(wxCommandEvent &);
  void OnProfilesProperties(wxCommandEvent &);
  void OnProfilesReport(wxCommandEvent &);

  // OVERRIDING DOCUMENT MEMBERS
  bool IsEfgView(void) const { return true; }
  bool IsNfgView(void) const { return false; }
  void OnUpdate(gbtGameView *);

public:
  // CONSTRUCTOR AND DESTRUCTOR
  gbtEfgFrame(gbtGameDocument *p_doc, wxWindow *p_parent);
  virtual ~gbtEfgFrame();

  DECLARE_EVENT_TABLE()
};

#endif // EFGSHOW_H
