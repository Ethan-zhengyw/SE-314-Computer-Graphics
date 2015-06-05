#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <cmath>

class Vector3
{
public:
	Vector3();
	Vector3(float x_, float y_, float z_);

	// 向量基本四则运算
	Vector3 operator+(Vector3 v);
	Vector3 operator-(Vector3 v);
	Vector3 operator*(float n);
	Vector3 operator/(float n);

	static Vector3 cross(Vector3, Vector3);  // 向量叉积运算
	static float magnitude(Vector3);         // 向量求模
	static Vector3 normalize(Vector3);       // 向量正则化，使之模为1

	float x, y, z;
};

#endif