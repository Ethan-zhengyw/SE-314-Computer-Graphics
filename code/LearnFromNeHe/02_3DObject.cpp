#define _CRT_SECURE_NO_DEPRECATE
#include <Windows.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glaux.h>
#pragma comment(lib, "glaux.lib")

HGLRC hRC = NULL;				// 窗口着色描述表句柄
HDC hDC = NULL;					// OpenGL渲染描述表句柄
HWND hWnd = NULL;				// 保存我们的窗口句柄
HINSTANCE hInstance;			// 保存程序的实例
bool keys[256];					// 保存键盘按键的数组
bool active = TRUE;				// 窗口的活动标志，缺省为TRUE
bool fullscreen = TRUE;			// 全屏标志缺省，缺省设定成全屏模式

// 下面的变量是用来操作图形移动旋转的
GLfloat inc_t = 0.1, inc_r = 20;
GLfloat tran_x = 0, tran_y = 0, tran_z = 0;  // 移动图形（其实就是把画画的位置改变下）
GLfloat rota_x = 0, rota_y = 0, rota_z = 0;  // 旋转图形
GLuint texture[1];						 // 存储一个纹理

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);  // WndProc的定义

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)  // 重置OpenGL窗口大小
{
	if (height == 0) {  // 防止被零除
		height = 1;     // 将Height设为1
	}

	glViewport(0, 0, width, height);	// 重置当前的视口
	glMatrixMode(GL_PROJECTION);		// 选择投影矩阵
	glLoadIdentity();					// 重置投影矩阵

	// 设置视口的大小
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);					// 选择模型观察矩阵
	glLoadIdentity();							// 重置模型观察矩阵
}
int InitGL(GLvoid)  // 此处开始对OpenGL进行所有设置
{
	glShadeModel(GL_SMOOTH);					// 启用阴影平滑
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// 黑色背景
	glClearDepth(1.0f);							// 设置深度缓存
	glEnable(GL_DEPTH_TEST);					// 启用深度测试
	glDepthFunc(GL_LEQUAL);						// 所作深度测试的类型
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // 告诉系统对透视进行修正
	return TRUE;								// 初始化 OK
}
GLvoid KillGLWindow(GLvoid)					// 正常销毁窗口
{
	if (fullscreen)	{  // 我们处于全屏模式吗?
		ChangeDisplaySettings(NULL, 0);		// 是的话，切换回桌面
		ShowCursor(TRUE);					// 显示鼠标指针
	}
	if (hRC) {  // 我们拥有OpenGL渲染描述表吗?
		if (!wglMakeCurrent(NULL, NULL)) {  // 我们能否释放DC和RC描述表?
			MessageBox(NULL, TEXT("释放DC或RC失败。"), TEXT("关闭错误"), MB_OK | MB_ICONINFORMATION);
		}
		if (!wglDeleteContext(hRC)) {  // 我们能否删除RC?
			MessageBox(NULL, TEXT("释放RC失败。"), TEXT("关闭错误"), MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;  // 将RC设为 NULL
	}
	if (hDC && !ReleaseDC(hWnd, hDC)) {  // 我们能否释放 DC?
		MessageBox(NULL, TEXT("释放DC失败。"), TEXT("关闭错误"), MB_OK | MB_ICONINFORMATION);
		hDC = NULL;  // 将 DC 设为 NULL
	}
	if (hWnd && !DestroyWindow(hWnd)) {  // 能否销毁窗口?
		MessageBox(NULL, TEXT("释放窗口句柄失败。"), TEXT("关闭错误"), MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;  // 将 hWnd 设为 NULL
	}
	if (!UnregisterClass(TEXT("OpenG"), hInstance)) {  // 能否注销类?
		MessageBox(NULL, TEXT("不能注销窗口类。"), TEXT("关闭错误"), MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;  // 将 hInstance 设为 NULL
	}
}
BOOL CreateGLWindow(wchar_t* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;				// 保存查找匹配的结果
	WNDCLASS	wc;							// 窗口类结构
	DWORD		dwExStyle;					// 扩展窗口风格
	DWORD		dwStyle;					// 窗口风格
	RECT WindowRect;						// 取得矩形的左上角和右下角的坐标值
	WindowRect.left = (long)0;				// 将Left   设为 0
	WindowRect.right = (long)width;			// 将Right  设为要求的宽度
	WindowRect.top = (long)0;				// 将Top    设为 0
	WindowRect.bottom = (long)height;		// 将Bottom 设为要求的高度
	fullscreen = fullscreenflag;			// 设置全局全屏标志
	hInstance = GetModuleHandle(NULL);		// 取得我们窗口的实例
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// 移动时重画，并为窗口取得DC
	wc.lpfnWndProc = (WNDPROC)WndProc;		// WndProc处理消息
	wc.cbClsExtra = 0;						// 无额外窗口数据
	wc.cbWndExtra = 0;						// 无额外窗口数据
	wc.hInstance = hInstance;				// 设置实例
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);		// 装入缺省图标
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// 装入鼠标指针
	wc.hbrBackground = NULL;					// GL不需要背景
	wc.lpszMenuName = NULL;						// 不需要菜单
	wc.lpszClassName = TEXT("OpenG");					// 设定类名字
	if (!RegisterClass(&wc)) {					// 尝试注册窗口类
		MessageBox(NULL, TEXT("注册窗口失败"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// 退出并返回FALSE
	}
	if (fullscreen) {							// 要尝试全屏模式吗?
		DEVMODE dmScreenSettings;				// 设备模式
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));		// 确保内存清空为零
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);			// Devmode 结构的大小
		dmScreenSettings.dmPelsWidth = width;						// 所选屏幕宽度
		dmScreenSettings.dmPelsHeight = height;						// 所选屏幕高度
		dmScreenSettings.dmBitsPerPel = bits;						// 每象素所选的色彩深度
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		// 尝试设置显示模式并返回结果。注: CDS_FULLSCREEN 移去了状态条。
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// 若模式失败，提供两个选项：退出或在窗口内运行。
			if (MessageBox(NULL, TEXT("全屏模式在当前显卡上设置失败！\n使用窗口模式？"), TEXT("Hello OpenGL!"), MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
				fullscreen = FALSE;  // 选择窗口模式(Fullscreen=FALSE)
			}
			else {  // 弹出一个对话框，告诉用户程序结束
				MessageBox(NULL, TEXT("程序将被关闭"), TEXT("错误"), MB_OK | MB_ICONSTOP);
				return FALSE;  //  退出并返回 FALSE
			}
		}
	}
	if (fullscreen)	{						// 仍处于全屏模式吗?
		dwExStyle = WS_EX_APPWINDOW;		// 扩展窗体风格
		dwStyle = WS_POPUP;					// 窗体风格
		ShowCursor(FALSE);					// 隐藏鼠标指针
	}
	else {
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;		// 扩展窗体风格
		dwStyle = WS_OVERLAPPEDWINDOW;						//  窗体风格
	}
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// 调整窗口达到真正要求的大小
	if (!(hWnd = CreateWindowEx(dwExStyle,				// 扩展窗体风格
		TEXT("OpenG"),			// 类名字
		title,			// 窗口标题
		WS_CLIPSIBLINGS |		// 必须的窗体风格属性
		WS_CLIPCHILDREN |		// 必须的窗体风格属性
		dwStyle,				// 选择的窗体属性
		0, 0,					// 窗口位置
		WindowRect.right - WindowRect.left,	// 计算调整好的窗口宽度
		WindowRect.bottom - WindowRect.top,	// 计算调整好的窗口高度
		NULL,					// 无父窗口
		NULL,					// 无菜单
		hInstance,				// 实例
		NULL)))	{				// 不向WM_CREATE传递任何东东
		KillGLWindow();							// 重置显示区
		MessageBox(NULL, TEXT("不能创建一个窗口设备描述表"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}
	static	PIXELFORMATDESCRIPTOR pfd =					// /pfd 告诉窗口我们所希望的东东，即窗口使用的像素格式
	{
		sizeof(PIXELFORMATDESCRIPTOR),					// 上述格式描述符的大小
		1,								// 版本号
		PFD_DRAW_TO_WINDOW |			// 格式支持窗口
		PFD_SUPPORT_OPENGL |			// 格式必须支持OpenGL
		PFD_DOUBLEBUFFER,				// 必须支持双缓冲
		PFD_TYPE_RGBA,					// 申请 RGBA 格式
		bits,							// 选定色彩深度
		0, 0, 0, 0, 0, 0,				// 忽略的色彩位
		0,								// 无Alpha缓存
		0,								// 忽略Shift Bit
		0,								// 无累加缓存
		0, 0, 0, 0,						// 忽略聚集位
		16,								// 16位 Z-缓存 (深度缓存)
		0,								// 无蒙板缓存
		0,								// 无辅助缓存
		PFD_MAIN_PLANE,					// 主绘图层
		0,								// Reserved
		0, 0, 0							// 忽略层遮罩
	};
	if (!(hDC = GetDC(hWnd))) {  // 取得设备描述表了么?
		KillGLWindow();			 // 重置显示区
		MessageBox(NULL, TEXT("不能创建一种相匹配的像素格式"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;			 // 返回 FALSE
	}
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) {  // Windows 找到相应的象素格式了吗?
		KillGLWindow();							// 重置显示区
		MessageBox(NULL, TEXT("不能设置像素格式"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}
	if (!SetPixelFormat(hDC, PixelFormat, &pfd)) {  // 能够设置象素格式么?
		KillGLWindow();								// 重置显示区
		MessageBox(NULL, TEXT("不能设置像素格式"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;  // 返回 FALSE
	}
	if (!(hRC = wglCreateContext(hDC)))	{  // 能否取得着色描述表?
		KillGLWindow();					   // 重置显示区
		MessageBox(NULL, TEXT("不能创建OpenGL渲染描述表"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;					   // 返回 FALSE
	}
	if (!wglMakeCurrent(hDC, hRC)) {  // 尝试激活着色描述表
		KillGLWindow();				  // 重置显示区
		MessageBox(NULL, TEXT("不能激活当前的OpenGL渲然描述表"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;				  // 返回 FALSE
	}
	ShowWindow(hWnd, SW_SHOW);					// 显示窗口
	SetForegroundWindow(hWnd);					// 略略提高优先级
	SetFocus(hWnd);								// 设置键盘的焦点至此窗口
	ReSizeGLScene(width, height);				// 设置透视 GL 屏幕
	if (!InitGL()) {  // 初始化新建的GL窗口
		KillGLWindow();							// 重置显示区
		MessageBox(NULL, TEXT("Initialization Failed."), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}
	return TRUE;								// 成功
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
// 窗口的句柄 窗口的消息 附加的消息内容 附加的消息内容
{
	switch (uMsg) {					// 检查Windows消息
	case WM_ACTIVATE: { 			// 监视窗口激活消息
		if (!HIWORD(wParam)) {		// 检查最小化状态
			active = TRUE;			// 程序处于激活状态
		}
		else {
			active = FALSE;			// 程序不再激活
		}
		return 0;					// 返回消息循环
	}
	case WM_SYSCOMMAND: {			// 系统中断命令
		switch (wParam) {			// 检查系统调用
		case SC_SCREENSAVE:			// 屏保要运行?
		case SC_MONITORPOWER:		// 显示器要进入节电模式?
			return 0;				// 阻止发生
		}
		break;						// 退出
	}
	case WM_CLOSE: {				// 收到Close消息?
		PostQuitMessage(0);			// 发出退出消息
		return 0;					// 返回
	}
	case WM_KEYDOWN: {			// 有键按下么?
		keys[wParam] = TRUE;		// 如果是，设为TRUE
		return 0;					// 返回
	}
	case WM_KEYUP: {				// 有键放开么?
		keys[wParam] = FALSE;		// 如果是，设为FALSE
		return 0;					// 返回
	}
	case WM_SIZE: {				// 调整OpenGL窗口大小
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));		// LoWord=Width,HiWord=Height
		return 0;						// 返回
	}
	}
	// 向 DefWindowProc传递所有未处理的消息。
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
int DrawGLScene(GLvoid)  // 从这里开始进行所有的绘制
{
	rota_x += 0.1;  // 自动旋转
	rota_y += 0.1;  // 自动旋转
	rota_z += 0.1;  // 自动旋转

	// tran_z += 0.1;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 清除屏幕和深度缓存
	glLoadIdentity();									 // 重置当前的模型观察矩阵

	glTranslatef(0.0f, 0.0f, -10.0f);		// 并移入屏幕 6.0，这样才看得见
	// gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);  // 如果并没有调用gluLookAt(),那么照相机就被设置为默认的位置和方向。在默认情况下，照相机位于原点，指向z轴的负方向，朝上向量为(0,1,0) 
	// glViewport(0, 0, 200, 200);

	glTranslatef(tran_x, tran_y, tran_z);
	glRotatef(rota_x, 1, 0, 0);
	glRotatef(rota_y, 0, 1, 0);
	glRotatef(rota_z, 0, 0, 1);

	// glBegin(GL_QUADS);  // 绘制四边形
	// 	glColor3f(1.0, 0.0, 0.0); glVertex2f(3.0, 0.0);					
	// 	glColor3f(0.0, 1.0, 0.0); glVertex2f(0.0, 1.0);	
	// 	glColor3f(0.0, 0.0, 1.0); glVertex2f(-3.0, 0.0);					
	// 	glColor3f(0.0, 0.0, 0.0); glVertex2f(0.0, -1.0);			
	// glEnd();

	// 自己猜测的绘制立体图形的方法：分别绘制四个三角形
	{
		glBegin(GL_TRIANGLES);  // 四面体的前侧面
		glColor3f(1.0f, 0.0f, 0.0f);  // 上红
		glVertex3f(0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);  // 左下绿
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glColor3f(0.0f, 0.0f, 1.0f);  // 右下蓝
		glVertex3f(1.0f, -1.0f, 1.0f);
		glEnd();
		glBegin(GL_TRIANGLES);  // 四面体的右侧面
		glColor3f(0.0f, 0.0f, 1.0f);  // 右下蓝
		glVertex3f(1.0f, -1.0f, 1.0f);
		glColor3f(0.0f, 0.0f, 0.0f);  // 后黑
		glVertex3f(0.0f, -1.0f, -1.0f);
		glColor3f(1.0f, 0.0f, 0.0f);  // 上红
		glVertex3f(0.0f, 1.0f, 0.0f);
		glEnd();
		glBegin(GL_TRIANGLES);  // 四面体的左侧面
		glColor3f(1.0f, 0.0f, 0.0f);  // 上红
		glVertex3f(0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 0.0f);  // 后黑
		glVertex3f(0.0f, -1.0f, -1.0f);
		glColor3f(0.0f, 1.0f, 0.0f);  // 左下绿
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glEnd();
		glBegin(GL_TRIANGLES);  // 四面体的左侧面
		glColor3f(0.0f, 1.0f, 0.0f);  // 左下绿
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glColor3f(0.0f, 0.0f, 1.0f);  // 右下蓝
		glVertex3f(1.0f, -1.0f, 1.0f);
		glColor3f(0.0f, 0.0f, 0.0f);  // 后黑
		glVertex3f(0.0f, -1.0f, -1.0f);
		glEnd();
	}

	// NeHe教程中的方法，有点出入，是一次性画完多个三角形的，共享顶点的
	/*{
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);   // 上方的红点
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // 左前下的绿点
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);  // 右前下的蓝点

	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);   // 上方的红点
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);  // 右前下的蓝点
	glColor3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, -1.0f, -1.0f); // 后方的黑点

	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);   // 上方的红点
	glColor3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, -1.0f, -1.0f); // 后方的黑点
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // 左前下的绿点

	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // 左前下的绿点
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);  // 右前下的蓝点
	glColor3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, -1.0f, -1.0f); // 后方的黑点
	glEnd();
	}*/

	// 画个正方体
	{
		glBegin(GL_QUADS);

		glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);    // 1
		glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);	   // 2
		glColor3f(1.0f, 1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);    // 3
		glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);   // 4

		glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);    // 1
		glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);   // 5
		glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);	   // 6
		glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);	   // 2

		glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);	   // 2
		glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);	   // 6
		glColor3f(1.0f, 1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);   // 7
		glColor3f(1.0f, 1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);    // 3

		glColor3f(1.0f, 1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);    // 3
		glColor3f(1.0f, 1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);   // 7
		glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // 8
		glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);   // 4

		glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);   // 4
		glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // 8
		glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);   // 5
		glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);    // 1

		glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);   // 5
		glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);	   // 6
		glColor3f(1.0f, 1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);   // 7
		glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // 8

		glEnd();
	}

	if (keys[VK_F1]) {					// F1键按下了么?
		keys[VK_F1] = FALSE;			// 若是，使对应的Key数组中的值为 FALSE
		KillGLWindow();					// 销毁当前的窗口
		fullscreen = !fullscreen;		// 切换 全屏 / 窗口 模式
		// 重建 OpenGL 窗口(修改)
		if (!CreateGLWindow(TEXT("Hello Color!"), 640, 480, 16, fullscreen)) {
			return 0;					// 如果窗口未能创建，程序退出
		}
	}

	if (keys[VK_UP])
		tran_y += inc_t; keys[VK_UP] = FALSE;
	if (keys[VK_DOWN])
		tran_y -= inc_t; keys[VK_DOWN] = FALSE;
	if (keys[VK_LEFT])
		tran_x -= inc_t; keys[VK_LEFT] = FALSE;
	if (keys[VK_RIGHT])
		tran_x += inc_t; keys[VK_RIGHT] = FALSE;
	if (keys['I'])
		tran_z -= inc_t; keys['I'] = FALSE;
	if (keys['O'])
		tran_z += inc_t; keys['O'] = FALSE;

	if (keys['A'])
		rota_y += inc_r; keys['A'] = FALSE;
	if (keys['D'])
		rota_y -= inc_r; keys['D'] = FALSE;
	if (keys['W'])
		rota_x += inc_r; keys['W'] = FALSE;
	if (keys['S'])
		rota_x -= inc_r; keys['S'] = FALSE;
	if (keys['Y'])
		rota_z += inc_r; keys['Y'] = FALSE;
	if (keys['U'])
		rota_z -= inc_r; keys['U'] = FALSE;

	return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//   当前窗口实例	      前一个窗口实例       命令行参数     窗口显示状态
{
	// MessageBox(NULL, TEXT("Hello OpenGL"), TEXT("Hello"), MB_YESNO | MB_OKCANCEL | MB_ICONINFORMATION);
	//		没有实际窗口对象  正文信息			标题			YES NO     OK   CANCEL     感叹号    下面的QUESTION是问好
	MSG	msg;							// Windowsx消息结构
	bool done = FALSE;					// 用来退出循环的Bool 变量
	// 提示用户选择运行模式
	// if (MessageBox(NULL, TEXT("你想在全屏模式下运行么？"), TEXT("设置全屏模式"), MB_YESNO | MB_ICONQUESTION) == IDNO) {
	fullscreen = FALSE;				// FALSE为窗口模式
	// }
	// 创建OpenGL窗口
	if (!CreateGLWindow(TEXT("Hello OpenGL!"), 640, 480, 16, fullscreen)) {
		return 0;						// 失败退出
	}
	while (!done)  {									 // 保持循环直到 done=TRUE
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {  // 有消息在等待吗?
			if (msg.message == WM_QUIT) {				 // 收到退出消息?
				done = TRUE;							 // 是，则done=TRUE
			}
			else {									 // 不是，处理窗口消息
				TranslateMessage(&msg);					 // 翻译消息
				DispatchMessage(&msg);					 // 发送消息
			}
		}
		else {										 // 如果没有消息
			// 绘制场景。监视ESC键和来自DrawGLScene()的退出消息
			if (active) {								 // 程序激活的么?
				if (keys[VK_ESCAPE]) {					 // ESC 按下了么?
					done = TRUE;						 // ESC 发出退出信号
				}
				else {						// 不是退出的时候，刷新屏幕
					DrawGLScene();				// 绘制场景
					SwapBuffers(hDC);			// 交换缓存 (双缓存)
				}
			}
			if (keys[VK_F1]) {					// F1键按下了么?
				keys[VK_F1] = FALSE;			// 若是，使对应的Key数组中的值为 FALSE
				KillGLWindow();					// 销毁当前的窗口
				fullscreen = !fullscreen;		// 切换 全屏 / 窗口 模式
				// 重建 OpenGL 窗口
				if (!CreateGLWindow(TEXT("Hello OpenGL!"), 640, 480, 16, fullscreen)) {
					return 0;					// 如果窗口未能创建，程序退出
				}
			}
		}
	}
	// 关闭程序
	KillGLWindow();								// 销毁窗口
	return (msg.wParam);						// 退出程序
}