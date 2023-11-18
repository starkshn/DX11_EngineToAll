#include "pch.h"
#include "Game.h"
#include "Camera.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Scene.h"
#include "SceneManager.h"

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

	SCENE->LoadScene(L"Test");
}

void Game::Update()
{
	_graphcis->RenderBegin();

	SCENE->Update();

	_graphcis->RenderEnd();
}

void Game::Render()
{
	

	{
		
	}

}
