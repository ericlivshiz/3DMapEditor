#include <SFML/Graphics.hpp>
#include "../Util/Program.h"

int main()
{
    Program program;

    sf::RenderWindow window(sf::VideoMode(program.getScreenDimensions().x, program.getScreenDimensions().y), "SHVIZIL 1.4");

    program.Initialize();

    while (window.isOpen())
    {
        sf::Event event;

        program.UpdatevForward();

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            program.HandleKeyPresses();
        }

        const sf::Color CLEAR_COLOR(81, 94, 60);
        window.clear(CLEAR_COLOR);

        program.Update();

        for (auto& triProjected : program.objMgr.vecTrianglesToRaster)
        {
            sf::VertexArray triVertex(sf::Triangles, 3);
            sf::VertexArray lineVertex(sf::Lines, 6); // 3 lines for the triangle

            program.objMgr.handleTriangles(triVertex, triProjected);
            window.draw(triVertex);

            if (program.editor.drawWireFrames)
            {
                program.objMgr.handleWireFrame(lineVertex, triProjected);
                window.draw(lineVertex);
            }
        }

        for (auto& triProjected : program.drawObj.drawing)
        {
            sf::VertexArray triVertex(sf::Triangles, 3);
            sf::VertexArray lineVertex(sf::Lines, 6); // 3 lines for the triangle

            program.objMgr.handleTriangles(triVertex, triProjected);
            window.draw(triVertex);

            if (program.editor.drawWireFrames)
            {
                program.objMgr.handleWireFrame(lineVertex, triProjected);
                window.draw(lineVertex);
            }
        }

        window.display();

    }

    return 0;
}