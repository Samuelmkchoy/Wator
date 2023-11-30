// test.cpp --- 
// 
// Filename: test.cpp
// Description: 
// Author: Ming Kit Choy
// Maintainer: 
// Created: Fri Nov  3 15:51:15 2023 (+0000)
// Last-Updated: Fri Nov  16 15:30:00 2023 (+0000)
//           By: Ming
//     Update #: 19
// 
// 

// Commentary: 
// 
// 
// 
// 
// 
// 
// 
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at
// your option) any later version.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
// 
// some helpful code here!
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// https://www.sfml-dev.org/tutorials/2.5/start-linux.php
// https://learnsfmcl.com/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Code:

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

const int EMPTY_ID = 0;
const int FISH_ID = 1;
const int SHARK_ID = 2;

const int WORLD_SIZE = 100;

void initializeWorld(int (&world)[WORLD_SIZE][WORLD_SIZE], int numFish, int numShark);
void updateWorld(int (&world)[WORLD_SIZE][WORLD_SIZE]);

int main()
{
    int world[WORLD_SIZE][WORLD_SIZE];
    initializeWorld(world, 20, 10);

    sf::RenderWindow window(sf::VideoMode(WORLD_SIZE * 8, WORLD_SIZE * 8), "SFML Wa-Tor world");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) // the font file path 
    {
        std::cerr << "Error loading font" << std::endl;
        return 1;
    }

    sf::Text text("", font, 20);
    text.setFillColor(sf::Color::Black);

 while (window.isOpen())
{
    sf::Event event;
    while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        updateWorld(world);

        window.clear(sf::Color::Blue); // Set water color to blue

        // Draw the world based on the updated state
        for (int i = 0; i < WORLD_SIZE; ++i)
        {
            for (int k = 0; k < WORLD_SIZE; ++k)
            {
                sf::RectangleShape rectangle(sf::Vector2f(8, 8));
                rectangle.setPosition(i * 8, k * 8);

                // Set colors based on cell contents
                if (world[i][k] == FISH_ID)
                    rectangle.setFillColor(sf::Color::Green);
                else if (world[i][k] == SHARK_ID)
                    rectangle.setFillColor(sf::Color::Red);

                window.draw(rectangle);
            }
        }

        // Display current number of fish and sharks
        int numFish = 0;
        int numShark = 0;

        for (int i = 0; i < WORLD_SIZE; ++i)
        {
            for (int k = 0; k < WORLD_SIZE; ++k)
            {
                if (world[i][k] == FISH_ID)
                    numFish++;
                else if (world[i][k] == SHARK_ID)
                    numShark++;
            }
        }

        text.setString("Fish: " + std::to_string(numFish) + " | Sharks: " + std::to_string(numShark));
        text.setPosition(10, 10); // Set position to top-left corner
        window.draw(text);

        window.display();
    }


    return 0;
}

void initializeWorld(int (&world)[WORLD_SIZE][WORLD_SIZE], int numFish, int numShark)
{
    std::srand(std::time(0));

    // Initialize the world with empty spaces
    for (int i = 0; i < WORLD_SIZE; ++i)
    {
        for (int k = 0; k < WORLD_SIZE; ++k)
        {
            world[i][k] = EMPTY_ID;
        }
    }

    // Initialize fish and sharks randomly in the world
    for (int i = 0; i < numFish; ++i)
    {
        int x = std::rand() % WORLD_SIZE;
        int y = std::rand() % WORLD_SIZE;
        world[x][y] = FISH_ID; // Set ID for fish
    }

    for (int i = 0; i < numShark; ++i)
    {
        int x = std::rand() % WORLD_SIZE;
        int y = std::rand() % WORLD_SIZE;
        world[x][y] = SHARK_ID; // Set ID for shark
    }
}

void updateWorld(int (&world)[WORLD_SIZE][WORLD_SIZE])
{
    for (int i = 0; i < WORLD_SIZE; ++i)
    {
        for (int k = 0; k < WORLD_SIZE; ++k)
        {
            if (world[i][k] == FISH_ID)
            {
                // Fish behavior
                int newX = i + rand() % 3 - 1;
                int newY = k + rand() % 3 - 1;

                if (newX >= 0 && newX < WORLD_SIZE && newY >= 0 && newY < WORLD_SIZE && world[newX][newY] == EMPTY_ID)
                {
                    world[newX][newY] = FISH_ID; // Fish moves
                    world[i][k] = EMPTY_ID;      // Original position becomes empty
                }
            }
            else if (world[i][k] == SHARK_ID)
            {
                // Shark behavior
                int newX, newY;

                // Check for adjacent fish
                for (int dx = -1; dx <= 1; ++dx)
                {
                    for (int dy = -1; dy <= 1; ++dy)
                    {
                        newX = i + dx;
                        newY = k + dy;

                        if (newX >= 0 && newX < WORLD_SIZE && newY >= 0 && newY < WORLD_SIZE && world[newX][newY] == FISH_ID)
                        {
                            // Shark moves to adjacent fish and eats it
                            world[newX][newY] = SHARK_ID;
                            world[i][k] = EMPTY_ID;
                            break;
                        }
                    }
                }

                if (world[i][k] == SHARK_ID) // Shark did not eat fish, move randomly
                {
                    newX = i + rand() % 3 - 1;
                    newY = k + rand() % 3 - 1;

                    if (newX >= 0 && newX < WORLD_SIZE && newY >= 0 && newY < WORLD_SIZE && world[newX][newY] == EMPTY_ID)
                    {
                        world[newX][newY] = SHARK_ID; // Shark moves
                        world[i][k] = EMPTY_ID;       // Original position becomes empty
                    }
                }
            }
        }
    }
}




// 
// test.cpp ends here
