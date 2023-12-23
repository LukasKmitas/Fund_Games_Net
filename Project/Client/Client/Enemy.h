#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Enemy
{
public:
	Enemy(int id, sf::Vector2f position, std::string name);
	~Enemy();

	void draw(sf::RenderWindow& window) const;

	void setID(int id){ m_id = id; }
	int getID(){ return m_id; }

	void setName(std::string name){ m_name = name; }
	std::string getName(){ return m_name; }

	void setPosition(sf::Vector2f position) { m_circle.setPosition(position); }
	sf::Vector2f getPosition() { return m_circle.getPosition(); }

	void setTaggedStatus(bool tagged);

	sf::CircleShape m_circle;
	bool isTagged;

	sf::Clock collisionCooldown;
	sf::Time cooldownDuration;

private:

	void initEnemy();

	int m_id;
	std::string m_name;

	sf::Vector2f m_velocity;
};

