#pragma once

#include "Errors.h"
#include <iostream>
#include <SDL\SDL.h>
#include <cstdlib>

void fatalError(std::string errString)
{
	std::cout << errString << std::endl;
	std::cout << "Enter any key to quit...";
	int temp;
	std::cin >> temp;
	SDL_Quit();
	exit(1);
}
