#include <GL/glut.h>
#include <GL/GL.h>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <Windows.h>
using namespace std;
const int screenWidth = 1280;
const int screenHeight = 960;
struct GLintPoint {
	GLint x = 0;GLint y = 0;
};
class polyGon {
public:
	vector<GLintPoint> polyGonArray;
};
GLintPoint CP;
polyGon polygon1, polygon2, polygon3;


void myInit();
void myDisplay();
float lerp(float a, float b, float t);
GLintPoint Tween(GLintPoint a, GLintPoint b, float t);
void drawTween(vector<GLintPoint>& a, vector<GLintPoint>& b, int n, float t);
void displayInterpolation();
void drawPolyGonPadding(polyGon polygon);
void drawPolyGonLine(polyGon polygon);

void myInit() {//改函数用来设置坐标系点大小等
	glClearColor(1.0, 1.0, 1.0, 0.0);	//背景颜色为白
	glColor3f(0.0f, 0.0f, 0.0f);		//画图颜色为黑
	glPointSize(2.0);					//点的大小为2×2像素
	glMatrixMode(GL_PROJECTION);		//设置相机形状
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)screenWidth, 0.0, (GLdouble)screenHeight);
}
void myDisplay()//回调显示函数
{
	glClear(GL_COLOR_BUFFER_BIT);//清空色彩缓存区,清屏
	polygon1.polyGonArray.clear();
	polygon2.polyGonArray.clear();
	glColor3f(0.9, 0.9, 0.9);
	glutSwapBuffers();
}
void displayInterpolation() {
	float t, delT;
	for (t = 0.0, delT=0.005; ; t+=delT)
	{
		if (t >= 1.0) {
			delT = -delT;
		}
		if (t < 0.0) {
			break;
		}
		drawTween(polygon1.polyGonArray, polygon2.polyGonArray, polygon1.polyGonArray.size(), t);
		glutSwapBuffers();
	}
}

void drawPolyGonPadding(polyGon polygon) {
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < polygon.polyGonArray.size(); i++)
	{
		glVertex2i(polygon.polyGonArray[i].x, polygon.polyGonArray[i].y);
	}
	glEnd();
	glFlush();
}
void drawPolyGonLine(polyGon polygon) {
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < polygon.polyGonArray.size(); i++)
	{
		glVertex2i(polygon.polyGonArray[i].x, polygon.polyGonArray[i].y);
	}
	glEnd();
	glFlush();
}
float lerp(float a, float b, float t) {
	return a + (b - a) * t;
}
GLintPoint Tween(GLintPoint a, GLintPoint b, float t) {
	GLintPoint p;
	p.x = lerp(a.x, b.x, t);
	p.y = lerp(a.y, b.y, t);
	return p;
}
void drawTween(vector<GLintPoint>& a, vector<GLintPoint>& b, int n, float t) {
	for (int i = 0;i < n;i++) {
		GLintPoint p;
		p = Tween(a[i], b[i], t);
		polygon3.polyGonArray.resize(n);
		polygon3.polyGonArray[i].x = p.x;
		polygon3.polyGonArray[i].y = p.y;
	}
	//每次刷新重新画
	glClear(GL_COLOR_BUFFER_BIT);//清空色彩缓存区,清屏
	drawPolyGonPadding(polygon1);
	drawPolyGonPadding(polygon2);
	drawPolyGonLine(polygon3);
}
void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
	GLint x = mouseX;
	GLint y = screenHeight - mouseY;
	switch (theKey) {
	case 'b':
		displayInterpolation();
		break;
	case 'r':
		myDisplay();//重绘
		break;
	case 'q':
		exit(-1);
	default:
		break;
	}
}
void myMouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			GLintPoint p;
			p.x = x;p.y = screenHeight - y;
			polygon1.polyGonArray.push_back(p);
			//每次要画的时候，先交换回去，画图，再交换回来，这样防止黑屏
			glutSwapBuffers();
			drawPolyGonPadding(polygon1);
			glutSwapBuffers();
		}
		if (button == GLUT_RIGHT_BUTTON) {
			GLintPoint p;
			p.x = x;p.y = screenHeight - y;
			polygon2.polyGonArray.push_back(p);
			glutSwapBuffers();
			drawPolyGonPadding(polygon2);
			glutSwapBuffers();
		}
	}
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);//初始化glut  （可不写默认）
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);//显示模式初始化 RGB显示模式，单缓冲模式
	glutInitWindowPosition(100, 100);//窗口窗口位置  （可不写默认）
	glutInitWindowSize(screenWidth, screenHeight);//窗口大小  （可不写默认）
	glutCreateWindow("Animation with tweeing");//窗口名称
	glutDisplayFunc(myDisplay);//自定义函数绘图
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	myInit();
	glutMainLoop();//消息循环相应
	return 0;
}

