#include "pch.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Game.h"
#include "Pipeline.h"

MeshRenderer::MeshRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) : Super(ComponentType::MeshRenderer), _device(device), _deviceContext(deviceContext)
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
