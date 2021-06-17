/*
Copyright (c) 2003-2013 Gino van den Bergen / Erwin Coumans  http://bulletphysics.org

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef B3_VECTOR3_H
#define B3_VECTOR3_H

//#include <stdint.h>
#include "b3Scalar.h"
#include "b3MinMax.h"
#include "b3AlignedAllocator.h"
#include "SpaceKitBullet/LinearMath/btScalar.h"

#ifdef B3_USE_DOUBLE_PRECISION
#define b3Vector3Data b3Vector3DoubleData
#define b3Vector3DataName "b3Vector3DoubleData"
#else
#define b3Vector3Data b3Vector3FloatData
#define b3Vector3DataName "b3Vector3FloatData"
#endif  //B3_USE_DOUBLE_PRECISION

#if defined B3_USE_SSE

//typedef  uint32_t __m128i __attribute__ ((vector_size(16)));

#ifdef _MSC_VER
#pragma warning(disable : 4556)  // value of intrinsic immediate argument '4294967239' is out of range '0 - 255'
#endif

#define B3_SHUFFLE(x, y, z, w) (((w) << 6 | (z) << 4 | (y) << 2 | (x)) & 0xff)
//#define b3_pshufd_ps( _a, _mask ) (__m128) _mm_shuffle_epi32((__m128i)(_a), (_mask) )
#define b3_pshufd_ps(_a, _mask) _mm_shuffle_ps((_a), (_a), (_mask))
#define b3_splat3_ps(_a, _i) b3_pshufd_ps((_a), B3_SHUFFLE(_i, _i, _i, 3))
#define b3_splat_ps(_a, _i) b3_pshufd_ps((_a), B3_SHUFFLE(_i, _i, _i, _i))

#define b3v3AbsiMask (_mm_set_epi32(0x00000000, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF))
#define b3vAbsMask (_mm_set_epi32(0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF))
#define b3vFFF0Mask (_mm_set_epi32(0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF))
#define b3v3AbsfMask b3CastiTo128f(b3v3AbsiMask)
#define b3vFFF0fMask b3CastiTo128f(b3vFFF0Mask)
#define b3vxyzMaskf b3vFFF0fMask
#define b3vAbsfMask b3CastiTo128f(b3vAbsMask)

const __m128 B3_ATTRIBUTE_ALIGNED16(b3vMzeroMask) = {-0.0_fl, -0.0_fl, -0.0_fl, -0.0_fl};
const __m128 B3_ATTRIBUTE_ALIGNED16(b3v1110) = {1.0_fl, 1.0_fl, 1.0_fl, 0.0_fl};
const __m128 B3_ATTRIBUTE_ALIGNED16(b3vHalf) = {0.5_fl, 0.5_fl, 0.5_fl, 0.5_fl};
const __m128 B3_ATTRIBUTE_ALIGNED16(b3v1_5) = {1.5_fl, 1.5_fl, 1.5_fl, 1.5_fl};

#endif

#ifdef B3_USE_NEON

const float32x4_t B3_ATTRIBUTE_ALIGNED16(b3vMzeroMask) = (float32x4_t){-0.0_fl, -0.0_fl, -0.0_fl, -0.0_fl};
const int32x4_t B3_ATTRIBUTE_ALIGNED16(b3vFFF0Mask) = (int32x4_t){0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0};
const int32x4_t B3_ATTRIBUTE_ALIGNED16(b3vAbsMask) = (int32x4_t){0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF};
const int32x4_t B3_ATTRIBUTE_ALIGNED16(b3v3AbsMask) = (int32x4_t){0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x0};

#endif

class b3Vector3;
class b3Vector4;

#if defined(B3_USE_SSE_IN_API) && defined(B3_USE_SSE)
//#if defined (B3_USE_SSE) || defined (B3_USE_NEON)
inline b3Vector3 b3MakeVector3(b3SimdFloat4 v);
inline b3Vector4 b3MakeVector4(b3SimdFloat4 vec);
#endif

inline b3Vector3 b3MakeVector3(b3Scalar x, b3Scalar y, b3Scalar z);
inline b3Vector3 b3MakeVector3(b3Scalar x, b3Scalar y, b3Scalar z, b3Scalar w);
inline b3Vector4 b3MakeVector4(b3Scalar x, b3Scalar y, b3Scalar z, b3Scalar w);

/**@brief b3Vector3 can be used to represent 3D points and vectors.
 * It has an un-used w component to suit 16-byte alignment when b3Vector3 is stored in containers. This extra component can be used by derived classes (Quaternion?) or by user
 * Ideally, this class should be replaced by a platform optimized SIMD version that keeps the data in registers
 */
class b3Vector3
{
public:
	union {
		btScalar m_floats[4];
		struct
		{
			btScalar x, y, z, w;
		};
	};

public:

	b3Vector3(): x(), y(), z(), w()
	{
	}

public:
	/**@brief Add a vector to this one
 * @param The vector to add to this one */
	B3_FORCE_INLINE b3Vector3& operator+=(const b3Vector3& v)
	{
		m_floats[0] += v.m_floats[0];
		m_floats[1] += v.m_floats[1];
		m_floats[2] += v.m_floats[2];
		return *this;
	}

	/**@brief Subtract a vector from this one
   * @param The vector to subtract */
	B3_FORCE_INLINE b3Vector3& operator-=(const b3Vector3& v)
	{
		m_floats[0] -= v.m_floats[0];
		m_floats[1] -= v.m_floats[1];
		m_floats[2] -= v.m_floats[2];
		return *this;
	}

	/**@brief Scale the vector
   * @param s Scale factor */
	B3_FORCE_INLINE b3Vector3& operator*=(const b3Scalar& s)
	{
		m_floats[0] *= s;
		m_floats[1] *= s;
		m_floats[2] *= s;
		return *this;
	}

	/**@brief Inversely scale the vector
   * @param s Scale factor to divide by */
	B3_FORCE_INLINE b3Vector3& operator/=(const b3Scalar& s)
	{
		b3FullAssert(s != b3Scalar(0.0_fl));

		return *this *= 1.0_fl / s;
	}

	/**@brief Return the dot product
   * @param v The other vector in the dot product */
	B3_FORCE_INLINE b3Scalar dot(const b3Vector3& v) const
	{
		return m_floats[0] * v.m_floats[0] +
			   m_floats[1] * v.m_floats[1] +
			   m_floats[2] * v.m_floats[2];
	}

	/**@brief Return the length of the vector squared */
	B3_FORCE_INLINE b3Scalar length2() const
	{
		return dot(*this);
	}

	/**@brief Return the length of the vector */
	B3_FORCE_INLINE b3Scalar length() const
	{
		return b3Sqrt(length2());
	}

	/**@brief Return the distance squared between the ends of this and another vector
   * This is symantically treating the vector like a point */
	B3_FORCE_INLINE b3Scalar distance2(const b3Vector3& v) const;

	/**@brief Return the distance between the ends of this and another vector
   * This is symantically treating the vector like a point */
	B3_FORCE_INLINE b3Scalar distance(const b3Vector3& v) const;

	B3_FORCE_INLINE b3Vector3& safeNormalize()
	{
		b3Scalar l2 = length2();
		//triNormal.normalize();
		if (l2 >= B3_EPSILON * B3_EPSILON)
		{
			(*this) /= b3Sqrt(l2);
		}
		else
		{
			setValue(1_fl, 0_fl, 0_fl);
		}
		return *this;
	}

	/**@brief Normalize this vector
   * x^2 + y^2 + z^2 = 1 */
	B3_FORCE_INLINE b3Vector3& normalize()
	{
		return *this /= length();
	}

	/**@brief Return a normalized version of this vector */
	B3_FORCE_INLINE b3Vector3 normalized() const;

	/**@brief Return a rotated version of this vector
   * @param wAxis The axis to rotate about
   * @param angle The angle to rotate by */
	B3_FORCE_INLINE b3Vector3 rotate(const b3Vector3& wAxis, const b3Scalar angle) const;

	/**@brief Return the angle between this and another vector
   * @param v The other vector */
	B3_FORCE_INLINE b3Scalar angle(const b3Vector3& v) const
	{
		b3Scalar s = b3Sqrt(length2() * v.length2());
		b3FullAssert(s != b3Scalar(0.0_fl));
		return b3Acos(dot(v) / s);
	}

	/**@brief Return a vector will the absolute values of each element */
	B3_FORCE_INLINE b3Vector3 absolute() const
	{
		return b3MakeVector3(
			b3Fabs(m_floats[0]),
			b3Fabs(m_floats[1]),
			b3Fabs(m_floats[2]));
	}

	/**@brief Return the cross product between this and another vector
   * @param v The other vector */
	B3_FORCE_INLINE b3Vector3 cross(const b3Vector3& v) const
	{
		return b3MakeVector3(
			m_floats[1] * v.m_floats[2] - m_floats[2] * v.m_floats[1],
			m_floats[2] * v.m_floats[0] - m_floats[0] * v.m_floats[2],
			m_floats[0] * v.m_floats[1] - m_floats[1] * v.m_floats[0]);
	}

	B3_FORCE_INLINE b3Scalar triple(const b3Vector3& v1, const b3Vector3& v2) const
	{
		return m_floats[0] * (v1.m_floats[1] * v2.m_floats[2] - v1.m_floats[2] * v2.m_floats[1]) +
			   m_floats[1] * (v1.m_floats[2] * v2.m_floats[0] - v1.m_floats[0] * v2.m_floats[2]) +
			   m_floats[2] * (v1.m_floats[0] * v2.m_floats[1] - v1.m_floats[1] * v2.m_floats[0]);
	}

	/**@brief Return the axis with the smallest value
   * Note return values are 0,1,2 for x, y, or z */
	B3_FORCE_INLINE int minAxis() const
	{
		return m_floats[0] < m_floats[1] ? (m_floats[0] < m_floats[2] ? 0 : 2) : (m_floats[1] < m_floats[2] ? 1 : 2);
	}

	/**@brief Return the axis with the largest value
   * Note return values are 0,1,2 for x, y, or z */
	B3_FORCE_INLINE int maxAxis() const
	{
		return m_floats[0] < m_floats[1] ? (m_floats[1] < m_floats[2] ? 2 : 1) : (m_floats[0] < m_floats[2] ? 2 : 0);
	}

	B3_FORCE_INLINE int furthestAxis() const
	{
		return absolute().minAxis();
	}

	B3_FORCE_INLINE int closestAxis() const
	{
		return absolute().maxAxis();
	}

	B3_FORCE_INLINE void setInterpolate3(const b3Vector3& v0, const b3Vector3& v1, b3Scalar rt)
	{
		b3Scalar s = 1.0_fl - rt;
		m_floats[0] = s * v0.m_floats[0] + rt * v1.m_floats[0];
		m_floats[1] = s * v0.m_floats[1] + rt * v1.m_floats[1];
		m_floats[2] = s * v0.m_floats[2] + rt * v1.m_floats[2];
		//don't do the unused w component
		//		m_co[3] = s * v0[3] + rt * v1[3];
	}

	/**@brief Return the linear interpolation between this and another vector
   * @param v The other vector
   * @param t The ration of this to v (t = 0 => return this, t=1 => return other) */
	B3_FORCE_INLINE b3Vector3 lerp(const b3Vector3& v, const b3Scalar& t) const
	{
		return b3MakeVector3(m_floats[0] + (v.m_floats[0] - m_floats[0]) * t,
							 m_floats[1] + (v.m_floats[1] - m_floats[1]) * t,
							 m_floats[2] + (v.m_floats[2] - m_floats[2]) * t);
	}

	/**@brief Elementwise multiply this vector by the other
   * @param v The other vector */
	B3_FORCE_INLINE b3Vector3& operator*=(const b3Vector3& v)
	{
		m_floats[0] *= v.m_floats[0];
		m_floats[1] *= v.m_floats[1];
		m_floats[2] *= v.m_floats[2];
		return *this;
	}

	/**@brief Return the x value */
	B3_FORCE_INLINE const b3Scalar& getX() const { return m_floats[0]; }
	/**@brief Return the y value */
	B3_FORCE_INLINE const b3Scalar& getY() const { return m_floats[1]; }
	/**@brief Return the z value */
	B3_FORCE_INLINE const b3Scalar& getZ() const { return m_floats[2]; }
	/**@brief Return the w value */
	B3_FORCE_INLINE const b3Scalar& getW() const { return m_floats[3]; }

	/**@brief Set the x value */
	B3_FORCE_INLINE void setX(b3Scalar _x) { m_floats[0] = _x; };
	/**@brief Set the y value */
	B3_FORCE_INLINE void setY(b3Scalar _y) { m_floats[1] = _y; };
	/**@brief Set the z value */
	B3_FORCE_INLINE void setZ(b3Scalar _z) { m_floats[2] = _z; };
	/**@brief Set the w value */
	B3_FORCE_INLINE void setW(b3Scalar _w) { m_floats[3] = _w; };

	//B3_FORCE_INLINE b3Scalar&       operator[](int i)       { return (&m_floats[0])[i];	}
	//B3_FORCE_INLINE const b3Scalar& operator[](int i) const { return (&m_floats[0])[i]; }
	///operator b3Scalar*() replaces operator[], using implicit conversion. We added operator != and operator == to avoid pointer comparisons.
	B3_FORCE_INLINE operator b3Scalar*() { return &m_floats[0]; }
	B3_FORCE_INLINE operator const b3Scalar*() const { return &m_floats[0]; }

	B3_FORCE_INLINE bool operator==(const b3Vector3& other) const
	{
		return ((m_floats[3] == other.m_floats[3]) &&
				(m_floats[2] == other.m_floats[2]) &&
				(m_floats[1] == other.m_floats[1]) &&
				(m_floats[0] == other.m_floats[0]));
	}

	B3_FORCE_INLINE bool operator!=(const b3Vector3& other) const
	{
		return !(*this == other);
	}

	/**@brief Set each element to the max of the current values and the values of another b3Vector3
   * @param other The other b3Vector3 to compare with
   */
	B3_FORCE_INLINE void setMax(const b3Vector3& other)
	{
		b3SetMax(m_floats[0], other.m_floats[0]);
		b3SetMax(m_floats[1], other.m_floats[1]);
		b3SetMax(m_floats[2], other.m_floats[2]);
		b3SetMax(m_floats[3], other.m_floats[3]);
	}

	/**@brief Set each element to the min of the current values and the values of another b3Vector3
   * @param other The other b3Vector3 to compare with
   */
	B3_FORCE_INLINE void setMin(const b3Vector3& other)
	{
		b3SetMin(m_floats[0], other.m_floats[0]);
		b3SetMin(m_floats[1], other.m_floats[1]);
		b3SetMin(m_floats[2], other.m_floats[2]);
		b3SetMin(m_floats[3], other.m_floats[3]);
	}

	B3_FORCE_INLINE void setValue(const b3Scalar& _x, const b3Scalar& _y, const b3Scalar& _z)
	{
		m_floats[0] = _x;
		m_floats[1] = _y;
		m_floats[2] = _z;
		m_floats[3] = b3Scalar(0.0_fl);
	}

	void getSkewSymmetricMatrix(b3Vector3 * v0, b3Vector3 * v1, b3Vector3 * v2) const
	{
		v0->setValue(0.0_fl, -getZ(), getY());
		v1->setValue(getZ(), 0.0_fl, -getX());
		v2->setValue(-getY(), getX(), 0.0_fl);
	}

	void setZero()
	{
		setValue(b3Scalar(0.0_fl), b3Scalar(0.0_fl), b3Scalar(0.0_fl));
	}

	B3_FORCE_INLINE bool isZero() const
	{
		return m_floats[0] == b3Scalar(0) && m_floats[1] == b3Scalar(0) && m_floats[2] == b3Scalar(0);
	}

	B3_FORCE_INLINE bool fuzzyZero() const
	{
		return length2() < B3_EPSILON;
	}

	B3_FORCE_INLINE void serialize(struct b3Vector3Data & dataOut) const;

	B3_FORCE_INLINE void deSerialize(const struct b3Vector3Data& dataIn);

	B3_FORCE_INLINE void serializeFloat(struct b3Vector3FloatData & dataOut) const;

	B3_FORCE_INLINE void deSerializeFloat(const struct b3Vector3FloatData& dataIn);

	B3_FORCE_INLINE void serializeDouble(struct b3Vector3DoubleData & dataOut) const;

	B3_FORCE_INLINE void deSerializeDouble(const struct b3Vector3DoubleData& dataIn);

	/**@brief returns index of maximum dot product between this and vectors in array[]
         * @param array The other vectors
         * @param array_count The number of other vectors
         * @param dotOut The maximum dot product */
	B3_FORCE_INLINE long maxDot(const b3Vector3* array, long array_count, b3Scalar& dotOut) const;

	/**@brief returns index of minimum dot product between this and vectors in array[]
         * @param array The other vectors
         * @param array_count The number of other vectors
         * @param dotOut The minimum dot product */
	B3_FORCE_INLINE long minDot(const b3Vector3* array, long array_count, b3Scalar& dotOut) const;

	/* create a vector as  b3Vector3( this->dot( b3Vector3 v0 ), this->dot( b3Vector3 v1), this->dot( b3Vector3 v2 ))  */
	B3_FORCE_INLINE b3Vector3 dot3(const b3Vector3& v0, const b3Vector3& v1, const b3Vector3& v2) const
	{
		return b3MakeVector3(dot(v0), dot(v1), dot(v2));
	}
};

/**@brief Return the sum of two vectors (Point symantics)*/
B3_FORCE_INLINE b3Vector3
operator+(const b3Vector3& v1, const b3Vector3& v2)
{
	return b3MakeVector3(
		v1.m_floats[0] + v2.m_floats[0],
		v1.m_floats[1] + v2.m_floats[1],
		v1.m_floats[2] + v2.m_floats[2]);
}

/**@brief Return the elementwise product of two vectors */
B3_FORCE_INLINE b3Vector3
operator*(const b3Vector3& v1, const b3Vector3& v2)
{
	return b3MakeVector3(
		v1.m_floats[0] * v2.m_floats[0],
		v1.m_floats[1] * v2.m_floats[1],
		v1.m_floats[2] * v2.m_floats[2]);
}

/**@brief Return the difference between two vectors */
B3_FORCE_INLINE b3Vector3
operator-(const b3Vector3& v1, const b3Vector3& v2)
{
	return b3MakeVector3(
		v1.m_floats[0] - v2.m_floats[0],
		v1.m_floats[1] - v2.m_floats[1],
		v1.m_floats[2] - v2.m_floats[2]);
}

/**@brief Return the negative of the vector */
B3_FORCE_INLINE b3Vector3
operator-(const b3Vector3& v)
{
	return b3MakeVector3(-v.m_floats[0], -v.m_floats[1], -v.m_floats[2]);
}

/**@brief Return the vector scaled by s */
B3_FORCE_INLINE b3Vector3
operator*(const b3Vector3& v, const b3Scalar& s)
{
	return b3MakeVector3(v.m_floats[0] * s, v.m_floats[1] * s, v.m_floats[2] * s);
}

/**@brief Return the vector scaled by s */
B3_FORCE_INLINE b3Vector3
operator*(const b3Scalar& s, const b3Vector3& v)
{
	return v * s;
}

/**@brief Return the vector inversely scaled by s */
B3_FORCE_INLINE b3Vector3
operator/(const b3Vector3& v, const b3Scalar& s)
{
	b3FullAssert(s != b3Scalar(0.0_fl));
	return v * (b3Scalar(1.0_fl) / s);
}

/**@brief Return the vector inversely scaled by s */
B3_FORCE_INLINE b3Vector3
operator/(const b3Vector3& v1, const b3Vector3& v2)
{
	return b3MakeVector3(
		v1.m_floats[0] / v2.m_floats[0],
		v1.m_floats[1] / v2.m_floats[1],
		v1.m_floats[2] / v2.m_floats[2]);
}

/**@brief Return the dot product between two vectors */
B3_FORCE_INLINE b3Scalar
b3Dot(const b3Vector3& v1, const b3Vector3& v2)
{
	return v1.dot(v2);
}

/**@brief Return the distance squared between two vectors */
B3_FORCE_INLINE b3Scalar
b3Distance2(const b3Vector3& v1, const b3Vector3& v2)
{
	return v1.distance2(v2);
}

/**@brief Return the distance between two vectors */
B3_FORCE_INLINE b3Scalar
b3Distance(const b3Vector3& v1, const b3Vector3& v2)
{
	return v1.distance(v2);
}

/**@brief Return the angle between two vectors */
B3_FORCE_INLINE b3Scalar
b3Angle(const b3Vector3& v1, const b3Vector3& v2)
{
	return v1.angle(v2);
}

/**@brief Return the cross product of two vectors */
B3_FORCE_INLINE b3Vector3
b3Cross(const b3Vector3& v1, const b3Vector3& v2)
{
	return v1.cross(v2);
}

B3_FORCE_INLINE b3Scalar
b3Triple(const b3Vector3& v1, const b3Vector3& v2, const b3Vector3& v3)
{
	return v1.triple(v2, v3);
}

/**@brief Return the linear interpolation between two vectors
 * @param v1 One vector
 * @param v2 The other vector
 * @param t The ration of this to v (t = 0 => return v1, t=1 => return v2) */
B3_FORCE_INLINE b3Vector3
b3Lerp(const b3Vector3& v1, const b3Vector3& v2, const b3Scalar& t)
{
	return v1.lerp(v2, t);
}

B3_FORCE_INLINE b3Scalar b3Vector3::distance2(const b3Vector3& v) const
{
	return (v - *this).length2();
}

B3_FORCE_INLINE b3Scalar b3Vector3::distance(const b3Vector3& v) const
{
	return (v - *this).length();
}

B3_FORCE_INLINE b3Vector3 b3Vector3::normalized() const
{
#if defined(B3_USE_SSE_IN_API) && defined(B3_USE_SSE)
	b3Vector3 norm = *this;

	return norm.normalize();
#else
	return *this / length();
#endif
}

B3_FORCE_INLINE b3Vector3 b3Vector3::rotate(const b3Vector3& wAxis, const b3Scalar _angle) const
{
	// wAxis must be a unit lenght vector
	b3Vector3 o = wAxis * wAxis.dot(*this);
	b3Vector3 _x = *this - o;
	b3Vector3 _y;

	_y = wAxis.cross(*this);

	return (o + _x * b3Cos(_angle) + _y * b3Sin(_angle));
}

B3_FORCE_INLINE long b3Vector3::maxDot(const b3Vector3* array, long array_count, b3Scalar& dotOut) const
{
#if defined(B3_USE_SSE) || defined(B3_USE_NEON)
#if defined _WIN32 || defined(B3_USE_SSE)
	const long scalar_cutoff = 10;
	long b3_maxdot_large(const float* array, const float* vec, unsigned long array_count, float* dotOut);
#elif defined B3_USE_NEON
	const long scalar_cutoff = 4;
	extern long (*_maxdot_large)(const float* array, const float* vec, unsigned long array_count, float* dotOut);
#endif
	if (array_count < scalar_cutoff)
#else

#endif  //B3_USE_SSE || B3_USE_NEON
	{
		b3Scalar maxDot = -B3_INFINITY;
		int i = 0;
		int ptIndex = -1;
		for (i = 0; i < array_count; i++)
		{
			b3Scalar dot = array[i].dot(*this);

			if (dot > maxDot)
			{
				maxDot = dot;
				ptIndex = i;
			}
		}

		b3Assert(ptIndex >= 0);
		if (ptIndex < 0)
		{
			ptIndex = 0;
		}
		dotOut = maxDot;
		return ptIndex;
	}
#if defined(B3_USE_SSE) || defined(B3_USE_NEON)
	return b3_maxdot_large((float*)array, (float*)&m_floats[0], array_count, &dotOut);
#endif
}

B3_FORCE_INLINE long b3Vector3::minDot(const b3Vector3* array, long array_count, b3Scalar& dotOut) const
{
#if defined(B3_USE_SSE) || defined(B3_USE_NEON)
#if defined B3_USE_SSE
	const long scalar_cutoff = 10;
	long b3_mindot_large(const float* array, const float* vec, unsigned long array_count, float* dotOut);
#elif defined B3_USE_NEON
	const long scalar_cutoff = 4;
	extern long (*b3_mindot_large)(const float* array, const float* vec, unsigned long array_count, float* dotOut);
#else
#error unhandled arch!
#endif

	if (array_count < scalar_cutoff)
#endif  //B3_USE_SSE || B3_USE_NEON
	{
		b3Scalar minDot = B3_INFINITY;
		int i = 0;
		int ptIndex = -1;

		for (i = 0; i < array_count; i++)
		{
			b3Scalar dot = array[i].dot(*this);

			if (dot < minDot)
			{
				minDot = dot;
				ptIndex = i;
			}
		}

		dotOut = minDot;

		return ptIndex;
	}
#if defined(B3_USE_SSE) || defined(B3_USE_NEON)
	return b3_mindot_large((float*)array, (float*)&m_floats[0], array_count, &dotOut);
#endif
}

class b3Vector4 : public b3Vector3
{
public:
	B3_FORCE_INLINE b3Vector4 absolute4() const
	{
		return b3MakeVector4(
			b3Fabs(m_floats[0]),
			b3Fabs(m_floats[1]),
			b3Fabs(m_floats[2]),
			b3Fabs(m_floats[3]));
	}

	b3Scalar getW() const { return m_floats[3]; }

	B3_FORCE_INLINE int maxAxis4() const
	{
		int maxIndex = -1;
		b3Scalar maxVal = b3Scalar(-B3_LARGE_FLOAT);
		if (m_floats[0] > maxVal)
		{
			maxIndex = 0;
			maxVal = m_floats[0];
		}
		if (m_floats[1] > maxVal)
		{
			maxIndex = 1;
			maxVal = m_floats[1];
		}
		if (m_floats[2] > maxVal)
		{
			maxIndex = 2;
			maxVal = m_floats[2];
		}
		if (m_floats[3] > maxVal)
		{
			maxIndex = 3;
		}

		return maxIndex;
	}

	B3_FORCE_INLINE int minAxis4() const
	{
		int minIndex = -1;
		b3Scalar minVal = b3Scalar(B3_LARGE_FLOAT);
		if (m_floats[0] < minVal)
		{
			minIndex = 0;
			minVal = m_floats[0];
		}
		if (m_floats[1] < minVal)
		{
			minIndex = 1;
			minVal = m_floats[1];
		}
		if (m_floats[2] < minVal)
		{
			minIndex = 2;
			minVal = m_floats[2];
		}
		if (m_floats[3] < minVal)
		{
			minIndex = 3;
			minVal = m_floats[3];
		}

		return minIndex;
	}

	B3_FORCE_INLINE int closestAxis4() const
	{
		return absolute4().maxAxis4();
	}

	/**@brief Set the values
   * @param x Value of x
   * @param y Value of y
   * @param z Value of z
   * @param w Value of w
   */
	B3_FORCE_INLINE void setValue(const b3Scalar& _x, const b3Scalar& _y, const b3Scalar& _z, const b3Scalar& _w)
	{
		m_floats[0] = _x;
		m_floats[1] = _y;
		m_floats[2] = _z;
		m_floats[3] = _w;
	}
};

///b3SwapVector3Endian swaps vector endianness, useful for network and cross-platform serialization
B3_FORCE_INLINE void b3SwapScalarEndian(const b3Scalar& sourceVal, b3Scalar& destVal)
{
#ifdef B3_USE_DOUBLE_PRECISION
	unsigned char* dest = (unsigned char*)&destVal;
	unsigned char* src = (unsigned char*)&sourceVal;
	dest[0] = src[7];
	dest[1] = src[6];
	dest[2] = src[5];
	dest[3] = src[4];
	dest[4] = src[3];
	dest[5] = src[2];
	dest[6] = src[1];
	dest[7] = src[0];
#else
	unsigned char* dest = (unsigned char*)&destVal;
	unsigned char* src = (unsigned char*)&sourceVal;
	dest[0] = src[3];
	dest[1] = src[2];
	dest[2] = src[1];
	dest[3] = src[0];
#endif  //B3_USE_DOUBLE_PRECISION
}
///b3SwapVector3Endian swaps vector endianness, useful for network and cross-platform serialization
B3_FORCE_INLINE void b3SwapVector3Endian(const b3Vector3& sourceVec, b3Vector3& destVec)
{
	for (int i = 0; i < 4; i++)
	{
		b3SwapScalarEndian(sourceVec[i], destVec[i]);
	}
}

///b3UnSwapVector3Endian swaps vector endianness, useful for network and cross-platform serialization
B3_FORCE_INLINE void b3UnSwapVector3Endian(b3Vector3& vector)
{
	b3Vector3 swappedVec;
	for (int i = 0; i < 4; i++)
	{
		b3SwapScalarEndian(vector[i], swappedVec[i]);
	}
	vector = swappedVec;
}

template <class T>
B3_FORCE_INLINE void b3PlaneSpace1(const T& n, T& p, T& q)
{
	if (b3Fabs(n[2]) > B3_SQRT12)
	{
		// choose p in y-z plane
		b3Scalar a = n[1] * n[1] + n[2] * n[2];
		b3Scalar k = b3RecipSqrt(a);
		p[0] = 0;
		p[1] = -n[2] * k;
		p[2] = n[1] * k;
		// set q = n x p
		q[0] = a * k;
		q[1] = -n[0] * p[2];
		q[2] = n[0] * p[1];
	}
	else
	{
		// choose p in x-y plane
		b3Scalar a = n[0] * n[0] + n[1] * n[1];
		b3Scalar k = b3RecipSqrt(a);
		p[0] = -n[1] * k;
		p[1] = n[0] * k;
		p[2] = 0;
		// set q = n x p
		q[0] = -n[2] * p[1];
		q[1] = n[2] * p[0];
		q[2] = a * k;
	}
}

struct b3Vector3FloatData
{
	float m_floats[4];
};

struct b3Vector3DoubleData
{
	double m_floats[4];
};

B3_FORCE_INLINE void b3Vector3::serializeFloat(struct b3Vector3FloatData& dataOut) const
{
	///could also do a memcpy, check if it is worth it
	for (int i = 0; i < 4; i++)
		dataOut.m_floats[i] = m_floats[i].ToFloat();
}

B3_FORCE_INLINE void b3Vector3::deSerializeFloat(const struct b3Vector3FloatData& dataIn)
{
	for (int i = 0; i < 4; i++)
		m_floats[i] = b3Scalar(dataIn.m_floats[i]);
}

B3_FORCE_INLINE void b3Vector3::serializeDouble(struct b3Vector3DoubleData& dataOut) const
{
	///could also do a memcpy, check if it is worth it
	for (int i = 0; i < 4; i++)
		dataOut.m_floats[i] = m_floats[i].ToDouble();
}

B3_FORCE_INLINE void b3Vector3::deSerializeDouble(const struct b3Vector3DoubleData& dataIn)
{
	for (int i = 0; i < 4; i++)
		m_floats[i] = b3Scalar(dataIn.m_floats[i]);
}

B3_FORCE_INLINE void b3Vector3::serialize(struct b3Vector3Data& dataOut) const
{
	///could also do a memcpy, check if it is worth it
	for (int i = 0; i < 4; i++)
		dataOut.m_floats[i] = m_floats[i].ToDouble();
}

B3_FORCE_INLINE void b3Vector3::deSerialize(const struct b3Vector3Data& dataIn)
{
	for (int i = 0; i < 4; i++)
		m_floats[i] = b3Scalar(dataIn.m_floats[i]);
}

inline b3Vector3 b3MakeVector3(b3Scalar x, b3Scalar y, b3Scalar z)
{
	b3Vector3 tmp;
	tmp.setValue(x, y, z);
	return tmp;
}

inline b3Vector3 b3MakeVector3(b3Scalar x, b3Scalar y, b3Scalar z, b3Scalar w)
{
	b3Vector3 tmp;
	tmp.setValue(x, y, z);
	tmp.w = w;
	return tmp;
}

inline b3Vector4 b3MakeVector4(b3Scalar x, b3Scalar y, b3Scalar z, b3Scalar w)
{
	b3Vector4 tmp;
	tmp.setValue(x, y, z, w);
	return tmp;
}

#if defined(B3_USE_SSE_IN_API) && defined(B3_USE_SSE)

inline b3Vector3 b3MakeVector3(b3SimdFloat4 v)
{
	b3Vector3 tmp;
	tmp.set128(v);
	return tmp;
}

inline b3Vector4 b3MakeVector4(b3SimdFloat4 vec)
{
	b3Vector4 tmp;
	tmp.set128(vec);
	return tmp;
}

#endif

#endif  //B3_VECTOR3_H
