#pragma once
#include <resources/ManagedMemoryRegistry.h>
#include <resources/managed-memory/ManagedCArray.h>
#include <utilities/CompatibleObject.h>
#include <random>
#include <windows.h>

namespace ui::operation_modes::modes::dvscene_editor {
	class Context : public CompatibleObject {
	public:
		using CompatibleObject::CompatibleObject;

		hh::fnd::Reference<hh::dv::DvSceneControl> goDVSC{};
		app::evt::EventScene* evtScene;

		double timelineFalloff;
		csl::ut::VariableString cutsceneName;

		Context(csl::fnd::IAllocator* allocator);
	};
}
