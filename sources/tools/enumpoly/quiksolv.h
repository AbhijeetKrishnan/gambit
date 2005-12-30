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

#include "libgambit/libgambit.h"
#include "odometer.h"
#include "gsolver.h"
#include "rectangl.h"
#include "gpoly.h"
#include "gpolylst.h"
#include "gpartltr.h"
#include "pelqhull.h"
#include "pelclass.h"

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
   The main constructor for this takes a gPolyList<T>.  The list must
be at least as long as the dimension Dmnsn() of the space of the
system.  The first Dmnsn() polynomials are interpreted as equations,
while remaining polynomials are interpreted as inequalities in the
sense that the polynomial is required to be nonnegative.
*/


// ***********************
//      class QuikSolv
// ***********************

template <class T> class QuikSolv {
 private:
  const gPolyList<T>                 System;
  const gPolyList<gDouble>           gDoubleSystem;
  const int                          NoEquations;
  const int                          NoInequalities;
  const ListOfPartialTrees<gDouble>  TreesOfPartials;
        bool                         HasBeenSolved;
        gbtList<gbtVector<gDouble> >     Roots;
  const bool                         isMultiaffine;
  const gbtRectArray<bool>             Equation_i_uses_var_j;

  // Supporting routines for the constructors

  gbtRectArray<bool> Eq_i_Uses_j()                                         const;

  // Get Roots Using Pelican

  bool AllRealRootsFromPelican(const gPolyList<gDouble> &, 
			             gbtList<gbtVector<gDouble> > &)         const;
  bool PelicanRoots(const gRectangle<T> &, 
		          gbtList<gbtVector<gDouble> > &)                    const;

  // Check whether roots are impossible

   bool SystemHasNoRootsIn(const gRectangle<gDouble>& r, gbtArray<int>&)   const;


  // Ask whether Newton's method leads to a root 

   bool NewtonRootInRectangle(  const gRectangle<gDouble>&, 
			              gbtVector<gDouble>&) const;
   bool NewtonRootNearRectangle(const gRectangle<gDouble>&, 
			              gbtVector<gDouble>&) const;


  // Ask whether we can prove that there is no root other than 
  // the one produced by the last step

   gDouble MaxDistanceFromPointToVertexAfterTransformation(
				      const gRectangle<gDouble>&,
				      const gbtVector<gDouble>&,
				      const Gambit::SquareMatrix<gDouble>&)     const;

   bool HasNoOtherRootsIn(const gRectangle<gDouble>&,
			  const gbtVector<gDouble>&,
			  const Gambit::SquareMatrix<gDouble>&)                 const;


  // Combine the last two steps into a single query

   bool NewtonRootIsOnlyInRct(const gRectangle<gDouble>&, 
			            gbtVector<gDouble>&) const;


  // Recursive parts of recursive methods

  void               FindRootsRecursion(      gbtList<gbtVector<gDouble> >*,
					const gRectangle<gDouble>&, 
					const int&,
					      gbtArray<int>&,
					      int &iterations,
					      int depth,
					const int&,
					      int*)                  const;

  const bool         ARootExistsRecursion(const gRectangle<gDouble>&, 
					        gbtVector<gDouble>&,
					  const gRectangle<gDouble>&, 
					        gbtArray<int>&)        const;

 public:
  class NewtonError : public gbtException  {
  public:
    virtual ~NewtonError();
    std::string GetDescription(void) const;   
  };
   QuikSolv(const gPolyList<T> &);  
   QuikSolv(const gPolyList<T> &, const int &);  
   QuikSolv(const QuikSolv<T> &);
   ~QuikSolv();

   // Operators
   QuikSolv<T>& operator= (const QuikSolv<T> &);
   bool         operator==(const QuikSolv<T> &) const;
   bool         operator!=(const QuikSolv<T> &) const;

   // Information
   inline const gSpace*                  AmbientSpace()              const 
     { return System.AmbientSpace(); }
   inline const term_order*              TermOrder()                 const 
     { return System.TermOrder(); }
   inline const int                      Dmnsn()                     const 
     { return System.Dmnsn(); }
   inline const gPolyList<T>             UnderlyingEquations()       const 
     { return System; }
   inline const bool                     WasSolved()                 const
     { return HasBeenSolved; }
   inline const gbtList<gbtVector<gDouble> > RootList()                  const
     { return Roots; }
   inline const bool                     IsMultiaffine()             const
     { return isMultiaffine; }

  // Refines the accuracy of roots obtained from other algorithms
  gbtVector<gDouble> NewtonPolishOnce(const gbtVector<gDouble> &)        const;
  gbtVector<gDouble> SlowNewtonPolishOnce(const gbtVector<gDouble> &)    const;
  gbtVector<gDouble> NewtonPolishedRoot(const gbtVector<gDouble> &)      const;

  // Checks for complex singular roots
   bool     MightHaveSingularRoots()                                 const;

  // The grand calculation - returns true if successful
   bool     FindCertainNumberOfRoots  (const gRectangle<T>&, 
				       const int&,
				       const int&);
   bool     FindRoots  (const gRectangle<T>&, const int&);
   bool     ARootExists (const gRectangle<T>&, gbtVector<gDouble>&)    const;
};  

#endif // QUIKSOLV_H
