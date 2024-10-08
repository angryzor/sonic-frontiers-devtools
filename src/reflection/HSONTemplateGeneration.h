#pragma once

namespace io::hson::templates {
	enum class TemplateType{
		HSON,
		RFL
	};

	void GenerateTemplate(const std::string& filename, TemplateType type);
}
