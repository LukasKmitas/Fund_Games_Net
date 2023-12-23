#include "Enemy.h"


Enemy::Enemy(int id, sf::Vector2f position, std::string name) : m_id(id), m_name("default")
{
	initEnemy();
	isTagged = false;
}


Enemy::~Enemy()
{
}

void Enemy::draw(sf::RenderWindow& window) const
{
	window.draw(m_circle);
}

void Enemy::setTaggedStatus(bool tagged)
{
	isTagged = tagged;
}

void Enemy::initEnemy()
{
	m_circle.setRadius(20.0f);
	m_circle.setFillColor(sf::Color::Green);
	m_circle.setPosition(100.0f, 100.0f);
}

