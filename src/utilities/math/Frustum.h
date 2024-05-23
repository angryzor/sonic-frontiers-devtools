#pragma once
#include "Ray.h"

struct FrustumRays {
	Ray3f topLeft;
	Ray3f topRight;
	Ray3f botRight;
	Ray3f botLeft;
};

struct Frustum {
	Eigen::Hyperplane<float, 3> planes[6];

	Frustum();
	Frustum(const Frustum& other);
	Frustum(Frustum&& other) noexcept;
	Frustum(const FrustumRays rays);
	Frustum(const Eigen::Hyperplane<float, 3> (&otherPlanes)[6]);
	Frustum(Eigen::Hyperplane<float, 3> (&&otherPlanes)[6]) noexcept;
	Frustum& operator=(const Frustum& other);
	Frustum& operator=(Frustum&& other) noexcept;
	Frustum& operator=(const FrustumRays& rays);
	Frustum& operator=(const Eigen::Hyperplane<float, 3> (&otherPlanes)[6]);
	Frustum& operator=(Eigen::Hyperplane<float, 3> (&&otherPlanes)[6]) noexcept;
	bool Test(const Eigen::Vector3f vec);
};
