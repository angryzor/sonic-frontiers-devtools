#include "Frustum.h"
#include "MathUtils.h"

Frustum::Frustum() : planes{}
{
}

Frustum::Frustum(const Frustum& other) : Frustum{ other.planes }
{
}

Frustum::Frustum(Frustum&& other) noexcept : Frustum{ std::move(other.planes) }
{
}

Frustum::Frustum(const Eigen::Hyperplane<float, 3>(&otherPlanes)[6])
{
	*this = otherPlanes;
}

Frustum::Frustum(Eigen::Hyperplane<float, 3> (&&otherPlanes)[6]) noexcept
{
	*this = std::move(otherPlanes);
}

Frustum& Frustum::operator=(const Frustum& other)
{
	return *this = other.planes;
}

Frustum& Frustum::operator=(Frustum&& other) noexcept
{
	return *this = std::move(other.planes);
}

Frustum& Frustum::operator=(const Eigen::Hyperplane<float, 3> (&otherPlanes)[6])
{
	for (size_t i = 0; i < 6; i++)
		planes[i] = otherPlanes[i];

	return *this;
}

Frustum& Frustum::operator=(Eigen::Hyperplane<float, 3> (&&otherPlanes)[6]) noexcept
{
	for (size_t i = 0; i < 6; i++)
		planes[i] = std::move(otherPlanes[i]);

	return *this;
}

bool Frustum::Test(const Eigen::Vector3f vec)
{
	for (size_t i = 0; i < 6; i++)
		if (planes[i].signedDistance(vec) < 0)
			return false;

	return true;
}
