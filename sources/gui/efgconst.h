//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Declaration of constants used for extensive form menu items
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

#ifndef EFGCONST_H
#define EFGCONST_H

typedef enum {
  GBT_EFG_FILE_IMPORT = 1112,
  GBT_EFG_FILE_IMPORT_COMLAB = 1113,
  GBT_EFG_FILE_EXPORT = 1110,
  GBT_EFG_FILE_EXPORT_POSTSCRIPT = 1111,
  GBT_EFG_FILE_EXPORT_JPEG = 1114,
  GBT_EFG_FILE_EXPORT_BMP = 1115,
  GBT_EFG_FILE_EXPORT_PNG = 1116,

  GBT_EFG_EDIT_INSERT = 1100,
  GBT_EFG_EDIT_DELETE = 1101,
  GBT_EFG_EDIT_REVEAL = 1407,
  GBT_EFG_EDIT_TOGGLE_SUBGAME = 1410,
  GBT_EFG_EDIT_MARK_SUBGAME_TREE = 1411,
  GBT_EFG_EDIT_UNMARK_SUBGAME_TREE = 1412,
  GBT_EFG_EDIT_GAME = 1419,
  GBT_EFG_EDIT_NODE = 1420,
  GBT_EFG_EDIT_MOVE = 1421,

  GBT_EFG_VIEW_PROFILES = 1850,
  GBT_EFG_VIEW_NAVIGATION = 1860,
  GBT_EFG_VIEW_OUTCOMES = 1861,
  GBT_EFG_VIEW_SUPPORTS = 1862,
  GBT_EFG_VIEW_ZOOMIN = 1601,
  GBT_EFG_VIEW_ZOOMOUT = 1602,
  GBT_EFG_VIEW_SUPPORT_REACHABLE = 1814,

  GBT_EFG_FORMAT_DISPLAY = 1603,
  GBT_EFG_FORMAT_DISPLAY_LAYOUT = 1620,
  GBT_EFG_FORMAT_DISPLAY_LEGEND = 1604,
  GBT_EFG_FORMAT_DISPLAY_COLORS = 1621,
  GBT_EFG_FORMAT_DISPLAY_DECIMALS = 1618,
  GBT_EFG_FORMAT_FONTS = 1612,
  GBT_EFG_FORMAT_FONTS_ABOVENODE = 1613,
  GBT_EFG_FORMAT_FONTS_BELOWNODE = 1614,
  GBT_EFG_FORMAT_FONTS_AFTERNODE = 1615,
  GBT_EFG_FORMAT_FONTS_ABOVEBRANCH = 1616,
  GBT_EFG_FORMAT_FONTS_BELOWBRANCH = 1617,

  GBT_EFG_TOOLS_DOMINANCE = 1800,
  GBT_EFG_TOOLS_EQUILIBRIUM = 1801,
  GBT_EFG_TOOLS_QRE = 1802,
  GBT_EFG_TOOLS_NFG = 1521,
  GBT_EFG_TOOLS_NFG_REDUCED = 1504,
  GBT_EFG_TOOLS_NFG_AGENT = 1519,

  // Menu items for support window
  GBT_EFG_SUPPORT_DUPLICATE = 2020,
  GBT_EFG_SUPPORT_DELETE = 2021,
  
  // Menu items for profiles window
  GBT_EFG_PROFILES_NEW = 2001,
  GBT_EFG_PROFILES_DUPLICATE = 2002,
  GBT_EFG_PROFILES_DELETE = 2003,
  GBT_EFG_PROFILES_PROPERTIES = 2004,
  GBT_EFG_PROFILES_REPORT = 2005
} gbtEfgMenuItems;

#endif // EFGCONST_H









