#include "RenderColoredScreen.h"
#include <random>
#include <chrono>
#include <iostream>
#include <thread> 

void RenderColoredScreen::OnStart()
{
}

int GetRandom()
{
	return rand();
}

void RenderColoredScreen::OnRender()
{
	int isEven = GetRandom() % 2;
	if (isEven)
		glClearColor(1, 0, 0, 1.0f);
	else
		glClearColor(0, 1, 0, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);

	std::this_thread::sleep_for(std::chrono::seconds(1));
}
