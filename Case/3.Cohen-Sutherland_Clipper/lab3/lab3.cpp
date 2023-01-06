#include <GL/glut.h>
#include <GL/GL.h>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;
const int screenWidth = 1280;
const int screenHeight = 960;
const int RectWidth = 640;
const int RectHeight = 480;
const int RectX = 320;
const int RectY = 320;
const int NUM = 10;
struct GLintPoint {
	GLint x, y;
};
struct lineEndPoint {
	GLintPoint p1, p2;
};
lineEndPoint lineArray[NUM];
struct RealRect {
	GLint left, right, top, bottom;
};

void myInit();
void myDisplay();
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);
void drawLineBlack(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
void drawLineRed(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
void displayRandLines();

void myInit() {//改函数用来设置坐标系点大小等
	glClearColor(1.0, 1.0, 1.0, 0.0);	//背景颜色为白
	glColor3f(0.0f, 0.0f, 0.0f);		//画图颜色为黑
	glPointSize(2.0);					//点的大小为2×2像素
	glMatrixMode(GL_PROJECTION);		//设置相机形状
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)screenWidth, 0.0, (GLdouble)screenHeight);
}

void drawLineBlack(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2) {
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();
}
void drawLineRed(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2) {
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();
}

void displayRandLines() {
	for (int i = 0;i < NUM;i++) {
		GLintPoint p1, p2;
		p1.x = rand() % 1280;p1.y = rand() % 960;
		p2.x = rand() % 1280;p2.y = rand() % 960;
		lineArray[i].p1 = p1;lineArray[i].p2 = p2;
		drawLineBlack(p1.x, p1.y, p2.x, p2.y);
	}
}

void ChopLine(GLintPoint& p, unsigned char code, float dely, float delx, RealRect W) {
	if (code & 8) {//左边
		p.y += (W.left - p.x) * dely / delx;
		p.x = W.left;
	}
	else if (code & 4) {//上边
		p.x += (W.top - p.y) * delx / dely;
		p.y = W.top;
	}
	else if (code & 2) {//右边
		p.y += (W.right - p.x) * dely / delx;
		p.x = W.right;
	}
	else if (code & 1) {//下边
		p.x += (W.bottom - p.y) * delx / dely;
		p.y = W.bottom;
	}
}
int clipSegment(GLintPoint& p1, GLintPoint& p2, RealRect W) {//算法
	do {
		//位置编码，每次循环都要重编一次
		unsigned char code1 = 0;
		if (p1.x < W.left)	code1 |= 8;
		if (p1.y > W.top)	code1 |= 4;
		if (p1.x > W.right)	code1 |= 2;
		if (p1.y < W.bottom)	code1 |= 1;
		unsigned char code2 = 0;
		if (p2.x < W.left)	code2 |= 8;
		if (p2.y > W.top)	code2 |= 4;
		if (p2.x > W.right)	code2 |= 2;
		if (p2.y < W.bottom)	code2 |= 1;

		if ((code1 | code2) == 0) {//平凡接受
			drawLineRed(p1.x, p1.y, p2.x, p2.y); 
			return 1;
		}
		if ((code1 & code2)) {//平凡拒绝
			return 0;
		}
		GLdouble dely, delx;
		delx = p2.x - p1.x;
		dely = p2.y - p1.y;
		//裁剪
		if(code1|0)
			ChopLine(p1, code1, dely, delx, W);
		else
			ChopLine(p2, code2, dely, delx, W);
	} while (1);
}


void myDisplay()//回调显示函数
{
	glClear(GL_COLOR_BUFFER_BIT);//清空色彩缓存区,清屏
	glColor3f(0.9, 0.9, 0.9);
	glRecti(RectX, RectY, RectX + RectWidth,RectY + RectHeight);
	displayRandLines();
	RealRect W;
	W.left = RectX;W.right = RectX + RectWidth;
	W.bottom = RectY;W.top = RectY + RectHeight;
	for (int i = 0;i < NUM;i++) {
		cout<<clipSegment(lineArray[i].p1, lineArray[i].p2, W);
	}
	glFlush();//送所有输出到显示设备
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
	GLint x = mouseX;
	GLint y = screenHeight - mouseY;
	switch (theKey) {
	case 'r':
		myDisplay();//重绘
		break;
	case 'q':
		exit(-1);
	default:
		break;
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);//初始化glut  （可不写默认）
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//显示模式初始化 RGB显示模式，单缓冲模式
	glutInitWindowPosition(100, 100);//窗口窗口位置  （可不写默认）
	glutInitWindowSize(screenWidth, screenHeight);//窗口大小  （可不写默认）
	glutCreateWindow("Cohen-Sutherland Clipper");//窗口名称
	glutDisplayFunc(myDisplay);//自定义函数绘图
	glutKeyboardFunc(myKeyboard);
	myInit();
	glutMainLoop();//消息循环相应
	return 0;
}

