#include <SFML/Graphics.hpp>
#include <optional>
#include<iostream>


void game(sf::RenderWindow& window) {

    sf::Texture texture("Assets\\Sprites\\playersprite.png");
    texture.setSmooth(true);
    sf::Sprite player(texture);
    player.setPosition({ 50.f, 500.f });


    bool canjump = true, canjump1 = false;

    sf::Clock clock;
    float deltaTime;


    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();

		//loop in sample on the official SFML website, if this is changed, the window stops responding
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !canjump && player.getPosition().y >= 495.0f && !canjump1) {
            canjump = true;
            canjump1 = true;
        }

        if (canjump) {
            player.move({ 0.0f, (-500.f * deltaTime) });
        }
        if (player.getPosition().y <= 350.f && canjump) {
            canjump = false;
        }


        if (player.getPosition().y < 500.0f && !canjump) {
            player.move({ 0.f, 450.f * deltaTime });
        }

        if (player.getPosition().y >= 495.0f && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) && canjump1) {
            player.setPosition({ player.getPosition().x, 500.0f });
            canjump1 = false;
        }

        window.clear();
        window.draw(player);
        window.display();
    }
}

int main()
{   
    bool play = false;
	std::cout << "Play the game? (1 for yes, 0 for no): ";
	std::cin >> play;
    if (play) {
        sf::RenderWindow window(sf::VideoMode({ 800, 640 }), "SFML Test");

        if (play) {
            game(window);
        }
    }
	std::cout << "Exiting the game. Goodbye!" << std::endl;

    return 0;
}