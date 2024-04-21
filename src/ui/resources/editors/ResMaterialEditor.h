#pragma once

#include <ui/common/StandaloneWindow.h>

class ResMaterialEditor : public StandaloneWindow {
	hh::fnd::Reference<hh::gfx::ResMaterial> resource;
public:
	ResMaterialEditor(csl::fnd::IAllocator* allocator, hh::gfx::ResMaterial* resource);
	virtual void RenderContents();
	static ResMaterialEditor* Create(csl::fnd::IAllocator* allocator, hh::gfx::ResMaterial* resource);
	void RenderFloatParameterEditor(size_t idx, hh::needle::ParameterFloatValue* data);
	void RenderBoolParameterEditor(size_t idx, hh::needle::ParameterBoolValue* data);
	void RenderIntParameterEditor(size_t idx, hh::needle::ParameterIntValue* data);
	void RenderSamplerParameterEditor(size_t idx, hh::needle::ParameterSamplerValue* data);
	void RenderRsFlagMaskParameterEditor(size_t idx, hh::needle::ParameterRsFlagMaskValue* data);
	void RenderShaderNameParameterEditor(size_t idx, hh::needle::ParameterShaderNameValue* data);
	void RenderTextureNameParameterEditor(size_t idx, hh::needle::ParameterTextureNameValue* data);
	void RenderUnimplementedTypeParameterEditor(size_t idx);
};