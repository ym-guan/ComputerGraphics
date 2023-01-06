#include <GL/glut.h>
#include <GL/GL.h>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
using namespace std;
const int screenWidth = 640;
const int screenHeight = 480;

struct GLintPoint {
	GLint x, y;
};
class GLintPointArray {//存储一个点阵对象，每个对象对应一个门或窗，等等
public:
	int num = 0;
	vector<GLintPoint> pt;
};
GLintPoint CP;	//存储当前的位置,用于create
GLintPointArray pointArray[100];

GLintPoint CPMove;  //存储位置，用于move
bool selected = false;		//用来鼠标拖动，标记第一个点是否选中

void lineTo(GLint x, GLint y);
void myInit();
void displayDelete();
void displayMove();
void myMouse(int button, int state, int x, int y);
void drawPolyLine(GLintPointArray poly, int closed);
void myDisplay();
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);
void myMovedMouse(int x, int y);

void lineTo(GLint x, GLint y) {
	glBegin(GL_LINES);
	glVertex2i(CP.x, CP.y);
	glVertex2i(x, y);
	glEnd();
	glFlush();
	CP.x = x;CP.y = y;
}
void myInit() {//改函数用来设置坐标系点大小等
	glClearColor(1.0, 1.0, 1.0, 0.0);	//背景颜色为白
	glColor3f(0.0f, 0.0f, 0.0f);		//画图颜色为黑
	glPointSize(2.0);					//点的大小为2×2像素
	glMatrixMode(GL_PROJECTION);		//设置相机形状
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)screenWidth, 0.0, (GLdouble)screenHeight);
}

void displayDelete() {
	glClear(GL_COLOR_BUFFER_BIT);//清空色彩缓存区,清屏
	for (int i = 0;i < 4;i++) {
		drawPolyLine(pointArray[i], 0);
	}
}

int index_move_i = -1;int index_move_j = -1;
void displayMove() {
	glClear(GL_COLOR_BUFFER_BIT);//清空色彩缓存区,清屏
	if (selected) {
		if (index_move_i != -1 && index_move_j != -1) {
			pointArray[index_move_i].pt[index_move_j].x = CPMove.x;
			pointArray[index_move_i].pt[index_move_j].y = CPMove.y;
		}
		for (int i = 0;i < 4;i++) {
			drawPolyLine(pointArray[i], 0);
		}
	}
}
void myMouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {//create一个
			lineTo(x, screenHeight - y);
			glFlush();
		}
		if (button == GLUT_RIGHT_BUTTON) {//move
			GLdouble Min;
			for (int i = 0; i < 4; i++) {
				for (int j = 0;j < pointArray[i].pt.size();j++) {
					if (abs(pointArray[i].pt[j].x - x) <= 10.0 && abs(pointArray[i].pt[j].y - (screenHeight - y)) <= 10.0) {
						GLdouble distance = (pointArray[i].pt[j].x - x) * (pointArray[i].pt[j].x - x) +
							(pointArray[i].pt[j].y - (screenHeight - y)) * (pointArray[i].pt[j].y - (screenHeight - y));
						Min = min(distance, Min);
						index_move_i = i;index_move_j = j;
					}
				}
			}
			selected = true;
			displayMove();
		}
		if (button == GLUT_MIDDLE_BUTTON) {//删除
			GLdouble Min;int index_i = -1;int index_j = -1;
			for (int i = 0; i < 4; i++){
				for (int j = 0;j < pointArray[i].pt.size();j++) {
					if (abs(pointArray[i].pt[j].x - x) <= 10.0 && abs(pointArray[i].pt[j].y - (screenHeight - y)) <= 10.0) {
						GLdouble distance = (pointArray[i].pt[j].x - x) * (pointArray[i].pt[j].x - x) +
							(pointArray[i].pt[j].y - (screenHeight - y)) * (pointArray[i].pt[j].y - (screenHeight - y));
						Min = min(distance, Min);
						index_i = i;index_j = j;
					}
				}
			}
			if (index_i != -1 && index_j != -1) {
				pointArray[index_i].pt.erase(pointArray[index_i].pt.begin() + index_j);
			}
			displayDelete();
			glFlush();
		}
	}
	return;
}

void myMovedMouse(int x,int y) {
	CPMove.x = x;
	CPMove.y = screenHeight - y;
	displayMove();
}
void drawPolyLine(GLintPointArray poly, int closed) {
	if (closed)
		glBegin(GL_LINE_LOOP);
	else
		glBegin(GL_LINE_STRIP);
	for (int i = 0; i < poly.pt.size(); i++)
	{
		glVertex2i(poly.pt[i].x, poly.pt[i].y);
	}
	glEnd();
	glFlush();
}

void myDisplay()//回调显示函数
{
	glClear(GL_COLOR_BUFFER_BIT);//清空色彩缓存区,清屏
	pointArray[0].pt.clear();
	pointArray[1].pt.clear();
	pointArray[2].pt.clear();
	pointArray[3].pt.clear();

	GLintPoint p;
	p.x = 120; p.y = 120;
	pointArray[0].pt.push_back(p);
	p.x = 120; p.y = 270;
	pointArray[0].pt.push_back(p);
	p.x = 210; p.y = 360;
	pointArray[0].pt.push_back(p);
	p.x = 300; p.y = 270;
	pointArray[0].pt.push_back(p);
	p.x = 300; p.y = 120;
	pointArray[0].pt.push_back(p);
	p.x = 120; p.y = 120;
	pointArray[0].pt.push_back(p);
	drawPolyLine(pointArray[0], 0);
	//烟囱
	p.x = 150;p.y = 300;
	pointArray[1].pt.push_back(p);
	p.x = 150;p.y = 360;
	pointArray[1].pt.push_back(p);
	p.x = 180;p.y = 360;
	pointArray[1].pt.push_back(p);
	p.x = 180;p.y = 330;
	pointArray[1].pt.push_back(p);
	p.x = 150;p.y = 300;
	pointArray[1].pt.push_back(p);
	drawPolyLine(pointArray[1], 0);

	p.x = 230;p.y = 230;
	pointArray[2].pt.push_back(p);
	p.x = 230;p.y = 260;
	pointArray[2].pt.push_back(p);
	p.x = 260;p.y = 260;
	pointArray[2].pt.push_back(p);
	p.x = 260;p.y = 230;
	pointArray[2].pt.push_back(p);
	p.x = 230;p.y = 230;
	pointArray[2].pt.push_back(p);
	drawPolyLine(pointArray[2], 0);

	p.x = 150;p.y = 120;
	pointArray[3].pt.push_back(p);
	p.x = 150;p.y = 240;
	pointArray[3].pt.push_back(p);
	p.x = 180;p.y = 240;
	pointArray[3].pt.push_back(p);
	drawPolyLine(pointArray[3], 0);
	CP.x = p.x;
	CP.y = p.y;
	glFlush();//送所有输出到显示设备
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
	GLint x = mouseX;
	GLint y = screenHeight - mouseY;
	switch (theKey) {
	case 'r':
		myDisplay();
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
	glutInitWindowSize(640, 480);//窗口大小  （可不写默认）
	glutCreateWindow("Polyline Editor");//窗口名称
	glutDisplayFunc(myDisplay);//自定义函数绘图
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);
	glutMotionFunc(myMovedMouse);
	myInit();
	glutMainLoop();//消息循环相应
	return 0;
}

