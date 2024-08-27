#pragma once

namespace io::hson::templates {
	enum TemplateType {
		HSON = 0,
		RFL
	};

	void GenerateTemplate(const std::string& filename, TemplateType type);
}
