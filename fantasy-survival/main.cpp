#include <SFML/Graphics.hpp>
#include <iostream>
#include "worldGeneration.cpp"

sf::Vector2i playerPosition = sf::Vector2i(0, 0);
//sf::Vector2i playerChunkPosition = sf::Vector2i(0, 0);
//sf::Vector2i playerMapPosition = sf::Vector2i(0, 0);

int main()
{
    // -------------------------- INITIALIZE --------------------------
    sf::RenderWindow window(sf::VideoMode(1024, 940), "Fantasy Survival"); //1280 720
    window.setVerticalSyncEnabled(true);

    // -------------------------- LOAD --------------------------------
    sf::Texture playerTexture;
    sf::Sprite playerSprite;

    if (playerTexture.loadFromFile("Assets/Player/Textures/Character-Base.png", sf::IntRect(0, 0, 32, 32))) {
        std::cout << "Player texture loaded" << std::endl;
        playerSprite.setTexture(playerTexture);
        playerSprite.setOrigin(sf::Vector2f(8.f, 8.f));
        playerSprite.setPosition(getScreenCentre());
    }
    else {
        std::cout << "Player texture not loaded" << std::endl;
    }

    // -------------------------- WORLD GENERATION --------------------------------
    generateWorld();
    initializeRegion();
    updateRegion(playerPosition);
    // -------------------------- UPDATE --------------------------
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
                // close window
            case sf::Event::Closed:
                window.close();
                break;

                // key input
            case sf::Event::KeyPressed:
                if (event.key.scancode == sf::Keyboard::Scan::Escape) {
                    window.close();
                }

                sf::Vector2i direction = sf::Vector2i(0, 0);
                if (event.key.scancode == sf::Keyboard::Scan::Numpad8)
                    direction = sf::Vector2i(0, -1);
                else if (event.key.scancode == sf::Keyboard::Scan::Numpad2)
                    direction = sf::Vector2i(0, 1);
                else if (event.key.scancode == sf::Keyboard::Scan::Numpad4)
                    direction = sf::Vector2i(-1, 0);
                else if (event.key.scancode == sf::Keyboard::Scan::Numpad6)
                    direction = sf::Vector2i(1, 0);
                else if (event.key.scancode == sf::Keyboard::Scan::Numpad7)
                    direction = sf::Vector2i(-1, -1);
                else if (event.key.scancode == sf::Keyboard::Scan::Numpad9)
                    direction = sf::Vector2i(1, -1);
                else if (event.key.scancode == sf::Keyboard::Scan::Numpad1)
                    direction = sf::Vector2i(-1, 1);
                else if (event.key.scancode == sf::Keyboard::Scan::Numpad3)
                    direction = sf::Vector2i(1, 1);

                if (direction != sf::Vector2i(0, 0)) {
                    playerPosition += direction;
                    updateRegion(playerPosition);
                }

                //playerSprite.setPosition(playerSprite.getPosition() + sf::Vector2f(direction.x * tileSize, direction.y * tileSize));

                break;
            }

            // -------------------------- DRAW --------------------------
            window.clear(sf::Color::Color(40, 40, 48));
            window.draw(tilemapTiles);
            window.draw(tilemapObjects);
            window.draw(playerSprite);
            window.display();
        }
    }

    // ----------------------------------------------------------

    return 0;
}