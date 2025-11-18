#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <optional>
#include<iostream>
#include<string>
#include<fstream>
#include<ctime>
#include<cstdlib>

int game(sf::RenderWindow& window);
void enterName(sf::RenderWindow& window);
void mmenu(sf::RenderWindow& window);
int score(sf::Clock& clock);
void saveRun(std::string , int score);
void loadScores();
bool checkCollision(const sf::Sprite& player, const sf::ConvexShape& obstacle);
sf::ConvexShape* setObstacles(const int n);

sf::Font font;

int main()
{
    srand(time(0));
    if (!(font.openFromFile("Assets\\Fonts\\8bitOperatorPlus8-Bold.ttf"))) {
        std::cout << "could not open font file";
    }
    bool play = true;
    bool start = false;

    sf::RenderWindow window(sf::VideoMode({ 1000, 600 }), "SFML Test");
    window.setKeyRepeatEnabled(false);
    if (play) {
        mmenu(window);
    }

    return 0;
}


void mmenu(sf::RenderWindow& window) {
    sf::Texture mainmenutexture("Assets\\Sprites\\mainmenu.png");
    sf::Sprite mainmenu(mainmenutexture);
    mainmenu.setPosition({ 0.f, 0.f });
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scan::C)
                {
					window.close();
                }

                if (keyPressed->scancode == sf::Keyboard::Scan::Z)
                {
					enterName(window);
                }
            }
        }
   //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
   //         //instead of loading game, display an intermediate screen (new function) to ask for the run's name
   //         // run's name will be a character array of fixed size
			////Then pass that name to game function to save the run under that name.
   //         //That name will be passed to the saveRun() function along with the score in that run
   //         //to store in a binary file
   //         game(window);
			////when the game function returns, return the intermediate screen to main menu
   //     }
        window.clear();
        window.draw(mainmenu);
        window.display();
    }
}
//
//
//
//
int game(sf::RenderWindow& window) {

    float jumprotate = 433.7349f, 
        jumpspeed = 500.f, 
        groundlevel = 450.f, 
        maxjump = 200.f, 
        xorigin = 150,
        deltaTime;

    sf::Texture ptexture("Assets\\Sprites\\playersprite.png");
    ptexture.setSmooth(true);
    sf::Sprite player(ptexture);
    player.setOrigin({ 50, 50 });
    player.setPosition({ xorigin, groundlevel });

	sf::Texture btexture("Assets\\Sprites\\background.png");
	btexture.setSmooth(true);
    sf::Sprite background(btexture);
	background.setPosition({ 0.f, 0.f });

	sf::Texture otexture("Assets\\Sprites\\obstacle.png");
	otexture.setSmooth(true);

	sf::ConvexShape* obstacle = setObstacles(3);

    sf::Vector2f positions[3];
    positions[0].x = 1000;
    positions[1].x = 1600;
    positions[2].x = 2100;

    positions[0].y = 416;
    positions[1].y = 416;
    positions[2].y = 416;


    obstacle[0].setPosition(positions[0]);
    obstacle[1].setPosition(positions[1]);
    obstacle[2].setPosition(positions[2]);

    float obsSpeed = 550.f;
    int min = 475;

    bool canjump = false, 
        canjump1 = false;

    sf::Clock clock, scoreclock;
    int cscore = 0;

    std::string sscore = "";

	int speeds[5] = { 550, 700, 750, 800, 1000 };
	int mins[5] = { 510, 575, 625, 690, 730 };

    sf::Text scoretext(font, sscore);
    sf::Text scoreheader(font, "SCORE: ");
	sf::Text controls(font, "Z:   JUMP\nC:   MENU");
    scoreheader.setPosition({ 25.f, 100.f });
    scoretext.setPosition({ 150.f, 100.f });
    controls.setPosition({25.f, 10.f});


    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("Assets\\Sounds\\jumpy_jumpy.wav")) {
        std::cout << "Sound file not found!" << std::endl;
        return cscore;
    }
    sf::Sound beep(buffer);

    bool spaces = true;
    int i;

    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();

		cscore = score(scoreclock); //score calculation in cscore, store cscore when run ends (pressed C or ran into obstacles)
		sscore = std::to_string(cscore);
        scoretext.setString(sscore);

        if (cscore < 2100) {
            obsSpeed = speeds[cscore / 500];
            min = mins[cscore / 500];
        }

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scan::C)
                {   //save here
                    delete[] obstacle;
					std::cout << "deleted obstacles" << std::endl;
                    return cscore;
                }
            }
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

        if (player.getPosition().y >= groundlevel - 5.f 
            && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) 
            && canjump1) 
        {
            player.setPosition({ player.getPosition().x, groundlevel });
            canjump1 = false;
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)
            && !canjump && player.getPosition().y >= groundlevel - 5.f &&
            !canjump1) 
        {
            canjump = true;
            canjump1 = true;
            beep.play();
        }

        for (i = 0; i < 3; i++) {
            obstacle[i].move({ -obsSpeed * deltaTime, 0 });
            positions[i].x -= obsSpeed * deltaTime;
            if (obstacle[i].getPosition().x < -100) {
                if (i == 0) {
                    positions[i].x = positions[2].x + rand() % 200 + min;;
                    obstacle[i].setPosition(positions[i]);
                }
                else {
                    positions[i].x = positions[i-1].x + rand() % 200 + min;;
                    obstacle[i].setPosition(positions[i]);
                }

                obstacle[i].setFillColor(sf::Color(rand() % 256, rand()%256, rand()%256));
            }
        }

        for (i = 0; i < 3; i++) {
            if (checkCollision(player, obstacle[i])) {
                delete[] obstacle;
                std::cout << "deleted obstacles" << std::endl;
                return cscore;
            }
        }

        window.clear();
        window.draw(background);
        window.draw(controls);
		window.draw(scoreheader);
        window.draw(scoretext);
        window.draw(player);
        for(i=0; i<3; i++)
            window.draw(obstacle[i]);

        window.display();
    }
}
//
//
//
//
bool checkCollision(const sf::Sprite& player, const sf::ConvexShape& obstacle)
{
    sf::FloatRect playerBounds = player.getGlobalBounds();
    for (size_t i = 0; i < obstacle.getPointCount(); ++i)
    {
        sf::Vector2f point = obstacle.getTransform().transformPoint(obstacle.getPoint(i));
        if (playerBounds.contains(point))
            return true;
    }

    return false;
}
//
//
//
//
int score(sf::Clock& clock) {

    int bonus = 1;
    int scale = 50;

    if (clock.getElapsedTime().asMilliseconds()/scale > 100) {
        bonus = 2;
    }
    else if (clock.getElapsedTime().asMilliseconds()/scale > 300) {
        bonus = 3;
    }
    else if (clock.getElapsedTime().asMilliseconds()/scale > 1000) {
        bonus = 4;
    }

    return (int)(clock.getElapsedTime().asMilliseconds()/scale * bonus);
}
//
//
//
//
sf::ConvexShape* setObstacles(const int n) {
    sf::ConvexShape* obstacles = new sf::ConvexShape[n];
    for (int i = 0; i < n; i++) {
        obstacles[i].setPointCount(3);
        obstacles[i].setPoint(0, sf::Vector2f(0, 90));
        obstacles[i].setPoint(1, sf::Vector2f(80, 90));
        obstacles[i].setPoint(2, sf::Vector2f(40, 0));
        obstacles[i].setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
    }
    return obstacles;
}
//
//
//
//
void saveRun(std::string name, int score) {
	std::cout << std::endl << "Saving run for " << name << " with score " << score << std::endl;
}
//
//
//
//
void loadScores() {

}

void enterName(sf::RenderWindow& window) {

	sf::Text entername(font, "ENTER NAME:");
	entername.setPosition({ 200, 250 });

    sf::String userInput;
	sf::Text userinput(font, "");
	userinput.setPosition({ 450, 250 });


    std::string playerName = userInput.toAnsiString();

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>())
            {
                if (keyPressed->unicode == 8) {
                    if (!userInput.isEmpty())
                    {
                        userInput.erase(userInput.getSize() - 1);
                        playerName = userInput.toAnsiString();
                    }
                }

                else if (keyPressed->unicode < 128 && keyPressed -> unicode != 13) // Ensure it's an ASCII character
                {
                    userInput += static_cast<char>(keyPressed->unicode);
                    playerName = userInput.toAnsiString();
				}

                else {
                    if (keyPressed->unicode == 13) { // Enter key
                        std::cout << "Player Name: " << playerName << std::endl;
                        int score = game(window);
						saveRun(playerName, score);
                        return;
					}
                }
            }

			userinput.setString(playerName);
        }


        window.clear();
        window.draw(entername);
        window.draw(userinput);
		window.display();
    }

}