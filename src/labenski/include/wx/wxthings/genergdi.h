/////////////////////////////////////////////////////////////////////////////
// Name:        genergdi.h
// Purpose:     Generic gdi pen and colour
// Author:      John Labenski
// Modified by:
// Created:     12/1/2000
// Copyright:   (c) John Labenski
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GENERGDI_H_
#define _WX_GENERGDI_H_

#include "wx/colour.h"
#include "wx/pen.h"
#include "wx/brush.h"
#include "wx/wxthings/thingdef.h"

class WXDLLIMPEXP_THINGS wxGenericColour;
class WXDLLIMPEXP_THINGS wxGenericPen;
class WXDLLIMPEXP_THINGS wxGenericBrush;

// Uncreated and invalid versions of the generic colour, pen, and brush
WXDLLIMPEXP_DATA_THINGS(extern const wxGenericColour) wxNullGenericColour;
WXDLLIMPEXP_DATA_THINGS(extern const wxGenericPen)    wxNullGenericPen;
WXDLLIMPEXP_DATA_THINGS(extern const wxGenericBrush)  wxNullGenericBrush;

// Arrays of the generic colours, pens, and brushes
#include "wx/dynarray.h"
#ifndef WX_DECLARE_OBJARRAY_WITH_DECL // for wx2.4 backwards compatibility
    #define WX_DECLARE_OBJARRAY_WITH_DECL(T, name, expmode) WX_DECLARE_USER_EXPORTED_OBJARRAY(T, name, WXDLLIMPEXP_THINGS)
#endif
WX_DECLARE_OBJARRAY_WITH_DECL(wxGenericColour, wxArrayGenericColour, class WXDLLIMPEXP_THINGS);
WX_DECLARE_OBJARRAY_WITH_DECL(wxGenericPen,    wxArrayGenericPen,    class WXDLLIMPEXP_THINGS);
WX_DECLARE_OBJARRAY_WITH_DECL(wxGenericBrush,  wxArrayGenericBrush,  class WXDLLIMPEXP_THINGS);

//----------------------------------------------------------------------------
// wxGenericColour - a RGBA colour that's converts from and to a wxColour.
//    You can have thousands of them without using up resources. (on MSW)
//----------------------------------------------------------------------------

class WXDLLIMPEXP_THINGS wxGenericColour : public wxObject
{
public:
    wxGenericColour() : wxObject() {}
    wxGenericColour( const wxGenericColour& c ) { Create(c); }
    wxGenericColour( const wxColour& c )        { Create(c); }
    wxGenericColour( unsigned char red,  unsigned char green,
                     unsigned char blue, unsigned char alpha=255 )
                : wxObject() { Create(red, green, blue, alpha); }
    wxGenericColour( unsigned long colABGR ) : wxObject() { CreateABGR(colABGR); }

    // Implicit conversion from the colour name
    wxGenericColour( const wxString &colourName ) : wxObject() { Create(colourName); }
    wxGenericColour( const char *colourName )     : wxObject() { Create(wxString::FromAscii(colourName)); }
#if wxUSE_UNICODE
    wxGenericColour( const wxChar *colourName )   : wxObject() { Create(wxString(colourName)); }
#endif

    virtual ~wxGenericColour() {}
    // Destroy the refed data
    void Destroy() { UnRef(); }
    // Is this colour valid, has refed data
    bool Ok() const { return m_refData != NULL; }

    // -----------------------------------------------------------------------
    // (re)Create this colour, unrefing this colour first.
    //    Use these to detach this colour from it's refed copies.
    void Create( const wxGenericColour& c );
    void Create( const wxColour& c);
    void Create( unsigned char red = 0, unsigned char green = 0,
                 unsigned char blue = 0, unsigned char alpha=255 );
    void CreateABGR( unsigned long colABGR );
    void CreateARGB( unsigned long colARGB );
    void Create( const wxString& colourName );

    // -----------------------------------------------------------------------
    // Set the colour values of the refed data.
    //   Use these to adjust the values of all the refed copies.
    void Set( const wxGenericColour &c );
    void Set( const wxColour& c );
    void Set( unsigned char red,  unsigned char green,
              unsigned char blue, unsigned char alpha=255 );
    void SetABGR( unsigned long colABGR );
    void SetARGB( unsigned long colARGB );
    void Set( const wxString& colourName );

    void SetRed  ( unsigned char r );
    void SetGreen( unsigned char g );
    void SetBlue ( unsigned char b );
    void SetAlpha( unsigned char a );

    // -----------------------------------------------------------------------
    // Get the colour values
    inline wxColour GetColour() const { return wxColour(GetRed(), GetGreen(), GetBlue()); }

    unsigned char GetRed() const;
    unsigned char GetGreen() const;
    unsigned char GetBlue() const;
    unsigned char GetAlpha() const;

    // wxWidgets wxColour compatibility functions
    unsigned char Red() const   { return GetRed(); }
    unsigned char Green() const { return GetGreen(); }
    unsigned char Blue() const  { return GetBlue(); }
    unsigned char Alpha() const { return GetAlpha(); }

    // -----------------------------------------------------------------------
    // Equivalency tests
    bool IsSameAs( const wxGenericColour& c ) const;
    bool IsSameAs( const wxColour& c ) const;

    // Get a "hot to cold" colour where i ranges from 0 to 255
    wxGenericColour GetHotColdColour(double i) const;

    // Read colour to and from string
    //wxString WriteString(const wxString& format = wxT("%d,%d,%d,%d")) const;
    //bool ReadString(const wxString& str, const wxString& format = wxT("%d,%d,%d,%d"));

    // operators
    bool operator == (const wxGenericColour& c) const
        { return m_refData == c.m_refData; }
    bool operator != (const wxGenericColour& c) const
        { return m_refData != c.m_refData; }

    wxGenericColour& operator = (const wxGenericColour& c)
    {
        if ( (*this) != c )
            Ref(c);
        return *this;
    }
    wxGenericColour& operator = ( const wxColour& c ) { Create(c); return *this; }

private:
    // ref counting code
    virtual wxObjectRefData *CreateRefData() const;
    virtual wxObjectRefData *CloneRefData(const wxObjectRefData *data) const;

    DECLARE_DYNAMIC_CLASS(wxGenericColour)
};

//----------------------------------------------------------------------------
// wxGenericPen - a pen structure that converts to and from wxPen
//    You can have thousands of them without using up resources. (on MSW)
//----------------------------------------------------------------------------

#if !wxCHECK_VERSION(2, 9, 0)
/* Backwards compatibility */
#define wxPenCap int
#define wxPenJoin int
#endif  /* wxCHECK_VERSION */

class WXDLLIMPEXP_THINGS wxGenericPen : public wxObject
{
public:
    wxGenericPen() : wxObject() {}
    wxGenericPen( const wxGenericPen &pen ) : wxObject() { Create(pen); }
    wxGenericPen( const wxPen &pen ) : wxObject() { Create(pen); }
    wxGenericPen( const wxGenericColour &colour, int width = 1, wxPenStyle style = wxPENSTYLE_SOLID,
                  wxPenCap cap = wxCAP_ROUND, wxPenJoin join = wxJOIN_ROUND ) : wxObject()
        { Create(colour, width, style, cap, join); }
    wxGenericPen( const wxColour &colour, int width = 1, wxPenStyle style = wxPENSTYLE_SOLID,
                  wxPenCap cap = wxCAP_ROUND, wxPenJoin join = wxJOIN_ROUND ) : wxObject()
        { Create(colour, width, style, cap, join); }
    virtual ~wxGenericPen() {}
    // Destroy the refed data
    void Destroy() { UnRef(); }
    // Is this colour valid, has refed data
    bool Ok() const { return m_refData != NULL; }

    // -----------------------------------------------------------------------
    // (re)Create this pen, unrefing this pen first.
    //    Use these to detach this pen from it's refed copies.
    void Create( const wxGenericPen &pen );
    void Create( const wxPen &pen );
    void Create( const wxGenericColour &colour, int width = 1, wxPenStyle style = wxPENSTYLE_SOLID,
                wxPenCap cap = wxCAP_ROUND, wxPenJoin join = wxJOIN_ROUND );
    void Create( const wxColour &colour, int width = 1, wxPenStyle style = wxPENSTYLE_SOLID,
                wxPenCap cap = wxCAP_ROUND, wxPenJoin join = wxJOIN_ROUND );

    // -----------------------------------------------------------------------
    // Set the values of the refed data.
    //   Use these to adjust the values of all the refed copies.
    void Set( const wxGenericPen& pen );
    void Set( const wxPen& pen );
    void SetColour( const wxGenericColour &colour );
    void SetColour( const wxColour &colour );
    void SetColour( int red, int green, int blue, int alpha=255 );
    void SetCap( wxPenCap capStyle );
    void SetJoin( wxPenJoin joinStyle );
    void SetStyle( wxPenStyle style );
    void SetWidth( int width );
    void SetDashes( int number_of_dashes, const wxDash *dash );

    // -----------------------------------------------------------------------
    // Get the pen values
    wxPen GetPen() const;

    wxGenericColour GetGenericColour() const;
    wxColour GetColour() const;
    int GetWidth() const;
    wxPenStyle GetStyle() const;
    wxPenCap GetCap() const;
    wxPenJoin GetJoin() const;
    int GetDashes(wxDash **ptr) const;
    int GetDashCount() const;
    wxDash* GetDash() const;

    // -----------------------------------------------------------------------
    // Equivalency tests
    bool IsSameAs(const wxGenericPen& pen) const;
    bool IsSameAs(const wxPen& pen) const;

    // Read pen to and from string
    //wxString WriteString() const;
    //bool ReadString(const wxString& str);

    // operators
    bool operator == (const wxGenericPen& pen) const
        { return m_refData == pen.m_refData; }
    bool operator != (const wxGenericPen& pen) const
        { return m_refData != pen.m_refData; }

    wxGenericPen& operator = (const wxGenericPen& pen)
    {
        if ( (*this) != pen )
            Ref(pen);
        return *this;
    }
    wxGenericPen& operator = ( const wxPen& pen ) { Create(pen); return *this; }

private:
    // ref counting code
    virtual wxObjectRefData *CreateRefData() const;
    virtual wxObjectRefData *CloneRefData(const wxObjectRefData *data) const;

    DECLARE_DYNAMIC_CLASS(wxGenericPen)
};

//----------------------------------------------------------------------------
// wxGenericBrush - a brush structure that converts to and from wxBrush
//    You can have thousands of them without using up resources. (on MSW)
//----------------------------------------------------------------------------

class WXDLLIMPEXP_THINGS wxGenericBrush : public wxObject
{
public:
    wxGenericBrush() : wxObject() {}
    wxGenericBrush( const wxGenericBrush &brush ) : wxObject() { Create(brush); }
    wxGenericBrush( const wxBrush &brush ) : wxObject() { Create(brush); }
    wxGenericBrush( const wxGenericColour &colour, wxBrushStyle style = wxBRUSHSTYLE_SOLID)
        : wxObject() { Create(colour, style); }
    wxGenericBrush( const wxColour &colour, wxBrushStyle style = wxBRUSHSTYLE_SOLID)
        : wxObject() { Create(colour, style); }
    wxGenericBrush( const wxBitmap& stipple ) : wxObject() { Create(stipple); }

    virtual ~wxGenericBrush() {}
    // Destroy the refed data
    void Destroy() { UnRef(); }
    // Is this colour valid, has refed data
    bool Ok() const { return m_refData != NULL; }

    // -----------------------------------------------------------------------
    // (re)Create this brush, unrefing this pen first.
    //    Use these to detach this pen from it's refed copies.
    void Create( const wxGenericBrush &brush );
    void Create( const wxBrush &brush );
    void Create( const wxGenericColour &colour, wxBrushStyle style = wxBRUSHSTYLE_SOLID );
    void Create( const wxColour &colour, wxBrushStyle style = wxBRUSHSTYLE_SOLID );
    void Create( const wxBitmap &stipple );

    // -----------------------------------------------------------------------
    // Set the values of the refed data.
    //   Use these to adjust the values of all the refed copies.
    void Set( const wxGenericBrush& brush );
    void Set( const wxBrush& brush );
    void SetColour( const wxGenericColour &colour );
    void SetColour( const wxColour &colour );
    void SetColour( int red, int green, int blue, int alpha=255 );
    void SetStyle( wxBrushStyle style );
    void SetStipple( const wxBitmap& stipple );

    // -----------------------------------------------------------------------
    // Get the brush values
    wxBrush GetBrush() const;

    wxGenericColour GetGenericColour() const;
    wxColour GetColour() const;
    wxBrushStyle GetStyle() const;
    wxBitmap* GetStipple() const;

    bool IsHatch() const
        { return (GetStyle()>=wxBRUSHSTYLE_FIRST_HATCH) && (GetStyle()<=wxBRUSHSTYLE_LAST_HATCH); }

    // -----------------------------------------------------------------------
    // Equivalency tests
    bool IsSameAs(const wxGenericBrush& brush) const;
    bool IsSameAs(const wxBrush& brush) const;

    // operators
    bool operator == (const wxGenericBrush& brush) const
        { return m_refData == brush.m_refData; }
    bool operator != (const wxGenericBrush& brush) const
        { return m_refData != brush.m_refData; }

    wxGenericBrush& operator = (const wxGenericBrush& brush)
    {
        if ( (*this) != brush )
            Ref(brush);
        return *this;
    }
    wxGenericBrush& operator = ( const wxBrush& brush ) { Create(brush); return *this; }

private:
    // ref counting code
    virtual wxObjectRefData *CreateRefData() const;
    virtual wxObjectRefData *CloneRefData(const wxObjectRefData *data) const;

    DECLARE_DYNAMIC_CLASS(wxGenericBrush)
};

#endif // _WX_GENERGDI_H_
