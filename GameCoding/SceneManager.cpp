#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "ResourceManager.h"
#include "Game.h"
#include "Mesh.h"
#include "Animator.h"
#include "CameraMove.h"

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


	// Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
		{
			camera->GetOrAddTransform();
			camera->AddComponent(make_shared<Camera>());

			// Test
			camera->AddComponent(make_shared<CameraMove>());
		}

		scene->AddGameObject(camera);
	}

	// GO
	{
		shared_ptr<GameObject> monster = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
		{
			// 여기서 크기를 100배로 늘리거나...
			monster->GetOrAddTransform();

			auto meshRenderer = make_shared<MeshRenderer>(_graphics->GetDevice(), _graphics->GetDeviceContext());
			monster->AddComponent(meshRenderer);

			auto material = RESOURCES->Get<Material>(L"DefaultMaterial");
			meshRenderer->SetMaterial(material);

			auto mesh = RESOURCES->Get<Mesh>(L"Rectangle");
			meshRenderer->SetMesh(mesh);
		}

		{
			auto animator = make_shared<Animator>();
			monster->AddComponent(animator);
			auto anim = RESOURCES->Get<Animation>(L"SnakeAnim");
			animator->SetAnimation(anim);
		}

		scene->AddGameObject(monster);
	}

	{
		shared_ptr<GameObject> monster2 = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
		monster2->GetOrAddTransform()->SetPosition(Vec3(-1.f, 1.f, 0.f));
		{
			// 여기서 크기를 100배로 늘리거나...
			monster2->GetOrAddTransform();

			auto meshRenderer = make_shared<MeshRenderer>(_graphics->GetDevice(), _graphics->GetDeviceContext());
			monster2->AddComponent(meshRenderer);

			auto material = RESOURCES->Get<Material>(L"DefaultMaterial");
			meshRenderer->SetMaterial(material);

			auto mesh = RESOURCES->Get<Mesh>(L"Rectangle");
			meshRenderer->SetMesh(mesh);
		}

		{
			auto animator = make_shared<Animator>();
			monster2->AddComponent(animator);
			auto anim = RESOURCES->Get<Animation>(L"SnakeAnim");
			animator->SetAnimation(anim);
		}

		scene->AddGameObject(monster2);
	}
	
	return scene;
}
