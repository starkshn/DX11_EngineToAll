#pragma once

#include "GameObject.h"

class SceneManager;
class TimeManager;
class InputManager;
class ResourceManager;
class RenderManager;

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
	shared_ptr<RenderManager>	GetRenderManager() { return _renderManager; }

private:
	HWND					_hWnd;

private:
	shared_ptr<Graphics>	_graphcis;

private:
	shared_ptr<SceneManager>		_sceneManager;
	shared_ptr<TimeManager>			_timeManager;
	shared_ptr<InputManager>		_inputManager;
	shared_ptr<ResourceManager>		_resourceManager;
	shared_ptr<RenderManager>		_renderManager;

};

extern unique_ptr<Game> GGame;