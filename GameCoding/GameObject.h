#pragma once

class MonoBehavior;
class Trnasform;
class Camera;
class MeshRenderer;

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	~GameObject();

public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FixedUpdate();

public:
	void AddComponent(shared_ptr<Component> component);

public:
	shared_ptr<Component>		GetFixedComponent(ComponentType type);
	shared_ptr<Transform>		GetTransform();
	shared_ptr<Transform>		GetOrAddTransform();
	shared_ptr<Camera>			GetCamera();
	shared_ptr<Camera>			GetOrAddCamera();
	shared_ptr<MeshRenderer>	GetMeshRenderer();
	// ..

private:
	ComPtr<ID3D11Device>		_device;

protected:
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	vector<shared_ptr<MonoBehavior>> _scripts;
};