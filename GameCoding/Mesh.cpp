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
	// 1. 도형 표현한다.
	_geometry_V = make_shared<Geometry<VertexTextureData>>();
	GeometryHelper::CreateRectangle(_geometry_V);

	// 2. 표현한 모양대로 Buffer를 만들어 달라고 요청한다.
	_vertexBuffer = make_shared<VertexBuffer>(_device);
	_vertexBuffer->Create(_geometry_V->GetVertices());

	// 3. 기하학의 정점을 표현(불필요한 인덱스 연산 피하기 위해)
	_indexBuffer = make_shared<IndexBuffer>(_device);
	_indexBuffer->Create(_geometry_V->GetIndices());
}
