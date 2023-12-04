
/**
 * @file main.cpp
 * @brief The main file for the Wa-Tor simulation.
 */
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

// ...

/**
 * @struct Organism
 * @brief Represents an organism in the Wa-Tor simulation.
 */

const int WATER_ID = 0;
const int FISH_ID = 1;
const int SHARK_ID = 2;

struct Organism
{
    int id;
    int age;
    int energy; //for sharks

};

void initializeWorld(Organism**& worldArray, int xdim, int ydim, int numFish, int numShark);
void cleanupWorld(Organism** worldArray, int xdim);
void renderWorld(sf::RenderWindow& window, Organism** worldArray, int xdim, int ydim, int cellXSize, int cellYSize);
void updateWorld(Organism** worldArray, int xdim, int ydim, int fishBreed, int sharkBreed, int starve, int gridSize, int numThreads);

int main(int argc, char* argv[])
{
    int xdim = 100;
    int ydim = 100;
    int WindowXSize = 800;
    int WindowYSize = 800;
    int cellXSize = WindowXSize / xdim;

    // Default parameters
    int numFish = 150;
    int numShark = 100;
    int fishBreed = 1;
    int sharkBreed = 8;
    int starve = 6;
    int gridSize = 100;
    int numThreads = 4;

    // Parse command-line arguments
    if (argc >= 2) numShark = std::atoi(argv[1]);
    if (argc >= 3) numFish = std::atoi(argv[2]);
    if (argc >= 4) fishBreed = std::atoi(argv[3]);
    if (argc >= 5) sharkBreed = std::atoi(argv[4]);
    if (argc >= 6) starve = std::atoi(argv[5]);
    if (argc >= 7) gridSize = std::atoi(argv[6]);
    if (argc >= 8) numThreads = std::atoi(argv[7]);

    Organism** worldArray = new Organism*[xdim];
    for (int i = 0; i < xdim; ++i)
    {
        worldArray[i] = new Organism[ydim]();
    }

    initializeWorld(worldArray, xdim, ydim, numFish, numShark);

    sf::RenderWindow window(sf::VideoMode(WindowXSize, WindowYSize), "SFML Wa-Tor world");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        updateWorld(worldArray, xdim, ydim, fishBreed, sharkBreed, starve, gridSize, numThreads);

        renderWorld(window, worldArray, xdim, ydim, cellXSize, WindowYSize / ydim);
    }

    cleanupWorld(worldArray, xdim);

    return 0;
}

/**
 * @brief Initializes the world with fish and sharks at random positions.
 * @param worldArray The 2D array representing the world.
 * @param xdim The width of the world.
 * @param ydim The height of the world.
 * @param numFish The number of fish to initialize.
 * @param numShark The number of sharks to initialize.
 */

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

/**
 * @brief Cleans up the allocated memory for the world array.
 * @param worldArray The 2D array representing the world.
 * @param xdim The width of the world.
 */
void cleanupWorld(Organism** worldArray, int xdim)
{
    for (int i = 0; i < xdim; ++i)
    {
        delete[] worldArray[i];
    }

    delete[] worldArray;
}

/**
 * @brief Renders the current state of the world using SFML.
 * @param window The SFML window to render in.
 * @param worldArray The 2D array representing the world.
 * @param xdim The width of the world.
 * @param ydim The height of the world.
 * @param cellXSize The size of each cell in the x-direction.
 * @param cellYSize The size of each cell in the y-direction.
 */
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


/**
 * @brief Updates the world according to the rules of the Wa-Tor simulation.
 * @param worldArray The 2D array representing the world.
 * @param xdim The width of the world.
 * @param ydim The height of the world.
 * @param fishBreed The age at which fish can reproduce.
 * @param sharkBreed The age at which sharks can reproduce.
 * @param starve The number of turns a shark can survive without eating.
 * @param gridSize The size of the grid for multi-threading.
 * @param numThreads The number of threads to use for updating.
 */
 
void updateWorld(Organism** worldArray, int xdim, int ydim, int fishBreed, int sharkBreed, int starve, int gridSize, int numThreads)
{
    // Loop through each cell in the world
    for (int i = 0; i < xdim; ++i)
    {
        for (int k = 0; k < ydim; ++k)
        {
            // Fish and shark logic
            if (worldArray[i][k].id == FISH_ID || worldArray[i][k].id == SHARK_ID)
            {
                // Increment age for both fish and sharks
                worldArray[i][k].age++;

                // Move logic for fish
                if (worldArray[i][k].id == FISH_ID)
                {
                    int newX = i + rand() % 3 - 1;
                    int newY = k + rand() % 3 - 1;

                    // Check if the new position is within bounds
                    if (newX >= 0 && newX < xdim && newY >= 0 && newY < ydim)
                    {
                        // If the new position is unoccupied, move there and reset age for the new fish
                        if (worldArray[newX][newY].id == WATER_ID)
                        {
                            worldArray[newX][newY] = worldArray[i][k];
                            worldArray[i][k].id = WATER_ID; // Leave the old position
                            worldArray[i][k].age = 0; // Reset age for the parent fish after reproduction
                        }
                    }
                }

                // Move logic for sharks
                else if (worldArray[i][k].id == SHARK_ID)
                {
                    int newX, newY;

                    // Check if there's a neighboring fish
                    std::vector<std::pair<int, int> > fishNeighbors;

                    for (int dx = -1; dx <= 1; ++dx)
                    {
                        for (int dy = -1; dy <= 1; ++dy)
                        {
                            int checkX = i + dx;
                            int checkY = k + dy;

                            if (checkX >= 0 && checkX < xdim && checkY >= 0 && checkY < ydim &&
                                worldArray[checkX][checkY].id == FISH_ID)
                            {
                                fishNeighbors.push_back(std::make_pair(checkX, checkY));
                            }
                        }
                    }

                    // If there are neighboring fish, move to a random fish cell
                    if (!fishNeighbors.empty())
                    {
                        std::pair<int, int> randomFish = fishNeighbors[rand() % fishNeighbors.size()];
                        newX = randomFish.first;
                        newY = randomFish.second;

                        // Devour the fish and earn energy
                        worldArray[i][k].energy += 1;
                        worldArray[newX][newY].id = WATER_ID; // Remove the eaten fish
                    }
                    else
                    {
                        // If no neighboring fish, move randomly to an unoccupied cell
                        newX = i + rand() % 3 - 1;
                        newY = k + rand() % 3 - 1;
                    }
                    // Check if the new position is within bounds
                    if (newX >= 0 && newX < xdim && newY >= 0 && newY < ydim)
                    {
                        // Move to the new position and lose energy
                        worldArray[newX][newY] = worldArray[i][k];
                        worldArray[i][k].id = WATER_ID; // Leave the old position
                        worldArray[i][k].energy--;

                        // Reproduction logic for sharks
                        if (worldArray[newX][newY].age >= sharkBreed && worldArray[i][k].energy >= starve && worldArray[i][k].age >= 5)
                        {
                            // Try to find a neighboring empty square
                            for (int dx = -1; dx <= 1; ++dx)
                            {
                                for (int dy = -1; dy <= 1; ++dy)
                                {
                                    int spawnX = newX + dx;
                                    int spawnY = newY + dy;

                                    // Check if the new position is within bounds
                                    if (spawnX >= 0 && spawnX < xdim && spawnY >= 0 && spawnY < ydim &&
                                        worldArray[spawnX][spawnY].id == WATER_ID)
                                    {
                                        // Leave a new shark
                                        worldArray[spawnX][spawnY].id = SHARK_ID;
                                        worldArray[spawnX][spawnY].age = 1; // Reset age for the new shark
                                        worldArray[newX][newY].age = 0; // Reset age for the parent shark
                                        worldArray[i][k].energy = 0; // Reset energy for the parent shark
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        // If the new position is out of bounds, do nothing
                    }

                    // Check if the shark has reached zero energy and should die
                    if (worldArray[i][k].energy <= 0)
                    {
                        worldArray[i][k].id = WATER_ID; // Shark dies, change the cell back to water
                    }
                }

                // Reproduction logic for both fish and sharks
                if (worldArray[i][k].age >= (worldArray[i][k].id == FISH_ID ? fishBreed : sharkBreed))
                {
                    // Try to find a neighboring empty square
                    for (int dx = -1; dx <= 1; ++dx)
                    {
                        for (int dy = -1; dy <= 1; ++dy)
                        {
                            int spawnX = i + dx;
                            int spawnY = k + dy;

                            // Check if the new position is within bounds
                            if (spawnX >= 0 && spawnX < xdim && spawnY >= 0 && spawnY < ydim &&
                                worldArray[spawnX][spawnY].id == WATER_ID)
                            {
                                // Leave a new organism (fish or shark)
                                worldArray[spawnX][spawnY].id = worldArray[i][k].id;
                                worldArray[spawnX][spawnY].age = 1; // Reset age for the new organism
                                worldArray[i][k].age = 0; // Reset age for the parent organism
                            }
                        }
                    }
                }
            }
        }
    }
}

