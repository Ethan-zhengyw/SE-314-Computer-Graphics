#include "vector3.h"

Vector3::Vector3()
	: x(1), y(1), z(1) {}

Vector3::Vector3(float x_, float y_, float z_)
	: x(x_), y(y_), z(z_) {}

Vector3 Vector3::operator+(Vector3 v)
{
	return Vector3(v.x + x, v.y + y, v.z + z);
}

Vector3 Vector3::operator-(Vector3 v)
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator*(float num)
{
	return Vector3(x * num, y * num, z * num);
}

Vector3 Vector3::operator/(float num)
{
	return Vector3(x / num, y / num, z / num);
}


Vector3 Vector3::cross(Vector3 v1, Vector3 v2)
{
	Vector3 vCross;

	vCross.x = v1.y * v2.z - v1.z * v2.y;  //     | i   j   k  |
	vCross.y = v1.z * v2.x - v1.x * v2.z;  //  det| x1  y1  z1 |
	vCross.z = v1.x * v2.y - v1.y * v2.x;  //     | x2  y2  z2 |

	return vCross;
}

float Vector3::magnitude(Vector3 v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Vector3::normalize(Vector3 v)
{
	return v / Vector3::magnitude(v);
}