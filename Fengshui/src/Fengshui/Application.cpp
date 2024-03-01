#include "Application.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <set>

namespace Fengshui
{
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

    void Application::Run()
    {
        Snake();
    }

	void Application::Snake()
	{
        //Initialize parameters
        int boardSize = 11;
        sf::RenderWindow window(sf::VideoMode(boardSize * 50.0f, boardSize * 50.0f), "SNAKE GAME");

        float step = 50.0f;
        sf::Vector2f snakeHeadPos = sf::Vector2f((boardSize / 2) * 50.0f, (boardSize / 2) * 50.0f);
        int snakeDir = 0;
        int prevDir = snakeDir;
        float stepTime = 0.5f;
        int score = 0;
        bool gameOver = false;

        sf::Font font;
        font.loadFromFile("E:/GitStuff/GameEngine/Fengshui/Font/arial.ttf");
        std::cout << font.getInfo().family << std::endl;

        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setString("SCORE: " + std::to_string(score));

        sf::Text gameOverText;
        gameOverText.setFont(font);
        gameOverText.setCharacterSize(32);
        gameOverText.setFillColor(sf::Color::White);
        gameOverText.setString("GAME OVER");

        sf::Text exitText;
        exitText.setFont(font);
        exitText.setCharacterSize(24);
        exitText.setFillColor(sf::Color::White);
        exitText.setString("PRESS ESCAPE TO LEAVE THE GAME");

        //Initialize snake
        sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(50.f, 50.0f));
        shape.setPosition(snakeHeadPos);
        shape.setFillColor(sf::Color::Green);

        std::list<sf::RectangleShape> snakeBody = std::list<sf::RectangleShape>();
        snakeBody.push_back(shape);
        std::list<sf::Vector2f> snakeBodyPos = std::list<sf::Vector2f>();
        snakeBodyPos.push_back(snakeHeadPos);

        //Initialize fruit
        sf::RectangleShape fruit = sf::RectangleShape(sf::Vector2f(20.f, 20.0f));
        fruit.setFillColor(sf::Color::Red);
        sf::Vector2f fruitPos;
        do
        {
            fruitPos = sf::Vector2f((rand() % boardSize) * 50.0f, (rand() % boardSize) * 50.0f);
        } while (std::find(snakeBodyPos.begin(), snakeBodyPos.end(), fruitPos) != snakeBodyPos.end() && snakeBody.size() < boardSize * boardSize);
        fruit.setPosition(fruitPos + sf::Vector2f(15.0f, 15.0f));


        sf::Clock clock;

        while (window.isOpen())
        {
            //setup
            float deltaTime = clock.getElapsedTime().asSeconds();

            //std::cout << deltaTime<<std::endl;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && prevDir != 1)
            {
                snakeDir = 3;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && prevDir != 3)
            {
                snakeDir = 1;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && prevDir != 0)
            {
                snakeDir = 2;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && prevDir != 2)
            {
                snakeDir = 0;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }

            //handle event
            sf::Event event;

            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }


            //adjust
            if (deltaTime > stepTime && !gameOver)
            {
                prevDir = snakeDir;
                switch (snakeDir)
                {
                case 0:
                    snakeHeadPos = snakeHeadPos - sf::Vector2f(0.0f, step);
                    break;
                case 1:
                    snakeHeadPos = snakeHeadPos + sf::Vector2f(step, 0.0f);
                    break;
                case 2:
                    snakeHeadPos = snakeHeadPos + sf::Vector2f(0.0f, step);
                    break;
                case 3:
                    snakeHeadPos = snakeHeadPos - sf::Vector2f(step, 0.0f);
                    break;
                default:
                    break;
                }
                if ((snakeHeadPos.x < 0.0f || snakeHeadPos.x >(boardSize - 1) * 50.0f || snakeHeadPos.y < 0.0f || snakeHeadPos.y >(boardSize - 1) * 50.0f || snakeBody.size() >= boardSize * boardSize || std::find(snakeBodyPos.begin(), snakeBodyPos.end(), snakeHeadPos) != snakeBodyPos.end()) && !gameOver)
                {
                    gameOver = true;
                    scoreText.setString("YOU HAVE ACHIEVED " + std::to_string(score) + " POINTS!");
                    sf::FloatRect bounds = scoreText.getGlobalBounds();
                    scoreText.setPosition(boardSize * 25.0f - bounds.width * 0.5f, boardSize * 25.0f - bounds.height * 0.5f + 50.0f);
                    sf::FloatRect gameOverBounds = gameOverText.getGlobalBounds();
                    gameOverText.setPosition(boardSize * 25.0f - gameOverBounds.width * 0.5f, boardSize * 25.0f - gameOverBounds.height * 0.5f - 50.0f);
                    gameOverBounds = exitText.getGlobalBounds();
                    exitText.setPosition(boardSize * 25.0f - gameOverBounds.width * 0.5f, boardSize * 25.0f + bounds.height * 0.5f + 100.0f);
                }
                else
                {
                    if (snakeHeadPos == fruitPos)
                    {
                        score += 10;
                        scoreText.setString("SCORE: " + std::to_string(score));
                        shape.setPosition(snakeHeadPos);
                        snakeBody.push_back(shape);
                        snakeBodyPos.push_back(snakeHeadPos);
                        do
                        {
                            fruitPos = sf::Vector2f((rand() % boardSize) * 50.0f, (rand() % boardSize) * 50.0f);
                        } while (std::find(snakeBodyPos.begin(), snakeBodyPos.end(), fruitPos) != snakeBodyPos.end() && snakeBody.size() < boardSize * boardSize);
                        fruit.setPosition(fruitPos + sf::Vector2f(15.0f, 15.0f));
                    }
                    else
                    {
                        sf::RectangleShape tail = snakeBody.front();
                        snakeBody.pop_front();
                        snakeBodyPos.pop_front();
                        tail.setPosition(snakeHeadPos);
                        snakeBody.push_back(tail);
                        snakeBodyPos.push_back(snakeHeadPos);
                    }
                }

                //redraw
                window.clear();
                if (!gameOver)
                {
                    for (sf::RectangleShape s : snakeBody)
                    {
                        window.draw(s);
                    }
                    if (snakeBody.size() < boardSize * boardSize)
                    {
                        window.draw(fruit);
                    }
                    window.draw(scoreText);
                }
                else
                {
                    window.draw(scoreText);
                    window.draw(gameOverText);
                    window.draw(exitText);
                }
                window.display();

                //reset timer
                clock.restart();
            }
        }

	}
}
