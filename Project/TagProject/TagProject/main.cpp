/// <summary>
/// simple game loop for SFML[2.5.1]
/// 
/// @author Peter Lowe
/// @date May 2019
/// </summary>

#define  _WINSOCK_DEPRECATED_NO_WARNINGS


#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h>
#include <SFML/Network.hpp>
#include <iostream>

struct Player {
    sf::Vector2f position;
    bool isIt;
};

struct GameState {
    Player players[3];
    bool gameRunning;
};


//#include "Game.h"

/// <summary>
/// main enrtry point
/// </summary>
/// <returns>success or failure</returns>
int main()
{


    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize WinSock." << std::endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(56000); // Replace with the server port
    serverAddr.sin_addr.s_addr = inet_addr("149.153.106.53"); // Replace with the server IP address


    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to the server." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    int clientIndex;
    recv(clientSocket, reinterpret_cast<char*>(&clientIndex), sizeof(int), 0);
    std::cout << "Connected as client " << clientIndex << std::endl;

    Player player;
    player.position = sf::Vector2f(0, 0); // Initial position
    player.isIt = false; // Initially, the player is not "it"

    while (true) {
        // IMPLEMENT THIS: Send the player's position to the server
        char positionBuffer[sizeof(sf::Vector2f)];
        sf::Vector2f playerPosition = player.position;
        memcpy(positionBuffer, &playerPosition, sizeof(sf::Vector2f));
        send(clientSocket, positionBuffer, sizeof(sf::Vector2f), 0);

        // IMPLEMENT THIS: Send the player's tag status to the server
        char tagBuffer[sizeof(bool)];
        bool isTagged = player.isIt;
        memcpy(tagBuffer, &isTagged, sizeof(bool));
        send(clientSocket, tagBuffer, sizeof(bool), 0);

        // IMPLEMENT THIS: Receive the updated game state from the server
        char gameStateBuffer[sizeof(GameState)];
        int bytesReceived = recv(clientSocket, gameStateBuffer, sizeof(GameState), 0);
        if (bytesReceived < 0) {
            std::cerr << "Error receiving game state from server." << std::endl;
            break; // Exit the loop in case of an error
        }

        if (bytesReceived == 0) {
            std::cout << "Server has closed the connection." << std::endl;
            break; // Exit the loop when the server closes the connection
        }

        // IMPLEMENT THIS: Deserialize the received game state and update player's information
        GameState receivedGameState;
        memcpy(&receivedGameState, gameStateBuffer, sizeof(GameState));
        player = receivedGameState.players[clientIndex]; // Update the player's position and "it" status

        // IMPLEMENT THIS: Implement game logic for tag
        // Update player's position, "it" status, and perform any other game-related operations

        // Sleep for a short time to control the update rate
        Sleep(50);
    }

    closesocket(clientSocket);
    WSACleanup();

    /*
    Game game;
    game.run();
    */

    return 1; // success
}