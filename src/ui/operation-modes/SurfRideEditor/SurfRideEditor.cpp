#include "SurfRideEditor.h"
#include <utilities/math/MathUtils.h>
#include <ui/common/viewers/Basic.h>

using namespace SurfRide;

SurfRideEditor::SurfRideEditor(csl::fnd::IAllocator* allocator) : OperationMode{ allocator }, projectTree{ allocator, *this }, timeline{ allocator, *this }, elementInspector{ allocator, *this }
{

}

SurfRideEditor::~SurfRideEditor()
{
}

void SurfRideEditor::Render()
{
	projectTree.Render();
	timeline.Render();
	elementInspector.Render();

	if (gocSprite == nullptr)
		return;

	HandleManipulation();
	HandleSelection();
	CalcSelectionAabb();
	DrawSelectionAabb();

	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
		selecting = false;
		dragSelecting = false;
		manipulating = false;
	}
}

void SurfRideEditor::HandleSelection() {
	auto& io = ImGui::GetIO();

	if (manipulating)
		return;

	if (!io.WantCaptureMouse && !ImGui::IsAnyItemHovered())
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			selecting = true;

	if (!selecting)
		return;

	if (auto* project = gocSprite->GetProject()) {
		if (!dragSelecting && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
			auto mousePos = ImGui::GetMousePos();

			for (auto* scene : project->scenes) {
				auto worldRay = ScreenPosToWorldRay(mousePos, (scene->camera.projectionMatrix * scene->camera.viewMatrix).inverse());

				for (auto* layer : scene->layers) {
					if (!(layer->flags & 0x100)) {
						for (auto* rootCast : layer->topLevelCasts) {
							if (auto* cast = RayCastCast(rootCast, worldRay)) {
								Select({ Selection::Type::CAST, cast });
								return;
							}
						}
					}
				}
			}
		}
		else if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
			dragSelecting = true;
			auto mouseStart = ImGui::GetMousePos() - ImGui::GetMouseDragDelta();
			auto mouseEnd = ImGui::GetMousePos();

			ImGui::Begin("Overlay");
			ImGui::GetWindowDrawList()->AddRectFilled(mouseStart, mouseEnd, 0x40FFFFFF);
			ImGui::End();

			focusedElements.clear();
			for (auto* scene : project->scenes) {
				auto frustum = ScreenRectToFrustum(mouseStart, mouseEnd, (scene->camera.projectionMatrix * scene->camera.viewMatrix).inverse());

				for (auto* layer : scene->layers)
					if (!(layer->flags & 0x100))
						for (auto* cast : layer->casts)
							if (cast->transform->display && frustum.Test((cast->transform->transformationMatrix * Eigen::Vector4f{ 0.0f, 0.0f, 0.0f, 1.0f }).hnormalized()))
								focusedElements.push_back({ Selection::Type::CAST, cast });
			}
		}
	}
}

void SurfRideEditor::HandleManipulation() {
	auto& io = ImGui::GetIO();

	if (dragSelecting)
		return;

	if (!io.WantCaptureMouse && !ImGui::IsAnyItemHovered())
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && IsMouseInSelectionAabb())
			manipulating = true;

	if (!manipulating)
		return;

	auto delta = io.MousePos - io.MousePosPrev;

	csl::ut::MoveArray<Cast*> castsToMove{ hh::fnd::MemoryRouter::GetTempAllocator() };
	for (auto& el : focusedElements) {
		if (el.type != Selection::Type::CAST)
			continue;

		auto* cast = el.cast;
		bool keepLooking{ true };
		while (keepLooking) {
			if (cast->parentCast != nullptr) {
				for (auto& el2 : focusedElements) {
					if (el2.type == Selection::Type::CAST && el2.cast == cast->parentCast) {
						keepLooking = false;
						break;
					}
				}

				cast = cast->parentCast;
			}
			else {
				castsToMove.push_back(el.cast);
				keepLooking = false;
			}
		}
	}

	for (auto* cast : castsToMove) {
		auto parentTransform = cast->parentCast == nullptr ? Eigen::Matrix4f::Identity() : Eigen::Matrix4f{ cast->parentCast->transform->transformationMatrix };
		
		Eigen::Matrix4f transform{ cast->layer->scene->camera.projectionMatrix * cast->layer->scene->camera.viewMatrix * parentTransform };
		Eigen::Vector3f positionInNDC{ (transform * cast->transform->position.homogeneous()).hnormalized() };
		Eigen::Vector3f ndcDelta{ 2.0f * delta.x / io.DisplaySize.x, 2.0f * -delta.y / io.DisplaySize.y, 0.0f };
		Eigen::Vector3f movedNDC{ positionInNDC + ndcDelta };
		Eigen::Vector3f movedLocal{ (transform.inverse() * movedNDC.homogeneous()).hnormalized() };

		cast->transform->position = movedLocal;
		cast->transform->dirtyFlag.flags.m_dummy |= cast->transform->dirtyFlag.transformAny.m_dummy;
	}
}

Cast* SurfRideEditor::RayCastCast(Cast* cast, const Ray3f& ray)
{
	if (!cast->transform->display)
		return nullptr;

	for (size_t i = cast->children.size(); i > 0; i--)
		if (auto* childPick = RayCastCast(cast->children[i - 1], ray))
			return childPick;

	if (Intersects(cast, ray))
		return cast;

	return nullptr;
}

bool SurfRideEditor::Intersects(const Cast* cast, const Ray3f& ray)
{
	switch (static_cast<SRS_CASTNODE::Type>(cast->flags & 0xF)) {
	case SRS_CASTNODE::Type::IMAGE:
		if (Intersects(*cast->transform, static_cast<const ImageCast*>(cast)->size, ray))
			return cast;
		break;
	case SRS_CASTNODE::Type::SLICE:
		if (Intersects(*cast->transform, static_cast<const SliceCast*>(cast)->size, ray))
			return cast;
		break;
	}
	return false;
}

bool SurfRideEditor::Intersects(const Transform& transform, const Vector2& size, const Ray3f& ray)
{
	auto halfSize = size / 2.0f;

	Eigen::Matrix4f invertedTransformMatrix{ transform.transformationMatrix.inverse() };
	Eigen::ParametrizedLine<float, 3> worldLine{ ray };
	Eigen::ParametrizedLine<float, 3> localLine{ invertedTransformMatrix * ray };
	Eigen::Hyperplane<float, 3> castPlane{ { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } };

	auto intersectionPoint = localLine.intersectionPoint(castPlane);
	auto botLeft = -halfSize;
	auto topRight = halfSize;

	return botLeft.x() <= intersectionPoint.x() && botLeft.y() <= intersectionPoint.y() && topRight.x() >= intersectionPoint.x() && topRight.y() >= intersectionPoint.y();
}

void SurfRideEditor::Select(const Selection& selection)
{
	Deselect();
	focusedElements.push_back(selection);
}

void SurfRideEditor::Deselect()
{
	focusedElements.clear();
}

void SurfRideEditor::UpdateSelectionAabb(const Eigen::Matrix4f& transform, const SurfRide::Vector2& size)
{
	selectionAabb.AddPoint({ (transform * Eigen::Vector4f{ -size.x() / 2.0f, size.y() / 2.0f, 0.0f, 1.0f }).hnormalized() });
	selectionAabb.AddPoint({ (transform * Eigen::Vector4f{ size.x() / 2.0f, size.y() / 2.0f, 0.0f, 1.0f }).hnormalized() });
	selectionAabb.AddPoint({ (transform * Eigen::Vector4f{ size.x() / 2.0f, -size.y() / 2.0f, 0.0f, 1.0f }).hnormalized() });
	selectionAabb.AddPoint({ (transform * Eigen::Vector4f{ -size.x() / 2.0f, -size.y() / 2.0f, 0.0f, 1.0f }).hnormalized() });
}

void SurfRideEditor::DrawSelectionAabb()
{
	if (haveSelectionAabb) {
		if (ImGui::Begin("Overlay")) {
			if (auto* dl = ImGui::GetCurrentWindow()->DrawList) {
				auto min = NDCCoordsToImGui({ selectionAabb.m_Min.x(), selectionAabb.m_Max.y() });
				auto max = NDCCoordsToImGui({ selectionAabb.m_Max.x(), selectionAabb.m_Min.y() });

				dl->AddRect({ min.x, max.y }, { max.x, min.y }, 0xFFFFFFFF);
			}
		}
		ImGui::End();
	}
}

bool SurfRideEditor::IsMouseInSelectionAabb()
{
	if (!haveSelectionAabb)
		return false;

	auto& io = ImGui::GetIO();
	auto mousePos = ImGui::GetMousePos();

	auto min = NDCCoordsToImGui({ selectionAabb.m_Min.x(), selectionAabb.m_Max.y() });
	auto max = NDCCoordsToImGui({ selectionAabb.m_Max.x(), selectionAabb.m_Min.y() });

	return min.x <= mousePos.x && min.y <= mousePos.y && max.x >= mousePos.x && max.y >= mousePos.y;
}

void SurfRideEditor::CalcSelectionAabb()
{
	haveSelectionAabb = false;
	selectionAabb.m_Min = { INFINITY, INFINITY, INFINITY };
	selectionAabb.m_Max = { -INFINITY, -INFINITY, -INFINITY };
	for (auto& el : focusedElements) {
		if (el.type != Selection::Type::CAST)
			continue;

		UpdateSelectionAabb(el.cast);
	}
}

void SurfRideEditor::UpdateSelectionAabb(const SurfRide::Cast* cast)
{
	for (auto* child : cast->children)
		UpdateSelectionAabb(child);

	switch (static_cast<SRS_CASTNODE::Type>(cast->flags & 0xF)) {
	case SRS_CASTNODE::Type::IMAGE: UpdateSelectionAabb(cast->layer->scene->camera.projectionMatrix * cast->layer->scene->camera.viewMatrix * cast->transform->transformationMatrix, static_cast<const ImageCast*>(cast)->size); haveSelectionAabb = true; break;
	case SRS_CASTNODE::Type::SLICE: UpdateSelectionAabb(cast->layer->scene->camera.projectionMatrix * cast->layer->scene->camera.viewMatrix * cast->transform->transformationMatrix, static_cast<const SliceCast*>(cast)->size); haveSelectionAabb = true; break;
	}
}

SurfRideEditor::Selection::Selection(Type type, Scene* scene) : type{ type }, scene{ scene } {}
SurfRideEditor::Selection::Selection(Type type, SRS_CAMERA* cameraData) : type{ type }, cameraData{ cameraData } {}
SurfRideEditor::Selection::Selection(Type type, Layer* layer) : type{ type }, layer{ layer } {}
SurfRideEditor::Selection::Selection(Type type, Cast* cast) : type{ type }, cast{ cast } {}

bool SurfRideEditor::Selection::operator==(const Selection& other) const
{
	if (type != other.type)
		return false;

	switch (type) {
	case Type::SCENE: return scene == other.scene;
	case Type::CAMERA_DATA: return cameraData == other.cameraData;
	case Type::LAYER: return layer == other.layer;
	case Type::CAST: return cast == other.cast;
	default: return true;
	}
}
