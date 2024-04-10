#pragma once

const char* GetExtensionByTypeInfo(const hh::fnd::ResourceTypeInfo* typeInfo);
const hh::fnd::ResourceTypeInfo* GetTypeInfoByExtension(const char* extension);
