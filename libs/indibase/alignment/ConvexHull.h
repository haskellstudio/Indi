/*!
 * \file ConvexHull.h
 *
 * \author Roger James
 * \date December 2013
 *
 */

#ifndef CONVEXHULL_H
#define CONVEXHULL_H

// This C++ code is based on the c code described below
// it was ported to c++ by Roger James in December 2013
// !!!!!!!!!!!!!!!!!!!!!!! IMPORTANT !!!!!!!!!!!!!!!!!!
// This must code must use integer coordinates. A naive conversion
// to floating point WILL NOT work. For the reasons behind this
// have a look at at section 4.3.5 of the O'Rourke book. For more
// information try http://www.mpi-inf.mpg.de/departments/d1/ClassroomExamples/
// For INDI alignment purposes we need to scale floating point coordinates
// into the integer space before using this algorithm.

/*
This code is described in "Computational Geometry in C" (Second Edition),
Chapter 4.  It is not written to be comprehensible without the
explanation in that book.

Input: 3n integer coordinates for the points.
Output: the 3D convex hull, in postscript with embedded comments
        showing the vertices and faces.

Compile: gcc -o chull chull.c (or simply: make)

Written by Joseph O'Rourke, with contributions by
  Kristy Anderson, John Kutcher, Catherine Schevon, Susan Weller.
Last modified: May 2000
Questions to orourke@cs.smith.edu.

--------------------------------------------------------------------
This code is Copyright 2000 by Joseph O'Rourke.  It may be freely
redistributed in its entirety provided that this copyright notice is
not removed.
--------------------------------------------------------------------
*/

#include <cstring> // I like to use NULL

namespace INDI {
namespace AlignmentSubsystem {

/*!
 * \class ConvexHull
 * \brief This class computes the convex hull of a set of 3d points.
 */
class ConvexHull
{
    public:
        ConvexHull() : vertices(NULL), edges(NULL), faces(NULL), debug(false), check(false) {}
        virtual ~ConvexHull() {}

    enum { X = 0, Y = 1, Z = 2 };

    template <class Type>
        static void add(Type& head, Type p)
        {
            if (NULL != head)
            {
                p->next = head;
                p->prev = head->prev;
                head->prev = p;
                p->prev->next = p;
            }
            else
            {
                head = p;
                head->next = head->prev = p;
            }
        };

    template <class Type>
        static void remove(Type& head, Type p)
        {
             if (NULL != head)
             {
				if ( head == head->next )
					head = NULL;
				else if ( p == head )
					head = head->next;
				p->next->prev = p->prev;
				p->prev->next = p->next;
				delete p;
            }
        };

    template <class Type>
        static void swap(Type& t, Type& x, Type& y)
        {
            t = x;
            x = y;
            y = t;
        };

    // Explicit forward declarations
    struct tVertexStructure;
    struct tFaceStructure;
    struct tEdgeStructure;

    /* Define structures for vertices, edges and faces */
    typedef struct tVertexStructure tsVertex;
    typedef tsVertex *tVertex;

    typedef struct tEdgeStructure tsEdge;
    typedef tsEdge *tEdge;

    typedef struct tFaceStructure tsFace;
    typedef tsFace *tFace;

    struct tVertexStructure {
       int      v[3];
       int	    vnum;
       tEdge    duplicate;  // pointer to incident cone edge (or NULL)
       bool     onhull;     // True iff point on hull.
       bool	    mark;       // True iff point already processed.
       tVertex  next, prev;
    };

    struct tEdgeStructure {
       tFace    adjface[2];
       tVertex  endpts[2];
       tFace    newface;    // pointer to incident cone face.
       bool     delete_it;  //  True iff edge should be delete.
       tEdge    next, prev;
    };

    struct tFaceStructure {
       tEdge    edge[3];
       tVertex  vertex[3];
       bool	    visible;    // True iff face visible from new point.
       tFace    next, prev;
    };

    /* Define flags */
    static const bool  ONHULL = true;
    static const bool REMOVED = true;
    static const bool VISIBLE = true;
    static const bool PROCESSED = true;
    static const int SAFE = 1000000;		/* Range of safe coord values. */

    tVertex vertices;
    tEdge edges;
    tFace faces;

    /** \brief AddOne is passed a vertex.  It first determines all faces visible from
    that point.  If none are visible then the point is marked as not
    onhull.  Next is a loop over edges.  If both faces adjacent to an edge
    are visible, then the edge is marked for deletion.  If just one of the
    adjacent faces is visible then a new face is constructed.
    */
    bool AddOne( tVertex p );

    /** \brief Checks that, for each face, for each i={0,1,2}, the [i]th vertex of
    that face is either the [0]th or [1]st endpoint of the [ith] edge of
    the face.
    */
    void CheckEndpts ( void );

    /** \brief CheckEuler checks Euler's relation, as well as its implications when
    all faces are known to be triangles.  Only prints positive information
    when debug is true, but always prints negative information.
    */
    void CheckEuler(int V, int E, int F );

    void Checks( void );

    /** \brief CleanEdges runs through the edge list and cleans up the structure.
    If there is a newface then it will put that face in place of the
    visible face and NULL out newface. It also deletes so marked edges.
    */
    void CleanEdges( void );

    /** \brief CleanFaces runs through the face list and deletes any face marked visible.
    */
    void CleanFaces( void );

    /** \brief CleanUp goes through each data structure list and clears all
    flags and NULLs out some pointers.  The order of processing
    (edges, faces, vertices) is important.
    */
    void CleanUp( tVertex *pvnext );

    /** \brief CleanVertices runs through the vertex list and deletes the
    vertices that are marked as processed but are not incident to any
    undeleted edges.
    The pointer to vnext, pvnext, is used to alter vnext in
    ConstructHull() if we are about to delete vnext.
    */
    void CleanVertices( tVertex *pvnext );

    /** \brief Collinear checks to see if the three points given are collinear,
    by checking to see if each element of the cross product is zero.
    */
    bool Collinear( tVertex a, tVertex b, tVertex c );

    /** \brief Consistency runs through the edge list and checks that all
    adjacent faces have their endpoints in opposite order.  This verifies
    that the vertices are in counterclockwise order.
    */
    void Consistency( void );

    /** \brief ConstructHull adds the vertices to the hull one at a time.  The hull
    vertices are those in the list marked as onhull.
    */
    void ConstructHull( void );

    /** \brief Convexity checks that the volume between every face and every
    point is negative.  This shows that each point is inside every face
    and therefore the hull is convex.
    */
    void Convexity( void );

    /** \brief DoubleTriangle builds the initial double triangle.  It first finds 3
     noncollinear points and makes two faces out of them, in opposite order.
     It then finds a fourth point that is not coplanar with that face.  The
     vertices are stored in the face structure in counterclockwise order so
     that the volume between the face and the point is negative. Lastly, the
     3 newfaces to the fourth point are constructed and the data structures
     are cleaned up.
    */
    void DoubleTriangle( void );

    /** \brief EdgeOrderOnFaces: puts e0 between v0 and v1, e1 between v1 and v2,
      e2 between v2 and v0 on each face.  This should be unnecessary, alas.
      Not used in code, but useful for other purposes.
    */
    void EdgeOrderOnFaces ( void );

    /** \brief MakeCcw puts the vertices in the face structure in counterclock wise
    order.  We want to store the vertices in the same
    order as in the visible face.  The third vertex is always p.

    Although no specific ordering of the edges of a face are used
    by the code, the following condition is maintained for each face f:
    one of the two endpoints of f->edge[i] matches f->vertex[i].
    But note that this does not imply that f->edge[i] is between
    f->vertex[i] and f->vertex[(i+1)%3].  (Thanks to Bob Williamson.)
    */
    void MakeCcw( tFace f, tEdge e, tVertex p );

    /** \brief MakeConeFace makes a new face and two new edges between the
    edge and the point that are passed to it. It returns a pointer to
    the new face.
    */
    tFace MakeConeFace( tEdge e, tVertex p );

    /** \brief MakeFace creates a new face structure from three vertices (in ccw
    order).  It returns a pointer to the face.
    */
    tFace MakeFace( tVertex v0, tVertex v1, tVertex v2, tFace f );

    /** \brief MakeNullEdge creates a new cell and initializes all pointers to NULL
    and sets all flags to off.  It returns a pointer to the empty cell.
    */
    tEdge MakeNullEdge( void );

    /** \brief MakeNullFace creates a new face structure and initializes all of its
    flags to NULL and sets all the flags to off.  It returns a pointer
    to the empty cell.
    */
    tFace MakeNullFace( void );

    /** \brief MakeNullVertex: Makes a vertex, nulls out fields.
    */
    tVertex	MakeNullVertex( void );

    /** \brief Print: Prints out the vertices and the faces.  Uses the vnum indices
    corresponding to the order in which the vertices were input.
    Output is in PostScript format. This code ignores the Z component of all vertices
    and does not scale the output to fit the page. It use on 3D hulls is not
    recommended.
    */
    void Print( void );

    void PrintEdges( void );

    void PrintFaces( void );

    void PrintObj( void );

    void PrintOut( tVertex v );

    void PrintPoint( tVertex p );

    void PrintVertices( void );

    /** \brief ReadVertices: Reads in the vertices, and links them into a circular
    list with MakeNullVertex.  There is no need for the # of vertices to be
    the first line: the function looks for EOF instead.  Sets the global
    variable vertices via the add<> template function.
    */
    void ReadVertices( void );

    /** \brief SubVec:  Computes a - b and puts it into c.
    */
    void SubVec( int a[3], int b[3], int c[3]);

    /** \brief Volumei returns the volume of the tetrahedron determined by f
    and p.
    */
    int Volumei( tFace f, tVertex p );

    /** \brief VolumeSign returns the sign of the volume of the tetrahedron determined by f
    and p.  VolumeSign is +1 iff p is on the negative side of f,
    where the positive side is determined by the rh-rule.  So the volume
    is positive if the ccw normal to f points outside the tetrahedron.
    The final fewer-multiplications form is due to Bob Williamson.
    */
    int VolumeSign(tFace f, tVertex p);

    private:
    bool debug;
    bool check;
};

} // namespace AlignmentSubsystem
} // namespace INDI

#endif // CONVEXHULL_H