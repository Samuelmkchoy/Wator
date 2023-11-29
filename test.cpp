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

const int WORLD_SIZE = 100;

// Function to initialize the world with fish and sharks
void initializeWorld(int (&world)[WORLD_SIZE][WORLD_SIZE], int numFish, int numShark);
void updateWorld(int (&world)[WORLD_SIZE][WORLD_SIZE]);

int main()
{
    int world[WORLD_SIZE][WORLD_SIZE];
    initializeWorld(world, 20, 10);

    sf::RenderWindow window(sf::VideoMode(WORLD_SIZE * 8, WORLD_SIZE * 8), "SFML Wa-Tor world");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        updateWorld(world);

        window.clear(sf::Color::Black);

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

        window.display();
    }

    return 0;
}

// Function to initialize the world with fish and sharks
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
    // Placeholder: Replace this with your Wa-Tor simulation logic
    // This is a simplified example, and you need to implement the actual logic
    for (int i = 0; i < WORLD_SIZE; ++i)
    {
        for (int k = 0; k < WORLD_SIZE; ++k)
        {
            // Randomly move fish or sharks (you should replace this with your logic)
            if (world[i][k] == FISH_ID || world[i][k] == SHARK_ID)
            {
                int newI = (i + rand() % 3 - 1 + WORLD_SIZE) % WORLD_SIZE;
                int newK = (k + rand() % 3 - 1 + WORLD_SIZE) % WORLD_SIZE;
                std::swap(world[i][k], world[newI][newK]);
            }
        }
    }
}



// 
// test.cpp ends here
