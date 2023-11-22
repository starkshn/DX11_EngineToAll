#include "pch.h"
#include "Game.h"
#include "Camera.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Scene.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "RenderManager.h"

unique_ptr<Game> GGame = make_unique<Game>();

Game::Game()
{
	 
}

Game::~Game()
{

}

void Game::Init(HWND hWnd)
{
	_hWnd			= hWnd;

	_graphcis		= make_shared<Graphics>(hWnd);

	_sceneManager	= make_shared<SceneManager>(_graphcis);
	_sceneManager->Init();

	_timeManager	= make_shared<TimeManager>();
	_timeManager->Init();

	_inputManager	= make_shared<InputManager>();
	_inputManager->Init(_hWnd);

	_resourceManager = make_shared<ResourceManager>(_graphcis->GetDevice());
	_resourceManager->Init();
	
	_renderManager = make_shared<RenderManager>(_graphcis->GetDevice(), _graphcis->GetDeviceContext());
	_renderManager->Init();

	SCENE->LoadScene(L"Test");
}

void Game::Update()
{
	TIME->Update();
	INPUT->Update();
	SCENE->Update();
}

void Game::Render()
{
	RENDER->Update(_graphcis);
}
