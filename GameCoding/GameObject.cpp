#include "pch.h"
#include "GameObject.h"
#include "MonoBehavior.h"
#include "Transform.h"


GameObject::GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	:
	_device(device), _deviceContext(deviceContext)
{

	// 1. ���� ǥ���Ѵ�.
	_geometry_V = make_shared<Geometry<VertexTextureData>>();
	GeometryHelper::CreateRectangle(_geometry_V);

	// 2. ǥ���� ����� Buffer�� ����� �޶�� ��û�Ѵ�.
	_vertexBuffer = make_shared<VertexBuffer>(_device);
	_vertexBuffer->Create(_geometry_V->GetVertices());

	// 3. �������� ������ ǥ��(���ʿ��� �ε��� ���� ���ϱ� ����)
	_indexBuffer = make_shared<IndexBuffer>(_device);
	_indexBuffer->Create(_geometry_V->GetIndices());

	// 4. Shader���� Blob�� �����.
	_vertexShader = make_shared<VertexShader>(_device);
	_vertexShader->Create(L"Default.hlsl", "VS", "vs_5_0");

	// 5. Shader�� �����Ǿ� �ִ� �͵� GPU, CPU���� �˷��ְ� ����
	_inputLayout = make_shared<InputLayout>(_device);
	_inputLayout->Create(VertexTextureData::descs, _vertexShader->GetBlob());

	// 6. vertexShader�� ���� �ϴ�.
	_pixelShader = make_shared<PixelShader>(_device);
	_pixelShader->Create(L"Default.hlsl", "PS", "ps_5_0");

	// 7. ���ϴ� ������� ���� �ϴ� �κ�
	_rasterizerState = make_shared<RasterizerState>(_device);
	_rasterizerState->Create();

	// 8. ��� ������ ���� ��Ÿ���� �κ�
	_blendState = make_shared<BlendState>(_device);
	_blendState->Create();

	// 9. ��� ����
	_constantBuffer = make_shared<ConstantBuffer<TransformData>>(_device, _deviceContext);
	_constantBuffer->Create();

	// 10. 
	_texture1 = make_shared<Texture>(_device);
	_texture1->Create(L"BFS.jpg");

	// 11.
	_samplerState = make_shared<SamplerState>(_device);
	_samplerState->Create();
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

	_transformData.matWorld = GetOrAddTransform()->GetWorldMatrix();

	// CPU -> GPU���� ������ ����
	_constantBuffer->CopyData(_transformData);
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

void GameObject::AddComponent(shared_ptr<Component> component)
{
	// this�� ����Ʈ ������ ����
	component->SetGameObject(shared_from_this());

	uint8 idx = static_cast<uint8>(component->GetType());
	if (idx < FIXED_COMPONENT_COUNT)
	{
		_components[idx] = component;
	}
	else
	{
		// static_pointer_cast�ص� �������(Ÿ���� Ȯ���ϴϱ�)
		_scripts.push_back(dynamic_pointer_cast<MonoBehavior>(component));
	}	
}

void GameObject::Render(shared_ptr<Pipeline> pipeline)
{
	PipelineInfo info;
	info.inputLayout		= _inputLayout;
	info.vertexShader		= _vertexShader;
	info.pixelShader		= _pixelShader;
	info.rasterizerState	= _rasterizerState;
	info.blendState			= _blendState;
	info.topology			= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	pipeline->UpdatePipeline(info);

	// IA
	pipeline->SetVertexBuffer(_vertexBuffer);
	pipeline->SetIndexBuffer(_indexBuffer);

	// VS
	pipeline->SetConstantBuffer(0, SS_VertexShader, _constantBuffer);

	// RS

	// PS
	pipeline->SetTexture(0, SS_PixelShader, _texture1);
	pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);
	
	// OM
	pipeline->DrawIndexed(_geometry_V->GetIndexCount(), 0, 0);
}
