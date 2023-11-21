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

	// 7. 원하는 방식으로 셋팅 하는 부분
	_rasterizerState = make_shared<RasterizerState>(_device);
	_rasterizerState->Create();

	// 8. 어떻게 섞여야 할지 나타내는 부분
	_blendState = make_shared<BlendState>(_device);
	_blendState->Create();

	// 9. 상수 버퍼
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

		// CPU -> GPU로의 데이터 복사
		_cameraBuffer->CopyData(_cameraData);
	}
	

	{
		// 일반 GameObject (Monster)
		_transformData.matWorld = GetTransform()->GetWorldMatrix();

		// CPU -> GPU로의 데이터 복사
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
