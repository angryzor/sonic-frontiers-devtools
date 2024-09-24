#include "FxCol.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>

using namespace app::gfx;

const char* collisionShapeShapeNames[] = { "Sphere", "Cylinder", "Anisotropic OBB", "Isotropic OBB" };
const char* collisionShapeTypeNames[] = { "Scene parameter index" };

bool Editor(const char* label, FxColCollisionShape::SphereExtents& extents) {
	bool edited{};
	
	ImGui::PushID(label);
	edited |= Editor("Radius", extents.radius);
	ImGui::PopID();

	return edited;
}

bool Editor(const char* label, FxColCollisionShape::CylinderExtents& extents) {
	bool edited{};

	ImGui::PushID(label);
	edited |= Editor("Radius", extents.radius);
	edited |= Editor("Half height", extents.halfHeight);
	edited |= Editor("Border thickness", extents.borderThickness);
	ImGui::PopID();

	return edited;
}

bool Editor(const char* label, FxColCollisionShape::AnisotropicObbExtents& extents) {
	bool edited{};

	ImGui::PushID(label);
	edited |= Editor("Depth", extents.depth);
	edited |= Editor("Width", extents.width);
	edited |= Editor("Height", extents.height);
	edited |= Editor("Maybe width and height border thickness (unused)", extents.maybeWidthAndHeightBorderThickness);
	edited |= Editor("Positive depth border thickness", extents.positiveDepthBorderThickness);
	edited |= Editor("Negative depth border thickness", extents.negativeDepthBorderThickness);
	ImGui::PopID();

	return edited;
}

bool Editor(const char* label, FxColCollisionShape::IsotropicObbExtents& extents) {
	bool edited{};

	ImGui::PushID(label);
	edited |= Editor("Depth", extents.depth);
	edited |= Editor("Width", extents.width);
	edited |= Editor("Height", extents.height);
	edited |= Editor("Border thickness", extents.borderThickness);
	ImGui::PopID();

	return edited;
}

bool Editor(const char* label, FxColCollisionShape::Shape shape, FxColCollisionShape::Extents& extents) {
	switch (shape) {
	case FxColCollisionShape::Shape::SPHERE: return Editor(label, extents.sphere);
	case FxColCollisionShape::Shape::CYLINDER: return Editor(label, extents.cylinder);
	case FxColCollisionShape::Shape::ANISOTROPIC_OBB: return Editor(label, extents.anisotropicObb);
	case FxColCollisionShape::Shape::ISOTROPIC_OBB: return Editor(label, extents.isotropicObb);
	default: assert(false);  return false;
	}
}

bool Editor(const char* label, FxColCollisionShape::SceneParameterIndexParameters& parameters) {
	bool edited{};

	ImGui::PushID(label);
	edited |= Editor("Scene parameter index", parameters.sceneParameterIndex);
	edited |= Editor("Interpolation Time", parameters.interpolationTime);
	ImGui::PopID();

	return edited;
}

bool Editor(const char* label, FxColCollisionShape::LightParameterIndexParameters& parameters) {
	bool edited{};

	ImGui::PushID(label);
	edited |= Editor("Light parameter index", parameters.lightParameterIndex);
	ImGui::PopID();

	return edited;
}

bool Editor(const char* label, FxColCollisionShape::CameraParameters& parameters) {
    bool edited{};

    ImGui::PushID(label);
    edited |= Editor("Unknown 1", parameters.unk1);
    edited |= Editor("Unknown 2", parameters.unk2);
    edited |= Editor("Bloom Scale", parameters.bloomScale);
    edited |= Editor("Auto Exposure Middle Gray", parameters.autoExposureMiddleGray);
    edited |= Editor("Unknown 3", parameters.unk3);
    edited |= Editor("Luminance", parameters.luminance);
    edited |= Editor("Auto Exposure Adapted Ratio", parameters.autoExposureAdaptedRatio);
    edited |= Editor("Interpolation Time", parameters.interpolationTime);
    ImGui::PopID();

    return edited;
}

bool Editor(const char* label, FxColCollisionShape::HeatHazeParameters& parameters) {
    bool edited{};

    ImGui::PushID(label);
    edited |= Editor("Heat Haze Max Height", parameters.heatHazeMaxHeight);
    edited |= Editor("Interpolation Time", parameters.interpolationTime);
    ImGui::PopID();

    return edited;
}

bool Editor(const char* label, FxColCollisionShape::Type type, FxColCollisionShape::Parameters& parameters) {
	switch (type) {
	case FxColCollisionShape::Type::SCENE_PARAMETER_INDEX: return Editor(label, parameters.sceneParameterIndex.sceneParameterIndex);
	case FxColCollisionShape::Type::LIGHT_PARAMETER_INDEX: return Editor(label, parameters.lightParameterIndex.lightParameterIndex);
	case FxColCollisionShape::Type::CAMERA: return Editor(label, parameters.camera);
	case FxColCollisionShape::Type::HEAT_HAZE: return Editor(label, parameters.heatHaze);
	default: assert(false); return false;
	}
}

bool Editor(const char* label, FxColCollisionShape& shape, ResFxColFile2* resource) {
	bool edited{};

	if (ImGui::TreeNode(label)) {
		if (resource == nullptr)
			Viewer("Name", shape.name);
		else
			edited |= InputText("Name", shape.name, resource);

		ImGui::SeparatorText("Collision shape");
		edited |= Editor("Position", shape.position);
		edited |= Editor("Rotation", shape.rotation);
		edited |= ComboEnum("Shape", shape.shape, collisionShapeShapeNames);
		edited |= Editor("Extents", shape.shape, shape.extents);

		ImGui::SeparatorText("Parameters");
		edited |= Editor("Priority", shape.priority);
		edited |= Editor("Unk1", shape.unk1);
		if (resource == nullptr)
			Viewer("Name", shape.unk2);
		else
			edited |= InputText("Name", shape.unk2, resource);

		edited |= ComboEnum("Type", shape.type, collisionShapeTypeNames);
		edited |= Editor("Parameters", shape.type, shape.parameters);
		ImGui::TreePop();
	}

	return edited;
}
