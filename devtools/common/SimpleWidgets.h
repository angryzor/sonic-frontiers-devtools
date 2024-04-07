void MatrixValues(const Eigen::Matrix4f& mat);
void ViewportDataInfo(const hh::gfnd::ViewportData& viewport);
void WorldPositionEditor(hh::fnd::WorldPosition& worldPos);
void InputText(const char* label, csl::ut::VariableString* str, ImGuiInputTextFlags flags = 0);
void InputText(const char* label, csl::ut::String* str, ImGuiInputTextFlags flags = 0);
void InputObjectId(const char* label, hh::game::ObjectId* id);

template<typename T>
void CheckboxFlags(const char* label, csl::ut::Bitset<T>* v, T value) {
	ImGui::CheckboxFlags(label, reinterpret_cast<std::underlying_type_t<T>*>(&v->m_dummy), static_cast<std::underlying_type_t<T>>(1) << static_cast<std::underlying_type_t<T>>(value));
}

template<typename T, size_t count>
void ComboEnum(const char* label, T* v, const char* (&names)[count]) {
	int x = static_cast<std::underlying_type_t<T>>(*v);
	ImGui::Combo(label, &x, names, count);
	*v = static_cast<T>(x);
}
