#pragma once

hh::game::ObjectTransformData Affine3fToObjectTransformData(const Eigen::Affine3f& transform);
Eigen::Affine3f ObjectTransformDataToAffine3f(const hh::game::ObjectTransformData& transformData);
void RecalculateLocalTransform(const Eigen::Affine3f& parentTransform, hh::game::ObjectData& objData);
void RecalculateAbsoluteTransform(const Eigen::Affine3f& parentTransform, hh::game::ObjectData& objData);
void RecalculateLocalTransform(const hh::game::ObjectTransformData& parentTransform, hh::game::ObjectData& objData);
void RecalculateAbsoluteTransform(const hh::game::ObjectTransformData& parentTransform, hh::game::ObjectData& objData);
void RecalculateLocalTransform(const hh::game::ObjectData& parent, hh::game::ObjectData& objData);
void RecalculateAbsoluteTransform(const hh::game::ObjectData& parent, hh::game::ObjectData& objData);
void UpdateAbsoluteTransform(const Eigen::Affine3f& newAbsoluteTransform, hh::game::ObjectData& objData);
void UpdateLocalTransform(const Eigen::Affine3f& newLocalTransform, hh::game::ObjectData& objData);
void UpdateGOCTransform(hh::game::ObjectData& objData, hh::game::GOCTransform& gocTransform);
