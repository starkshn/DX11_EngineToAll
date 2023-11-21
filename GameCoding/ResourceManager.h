#pragma once

#include "ResourceBase.h"

class Mesh;
class Shader;
class Texture;
class Material;
class Animation;

class ResourceManager
{
public:
	ResourceManager(ComPtr<ID3D11Device> device);
	~ResourceManager();

public:
	void Init();

	template<typename T>
	shared_ptr<T> Load(const wstring& key, const wstring path);
	
	template <typename T>
	bool Add(const wstring& key, shared_ptr<T> obj);

	template <typename T>
	shared_ptr<T> Get(const wstring& key);

	// Get<Texture>
	template <typename T>
	ResourceType GetResourceType();
	
private:
	void CreateDefaultTexture();
	void CreateDefaultMesh();
	void CreateDefaultShader();
	void CreateDefaultMaterial();
	void CreateDefaultAnimation();

private:
	ComPtr<ID3D11Device> _device;

	using KeyObjMap = map<wstring, shared_ptr<ResourceBase>>;
	array<KeyObjMap, RESOURCE_TYPE_COUNT> _resources;
};

template<typename T>
inline shared_ptr<T> ResourceManager::Load(const wstring& key, const wstring path)
{
	// 1. 원래 메모리에 들고 있었다면 바로 return
	auto objectType = GetResourceType<T>();
	KeyObjMap keyObjMap = _resources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	// 2. 안 들고 있던 경우라면
	shared_ptr<T> object = make_shared<T>();
	object->Load(path); // 오버 라이딩 해야한다? (오버로딩 해야하지 않나?)
	keyObjMap[key] = object;

	return object;
}

template<typename T>
inline bool ResourceManager::Add(const wstring& key, shared_ptr<T> object)
{
	ResourceType resourceType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	auto findIt = keyObjMap.find(key);

	if (findIt != keyObjMap.end())
		return false;

	keyObjMap[key] = object;
	return true;
}

template<typename T>
inline shared_ptr<T> ResourceManager::Get(const wstring& key)
{
	ResourceType resourceType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	auto findIt = keyObjMap.find(key);

	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	return nullptr;
}

template<typename T>
inline ResourceType ResourceManager::GetResourceType()
{
	if (std::is_same_v<T, Mesh>)
		return ResourceType::Mesh;

	if (std::is_same_v<T, Shader>)
		return ResourceType::Shader;

	if (std::is_same_v<T, Texture>)
		return ResourceType::Texture;

	if (std::is_same_v<T, Material>)
		return ResourceType::Material;

	if (std::is_same_v<T, Animation>)
		return ResourceType::Animation;

	assert(false);
	return ResourceType::None;
}
