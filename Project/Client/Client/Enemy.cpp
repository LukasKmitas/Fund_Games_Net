#include "Enemy.h"


Enemy::Enemy(int id, sf::Vector2f position, std::string name) : m_id(id), m_name("default")
{
	/*if (!pTexture.loadFromFile("ASSETS/IMAGES/player.png"))
		std::cout << "Error loading player.png" << std::endl;
	this->setTexture(pTexture);
	this->setPosition(position);
	this->setName(name);*/

	initEnemy();
}


Enemy::~Enemy()
{
}

void Enemy::draw(sf::RenderWindow& window) const
{
	window.draw(m_circle);
}

void Enemy::initEnemy()
{
	m_circle.setRadius(20.0f);
	m_circle.setFillColor(sf::Color::Green);
	m_circle.setPosition(100.0f, 100.0f);
}

