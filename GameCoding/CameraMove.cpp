#include "pch.h"
#include "CameraMove.h"


void CameraMove::Update()
{
	auto pos = GetTransform()->GetPosition();
	pos.x += 0.01f;

	GetTransform()->SetPosition(pos);
}
