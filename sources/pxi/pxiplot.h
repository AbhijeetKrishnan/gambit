//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Interface to PXI plot base class
//

#ifndef PXIPLOT_H
#define PXIPLOT_H

#include "pxi.h"
#include "overlay.h"
#include "series.h"

class PxiAxisDisplayProperties {
public:
  bool m_shown, m_ticks, m_numbers;
};

class PxiAxisScaleProperties {
public:
  wxString m_minimum, m_maximum;
  int m_divisions;
  bool m_useLog, m_canUseLog;

  double GetMinimum(void) const;
  double GetMaximum(void) const;
};

inline double PxiAxisScaleProperties::GetMinimum(void) const
{
  double d;
  m_minimum.ToDouble(&d);
  return d;
}

inline double PxiAxisScaleProperties::GetMaximum(void) const
{
  double d;
  m_maximum.ToDouble(&d);
  return d;
}

class PxiAxisProperties  {
public:
  wxFont m_font;
  wxColour m_color;
  PxiAxisScaleProperties m_scale;
  PxiAxisDisplayProperties m_display;
};

class PxiTitleProperties {
public:
  wxString m_title;
  wxFont m_font;
  wxColour m_color;
};

class PxiLegendProperties {
public:
  bool m_showLegend;
  wxFont m_font;
  wxColour m_color;
};

class PxiPlot : public wxScrolledWindow {
friend class PxiChild;
public:
  typedef struct LABELSTRUCT {
    friend gbtOutput &operator<<(gbtOutput &op,const LABELSTRUCT &l);
    wxString label;
    double x,y;
    LABELSTRUCT(void):label(""),x(0.0),y(0.0) {}
    LABELSTRUCT &operator=(const LABELSTRUCT &l)
      {label=l.label;x=l.x;y=l.y; return *this;}
    int operator==(const LABELSTRUCT &l)
      {return (label==l.label && x==l.x && y==l.y);}
    int operator!=(const LABELSTRUCT &l)
      {return !(*this==l);}
  } label_struct;

protected:
  const gbtArray<PxiFile *> &m_qreFiles;
  gbtBlock<label_struct> labels;          // labels for generic text
  const ExpData &m_expData;             // reference to experimental data
  bool m_landscape;                     // landscap mode if true
  int m_width, m_height;                // width, height of page
  double m_scale;                        // scaling factor
  wxMemoryDC *m_dc;        // stored DC
  int m_page;
  PxiAxisProperties m_lambdaAxisProp, m_probAxisProp;
  PxiTitleProperties m_titleProp;
  PxiLegendProperties m_legendProp;
  Overlay::Properties m_overlayProp;
  Series::Properties m_seriesProp;

  int Width(void) const { return (m_landscape) ? m_height : m_width; }
  int Height(void) const { return (m_landscape) ? m_width : m_height; }
  void SetLandscape(bool p_flag) { m_landscape = p_flag; }
  bool GetLandscape(void) const { return m_landscape; }
  double GetScale(void) const { return m_scale; } 
  void SetScale(double x);

  virtual void DoPlot(wxDC &dc,
		      int x0, int y0, int cw,int ch, int level=1) = 0;
  void PlotLabels(wxDC &dc, int ch, int cw);
  void DrawToken(wxDC &dc, int x, int y, int point);

  // Event handlers
  void OnPaint(wxPaintEvent &);

public:
  PxiPlot(wxWindow *p_parent, const wxPoint &p_position,
	  const wxSize &p_size, const gbtArray<PxiFile *> &, int p_page,
	  const ExpData &p_expData);
  virtual ~PxiPlot();

  void Render(void);
  void Update(wxDC &dc, int device);

  //  const PxiFile &Header(void) { return m_header; }

  // Interface to property classes
  PxiAxisProperties &GetLambdaAxisProperties(void) { return m_lambdaAxisProp; }
  PxiAxisProperties &GetProbAxisProperties(void) { return m_probAxisProp; }
  PxiTitleProperties &GetTitleProperties(void) { return m_titleProp; }
  PxiLegendProperties &GetLegendProperties(void) { return m_legendProp; }
  Overlay::Properties &GetOverlayProperties(void) { return m_overlayProp; }
  Series::Properties &GetSeriesProperties(void) { return m_seriesProp; }

  bool IsStrategyShown(int iset, int st) const { return (iset == m_page); }

  DECLARE_EVENT_TABLE()
};

#endif  // PXIPLOT_H
