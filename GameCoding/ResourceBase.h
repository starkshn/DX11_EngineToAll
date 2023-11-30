#pragma once

enum class ResourceType : uint8
{
	None = -1,

	Mesh,
	Shader,
	Texture,
	Material,
	Animation,

	End
};

enum
{
	RESOURCE_TYPE_COUNT = static_cast<uint8>(ResourceType::End),
};

class ResourceBase : public enable_shared_from_this<ResourceBase>
{
public:
	ResourceBase(ResourceType type);
	virtual ~ResourceBase();

protected:
	virtual void Load(const wstring& path);
	virtual void Save(const wstring& path);

public:
	void SetName(const wstring& name) { _name = name; }
	const wstring& GetName() { return _name; }
	
	const ResourceType GetType() { return _type; }
	const uint32 GetId() { return _id; }

protected:
	ResourceType	_type = ResourceType::None;
	wstring			_name;
	wstring			_path;
	uint32			_id{ 0 };
};

