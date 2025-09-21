#include <SFML/Graphics.hpp>

#include <chrono>
#include <random>

namespace {
constexpr unsigned int CELL_SIZE = 20;
constexpr unsigned int DEFAULT_WIDTH = 800;
constexpr unsigned int DEFAULT_HEIGHT = 600;
constexpr float CELL_SIZE_F = static_cast<float>(CELL_SIZE);
}

class Food {
public:
    Food()
        : m_shape(),
          m_rng(static_cast<std::mt19937::result_type>(
              std::chrono::steady_clock::now().time_since_epoch().count())) {
        m_shape.setSize(sf::Vector2f(CELL_SIZE_F, CELL_SIZE_F));
        m_shape.setFillColor(sf::Color::Red);
        spawn();
    }

    void spawn() {
        spawn(sf::Vector2u(DEFAULT_WIDTH, DEFAULT_HEIGHT));
    }

    void spawn(const sf::Vector2u& windowSize) {
        const unsigned int cellsX = windowSize.x / CELL_SIZE;
        const unsigned int cellsY = windowSize.y / CELL_SIZE;

        std::uniform_int_distribution<unsigned int> distX(0, cellsX > 0 ? cellsX - 1 : 0);
        std::uniform_int_distribution<unsigned int> distY(0, cellsY > 0 ? cellsY - 1 : 0);

        const float x = static_cast<float>(distX(m_rng) * CELL_SIZE);
        const float y = static_cast<float>(distY(m_rng) * CELL_SIZE);

        m_shape.setPosition(sf::Vector2f(x, y));
    }

    const sf::RectangleShape& getShape() const {
        return m_shape;
    }

private:
    sf::RectangleShape m_shape;
    std::mt19937 m_rng;
};
