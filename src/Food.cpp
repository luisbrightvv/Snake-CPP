#include <SFML/Graphics.hpp>
#include <chrono>
#include <random>
#include <cmath>

namespace {
constexpr unsigned int FOOD_CELL_SIZE = 20;
constexpr unsigned int DEFAULT_WIDTH = 800;
constexpr unsigned int DEFAULT_HEIGHT = 600;
constexpr float FOOD_CELL_SIZE_F = static_cast<float>(FOOD_CELL_SIZE);
}

class Food {
public:
    Food()
        : m_shape(),
          m_rng(static_cast<std::mt19937::result_type>(
              std::chrono::steady_clock::now().time_since_epoch().count())) {
        m_shape.setRadius(FOOD_CELL_SIZE_F / 2.0f);
        m_shape.setFillColor(sf::Color::Red);
        spawn();
    }

    void spawn() {
        spawn(sf::Vector2u(DEFAULT_WIDTH, DEFAULT_HEIGHT));
    }

    void spawn(const sf::Vector2u& windowSize) {
        const unsigned int cellsX = windowSize.x / FOOD_CELL_SIZE;
        const unsigned int cellsY = windowSize.y / FOOD_CELL_SIZE;

        std::uniform_int_distribution<unsigned int> distX(0, cellsX > 0 ? cellsX - 1 : 0);
        std::uniform_int_distribution<unsigned int> distY(0, cellsY > 0 ? cellsY - 1 : 0);

        const float x = static_cast<float>(distX(m_rng) * FOOD_CELL_SIZE);
        const float y = static_cast<float>(distY(m_rng) * FOOD_CELL_SIZE);

        m_shape.setPosition(sf::Vector2f(x, y));
    }

    void update() {
        float time = m_animClock.getElapsedTime().asSeconds();
        float scale = 1.0f + 0.1f * std::sin(time * 3.0f);
        m_shape.setScale(sf::Vector2f(scale, scale));
    }

    const sf::CircleShape& getShape() const {
        return m_shape;
    }

private:
    sf::CircleShape m_shape;
    std::mt19937 m_rng;
    sf::Clock m_animClock;
};
