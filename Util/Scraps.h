// All this just works by telling chatGPT,
// "Hey this is the code I stole from Javid's game engine, 
//  copy it to fit my program please"

#pragma once

#include <SFML/Graphics.hpp>

class Scraps {
public:
    virtual void Draw(int x, int y, short c, short col, sf::Vector2f sd, sf::RenderWindow& window)
    {
        c = 0x2588;
        col = 0x000F;

        // Assuming that window is an instance of sf::RenderWindow
        if (x >= 0 && x < sd.x && y >= 0 && y < sd.y)
        {
            // Assuming CLEAR_COLOR is the background color of your window
            sf::Color pixelColor(col & 0xFF, (col >> 8) & 0xFF, (col >> 16) & 0xFF);
            sf::RectangleShape pixel(sf::Vector2f(1.0f, 1.0f));
            pixel.setPosition(static_cast<float>(x), static_cast<float>(y));
            pixel.setFillColor(pixelColor);
            window.draw(pixel);
        }
    }


    
};