#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Player : public sf::Sprite
{
public:
	Player();
	~Player();

	void update(sf::Time deltaTime);
	void render(sf::RenderWindow& m_window);

	void setID(int id){ m_id = id; }
	int getID(){ return m_id; }

	void setName(std::string name) { m_name = name; }
	std::string getName(){ return m_name; }

	sf::Vector2f getPosition() { return m_circle.getPosition(); }
	float getRadius() { return m_circle.getRadius(); }

private:

	void initPlayer();
	void movement(sf::Time deltaTime);

	sf::Texture m_playerTexture;
	sf::Sprite m_playerSprite;

	sf::CircleShape m_circle;

	int m_id;
	std::string m_name;

	float playerSpeed = 200.0f;
	sf::Vector2f m_velocity;
};

