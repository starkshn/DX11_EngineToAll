#pragma once

enum ShaderScope : uint32
{
	SS_None = 0,
	SS_VertexShader = (1 << 0),
	SS_PixelShader = (1 << 1),
	SS_Both = SS_VertexShader | SS_PixelShader,
};


class ShaderBase
{
public:
	ShaderBase(ComPtr<ID3D11Device> device);
	virtual ~ShaderBase();

public:
	ComPtr<ID3DBlob> GetBlob() { return _blob; }
	virtual void Create(const wstring& path, const string& name, const string& version) abstract;

protected:
	void LoadShaderFromFile(const wstring& path, const string& name, const string& version);

protected:
	ComPtr<ID3D11Device>	_device;
	ComPtr<ID3DBlob>		_blob;

	wstring		_path;
	string		_name;
};

class VertexShader : public ShaderBase
{
	using Super = ShaderBase;
public:
	VertexShader(ComPtr<ID3D11Device> device);
	virtual ~VertexShader() override;

public:
	ComPtr<ID3D11VertexShader> GetComPtr() { return _vertexShader; }

public:
	virtual void Create(const wstring& path, const string& name, const string& version) override;

protected:
	ComPtr<ID3D11VertexShader>		_vertexShader = nullptr;
};


class PixelShader : public ShaderBase
{
	using Super = ShaderBase;
public:
	PixelShader(ComPtr<ID3D11Device> device);
	virtual ~PixelShader() override;

public:
	ComPtr<ID3D11PixelShader> GetComPtr() { return _pixelShader; }

public:
	virtual void Create(const wstring& path, const string& name, const string& version) override;

protected:
	ComPtr<ID3D11PixelShader>		_pixelShader = nullptr;
};

