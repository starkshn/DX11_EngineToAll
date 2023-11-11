#include "pch.h"
#include "Transform.h"

Transform::Transform() 
	: 
	Super(ComponentType::Transform)
{
	
}

Transform::~Transform()
{

}

void Transform::Awake()
{

}

void Transform::Update()
{

}

Vec3 ToEulerAngles(Quaternion q)
{
	Vec3 angles;

	// roll (x-axis rotation)
	double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
	double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
	angles.y = 2 * std::atan2(sinp, cosp) - 3.14159f / 2;

	// yaw (z-axis rotation)
	double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}

void Transform::UpdateTransform()
{
	// SRT
	Matrix matScale = Matrix::CreateScale(_localScale);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	// 부모님을 기준으로하는 나의 로컬 좌표(변환 좌표)
	_matLocal = matScale * matRotation * matTranslation;

	if (HasParent())
	{
		// _matWorld는 월드 변환행렬이 된다.
		_matWorld = _matLocal * _parent->GetWorldMatrix();
	}
	else
	{
		// 부모가 없다면 LocalMatrix와 WorldMatrix가 같다.
		_matWorld = _matLocal;
	}

	Quaternion quat;
	_matWorld.Decompose(_scale, quat, _position);
	_rotation = ToEulerAngles(quat);

	// 곱하는 방식이 두가지.
	// TransformCoord
	// TransformNormal (방향)
	// (1, 0, 0) * MatWorld => World를 기준으로 하는 right vector구함.
	_right	= Vec3::TransformNormal(Vec3::Right, _matWorld);
	_up		= Vec3::TransformNormal(Vec3::Up, _matWorld);
	_look	= Vec3::TransformNormal(Vec3::Backward, _matWorld);

	// Children
	for (const shared_ptr<Transform>& child : _children)
	{
		child->UpdateTransform();
	}
}

void Transform::SetScale(const Vec3& worldScale)
{
	if (HasParent())
	{
		Vec3 parentScale = _parent->GetScale();
		Vec3 scale = worldScale;
		scale.x /= parentScale.x;
		scale.y /= parentScale.y;
		scale.z /= parentScale.z;
		SetLocalScale(scale);
	}
	else
	{
		SetLocalScale(worldScale);
	}
}

void Transform::SetRotation(const Vec3& worldRotation)
{
	if (HasParent())
	{
		// 월드에서 부모 로컬로 가는 좌표계이다.
		Matrix inversMatrix = _parent->GetWorldMatrix().Invert();

		Vec3 rotation;
		rotation.TransformNormal(worldRotation, inversMatrix);

		SetLocalRotation(rotation);
	}
	else
	{
		SetLocalRotation(worldRotation);
	}
}

void Transform::SetPosition(const Vec3& worldPosition)
{
	if (HasParent())
	{
		// 월드에서 부모 로컬로 가는 좌표계이다.
		Matrix worldToParentLocalMatrix = _parent->GetWorldMatrix().Invert();
		
		Vec3 position;
		//Transform는 TransformCoord방식이다.
		position.Transform(worldPosition, worldToParentLocalMatrix);

		SetLocalPosition(position);
	}
	else
	{
		SetLocalPosition(worldPosition);
	}
}
