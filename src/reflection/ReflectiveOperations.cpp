#include "ReflectiveOperations.h"

//using namespace hh::fnd;
//
//void* ReflectiveOperations::InitializeObject(csl::fnd::IAllocator* allocator, const hh::fnd::RflClass* rflClass) {
//}
//
//void ReflectiveOperations::InitializeMember(void* obj, csl::fnd::IAllocator* allocator, hh::fnd::RflClassMember& member) {
//	switch (member.type) {
//	case RflClassMember::TYPE_POINTER:
//		allocator->Alloc(member.GetSubTypeSizeInBytes(), member.GetAlignment());
//		break;
//	case RflClassMember::TYPE_ARRAY:
//		RenderStubRflParamEditor(name, member);
//		break;
//	case RflClassMember::TYPE_SIMPLE_ARRAY:
//		RenderStubRflParamEditor(name, member);
//		break;
//	case RflClassMember::TYPE_ENUM:
//		RenderEnumEditor(name, obj, member);
//		break;
//	case RflClassMember::TYPE_STRUCT:
//		RenderStructRflParamEditor(name, obj, member->m_pClass);
//		break;
//	case RflClassMember::TYPE_FLAGS:
//		RenderFlagsEditor(name, obj, member);
//		break;
//	case RflClassMember::TYPE_CSTRING:
//		ImGui::Text("%s: %s", name, static_cast<const char*>(obj));
//		break;
//	case RflClassMember::TYPE_STRING:
//		InputText(name, static_cast<csl::ut::VariableString*>(obj));
//		break;
//	default:
//		InitializePrimitiveType(obj, member.type);
//		break;
//	}
//}
//void ReflectiveOperations::InitializePrimitiveType(void* obj, hh::fnd::RflClassMember::Type type) {
//	switch (type) {
//	case RflClassMember::TYPE_BOOL:
//		*static_cast<bool*>(obj) = false;
//		break;
//	case RflClassMember::TYPE_SINT8:
//		*static_cast<int8_t*>(obj) = 0;
//		break;
//	case RflClassMember::TYPE_UINT8:
//		*static_cast<uint8_t*>(obj) = 0;
//		break;
//	case RflClassMember::TYPE_SINT16:
//		*static_cast<int16_t*>(obj) = 0;
//		break;
//	case RflClassMember::TYPE_UINT16:
//		*static_cast<uint16_t*>(obj) = 0;
//		break;
//	case RflClassMember::TYPE_SINT32:
//		*static_cast<int32_t*>(obj) = 0;
//		break;
//	case RflClassMember::TYPE_UINT32:
//		*static_cast<uint32_t*>(obj) = 0;
//		break;
//	case RflClassMember::TYPE_SINT64:
//		*static_cast<int64_t*>(obj) = 0;
//		break;
//	case RflClassMember::TYPE_UINT64:
//		*static_cast<uint64_t*>(obj) = 0;
//		break;
//	case RflClassMember::TYPE_FLOAT:
//		*static_cast<float*>(obj) = 0.0f;
//		break;
//	case RflClassMember::TYPE_VECTOR2:
//		*static_cast<csl::math::Vector2*>(obj) = csl::math::Vector2{};
//		break;
//	case RflClassMember::TYPE_VECTOR3:
//		*static_cast<csl::math::Vector3*>(obj) = csl::math::Vector3{};
//		break;
//	case RflClassMember::TYPE_VECTOR4:
//		*static_cast<csl::math::Vector4*>(obj) = csl::math::Vector4{};
//		break;
//	case RflClassMember::TYPE_QUATERNION:
//		*static_cast<csl::math::Quaternion*>(obj) = csl::math::Quaternion{};
//		break;
//	case RflClassMember::TYPE_MATRIX34:
//		*static_cast<csl::math::Matrix34*>(obj) = csl::math::Matrix34{};
//		break;
//	case RflClassMember::TYPE_MATRIX44:
//		*static_cast<csl::math::Matrix44*>(obj) = csl::math::Matrix44{};
//		break;
//	case RflClassMember::TYPE_OBJECT_ID:
//		*static_cast<hh::game::ObjectId*>(obj) = hh::game::ObjectId{};
//		break;
//	case RflClassMember::TYPE_POSITION:
//		*static_cast<csl::math::Position*>(obj) = csl::math::Position{};
//		break;
//	case RflClassMember::TYPE_COLOR_BYTE:
//		*static_cast<csl::ut::Color<uint8_t>*>(obj) = csl::ut::Color<uint8_t>{};
//		break;
//	case RflClassMember::TYPE_COLOR_FLOAT:
//		*static_cast<csl::ut::Color<float>*>(obj) = csl::ut::Color<float>{};
//		break;
//	default:
//		assert(!"Unknown primitive type");
//		break;
//	}
//}
//		case RflClassMember::TYPE_OBJECT_ID:
//			RenderStubRflParamEditor(name, member);
//			break;
//		case RflClassMember::TYPE_POSITION:
//			RenderSliderlessScalarRflParamEditor<csl::math::Vector3>(name, obj, member, ImGuiDataType_Float, 3, "RangeFloat");
//			break;
//		case RflClassMember::TYPE_COLOR_BYTE: {
//			auto originalColor{ static_cast<csl::ut::Color<uint8_t>*>(obj) };
//			float colorAsFloat[]{
//				static_cast<float>(originalColor->r) / 255,
//				static_cast<float>(originalColor->g) / 255,
//				static_cast<float>(originalColor->b) / 255,
//				static_cast<float>(originalColor->a) / 255,
//			};
//			float editableColor[4]{ colorAsFloat[0], colorAsFloat[1], colorAsFloat[2], colorAsFloat[3] };
//
//			ImGui::ColorEdit4(name, editableColor, ImGuiColorEditFlags_Uint8);
//
//			if (colorAsFloat[0] != editableColor[0] || colorAsFloat[1] != editableColor[1] || colorAsFloat[2] != editableColor[2] || colorAsFloat[3] != editableColor[3]) {
//				originalColor->r = static_cast<uint8_t>(editableColor[0] * 255);
//				originalColor->g = static_cast<uint8_t>(editableColor[1] * 255);
//				originalColor->b = static_cast<uint8_t>(editableColor[2] * 255);
//				originalColor->a = static_cast<uint8_t>(editableColor[3] * 255);
//			}
//
//			break;
//		}
//		case RflClassMember::TYPE_COLOR_FLOAT:
//			ImGui::ColorEdit4(name, static_cast<float*>(obj), ImGuiColorEditFlags_Float);
//			break;
//		}
//	}
//
//}