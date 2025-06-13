#include <iostream>
#include <string>
#include <windows.h> // debug

#include "game.h"

void CreateConsole() // debug
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    std::cout.clear();
    std::cerr.clear();
}

struct Config
{
    int width = 0;
    int height = 0;
    int speed = 0;
};

Config parseConfigFromConsole(int argc, char* argv[])
{
    Config conf;

    for(int i = 1; i < argc; ++i)
    {
        std::string arg{argv[i]};
        if(arg.rfind("--width=", 0) == 0)
            conf.width = std::stoi(arg.substr(8));
        else if(arg.rfind("--height=", 0) == 0)
            conf.height = std::stoi(arg.substr(9));
        else if(arg.rfind("--speed=", 0) == 0)
            conf.speed = std::stoi(arg.substr(8));
    }
    
    return conf;
}

int main(int argc, char *argv[])
{
    CreateConsole(); // debug
    try
    {
        Config conf{ parseConfigFromConsole(argc, argv) };
        Game app{ conf.width, conf.height, 10, 1 };
        app.start();
    }
    catch(std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return -1;
    }
    return 0;
}