#include "Game.h"

Game::Game() :
	m_window{ sf::VideoMode{800U,600U,32U}, "Tag Network Game"},
	m_exitGame{false}
{
	port = 45000;
	ipAdress = "localhost";
	network = std::make_unique<Network>(ipAdress, port);
	
	//initPlayerName();

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
		enemies[i]->setPosition(enemies[i]->m_circle.getPosition());
		if (&m_player.isTagged && checkCollision(m_player, *enemies[i]))
		{
			if (!enemies[i]->isTagged && enemies[i]->collisionCooldown.getElapsedTime() > enemies[i]->cooldownDuration)
			{
				enemies[i]->isTagged = true;
				m_player.isTagged = false;

				enemies[i]->cooldownDuration = sf::seconds(3.0f);
				enemies[i]->collisionCooldown.restart();
			}
		}
		if (enemies[i]->isTagged && enemies[i]->collisionCooldown.getElapsedTime() < enemies[i]->cooldownDuration)
		{
			enemies[i]->m_circle.setFillColor(sf::Color::Yellow);
			std::cout << "Player is on cooldown. Remaining time: " << enemies[i]->cooldownDuration.asSeconds() - enemies[i]->collisionCooldown.getElapsedTime().asSeconds() << " seconds." << std::endl;
		}
	}

	if (m_window.hasFocus()) // To be able to move in that 1 window
	{
		m_player.update(t_deltaTime);
	}
	checkTags();
}

void Game::render()
{
	m_window.clear(sf::Color::White);

	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->draw(m_window);

		if (enemies[i]->isTagged)
		{
			enemies[i]->m_circle.setFillColor(sf::Color::Red);
		}
		else
		{
			enemies[i]->m_circle.setFillColor(sf::Color::Magenta);
		}
	}

	if (m_player.isTagged)
	{
		m_player.m_circle.setFillColor(sf::Color::Red);
	}
	else
	{
		m_player.m_circle.setFillColor(sf::Color::Green);
	}
	m_player.render(m_window);
	m_window.display();
}

/// <summary>
/// To create a new for new players
/// </summary>
void Game::initPlayerName()
{
	std::cout << "Please enter your name" << std::endl;
	std::cin >> m_name;
	m_player.setName(m_name);
}

/// <summary>
/// Checks who is tagged and who isnt
/// </summary>
void Game::checkTags()
{
	m_player.isTagged = true;

	for (const auto& enemy : enemies)
	{
		if (enemy->isTagged)
		{
			m_player.isTagged = false;
			return;
		}
	}
}

/// <summary>
/// Check the collisions between players
/// </summary>
/// <param name="player"> PLayer </param>
/// <param name="enemy"> Enemies </param>
/// <returns></returns>
bool Game::checkCollision(Player& player, Enemy& enemy)
{
	sf::FloatRect playerBounds = player.m_circle.getGlobalBounds();
	sf::FloatRect enemyBounds = enemy.m_circle.getGlobalBounds();

	if (playerBounds.intersects(enemyBounds))
	{
		std::cout << "Enemy Tagged" << std::endl;
		return true;
	}
	return false;
}
