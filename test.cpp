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

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

const int EMPTY_ID = 0;
const int FISH_ID = 1;
const int SHARK_ID = 2;

const int REPRODUCTION_AGE = 1;
const int FISH_MAX_AGE = 10;  // Maximum age for fish
const int SHARK_STARVATION = 5;  // Number of iterations a shark can survive without eating
const int SHARK_REPRODUCTION_AGE = 20;
const int SHARK_ENERGY_GAIN = 0;

struct Fish
{
    int age;
};

struct Shark
{
    int age;
    int energy;
};

void initializeWorld(Fish**& fishArray, Shark**& sharkArray, int xdim, int ydim, int numFish, int numShark);
void cleanupWorld(Fish** fishArray, Shark** sharkArray, int xdim);

int main()
{
    int xdim = 100;
    int ydim = 100;
    int WindowXSize = 800;
    int WindowYSize = 600;
    int cellXSize = WindowXSize / xdim;
    int cellYSize = WindowYSize / ydim;

    Fish** fishArray = new Fish*[xdim];
    for (int i = 0; i < xdim; ++i)
    {
        fishArray[i] = new Fish[ydim]();
    }

    Shark** sharkArray = new Shark*[xdim];
    for (int i = 0; i < xdim; ++i)
    {
        sharkArray[i] = new Shark[ydim]();
    }

    initializeWorld(fishArray, sharkArray, xdim, ydim, 50, 5);

    sf::RenderWindow window(sf::VideoMode(WindowXSize, WindowYSize), "SFML Wa-Tor world");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        for (int i = 0; i < xdim; ++i)
        {
            for (int k = 0; k < ydim; ++k)
            {
                // Fish reproduction and movement logic
                if (fishArray[i][k].age > 0)
                {
                    // Increment fish age
                    fishArray[i][k].age++;

                    // Check if the fish can reproduce
                    if (fishArray[i][k].age >= REPRODUCTION_AGE)
                    {
                        int newX = i + rand() % 3 - 1;
                        int newY = k + rand() % 3 - 1;

                        // Check if the new position is within bounds
                        if (newX >= 0 && newX < xdim && newY >= 0 && newY < ydim)
                        {
                            // If the new position is unoccupied, move there and reset age
                            if (fishArray[newX][newY].age == 0)
                            {
                                fishArray[newX][newY].age = 1;
                                fishArray[i][k].age = 0; // Leave the old position
                            }
                        }
                    }
                }

                // Shark movement and hunting logic
                if (sharkArray[i][k].energy > 0)
                {
                    int newX = i + rand() % 3 - 1;
                    int newY = k + rand() % 3 - 1;

                    // Check if the new position is within bounds
                    if (newX >= 0 && newX < xdim && newY >= 0 && newY < ydim)
                    {
                        // If the new position is occupied by a fish, eat it
                        if (fishArray[newX][newY].age > 0)
                        {
                            sharkArray[newX][newY].energy += 1; // Gain energy from eating fish
                            fishArray[newX][newY].age = 0;      // Remove the eaten fish
                        }
                        else if (fishArray[newX][newY].age == 0 && sharkArray[newX][newY].energy == 0)
                        {
                            // If the new position is unoccupied, move there and lose energy
                            sharkArray[newX][newY] = sharkArray[i][k];
                            sharkArray[i][k].energy--;
                        }
                    }
                    else
                    {
                        // If the new position is out of bounds, lose energy
                        sharkArray[i][k].energy--;
                    }
                }

                // Shark reproduction logic
                if (sharkArray[i][k].age >= SHARK_REPRODUCTION_AGE && sharkArray[i][k].energy >= SHARK_ENERGY_GAIN)
                {
                    // Try to find a neighboring empty square
                    for (int dx = -1; dx <= 1; ++dx)
                    {
                        for (int dy = -1; dy <= 1; ++dy)
                        {
                            int newX = i + dx;
                            int newY = k + dy;

                            // Check if the new position is within bounds
                            if (newX >= 0 && newX < xdim && newY >= 0 && newY < ydim)
                            {
                                // If the new position is unoccupied, leave a new shark
                                if (fishArray[newX][newY].age == 0 && sharkArray[newX][newY].age == 0)
                                {
                                    sharkArray[newX][newY].age = 1;
                                    sharkArray[newX][newY].energy = sharkArray[i][k].energy;  // Initial energy for the new shark
                                    sharkArray[i][k].energy -= SHARK_ENERGY_GAIN;              // Deduct energy from the parent
                                }
                            }
                        }
                    }
                }

                // Shark starvation logic
                if (sharkArray[i][k].energy <= 0)
                {
                    sharkArray[i][k].age = 0; // Shark dies due to starvation
                }
            }
        }

        window.clear(sf::Color(0, 0, 255));

        int fishCount = 0;
        int sharkCount = 0;

        for (int i = 0; i < xdim; ++i)
        {
            for (int k = 0; k < ydim; ++k)
            {
                sf::RectangleShape rectangle(sf::Vector2f(cellXSize, cellYSize));
                rectangle.setPosition(i * cellXSize, k * cellYSize);

                // Fish and shark visualization
                if (fishArray[i][k].age > 0)
                {
                    rectangle.setFillColor(sf::Color::Green);
                    fishCount++;
                }
                else if (sharkArray[i][k].age > 0)
                {
                    rectangle.setFillColor(sf::Color::Red);
                    sharkCount++;
                }
                else
                {
                    rectangle.setFillColor(sf::Color::Blue);
                }

                window.draw(rectangle);
            }
        }

        // Display fish and shark counts
        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Error loading font\n";
            return 1;
        }

        sf::Text fishText, sharkText;
        fishText.setFont(font);
        sharkText.setFont(font);

        fishText.setString("Fish: " + std::to_string(fishCount));
        sharkText.setString("Sharks: " + std::to_string(sharkCount));

        fishText.setCharacterSize(20);
        sharkText.setCharacterSize(20);

        fishText.setPosition(10, 10);
        sharkText.setPosition(10, 40);

        window.draw(fishText);
        window.draw(sharkText);

        window.display();
    }

    cleanupWorld(fishArray, sharkArray, xdim);

    return 0;
}

void initializeWorld(Fish**& fishArray, Shark**& sharkArray, int xdim, int ydim, int numFish, int numShark)
{
    std::srand(std::time(0));

    for (int i = 0; i < numFish; ++i)
    {
        int x = std::rand() % xdim;
        int y = std::rand() % ydim;
        fishArray[x][y].age = 1;
    }

    for (int i = 0; i < numShark; ++i)
    {
        int x = std::rand() % xdim;
        int y = std::rand() % ydim;
        sharkArray[x][y].age = 1;
        sharkArray[x][y].energy = 5; // Adjust initial shark energy as needed
    }
}

void cleanupWorld(Fish** fishArray, Shark** sharkArray, int xdim)
{
    for (int i = 0; i < xdim; ++i)
    {
        delete[] fishArray[i];
        delete[] sharkArray[i];
    }

    delete[] fishArray;
    delete[] sharkArray;
}
// 
// test.cpp ends here
