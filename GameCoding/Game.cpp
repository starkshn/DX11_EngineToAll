#include "pch.h"
#include "Game.h"
#include "Camera.h"

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

	// GO
	_monster = make_shared<GameObject>(_graphcis->GetDevice(), _graphcis->GetDeviceContext());
	{
		_monster->GetOrAddTransform();
		// ..
	}

	_camera = make_shared<GameObject>(_graphcis->GetDevice(), _graphcis->GetDeviceContext());
	{
		_camera->GetOrAddTransform();
		_camera->AddComponent(make_shared<Camera>());
	}
}

void Game::Update()
{
	_monster->Update();
	_camera->Update();
}

void Game::Render()
{
	_graphcis->RenderBegin();

	{
		_monster->Render(_pipeline);
	}

	_graphcis->RenderEnd();
}
