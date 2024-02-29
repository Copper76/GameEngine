#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(400, 400), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.getPosition();
    shape.setFillColor(sf::Color::Green);
    float speed = 50.0f;
    float tickRate = 0.001f;

    sf::Clock clock;

    while (window.isOpen())
    {
        //setup
        float deltaTime = clock.getElapsedTime().asSeconds();
        //ensure deltaTime is bigger than zero
        if (deltaTime < tickRate)
        {
            continue;
        }

        //std::cout << deltaTime<<std::endl;
        sf::Vector2f moveAmount = sf::Vector2f(0.0f, 0.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            moveAmount -= sf::Vector2f(1.0f, 0.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            moveAmount += sf::Vector2f(1.0f, 0.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            moveAmount += sf::Vector2f(0.0f, 1.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            moveAmount -= sf::Vector2f(0.0f, 1.0f);
        }

        //handle event
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        //adjust
        shape.move(moveAmount);

        //redraw
        window.clear();
        window.draw(shape);
        window.display();

        //end of cycle
        clock.restart();
    }

    return 0;
}