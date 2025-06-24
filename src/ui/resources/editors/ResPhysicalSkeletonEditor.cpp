#include "ResPhysicalSkeletonEditor.h"
#include <ui/common/viewers/Basic.h>
#include <ui/GlobalSettings.h>
#include <ucsl-reflection/reflections/resources/pba/v1.h>
#include <rip/binary/containers/binary-file/v2.h>

ResPhysicalSkeletonEditor::ResPhysicalSkeletonEditor(csl::fnd::IAllocator* allocator, hh::pba::ResPhysicalSkeleton* resource, hh::pba::GOCPhysicalAnimation* goc) : StandaloneWindow{ allocator }, resource{ resource }
{
	char namebuf[500];
	snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx)", resource->GetName(), resource->GetClass().pName, (size_t)&resource, (size_t)resource->unpackedBinaryData);
	SetTitle(namebuf);
	binaryData = static_cast<ucsl::resources::pba::v1::PbaData*>(resource->unpackedBinaryData);
	if (goc)
		gocPhys = goc;
}

ResPhysicalSkeletonEditor* ResPhysicalSkeletonEditor::Create(csl::fnd::IAllocator* allocator, hh::pba::ResPhysicalSkeleton* resource, hh::pba::GOCPhysicalAnimation* goc) {
	return new (allocator) ResPhysicalSkeletonEditor(allocator, resource, goc);
}

const char* limitModeNames[] = {
	"None",
	"Enabled",
	"Disabled"
};

bool Editor(ucsl::resources::pba::v1::Limit& limit) {
	bool changed = false;
	changed |= ComboEnum("Limit Mode", limit.mode, limitModeNames);
	changed |= Editor("Low Limit", limit.lowLimit);
	changed |= Editor("High Limit", limit.highLimit);
	if (ImGui::TreeNode("Spring Settings")) {
		changed |= Editor("Spring Enabled", limit.springEnabled);
		changed |= Editor("Spring Stiffness", limit.springStiffness);
		changed |= Editor("Spring Damping", limit.springDamping);
		ImGui::TreePop();
	}
	return changed;
}

void ResPhysicalSkeletonEditor::RenderContents()
{
	bool clickedExport = false;

	if (ImGui::BeginMenuBar()) {
		ImGui::MenuItem("Export", nullptr, &clickedExport);
		ImGui::EndMenuBar();
	}

	if (clickedExport) {
		IGFD::FileDialogConfig cfg{};
		cfg.path = GlobalSettings::defaultFileDialogDirectory;
		cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
		cfg.userDatas = binaryData;
		ImGuiFileDialog::Instance()->OpenDialog("ResPhysicalSkeletonExportDialog", "Choose File", ".pba", cfg);
	}
	if (ImGuiFileDialog::Instance()->Display("ResPhysicalSkeletonExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			auto* exportData = static_cast<ucsl::resources::pba::v1::PbaData*>(ImGuiFileDialog::Instance()->GetUserDatas());

			std::ofstream ofs{ ImGuiFileDialog::Instance()->GetFilePathName(), std::ios::trunc | std::ios::binary };
			rip::binary::containers::binary_file::v2::BinaryFileSerializer<size_t> serializer{ ofs };
			serializer.serialize<he2sdk::ucsl::GameInterface>(*exportData);
		}
		ImGuiFileDialog::Instance()->Close();
	}

	bool changed = false;
	changed |= InputText("Skeleton Name", binaryData->skeletonName, resource);
	if (ImGui::TreeNode("Rigid Bodies")) {
		for (auto i = 0; i < binaryData->rigidBodyCount; i++) {
			auto& rigidBody = binaryData->rigidBodies[i];
			ImGui::PushID(&rigidBody);
			if (ImGui::TreeNode("%s###", rigidBody.boneName)) {
				changed |= InputText("Bone Name", rigidBody.boneName, resource);
				if (ImGui::TreeNode("Shape Settings")) {
					changed |= Editor("Is Box", rigidBody.isShapeBox);
					changed |= Editor("Radius", rigidBody.shapeRadius);
					changed |= Editor("Height", rigidBody.shapeHeight);
					changed |= Editor("Depth", rigidBody.shapeDepth);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Physics Settings")) {
					changed |= Editor("Is Static Object", rigidBody.isStaticObject);
					changed |= Editor("Mass", rigidBody.mass);
					changed |= Editor("Friction", rigidBody.friction);
					changed |= Editor("Restitution", rigidBody.restitution);
					changed |= Editor("Linear Damping", rigidBody.linearDamping);
					changed |= Editor("Angular Damping", rigidBody.angularDamping);
					ImGui::TreePop();
				}
				changed |= Editor("Offset Position", rigidBody.offsetPosition);
				changed |= Editor("Offset Rotation", rigidBody.offsetRotation);
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Constraints")) {
		for (auto i = 0; i < binaryData->constraintCount; i++) {
			auto& constraint = binaryData->constraints[i];
			ImGui::PushID(&constraint);
			if (ImGui::TreeNode("%s###", constraint.boneName)) {
				changed |= InputText("Bone Name", constraint.boneName, resource);
				changed |= DragScalar("Unk0", constraint.unk0);
				changed |= DragScalar("Unk1", constraint.unk1);
				changed |= Editor("Iteration Count", constraint.iterationCount);
				changed |= Editor("Local Parent Bone Index", constraint.localParentBoneIndex);
				changed |= Editor("Local Bone Index", constraint.localBoneIndex);
				changed |= Editor("Skeleton Parent Bone Index", constraint.skeletonParentBoneIndex);
				if (ImGui::TreeNode("Linear Limits")) {
					for (auto x = 0; x < 3; x++) {
						auto& limit = constraint.linearLimits[x];
						changed |= Editor(limit);
						ImGui::Separator();
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Angular Limits")) {
					for (auto x = 0; x < 3; x++) {
						auto& limit = constraint.angularLimits[x];
						changed |= Editor(limit);
						ImGui::Separator();
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Point A")) {
					changed |= Editor("Offset Position", constraint.offsetPositionA);
					changed |= Editor("Offset Rotation", constraint.offsetRotationA);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Point B")) {
					changed |= Editor("Offset Position", constraint.offsetPositionB);
					changed |= Editor("Offset Rotation", constraint.offsetRotationB);
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Soft Bodies")) {
		for (auto i = 0; i < binaryData->softBodyCount; i++) {
			auto& softBody = binaryData->softBodies[i];
			ImGui::PushID(&softBody);
			if (ImGui::TreeNode("%s###", softBody.name)) {
				changed |= InputText("Name", softBody.name, resource);
				changed |= Editor("Scale", softBody.scale);
				if (ImGui::TreeNode("Physics Settings")) {
					changed |= Editor("Damping Coefficient", softBody.dampingCoeff);
					changed |= Editor("Drag Coefficient", softBody.dragCoeff);
					changed |= Editor("Lift Coefficient", softBody.liftCoeff);
					changed |= Editor("Dynamic Friction Coefficient", softBody.dynamicFrictionCoeff);
					changed |= Editor("Pose Matching Coefficient", softBody.poseMatchingCoeff);
					changed |= Editor("Rigid Contact Coefficient", softBody.rigidContactCoeff);
					changed |= Editor("Kinetic Contacts Hardness", softBody.kineticContactsHardness);
					changed |= Editor("Soft Contacts Hardness", softBody.softContactsHardness);
					changed |= Editor("Anchors Hardness", softBody.anchorsHardness);
					changed |= DragScalar("Position Iteration Count", softBody.positionIterationCount);
					ImGui::TreePop();
				}
				changed |= DragScalar("Unk0", softBody.unk0);
				changed |= Editor("Unk1", softBody.unk1);
				if (ImGui::TreeNode("Nodes")) {
					for (auto x = 0; x < softBody.nodeCount; x++) {
						auto& node = softBody.nodes[x];
						ImGui::PushID(&node);
						if (ImGui::TreeNode("%s###", node.boneName)) {
							changed |= Editor("Mass", node.mass);
							changed |= Editor("Unk0", node.unk0);
							changed |= Editor("Is Pinned", node.isPinned);
							changed |= Editor("Child ID", node.childId);
							changed |= Editor("Parent ID", node.parentId);
							changed |= Editor("Unk1", node.unk1);
							changed |= Editor("Unk2", node.unk2);
							changed |= Editor("Sibling Left ID", node.siblingLeftId);
							changed |= Editor("Sibling Right ID", node.siblingRightId);
							ImGui::TreePop();
						}
						ImGui::PopID();
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Links")) {
					for (auto x = 0; x < softBody.linkCount; x++) {
						auto& link = softBody.links[x];
						ImGui::PushID(&link);
						char buffer[32];
						snprintf(buffer, sizeof(buffer), "%d###", x);
						if (ImGui::TreeNode(buffer)) {
							changed |= Editor("Vertices", link.verts);
							changed |= Editor("Rest Length", link.restLength);
							changed |= Editor("Stiffness", link.stiffness);
							ImGui::TreePop();
						}
						ImGui::PopID();
					}
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		ImGui::TreePop();
	}
	if (changed) {
		resource->Reload(binaryData, resource->GetSize());
		if (gocPhys) {
			gocPhys->UnkFunc17();
			gocPhys->UnkFunc16(gocPhys->gocAnimator0);
		}
	}
}
