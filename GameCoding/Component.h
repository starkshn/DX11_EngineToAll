#pragma once

class GameObject;
class Transform;

enum class ComponentType : uint8
{
	Transform,
	MeshRenderer,
	Camera,
	Animator,
	// ... 
	Script,

	End,
};

enum
{
	FIXED_COMPONENT_COUNT = static_cast<uint8>(ComponentType::End) -1
};

class Component
{
public:
	Component(ComponentType type);
	virtual ~Component();

public:
	virtual void Awake()		{}
	virtual void Start()		{}
	virtual void Update()		{}
	virtual void LateUpdate()	{}
	virtual void FixedUpdate()	{}

public:
	friend class GameObject;

public:
	shared_ptr<GameObject>	GetGameObject();
	shared_ptr<Transform>	GetTransform();

public:
	const ComponentType GetType() { return _type; }

private:
	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }
	
protected:
	ComponentType			_type;
	weak_ptr<GameObject>	_gameObject;
};

