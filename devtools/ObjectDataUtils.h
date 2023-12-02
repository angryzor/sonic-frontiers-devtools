#pragma once

hh::game::ObjectTransformData TransformToObjectTransformData(const Eigen::Affine3f& transform);
Eigen::Affine3f ObjectTransformDataToTransform(hh::game::ObjectTransformData& transformData);
