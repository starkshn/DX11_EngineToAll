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

private:
	ComPtr<ID3D11Device>						_device;
	ComPtr<ID3D11DeviceContext>					_deviceContext;

	friend class RenderManager;

	// Mesh
	shared_ptr<Geometry<VertexTextureData>>		_geometry_V;
	shared_ptr<Geometry<VertexColorData>>		_geometry_C;
	shared_ptr<VertexBuffer>					_vertexBuffer;
	shared_ptr<IndexBuffer>						_indexBuffer;

	// Material
	shared_ptr<InputLayout>						_inputLayout;
	shared_ptr<VertexShader>					_vertexShader;
	shared_ptr<PixelShader>						_pixelShader;
	shared_ptr<Texture>							_texture1;

private:

};

