#pragma once
#include "Application.h"

class RenderColoredScreen : public Application
{
protected:
	void OnStart() override;
	void OnRender() override;
};