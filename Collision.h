#pragma once
#include"src/DataModel/Vector3.h"
#include<cmath>

// posA, posB   : それぞれの中心座標
// halfSizeA, halfSizeB : それぞれの半径(中心から面までの距離)
inline bool CheckAABBCollision(
    const Vector3& posA, const Vector3& halfSizeA,
    const Vector3& posB, const Vector3& halfSizeB) {

    return
        std::abs(posA.x - posB.x) <= (halfSizeA.x + halfSizeB.x) &&
        std::abs(posA.y - posB.y) <= (halfSizeA.y + halfSizeB.y) &&
        std::abs(posA.z - posB.z) <= (halfSizeA.z + halfSizeB.z);
}