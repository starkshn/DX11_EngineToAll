#include "pch.h"
#include "RenderManager.h"
#include "Pipeline.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Game.h"

RenderManager::RenderManager(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	: _device(device), _deviceContext(deviceContext)
{

}

RenderManager::~RenderManager()
{
	
}

void RenderManager::Init()
{
	_pipeLine = make_shared<Pipeline>(_deviceContext);

	_cameraBuffer = make_shared<ConstantBuffer<CameraData>>(_device, _deviceContext);
	_cameraBuffer->Create();

	_transformBuffer = make_shared<ConstantBuffer<TransformData>>(_device, _deviceContext);
	_transformBuffer->Create();

	// 7. 원하는 방식으로 셋팅 하는 부분
	_rasterizerState = make_shared<RasterizerState>(_device);
	_rasterizerState->Create();

	// 8. 어떻게 섞여야 할지 나타내는 부분
	_blendState = make_shared<BlendState>(_device);
	_blendState->Create();

	_samplerState = make_shared<SamplerState>(_device);
	_samplerState->Create();
}

void RenderManager::Update(shared_ptr<Graphics> graphics)
{
	graphics->RenderBegin();

	PushCameraData();
	GatherRenderableObjects();
	RenderObjects();

	graphics->RenderEnd();
}

void RenderManager::PushCameraData()
{
	_cameraData.matView = Camera::S_MatView;
	_cameraData.matProjection = Camera::S_MatProjection;
	_cameraBuffer->CopyData(_cameraData);
}

void RenderManager::PushTransformData()
{
	_transformBuffer->CopyData(_transformData);
}

void RenderManager::GatherRenderableObjects()
{
	_renderObjects.clear();
	
	auto& gameObjects = SCENE->GetActiveScene()->GetGameObjects();
	for (const shared_ptr<GameObject>& gameObject : gameObjects)
	{
		shared_ptr<MeshRenderer> meshRenderer = gameObject->GetMeshRenderer();
		if (meshRenderer != nullptr)
		{
			_renderObjects.push_back(gameObject);
		}
	}
}

void RenderManager::RenderObjects()
{
	for (const shared_ptr<GameObject>& gameObject : _renderObjects)
	{
		shared_ptr<MeshRenderer> meshRenderer = gameObject->GetMeshRenderer();
		if (meshRenderer == nullptr) continue;

		shared_ptr<Transform> transform = gameObject->GetTransform();
		if (transform == nullptr) continue;

		// SRT
		_transformData.matWorld = transform->GetWorldMatrix();
		PushTransformData();
		
		PipelineInfo info;
		info.inputLayout = meshRenderer->_inputLayout;
		info.vertexShader = meshRenderer->_vertexShader;
		info.pixelShader = meshRenderer->_pixelShader;
		info.rasterizerState = _rasterizerState;
		info.blendState = _blendState;
		info.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		_pipeLine->UpdatePipeline(info);

		// IA
		_pipeLine->SetVertexBuffer(meshRenderer->_vertexBuffer);
		_pipeLine->SetIndexBuffer(meshRenderer->_indexBuffer);

		// VS
		_pipeLine->SetConstantBuffer(0, SS_VertexShader, _cameraBuffer);
		_pipeLine->SetConstantBuffer(1, SS_VertexShader, _transformBuffer);

		// RS

		// PS
		_pipeLine->SetTexture(0, SS_PixelShader, meshRenderer->_texture1);
		_pipeLine->SetSamplerState(0, SS_PixelShader, _samplerState);

		// OM
		_pipeLine->DrawIndexed(meshRenderer->_geometry_V->GetIndexCount(), 0, 0);
		
	}
}
