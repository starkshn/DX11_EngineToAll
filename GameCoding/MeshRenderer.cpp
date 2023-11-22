#include "pch.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Game.h"
#include "Pipeline.h"

MeshRenderer::MeshRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) : Super(ComponentType::MeshRenderer), _device(device), _deviceContext(deviceContext)
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

	// 10. 
	_texture1 = make_shared<Texture>(_device);
	_texture1->Create(L"BFS.jpg");
}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::Awake()
{

}

void MeshRenderer::Start()
{

}

void MeshRenderer::Update()
{

}

void MeshRenderer::LateUpdate()
{

}

void MeshRenderer::FixedUpdate()
{

}
