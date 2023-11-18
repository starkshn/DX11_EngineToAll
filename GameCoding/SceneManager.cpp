#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshRenderer.h"


SceneManager::SceneManager(shared_ptr<Graphics> graphics)
	: _graphics(graphics)
{
	
} 

SceneManager::~SceneManager()
{

}

void SceneManager::Init()
{
	if (_activeScene == nullptr) return;

	_activeScene->Awake();
	_activeScene->Start();
}

void SceneManager::Update()
{
	_activeScene->Update();
	_activeScene->LateUpdate();

	// 물리 관련
	_activeScene->FixedUpdate();
}

void SceneManager::LoadScene(wstring sceneName)
{
	_activeScene = LoadTestScene();
	Init();
}

shared_ptr<Scene> SceneManager::LoadTestScene()
{
	shared_ptr<Scene> scene = make_shared<Scene>();

	// TODO

	// GO
	{
		shared_ptr<GameObject> monster = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
		{
			// 여기서 크기를 100배로 늘리거나...
			monster->GetOrAddTransform();
			monster->AddComponent(make_shared<MeshRenderer>(_graphics->GetDevice(), _graphics->GetDeviceContext()));
			scene->AddGameObject(monster);
		}
	}
	
	// Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
		{
			camera->GetOrAddTransform();
			camera->AddComponent(make_shared<Camera>());
			scene->AddGameObject(camera);
		}
	}
	
	return scene;
}
