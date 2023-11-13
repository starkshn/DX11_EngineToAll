#include "pch.h"
#include "GameObject.h"
#include "MonoBehavior.h"
#include "Transform.h"
#include "Camera.h"
#include "MeshRenderer.h"


GameObject::GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	:
	_device(device)
{
	
}

GameObject::~GameObject()
{
}

void GameObject::Awake()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Awake();
	}

	for (shared_ptr<MonoBehavior>& script : _scripts)
	{
		script->Awake();
	}
}

void GameObject::Start()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Start();
	}

	for (shared_ptr<MonoBehavior>& script : _scripts)
	{
		script->Start();
	}
}

void GameObject::Update()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Update();
	}

	for (shared_ptr<MonoBehavior>& script : _scripts)
	{
		script->Update();
	}

	if (GetCamera())
		return;
	
}

void GameObject::LateUpdate()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->LateUpdate();
	}

	for (shared_ptr<MonoBehavior>& script : _scripts)
	{
		script->LateUpdate();
	}
}

void GameObject::FixedUpdate()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->FixedUpdate();
	}

	for (shared_ptr<MonoBehavior>& script : _scripts)
	{
		script->FixedUpdate();
	}
}

shared_ptr<Component> GameObject::GetFixedComponent(ComponentType type)
{
	uint8 index = static_cast<uint8>(type);
	assert(index < FIXED_COMPONENT_COUNT);
	return _components[index];
}

shared_ptr<Transform> GameObject::GetTransform()
{
	shared_ptr<Component> comp = GetFixedComponent(ComponentType::Transform);
	return static_pointer_cast<Transform>(comp);
}

shared_ptr<Transform> GameObject::GetOrAddTransform()
{
	if (GetTransform() == nullptr)
	{
		shared_ptr<Transform> transform = make_shared<Transform>();
		AddComponent(transform);
	}

	return GetTransform();
}

shared_ptr<Camera> GameObject::GetCamera()
{
	shared_ptr<Component> comp = GetFixedComponent(ComponentType::Camera);
	return static_pointer_cast<Camera>(comp);
}

shared_ptr<Camera> GameObject::GetOrAddCamera()
{
	if (GetCamera() == nullptr)
	{
		shared_ptr<Camera> camera = make_shared<Camera>();
		AddComponent(camera);
	}

	return GetCamera();
}

shared_ptr<MeshRenderer> GameObject::GetMeshRenderer()
{
	shared_ptr<Component> comp = GetFixedComponent(ComponentType::MeshRenderer);
	return static_pointer_cast<MeshRenderer>(comp);
}

void GameObject::AddComponent(shared_ptr<Component> component)
{
	// this의 스마트 포인터 버젼
	component->SetGameObject(shared_from_this());

	uint8 idx = static_cast<uint8>(component->GetType());
	if (idx < FIXED_COMPONENT_COUNT)
	{
		_components[idx] = component;
	}
	else
	{
		// static_pointer_cast해도 상관없음(타입이 확실하니까)
		_scripts.push_back(dynamic_pointer_cast<MonoBehavior>(component));
	}	
}
