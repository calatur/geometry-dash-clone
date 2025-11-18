
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <optional>
#include<iostream>
#include<string>
#include<fstream>
#include<ctime>
#include<cstdlib>
void game(sf::RenderWindow& window);
void mmenu(sf::RenderWindow& window);
int score(sf::Clock& clock);
void saveRun(int score, sf::RenderWindow& window);
void loadScores();
bool checkCollision(const sf::Sprite& player, const sf::ConvexShape& obstacle);
void loadScores(sf::RenderWindow& window);
sf::Font font;

int cscore = 0;
int main()
{
    srand(time(NULL));
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
                    game(window);
                }
                if (keyPressed->scancode == sf::Keyboard::Scan::X)
                {
                    saveRun(cscore, window);
                }

            }
        }

        window.clear();
        window.draw(mainmenu);
        window.display();
    }
}

void game(sf::RenderWindow& window) {

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

    sf::Texture otexture[3];
    std::string paths[3] = { "Assets\\Sounds\\obstacle_0.jpeg", "Assets\\Sounds\\obstacle_1.jpeg", "Assets\\Sounds\\obstacle_2.jpeg"
    };

    for (int i = 0; i < 3; i++) {
        if (!otexture[i].loadFromFile(paths[i])) {
            std::cerr << "Could not load texture file: " << std::endl;
            return;
        }
        otexture[i].setSmooth(true);
    }


    sf::ConvexShape obstacle[3];
    obstacle[0].setPointCount(4);
    obstacle[0].setPoint(0, sf::Vector2f(0, 0));
    obstacle[0].setPoint(1, sf::Vector2f(100, 100));
    obstacle[0].setPoint(2, sf::Vector2f(0, 100));
    obstacle[0].setPoint(3, sf::Vector2f(100, 0));
    obstacle[0].setTexture(&otexture[0]);

    obstacle[1].setPointCount(4);
    obstacle[1].setPoint(0, sf::Vector2f(0, 0));
    obstacle[1].setPoint(1, sf::Vector2f(100, 100));
    obstacle[1].setPoint(2, sf::Vector2f(0, 100));
    obstacle[1].setPoint(3, sf::Vector2f(100, 0));
    obstacle[1].setTexture(&otexture[1]);

    obstacle[2].setPointCount(4);
    obstacle[2].setPoint(0, sf::Vector2f(0, 0));
    obstacle[2].setPoint(1, sf::Vector2f(100, 100));
    obstacle[2].setPoint(2, sf::Vector2f(0, 100));
    obstacle[2].setPoint(3, sf::Vector2f(100, 0));
    obstacle[2].setTexture(&otexture[2]);

    sf::Vector2f positions[3];
    positions[0].x = 1000;
    positions[1].x = 1500;
    positions[2].x = 2000;

    positions[0].y = 416;
    positions[1].y = 416;
    positions[2].y = 416;


    obstacle[0].setPosition({ 1000, 416 });
    obstacle[1].setPosition({ 1600 , 416 });
    obstacle[2].setPosition({ 2100, 416 });

    float obsSpeed = 550.f;
    int min = 475;

    bool canjump = false,
        canjump1 = false;

    sf::Clock clock, scoreclock;
    std::string sscore = "";
    //int cscore = 0;

    sf::Text scoretext(font, sscore);
    sf::Text scoreheader(font, "SCORE: ");
    sf::Text controls(font, "Z:   JUMP\nC:   MENU");
    scoreheader.setPosition({ 25.f, 100.f });
    scoretext.setPosition({ 150.f, 100.f });
    controls.setPosition({ 25.f, 10.f });


    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("Assets\\Sounds\\jumpy_jumpy.wav")) {
        std::cout << "Sound file not found!" << std::endl;
        return;
    }
    sf::Sound jump(buffer);

    bool spaces = true;

    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();

        cscore = score(scoreclock); //score calculation in cscore, store cscore when run ends (pressed C or ran into obstacles)
        sscore = std::to_string(cscore);
        scoretext.setString(sscore);

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scan::C) {
                    window.close();
                }
                if (keyPressed->scancode == sf::Keyboard::Scan::X) {
                    //save here
                    //std::cout << "Hello" << std::endl;
                    //instead of loading game, display an intermediate screen (new function) to ask for the run's name
                    //run's name will be a character array of fixed size
                    //Then pass that name to game function to save the run under that name.
                    //That name will be passed to the saveRun() function along with the score in that run
                    //to store in a binary file

                    //when the game function returns, return the intermediate screen to main menu
                }
            }
        }

        if (player.getPosition().y >= groundlevel - 5.f) {
            player.setRotation(sf::degrees(0.f));
        }

        if (canjump) {
            player.move({ 0.0f, (-(jumpspeed)*deltaTime) });
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
            jump.play();
        }


        int i;

        for (i = 0; i < 3; i++) {
            obstacle[i].move({ -obsSpeed * deltaTime, 0 });
            positions[i].x -= obsSpeed * deltaTime;
        }


        if (obstacle[0].getPosition().x < -100) {
            positions[0].x = positions[2].x + rand() % 200 + min;;
            obstacle[0].setPosition(positions[0]);
        }

        if (obstacle[1].getPosition().x < -100) {
            positions[1].x = positions[0].x + rand() % 200 + min;
            obstacle[1].setPosition(positions[1]);
        }

        if (obstacle[2].getPosition().x < -100) {
            positions[2].x = positions[1].x + rand() % 200 + min;
            obstacle[2].setPosition(positions[2]);
        }


        for (i = 0; i < 3; i++) {
            if (checkCollision(player, obstacle[i])) {
                return;
            }
        }

        window.clear();
        window.draw(background);
        window.draw(controls);
        window.draw(scoreheader);
        window.draw(scoretext);
        window.draw(player);
        for (i = 0; i < 3; i++)
            window.draw(obstacle[i]);

        window.display();
    }
}

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


int score(sf::Clock& clock) {

    int bonus = 1;
    int scale = 50;

    if (clock.getElapsedTime().asMilliseconds() / scale > 100) {
        bonus = 2;
    }
    else if (clock.getElapsedTime().asMilliseconds() / scale > 300) {
        bonus = 3;
    }
    else if (clock.getElapsedTime().asMilliseconds() / scale > 1000) {
        bonus = 4;
    }

    return (int)(clock.getElapsedTime().asMilliseconds() / scale * bonus);
}

void saveRun(int cscore, sf::RenderWindow& window) {
    sf::Texture btexture("Assets\\Sprites\\background.png");
    btexture.setSmooth(true);
    sf::Sprite background(btexture);
    background.setPosition({ 0.f, 0.f });
    sf::Text message(font, "ENTER RUNNER'S NAME: ");
    sf::Text message1(font, "PRESS R TO SEE ALL RECORDS! ");
    message.setPosition(sf::Vector2f{ 288, 250 });
    message1.setPosition(sf::Vector2f{ 240, 290 });

    char name;
    //read();




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
                if (keyPressed->scancode == sf::Keyboard::Scan::R)
                {
                    loadScores(window);
                }
            }
        }
        window.clear();
        window.draw(background);
        window.draw(message);
        window.draw(message1);

        window.display();

    }
}
void loadScores(sf::RenderWindow& window) {
    sf::Texture btexture("Assets\\Sprites\\background.png");
    btexture.setSmooth(true);
    sf::Sprite background(btexture);
    background.setPosition({ 0.f, 0.f });
    sf::Text title(font, "Previous Scores: ");
    title.setPosition(sf::Vector2f{ 10, 10 });

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
            }
        }
        window.clear();
        window.draw(background);
        window.draw(title);


        window.display();

    }

}
//void read() {
//    int* score1 = nullptr, n, count = 0;
//    char** name1 = nullptr;
//    char ch[30];
//
//    std::ifstream fin("Scores.xyz", std::ios::binary);
//    while (fin.read((char*)ch, 20) && fin.read((char*)&n, sizeof(n))) {
//        int* arr1 = new int[count + 1];
//        char** arr2;
//
//        for (int i = 0; i < count; i++) {
//            arr1[i] = score1[i];
//            arr2[i] = new char[strlen(name1[i]) + 1];
//            strcpy(arr2[i], name1[i]);
//        }
//
//        arr1[count] = n;
//        arr2[count] = new char[strlen(ch) + 1];
//        strcpy(arr2[count], ch);
//
//        delete[] score1;
//        for (int i = 0; i < count; i++)
//            delete[] name1[i];
//        delete[] name1;
//
//        score1 = arr1;
//        name1 = arr2;
//        count++;
//    }
//    fin.close();
//    if (count == 0) {
//        std::cout << "No scores available.\n";
//        return;
//    }
//    else {
//        highest_score(name1, score1, count);
//        display(name1, score1, count);
//    }
//    delete[] score1;
//    for (int i = 0; i < count; i++)
//        delete[] name1[i];
//    delete[] name1;
//}
//void write(char name[], int score) {
//    std::ofstream fout("Scores.xyz", std::ios::app | std::ios::binary);
//    fout.write(name, 20);
//    fout.write((char*)&score, sizeof(score));
//    fout.close();
//}
//void display(char** name, int score[], int count) {
//    if (count > 5) {
//        count = 5;
//    }
//    std::cout << "Top " << count << " Players Scores:\n";
//    for (int i = 0; i < count; i++) {
//        std::cout << name[i] << " : " << score[i] << endl;
//    }
//}
//void highest_score(char** name, int score[], int count) {
//    for (int i = 0; i < count - 1; i++) {
//        for (int j = 0; j < count - 1 - i; j++) {
//            if (score[j] < score[j + 1]) {
//                std::swap(score[j], score[j + 1]);
//                std::swap(name[j], name[j + 1]);
//            }
//        }
//    }
//}
