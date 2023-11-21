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
	_pipeline		= make_shared<Pipeline>(_graphcis->GetDeviceContext());

	_sceneManager	= make_shared<SceneManager>(_graphcis);
	_sceneManager->Init();

	_timeManager	= make_shared<TimeManager>();
	_timeManager->Init();

	_inputManager	= make_shared<InputManager>();
	_inputManager->Init(_hWnd);

	_resourceManager = make_shared<ResourceManager>(_graphcis->GetDevice());
	_resourceManager->Init();

	SCENE->LoadScene(L"Test");
}

void Game::Update()
{
	_graphcis->RenderBegin();

	TIME->Update();
	INPUT->Update();
	SCENE->Update();
	
	_graphcis->RenderEnd();
}

void Game::Render()
{
	

	{
		
	}

}
