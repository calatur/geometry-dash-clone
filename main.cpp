#include <SFML/Graphics.hpp>
#include <optional>


int main()
{   
	sf::Texture texture("playersprite.png");
	texture.setSmooth(true);
    sf::Sprite player(texture);
    player.setPosition({ 50.f, 500.f });


    bool canjump = true, canjump1 = false;
    sf::RenderWindow window(sf::VideoMode({ 800, 640 }), "SFML Test");

    sf::Clock clock;
    float deltaTime;


    while (window.isOpen())
    {
		deltaTime = clock.restart().asSeconds();
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
                player.move({ 0.0f, ( -500.f * deltaTime)});   
            }
        if (player.getPosition().y <= 350.f && canjump) {
            canjump = false;
		}


        if (player.getPosition().y < 500.0f && !canjump) {
            player.move({0.f, 450.f*deltaTime});
        }

        if (player.getPosition().y >= 495.0f && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) && canjump1) {
            player.setPosition({ player.getPosition().x, 500.0f });
            //canjump = true;
            canjump1 = false;
        }

        window.clear();
        window.draw(player);
        window.display();
    }
}