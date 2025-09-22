#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdint>
#include <vector>

namespace {
constexpr float SNAKE_CELL_SIZE = 20.0f;
constexpr float START_X = 400.0f;
constexpr float START_Y = 300.0f;
const sf::Color HEAD_COLOR(180, 255, 140);

bool isAxisAlignedStep(const sf::Vector2f& value) {
    const bool horizontal = std::fabs(value.x) == SNAKE_CELL_SIZE && value.y == 0.0f;
    const bool vertical = std::fabs(value.y) == SNAKE_CELL_SIZE && value.x == 0.0f;
    return horizontal || vertical;
}

sf::RectangleShape makeSegment(const sf::Vector2f& position) {
    sf::RectangleShape segment;
    segment.setSize(sf::Vector2f(SNAKE_CELL_SIZE, SNAKE_CELL_SIZE));
    segment.setPosition(position);
    return segment;
}
}

class Snake {
public:
    Snake()
        : m_segments(),
          m_direction(SNAKE_CELL_SIZE, 0.0f),
          m_colorOffset(0) {
        auto head = makeSegment(sf::Vector2f(START_X, START_Y));
        head.setFillColor(HEAD_COLOR);
        m_segments.push_back(head);
    }

    void setDirection(const sf::Vector2f& dir) {
        if (isAxisAlignedStep(dir)) {
            m_direction = dir;
        }
    }

    void move() {
        if (m_segments.empty()) {
            return;
        }

        std::vector<sf::Vector2f> previousPositions;
        previousPositions.reserve(m_segments.size());
        for (const auto& segment : m_segments) {
            previousPositions.push_back(segment.getPosition());
        }

        m_segments.front().move(m_direction);

        for (std::size_t i = 1; i < m_segments.size(); ++i) {
            m_segments[i].setPosition(previousPositions[i - 1]);
        }

        ++m_colorOffset;
        applyDynamicColors();
    }

    void grow() {
        sf::Vector2f tailPosition = m_segments.empty()
                                       ? sf::Vector2f(START_X, START_Y)
                                       : m_segments.back().getPosition();
        auto segment = makeSegment(tailPosition);
        m_segments.push_back(segment);
        applyDynamicColors();
    }

    const sf::RectangleShape& getHead() const {
        return m_segments.front();
    }

    const std::vector<sf::RectangleShape>& getBody() const {
        return m_segments;
    }

private:
    void applyDynamicColors() {
        if (m_segments.empty()) {
            return;
        }

        m_segments.front().setFillColor(HEAD_COLOR);

        for (std::size_t i = 1; i < m_segments.size(); ++i) {
            const std::size_t len = i + m_colorOffset;
            const sf::Color dynamicColor(
                static_cast<std::uint8_t>(100 + (len * 5) % 155),
                static_cast<std::uint8_t>(255 - (len * 3) % 200),
                static_cast<std::uint8_t>(100 + (len * 7) % 155));
            m_segments[i].setFillColor(dynamicColor);
        }
    }

    std::vector<sf::RectangleShape> m_segments;
    sf::Vector2f m_direction;
    std::size_t m_colorOffset;
};
