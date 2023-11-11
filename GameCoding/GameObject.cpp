#include "pch.h"
#include "GameObject.h"
#include "MonoBehavior.h"
#include "Transform.h"


GameObject::GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	:
	_device(device), _deviceContext(deviceContext)
{

	// 1. 도형 표현한다.
	_geometry_V = make_shared<Geometry<VertexTextureData>>();
	GeometryHelper::CreateRectangle(_geometry_V);

	// 2. 표현한 모양대로 Buffer를 만들어 달라고 요청한다.
	_vertexBuffer = make_shared<VertexBuffer>(_device);
	_vertexBuffer->Create(_geometry_V->GetVertices());

	// 3. 기하학의 정점을 표현(불필요한 인덱스 연산 피하기 위해)
	_indexBuffer = make_shared<IndexBuffer>(_device);
	_indexBuffer->Create(_geometry_V->GetIndices());

	// 4. Shader만들어서 Blob을 만든다.
	_vertexShader = make_shared<VertexShader>(_device);
	_vertexShader->Create(L"Default.hlsl", "VS", "vs_5_0");

	// 5. Shader에 구성되어 있는 것들 GPU, CPU에게 알려주고 연결
	_inputLayout = make_shared<InputLayout>(_device);
	_inputLayout->Create(VertexTextureData::descs, _vertexShader->GetBlob());

	// 6. vertexShader와 동일 하다.
	_pixelShader = make_shared<PixelShader>(_device);
	_pixelShader->Create(L"Default.hlsl", "PS", "ps_5_0");

	// 7. 원하는 방식으로 셋팅 하는 부분
	_rasterizerState = make_shared<RasterizerState>(_device);
	_rasterizerState->Create();

	// 8. 어떻게 섞여야 할지 나타내는 부분
	_blendState = make_shared<BlendState>(_device);
	_blendState->Create();

	// 9. 상수 버퍼
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

	// CPU -> GPU로의 데이터 복사
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
