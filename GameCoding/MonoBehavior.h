#pragma once
#include "Component.h"
class MonoBehavior : public Component
{
	using Super = Component;

public:
	MonoBehavior();
	virtual ~MonoBehavior() override;

public:
	virtual void Awake() override;
	virtual void Update() override;

};

