#pragma once

#include "GameObject.h"

class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hWnd);
	void Update();
	void Render();

private:
	HWND					_hWnd;

private:
	shared_ptr<Graphics>	_graphcis;
	shared_ptr<Pipeline>	_pipeline;
	
private:
	// Temp
	shared_ptr<GameObject>	_monster;
	shared_ptr<GameObject>	_camera;
};
