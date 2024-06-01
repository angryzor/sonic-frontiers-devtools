#pragma once

namespace heur::resources {
    class ResComputeShader { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
    class ResFragmentShader { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
    class ResVertexShader { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
    class ResLevel { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
    class ResMasterLevel { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
    class ResBitmapFont { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
    class ResScalableFontSet { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
    class ResEffect { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
    class ResPhysicalSkeleton { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
                                      //class ResSoftBody { public: inline static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
                                      //class ResHelperBone { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
                                      //class ResOrcaData { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
    class ResAnimation { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
    class ResSkeleton { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
    class ResDvScene { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
}

namespace app::gfx {
    class ResPointcloudModel { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
}

const char* GetExtensionByTypeInfo(const hh::fnd::ResourceTypeInfo* typeInfo);
const hh::fnd::ResourceTypeInfo* GetTypeInfoByExtension(const char* extension);
