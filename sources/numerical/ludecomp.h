//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Interface to LU decomposition classes
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

#ifndef LUDECOMP_H
#define LUDECOMP_H

#include "base/base.h"
#include "math/gvector.h"
#include "math/gmatrix.h"
#include "basis.h"

template <class T> class Tableau;

// ---------------------------------------------------------------------------
// Class EtaMatrix
// ---------------------------------------------------------------------------

template <class T> class EtaMatrix {
  public:
  int col;
  gbtVector<T> etadata;
  
  
  EtaMatrix(int c, gbtVector<T> &v) : col(c), etadata(v) {};

// required for list class
bool operator==(const EtaMatrix<T> &) const;
bool operator!=(const EtaMatrix<T> &) const;

};

template <class T> gbtOutput &operator<<( gbtOutput &, const EtaMatrix<T> &);

// ---------------------------------------------------------------------------
// Class LUdecomp
// ---------------------------------------------------------------------------

template <class T> class LUdecomp {

private:

  Tableau<T> &tab;
  Basis &basis;

  gbtList< EtaMatrix<T> > L;
  gbtList< EtaMatrix<T> > U;
  gbtList< EtaMatrix<T> > E;
  gbtList< int > P;

  gbtVector<T> scratch1; // scratch vectors so we don't reallocate them
  gbtVector<T> scratch2; // everytime we do something.

  int refactor_number;
  int iterations;
  int total_operations;

  const LUdecomp<T> *parent;
  int copycount;

public:
  class BadDim : public gbtException  {
  public:
    virtual ~BadDim();
    gbtText Description(void) const;
  };
  class BadPivot : public gbtException  {
  public:
    virtual ~BadPivot();
    gbtText Description(void) const;
  };
  class BadCount : public gbtException  {
  public:
    virtual ~BadCount();
    gbtText Description(void) const;
  };

  // ------------------------
  // Constructors, Destructor
  // ------------------------
    

  // don't use this copy constructor
  LUdecomp( const LUdecomp<T> &a) : tab(a.tab), basis(a.basis) { assert(0); };

  // copy constructor
  // note:  Copying will fail an assertion if you try to update or delete
  //        the original before the copy has been deleted, refactored
  //        Or set to something else.
  LUdecomp( const LUdecomp<T> &, Tableau<T> & );

  // Decompose given matrix
  LUdecomp( Tableau<T> &, int rfac = 0 ); 

  // Destructor
  ~LUdecomp();

  // don't use the equals operator, use the Copy function instead
  LUdecomp<T>& operator=(const LUdecomp<T>&) { assert(0); return *this; };


  // --------------------
  // Public Members
  // --------------------

  // copies the LUdecomp given (expect for the basis &).
  void Copy( const LUdecomp<T> &, Tableau<T> & );

  // replace (update) the column given with the vector given.
  void update( int, int matcol ); // matcol is the column number in the matrix

  // refactor 
  void refactor();
  
  // solve: Bk d = a
  void solve (const gbtVector<T> &, gbtVector<T> & ) const;

  // solve: y Bk = c
  void solveT( const gbtVector<T> &, gbtVector <T> & ) const;

  // set number of etamatrices added before refactoring;
  // if number is set to zero, refactoring is done automatically.
  // if number is < 0, no refactoring is done;
  void SetRefactor( int );

  //-------------------
  // Private Members
  //-------------------

private:
  
  void FactorBasis();

  void GaussElem( gbtMatrix<T> &, int, int );

  bool CheckBasis();
  bool RefactorCheck();

  void BTransE( gbtVector<T> & ) const;
  void FTransE( gbtVector<T> & ) const;
  void BTransU( gbtVector<T> & ) const;
  void FTransU( gbtVector<T> & ) const;
  void LPd_Trans( gbtVector<T> & ) const;
  void yLP_Trans( gbtVector<T> & ) const;

  void VectorEtaSolve( const gbtVector<T> &v,  
		      const EtaMatrix<T> &, 
		      gbtVector<T> &y ) const;

  void EtaVectorSolve( const gbtVector<T> &v, 
		      const EtaMatrix<T> &,
		      gbtVector<T> &d ) const;

  void yLP_mult( const gbtVector<T> &y, int j, gbtVector<T> &) const;

  void LPd_mult( gbtVector<T> &d, int j, gbtVector<T> &) const;


};  // end of class LUdecomp
    
#endif // LUDECOMP_H
