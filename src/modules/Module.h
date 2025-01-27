#pragma once
#include <utilities/CompatibleObject.h>

class Module : public CompatibleObject {
public:
	using CompatibleObject::CompatibleObject;

	virtual unsigned int GetId() const = 0;
	virtual void Update() = 0;
};
