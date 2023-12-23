#include "Player.h"


Player::Player() : m_id(-1), isTagged(false)
{
	initPlayer();
	m_velocity = sf::Vector2f(0.0f, 0.0f);
}

Player::~Player()
{
}

void Player::update(sf::Time deltaTime)
{
	movement(deltaTime);
}

void Player::render(sf::RenderWindow& m_window)
{
	m_window.draw(m_circle);
}

void Player::initPlayer()
{
	m_circle.setRadius(20.0f);
	m_circle.setFillColor(sf::Color::Green);
	m_circle.setPosition(100.0f, 100.0f);
}

void Player::movement(sf::Time deltaTime)
{
    m_velocity = sf::Vector2f(0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        m_velocity.y = -playerSpeed;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        m_velocity.x = -playerSpeed;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        m_velocity.y = playerSpeed;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        m_velocity.x = playerSpeed;
    }

    sf::Vector2f displacement = m_velocity * deltaTime.asSeconds();
    m_circle.move(displacement);
}
