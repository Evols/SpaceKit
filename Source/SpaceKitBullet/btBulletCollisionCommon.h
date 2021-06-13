/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef BULLET_COLLISION_COMMON_H
#define BULLET_COLLISION_COMMON_H

///Common headerfile includes for Bullet Collision Detection

///Bullet's btCollisionWorld and btCollisionObject definitions
#include "SpaceKitBullet/BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "SpaceKitBullet/BulletCollision/CollisionDispatch/btCollisionObject.h"

///Collision Shapes
#include "SpaceKitBullet/BulletCollision/CollisionShapes/btBoxShape.h"
#include "SpaceKitBullet/BulletCollision/CollisionShapes/btSphereShape.h"
#include "SpaceKitBullet/BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "SpaceKitBullet/BulletCollision/CollisionShapes/btCylinderShape.h"
#include "SpaceKitBullet/BulletCollision/CollisionShapes/btConeShape.h"
#include "SpaceKitBullet/BulletCollision/CollisionShapes/btStaticPlaneShape.h"
#include "SpaceKitBullet/BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "SpaceKitBullet/BulletCollision/CollisionShapes/btTriangleMesh.h"
#include "SpaceKitBullet/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h"
#include "SpaceKitBullet/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "SpaceKitBullet/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h"
#include "SpaceKitBullet/BulletCollision/CollisionShapes/btTriangleMeshShape.h"
#include "SpaceKitBullet/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h"
#include "SpaceKitBullet/BulletCollision/CollisionShapes/btCompoundShape.h"
#include "SpaceKitBullet/BulletCollision/CollisionShapes/btTetrahedronShape.h"
#include "SpaceKitBullet/BulletCollision/CollisionShapes/btEmptyShape.h"
#include "SpaceKitBullet/BulletCollision/CollisionShapes/btMultiSphereShape.h"
#include "SpaceKitBullet/BulletCollision/CollisionShapes/btUniformScalingShape.h"

///Narrowphase Collision Detector
#include "SpaceKitBullet/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h"

//#include "SpaceKitBullet/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.h"
#include "SpaceKitBullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"

///Dispatching and generation of collision pairs (broadphase)
#include "SpaceKitBullet/BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "SpaceKitBullet/BulletCollision/BroadphaseCollision/btSimpleBroadphase.h"
#include "SpaceKitBullet/BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#include "SpaceKitBullet/BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"

///Math library & Utils
#include "SpaceKitBullet/LinearMath/btQuaternion.h"
#include "SpaceKitBullet/LinearMath/btTransform.h"
#include "SpaceKitBullet/LinearMath/btDefaultMotionState.h"
#include "SpaceKitBullet/LinearMath/btQuickprof.h"
#include "SpaceKitBullet/LinearMath/btIDebugDraw.h"
#include "SpaceKitBullet/LinearMath/btSerializer.h"

#endif  //BULLET_COLLISION_COMMON_H
