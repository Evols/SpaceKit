//
//  btReducedVector.cpp
//  LinearMath
//
//  Created by Xuchen Han on 4/4/20.
//
#include <stdio.h>
#include "btReducedVector.h"
#include <cmath>

// returns the projection of this onto other
btReducedVector btReducedVector::proj(const btReducedVector& other) const
{
    btReducedVector ret(m_sz);
    btScalar other_length2 = other.length2();
    if (other_length2 < SIMD_EPSILON)
    {
        return ret;
    }
    return other*(this->dot(other))/other_length2;
}

void btReducedVector::normalize()
{
    if (this->length2() < SIMD_EPSILON)
    {
        m_indices.clear();
        m_vecs.clear();
        return;
    }
    *this /= URealFloatMath::Sqrt(this->length2());
}

bool btReducedVector::testAdd() const
{
    int sz = 5;
    btAlignedObjectArray<int> id1;
    id1.push_back(1);
    id1.push_back(3);
    btAlignedObjectArray<btVector3> v1;
    v1.push_back(btVector3(1_fl,0_fl,1_fl));
    v1.push_back(btVector3(3_fl,1_fl,5_fl));
    btAlignedObjectArray<int> id2;
    id2.push_back(2);
    id2.push_back(3);
    id2.push_back(5);
    btAlignedObjectArray<btVector3> v2;
    v2.push_back(btVector3(2_fl,3_fl,1_fl));
    v2.push_back(btVector3(3_fl,4_fl,9_fl));
    v2.push_back(btVector3(0_fl,4_fl,0_fl));
    btAlignedObjectArray<int> id3;
    id3.push_back(1);
    id3.push_back(2);
    id3.push_back(3);
    id3.push_back(5);
    btAlignedObjectArray<btVector3> v3;
    v3.push_back(btVector3(1_fl,0_fl,1_fl));
    v3.push_back(btVector3(2_fl,3_fl,1_fl));
    v3.push_back(btVector3(6_fl,5_fl,14_fl));
    v3.push_back(btVector3(0_fl,4_fl,0_fl));
    btReducedVector rv1(sz, id1, v1);
    btReducedVector rv2(sz, id2, v2);
    btReducedVector ans(sz, id3, v3);
    bool ret = ((ans == rv1+rv2) && (ans == rv2+rv1));
    if (!ret)
        printf("btReducedVector testAdd failed\n");
    return ret;
}

bool btReducedVector::testMinus() const
{
    int sz = 5;
    btAlignedObjectArray<int> id1;
    id1.push_back(1);
    id1.push_back(3);
    btAlignedObjectArray<btVector3> v1;
    v1.push_back(btVector3(1_fl,0_fl,1_fl));
    v1.push_back(btVector3(3_fl,1_fl,5_fl));
    btAlignedObjectArray<int> id2;
    id2.push_back(2);
    id2.push_back(3);
    id2.push_back(5);
    btAlignedObjectArray<btVector3> v2;
    v2.push_back(btVector3(2_fl,3_fl,1_fl));
    v2.push_back(btVector3(3_fl,4_fl,9_fl));
    v2.push_back(btVector3(0_fl,4_fl,0_fl));
    btAlignedObjectArray<int> id3;
    id3.push_back(1);
    id3.push_back(2);
    id3.push_back(3);
    id3.push_back(5);
    btAlignedObjectArray<btVector3> v3;
    v3.push_back(btVector3(-1_fl,-0_fl,-1_fl));
    v3.push_back(btVector3(2_fl,3_fl,1_fl));
    v3.push_back(btVector3(0_fl,3_fl,4_fl));
    v3.push_back(btVector3(0_fl,4_fl,0_fl));
    btReducedVector rv1(sz, id1, v1);
    btReducedVector rv2(sz, id2, v2);
    btReducedVector ans(sz, id3, v3);
    bool ret = (ans == rv2-rv1);
    if (!ret)
        printf("btReducedVector testMinus failed\n");
    return ret;
}

bool btReducedVector::testDot() const
{
    int sz = 5;
    btAlignedObjectArray<int> id1;
    id1.push_back(1);
    id1.push_back(3);
    btAlignedObjectArray<btVector3> v1;
    v1.push_back(btVector3(1_fl,0_fl,1_fl));
    v1.push_back(btVector3(3_fl,1_fl,5_fl));
    btAlignedObjectArray<int> id2;
    id2.push_back(2);
    id2.push_back(3);
    id2.push_back(5);
    btAlignedObjectArray<btVector3> v2;
    v2.push_back(btVector3(2_fl,3_fl,1_fl));
    v2.push_back(btVector3(3_fl,4_fl,9_fl));
    v2.push_back(btVector3(0_fl,4_fl,0_fl));
    btReducedVector rv1(sz, id1, v1);
    btReducedVector rv2(sz, id2, v2);
    btScalar ans = 58_fl;
    bool ret = (ans == rv2.dot(rv1) && ans == rv1.dot(rv2));
    ans = 14_fl+16_fl+9_fl+16_fl+81_fl;
    ret &= (ans==rv2.dot(rv2));
    
    if (!ret)
        printf("btReducedVector testDot failed\n");
    return ret;
}

bool btReducedVector::testMultiply() const
{
    int sz = 5;
    btAlignedObjectArray<int> id1;
    id1.push_back(1);
    id1.push_back(3);
    btAlignedObjectArray<btVector3> v1;
    v1.push_back(btVector3(1_fl,0_fl,1_fl));
    v1.push_back(btVector3(3_fl,1_fl,5_fl));
    btScalar s = 2_fl;
    btReducedVector rv1(sz, id1, v1);
    btAlignedObjectArray<int> id2;
    id2.push_back(1);
    id2.push_back(3);
    btAlignedObjectArray<btVector3> v2;
    v2.push_back(btVector3(2_fl,0_fl,2_fl));
    v2.push_back(btVector3(6_fl,2_fl,10_fl));
    btReducedVector ans(sz, id2, v2);
    bool ret = (ans == rv1*s);
    if (!ret)
        printf("btReducedVector testMultiply failed\n");
    return ret;
}

void btReducedVector::test() const
{
    bool ans = testAdd() && testMinus() && testDot() && testMultiply();
    if (ans)
    {
        printf("All tests passed\n");
    }
    else
    {
        printf("Tests failed\n");
    }
}
