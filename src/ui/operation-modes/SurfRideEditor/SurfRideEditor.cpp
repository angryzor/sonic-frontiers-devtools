#include "SurfRideEditor.h"

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
}

SurfRideEditor::Selection::Selection(Type type, SurfRide::Scene* scene) : type{ type }, scene{ scene } {}
SurfRideEditor::Selection::Selection(Type type, SurfRide::SRS_CAMERA* cameraData) : type{ type }, cameraData{ cameraData } {}
SurfRideEditor::Selection::Selection(Type type, SurfRide::Layer* layer) : type{ type }, layer{ layer } {}
SurfRideEditor::Selection::Selection(Type type, SurfRide::Cast* cast) : type{ type }, cast{ cast } {}
