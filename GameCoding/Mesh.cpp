#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(ComPtr<ID3D11Device> device)
	: Super(ResourceType::Mesh),
	_device(device)
{

}

Mesh::~Mesh()
{

}

void Mesh::CreateDefaultRectangle()
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
}
