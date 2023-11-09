#pragma once
class StandaloneWindow : public hh::fnd::ReferencedObject
{
public:
	StandaloneWindow(csl::fnd::IAllocator* allocator);
	virtual void Render() = 0;
};

