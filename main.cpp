#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include <iostream>
#include <fstream>
#include "MapHelper.h"
#include "game.h"
#include <chrono>
#include "SDL2/SDL.h"

// mac run 9
// linux run 6
// windows ss


int main(int argc, char* argv[])
{
    // chrono code and savind output was provided by chatgpt 
    game myGame = game();
    auto start = std::chrono::high_resolution_clock::now();
    myGame.RunGame();

    // End timing
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;

    std::ofstream log("timing_log.txt");
    if (log.is_open()) {
        log << "Game ran for: " << duration.count() << " seconds\n";
        log.close();
    }
    return 0;
}

