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
#include <cstring>

// Define constants for cell contents
const int EMPTY_ID = 0;
const int FISH_ID = 1;
const int SHARK_ID = 2;

// Function to initialize the world with fish and sharks
void initializeWorld(int (&world)[100][100], int xdim, int ydim, int numFish, int numShark);

int main()
{
    int xdim = 100;
    int ydim = 100;
    int WindowXSize = 800;
    int WindowYSize = 600;
    int cellXSize = WindowXSize / xdim;
    int cellYSize = WindowYSize / ydim;

    int world[100][100];

    // Function to initialize the world with fish and sharks
    initializeWorld(world, xdim, ydim, 20, 10); // Adjust the initial population numbers

    sf::RenderWindow window(sf::VideoMode(WindowXSize, WindowYSize), "SFML Wa-Tor world");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Simulation logic goes here...
        int newWorld[100][100];

        for (int i = 0; i < xdim; ++i)
        {
            for (int k = 0; k < ydim; ++k)
            {
                // Implement Wa-Tor simulation rules
                // (fish movement, shark movement, reproduction, starvation, etc.)
                // Update newWorld based on the rules
                newWorld[i][k] = world[i][k]; // Placeholder, replace with actual logic
            }
        }

        // Copy the updated world to the original array
        std::memcpy(world, newWorld, sizeof(world));

        window.clear(sf::Color::Black);

        // Draw the world based on the updated state
        for (int i = 0; i < xdim; ++i)
        {
            for (int k = 0; k < ydim; ++k)
            {
                sf::RectangleShape rectangle(sf::Vector2f(cellXSize, cellYSize));
                rectangle.setPosition(i * cellXSize, k * cellYSize);

                // Set colors based on cell contents
                if (world[i][k] == FISH_ID)
                    rectangle.setFillColor(sf::Color::Green);
                else if (world[i][k] == SHARK_ID)
                    rectangle.setFillColor(sf::Color::Red);

                window.draw(rectangle);
            }
        }

        window.display();
    }

    return 0;
}

// Function to initialize the world with fish and sharks
void initializeWorld(int (&world)[100][100], int xdim, int ydim, int numFish, int numShark)
{
    std::srand(std::time(0)); // Seed the random number generator

    // Initialize the world with empty spaces
    for (int i = 0; i < xdim; ++i)
    {
        for (int k = 0; k < ydim; ++k)
        {
            world[i][k] = EMPTY_ID;
        }
    }

    // Initialize fish and sharks randomly in the world
    for (int i = 0; i < numFish; ++i)
    {
        int x = std::rand() % xdim;
        int y = std::rand() % ydim;
        world[x][y] = FISH_ID; // Set ID for fish
    }

    for (int i = 0; i < numShark; ++i)
    {
        int x = std::rand() % xdim;
        int y = std::rand() % ydim;
        world[x][y] = SHARK_ID; // Set ID for shark
    }
}



// 
// test.cpp ends here
