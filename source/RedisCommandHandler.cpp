#include "../include/RedisCommandHandler.h"
#include <vector>
#include <sstream>
#include <algorithm>    
#include <iostream>



// RESP Parser

std::vector<std::string> parseRESP(const std::string& input) {
    std::vector<std::string> tokens;

    if(input.empty()) return tokens; // Not a valid RESP array
        
    
    if(input[0] != '*') 
    {
        std::istringstream ss(input);
        std::string token;

        while(ss >> token) {
            tokens.push_back(token);
        }

        return tokens;
    }

    size_t pos = 1; // Skip the initial '*'
    
    if(input[pos] != '*') return tokens ;
    pos++; 

    size_t crlf = input.find("\r\n",pos); 

    if(crlf == std::string::npos) return tokens ;

    int numElements = std::stoi(input.substr(pos, crlf - pos));
    pos = crlf + 2; // Move past the CRLF

    for(int i = 0  ; i < numElements ; i++)
    {
        if(pos > input.size() || input[pos] != '$') return tokens ;
        pos++;

        crlf = input.find("\r\n", pos);
        if(crlf == std::string::npos) return tokens ;

        int len = std::stoi(input.substr(pos, crlf - pos));
       
        pos = crlf+2 ;
        std::string token = input.substr(pos, len);
        tokens.push_back(token);
        pos+=len+2; 
    }

    return tokens;
} 


RedisCommandHandler::RedisCommandHandler(){}

std::string RedisCommandHandler::processCommand(const std::string& commandLine) {
  std::vector<std::string> tokens = parseRESP(commandLine);

  if(tokens.empty()) return "-Error Empty command\r\n";

  for(auto& token : tokens) {
    std::cout<<"Token: " << token << std::endl;
  }

  std::string cmd = tokens[0];
  std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
  std::ostringstream response;

  //todo : connect to database , check commands 

  return response.str();
}