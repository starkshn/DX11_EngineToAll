#pragma once

#include "GameObject.h"

class SceneManager;

class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hWnd);
	void Update();
	void Render();

public:
	shared_ptr<SceneManager> GetSceneManager() { return _sceneManager; }

private:
	HWND					_hWnd;

private:
	shared_ptr<Graphics>	_graphcis;
	shared_ptr<Pipeline>	_pipeline;

private:
	shared_ptr<SceneManager> _sceneManager;

};

extern unique_ptr<Game> GGame;