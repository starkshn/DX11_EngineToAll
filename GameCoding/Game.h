#pragma once

#include "GameObject.h"

class SceneManager;
class TimeManager;
class InputManager;
class ResourceManager;

class Pipeline;

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
	shared_ptr<SceneManager>	GetSceneManager() { return _sceneManager; }
	shared_ptr<TimeManager>		GetTimeManager() { return _timeManager; }
	shared_ptr<InputManager>	GetInputManager() { return _inputManager; }
	shared_ptr<ResourceManager>	GetResourceManager() { return _resourceManager; }

	shared_ptr<Pipeline>		GetPipeline() { return _pipeline; }

private:
	HWND					_hWnd;

private:
	shared_ptr<Graphics>	_graphcis;
	shared_ptr<Pipeline>	_pipeline;

private:
	shared_ptr<SceneManager>		_sceneManager;
	shared_ptr<TimeManager>			_timeManager;
	shared_ptr<InputManager>		_inputManager;
	shared_ptr<ResourceManager>		_resourceManager;

};

extern unique_ptr<Game> GGame;