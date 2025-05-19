#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace sf;

enum GameMode { NONE, ONE_PLAYER, TWO_PLAYER };
enum MenuResult { MENU_NONE, MENU_ONE, MENU_TWO, MENU_OPTIONS, MENU_EXIT };

struct ResolutionOption {
    string label;
    Vector2u size;
};

MenuResult showMenu(RenderWindow& window, Font& font, Texture& backgroundTexture, Vector2u& windowSize);
bool showOptions(RenderWindow& window, Font& font, Texture& backgroundTexture, Vector2u& windowSize);
bool runGame(RenderWindow& window, Font& font, Texture& backgroundTexture, Texture& playerTexture, Texture& obstacleTexture, GameMode mode, sf::Sound& engineSound, sf::Sound& secondSound);
void showEndScreen(RenderWindow& window, Font& font);

int main()
{
    srand(static_cast<unsigned>(time(0)));

    Vector2u windowSize(1500, 1000);
    RenderWindow window(VideoMode(windowSize.x, windowSize.y), "Drunk Drivers");

    Texture backgroundTexture, playerTexture, obstacleTexture;
    Font font;

    if (!backgroundTexture.loadFromFile("C:\\Users\\georg\\Desktop\\game ess\\ChatGPTimag.png") ||
        !playerTexture.loadFromFile("C:\\Users\\georg\\Desktop\\game ess\\blue.png") ||
        !obstacleTexture.loadFromFile("C:\\Users\\georg\\Desktop\\game ess\\black.png") ||
        !font.loadFromFile("C:\\Users\\georg\\Desktop\\game ess\\slkscre.ttf")) {
        cerr << "Error: Could not load one or more resources." << endl;
        return -1;
    }

    SoundBuffer engineBuffer;
    if (!engineBuffer.loadFromFile("C:\\Users\\georg\\Desktop\\game ess\\starting_engine(1.wav")) {
        cerr << "Error: Could not load engine sound." << endl;
        return -1;
    }
    Sound engineSound;
    engineSound.setBuffer(engineBuffer);
    engineSound.setLoop(false);
    engineSound.setVolume(50.f);

    SoundBuffer secondBuffer;
    if (!secondBuffer.loadFromFile("C:\\Users\\georg\\Desktop\\game ess\\audiomass-output2.wav")) {
        cerr << "Error: Could not load second sound." << endl;
        return -1;
    }
    Sound secondSound;
    secondSound.setBuffer(secondBuffer);
    secondSound.setLoop(true);
    secondSound.setVolume(50.f);

    while (window.isOpen())
    {
        windowSize = window.getSize();

        MenuResult menuResult = showMenu(window, font, backgroundTexture, windowSize);
        if (menuResult == MENU_EXIT || !window.isOpen())
            break;
        if (menuResult == MENU_OPTIONS)
        {
            if (!showOptions(window, font, backgroundTexture, windowSize))
                break;
            window.create(VideoMode(windowSize.x, windowSize.y), "Drunk Drivers");
            window.setView(View(FloatRect(0, 0, static_cast<float>(windowSize.x), static_cast<float>(windowSize.y))));
            continue;
        }

        GameMode mode = (menuResult == MENU_ONE) ? ONE_PLAYER : (menuResult == MENU_TWO) ? TWO_PLAYER : NONE;
        if (mode == NONE)
            break;

        secondSound.stop();

        engineSound.setLoop(false);
        engineSound.play();
        secondSound.setLoop(true);
        secondSound.play();

        bool survived = runGame(window, font, backgroundTexture, playerTexture, obstacleTexture, mode, engineSound, secondSound);

        if (!survived)
        {
            showEndScreen(window, font);
        }
    }

    return 0;
}

MenuResult showMenu(RenderWindow& window, Font& font, Texture& backgroundTexture, Vector2u& windowSize)
{
    while (window.isOpen())
    {
        windowSize = window.getSize();
        Sprite background(backgroundTexture);
        Vector2u textureSize = backgroundTexture.getSize();
        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;                //fortwsh to background kai rythmish tis klimakas
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
        background.setScale(scaleX, scaleY);

        unsigned int titleFontSize = static_cast<unsigned int>(windowSize.y * 0.07f);
        unsigned int buttonFontSize = static_cast<unsigned int>(windowSize.y * 0.05f);
        float centerX = windowSize.x / 2.f;
        float startY = windowSize.y * 0.25f;
        float buttonSpacing = windowSize.y * 0.12f;

        Text title("Drunk Drivers", font, titleFontSize);
        title.setFillColor(Color::Yellow);
        title.setOrigin(title.getLocalBounds().width / 2.f, title.getLocalBounds().height / 2.f);           //dhmiourgia titlou
        title.setPosition(centerX, startY);

        Text onePlayer("1 Player", font, buttonFontSize);
        onePlayer.setFillColor(Color::White);
        onePlayer.setOrigin(onePlayer.getLocalBounds().width / 2.f, onePlayer.getLocalBounds().height / 2.f);       //dhmiourgia epiloghs prwtou paikti
        onePlayer.setPosition(centerX, startY + buttonSpacing);

        Text twoPlayer("2 Players", font, buttonFontSize);
        twoPlayer.setFillColor(Color::White);
        twoPlayer.setOrigin(twoPlayer.getLocalBounds().width / 2.f, twoPlayer.getLocalBounds().height / 2.f);       //dhmiourgia epiloghs deuterou paikti
        twoPlayer.setPosition(centerX, startY + buttonSpacing * 2);

        Text options("Options", font, buttonFontSize);
        options.setFillColor(Color::White);
        options.setOrigin(options.getLocalBounds().width / 2.f, options.getLocalBounds().height / 2.f);     //dhmiourgia epiloghs options
        options.setPosition(centerX, startY + buttonSpacing * 3);

        bool menuActive = true;
        while (menuActive && window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    return MENU_EXIT;

                if (event.type == Event::Resized)
                {
                    windowSize = window.getSize();
                    menuActive = false;
                    break;
                }

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
                {
                    float mouseX = static_cast<float>(Mouse::getPosition(window).x);
                    float mouseY = static_cast<float>(Mouse::getPosition(window).y);
                    if (onePlayer.getGlobalBounds().contains(mouseX, mouseY))
                        return MENU_ONE;
                    if (twoPlayer.getGlobalBounds().contains(mouseX, mouseY))                   //anagnwrish 8eshs mouse 
                        return MENU_TWO;
                    if (options.getGlobalBounds().contains(mouseX, mouseY))
                        return MENU_OPTIONS;
                }
            }

            float mouseX = static_cast<float>(Mouse::getPosition(window).x);
            float mouseY = static_cast<float>(Mouse::getPosition(window).y);
            onePlayer.setFillColor(onePlayer.getGlobalBounds().contains(mouseX, mouseY) ? Color::Red : Color::White);
            twoPlayer.setFillColor(twoPlayer.getGlobalBounds().contains(mouseX, mouseY) ? Color::Red : Color::White);
            options.setFillColor(options.getGlobalBounds().contains(mouseX, mouseY) ? Color::Red : Color::White);

            window.clear();
            window.draw(background);
            window.draw(title);
            window.draw(onePlayer);             //fortwsh background,titlou kai epilogwn
            window.draw(twoPlayer);
            window.draw(options);
            window.display();
        }
    }
    return MENU_EXIT;
}

bool showOptions(RenderWindow& window, Font& font, Texture& backgroundTexture, Vector2u& windowSize)        //synarthsh epiloghs resolution
{
    vector<ResolutionOption> resolutions = {
        {"800 x 600", {800, 600}},
        {"1280 x 720", {1280, 720}},
        {"1500 x 1000", {1500, 1000}},
        {"1920 x 1080", {1920, 1080}}
    };

    while (window.isOpen())
    {
        windowSize = window.getSize();
        Sprite background(backgroundTexture);
        Vector2u textureSize = backgroundTexture.getSize();
        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
        background.setScale(scaleX, scaleY);

        unsigned int titleFontSize = static_cast<unsigned int>(windowSize.y * 0.06f);
        unsigned int buttonFontSize = static_cast<unsigned int>(windowSize.y * 0.045f);
        float centerX = windowSize.x / 2.f;
        float startY = windowSize.y * 0.25f;
        float buttonSpacing = windowSize.y * 0.12f;

        Text title("Select Resolution", font, titleFontSize);
        title.setFillColor(Color::Yellow);
        title.setOrigin(title.getLocalBounds().width / 2.f, title.getLocalBounds().height / 2.f);
        title.setPosition(centerX, startY);

        vector<Text> buttons;
        for (size_t i = 0; i < resolutions.size(); ++i)
        {
            Text btn(resolutions[i].label, font, buttonFontSize);
            btn.setFillColor(Color::White);
            btn.setOrigin(btn.getLocalBounds().width / 2.f, btn.getLocalBounds().height / 2.f);
            btn.setPosition(centerX, startY + buttonSpacing * (i + 1));
            buttons.push_back(btn);
        }

        bool optionsActive = true;
        while (optionsActive && window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    return false;

                if (event.type == Event::Resized)
                {
                    windowSize = window.getSize();
                    optionsActive = false;
                    break;
                }

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
                {
                    float mouseX = static_cast<float>(Mouse::getPosition(window).x);
                    float mouseY = static_cast<float>(Mouse::getPosition(window).y);
                    for (size_t i = 0; i < buttons.size(); ++i)
                    {
                        if (buttons[i].getGlobalBounds().contains(mouseX, mouseY))
                        {
                            windowSize = resolutions[i].size;
                            return true;
                        }
                    }
                }
            }

            float mouseX = static_cast<float>(Mouse::getPosition(window).x);
            float mouseY = static_cast<float>(Mouse::getPosition(window).y);
            for (size_t i = 0; i < buttons.size(); ++i)
            {
                if (buttons[i].getGlobalBounds().contains(mouseX, mouseY))
                    buttons[i].setFillColor(Color::Red);
                else
                    buttons[i].setFillColor(Color::White);
            }

            window.clear();
            window.draw(background);
            window.draw(title);
            for (const auto& btn : buttons)
                window.draw(btn);
            window.display();
        }
    }
    return false;
}

bool runGame(RenderWindow& window, Font& font, Texture& backgroundTexture, Texture& playerTexture, Texture& obstacleTexture, GameMode mode, sf::Sound& engineSound, sf::Sound& secondSound)
{
    Vector2u windowSize = window.getSize();
    Vector2u textureSize = backgroundTexture.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    float roadLeft = windowSize.x * 0.11f;
    float roadRight = windowSize.x * 0.89f;
    float roadTop = 0.f;
    float roadBottom = static_cast<float>(windowSize.y);

    Sprite background1(backgroundTexture);
    Sprite background2(backgroundTexture);
    background1.setScale(scaleX, scaleY);               //sxediasmos background
    background2.setScale(scaleX, scaleY);
    background1.setPosition(0.f, 0.f);
    background2.setPosition(0.f, -static_cast<float>(windowSize.y));

    Sprite player1(playerTexture);
    player1.setScale(scaleX / 13, scaleY / 10);//<----------------------
    FloatRect p1Bounds = player1.getGlobalBounds();                     //sxediasmos paikth 1
    player1.setPosition(
        windowSize.x / 2.f - p1Bounds.width / 2.f,
        windowSize.y * 0.8f - p1Bounds.height / 2.f
    );

    Sprite player2(playerTexture);
    player2.setScale(scaleX / 13, scaleY / 10);//<------------------------
    FloatRect p2Bounds = player2.getGlobalBounds();                             //sxediasmos paikth 2
    player2.setPosition(
        windowSize.x / 2.f - p2Bounds.width / 2.f,
        windowSize.y * 0.9f - p2Bounds.height / 2.f
    );

    vector<Sprite> obstacles;
    Clock spawnClock, gameClock, deltaClock;
    float spawnInterval = 1.0f;                         //arxikes times obstacle
    float obstacleSpeed = 400.f;
    float speedIncreaseRate = 5.0f;

    Text counterText;
    counterText.setFont(font);
    counterText.setCharacterSize(24);
    counterText.setFillColor(Color::White);
    counterText.setPosition(10.f, 10.f);

    float backgroundSpeed = 100.f;
    float backgroundSpeedMax = 500.f;               //times gia enallagh background
    float backgroundAcceleration = 200.f;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) {
                engineSound.stop();
                secondSound.stop();
                window.close();
                return false;
            }
        }

        float deltaTime = deltaClock.restart().asSeconds();

        if (backgroundSpeed < backgroundSpeedMax)
            backgroundSpeed = min(backgroundSpeed + backgroundAcceleration * deltaTime, backgroundSpeedMax);

        background1.move(0.f, backgroundSpeed * deltaTime);
        background2.move(0.f, backgroundSpeed * deltaTime);
        if (background1.getPosition().y >= static_cast<float>(windowSize.y))
            background1.setPosition(0.f, background2.getPosition().y - static_cast<float>(windowSize.y));       //eikonikh allagh background 
        if (background2.getPosition().y >= static_cast<float>(windowSize.y))
            background2.setPosition(0.f, background1.getPosition().y - static_cast<float>(windowSize.y));

        int elapsedTime = static_cast<int>(gameClock.getElapsedTime().asSeconds());
        counterText.setString("Time: " + to_string(elapsedTime) + "s");

        if (Keyboard::isKeyPressed(Keyboard::Left))
            player1.move(-300.f * deltaTime, 0.f);
        if (Keyboard::isKeyPressed(Keyboard::Right))
            player1.move(300.f * deltaTime, 0.f);                   //elegxos kinisewn paikth 1
        if (Keyboard::isKeyPressed(Keyboard::Up))
            player1.move(0.f, -300.f * deltaTime);
        if (Keyboard::isKeyPressed(Keyboard::Down))
            player1.move(0.f, 300.f * deltaTime);

        FloatRect p1Bounds = player1.getGlobalBounds();
        Vector2f p1Pos = player1.getPosition();
        p1Pos.x = max(roadLeft, min(p1Pos.x, roadRight - p1Bounds.width));
        p1Pos.y = max(roadTop, min(p1Pos.y, roadBottom - p1Bounds.height));
        player1.setPosition(p1Pos);

        if (mode == TWO_PLAYER)
        {
            if (Keyboard::isKeyPressed(Keyboard::A))
                player2.move(-300.f * deltaTime, 0.f);
            if (Keyboard::isKeyPressed(Keyboard::D))
                player2.move(300.f * deltaTime, 0.f);           //elegxos kinisewn paikth 2
            if (Keyboard::isKeyPressed(Keyboard::W))
                player2.move(0.f, -300.f * deltaTime);
            if (Keyboard::isKeyPressed(Keyboard::S))
                player2.move(0.f, 300.f * deltaTime);

            FloatRect p2Bounds = player2.getGlobalBounds();
            Vector2f p2Pos = player2.getPosition();
            p2Pos.x = max(roadLeft, min(p2Pos.x, roadRight - p2Bounds.width));
            p2Pos.y = max(roadTop, min(p2Pos.y, roadBottom - p2Bounds.height));
            player2.setPosition(p2Pos);
        }

        if (spawnClock.getElapsedTime().asSeconds() > spawnInterval)
        {
            if (spawnInterval >= 0.3f) spawnInterval -= 0.01f;
            Sprite obstacle(obstacleTexture);
            obstacle.setScale(scaleX / 13, scaleY / 10);//<----------------------

            float obstacleWidth = obstacle.getGlobalBounds().width;
            float minX = roadLeft;
            float maxX = roadRight - obstacleWidth;

            float spawnX = minX + static_cast<float>(rand()) / RAND_MAX * (maxX - minX);            //ru8miseis gia kinhsh obstacle

            bool overlap = false;
            float minGapY = 200.f;
            for (const auto& obs : obstacles) {
                float ox = obs.getPosition().x;
                float oy = obs.getPosition().y;
                if (abs(ox - spawnX) < obstacleWidth && oy < minGapY) {
                    overlap = true;
                    break;
                }
            }

            if (!overlap) {
                obstacle.setPosition(spawnX, -50.f);
                obstacles.push_back(obstacle);
                spawnClock.restart();
            }
        }

        obstacleSpeed += speedIncreaseRate * deltaTime;

        for (auto& obstacle : obstacles)
        {
            obstacle.move(0.f, obstacleSpeed * deltaTime);
            FloatRect oBounds = obstacle.getGlobalBounds();
            Vector2f oPos = obstacle.getPosition();
            oPos.x = max(roadLeft, min(oPos.x, roadRight - oBounds.width));
            obstacle.setPosition(oPos);
        }

        for (const auto& obstacle : obstacles)
        {
            /* if (player1.getGlobalBounds().intersects(obstacle.getGlobalBounds()))
             {
                 engineSound.stop();
                 secondSound.stop();
                 return false;                                                                                   //elegxos gia syngkroush me to empodio
             }
             if (mode == TWO_PLAYER && player2.getGlobalBounds().intersects(obstacle.getGlobalBounds()))
             {
                 engineSound.stop();
                 secondSound.stop();
                 return false;
             }*/

            FloatRect bounds = player1.getGlobalBounds();

            float shrinkFactor = 0.2f;
            FloatRect customBounds(
                bounds.left + bounds.width * shrinkFactor / 2.f,
                bounds.top + bounds.height * shrinkFactor / 2.f,                    //deuteros tropos elegxou syngkroushs me mikrotero hitbox
                bounds.width * (1.f - shrinkFactor),
                bounds.height * (1.f - shrinkFactor)
            );

            FloatRect obstacleBounds = obstacle.getGlobalBounds();
            FloatRect customObstacleBounds(
                obstacleBounds.left + obstacleBounds.width * shrinkFactor / 2.f,
                obstacleBounds.top + obstacleBounds.height * shrinkFactor / 2.f,
                obstacleBounds.width * (1.f - shrinkFactor),
                obstacleBounds.height * (1.f - shrinkFactor)
            );

            if (customBounds.intersects(customObstacleBounds))
            {
                engineSound.stop();
                secondSound.stop();
                return false;
            }
        }

        obstacles.erase(remove_if(obstacles.begin(), obstacles.end(),
            [windowSize](const Sprite& obstacle) {
                return obstacle.getPosition().y > windowSize.y;
            }),
            obstacles.end());

        window.clear();
        window.draw(background1);
        window.draw(background2);

        window.draw(player1);
        if (mode == TWO_PLAYER)
            window.draw(player2);
        for (const auto& obstacle : obstacles)
            window.draw(obstacle);
        window.draw(counterText);
        window.display();
    }

    engineSound.stop();
    secondSound.stop();
    return true;
}

void showEndScreen(RenderWindow& window, Font& font)
{
    sleep(seconds(0.3));
    Text endText("Game Over", font, 60);
    endText.setFillColor(Color::Red);                       //emfansh tou game over
    FloatRect bounds = endText.getLocalBounds();
    endText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    endText.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

    window.clear();
    window.draw(endText);
    window.display();
    sleep(seconds(2));
    window.close();
}
