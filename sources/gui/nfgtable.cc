//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Implementation of normal form table class
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
#include "wx/config.h"
#include "wx/tokenzr.h"  // for wxStringTokenizer
#include "nfgshow.h"
#include "nfgtable.h"
#include "id.h"

//---------------------------------------------------------------------
//                   class gbtPayoffVectorRenderer
//---------------------------------------------------------------------

class gbtPayoffVectorRenderer : public wxGridCellRenderer {
private:
  gbtGameDocument *m_doc;

protected:
  // set the text colours before drawing
  void SetTextColoursAndFont(wxGrid &grid, wxGridCellAttr &attr,
			     wxDC &dc, bool isSelected);

  // calc the string extent for given string/font
  wxSize DoGetBestSize(wxGridCellAttr &attr, wxDC &dc,
		       const wxString &text);
public:
  gbtPayoffVectorRenderer(gbtGameDocument *p_doc)
    : m_doc(p_doc) { }

  // draw the string
  virtual void Draw(wxGrid &grid, wxGridCellAttr &attr,
		    wxDC &dc, const wxRect &rect,
		    int row, int col, bool isSelected);

  // return the string extent
  virtual wxSize GetBestSize(wxGrid &grid, wxGridCellAttr &attr,
			     wxDC &dc, int row, int col);

  virtual wxGridCellRenderer *Clone() const
    { return new gbtPayoffVectorRenderer(m_doc); }
};

void gbtPayoffVectorRenderer::SetTextColoursAndFont(wxGrid &p_grid,
						    wxGridCellAttr &p_attr,
						    wxDC &p_dc,
						    bool p_isSelected)
{
  p_dc.SetBackgroundMode(wxTRANSPARENT);

  if (p_isSelected) {
    p_dc.SetTextBackground(p_grid.GetSelectionBackground());
    p_dc.SetTextForeground(p_grid.GetSelectionForeground());
  }
  else {
    p_dc.SetTextBackground(p_attr.GetBackgroundColour());
    p_dc.SetTextForeground(p_attr.GetTextColour());
  }

  p_dc.SetFont(p_attr.GetFont());
}

wxSize gbtPayoffVectorRenderer::DoGetBestSize(wxGridCellAttr &p_attr,
					      wxDC &p_dc,
					      const wxString &p_text)
{
  wxCoord x = 0, y = 0;
  p_dc.SetFont(p_attr.GetFont());
  p_dc.GetTextExtent(p_text, &x, &y);
  return wxSize((int) (x * 1.10), (int) (y * 1.25));
}

wxSize gbtPayoffVectorRenderer::GetBestSize(wxGrid &p_grid,
					    wxGridCellAttr &p_attr,
					    wxDC &p_dc, int p_row, int p_col)
{
  return DoGetBestSize(p_attr, p_dc,
		       _T("(") + p_grid.GetCellValue(p_row, p_col) + _T(")"));
}

void gbtPayoffVectorRenderer::Draw(wxGrid &p_grid, wxGridCellAttr &p_attr,
				   wxDC &p_dc, const wxRect &p_rectCell,
				   int p_row, int p_col, bool p_isSelected)
{
  wxGridCellRenderer::Draw(p_grid, p_attr, p_dc, p_rectCell,
			   p_row, p_col, p_isSelected);

  // now we only have to draw the text
  SetTextColoursAndFont(p_grid, p_attr, p_dc, p_isSelected);

  wxString text = p_grid.GetCellValue(p_row, p_col);

  wxRect rect = p_rectCell;
  wxCoord x, y;
  p_dc.GetTextExtent(_T("(") + text + _T(")"), &x, &y);
  rect.x = rect.x + (rect.width - x) / 2;

  rect.Inflate(-1);
  p_grid.DrawTextRectangle(p_dc, wxT("("), rect);
  p_dc.GetTextExtent(wxT("("), &x, &y);
  rect.x += x;

  wxStringTokenizer tok(text, wxT(","));
  for (int pl = 1; pl <= m_doc->GetNfg().NumPlayers(); pl++) {
    p_dc.SetTextForeground(m_doc->GetPreferences().PlayerColor(pl));
    wxString payoff = tok.GetNextToken();
    p_grid.DrawTextRectangle(p_dc, payoff, rect);
    p_dc.GetTextExtent(payoff, &x, &y);
    rect.x += x;
    
    p_dc.SetTextForeground(*wxBLACK);
    if (pl < m_doc->GetNfg().NumPlayers()) {
      p_grid.DrawTextRectangle(p_dc, wxT(","), rect);
      p_dc.GetTextExtent(wxT(")"), &x, &y);
      rect.x += x;
    }
    else {
      p_grid.DrawTextRectangle(p_dc, wxT(")"), rect); 
    }
  }
}


//---------------------------------------------------------------------
//                   class gbtSchellingRenderer
//---------------------------------------------------------------------

//
// The Schelling-style renderer puts the row player payoff in the lower-left
// corner, and the column player payoff in the upper-right corner, in
// a style which originated (apparently) with Thomas Schelling.
// 
// This style is only valid for two-player games, and the code is written
// assuming this.  Bad Things might happen if this assumption is violated.
//
class gbtSchellingRenderer : public wxGridCellRenderer {
private:
  gbtGameDocument *m_doc;

protected:
  // set the text colours before drawing
  void SetTextColoursAndFont(wxGrid &grid, wxGridCellAttr &attr,
			     wxDC &dc, bool isSelected);

  // calc the string extent for given string/font
  wxSize DoGetBestSize(wxGridCellAttr &attr, wxDC &dc,
		       const wxString &text);
public:
  gbtSchellingRenderer(gbtGameDocument *p_doc)
    : m_doc(p_doc) { }

  // draw the string
  virtual void Draw(wxGrid &grid, wxGridCellAttr &attr,
		    wxDC &dc, const wxRect &rect,
		    int row, int col, bool isSelected);

  // return the string extent
  virtual wxSize GetBestSize(wxGrid &grid, wxGridCellAttr &attr,
			     wxDC &dc, int row, int col);

  virtual wxGridCellRenderer *Clone() const
    { return new gbtSchellingRenderer(m_doc); }
};

void gbtSchellingRenderer::SetTextColoursAndFont(wxGrid &p_grid,
						 wxGridCellAttr &p_attr,
						 wxDC &p_dc,
						 bool p_isSelected)
{
  p_dc.SetBackgroundMode(wxTRANSPARENT);

  if (p_isSelected) {
    p_dc.SetTextBackground(p_grid.GetSelectionBackground());
    p_dc.SetTextForeground(p_grid.GetSelectionForeground());
  }
  else {
    p_dc.SetTextBackground(p_attr.GetBackgroundColour());
    p_dc.SetTextForeground(p_attr.GetTextColour());
  }

  p_dc.SetFont(p_attr.GetFont());
}

wxSize gbtSchellingRenderer::DoGetBestSize(wxGridCellAttr &p_attr,
					   wxDC &p_dc,
					   const wxString &p_text)
{
  wxCoord x = 0, y = 0;
  p_dc.SetFont(p_attr.GetFont());
  p_dc.GetTextExtent(p_text, &x, &y);
  return wxSize(x, 2 * y);
}

wxSize gbtSchellingRenderer::GetBestSize(wxGrid &p_grid,
					 wxGridCellAttr &p_attr,
					 wxDC &p_dc, int p_row, int p_col)
{
  return DoGetBestSize(p_attr, p_dc,
		       p_grid.GetCellValue(p_row, p_col));
}

void gbtSchellingRenderer::Draw(wxGrid &p_grid, wxGridCellAttr &p_attr,
				wxDC &p_dc, const wxRect &p_rectCell,
				int p_row, int p_col, bool p_isSelected)
{
  wxGridCellRenderer::Draw(p_grid, p_attr, p_dc, p_rectCell,
			   p_row, p_col, p_isSelected);

  // now we only have to draw the text
  SetTextColoursAndFont(p_grid, p_attr, p_dc, p_isSelected);

  wxStringTokenizer tok(p_grid.GetCellValue(p_row, p_col), wxT(","));

  // A small margin to place around payoffs to avoid butting up against
  // grid lines
  const int c_margin = 3;
  wxCoord x, y;
  
  // Draw player 1's payoffs, lower-left corner
  wxRect rect = p_rectCell;
  rect.x += c_margin;
  rect.y += rect.height / 2;
  p_dc.SetTextForeground(m_doc->GetPreferences().PlayerColor(1));
  p_grid.DrawTextRectangle(p_dc, tok.GetNextToken(), rect);

  // Draw player 2's payoffs, upper-right corner
  wxString player2 = tok.GetNextToken();
  p_dc.GetTextExtent(player2, &x, &y);
  rect = p_rectCell;
  rect.x = rect.x + rect.width - x - c_margin;
  rect.y += c_margin;
  p_dc.SetTextForeground(m_doc->GetPreferences().PlayerColor(2));
  p_grid.DrawTextRectangle(p_dc, player2, rect);
}


//---------------------------------------------------------------------
//                       class NfgGridTable
//---------------------------------------------------------------------

class NfgGridTable : public wxGridTableBase {
private:
  gbtGameDocument *m_doc;
  gbtNfgTable *m_table;

public:
  NfgGridTable(gbtNfgTable *p_table, gbtGameDocument *p_doc);
  virtual ~NfgGridTable() { }

  int GetNumberRows(void);
  int GetNumberCols(void);
  wxString GetValue(int row, int col);
  wxString GetRowLabelValue(int);
  wxString GetColLabelValue(int);
  void SetValue(int, int, const wxString &);
  bool IsEmptyCell(int, int) { return false; }

  bool InsertRows(size_t pos = 0, size_t numRows = 1);
  bool AppendRows(size_t numRows = 1);
  bool DeleteRows(size_t pos = 0, size_t numRows = 1);
  bool InsertCols(size_t pos = 0, size_t numCols = 1);
  bool AppendCols(size_t numCols = 1);
  bool DeleteCols(size_t pos = 0, size_t numCols = 1);

  bool CanHaveAttributes(void) { return true; }
  wxGridCellAttr *GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind);
};

NfgGridTable::NfgGridTable(gbtNfgTable *p_table, gbtGameDocument *p_doc)
  : m_doc(p_doc), m_table(p_table)
{ }

int NfgGridTable::GetNumberRows(void)
{
  return (m_doc->GetNfgSupport().NumStrats(m_doc->GetRowPlayer()) +
	  m_table->ShowProbs() + m_table->ShowDominance() +
	  m_table->ShowValues());
}

int NfgGridTable::GetNumberCols(void)
{
  return (m_doc->GetNfgSupport().NumStrats(m_doc->GetColPlayer()) +
	  m_table->ShowProbs() + m_table->ShowDominance() + 
	  m_table->ShowValues());
}

wxString NfgGridTable::GetRowLabelValue(int p_row)
{
  int numStrats = m_doc->GetNfgSupport().NumStrats(m_doc->GetRowPlayer());
  if (p_row + 1 <= numStrats) {
    return wxString::Format(wxT("%s"), (char *) m_doc->GetNfgSupport().GetStrategy(m_doc->GetRowPlayer(), p_row+1).GetLabel());
  }
  else if (p_row + 1 == numStrats + m_table->ShowDominance()) {
    return wxT("Dom");
  }
  else if (p_row + 1 == 
	   numStrats + m_table->ShowDominance() + m_table->ShowProbs()) {
    return wxT("Prob");
  }
  else {
    return wxT("Val");
  }
}

wxString NfgGridTable::GetColLabelValue(int p_col)
{
  int numStrats = m_doc->GetNfgSupport().NumStrats(m_doc->GetColPlayer());
  if (p_col + 1 <= numStrats) {
    return wxString::Format(wxT("%s"), (char *) m_doc->GetNfgSupport().GetStrategy(m_doc->GetColPlayer(), p_col+1).GetLabel());
  }
  else if (p_col + 1 == numStrats + m_table->ShowDominance()) {
    return wxT("Dom");
  }
  else if (p_col + 1 == 
	   numStrats + m_table->ShowDominance() + m_table->ShowProbs()) {
    return wxT("Prob");
  }
  else {
    return wxT("Val");
  }
}

wxString NfgGridTable::GetValue(int row, int col)
{
  int rowPlayer = m_doc->GetRowPlayer();
  int colPlayer = m_doc->GetColPlayer();
  const gbtNfgSupport &support = m_doc->GetNfgSupport();
  int numRowStrats = support.NumStrats(rowPlayer);
  int numColStrats = support.NumStrats(colPlayer);

  if (row < numRowStrats && col < numColStrats) {
    gbtArray<int> strategy(m_doc->GetContingency());
    strategy[m_doc->GetRowPlayer()] = row + 1;
    strategy[m_doc->GetColPlayer()] = col + 1;
    
    gbtNfgContingency profile(m_doc->GetNfg());
    for (int pl = 1; pl <= strategy.Length(); pl++) {
      profile.Set(pl, support.GetStrategy(pl, strategy[pl]));
    }

    if (m_doc->HasEfg() ||
	m_doc->GetPreferences().OutcomeLabel() == GBT_OUTCOME_LABEL_PAYOFFS) {
      wxString ret = wxT("");
      for (int pl = 1; pl <= strategy.Length(); pl++) {
	ret += wxString::Format(wxT("%s"),
				(char *) ToText(profile.GetPayoff(m_doc->GetNfg().GetPlayer(pl)),
						m_doc->GetPreferences().NumDecimals()));
	if (pl < strategy.Length()) {
	  ret += wxT(",");
	}
      }
      ret += wxT("");
      return ret;
    }
    else {
      gbtNfgOutcome outcome = profile.GetOutcome();
      if (!outcome.IsNull()) {
	wxString ret = wxString::Format(wxT("%s"), (char *) outcome.GetLabel());
	if (ret == wxT("")) {
	  ret = wxString::Format(wxT("Outcome%d"), outcome.GetId());
	}
	return ret;
      }
      else {
	return wxT("Null");
      }
    }
  }
  else if (row < numRowStrats &&
	   col == numColStrats + m_table->ShowDominance() - 1) {
    gbtNfgStrategy strategy = support.GetStrategy(rowPlayer, row + 1);
    if (support.IsDominated(strategy, true)) {
      return wxT("S");
    }
    else if (support.IsDominated(strategy, false)) {
      return wxT("W");
    }
    else {
      return wxT("N");
    }
  }
  else if (row == numRowStrats + m_table->ShowDominance() - 1 &&
	   col < numColStrats) {
    gbtNfgStrategy strategy = support.GetStrategy(colPlayer, col + 1);
    if (support.IsDominated(strategy, true)) {
      return wxT("S");
    }
    else if (support.IsDominated(strategy, false)) {
      return wxT("W");
    }
    else {
      return wxT("N");
    }
  }
  else if (row < numRowStrats && 
	   col == numColStrats + m_table->ShowDominance() + m_table->ShowProbs() - 1) {
    gbtNfgStrategy strategy = support.GetStrategy(rowPlayer, row + 1);
    return wxString::Format(wxT("%s"), 
			    (char *) ToText(m_doc->GetMixedProfile()(strategy),
					    m_doc->GetPreferences().NumDecimals()));
  }
  else if (row == numRowStrats + m_table->ShowDominance() + m_table->ShowProbs() - 1 && 
	   col < numColStrats) {
    gbtNfgStrategy strategy = support.GetStrategy(colPlayer, col + 1);
    return wxString::Format(wxT("%s"), 
			    (char *) ToText(m_doc->GetMixedProfile()(strategy),
					    m_doc->GetPreferences().NumDecimals()));
  }
  else if (row < numRowStrats && 
	   col == numColStrats + m_table->ShowDominance() + m_table->ShowProbs() + m_table->ShowValues() - 1) {
    gbtNfgStrategy strategy = support.GetStrategy(rowPlayer, row + 1);
    return wxString::Format(wxT("%s"),
			    (char *) ToText(m_doc->GetMixedProfile().GetStrategyValue(strategy),
					    m_doc->GetPreferences().NumDecimals()));
  }
  else if (row == numRowStrats + m_table->ShowDominance() + m_table->ShowProbs() + m_table->ShowValues() - 1 && 
	   col < numColStrats) {
    gbtNfgStrategy strategy = support.GetStrategy(colPlayer, col + 1);
    return wxString::Format(wxT("%s"),
			    (char *) ToText(m_doc->GetMixedProfile().GetStrategyValue(strategy),
					    m_doc->GetPreferences().NumDecimals()));
  }

  return wxT("");
}

void NfgGridTable::SetValue(int row, int col, const wxString &)
{
  wxGridTableMessage msg(this, wxGRIDTABLE_REQUEST_VIEW_GET_VALUES, row, col);
  GetView()->ProcessTableMessage(msg);
}
	
bool NfgGridTable::InsertRows(size_t pos, size_t numRows)
{
  wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_INSERTED,
			 pos, numRows);
  GetView()->ProcessTableMessage(msg);
  return true;
}

bool NfgGridTable::AppendRows(size_t numRows)
{
  wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_APPENDED, numRows);
  GetView()->ProcessTableMessage(msg);
  return true;
}

bool NfgGridTable::DeleteRows(size_t pos, size_t numRows)
{
  wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_DELETED,
			 pos, numRows);
  GetView()->ProcessTableMessage(msg);
  return true;
}

bool NfgGridTable::InsertCols(size_t pos, size_t numCols)
{
  wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_COLS_INSERTED,
			 pos, numCols);
  GetView()->ProcessTableMessage(msg);
  return true;
}

bool NfgGridTable::AppendCols(size_t numCols)
{
  wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_COLS_APPENDED, numCols);
  GetView()->ProcessTableMessage(msg);
  return true;
}

bool NfgGridTable::DeleteCols(size_t pos, size_t numCols)
{
  wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_COLS_DELETED,
			 pos, numCols);
  GetView()->ProcessTableMessage(msg);
  return true;
}

wxGridCellAttr *NfgGridTable::GetAttr(int row, int col,
				      wxGridCellAttr::wxAttrKind /*any*/)
{
  wxGridCellAttr *attr = new wxGridCellAttr;

  if (row >= m_doc->GetNfgSupport().NumStrats(m_doc->GetRowPlayer()) &&
      col >= m_doc->GetNfgSupport().NumStrats(m_doc->GetColPlayer())) {
    attr->SetBackgroundColour(*wxBLACK);
  }
  else if (row >= m_doc->GetNfgSupport().NumStrats(m_doc->GetRowPlayer()) ||
	   col >= m_doc->GetNfgSupport().NumStrats(m_doc->GetColPlayer())) {
    attr->SetBackgroundColour(*wxLIGHT_GREY);
  }
  else {
    if (m_doc->GetNfg().NumPlayers() == 2) {
      attr->SetRenderer(new gbtSchellingRenderer(m_doc));
    }
    else {
      attr->SetRenderer(new gbtPayoffVectorRenderer(m_doc));
    }
  }

  attr->SetAlignment(wxALIGN_CENTER, wxALIGN_CENTER);

  return attr;
}

//======================================================================
//                   class gbtNfgTable: Member functions
//======================================================================

BEGIN_EVENT_TABLE(gbtNfgTable, wxGrid)
  EVT_GRID_CELL_LEFT_CLICK(gbtNfgTable::OnLeftClick)
  EVT_GRID_CELL_LEFT_DCLICK(gbtNfgTable::OnLeftDoubleClick)
  EVT_GRID_LABEL_LEFT_CLICK(gbtNfgTable::OnLabelLeftClick)
END_EVENT_TABLE()

gbtNfgTable::gbtNfgTable(gbtGameDocument *p_doc, wxWindow *p_parent)
  : wxGrid(p_parent, -1, wxDefaultPosition, wxDefaultSize),
    gbtGameView(p_doc),
    m_editable(true), 
    m_showProb(0), m_showDom(0), m_showValue(0)
{
  SetAutoLayout(true);

  SetTable(new NfgGridTable(this, m_doc), true);
  SetGridCursor(0, 0);
  SetEditable(false);
  SetDefaultCellFont(m_doc->GetPreferences().GetDataFont());
  SetLabelFont(m_doc->GetPreferences().GetLabelFont());
  SetDefaultCellAlignment(wxALIGN_CENTER, wxALIGN_CENTER);
  SetRowLabelAlignment(wxALIGN_CENTER, wxALIGN_CENTER);
  SetColLabelAlignment(wxALIGN_CENTER, wxALIGN_CENTER);
  DisableDragRowSize();
  DisableDragColSize();
  AutoSizeRows();
  AutoSizeColumns();
  AdjustScrollbars();
  SetSize(GetBestSize());
  Show(true);
}

void gbtNfgTable::OnUpdate(gbtGameView *)
{ 
  if (!m_doc->ShowNfg()) {
    Show(false);
    return;
  }

  SetDefaultCellFont(m_doc->GetPreferences().GetDataFont());
  SetLabelFont(m_doc->GetPreferences().GetLabelFont());

  const gbtNfgSupport &support = m_doc->GetNfgSupport();
  int rowPlayer = m_doc->GetRowPlayer(), colPlayer = m_doc->GetColPlayer();
  BeginBatch();
  int stratRows = GetRows() - m_showProb - m_showDom - m_showValue;
  int stratCols = GetCols() - m_showProb - m_showDom - m_showValue;

  if (support.NumStrats(rowPlayer) < stratRows) {
    DeleteRows(0, stratRows - support.NumStrats(rowPlayer));
  }
  else if (support.NumStrats(rowPlayer) > stratRows) {
    InsertRows(0, support.NumStrats(rowPlayer) - stratRows); 
  }

  if (support.NumStrats(colPlayer) < stratCols) {
    DeleteCols(0, stratCols - support.NumStrats(colPlayer));
  }
  else if (support.NumStrats(colPlayer) > stratCols) {
    InsertCols(0, support.NumStrats(colPlayer) - stratCols);
  }

#ifdef UNUSED
  // This is currently disabled, as it scales very badly due to the
  // expense in the generation of cell values in the virtual table.
  // Hopefully, we can bring this back soon when the table views are
  // made "smarter".
  AutoSizeRows();
  AutoSizeColumns();
  // Set all strategy columns to be the same width, which is
  // the narrowest width which fits all the entries
  int max = 0;
  for (int col = 0; col < support.NumStrats(colPlayer); col++) {
    if (GetColSize(col) > max) {
      max = GetColSize(col);
    }
  }
  for (int col = 0; col < support.NumStrats(colPlayer); col++) {
    SetColSize(col, max);
  }
#endif  // UNUSED

  SetGridCursor(m_doc->GetContingency()[m_doc->GetRowPlayer()] - 1,
		m_doc->GetContingency()[m_doc->GetColPlayer()] - 1);
  EndBatch();
  AdjustScrollbars();
  ForceRefresh();
  Layout();
  Show(true);
}

void gbtNfgTable::ToggleProbs(void)
{
  m_showProb = 1 - m_showProb;
  if (m_showProb) {
    AppendCols();
    AppendRows();
  }
  else {
    DeleteCols();
    DeleteRows();
  }
  AutoSizeRows();
  AutoSizeColumns();
  AdjustScrollbars();
  Refresh();
}

void gbtNfgTable::ToggleDominance(void)
{
  m_showDom = 1 - m_showDom;
  if (m_showDom) {
    AppendCols();
    AppendRows();
  }
  else {
    DeleteCols();
    DeleteRows();
  }
  AutoSizeRows();
  AutoSizeColumns();
  AdjustScrollbars();
  Refresh();
}

void gbtNfgTable::ToggleValues(void)
{
  m_showValue = 1 - m_showValue;
  if (m_showValue) {
    AppendCols();
    AppendRows();
  }
  else {
    DeleteCols();
    DeleteRows();
  }
  AutoSizeRows();
  AutoSizeColumns();
  AdjustScrollbars();
  Refresh();
}

void gbtNfgTable::OnLeftClick(wxGridEvent &p_event)
{
  if (p_event.GetRow() >= m_doc->GetNfgSupport().NumStrats(m_doc->GetRowPlayer()) ||
      p_event.GetCol() >= m_doc->GetNfgSupport().NumStrats(m_doc->GetColPlayer())) {
    p_event.Veto();
  }
  else {
    gbtArray<int> contingency = m_doc->GetContingency();
    contingency[m_doc->GetRowPlayer()] = p_event.GetRow() + 1;
    contingency[m_doc->GetColPlayer()] = p_event.GetCol() + 1;
    m_doc->SetContingency(contingency);
    // now continue with the default behavior (i.e., highlight the new cell)
  }
}

void gbtNfgTable::OnLeftDoubleClick(wxGridEvent &p_event)
{
  if (m_editable &&
      p_event.GetRow() < m_doc->GetNfgSupport().NumStrats(m_doc->GetRowPlayer()) &&
      p_event.GetCol() < m_doc->GetNfgSupport().NumStrats(m_doc->GetColPlayer())) {
    wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED,
			 GBT_MENU_EDIT_CONTINGENCY);
    GetParent()->AddPendingEvent(event);
  }
}

void gbtNfgTable::OnLabelLeftClick(wxGridEvent &p_event)
{
  // for the moment, just veto it
  p_event.Veto();
}
