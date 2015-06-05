#include <GL/glut.h>
#include "camera.h"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900
#define SPEED 10.0f  // 摄像机移动的速度

Vector3 Camera::getPosition() {
	return vPosition;		
}
Vector3 Camera::getView() {	
	return vView;			
}
Vector3 Camera::getUp() {	
	return vUp;		
}
Vector3 Camera::getStrafe() {
	return vStrafe;		
}

Camera::Camera()
{
	vPosition = Vector3(0.0, 0.0, 0.0);  // 摄像机放在原点
	vView = Vector3(0.0, 1.0, 0.5);      // 看着屏幕外边、偏上
	vUp = Vector3(0.0, 0.0, 1.0);        // 头顶朝着正上方，不歪
}

void Camera::setPosition(float positionX, float positionY, float positionZ,
	float viewX, float viewY, float viewZ, float upX, float upY, float upZ)
{
	vPosition = Vector3(positionX, positionY, positionZ);
	vView = Vector3(viewX, viewY, viewZ);
	vUp	= Vector3(upX, upY, upZ);
}

void Camera::Look()
{
	gluLookAt(
		vPosition.x, vPosition.y, vPosition.z,
		vView.x, vView.y, vView.z,
		vUp.x, vUp.y, vUp.z);
}

void Camera::update()
{
	vStrafe = Vector3::normalize(Vector3::cross(vView - vPosition, vUp));
}

void Camera::move(float speed)
{
	// 先获取当前摄像机拍摄的单位方向向量
	Vector3 v = Vector3::normalize(vView - vPosition);

	float biasX = v.x * speed;
	float biasY = v.y * speed;
	float biasZ = v.z * speed;

	// 移动摄像机的位置
	vPosition.x += biasX;
	vPosition.y += biasY;
	vPosition.z += biasZ;

	// 记得要移动看的点（头不能转动），不然就相当于一边走一边歪头
	vView.x += biasX;
	vView.y += biasY;
	vView.z += biasZ;
}

void Camera::strafe(float speed)
{
	// 移动的方式和move一样，不过方向是strafe（右边朝向）
	// 是“看”的方向与头顶朝向的叉积
	vPosition.x += vStrafe.x * speed;
	vPosition.z += vStrafe.z * speed;

	vView.x += vStrafe.x * speed;
	vView.z += vStrafe.z * speed;
}

void Camera::processKey(unsigned char key, int x, int y)
{
	update();

	float speed = SPEED;

	if (key == 'w' || key == 'W')
		move(speed);

	if (key == 's' || key == 'S')
		move(-speed);

	if (key == 'a' || key == 'A')
		strafe(-speed);

	if (key == 'd' || key == 'D')
		strafe(speed);
}

void Camera::processMouse(int x, int y)
{
	int middleX = SCREEN_WIDTH  >> 1;
	int middleY = SCREEN_HEIGHT >> 1;

	float angleH = 0.0f;
	float angleV = 0.0f;
	static float currentRotX = 0.0f;

	// 如果还在中点说明没有移动，那就不用继续处理了
	if( (x == middleX) && (y == middleY) ) return;  

	glutWarpPointer(middleX, middleY);  // 重新把指针放到屏幕中间					

	angleH = (float)(middleX - x) / (0.5 * SCREEN_WIDTH);
	angleV = (float)(middleY - y) / (0.5 * SCREEN_HEIGHT);

	static float lastRotX = 0.0f; 
 	lastRotX = currentRotX;
	
	currentRotX += angleV;
 
	if(currentRotX > 1.0f) {
		currentRotX = 1.0f;
		if(lastRotX != 1.0f) {
			Vector3 vAxis = Vector3::cross(vView - vPosition, vUp);
			vAxis = Vector3::normalize(vAxis);
			rotateView(1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	} else if(currentRotX < -1.0f) {
		currentRotX = -1.0f;
		if(lastRotX != -1.0f) {
			Vector3 vAxis = Vector3::cross(vView - vPosition, vUp);
			vAxis = Vector3::normalize(vAxis);
			rotateView(-1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	} else {	
		Vector3 vAxis = Vector3::cross(vView - vPosition, vUp);
		vAxis = Vector3::normalize(vAxis);
	
		rotateView(angleV, vAxis.x, vAxis.y, vAxis.z);
	}

	rotateView(angleH, 0, 1, 0);
}

void Camera::rotateView(float angle, float x, float y, float z)
{
	// 获取朝向向量，同时定义一个新的朝向向量，准备替换
	Vector3 v = vView - vPosition, vNew;

	// 求新的朝向向量
	float cosTheta = cos(angle);
	float sinTheta = sin(angle);

	vNew.x = (cosTheta + (1 - cosTheta) * x * x) * v.x;
	vNew.x += ((1 - cosTheta) * x * y - z * sinTheta) * v.y;
	vNew.x += ((1 - cosTheta) * x * z + y * sinTheta) * v.z;

	vNew.y = ((1 - cosTheta) * x * y + z * sinTheta) * v.x;
	vNew.y += (cosTheta + (1 - cosTheta) * y * y) * v.y;
	vNew.y += ((1 - cosTheta) * y * z - x * sinTheta) * v.z;

	vNew.z = ((1 - cosTheta) * x * z - y * sinTheta) * v.x;
	vNew.z += ((1 - cosTheta) * y * z + x * sinTheta) * v.y;
	vNew.z += (cosTheta + (1 - cosTheta) * z * z) * v.z;

	// 新的观测点就是相机的位置加上新的朝向向量
	vView = vPosition + vNew;
}
