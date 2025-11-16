#include <SFML/Graphics.hpp>
#include <optional>
#include<iostream>



void game(sf::RenderWindow& window) {

    float jumprotate = 433.7349f, jumpspeed = 500.f, groundlevel = 450.f, maxjump = 200.f, xorigin = 150;

    sf::Texture ptexture("Assets\\Sprites\\playersprite.png");
    ptexture.setSmooth(true);

	sf::Texture btexture("Assets\\Sprites\\background.png");
	btexture.setSmooth(true);
    sf::Sprite background(btexture);
	background.setPosition({ 0.f, 0.f });

	sf::Texture otexture("Assets\\Sprites\\obstacle.png");
	otexture.setSmooth(true);



    sf::Sprite player(ptexture);
    player.setOrigin({50, 50});
    player.setPosition({ xorigin, groundlevel });


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

        if (player.getPosition().y >= groundlevel -5.f) {
            player.setRotation(sf::degrees(0.f));
        }

        if (canjump) {
            player.move({ 0.0f, (-(jumpspeed) * deltaTime) });
            player.rotate(sf::degrees(jumprotate * deltaTime));
        }
        if (player.getPosition().y <= groundlevel - maxjump && canjump) {
            canjump = false;
        }


        if (player.getPosition().y < groundlevel && !canjump) {
            player.move({ 0.f, jumpspeed * deltaTime });
            player.rotate(sf::degrees(jumprotate * deltaTime));
        }

        if (player.getPosition().y >= groundlevel - 5.f && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) && canjump1) {
            player.setPosition({ player.getPosition().x, groundlevel });
            canjump1 = false;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !canjump && player.getPosition().y >= groundlevel - 5.f && !canjump1) {
            canjump = true;
            canjump1 = true;
        }

        window.clear();
        window.draw(background);
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
        sf::RenderWindow window(sf::VideoMode({ 1000, 600 }), "SFML Test");

        if (play) {
            game(window);
        }
    }
	std::cout << "Exiting the game. Goodbye!" << std::endl;

    return 0;
}