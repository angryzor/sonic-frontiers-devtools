#pragma once

bool CalcApproxAabb(const csl::ut::MoveArray<hh::game::GameObject*>& objects, csl::geom::Aabb& aabb);
bool CalcApproxAabb(hh::game::ObjectWorldChunk* chunk, const csl::ut::MoveArray<hh::game::ObjectData*>& objects, csl::geom::Aabb& aabb);

void AddAabb(csl::geom::Aabb& aabb, const csl::geom::Aabb& other);
csl::geom::Aabb Union(const csl::geom::Aabb& one, const csl::geom::Aabb& other);

csl::geom::Aabb CalcAabb(const csl::geom::Sphere& sphere);
csl::geom::Aabb CalcAabb(const csl::geom::Cylinder& sphere);
csl::geom::Aabb CalcAabb(const csl::geom::Obb& sphere);
