#pragma once
#include "Component.h"

enum class ProjectionType
{
	Perspective,	// 원근 투영
	Orthographic,	// 직교 투영
};

class Camera : public Component
{
	using Super = Component;

public:
	Camera();
	virtual ~Camera() override;

public:
	virtual void Update() override;

public:
	void SetProjectionType(ProjectionType type) { _type = type; }
	const ProjectionType GetProjectionType() { return _type; }

public:
	void UpdateMatrix();

private:
	ProjectionType _type = ProjectionType::Orthographic;

public:
	static Matrix S_MatView;
	static Matrix S_MatProjection;
};

