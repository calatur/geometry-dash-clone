#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <optional>
#include<iostream>
#include<string>
#include<fstream>
#include<ctime>
#include<cstdlib>
#include<cstring>

int game(sf::RenderWindow& window);
void enterName(sf::RenderWindow& window);
void mmenu(sf::RenderWindow& window);
int score(sf::Clock& clock);
void saveRun(std::string , int score);
bool checkCollision(const sf::Sprite& player, const sf::ConvexShape& obstacle);
sf::ConvexShape* setObstacles(const int n);
int quitGame(sf::RenderWindow& window);
void lost(sf::RenderWindow& window, int score);
void highest_score(char** name, int score[], int count);
void Records(sf::RenderWindow& window);

sf::Font font;

int main()
{
    srand(time(0));
    if (!(font.openFromFile("Assets\\Fonts\\8bitOperatorPlus8-Bold.ttf"))) {
        std::cout << "could not open font file";
    }

	std::ifstream fin("Scores.xyz", std::ios::binary);
    if (!fin) {
        std::cout << "No file!" << std::endl;
        std::ofstream errorfile("Scores.xyz", std::ios::out);
        errorfile.close();
    }
    fin.close();

    sf::RenderWindow window(sf::VideoMode({ 1000, 600 }), "Geomentry Dash Endless");
    window.setKeyRepeatEnabled(false);
    mmenu(window);
    window.close();


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
                    if (quitGame(window)) {
                        return;
                    }
                    
                }

                if (keyPressed->scancode == sf::Keyboard::Scan::Z)
                {
					enterName(window);
                }

                if (keyPressed->scancode == sf::Keyboard::Scan::X)
                {
					Records(window);
                }
            }
        }

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

		cscore = score(scoreclock);
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
                if (keyPressed->scancode == sf::Keyboard::Scan::C){
                    scoreclock.stop();
                    if (quitGame(window)) {
						delete[] obstacle;
                        std::cout << "deleted obstacles" << std::endl;
                        return cscore;
                    }
                    scoreclock.start();
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
				lost(window, cscore);
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
    std::ofstream fout("Scores.xyz", std::ios::app | std::ios::binary);
    char arr[30] = { 0 };
    int len = std::min(29, (int)name.size());
    memcpy(arr, name.c_str(), len);
    fout.write(arr, 30);
    fout.write((char*)&score, sizeof(score));
    fout.close();
}
//
//
//
//
void enterName(sf::RenderWindow& window) {

    sf::Texture btexture("Assets\\Sprites\\background.png");
    btexture.setSmooth(true);
    sf::Sprite background(btexture);
    background.setPosition({ 0.f, 0.f });

	sf::Text entername(font, "ENTER NAME:");
	entername.setPosition({ 200, 250 });

    sf::String userInput;
	sf::Text userinput(font, "");
	userinput.setPosition({ 450, 250 });

    bool ignoreFirstInput = true;
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

                if (ignoreFirstInput) {
                    ignoreFirstInput = false;
                    continue;
                }



                if (keyPressed->unicode == 8) {
                    if (!userInput.isEmpty())
                    {
                        userInput.erase(userInput.getSize() - 1);
                        playerName = userInput.toAnsiString();
                    }
                }

                else if (keyPressed->unicode < 128 && keyPressed -> unicode != 13)
                {
                    userInput += static_cast<char>(keyPressed->unicode);
                    playerName = userInput.toAnsiString();
				}

                else {
                    if (keyPressed->unicode == 13) {
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
        window.draw(background);
        window.draw(entername);
        window.draw(userinput);
		window.display();
    }

}
//
//
//
//
int quitGame(sf::RenderWindow& window){
    sf::Texture btexture("Assets\\Sprites\\background.png");
    btexture.setSmooth(true);
    sf::Sprite background(btexture);
    background.setPosition({ 0.f, 0.f });

    sf::Text quittext(font, "QUIT GAME?");
    sf::Text confirmtext(font, "YES (Z)\tNo (C)");

    quittext.setPosition({ 400.f, 250.f });
    confirmtext.setPosition({ 350.f, 300.f });

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scan::C) {
                    return 0;
                }
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scan::Z) {
                    return 1;
                    //window.close();
                }
            }

            window.clear();
            window.draw(background);
            window.draw(quittext);
			window.draw(confirmtext);
            window.display();

        }
    }
}
//
//
//
//
void lost(sf::RenderWindow& window, int score) {
    sf::Text losttext(font, "YOU LOST!");
    sf::Text scoretext(font, "SCORE: " + std::to_string(score));
    sf::Text menutext(font, "PRESS ANY KEY TO RETURN TO MENU");
    losttext.setPosition({ 400.f, 200.f });
    scoretext.setPosition({ 400.f, 250.f });
    menutext.setPosition({ 200.f, 300.f });
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                return;
            }
        }

        window.clear();
        window.draw(losttext);
        window.draw(scoretext);
        window.draw(menutext);
        window.display();
    }
}






void Records(sf::RenderWindow& window) {
    sf::Texture btexture("Assets\\Sprites\\background.png");
    btexture.setSmooth(true);
    sf::Sprite background(btexture);
    background.setPosition({ 0.f, 0.f });

    int* scores = nullptr;
    char** names = nullptr;
    int count = 0;


    char ch[30];
    std::ifstream fin("Scores.xyz", std::ios::binary);
    if (!fin) { 
        std::cout << "No file!" << std::endl;
		std::ofstream errorfile("Scores.xyz", std::ios::out);
        return; 
    }

    while (fin.read((char*)ch, 30)) {
        int sc;
        if (!fin.read((char*)&sc, sizeof(sc))) break;

        int* newScores = new int[count + 1];
        char** newNames = new char* [count + 1];

        for (int i = 0; i < count; i++) {
            newScores[i] = scores[i];
            newNames[i] = new char[strlen(names[i]) + 1];
            strcpy_s(newNames[i], strlen(names[i]) + 1, names[i]);
        }

        newScores[count] = sc;
        newNames[count] = new char[strlen(ch) + 1];
        strcpy_s(newNames[count], strlen(ch) + 1, ch);

        delete[] scores;
        for (int i = 0; i < count; i++) delete[] names[i];
        delete[] names;

        scores = newScores;
        names = newNames;
        count++;
    }
    fin.close();

    highest_score(names, scores, count);

    sf::Text title(font, "HIGH SCORES", 50);
    title.setFillColor(sf::Color(210, 200, 23));
    title.setPosition({ 350.f, 20.f });
    sf::Text info(font, "Press C to return to Menu", 25);
    info.setPosition({ 300.f, 550.f });

    sf::Text scoreTexts[5] = { sf::Text(font), sf::Text(font), sf::Text(font), sf::Text(font), sf::Text(font) };
    int displayCount = std::min(5, count);
    for (int i = 0; i < displayCount; i++) {
        std::string s = std::string(names[i]) + " : " + std::to_string(scores[i]);
        scoreTexts[i].setFont(font);
        scoreTexts[i].setString(s);
        scoreTexts[i].setCharacterSize(30);
        scoreTexts[i].setPosition({ 350.f, 100.f + i * 50.f });
    }



while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) window.close();

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scan::C) {
                for (int i = 0; i < count; i++) delete[] names[i];
                delete[] names;
                delete[] scores;
                return;
            }
        }
    }

    window.clear();
    window.draw(background);
    window.draw(title);
    window.draw(info);
    for (int i = 0; i < displayCount; i++) window.draw(scoreTexts[i]);
    window.display();
}
}
//
//
//
//
void highest_score(char** name, int score[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            if (score[j] < score[j + 1]) {
                std::swap(score[j], score[j + 1]);
                std::swap(name[j], name[j + 1]);
            }
        }
    }
}