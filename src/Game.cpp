#include <SFML/Graphics.hpp>
#include "Snake.cpp"
#include "Food.cpp"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();

    sf::RenderWindow window;
    Snake snake;
    Food food;
};

Game::Game()
    : window(sf::VideoMode(800, 600), "Snake Game"), snake(), food() {
    window.setFramerateLimit(60);
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update() {
    // Nothing yet
}

void Game::render() {
    window.clear(sf::Color::Black);

    // Draw snake head
    window.draw(snake.getHead());

    // Draw food
    window.draw(food.getShape());

    window.display();
}