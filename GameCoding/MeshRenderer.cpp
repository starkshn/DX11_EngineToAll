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

	// 7. ���ϴ� ������� ���� �ϴ� �κ�
	_rasterizerState = make_shared<RasterizerState>(_device);
	_rasterizerState->Create();

	// 8. ��� ������ ���� ��Ÿ���� �κ�
	_blendState = make_shared<BlendState>(_device);
	_blendState->Create();

	// 9. ��� ����
	_cameraBuffer = make_shared<ConstantBuffer<CameraData>>(_device, _deviceContext);
	_cameraBuffer->Create();

	_transformBuffer = make_shared<ConstantBuffer<TransformData>>(_device, _deviceContext);
	_transformBuffer->Create();

	// 10. 
	_texture1 = make_shared<Texture>(_device);
	_texture1->Create(L"BFS.jpg");

	// 11.
	_samplerState = make_shared<SamplerState>(_device);
	_samplerState->Create();
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
	{
		// Camera
		_cameraData.matView = Camera::S_MatView;
		_cameraData.matProjection = Camera::S_MatProjection;
		// _cameraData.matView = Matrix::Identity;
		// _cameraData.matProjection = Matrix::Identity;

		// CPU -> GPU���� ������ ����
		_cameraBuffer->CopyData(_cameraData);
	}
	

	{
		// �Ϲ� GameObject (Monster)
		_transformData.matWorld = GetTransform()->GetWorldMatrix();

		// CPU -> GPU���� ������ ����
		_transformBuffer->CopyData(_transformData);
	}

	// Render
	Render(PIPE);
}

void MeshRenderer::LateUpdate()
{
}

void MeshRenderer::FixedUpdate()
{
}

void MeshRenderer::Render(shared_ptr<Pipeline> pipeline)
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
	pipeline->SetConstantBuffer(0, SS_VertexShader, _cameraBuffer);
	pipeline->SetConstantBuffer(1, SS_VertexShader, _transformBuffer);

	// RS

	// PS
	pipeline->SetTexture(0, SS_PixelShader, _texture1);
	pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);

	// OM
	pipeline->DrawIndexed(_geometry_V->GetIndexCount(), 0, 0);
}
