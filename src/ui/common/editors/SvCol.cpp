#include "SvCol.h"
#include <resources/ManagedMemoryRegistry.h>

bool Editor(const char* label, ucsl::resources::svcol::v1::SectorFilterData& filter);

#include <ui/common/inputs/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>

using namespace ucsl::resources::svcol::v1;

bool Editor(const char* label, ucsl::resources::svcol::v1::SectorFilterData& filter) {
	bool edited{};
	edited |= Editor("Enabled", filter.enabled);
	edited |= Editor("Sector ID", filter.sectorId);
	return edited;
}

bool Editor(const char* label, ShapeData& shape, app::ResSvCol2* resource) {
	bool edited{};
	
	unsigned char minSector{ 0 };
	unsigned char maxSector{ 127 };

	edited |= InputText("Name", shape.name, resource);
	edited |= Editor("Priority", shape.priority);
	edited |= Editor("Position", shape.position);
	edited |= Editor("Rotation", shape.rotation);
	edited |= Editor("Depth", shape.depth);
	edited |= Editor("Width", shape.width);
	edited |= Editor("Height", shape.height);
	edited |= DragScalar("Sky sector ID", shape.skySectorId, &minSector, &maxSector);

	resources::ManagedCArray<SectorFilterData, unsigned int> groundSectorFilters{ resource, shape.groundSectorFilters, shape.groundSectorFilterCount };

	edited |= Editor("Ground sector filters", groundSectorFilters);

	return edited;
}
