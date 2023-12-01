// test.cpp
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

const int WATER_ID = 0;
const int FISH_ID = 1;
const int SHARK_ID = 2;

const int REPRODUCTION_AGE = 1;
const int SHARK_MAX_AGE = 10;  // Maximum age for sharks
const int SHARK_REPRODUCTION_AGE = 10;

struct Organism
{
    int id;
    int age;
    // No energy parameter for sharks
};

void initializeWorld(Organism**& worldArray, int xdim, int ydim, int numFish, int numShark);
void cleanupWorld(Organism** worldArray, int xdim);
void renderWorld(sf::RenderWindow& window, Organism** worldArray, int xdim, int ydim, int cellXSize, int cellYSize);
void updateWorld(Organism** worldArray, int xdim, int ydim);

int main()
{
    int xdim = 100;
    int ydim = 100;
    int WindowXSize = 800;
    int WindowYSize = 600;
    int cellXSize = WindowXSize / xdim;
    int cellYSize = WindowYSize / ydim;

    Organism** worldArray = new Organism*[xdim];
    for (int i = 0; i < xdim; ++i)
    {
        worldArray[i] = new Organism[ydim]();
    }

    initializeWorld(worldArray, xdim, ydim, 500, 50);

    sf::RenderWindow window(sf::VideoMode(WindowXSize, WindowYSize), "SFML Wa-Tor world");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        updateWorld(worldArray, xdim, ydim);

        renderWorld(window, worldArray, xdim, ydim, cellXSize, cellYSize);
    }

    cleanupWorld(worldArray, xdim);

    return 0;
}

void initializeWorld(Organism**& worldArray, int xdim, int ydim, int numFish, int numShark)
{
    std::srand(std::time(0));

    for (int i = 0; i < numFish; ++i)
    {
        int x = std::rand() % xdim;
        int y = std::rand() % ydim;
        worldArray[x][y].id = FISH_ID;
        worldArray[x][y].age = 1;
    }

    for (int i = 0; i < numShark; ++i)
    {
        int x = std::rand() % xdim;
        int y = std::rand() % ydim;
        worldArray[x][y].id = SHARK_ID;
        worldArray[x][y].age = 1;
    }
}

void cleanupWorld(Organism** worldArray, int xdim)
{
    for (int i = 0; i < xdim; ++i)
    {
        delete[] worldArray[i];
    }

    delete[] worldArray;
}

void renderWorld(sf::RenderWindow& window, Organism** worldArray, int xdim, int ydim, int cellXSize, int cellYSize)
{
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
            if (worldArray[i][k].id == FISH_ID)
            {
                rectangle.setFillColor(sf::Color::Green);
                fishCount++;
            }
            else if (worldArray[i][k].id == SHARK_ID)
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
        return;
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

void updateWorld(Organism** worldArray, int xdim, int ydim)
{
    for (int i = 0; i < xdim; ++i)
    {
        for (int k = 0; k < ydim; ++k)
        {
            // Fish reproduction and movement logic
            if (worldArray[i][k].id == FISH_ID)
            {
                // Increment fish age
                worldArray[i][k].age++;

                // Check if the fish can reproduce
                if (worldArray[i][k].age >= REPRODUCTION_AGE)
                {
                    int newX = i + rand() % 3 - 1;
                    int newY = k + rand() % 3 - 1;

                    // Check if the new position is within bounds
                    if (newX >= 0 && newX < xdim && newY >= 0 && newY < ydim)
                    {
                        // If the new position is unoccupied, move there and reset age for the new fish
                        if (worldArray[newX][newY].id == WATER_ID)
                        {
                            worldArray[newX][newY].id = FISH_ID;
                            worldArray[newX][newY].age = 1; // Reset age for the new fish
                            worldArray[i][k].age = 0; // Reset age for the parent fish after reproduction
                            worldArray[i][k].id = WATER_ID; // Leave the old position
                        }
                    }
                }
            }

            // Shark movement and hunting logic
            else if (worldArray[i][k].id == SHARK_ID)
            {
                // Increment shark age
                worldArray[i][k].age++;

                // Check if the shark has reached its maximum age
                if (worldArray[i][k].age > SHARK_MAX_AGE)
                {
                    // Shark dies of old age, change the cell back to water
                    worldArray[i][k].id = WATER_ID;
                }
                else
                {
                    int newX = i + rand() % 3 - 1;
                    int newY = k + rand() % 3 - 1;

                    // Check if the new position is within bounds
                    if (newX >= 0 && newX < xdim && newY >= 0 && newY < ydim)
                    {
                        // If the new position is occupied by a fish, eat it
                        if (worldArray[newX][newY].id == FISH_ID)
                        {
                            worldArray[newX][newY].id = WATER_ID; // Remove the eaten fish
                            worldArray[i][k].age = 1; // Reset age after eating
                        }
                        else if (worldArray[newX][newY].id == WATER_ID)
                        {
                            // If the new position is unoccupied, move there and check for reproduction
                            worldArray[newX][newY] = worldArray[i][k];
                            worldArray[i][k].id = WATER_ID; // Leave the old position

                            // Shark reproduction logic
                            if (worldArray[newX][newY].age >= SHARK_REPRODUCTION_AGE)
                            {
                                // Try to find a neighboring empty square
                                for (int dx = -1; dx <= 1; ++dx)
                                {
                                    for (int dy = -1; dy <= 1; ++dy)
                                    {
                                        int spawnX = newX + dx;
                                        int spawnY = newY + dy;

                                        // Check if the new position is within bounds
                                        if (spawnX >= 0 && spawnX < xdim && spawnY >= 0 && spawnY < ydim)
                                        {
                                            // If the new position is unoccupied, leave a new shark
                                            if (worldArray[spawnX][spawnY].id == WATER_ID)
                                            {
                                                worldArray[spawnX][spawnY].id = SHARK_ID;
                                                worldArray[spawnX][spawnY].age = 1; // Reset age for the new shark
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        // If the new position is out of bounds, do nothing
                    }
                }
            }
        }
    }
}
