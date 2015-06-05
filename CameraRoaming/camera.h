#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vector3.h"

class Camera {
public:
	Camera();	

	Vector3 getPosition();
	Vector3 getView();
	Vector3 getUp();
	Vector3 getStrafe();
	
	// 设置摄像机的位置、观看的坐标、以及头顶的朝向
	void setPosition(float positionX, float positionY, float positionZ,
		float viewX, float viewY, float viewZ,
		float upX, float upY, float upZ);

	void update();  // 每次回掉函数中首先执行的函数，更新strafe向量，保证左右移动的正确

	void processMouse(int x, int y); 
	void rotateView(float angle, float X, float Y, float Z);

	// 在按下键盘后调用的函数，相对于相机朝向的方向的前后(move)左右(strafe)移动摄像机
	void processKey(unsigned char key, int x, int y);
	void strafe(float speed);  // 左右移动摄像机, speed的正负决定摄像机移动的方向
	void move(float speed);  // 前后移动摄像机

	void Look();

private:
	Vector3 vPosition;					
	Vector3 vView;						
	Vector3 vUp;		
	Vector3 vStrafe;						
};

#endif