#include "Needle.h"

bool InputText(const char* label, hh::needle::intrusive_ptr<hh::needle::CNameIDObject>& str, ImGuiInputTextFlags flags)
{
	char name[256];

	strcpy_s(name, str->name);

	bool edited = ImGui::InputText(label, name, sizeof(name));

	if (edited)
		str = hh::needle::CNameIDObject::RegisterUniqueObject(name, nullptr);

	return edited;
}
