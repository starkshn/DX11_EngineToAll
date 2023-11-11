#pragma once
#include "Component.h"

class Transform : public Component
{
	using Super = Component;

public:
	Transform();
	~Transform();

public:
	virtual void Awake() override;
	virtual void Update() override;

public:
	void UpdateTransform();

public:
	// Local
	Vec3 GetLocalScale() { return _localScale; }
	void SetLocalScale(const Vec3& localScale) { _localScale = localScale; UpdateTransform(); }

	Vec3 GetLocalRotation() { return _localRotation; }
	void SetLocalRotation(const Vec3& localRotation) {
		_localRotation = localRotation; UpdateTransform(); }

	Vec3 GetLocalPosition() { return _localPosition; }
	void SetLocalPosition(const Vec3& localPosition) {
		_localPosition = localPosition; UpdateTransform(); }

public:
	// World
	Vec3 GetScale() { return _scale; }
	void SetScale(const Vec3& worldScale);

	Vec3 GetRotation() { return _rotation; }
	void SetRotation(const Vec3& worldRotation);

	Vec3 GetPosition() { return _position; }
	void SetPosition(const Vec3& worldPosition);

public:
	// Right, Up, Look
	Vec3 GetRight() { return _matWorld.Right(); }
	Vec3 GetUp()	{ return _matWorld.Up(); }
	Vec3 GetLook()	{ return _matWorld.Backward(); }

public:
	// GetWorldMatrix
	Matrix GetWorldMatrix() { return _matWorld; }

public:
	// Parent
	bool HasParent() const { return _parent != nullptr; };
	shared_ptr<Transform> GetParent() { return _parent; }
	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }
	
public:
	// Child
	const vector<shared_ptr<Transform>>& GetChildren() { return _children; }
	void AddChild(const shared_ptr<Transform>& child) { _children.push_back(child); }


private:
	// Local
	Vec3 _localScale	= { 1.f, 1.f, 1.f };
	Vec3 _localRotation = { 0.f, 0.f, 0.f };
	Vec3 _localPosition = { 0.f, 0.f, 0.f };

private:
	// Cache
	Matrix _matLocal	= Matrix::Identity; // 항등행렬
	Matrix _matWorld	= Matrix::Identity;

private:
	// SRT
	Vec3 _scale			= {};
	Vec3 _rotation		= {};
	Vec3 _position		= {};

private:
	// right, up, look
	Vec3 _right;
	Vec3 _up;
	Vec3 _look;

private:
	shared_ptr<Transform>			_parent;
	vector<shared_ptr<Transform>>	_children;
};

