//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Declaration of window to draw correspondence plots
//
// This file is part of Gambit
// Copyright (c) 2002, The Gambit Project
// This code is based loosely upon the plot windows in PXI, which was
// Copyright (c) Eugene Grayver.  The author has graciously permitted its
// use as a model for these classes.
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

#ifndef CORPLOT_H
#define CORPLOT_H

#include <wx/treectrl.h>

// For MixedSolution; when gbtCorBranch goes abstract, should be removed
#include "nash/mixedsol.h"
#include "nash/behavsol.h"

//
// gbtCorBranch is meant to encapsulate the idea of a branch of a
// correspondence.  For now, implement only for mixed profiles and
// QREs.  This must be improved and generalized once API is stable.
//
class gbtCorBranch {
public:
  // Returns the number of dimensions of the product of simplices
  virtual int NumDimensions(void) const = 0;
  // Returns 'true' if the 'index'th dimension is to be plotted
  virtual bool IsDimensionShown(int p_index) const = 0;
  // Set whether the dimension is to be plotted
  virtual void ShowDimension(int p_index, bool p_show) = 0;
  // Returns the number of data points in the branch
  virtual int NumData(void) const = 0;

  // Get the value in the 'p_dim'th dimension of the 'p_index'th point 
  virtual double GetValue(int p_index, int p_dim) const = 0;
  // Get the parameter of the 'p_index'th point
  virtual double GetParameter(int p_index) const = 0;

  // Get the maximum value of the parameter
  virtual double GetMaxParameter(void) const = 0;
  // Get the minimum value of the parameter
  virtual double GetMinParameter(void) const = 0;

  // Get the label associated with dimension 'p_dim'
  virtual gbtText GetLabel(int p_dim) const { return "Series " + ToText(p_dim); }
};

class gbtCorBranchMixed : public gbtCorBranch {
private:
  gbtList<MixedSolution> m_data;
  gbtArray<bool> m_shown;

public:
  // Constructors
  gbtCorBranchMixed(void);
  gbtCorBranchMixed(const gbtList<MixedSolution> &);
  virtual ~gbtCorBranchMixed() { }

  // Returns the number of dimensions of the product of simplices
  int NumDimensions(void) const;
  // Returns 'true' if the 'index'th dimension is to be plotted
  bool IsDimensionShown(int p_index) const { return m_shown[p_index]; }
  // Set whether the dimension is to be plotted
  void ShowDimension(int p_index, bool p_show) { m_shown[p_index] = p_show; }
  // Returns the number of data points in the branch
  int NumData(void) const;

  // Get the value in the 'p_dim'th dimension of the 'p_index'th point 
  double GetValue(int p_index, int p_dim) const;
  // Get the parameter of the 'p_index'th point
  double GetParameter(int p_index) const;

  // Get the maximum value of the parameter
  double GetMaxParameter(void) const;
  // Get the minimum value of the parameter
  double GetMinParameter(void) const;

  // Get the label associated with dimension 'p_dim'
  gbtText GetLabel(int p_dim) const;
};

class gbtCorBranchBehav : public gbtCorBranch {
private:
  gbtList<BehavSolution> m_data;
  gbtArray<bool> m_shown;

public:
  // Constructors
  gbtCorBranchBehav(void);
  gbtCorBranchBehav(const gbtList<BehavSolution> &);
  virtual ~gbtCorBranchBehav() { }

  // Returns the number of dimensions of the product of simplices
  int NumDimensions(void) const;
  // Returns 'true' if the 'index'th dimension is to be plotted
  bool IsDimensionShown(int p_index) const { return m_shown[p_index]; }
  // Set whether the dimension is to be plotted
  void ShowDimension(int p_index, bool p_show) { m_shown[p_index] = p_show; }
  // Returns the number of data points in the branch
  int NumData(void) const;

  // Get the value in the 'p_dim'th dimension of the 'p_index'th point 
  double GetValue(int p_index, int p_dim) const;
  // Get the parameter of the 'p_index'th point
  double GetParameter(int p_index) const;

  // Get the maximum value of the parameter
  double GetMaxParameter(void) const;
  // Get the minimum value of the parameter
  double GetMinParameter(void) const;
};

class gbtCorPlotXAxis {
private:
  int m_numDivisions;
  double m_minValue, m_maxValue;
  wxFont m_labelFont;
  wxColour m_labelColor;
    
public:
  gbtCorPlotXAxis(void);

  int NumDivisions(void) const { return m_numDivisions; }
  void SetNumDivisions(int p_numDivisions) { m_numDivisions = p_numDivisions; }

  double MinValue(void) const { return m_minValue; }
  void SetMinValue(double p_minValue) { m_minValue = p_minValue; }

  double MaxValue(void) const { return m_maxValue; }
  void SetMaxValue(double p_maxValue) { m_maxValue = p_maxValue; }

  const wxFont &GetLabelFont(void) const { return m_labelFont; }
  void SetLabelFont(const wxFont &p_labelFont) { m_labelFont = p_labelFont; }

  const wxColour &GetLabelColor(void) const { return m_labelColor; }
  void SetLabelColor(const wxColour &p_labelColor)
    { m_labelColor = p_labelColor; }
};

class gbtCorPlotYAxis {
private:
  int m_numDivisions;
  double m_minValue, m_maxValue;
  wxFont m_labelFont;
  wxColour m_labelColor;

public:
  gbtCorPlotYAxis(void);

  int NumDivisions(void) const { return m_numDivisions; }
  void SetNumDivisions(int p_numDivisions) { m_numDivisions = p_numDivisions; }

  double MinValue(void) const { return m_minValue; }
  void SetMinValue(double p_minValue) { m_minValue = p_minValue; }

  double MaxValue(void) const { return m_maxValue; }
  void SetMaxValue(double p_maxValue) { m_maxValue = p_maxValue; }

  const wxFont &GetLabelFont(void) const { return m_labelFont; }
  void SetLabelFont(const wxFont &p_labelFont) { m_labelFont = p_labelFont; }

  const wxColour &GetLabelColor(void) const { return m_labelColor; }
  void SetLabelColor(const wxColour &p_labelColor)
    { m_labelColor = p_labelColor; }
};

class gbtCorPlotWindow : public wxScrolledWindow {
private:
  gbtCorBranch *m_cor;
  gbtCorPlotXAxis m_xAxis;
  gbtCorPlotYAxis m_yAxis;

  // Margin outside plot area
  int m_marginX, m_marginY;

  // Private auxiliary drawing functions
  void DrawXAxis(wxDC &);
  void DrawYAxis(wxDC &);
  void DrawLegend(wxDC &);
  void DrawDimension(wxDC &, int);

  // Converting from data values to chart coordinates
  void DataToXY(double p_param, double p_value, int &p_x, int &p_y) const;

  // Event handlers
  void OnPaint(wxPaintEvent &);

public:
  gbtCorPlotWindow(wxWindow *p_parent,
		   const wxPoint & = wxDefaultPosition,
		   const wxSize & = wxDefaultSize);
  virtual ~gbtCorPlotWindow();

  void SetCorrespondence(gbtCorBranch *p_branch);
  gbtCorBranch *GetCorrespondence(void) const;

  void OnDraw(wxDC &);

  DECLARE_EVENT_TABLE()
};

class gbtCorPlotFrame : public wxFrame {
protected:
  gbtCorPlotWindow *m_plot;

  void OnFileExportBMP(wxCommandEvent &);
  void OnFileExportJPEG(wxCommandEvent &);
  void OnFileExportPNG(wxCommandEvent &);
  void OnFileExportPS(wxCommandEvent &);

  virtual void OnEditSupport(wxCommandEvent &) { }

public:
  gbtCorPlotFrame(wxWindow *p_parent, const wxPoint &, const wxSize &);

  void SetCorrespondence(gbtCorBranch *p_branch)
    { m_plot->SetCorrespondence(p_branch); }
  gbtCorBranch *GetCorrespondence(void) const
    { return m_plot->GetCorrespondence(); }

  DECLARE_EVENT_TABLE()
};

class gbtNfgCorPlotFrame : public gbtCorPlotFrame {
private:
  gbtNfgSupport m_support;

protected:
  void OnEditSupport(wxCommandEvent &);

public:
  gbtNfgCorPlotFrame(const gbtNfgSupport &p_support,
		     wxWindow *p_parent, const wxPoint &, const wxSize &);
};

class gbtEfgCorPlotFrame : public gbtCorPlotFrame {
private:
  gbtEfgSupport m_support;

protected:
  void OnEditSupport(wxCommandEvent &);

public:
  gbtEfgCorPlotFrame(const gbtEfgSupport &p_support,
		     wxWindow *p_parent, const wxPoint &, const wxSize &);
};

#endif  // CORPLOT_H
