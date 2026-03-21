#include "../include/RedisServer.h"
#include "../include/RedisCommandHandler.h"

#include<iostream>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<thread>
#include<vector>
#include<chrono>
#include <cstring>


static RedisServer* globalServer = nullptr;

RedisServer::RedisServer(int port) :port(port), running(true),server_socket(-1)
{
    globalServer = this;
}

void RedisServer::shutdown()
{
    running = false;
    if (server_socket != -1) {
        close(server_socket);
    }


    std::cout<<"Server shutdown on port: "<<port<<std::endl;
}

void RedisServer::run()
{
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket <0 ) {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }

   int opt = 1;

   setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
        return;
    }

    if (listen(server_socket, 10) < 0) {
        std::cerr << "Failed to listen on socket" << std::endl;
        return;
    }

    std::cout << "RedisServer is Listening on port: " << port << std::endl;

    std::vector<std::thread> threads;
    RedisCommandHandler commandHandler;


    while (running) {
        int client_socket = accept(server_socket, nullptr, nullptr);
        
        if (client_socket < 0) {
            if (running) {
                std::cerr << "Failed to accept connection" << std::endl;
            }
            continue;
        }

        // Handle client connection in a separate thread or process
        // For simplicity, we just close the client socket here
        threads.emplace_back([client_socket, &commandHandler]() {
            char buffer[1024];

            while (true) {
                memset(buffer, 0, sizeof(buffer));

                int bytesRead = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

                if (bytesRead <= 0) {
                    break; // Connection closed or error
                }

                std::string request(buffer, bytesRead);
                std::string response = commandHandler.processCommand(request);
                
                send(client_socket, response.c_str(), response.size(), 0);
            }

            close(client_socket);
            
        });
    }


    for(auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}