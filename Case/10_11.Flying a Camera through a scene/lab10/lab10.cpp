// lab10_11.cpp : “Flying” a Camera through a scene-1
#include <GL/glut.h>
#include <GL/GL.h>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
using namespace std;

const int screenWidth = 960;
const int screenHeight = 640;

class Point3 {
public:
	GLdouble x, y, z;
	Point3() { x = 0.0; y = 0.0;z = 0.0; }
	Point3(GLdouble x, GLdouble y, GLdouble z) { this->x = x;this->y = y;this->z = z; }
	void set(Point3 p) { this->x = p.x;this->y = p.y;this->z = p.z; }
	void set(GLdouble x, GLdouble y, GLdouble z) { this->x = x;this->y = y;this->z = z; }
};
class Vector3 {
public:
	GLdouble x, y, z;
	Vector3() { x = 0.0; y = 0.0;z = 0.0; }
	Vector3(GLdouble x, GLdouble y, GLdouble z) { this->x = x;this->y = y;this->z = z; }
	void set(Vector3 p) { this->x = p.x;this->y = p.y;this->z = p.z; }
	void set(GLdouble x, GLdouble y, GLdouble z) { this->x = x;this->y = y;this->z = z; }
	double dot(Vector3 vec) { return this->x * vec.x + this->y * vec.y + this->z * vec.z; }
	Vector3 cross(Vector3 vec) {//叉积
		Vector3 newVec;
		newVec.x = this->y * vec.z - this->z * vec.y;
		newVec.y = this->x * vec.z - this->z * vec.x;
		newVec.z = this->x * vec.y - this->y * vec.x;
		return newVec;
	}
	void normalize() {//单位化
		double t = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
		this->x = this->x / t;
		this->y = this->y / t;
		this->z = this->z / t;
	}
};
//物体类
class Item {
public:
	GLdouble x, y, z;
	double xyangle, yzangle, xzangle;
	Item() { this->x = 0.0;this->y = 0.0;this->z = 0.0;
	this->xyangle = 0.0;this->xzangle = 0.0;this->yzangle = 0.0;}
	Item(GLdouble x, GLdouble y, GLdouble z) {
		this->x = x;this->y = y;this->z = z; 
		this->xyangle = 0.0;this->yzangle = 0.0;this->xzangle = 0.0;
	}
	void slide(double dx, double dy, double dz) {//平移
		this->x = this->x + dx;
		this->y = this->y + dy;
		this->z = this->z + dz;
	}
	void rotate(double xyangle, double yzangle, double xzangle) {//旋转
		this->xyangle = this->xyangle + xyangle;
		this->yzangle = this->yzangle + yzangle;
		this->xzangle = this->xzangle + xzangle;
	}
	~Item() {}
};
//相机类
class Camera {
private:
	Point3 eye, look;
	Vector3 up;
	Vector3 u, v, n;
	double viewAngle, aspect, nearDist, farDist;
	void setModelViewMatrix();
public:
	Camera() {};
	Camera(Point3 eye,Point3 look,Vector3 up) {
		this->eye = eye;this->look = look;this->up = up;
	}
	void set(Point3 eye, Point3 look, Vector3 up);
	void roll(float angle);
	void pitch(float angle);
	void yaw(float angle);
	void slide(float du, float dv, float dn);
	void setShape(float viewAngle, float aspect, float Near, float Far);
	//void getShape(float& viewAngle, float& aspect, float& Near, float& Far);
	~Camera() {};
};
void Camera::setModelViewMatrix() {//处理从世界坐标到摄像机坐标的转换矩阵V
	float m[16];
	Vector3 eVec(eye.x, eye.y, eye.z);
	m[0] = u.x;	m[4] = u.y;	m[8] = u.z;		m[12] = -eVec.dot(u);
	m[1] = v.x;	m[5] = v.y;	m[9] = v.z;		m[13] = -eVec.dot(v);
	m[2] = n.x;	m[6] = n.y;	m[10] = n.z;	m[14] = -eVec.dot(n);
	m[3] = 0;	m[7] = 0;	m[11] = 0;		m[15] = 1.0;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m);
}
void Camera::set(Point3 Eye, Point3 look, Vector3 up) {
	//make u,v,n vectors
	eye.set(Eye);
	n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z);
	u.set(up.cross(n));
	v.set(n.cross(u));
	u.normalize();v.normalize();n.normalize();
	setModelViewMatrix();
}
void Camera::slide(float du, float dv, float dn) {//平移摄像机
	eye.x += du * u.x + dv * v.x + dn * n.x;
	eye.y += du * u.y + dv * v.y + dn * n.y;
	eye.z += du * u.z + dv * v.z + dn * n.z;
	look.x += du * u.x + dv * v.x + dn * n.x;
	look.y += du * u.y + dv * v.y + dn * n.y;
	look.z += du * u.z + dv * v.z + dn * n.z;
	setModelViewMatrix();
}
void Camera::roll(float angle) {//旋转摄像机，n轴不动，u,v旋转
	float cs = cos(3.14159265 / 180 * angle);
	float sn = sin(3.14159265 / 180 * angle);
	Vector3 t(u);
	u.set(cs * t.x + sn * v.x, cs * t.y + sn * v.y, cs * t.z + sn * v.z);
	v.set(-sn * t.x + cs * v.x, -sn * t.y + cs * v.y, -sn * t.z + cs * v.z);
	setModelViewMatrix();
}
void Camera::pitch(float angle) {//倾斜摄像机，u轴不动，n,v旋转,倾斜为正摄像机向上翘
	float cs = cos(3.14159265 / 180 * angle);
	float sn = sin(3.14159265 / 180 * angle);
	Vector3 t(v);
	v.set(cs * t.x + sn * n.x, cs * t.y + sn * n.y, cs * t.z + sn * n.z);
	n.set(-sn * t.x + cs * n.x, -sn * t.y + cs * n.y, -sn * t.z + cs * n.z);
	setModelViewMatrix();
}
void Camera::yaw(float angle) {//偏移摄像机，v轴不动，u,n旋转,偏移为正摄像机向左偏
	float cs = cos(3.14159265 / 180 * angle);
	float sn = sin(3.14159265 / 180 * angle);
	Vector3 t(n);
	n.set(cs * t.x + sn * u.x, cs * t.y + sn * u.y, cs * t.z + sn * u.z);
	u.set(-sn * t.x + cs * u.x, -sn * t.y + cs * u.y, -sn * t.z + cs * u.z);
	setModelViewMatrix();
}
void Camera::setShape(float viewAngle, float aspect, float Near, float Far) {
	glMatrixMode(GL_PROJECTION);		//设置投影矩阵为当前矩阵
	glLoadIdentity();					//设置为单位阵
	gluPerspective(viewAngle, aspect, Near, Far);//对投影矩阵进行透视变换
}

Point3 eye(0, 0, 10);Point3 look(0, 0, 0);Vector3 up(0, 1, 0);
Camera cam;
Item item1(-1, 0, 0), item2(1, 0, 0);

void myKeyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	//整体
	case 'l':	cam.slide(-0.2, 0, 0);break;			//左移
	case 'l' - 96:	cam.slide(0.2, 0, 0);break;			//ctrl+L，右移
	case 'u':	cam.slide(0, 0.2, 0);break;				//上移
	case 'u' - 96:	cam.slide(0, -0.2, 0);break;		//下移
	case 'f':	cam.slide(0, 0, 0.2);break;				//forward
	case 'f' - 96:	cam.slide(0, 0, -0.2);break;		//back
	case 'p':	cam.pitch(-1.0);break;
	case 'p' - 96:	cam.pitch(1.0);break;
	case 'y':	cam.yaw(-1.0);break;
	case 'y' - 96:	cam.yaw(1.0);break;
	case 'r':	cam.roll(1.0);break;					//右转
	case 'r' - 96:	cam.roll(-1.0);break;				//左转
	//茶壶1
	case 'q':	item1.slide(-0.1, 0, 0);break;			//左移
	case 'q' - 96:	item1.slide(0.1, 0, 0);break;		//右移
	case 'w':	item1.slide(0.0, 0.1, 0);break;			//上移
	case 'w' - 96:	item1.slide(0.0, -0.1, 0);break;	//下移
	case 'e':	item1.slide(0.0, 0.0, 0.1);break;		//前移
	case 'e' - 96:	item1.slide(0.0, 0.0, -0.1);break;	//后移
	case 'a':	item1.rotate(0.5, 0, 0);break;			//绕z旋转
	case 'a' - 96:	item1.rotate(-0.5, 0, 0);break;		//
	case 's':	item1.rotate(0, 0.5, 0);break;			//绕x旋转
	case 's' - 96:	item1.rotate(0, -0.5, 0);break;		//
	case 'd':	item1.rotate(0, 0, 0.5);break;			//绕y旋转
	case 'd' - 96:	item1.rotate(0, 0, -0.5);break;		//
	//茶壶2
	case 'h':	item2.slide(-0.1, 0, 0);break;			//左移
	case 'h' - 96:	item2.slide(0.1, 0, 0);break;		//右移
	case 'j':	item2.slide(0.0, 0.1, 0);break;			//上移
	case 'j' - 96:	item2.slide(0.0, -0.1, 0);break;	//下移
	case 'k':	item2.slide(0.0, 0.0, 0.1);break;		//前移
	case 'k' - 96:	item2.slide(0.0, 0.0, -0.1);break;	//后移
	case 'b':	item2.rotate(0.5, 0, 0);break;			//绕z旋转
	case 'b' - 96:	item2.rotate(-0.5, 0, 0);break;		//
	case 'n':	item2.rotate(0, 0.5, 0);break;			//绕x旋转
	case 'n' - 96:	item2.rotate(0, -0.5, 0);break;		//
	case 'm':	item2.rotate(0, 0, 0.5);break;			//绕y旋转
	case 'm' - 96:	item2.rotate(0, 0, -0.5);break;		//
	default:
		break;
	}
	glutPostRedisplay();	//draw it again
}
void myDisplay()//回调显示函数
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清空色彩缓存区,清屏
	glColor3f(0.0f, 0.0f, 0.0f);
	//画茶壶
	glPushMatrix();//push pop很重要！
	glTranslatef(item1.x, item1.y, item1.z);
	glRotatef(item1.xyangle, 0, 0, 1);
	glRotatef(item1.yzangle, 1, 0, 0);
	glRotatef(item1.xzangle, 0, 1, 0);
	glutWireCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(item2.x, item2.y, item2.z);
	glRotatef(item2.xyangle, 0, 0, 1);
	glRotatef(item2.yzangle, 1, 0, 0);
	glRotatef(item2.xzangle, 0, 1, 0);
	glutWireCube(1.0);
	glPopMatrix();

	//画地面
	glColor3f(0.0f, 1.0f, 0.0f);
	for (float i = -50;i < 50;i += 0.5) {
		glBegin(GL_LINE_LOOP);
		glVertex3d(i, 0, 100);
		glVertex3d(i, 0, -100);
		glEnd();
	}
	for (float i = -50;i < 50;i += 0.5) {
		glBegin(GL_LINE_LOOP);
		glVertex3d(100, 0, i);
		glVertex3d(-100, 0, i);
		glEnd();
	}
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);//初始化glut  （可不写默认）
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);//显示模式初始化 RGB显示模式，单缓冲模式
	glutInitWindowPosition(100, 100);//窗口窗口位置  （可不写默认）
	glutInitWindowSize(screenWidth, screenHeight);//窗口大小  （可不写默认）
	glutCreateWindow("fly a Camera");//窗口名称
	glutDisplayFunc(myDisplay);//自定义函数绘图
	glutKeyboardFunc(myKeyboard);
	//设置
	glClearColor(1.0, 1.0, 1.0, 0.0);	//背景颜色为白
	glColor3f(0.0f, 0.0f, 0.0f);		//画图颜色为黑
	glViewport(0, 0, screenWidth, screenHeight);

	cam.set(eye, look, up);
	cam.setShape(30.0f, 64.0f / 48.0f, 0.5f, 50.0f);//有效

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);

	glutMainLoop();//消息循环相应
	return 0;
}

