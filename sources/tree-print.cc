//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Declaration of printout for extensive form tree
//
// This file is part of Gambit
// Copyright (c) 2004y, The Gambit Project
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

#include "tree-print.h"
#include "tree-display.h"   // for layout

gbtTreePrintout::gbtTreePrintout(const gbtTreeLayout &p_layout,
				 const wxString &p_title)
  : wxPrintout(p_title), m_layout(p_layout)
{ }

//
// Does the actual work of printing, by computing an appropriate scaling
// for a full-page printout.
// This code is based on the code in the printing sample from wxWindows
//

bool gbtTreePrintout::OnPrintPage(int)
{
  wxDC *dc = GetDC();
  if (!dc) return false;
    
  dc->SetBackgroundMode(wxTRANSPARENT);
   
  // The actual size of the tree, in pixels
  int maxX = m_layout.GetMaxX();
  int maxY = m_layout.GetMaxY();

  // Margins
  int marginX = 50;
  int marginY = 50;

  maxX += 2 * marginX;
  maxY += 2 * marginY;

  // Get the size of the DC in pixels
  wxCoord w, h;
  dc->GetSize(&w, &h);

  // Calculate a scaling factor
  float scaleX = (float) w / (float) maxX;
  float scaleY = (float) h / (float) maxY;

  float actualScale = (scaleX < scaleY) ? scaleX : scaleY;

  // Calculate the position on the DC to center the tree
  float posX = (float) ((w - (m_layout.GetMaxX() * actualScale)) / 2.0);
  float posY = (float) ((h - (m_layout.GetMaxY() * actualScale)) / 2.0);

  // Set the scale and origin
  dc->SetUserScale(actualScale, actualScale);
  dc->SetDeviceOrigin((long) posX, (long) posY);

  // Draw!
  m_layout.DrawTree(*dc);
    
  return true;
}

bool gbtTreePrintout::HasPage(int page)
{
  return (page <= 1);
}

bool gbtTreePrintout::OnBeginDocument(int startPage, int endPage)
{
  if (!wxPrintout::OnBeginDocument(startPage, endPage))
    return false;
    
  return true;
}

// Since we can not get at the actual device context in this function, we
// have no way to tell how many pages will be used in the wysiwyg mode. So,
// we have no choice but to disable the From:To page selection mechanism.
void gbtTreePrintout::GetPageInfo(int *minPage, int *maxPage,
			      int *selPageFrom, int *selPageTo)
{
  *minPage = 1;
  *maxPage = 1;
  *selPageFrom = 1;
  *selPageTo = 1;
}
