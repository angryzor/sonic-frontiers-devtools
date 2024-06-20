#include "GOCVisualDebugDrawRenderer.h"
#include <utilities/math/MathUtils.h>

using namespace hh::game;
using namespace hh::gfx;

bool GOCVisualDebugDrawRenderer::colliderFilters[32][32]{ false };
bool GOCVisualDebugDrawRenderer::renderGOCVisualDebugDraw{ true };
bool GOCVisualDebugDrawRenderer::renderColliders{ true };
bool GOCVisualDebugDrawRenderer::renderOcclusionCapsules{ true };
uint8_t GOCVisualDebugDrawRenderer::gocVisualDebugDrawOpacity{ 80 };

#ifdef DEVTOOLS_TARGET_SDK_wars
constexpr size_t gocVisualDebugDrawSetupAddr = 0x140682D50;
#endif
#ifdef DEVTOOLS_TARGET_SDK_rangers
constexpr size_t gocVisualDebugDrawSetupAddr = 0x140D06320;
#endif

class GOCMyVisualDebugDraw : public GOCVisualDebugDraw {
public:
	//hh::fnd::Reference<hh::gfnd::GraphicsGeometry> geometry;
	hh::fnd::Reference<hh::gfnd::GraphicsGeometry> fillGeometry;
	csl::ut::Color<uint8_t> color{ 0, 0, 0, 0 };
	bool hasGeometry{};

	GOCMyVisualDebugDraw(csl::fnd::IAllocator* allocator);
	virtual void OnGOCEvent(GOCEvent event, GameObject& ownerGameObject, void* data);

	static void InstallHooks();

	GOCOMPONENT_CLASS_DECLARATION(GOCMyVisualDebugDraw)
};

hh::game::GOComponent* GOCMyVisualDebugDraw::Create(csl::fnd::IAllocator* allocator) {
	return new (std::align_val_t(alignof(GOCMyVisualDebugDraw)), allocator) GOCMyVisualDebugDraw(allocator);
}

GOCMyVisualDebugDraw::GOCMyVisualDebugDraw(csl::fnd::IAllocator* allocator)
#ifdef DEVTOOLS_TARGET_SDK_wars
	: GOCVisualDebugDraw{}
#else
	: GOCVisualDebugDraw{ allocator }
#endif
	//, geometry{ RESOLVE_STATIC_VARIABLE(hh::gfnd::DrawSystemNeedle::CreateGraphicsGeometry)(nullptr, allocator) }
	, fillGeometry{ RESOLVE_STATIC_VARIABLE(hh::gfnd::DrawSystemNeedle::CreateGraphicsGeometry)(nullptr, allocator) }
{
}

void GOCMyVisualDebugDraw::OnGOCEvent(GOCEvent event, GameObject& ownerGameObject, void* data) {
	if (event == GOCEvent::ADDED_TO_OBJECT)
		GOCVisualDebugDrawRenderer::instance->AddGOC(this);
	if (event == GOCEvent::REMOVED_FROM_OBJECT)
		GOCVisualDebugDrawRenderer::instance->RemoveGOC(this);
	
	GOCVisualDebugDraw::OnGOCEvent(event, ownerGameObject, data);
}

void GOCMyVisualDebugDraw::InstallHooks() {
	reinterpret_cast<hh::game::GOComponentClass*>(reinterpret_cast<size_t>(hh::gfx::GOCVisualDebugDraw::GetClass()))->instantiator = &GOCMyVisualDebugDraw::Create;
}

HOOK(bool, __fastcall, VisualDebugDrawSetup, gocVisualDebugDrawSetupAddr, GOCMyVisualDebugDraw* gocVisual, const GOCVisualDebugDraw::SetupInfo& setupInfo) {
	bool ret = originalVisualDebugDrawSetup(gocVisual, setupInfo);

	if (setupInfo.geometry != nullptr) {
		//gocVisual->geometry->InitializeEdge(GOCVisualDebugDrawRenderer::instance->drawContext, *setupInfo.geometry);
		//gocVisual->geometry->SetColor(setupInfo.color);

		csl::ut::Color8 fillColor{ setupInfo.color };
		//fillColor.a = 80;
		fillColor.a = GOCVisualDebugDrawRenderer::gocVisualDebugDrawOpacity;
		gocVisual->fillGeometry->Initialize(GOCVisualDebugDrawRenderer::instance->drawContext, *setupInfo.geometry);
		gocVisual->fillGeometry->SetColor(fillColor);

		gocVisual->hasGeometry = true;
	}

	gocVisual->color = setupInfo.color;

	return ret;
}

hh::fnd::Reference<GOCVisualDebugDrawRenderer> GOCVisualDebugDrawRenderer::instance = nullptr;

void GOCVisualDebugDrawRenderer::InstallHooks() {
	INSTALL_HOOK(VisualDebugDrawSetup);
	GOCMyVisualDebugDraw::InstallHooks();
}

#ifdef DEVTOOLS_TARGET_SDK_wars
GOCVisualDebugDrawRenderer::GOCVisualDebugDrawRenderer(csl::fnd::IAllocator* allocator)
	: CompatibleObject{ allocator }
	, drawContext{ RESOLVE_STATIC_VARIABLE(hh::gfnd::DrawSystem::CreateDrawContext)(allocator) }
	, renderable{ new (allocator) Renderable(allocator, this) }
#else
GOCVisualDebugDrawRenderer::GOCVisualDebugDrawRenderer(csl::fnd::IAllocator* allocator)
	: CompatibleObject{ allocator }
	, memCtx{ true }
	, unk2{ &memCtx }
	, unk3{ &unk2 }
	//, sharedDDRes{ RESOLVE_STATIC_VARIABLE(hh::gfnd::DrawSystem::CreateSharedDebugDrawResource)(allocator) }
	, drawContext{ RESOLVE_STATIC_VARIABLE(hh::gfnd::DrawSystem::CreateDrawContext)(allocator, &unk3) }
	, renderable{ new (allocator) Renderable(allocator, this) }
#endif
{
	renderable->name = "DevTools Debug Overlay";
	hh::gfnd::GraphicsContext::GetInstance()->AddRenderableToViewport(renderable, 7);// 5);
}

void GOCVisualDebugDrawRenderer::AddGOC(GOCMyVisualDebugDraw* goc) {
	gocs.push_back(goc);
}

void GOCVisualDebugDrawRenderer::RemoveGOC(GOCMyVisualDebugDraw* goc) {
	gocs.remove(gocs.find(goc));
}

void GOCVisualDebugDrawRenderer::Renderable::Render(const hh::gfnd::RenderableParameter* renderableParameter) {
	auto* gameManager = hh::game::GameManager::GetInstance();
	if (!gameManager)
		return;

	auto* cameraManager = gameManager->GetService<hh::game::CameraManager>();
	if (!cameraManager)
		return;

	auto* camera = cameraManager->GetTopComponent(0);
	if (!camera)
		return;

	renderer->drawContext->BeginScene(renderableParameter);

	hh::gfnd::DrawContext::BeginDrawInfo bdi{};
	memcpy(bdi.viewMatrix, camera->viewportData.viewMatrix.data(), sizeof(camera->viewportData.viewMatrix));
	memcpy(bdi.projMatrix, camera->viewportData.projMatrix.data(), sizeof(camera->viewportData.projMatrix));
	renderer->drawContext->BeginDraw(bdi);
	renderer->drawContext->SetCullingMode(1);

	for (auto& goc : renderer->gocs) {
		if (!goc->hasGeometry || (!(renderer->enabled && renderGOCVisualDebugDraw) && !(goc->visualFlags.test(GOCVisualDebugDraw::Flag::VISIBLE))))
			continue;

		//goc->geometry->Render(renderer->drawContext, goc->worldMatrix);
		goc->fillGeometry->Render(renderer->drawContext, goc->worldMatrix);
	}

	if (renderer->enabled) {
		if (renderColliders) {
			for (auto* gameObject : gameManager->objects) {
				//if (auto* gocV = gameObject->GetComponent<GOCVisualTransformed>()) {
				//	renderer->drawContext->DrawOBB(gocV->worldMatrix, { 1, 1, 1 }, { 255, 255, 255, 0 });
				//	renderer->drawContext->DrawAABB(gocV->transformedAabb.m_Min, gocV->transformedAabb.m_Max, { 255, 255, 255, 255 });
				//}
				for (auto* goc : gameObject->components) {
					if (goc->pStaticClass == hh::physics::GOCSphereCollider::GetClass()) {
						auto* cGoc = static_cast<hh::physics::GOCSphereCollider*>(goc);
						if (colliderFilters[gameObject->layer][cGoc->filterCategory])
							renderer->drawContext->DrawSphere(cGoc->GetWorldTransform(), cGoc->radius, { 0, 255, 255, 255 });
					}
					else if (goc->pStaticClass == hh::physics::GOCBoxCollider::GetClass()) {
						auto* cGoc = static_cast<hh::physics::GOCBoxCollider*>(goc);
						if (colliderFilters[gameObject->layer][cGoc->filterCategory])
							renderer->drawContext->DrawOBB(cGoc->GetWorldTransform(), cGoc->dimensions, { 0, 255, 255, 255 });
					}
					else if (goc->pStaticClass == hh::physics::GOCCapsuleCollider::GetClass()) {
						auto* cGoc = static_cast<hh::physics::GOCCapsuleCollider*>(goc);
						if (colliderFilters[gameObject->layer][cGoc->filterCategory])
							renderer->drawContext->DrawCapsule(cGoc->GetWorldTransform(), cGoc->height, cGoc->radius, { 0, 255, 255, 255 });
					}
					else if (goc->pStaticClass == hh::physics::GOCCylinderCollider::GetClass()) {
						auto* cGoc = static_cast<hh::physics::GOCCylinderCollider*>(goc);
						if (colliderFilters[gameObject->layer][cGoc->filterCategory])
							renderer->drawContext->DrawCylinder(cGoc->GetWorldTransform(), cGoc->height, cGoc->radius, { 0, 255, 255, 255 });
					}
				}
			}
		}
#ifdef DEVTOOLS_TARGET_SDK_wars
		if (renderOcclusionCapsules) {
			for (auto* gameObject : gameManager->objects) {
				for (auto* goc : gameObject->components) {
					if (goc->pStaticClass == hh::gfx::GOCOcclusionCapsule::GetClass()) {
						auto* cGoc = static_cast<hh::gfx::GOCOcclusionCapsule*>(goc);
						if (cGoc->frame == nullptr)
							continue;

						Eigen::Affine3f affine;
						affine.fromPositionOrientationScale(cGoc->worldPos.m_Position, cGoc->worldPos.m_Rotation, cGoc->scale);
						renderer->drawContext->DrawSphere({ TransformToAffine3f(cGoc->frame->fullTransform) * affine }, 1, { 255, 0, 255, 255 });
					}
				}
			}
		}
#endif

		//auto renderParam = static_cast<RenderingEngineRangers*>(static_cast<RenderManager*>(RenderManager::GetInstance())->GetNeedleResourceDevice())->mainRenderUnit->pipelineInfo->renderParam;
		//for (size_t i = 0; i < renderParam.numViewports; i++) {
		//	auto inverseMat = (renderParam.viewports[i].camera->GetProjectionMatrix() * renderParam.viewports[i].camera->GetViewMatrix()).inverse();
		//	auto frustumRays = ScreenRectToFrustumRays(Eigen::Vector2f{ -1, -1 }, Eigen::Vector2f{ 1, 1 }, inverseMat);
		//	hh::gfnd::DrawVertex vertices[8]{
		//		{ frustumRays.topLeft.start.x(), frustumRays.topLeft.start.y(), frustumRays.topLeft.start.z(), 0xFFFFFFFF },
		//		{ frustumRays.topLeft.end.x(), frustumRays.topLeft.end.y(), frustumRays.topLeft.end.z(), 0xFFFFFFFF },
		//		{ frustumRays.topRight.start.x(), frustumRays.topRight.start.y(), frustumRays.topRight.start.z(), 0xFFFFFFFF },
		//		{ frustumRays.topRight.end.x(), frustumRays.topRight.end.y(), frustumRays.topRight.end.z(), 0xFFFFFFFF },
		//		{ frustumRays.botRight.start.x(), frustumRays.botRight.start.y(), frustumRays.botRight.start.z(), 0xFFFFFFFF },
		//		{ frustumRays.botRight.end.x(), frustumRays.botRight.end.y(), frustumRays.botRight.end.z(), 0xFFFFFFFF },
		//		{ frustumRays.botLeft.start.x(), frustumRays.botLeft.start.y(), frustumRays.botLeft.start.z(), 0xFFFFFFFF },
		//		{ frustumRays.botLeft.end.x(), frustumRays.botLeft.end.y(), frustumRays.botLeft.end.z(), 0xFFFFFFFF },
		//	};
		//	unsigned short indices[8]{ 0, 1, 2, 3, 4, 5, 6, 7 };

		//	renderer->drawContext->DrawPrimitive(hh::gfnd::PrimitiveType::LINE_LIST, vertices, indices, 8);
		//}
	}

	for (auto* debugRenderable : renderer->additionalRenderables)
		debugRenderable->RenderDebugVisuals(*renderer->drawContext);

	renderer->drawContext->EndDraw();
	renderer->drawContext->EndScene();
}

GOCVisualDebugDrawRenderer::Renderable::Renderable(csl::fnd::IAllocator* allocator, GOCVisualDebugDrawRenderer* renderer)
#ifdef DEVTOOLS_TARGET_SDK_wars
	: hh::gfnd::Renderable{}
#else
	: hh::gfnd::Renderable{ allocator }
#endif
	, renderer{ renderer }
{
}

DebugRenderable::DebugRenderable() {
	GOCVisualDebugDrawRenderer::instance->additionalRenderables.push_back(this);
}

DebugRenderable::~DebugRenderable() {
	GOCVisualDebugDrawRenderer::instance->additionalRenderables.remove(GOCVisualDebugDrawRenderer::instance->additionalRenderables.find(this));
}
