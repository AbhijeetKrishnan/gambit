//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Interface to quick-solver classes
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

#ifndef QUIKSOLV_H
#define QUIKSOLV_H

#include "base/base.h"
#include "base/gstatus.h"
#include "base/odometer.h"
#include "gsolver.h"
#include "rectangl.h"
#include "gpoly.h"
#include "gpolylst.h"
#include "gpartltr.h"
#include "pelican/pelqhull.h"
#include "poly/pelclass.h"

class gbtNewtonPolishException : public gbtException  {
public:
  virtual ~gbtNewtonPolishException() { }
  std::string GetDescription(void) const 
  { return "Newton method failed to polish approximate root"; }
};

/*
    The (optimistically named) class described in this file is a method
of finding the roots of a system of polynomials and inequalities, with
equal numbers of equations and unknowns, that lie inside a given
rectangle.  The general idea is to first ask whether the Taylor's
series information at the center of the rectangle precludes the
existence of roots, and if it does not, whether Newton's method leads
to a root, and if it does, whether the Taylor's series information at
the root precludes the existence of another root.  If the roots in the
rectangle are not resolved by these queries, the rectangle is
subdivided into 2^d subrectangles, and the process is repeated on
each.  This continues until it has been shown that all roots have been
found, or a predetermined search depth is reached.  The bound on depth
is necessary because the procedure will not terminate if there are
singular roots.
*/

/*
   The main constructor for this takes a gbtPolyMultiList<T>.  The list must
be at least as long as the dimension Dmnsn() of the space of the
system.  The first Dmnsn() polynomials are interpreted as equations,
while remaining polynomials are interpreted as inequalities in the
sense that the gbtPolyUni is required to be nonnegative.
*/


// ***********************
//      class gbtPolyQuickSolve
// ***********************

template <class T> class gbtPolyQuickSolve {
 private:
  const gbtPolyMultiList<T>                 System;
  const gbtPolyMultiList<double>           gDoubleSystem;
  const int                          NoEquations;
  const int                          NoInequalities;
  const gbtPolyPartialTreeList<double>  TreesOfPartials;
        bool                         HasBeenSolved;
        gbtList<gbtVector<double> >     Roots;
  const bool                         isMultiaffine;
  const gbtRectArray<bool>             Equation_i_uses_var_j;
  gbtStatus &m_status;

  // Supporting routines for the constructors

  gbtRectArray<bool> Eq_i_Uses_j()                                         const;

  // Get Roots Using Pelican

  bool AllRealRootsFromPelican(const gbtPolyMultiList<double> &, 
			             gbtList<gbtVector<double> > &)         const;
  bool PelicanRoots(const gRectangle<T> &, 
		          gbtList<gbtVector<double> > &)                    const;

  // Check whether roots are impossible

   bool SystemHasNoRootsIn(const gRectangle<double>& r, gbtArray<int>&)   const;


  // Ask whether Newton's method leads to a root 

   bool NewtonRootInRectangle(  const gRectangle<double>&, 
			              gbtVector<double>&) const;
   bool NewtonRootNearRectangle(const gRectangle<double>&, 
			              gbtVector<double>&) const;


  // Ask whether we can prove that there is no root other than 
  // the one produced by the last step

   double MaxDistanceFromPointToVertexAfterTransformation(
				      const gRectangle<double>&,
				      const gbtVector<double>&,
				      const gbtSquareMatrix<double>&)     const;

   bool HasNoOtherRootsIn(const gRectangle<double>&,
			  const gbtVector<double>&,
			  const gbtSquareMatrix<double>&)                 const;


  // Combine the last two steps into a single query

   bool NewtonRootIsOnlyInRct(const gRectangle<double>&, 
			            gbtVector<double>&) const;


  // Recursive parts of recursive methods

  void               FindRootsRecursion(      gbtList<gbtVector<double> >*,
					const gRectangle<double>&, 
					const int&,
					      gbtArray<int>&,
					      int&,
					const int&,
					      int*)                  const;

  const bool         ARootExistsRecursion(const gRectangle<double>&, 
					        gbtVector<double>&,
					  const gRectangle<double>&, 
					        gbtArray<int>&)        const;

 public:
   gbtPolyQuickSolve(const gbtPolyMultiList<T> &, gbtStatus &);  
   gbtPolyQuickSolve(const gbtPolyMultiList<T> &, const int &, gbtStatus &);  
   gbtPolyQuickSolve(const gbtPolyQuickSolve<T> &);
   ~gbtPolyQuickSolve();

   // Operators
   gbtPolyQuickSolve<T>& operator= (const gbtPolyQuickSolve<T> &);
   bool         operator==(const gbtPolyQuickSolve<T> &) const;
   bool         operator!=(const gbtPolyQuickSolve<T> &) const;

   // Information
   inline const gbtPolySpace*                  AmbientSpace()              const 
     { return System.AmbientSpace(); }
   inline const gbtPolyTermOrder*              TermOrder()                 const 
     { return System.TermOrder(); }
   inline const int                      Dmnsn()                     const 
     { return System.Dmnsn(); }
   inline const gbtPolyMultiList<T>             UnderlyingEquations()       const 
     { return System; }
   inline const bool                     WasSolved()                 const
     { return HasBeenSolved; }
   inline const gbtList<gbtVector<double> > RootList()                  const
     { return Roots; }
   inline const bool                     IsMultiaffine()             const
     { return isMultiaffine; }

  // Refines the accuracy of roots obtained from other algorithms
  gbtVector<double> NewtonPolishOnce(const gbtVector<double> &)        const;
  gbtVector<double> SlowNewtonPolishOnce(const gbtVector<double> &)    const;
  gbtVector<double> NewtonPolishedRoot(const gbtVector<double> &)      const;

  // Checks for complex singular roots
   bool     MightHaveSingularRoots()                                 const;

  // The grand calculation - returns true if successful
   bool     FindCertainNumberOfRoots  (const gRectangle<double>&, 
				       const int&,
				       const int&);
   bool     FindRoots  (const gRectangle<double>&, const int&);
   bool     ARootExists (const gRectangle<double>&, gbtVector<double>&)    const;
};  

template <class T> std::ostream &operator<<(std::ostream &output, const gbtPolyQuickSolve<T> &);

#endif // QUIKSOLV_H
