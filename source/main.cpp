#include "../include/RedisServer.h"
#include "../include/RedisCommandHandler.h"


#include <iostream>
#include <thread>
#include <chrono>


int main(int argc, char* argv[]) {
   int port = 6379; // Default port number

   std::cout<<argv[0]<<std::endl;

    if (argc > 1) {
        port = std::stoi(argv[1]); 
    }

    RedisServer server(port);
    
    std::thread persistanceThread([]() {
        while (true) {
            std::cout<<"Persisting data to disk..."<<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
            // Here you would add code to save the server state to disk
            //dump the data to disk every 5 minutes
        }
    });

    persistanceThread.detach();
    
    server.run();
    
    return 0;
}