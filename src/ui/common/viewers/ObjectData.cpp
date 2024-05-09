#include "ObjectData.h"
#include "Basic.h"

void Viewer(const char* label, const hh::game::ObjectTransformData& obj)
{
	if (BeginVectorViewerTable(label, false)) {
		Viewer("Position", obj.position);
		Viewer("Rotation", obj.rotation);
		EndVectorViewerTable();
	}
}
