#pragma once
#include <iostream>
#include <SFML/Network.hpp>
#include <memory>
#include <vector>
#include <string>
#include "Player.h"

class Server
{
public:
	Server();

	void run();

private:

	void sendPacket(sf::Packet& packet, unsigned int skip = -1);
	void tagPlayer(int playerID, bool isTagged);
	bool m_isRunning;

	sf::IpAddress m_ipAdress;
	unsigned short m_port;

	sf::TcpListener m_listener;
	sf::SocketSelector m_selector;

	int m_maxPlayerNumber;
	int m_playerNumber;
	int m_currentID;

	std::vector<Player> m_playerList;
	char m_tmp[1400];

	sf::Clock m_clock;

};
