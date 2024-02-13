//#pragma once
//
//#include <SFML/Graphics.hpp>
//#include "../ObjectMgr/ObjectMgr.h"
//#include "../Util/Program.h"
//#include <>
//
//
//class Renderer {
//	void drawToWindow(sf::RenderWindow& window)
//	{
//        // Draw Triangles Projections
//        for (Triangle& program.triProjected : program.vecTrianglesToRaster)
//        {
//            sf::VertexArray triVertex(sf::Triangles, 3);
//            sf::VertexArray lineVertex(sf::Lines, 6); // 3 lines for the triangle
//
//            objMgr.handleTriangles(triVertex, program.triProjected);
//            window.draw(triVertex);
//
//            if (drawWireFrames)
//            {
//                objMgr.handleWireFrame(lineVertex, triProjected);
//                window.draw(lineVertex);
//            }
//
//        }
//	}
//};