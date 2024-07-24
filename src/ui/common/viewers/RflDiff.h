#pragma once
#include <reflection/RflDiff.h>

void Viewer(const char* label, const RflDiffResult& diff, void* obj, const hh::fnd::RflClass* rflClass);
void Rfl2HmmViewer(const char* label, const RflDiffResult& diff, void* obj, const hh::fnd::RflClass* rflClass);
