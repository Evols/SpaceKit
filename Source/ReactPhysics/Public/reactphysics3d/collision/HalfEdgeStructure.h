/********************************************************************************
* ReactPhysics3D physics library, http://www.reactphysics3d.com                 *
* Copyright (c) 2010-2020 Daniel Chappuis                                       *
*********************************************************************************
*                                                                               *
* This software is provided 'as-is', without any express or implied warranty.   *
* In no event will the authors be held liable for any damages arising from the  *
* use of this software.                                                         *
*                                                                               *
* Permission is granted to anyone to use this software for any purpose,         *
* including commercial applications, and to alter it and redistribute it        *
* freely, subject to the following restrictions:                                *
*                                                                               *
* 1. The origin of this software must not be misrepresented; you must not claim *
*    that you wrote the original software. If you use this software in a        *
*    product, an acknowledgment in the product documentation would be           *
*    appreciated but is not required.                                           *
*                                                                               *
* 2. Altered source versions must be plainly marked as such, and must not be    *
*    misrepresented as being the original software.                             *
*                                                                               *
* 3. This notice may not be removed or altered from any source distribution.    *
*                                                                               *
********************************************************************************/

#ifndef REACTPHYSICS3D_HALF_EDGE_STRUCTURE_MESH_H
#define REACTPHYSICS3D_HALF_EDGE_STRUCTURE_MESH_H

// Libraries
#include <reactphysics3d/mathematics/mathematics.h>

namespace reactphysics3d {

// Class HalfEdgeStructure
/**
 * This class describes a polyhedron mesh made of faces and vertices.
 * The faces do not have to be triangle. Note that the half-edge structure
 * is only valid if the mesh is closed (each edge has two adjacent faces).
 */
class HalfEdgeStructure {

    public:

        using VerticesPair = Pair<uint8, uint8>;

        /// Edge
        struct Edge {
            uint8 vertexIndex;       // Index of the vertex at the beginning of the edge
            uint8 twinEdgeIndex;     // Index of the twin edge
            uint8 faceIndex;         // Adjacent face index of the edge
            uint8 nextEdgeIndex;     // Index of the next edge
        };

        /// Face
        struct Face {
            uint8 edgeIndex;             // Index of an half-edge of the face
            List<uint8> faceVertices;	// Index of the vertices of the face

            /// Constructor
            Face(MemoryAllocator& allocator) : faceVertices(allocator) {}

            /// Constructor
            Face(List<uint8> vertices) : faceVertices(vertices) {}
        };

        /// Vertex
        struct Vertex {
            uint8 vertexPointIndex;  // Index of the vertex point in the origin vertex array
            uint8 edgeIndex;         // Index of one edge emanting from this vertex

            /// Constructor
            Vertex(uint8 vertexCoordsIndex) : vertexPointIndex(vertexCoordsIndex) { }
        };

    private:

        /// Reference to a memory allocator
        MemoryAllocator& mAllocator;

        /// All the faces
        List<Face> mFaces;

        /// All the vertices
        List<Vertex> mVertices;

        /// All the half-edges
        List<Edge> mEdges;

    public:

        /// Constructor
        HalfEdgeStructure(MemoryAllocator& allocator, uint8 facesCapacity, uint8 verticesCapacity,
                          uint8 edgesCapacity) :mAllocator(allocator), mFaces(allocator, facesCapacity),
                          mVertices(allocator, verticesCapacity), mEdges(allocator, edgesCapacity) {}

        /// Destructor
        ~HalfEdgeStructure() = default;

        /// Initialize the structure (when all vertices and faces have been added)
        void init();

        /// Add a vertex
        uint8 addVertex(uint8 vertexPointIndex);

        /// Add a face
        void addFace(List<uint8> faceVertices);

        /// Return the number of faces
        uint8 getNbFaces() const;

        /// Return the number of half-edges
        uint8 getNbHalfEdges() const;

        /// Return the number of vertices
        uint8 getNbVertices() const;

        /// Return a given face
        const Face& getFace(uint8 index) const;

        /// Return a given edge
        const Edge& getHalfEdge(uint8 index) const;

        /// Return a given vertex
        const Vertex& getVertex(uint8 index) const;

};

// Add a vertex
/**
 * @param vertexPointIndex Index of the vertex in the vertex data array
 */
inline uint8 HalfEdgeStructure::addVertex(uint8 vertexPointIndex) {
    Vertex vertex(vertexPointIndex);
    mVertices.add(vertex);
    return mVertices.size() - 1;
}

// Add a face
/**
 * @param faceVertices List of the vertices in a face (ordered in CCW order as seen from outside
 *                     the polyhedron
 */
inline void HalfEdgeStructure::addFace(List<uint8> faceVertices) {

    // Create a new face
    Face face(faceVertices);
    mFaces.add(face);
}

// Return the number of faces
/**
 * @return The number of faces in the polyhedron
 */
inline uint8 HalfEdgeStructure::getNbFaces() const {
    return static_cast<uint8>(mFaces.size());
}

// Return the number of edges
/**
 * @return The number of edges in the polyhedron
 */
inline uint8 HalfEdgeStructure::getNbHalfEdges() const {
    return static_cast<uint8>(mEdges.size());
}

// Return the number of vertices
/**
 * @return The number of vertices in the polyhedron
 */
inline uint8 HalfEdgeStructure::getNbVertices() const {
    return static_cast<uint8>(mVertices.size());
}

// Return a given face
/**
 * @return A given face of the polyhedron
 */
inline const HalfEdgeStructure::Face& HalfEdgeStructure::getFace(uint8 index) const {
    assert(index < mFaces.size());
    return mFaces[index];
}

// Return a given edge
/**
 * @return A given edge of the polyhedron
 */
inline const HalfEdgeStructure::Edge& HalfEdgeStructure::getHalfEdge(uint8 index) const {
    assert(index < mEdges.size());
    return mEdges[index];
}

// Return a given vertex
/**
 * @return A given vertex of the polyhedron
 */
inline const HalfEdgeStructure::Vertex& HalfEdgeStructure::getVertex(uint8 index) const {
    assert(index < mVertices.size());
    return mVertices[index];
}

}

#endif

