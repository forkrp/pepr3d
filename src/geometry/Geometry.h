#pragma once

// #include <CGAL/IO/Color.h>
#include <vector>
#include <cassert>

// #include "Triangle.h" // Uses CGAL

// using Color = CGAL::Color;

struct PeprColor // Will be changed for CGAL::Color when CGAL is operational
{
    float rgb[3];

    float r() const { return rgb[0]; }
    float g() const { return rgb[1]; }
    float b() const { return rgb[2]; }
};


struct DataTriangle // Will be changed for DataTriangle from Triangle.h after CGAL
{
    glm::vec3 vertices[3];

    PeprColor color;

    void setColor(PeprColor col)
    {
        color = col;
    }

    DataTriangle(glm::vec3 x, glm::vec3 y, glm::vec3 z)
    {
        vertices[0] = x;
        vertices[1] = y;
        vertices[2] = z;
        PeprColor red;
        red.rgb[0] = 1;
        red.rgb[1] = 0;
        red.rgb[2] = 0;
        color = red;
    }
};

class Geometry {
    /// Triangle soup of the model mesh, containing CGAL::Triangle_3 data for AABB tree.
    std::vector<DataTriangle> mTriangles;

    /// Vertex buffer with the same data as mTriangles for OpenGL to render the mesh.
    /// Contains position and color data for each vertex.
    std::vector<glm::vec3> mVertexBuffer;

    /// Index buffer for OpenGL frontend., specifying the same triangles as in mTriangles.
    std::vector<uint32_t> mIndexBuffer;

   public:
    /// Empty constructor rendering a triangle to debug
    Geometry()
    {
        mTriangles.emplace_back(glm::vec3(-1,-1,0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));        

        generateVertexBuffer();
        generateIndexBuffer();

        assert(mIndexBuffer.size() == mVertexBuffer.size());
        assert(mVertexBuffer.size() == 3);
    }

    /// Returns a constant iterator to the vertex buffer
    std::vector<glm::vec3> getVertexBuffer() const {
        return mVertexBuffer;
    }

    /// Returns a constant iterator to the index buffer
    std::vector<uint32_t> getIndexBuffer() const {
        return mIndexBuffer;
    }

    /// Loads new geometry into the private data, rebuilds the vertex and index buffers
    /// automatically.
    void loadNewGeometry() {
        /// Load into mTriangles
        // TODO: Loading code goes here, just load the mTriangles, everything else is automatic

        /// Generate new vertex buffer
        generateVertexBuffer();

        /// Generate new index buffer
        generateIndexBuffer();
    }

    /// Set new triangle color
    void setTriangleColor(const size_t triangleIndex, const PeprColor newColor)
    {
        // Vertex buffer has 6 floats for each vertex, each triangle has 3 vertices
        const size_t vertexPosition = triangleIndex * 6 * 3;
        assert(mVertexBuffer.size() < vertexPosition + 5);
        
        const size_t rPosition = vertexPosition + 3;
        const size_t gPosition = vertexPosition + 4;
        const size_t bPosition = vertexPosition + 5;

        /*mVertexBuffer[rPosition] = newColor.r();
        mVertexBuffer[gPosition] = newColor.g();
        mVertexBuffer[bPosition] = newColor.b();*/

        assert(triangleIndex < mTriangles.size());
        mTriangles[triangleIndex].setColor(newColor);
    }

   private:
    void generateVertexBuffer()
    {
        mVertexBuffer.clear();
        mVertexBuffer.reserve(3 * mTriangles.size());

        for(size_t i = 0; i < mTriangles.size(); ++i)
        {
            mVertexBuffer.emplace_back(mTriangles[i].vertices[0].x, mTriangles[i].vertices[0].y, mTriangles[i].vertices[0].z);           
            mVertexBuffer.emplace_back(mTriangles[i].vertices[1].x, mTriangles[i].vertices[1].y, mTriangles[i].vertices[1].z);           
            mVertexBuffer.emplace_back(mTriangles[i].vertices[2].x, mTriangles[i].vertices[2].y, mTriangles[i].vertices[2].z);           
        }
    }

    void generateIndexBuffer()
    {
        mIndexBuffer.clear();
        mIndexBuffer.reserve(mVertexBuffer.size());

        for(size_t i = 0; i < mVertexBuffer.size(); ++i)
        {
            mIndexBuffer.push_back(i);
        }
    }
};
