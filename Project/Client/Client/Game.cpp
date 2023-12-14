#include "Game.h"

Game::Game() :
	m_window{ sf::VideoMode{800U,600U,32U}, "Tag Network Game"},
	m_exitGame{false}
{
	port = 45000;
	ipAdress = "localhost";
	network = std::make_unique<Network>(ipAdress, port);
	
	initPlayerName();

	socket.setBlocking(false);
}

Game::~Game()
{
	network->disconnect(&m_player);
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps);
	while (m_window.isOpen())
	{
		processEvents();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents();
			update(timePerFrame);
		}
		render();
	}
}


void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if (sf::Event::Closed == newEvent.type)
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type)
		{
			processKeys(newEvent);
		}
	}
}

void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
}


void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	sf::Vector2f newPosition = m_player.getPosition();
	if (newPosition != lastSentPosition)
	{
		lastSentPosition = newPosition;
		network->sendPosition(&m_player);
	}

	network->receive(enemies, &m_player);

	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->setPosition(enemies[i]->getPosition());
	}

	if (m_window.hasFocus()) // To be able to move in that 1 window
	{
		m_player.update(t_deltaTime);
	}
}


void Game::render()
{
	m_window.clear(sf::Color::White);

	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->draw(m_window);
	}
	m_player.render(m_window);

	m_window.display();
}

void Game::initPlayerName()
{
	std::cout << "Please enter your name" << std::endl;
	std::cin >> m_name;
	m_player.setName(m_name);
}
