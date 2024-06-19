#include "Pch.h"
#include "ObjectDataUtils.h"
#include <utilities/math/MathUtils.h>

hh::game::ObjectTransformData Affine3fToObjectTransformData(const Eigen::Affine3f& transform) {
	Eigen::Matrix3f absoluteRotation;
	Eigen::Matrix3f absoluteScaling;

	transform.computeRotationScaling(&absoluteRotation, &absoluteScaling);

	return { { transform.translation() }, MatrixToEuler(absoluteRotation) };
}

Eigen::Affine3f ObjectTransformDataToAffine3f(const hh::game::ObjectTransformData& transformData) {
	Eigen::Affine3f transform;

	transform.fromPositionOrientationScale(transformData.position, EulerToQuat(transformData.rotation), csl::math::Vector3{ 1.0f, 1.0f, 1.0f });

	return transform;
}

void RecalculateLocalTransform(const Eigen::Affine3f& parentTransform, hh::game::ObjectData& objData) {
	auto absoluteTransform = ObjectTransformDataToAffine3f(objData.transform);

	objData.localTransform = Affine3fToObjectTransformData(parentTransform.inverse() * absoluteTransform);
}

void RecalculateAbsoluteTransform(const Eigen::Affine3f& parentTransform, hh::game::ObjectData& objData) {
	auto localTransform = ObjectTransformDataToAffine3f(objData.localTransform);

	objData.transform = Affine3fToObjectTransformData(parentTransform * localTransform);
}

void RecalculateLocalTransform(const hh::game::ObjectTransformData& parentTransform, hh::game::ObjectData& objData) {
	RecalculateLocalTransform(ObjectTransformDataToAffine3f(parentTransform), objData);
}

void RecalculateAbsoluteTransform(const hh::game::ObjectTransformData& parentTransform, hh::game::ObjectData& objData) {
	RecalculateAbsoluteTransform(ObjectTransformDataToAffine3f(parentTransform), objData);
}

void RecalculateLocalTransform(const hh::game::ObjectData& parent, hh::game::ObjectData& objData) {
	RecalculateLocalTransform(parent.transform, objData);
}

void RecalculateAbsoluteTransform(const hh::game::ObjectData& parent, hh::game::ObjectData& objData) {
	RecalculateAbsoluteTransform(parent.transform, objData);
}

void UpdateAbsoluteTransform(const Eigen::Affine3f& newAbsoluteTransform, hh::game::ObjectData& objData) {
	auto absoluteTransform = ObjectTransformDataToAffine3f(objData.transform);
	auto localTransform = ObjectTransformDataToAffine3f(objData.localTransform);
	auto parentTransform = absoluteTransform * localTransform.inverse();

	objData.transform = Affine3fToObjectTransformData(newAbsoluteTransform);
	objData.localTransform = Affine3fToObjectTransformData(parentTransform.inverse() * newAbsoluteTransform);
}

void UpdateLocalTransform(const Eigen::Affine3f& newLocalTransform, hh::game::ObjectData& objData) {
	auto absoluteTransform = ObjectTransformDataToAffine3f(objData.transform);
	auto localTransform = ObjectTransformDataToAffine3f(objData.localTransform);
	auto parentTransform = absoluteTransform * localTransform.inverse();

	objData.transform = Affine3fToObjectTransformData(parentTransform * newLocalTransform);
	objData.localTransform = Affine3fToObjectTransformData(newLocalTransform);
}

void UpdateGOCTransform(hh::game::ObjectData& objData, hh::game::GOCTransform& gocTransform) {
	// Depending on whether the parent was able to be spawned, the object uses the local or the absolute transform as the GOC transform, so we have to replicate that here.
	if (gocTransform.IsExistParent())
		gocTransform.SetLocalTransform({ objData.localTransform.position, EulerToQuat(objData.localTransform.rotation), csl::math::Vector3{ 1.0f, 1.0f, 1.0f } });
	else
		gocTransform.SetLocalTransform({ objData.transform.position, EulerToQuat(objData.transform.rotation), csl::math::Vector3{ 1.0f, 1.0f, 1.0f } });
}
