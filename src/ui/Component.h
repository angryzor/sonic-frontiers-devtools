#pragma once
#include <ui/Action.h>
#include <utilities/CompatibleObject.h>

class Component : public CompatibleObject, public ActionDispatcher, public ActionProcessor {
public:
	using CompatibleObject::CompatibleObject;

	virtual void Render();
};
