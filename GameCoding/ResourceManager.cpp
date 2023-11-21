#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"

ResourceManager::ResourceManager(ComPtr<ID3D11Device> device)
	: _device(device)
{

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::Init()
{
	CreateDefaultTexture();
	CreateDefaultMesh();
	CreateDefaultShader();
	CreateDefaultMaterial();
	CreateDefaultAnimation();
}

void ResourceManager::CreateDefaultTexture()
{
	{
		auto texture = make_shared<Texture>(_device);
		texture->SetName(L"Poo");
		texture->Create(L"BFS.jpg");
		Add(texture->GetName(), texture);
	}
	
	Get<Texture>(L"Poo");
}

void ResourceManager::CreateDefaultMesh()
{

}

void ResourceManager::CreateDefaultShader()
{

}

void ResourceManager::CreateDefaultMaterial()
{

}

void ResourceManager::CreateDefaultAnimation()
{

}
