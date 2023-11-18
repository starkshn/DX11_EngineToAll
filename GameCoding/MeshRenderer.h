#pragma once
#include "Component.h"
class MeshRenderer : public Component
{
	using Super = Component;
public:
	MeshRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	virtual ~MeshRenderer() override;

public:
	virtual void Awake()		override;
	virtual void Start()		override;
	virtual void Update()		override;
	virtual void LateUpdate()	override;
	virtual void FixedUpdate()	override;

public:
	void Render(shared_ptr<Pipeline> pipeline);

private:
	ComPtr<ID3D11Device>						_device;
	ComPtr<ID3D11DeviceContext>					_deviceContext;

	// Mesh
	shared_ptr<Geometry<VertexTextureData>>		_geometry_V;
	shared_ptr<Geometry<VertexColorData>>		_geometry_C;
	shared_ptr<VertexBuffer>					_vertexBuffer;
	shared_ptr<IndexBuffer>						_indexBuffer;

	// Material
	shared_ptr<InputLayout>						_inputLayout;
	shared_ptr<VertexShader>					_vertexShader;
	shared_ptr<PixelShader>						_pixelShader;
	shared_ptr<RasterizerState>					_rasterizerState;
	shared_ptr<Texture>							_texture1;
	shared_ptr<SamplerState>					_samplerState;
	shared_ptr<BlendState>						_blendState;

private:
	// Camera
	CameraData									_cameraData;
	shared_ptr<ConstantBuffer<CameraData>>		_cameraBuffer;

private:
	// SRT
	TransformData								_transformData;
	shared_ptr<ConstantBuffer<TransformData>>	_transformBuffer;
};

