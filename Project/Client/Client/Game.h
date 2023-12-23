#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "Network.h"
#include "Player.h"
#include "Enemy.h"

class Game
{
public:
	Game();
	~Game();

	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();

	void initPlayerName();

	void checkTags();
	bool checkCollision(Player& player, Enemy& enemy);

	unsigned short port;
	sf::IpAddress ipAdress;
	sf::TcpSocket socket;

	sf::Clock m_clock; //To make sure the time aligns between clients
	sf::Time m_positionTimer;

	Player m_player;
	std::vector<std::unique_ptr<Enemy>> enemies;
	std::unique_ptr<Network> network;

	std::string m_name;
	sf::Vector2f lastSentPosition;

	sf::RenderWindow m_window;

	bool m_exitGame;
};

#endif