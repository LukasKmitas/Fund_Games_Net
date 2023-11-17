#ifdef _DEBUG 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-network.lib") 
#endif 

#include <WinSock2.h>
#include <SFML/Network.hpp>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

struct Player {
    sf::Vector2f position;
    bool isIt;
};

struct GameState {
    Player players[3];
    bool gameRunning;
};

int main()
{
    /*
    -----------------------------------
    //  INITIALISE WINSOCK
    -----------------------------------
    */
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != NO_ERROR) {
        std::cerr << "Initialization failed." << std::endl;
        return 1;
    }
    std::cout << "Winsock initialized." << std::endl;

    /*
    -----------------------------------
    //  CREATE LISTENING SOCKET
    -----------------------------------
    */

    SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listeningSocket == INVALID_SOCKET) {
        std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    std::cout << "Listening socket created." << std::endl;

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(56000); // Use any port that is available
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the socket
    if (bind(listeningSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(listeningSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Socket bound." << std::endl;

    // Listen on the socket
    if (listen(listeningSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(listeningSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Listening for connections." << std::endl;

    /*
    -----------------------------------
    //  ACCEPT CONNECTIONS
    -----------------------------------
    */

    struct ClientInfo {
        int index;
        SOCKET socket;
    };

    std::vector<ClientInfo> connectedClients; // Store client information
    int nextClientIndex = 0; // Initialize the next available index

    SOCKET clientSockets[3];
    for (int i = 0; i < 3; i++) {
        clientSockets[i] = accept(listeningSocket, NULL, NULL);
        if (clientSockets[i] == INVALID_SOCKET) {
            std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
            closesocket(listeningSocket);
            WSACleanup();
            return 1;
        }

        ClientInfo newClient;
        newClient.index = nextClientIndex++;
        newClient.socket = clientSockets[i];
        connectedClients.push_back(newClient);

        std::cout << "Accepted connection from client " << i << std::endl;

        // Send the assigned index to the client
        int clientIndex = newClient.index;
        send(clientSockets[i], reinterpret_cast<char*>(&clientIndex), sizeof(int), 0);
    }
    std::cout << "All clients connected!" << std::endl;

    /*
    -----------------------------------
    //  GAME LOGIC
    -----------------------------------
    */

    GameState gameState;
    gameState.gameRunning = true;
    for (int i = 0; i < 3; ++i) {
        gameState.players[i].position = sf::Vector2f(0, 0); // initial position
        gameState.players[i].isIt = false;
    }
    gameState.players[0].isIt = true;

    while (gameState.gameRunning) {
        for (int i = 0; i < 3; ++i) {
            // First receive the new position
            char positionBuffer[256];
            int bytesReceived = recv(clientSockets[i], positionBuffer, 256, 0);
            if (bytesReceived > 0) {
                sf::Vector2f newPosition;
                memcpy(&newPosition, positionBuffer, sizeof(sf::Vector2f));
                gameState.players[i].position = newPosition;
            }

            // Then check if this player has tagged someone
            char tagBuffer[256];
            bytesReceived = recv(clientSockets[i], tagBuffer, 256, 0);
            if (bytesReceived > 0) {
                bool tagged;
                memcpy(&tagged, tagBuffer, sizeof(bool));
                if (tagged) {
                    gameState.players[i].isIt = true;
                }
                else {
                    gameState.players[i].isIt = false;
                }
            }
        }

        // Process game logic here

        // Send updated game state to all players
        for (int i = 0; i < 3; ++i) {
            char gameStateBuffer[1024];
            memcpy(gameStateBuffer, &gameState, sizeof(GameState));
            send(clientSockets[i], gameStateBuffer, sizeof(GameState), 0);
        }
    }

    /*
    -----------------------------------
    //  CLOSE SOCKETS
    -----------------------------------
    */

    for (int i = 0; i < 3; ++i) {
        closesocket(clientSockets[i]);
    }
    std::cout << "Client sockets closed." << std::endl;

    closesocket(listeningSocket);
    WSACleanup();
    std::cout << "Listening socket closed." << std::endl;

    return 1;
}
