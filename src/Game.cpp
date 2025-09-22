#include <SFML/Graphics.hpp>

#include <array>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

#include "Snake.cpp"
#include "Food.cpp"

namespace {
constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int WINDOW_HEIGHT = 600;
constexpr float GRID_CELL_SIZE = 20.0f;
constexpr float MOVE_INTERVAL_SECONDS = 0.12f;
const sf::Color BACKGROUND_COLOR(30, 30, 30);

bool isOpposite(const sf::Vector2f& lhs, const sf::Vector2f& rhs) {
    return lhs.x == -rhs.x && lhs.y == -rhs.y;
}

void centerText(sf::Text& text, float centerY) {
    const sf::FloatRect bounds = text.getLocalBounds();
    const sf::Vector2f origin(bounds.position.x + bounds.size.x / 2.0f,
                               bounds.position.y + bounds.size.y / 2.0f);
    text.setOrigin(origin);
    text.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, centerY));
}
}

class Game {
public:
    Game();
    void run();

private:
    enum class State {
        Start,
        Playing,
        GameOver
    };

    void processEvents();
    void handleKeyPress(sf::Keyboard::Key key);
    void update();
    void handleCollisions();
    void render();

    bool isOutOfBounds(const sf::Vector2f& position) const;
    void reset();
    void startGame();
    void updateScoreDisplay();
    void centerWithShadow(sf::Text& text, sf::Text& shadow, float centerY);
    void drawShadowedText(const std::unique_ptr<sf::Text>& shadow,
                          const std::unique_ptr<sf::Text>& text);

    sf::RenderWindow m_window;
    Snake m_snake;
    Food m_food;
    sf::Clock m_moveClock;
    float m_moveInterval;
    sf::Vector2f m_direction;
    unsigned int m_score;
    State m_state;

    sf::Font m_font;
    bool m_fontLoaded;
    std::unique_ptr<sf::Text> m_scoreText;
    std::unique_ptr<sf::Text> m_scoreShadow;
    std::unique_ptr<sf::Text> m_gameOverText;
    std::unique_ptr<sf::Text> m_gameOverShadow;
    std::unique_ptr<sf::Text> m_titleText;
    std::unique_ptr<sf::Text> m_titleShadow;
};

Game::Game()
    : m_window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Snake Game"),
      m_snake(),
      m_food(),
      m_moveInterval(MOVE_INTERVAL_SECONDS),
      m_direction(GRID_CELL_SIZE, 0.0f),
      m_score(0),
      m_state(State::Start),
      m_fontLoaded(false) {
    m_window.setFramerateLimit(60);

    const std::array<std::filesystem::path, 3> fontCandidates = {
        std::filesystem::path("assets/fonts/PressStart2P.ttf"),
        std::filesystem::path("../assets/fonts/PressStart2P.ttf"),
        std::filesystem::path("../../assets/fonts/PressStart2P.ttf")};

    for (const auto& candidate : fontCandidates) {
        if (std::filesystem::exists(candidate) && m_font.openFromFile(candidate)) {
            m_fontLoaded = true;
            break;
        }
    }

    if (!m_fontLoaded) {
        const std::array<std::filesystem::path, 4> fallbackFonts = {
            std::filesystem::path("/System/Library/Fonts/Supplemental/Arial.ttf"),
            std::filesystem::path("/System/Library/Fonts/Supplemental/Courier New.ttf"),
            std::filesystem::path("/Library/Fonts/Arial.ttf"),
            std::filesystem::path("/Library/Fonts/Verdana.ttf")};

        for (const auto& fallback : fallbackFonts) {
            if (std::filesystem::exists(fallback) && m_font.openFromFile(fallback)) {
                m_fontLoaded = true;
                std::cerr << "Warning: PressStart2P.ttf not found. Using fallback font: "
                          << fallback << std::endl;
                break;
            }
        }
    }

    if (!m_fontLoaded) {
        std::cerr << "Warning: PressStart2P.ttf not found and no fallback font available. UI text will not be displayed."
                  << std::endl;
    } else {
        m_scoreText = std::make_unique<sf::Text>(m_font, "Score: 0", 24);
        m_scoreText->setFillColor(sf::Color::Yellow);
        m_scoreText->setPosition(sf::Vector2f(10.0f, 10.0f));

        m_scoreShadow = std::make_unique<sf::Text>(*m_scoreText);
        m_scoreShadow->setFillColor(sf::Color::Black);
        m_scoreShadow->setPosition(m_scoreText->getPosition() + sf::Vector2f(2.0f, 2.0f));

        m_gameOverText = std::make_unique<sf::Text>(m_font, "Game Over\nPress SPACE to Restart", 32);
        m_gameOverText->setFillColor(sf::Color::White);
        m_gameOverText->setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f - 200.0f,
                                                 WINDOW_HEIGHT / 2.0f - 50.0f));

        m_gameOverShadow = std::make_unique<sf::Text>(*m_gameOverText);
        m_gameOverShadow->setFillColor(sf::Color::Black);
        m_gameOverShadow->setPosition(m_gameOverText->getPosition() + sf::Vector2f(2.0f, 2.0f));

        m_titleText = std::make_unique<sf::Text>(m_font, "Snake C++ Edition\nPress SPACE to Start", 28);
        m_titleText->setFillColor(sf::Color::White);
        m_titleText->setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f - 250.0f,
                                              WINDOW_HEIGHT / 2.0f - 50.0f));

        m_titleShadow = std::make_unique<sf::Text>(*m_titleText);
        m_titleShadow->setFillColor(sf::Color::Black);
        m_titleShadow->setPosition(m_titleText->getPosition() + sf::Vector2f(2.0f, 2.0f));

        centerWithShadow(*m_gameOverText, *m_gameOverShadow, WINDOW_HEIGHT / 2.0f);
        centerWithShadow(*m_titleText, *m_titleShadow, WINDOW_HEIGHT / 2.0f);

        updateScoreDisplay();
    }

    m_food.spawn(m_window.getSize());
    m_snake.setDirection(m_direction);
}

void Game::run() {
    while (m_window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    while (const std::optional<sf::Event> event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            handleKeyPress(keyPressed->code);
        }
    }
}

void Game::handleKeyPress(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Key::Escape) {
        m_window.close();
        return;
    }

    if (key == sf::Keyboard::Key::Space) {
        if (m_state == State::Start || m_state == State::GameOver) {
            startGame();
        }
        return;
    }

    if (m_state != State::Playing) {
        return;
    }

    sf::Vector2f requested = m_direction;
    switch (key) {
        case sf::Keyboard::Key::Up:
            requested = sf::Vector2f(0.0f, -GRID_CELL_SIZE);
            break;
        case sf::Keyboard::Key::Down:
            requested = sf::Vector2f(0.0f, GRID_CELL_SIZE);
            break;
        case sf::Keyboard::Key::Left:
            requested = sf::Vector2f(-GRID_CELL_SIZE, 0.0f);
            break;
        case sf::Keyboard::Key::Right:
            requested = sf::Vector2f(GRID_CELL_SIZE, 0.0f);
            break;
        default:
            return;
    }

    if (!isOpposite(requested, m_direction)) {
        m_direction = requested;
        m_snake.setDirection(m_direction);
    }
}

void Game::update() {
    m_food.update();

    if (m_state != State::Playing) {
        return;
    }

    if (m_moveClock.getElapsedTime().asSeconds() >= m_moveInterval) {
        m_snake.move();
        m_moveClock.restart();
        handleCollisions();
    }
}

void Game::handleCollisions() {
    const sf::Vector2f headPos = m_snake.getHead().getPosition();

    if (isOutOfBounds(headPos)) {
        m_state = State::GameOver;
        return;
    }

    const auto& segments = m_snake.getBody();
    for (std::size_t i = 1; i < segments.size(); ++i) {
        if (segments[i].getPosition() == headPos) {
            m_state = State::GameOver;
            return;
        }
    }

    const sf::Vector2f headCenter = headPos + sf::Vector2f(GRID_CELL_SIZE * 0.5f, GRID_CELL_SIZE * 0.5f);
    const sf::Vector2f foodPosition = m_food.getShape().getPosition();

    if (std::abs(headCenter.x - foodPosition.x) < 0.1f &&
        std::abs(headCenter.y - foodPosition.y) < 0.1f) {
        m_snake.grow();
        m_food.spawn(m_window.getSize());
        m_score += 10;
        updateScoreDisplay();
    }
}

void Game::render() {
    m_window.clear(BACKGROUND_COLOR);

    if (m_state != State::Start) {
        for (const auto& segment : m_snake.getBody()) {
            m_window.draw(segment);
        }
        m_window.draw(m_food.getShape());
    }

    drawShadowedText(m_scoreShadow, m_scoreText);

    if (m_state == State::Start) {
        drawShadowedText(m_titleShadow, m_titleText);
    } else if (m_state == State::GameOver) {
        drawShadowedText(m_gameOverShadow, m_gameOverText);
    }

    m_window.display();
}

bool Game::isOutOfBounds(const sf::Vector2f& position) const {
    return position.x < 0.0f || position.y < 0.0f ||
           position.x >= static_cast<float>(WINDOW_WIDTH) ||
           position.y >= static_cast<float>(WINDOW_HEIGHT);
}

void Game::reset() {
    m_snake = Snake();
    m_direction = sf::Vector2f(GRID_CELL_SIZE, 0.0f);
    m_snake.setDirection(m_direction);
    m_food.spawn(m_window.getSize());
    m_moveClock.restart();
    m_score = 0;
    updateScoreDisplay();
}

void Game::startGame() {
    reset();
    m_state = State::Playing;
}

void Game::updateScoreDisplay() {
    if (!m_scoreText || !m_scoreShadow) {
        return;
    }

    const std::string scoreString = "Score: " + std::to_string(m_score);
    m_scoreText->setString(scoreString);
    m_scoreShadow->setString(scoreString);
    m_scoreShadow->setPosition(m_scoreText->getPosition() + sf::Vector2f(2.0f, 2.0f));
}

void Game::centerWithShadow(sf::Text& text, sf::Text& shadow, float centerY) {
    centerText(text, centerY);
    centerText(shadow, centerY);
    shadow.move(sf::Vector2f(2.0f, 2.0f));
}

void Game::drawShadowedText(const std::unique_ptr<sf::Text>& shadow,
                            const std::unique_ptr<sf::Text>& text) {
    if (!shadow || !text) {
        return;
    }

    m_window.draw(*shadow);
    m_window.draw(*text);
}
