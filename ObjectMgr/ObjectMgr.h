#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <strstream>

struct vec2D {
    float u = 0;
    float v = 0;
};

struct vec3D {
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 1; // Need a 4th term for matrix vector multiplication
};

struct Triangle {
    vec3D t[3];
    // texture coordinates
    vec2D tc[3];

    sf::Color triColor;

    void setColor(sf::Color& c)
    {
        triColor = c;
    }

    sf::Color getColor() {
        return triColor;
    }
};

struct Mesh {
    std::vector<Triangle> tris;
    sf::Vector3f position;

    sf::Vector3f getPosition() {
        return position;
    }

    void setPosition(sf::Vector3f& newPosition) {
        position = newPosition;
    }

    bool loadFromObjFile(std::string sFilename)
    {
        std::ifstream f(sFilename);
        if (!f.is_open())
        {
            std::cout << "File wasn't found" << std::endl;
            return false;

        }

        // Local cache of verts
        std::vector<vec3D> verts;

        while (!f.eof())
        {
            char line[128];
            f.getline(line, 128);

            std::strstream s;
            s << line;

            char junk;

            if (line[0] == 'v')
            {
                vec3D v;
                s >> junk >> v.x >> v.y >> v.z;
                verts.push_back(v);
            }

            if (line[0] == 'f')
            {
                int f[3];
                s >> junk >> f[0] >> f[1] >> f[2];

                // All of the info in an obj file starts counting from 1, not 0 (therefore - 1)
                tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
            }
        }

        return true;
    }
};

struct Drawing {
    std::vector<Triangle> drawing;

    void saveDrawing(std::vector<Triangle>& triToRaster) {
        if (!triToRaster.empty())
        {
            drawing = triToRaster;
        }
    }

    void loadDrawing(std::vector<Triangle>& triToRaster) {
        triToRaster = drawing;
    }

    void addToMesh(std::vector<Triangle>& meshTris) {
        meshTris.insert(meshTris.end(), drawing.begin(), drawing.end());
    }

    void clearDrawing() {
        drawing.clear();
    }

    void storeDrawingToFile(Mesh& mesh) {

    }


    
};

struct Map {
    std::vector<Drawing> Drawings;
};


class ObjectMgr {
public:
    std::vector<Triangle> vecTrianglesToRaster;


    ObjectMgr(Mesh& mesh) {
        setStartingPos(mesh);
    }

    sf::Vector3f getPosition(Mesh& mesh) {
        return mesh.getPosition();
    }

    void handleTriangles(sf::VertexArray& vertex, Triangle& triProjected)
    {
        sf::Color black(13, 41, 74);

        // Draw Triangles
        for (int i = 0; i < 3; i++)
        {
            vertex[i].position = sf::Vector2f(triProjected.t[i].x, triProjected.t[i].y);
            vertex[i].color = black;
        }

    }

    void addToMeshPos(Mesh& mesh, sf::Vector3f& newPos) {
        sf::Vector3f UpdatedVec;
        sf::Vector3f oldPos = mesh.getPosition();
        UpdatedVec = sf::Vector3f(oldPos.x + newPos.x, oldPos.y + newPos.y, oldPos.z + newPos.z);
        mesh.setPosition(UpdatedVec);
    }

    void setMeshNewPos(Mesh& mesh, sf::Vector3f& newPos) {
        mesh.setPosition(newPos);
    }

    void handleWireFrame(sf::VertexArray& vertex, Triangle& triProjected)
    {
        // Draw Wireframes
        sf::Color wireframeColor(252, 252, 63);

        for (int i = 0; i < 3; i++)
        {
            int nextIndex = (i + 1) % 3;
            vertex[i * 2].position = sf::Vector2f(triProjected.t[i].x, triProjected.t[i].y);
            vertex[i * 2 + 1].position = sf::Vector2f(triProjected.t[nextIndex].x, triProjected.t[nextIndex].y);

            vertex[i * 2].color = wireframeColor;
            vertex[i * 2 + 1].color = wireframeColor;
        }

    }

    void createCubeMesh(Mesh& m)
    {
        m.tris = {
            // SOUTH
           {vec3D{0.0f, 0.0f, 0.0f},     vec3D{0.0f, 1.0f, 0.0f},     vec3D{1.0f, 1.0f, 0.0f}, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,},
           {vec3D{0.0f, 0.0f, 0.0f},     vec3D{1.0f, 1.0f, 0.0f},     vec3D{1.0f, 0.0f, 0.0f}, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,},

           // EAST
          {vec3D{1.0f, 0.0f, 0.0f},     vec3D{1.0f, 1.0f, 0.0f},     vec3D{1.0f, 1.0f, 1.0f}, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,},
          {vec3D{1.0f, 0.0f, 0.0f},     vec3D{1.0f, 1.0f, 1.0f},     vec3D{1.0f, 0.0f, 1.0f}, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,},

          // NORTH
         {vec3D{1.0f, 0.0f, 1.0f},     vec3D{1.0f, 1.0f, 1.0f},     vec3D{0.0f, 1.0f, 1.0f}, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,},
         {vec3D{1.0f, 0.0f, 1.0f},     vec3D{0.0f, 1.0f, 1.0f},     vec3D{0.0f, 0.0f, 1.0f}, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,},

         //// WEST
        {vec3D{0.0f, 0.0f, 1.0f},     vec3D{0.0f, 1.0f, 1.0f},     vec3D{0.0f, 1.0f, 0.0f}, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,},
        {vec3D{0.0f, 0.0f, 1.0f},     vec3D{0.0f, 1.0f, 0.0f},     vec3D{0.0f, 0.0f, 0.0f}, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,},

        //// TOP
       {vec3D{0.0f, 1.0f, 0.0f},     vec3D{0.0f, 1.0f, 1.0f},     vec3D{1.0f, 1.0f, 1.0f}, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,},
       {vec3D{0.0f, 1.0f, 0.0f},     vec3D{1.0f, 1.0f, 1.0f},     vec3D{1.0f, 1.0f, 0.0f}, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,},

       //// BOTTOM
      {vec3D{1.0f, 0.0f, 1.0f},     vec3D{0.0f, 0.0f, 1.0f},     vec3D{0.0f, 0.0f, 0.0f}, 0.01, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,},
      {vec3D{1.0f, 0.0f, 1.0f},     vec3D{0.0f, 0.0f, 0.0f},     vec3D{1.0f, 0.0f, 0.0f}, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,},


        };
    }

    void createPlaneMesh(Mesh& m)
    {
        m.tris = {
            // SOUTH
            {vec3D{0.0f, 0.0f, 0.0f},     vec3D{0.0f, 0.1f, 0.0f},     vec3D{1.0f, 0.1f, 0.0f}},
            {vec3D{0.0f, 0.0f, 0.0f},     vec3D{1.0f, 0.1f, 0.0f},     vec3D{1.0f, 0.0f, 0.0f}},

            // EAST
            {vec3D{1.0f, 0.0f, 0.0f},     vec3D{1.0f, 0.1f, 0.0f},     vec3D{1.0f, 0.1f, 1.0f}},
            {vec3D{1.0f, 0.0f, 0.0f},     vec3D{1.0f, 0.1f, 1.0f},     vec3D{1.0f, 0.0f, 1.0f}},

            // NORTH
            {vec3D{1.0f, 0.0f, 1.0f},     vec3D{1.0f, 0.1f, 1.0f},     vec3D{0.0f, 0.1f, 1.0f}},
            {vec3D{1.0f, 0.0f, 1.0f},     vec3D{0.0f, 0.1f, 1.0f},     vec3D{0.0f, 0.0f, 1.0f}},

            //// WEST
            {vec3D{0.0f, 0.0f, 1.0f},     vec3D{0.0f, 0.1f, 1.0f},     vec3D{0.0f, 0.1f, 0.0f}},
            {vec3D{0.0f, 0.0f, 1.0f},     vec3D{0.0f, 0.1f, 0.0f},     vec3D{0.0f, 0.0f, 0.0f}},

            //// TOP
          {vec3D{0.0f, 0.1f, 0.0f},     vec3D{0.0f, 0.1f, 1.0f},     vec3D{1.0f, 0.1f, 1.0f}},
          {vec3D{0.0f, 0.1f, 0.0f},     vec3D{1.0f, 0.1f, 1.0f},     vec3D{1.0f, 0.1f, 0.0f}},

          //// BOTTOM
         {vec3D{1.0f, 0.0f, 1.0f},     vec3D{0.0f, 0.0f, 1.0f},     vec3D{0.0f, 0.0f, 0.0f}},
         {vec3D{1.0f, 0.0f, 1.0f},     vec3D{0.0f, 0.0f, 0.0f},     vec3D{1.0f, 0.0f, 0.0f}},

        };
    }

    void createPyramidMesh(Mesh& m)
    {
        m.tris = {
            // SOUTH
            {vec3D{0.0f, 0.0f, 0.0f},     vec3D{0.5f, 1.f, 0.0f},     vec3D{1.0f, 0.0f, 0.0f}},
            {vec3D{0.0f, 0.0f, 0.0f},     vec3D{1.0f, 0.1f, 0.0f},     vec3D{1.0f, 0.0f, 0.0f}},

            // EAST
            //{vec3D{1.0f, 0.0f, 0.0f},     vec3D{1.0f, 0.5f, 0.5f},     vec3D{1.0f, 0.1f, 1.0f}},
            //{vec3D{1.0f, 0.0f, 0.0f},     vec3D{1.0f, 0.1f, 1.0f},     vec3D{1.0f, 0.0f, 1.0f}},

            //// NORTH
            //{vec3D{1.0f, 0.0f, 1.0f},     vec3D{1.0f, 0.1f, 1.0f},     vec3D{0.0f, 0.1f, 1.0f}},
            //{vec3D{1.0f, 0.0f, 1.0f},     vec3D{0.0f, 0.1f, 1.0f},     vec3D{0.0f, 0.0f, 1.0f}},

            ////// WEST
            //{vec3D{0.0f, 0.0f, 1.0f},     vec3D{0.0f, 0.1f, 1.0f},     vec3D{0.0f, 0.1f, 0.0f}},
            //{vec3D{0.0f, 0.0f, 1.0f},     vec3D{0.0f, 0.1f, 0.0f},     vec3D{0.0f, 0.0f, 0.0f}},

            ////// TOP
            //{vec3D{0.0f, 0.1f, 0.0f},     vec3D{0.0f, 0.1f, 1.0f},     vec3D{1.0f, 0.1f, 1.0f}},
            //{vec3D{0.0f, 0.1f, 0.0f},     vec3D{1.0f, 0.1f, 1.0f},     vec3D{1.0f, 0.1f, 0.0f}},

            ////// BOTTOM
            //{vec3D{1.0f, 0.0f, 1.0f},     vec3D{0.0f, 0.0f, 1.0f},     vec3D{0.0f, 0.0f, 0.0f}},
            //{vec3D{1.0f, 0.0f, 1.0f},     vec3D{0.0f, 0.0f, 0.0f},     vec3D{1.0f, 0.0f, 0.0f}},

        };
    }

    void setStartingPos(Mesh& mesh) {
        startingPos = { 0.0f, 0.0f, 5.0f };
        mesh.setPosition(startingPos);
    }

    sf::Vector3f startingPos;
    sf::Vector3f currPos;
};