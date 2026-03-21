#ifndef REDIS_COMMAND_HANDLER_H
#define REDIS_COMMAND_HANDLER_H 
#include <string>


class RedisCommandHandler {
public:
    RedisCommandHandler();
    //Process the command and return the response in RESP format
    std::string processCommand(const std::string& commandLine);

};

#endif