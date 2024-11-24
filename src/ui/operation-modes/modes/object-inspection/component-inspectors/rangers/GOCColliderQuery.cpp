#include <ui/operation-modes/modes/object-inspection/component-inspectors/GOCColliderQuery.h>
#include <ui/common/editors/Basic.h>

void RenderComponentInspector(app::physics::GOCColliderQuery& component)
{
	Editor("Local transform", component.localTransform);
	Editor("World transform", component.worldTransform);
}

void RenderComponentInspector(app::physics::GOCMoveSphereColliderQuery& component)
{
	RenderComponentInspector(static_cast<app::physics::GOCColliderQuery&>(component));

	Editor("unk201", component.unk201);
	Editor("unk201", component.unk202);
}
