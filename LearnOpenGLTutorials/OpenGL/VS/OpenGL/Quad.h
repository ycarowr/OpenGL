#pragma once
#include "Application.h"
#include <vector>


class Quad : public Application
{
	public:
		Quad(std::vector<float>* vertices, std::vector<unsigned int>* indices);
};

