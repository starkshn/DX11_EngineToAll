#pragma once
#include "Component.h"

class MonoBehaviour : public Component
{
	using Super = Component;

public:
	MonoBehaviour();
	virtual ~MonoBehaviour() override;

public:
	virtual void Awake() override;
	virtual void Update() override;

};

