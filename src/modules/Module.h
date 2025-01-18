#pragma once
#include <utilities/CompatibleObject.h>

class Module : public CompatibleObject {
public:
	using CompatibleObject::CompatibleObject;

	virtual void Update() = 0;
};
