#pragma once

hh::game::ObjectTransformData Affine3fToObjectTransformData(const Eigen::Affine3f& transform);
Eigen::Affine3f ObjectTransformDataToAffine3f(hh::game::ObjectTransformData& transformData);
void UpdateAbsoluteTransform(const Eigen::Affine3f& newAbsoluteTransform, hh::game::ObjectData& objData);
void UpdateLocalTransform(const Eigen::Affine3f& newLocalTransform, hh::game::ObjectData& objData);
void UpdateGOCTransform(hh::game::ObjectData& objData, hh::game::GOCTransform& gocTransform);
