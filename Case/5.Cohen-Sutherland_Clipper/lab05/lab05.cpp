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
const int NUM = 10;
struct GLdoublePointAndNorm {
	GLdouble x = 0.0, y = 0.0, z = 0.0;
};
struct LineSegment {
	GLdoublePointAndNorm first, second, norm, pt;
};
class LineList {
public:
	int num = 0;
	vector<LineSegment> lineArray;
};

LineList L;
LineSegment randLineArray[NUM];//随机生成线段
vector<GLdoublePointAndNorm> linePolygon;//生成多边形的端点

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
	glLineWidth(2.0);
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
void displayRandLines() {//随机生成线段
	for (int i = 0;i < NUM;i++) {
		GLdoublePointAndNorm p1, p2;
		p1.x = rand() % 1280;p1.y = rand() % 960;
		p2.x = rand() % 1280;p2.y = rand() % 960;
		p1.z = 1.0;p2.z = 1.0;
		randLineArray[i].first = p1;randLineArray[i].second = p2;
		drawLineBlack(p1.x, p1.y, p2.x, p2.y);
	}
}
void drawPolygon(vector<GLdoublePointAndNorm>& linePolygon,bool isclosed) {//画多边形
	glColor3f(0.0f, 0.0f, 0.0f);
	if(isclosed)
		glBegin(GL_LINE_LOOP);
	if(!isclosed)
		glBegin(GL_LINE_STRIP);
	for (int i = 0;i < linePolygon.size();i++) {
		glVertex2i(linePolygon[i].x, linePolygon[i].y);
	}
	glEnd();
	glFlush();
}
double	dot(GLdoublePointAndNorm a, GLdoublePointAndNorm b) {//定义叉乘运算
	//向量z都为0故不写
	return a.x * b.x + a.y * b.y;
}
int chopCI(double numer, double denom, double& tIn, double& tOut) {
	double tHit;
	if (denom < 0) {//射线是射入的
		tHit = numer / denom;
		if (tHit > tOut)	return 0;//提前退出
		else if (tHit > tIn)	tIn = tHit;//选择较大的
	}
	else if (denom > 0) {//射线是射出的
		tHit = numer / denom;
		if (tHit < tIn)	return 0;//提前退出
		else if (tHit < tOut)	tOut = tHit;//选择较大的
	}
	else {//denom为0,射线与直线平行
		if (numer <= 0)	return 0;//在外半空间
	}
	return 1;//候选区间非空
}
int CyrusBeckClip(LineSegment& seg, LineList& L) {
	double numer, denom;
	double tIn = 0.0, tOut = 1.0;
	GLdoublePointAndNorm c, tmp;//c是seg的向量，tmp是边上一点与射线一点的差
	c.x = seg.second.x - seg.first.x;
	c.y = seg.second.y - seg.first.y;
	c.z = 0;
	for (int i = 0; i < L.num; i++)
	{
		tmp.x = L.lineArray[i].second.x - seg.first.x;
		tmp.y = L.lineArray[i].second.y - seg.first.y;
		tmp.z = 0;
		numer = dot(L.lineArray[i].norm, tmp);
		denom = dot(L.lineArray[i].norm, c);
		if (!chopCI(numer, denom, tIn, tOut))	return 0;//提前退出
	}
	if (tIn >= tOut) {
		return 0;
	}
	//调整线段的端点，先调整第二个端点
	if (tOut < 1.0) {//改变第二个端点
		seg.second.x = seg.first.x + c.x * tOut;
		seg.second.y = seg.first.y + c.y * tOut;
	}
	if (tIn > 0.0) {//改变第一个端点
		seg.first.x = seg.first.x + c.x * tIn;
		seg.first.y = seg.first.y + c.y * tIn;
	}
	return 1;
}

void myDisplay()//回调显示函数
{
	glClear(GL_COLOR_BUFFER_BIT);//清空色彩缓存区,清屏
	glColor3f(0.9, 0.9, 0.9);
	linePolygon.clear();
	glFlush();//送所有输出到显示设备
}
void displayAll() {//画多边形，随机线段以及裁剪后的线段
	drawPolygon(linePolygon, 1);
	for (int i = 0; i < linePolygon.size(); i++)//把多边形的点坐标转化为LineList中的值
	{
		LineSegment newSeg;
		newSeg.first = linePolygon[i];
		newSeg.second = linePolygon[i + 1 == linePolygon.size() ? 0 : i + 1];
		//逆时针多边形，外向量顺时针正交
		newSeg.norm.x = newSeg.second.y - newSeg.first.y;
		newSeg.norm.y = -(newSeg.second.x - newSeg.first.x);
		newSeg.norm.z = 0;
		L.lineArray.push_back(newSeg);
	}
	L.num = linePolygon.size();
	displayRandLines();
	for (int i = 0; i < NUM; i++)
	{
		if (CyrusBeckClip(randLineArray[i], L)) {//返回1，说明存在候选区间，此时画红线，否则不画
			drawLineRed(randLineArray[i].first.x, randLineArray[i].first.y,
				randLineArray[i].second.x, randLineArray[i].second.y);
		}
	}
}
void myMouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			GLdoublePointAndNorm p;
			p.x = x;p.y = screenHeight - y;p.z = 1;
			linePolygon.push_back(p);
			drawPolygon(linePolygon, 0);
		}
		if (button == GLUT_RIGHT_BUTTON) {
			displayAll();
		}
	}
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
	glutCreateWindow("Cyrus-Beck Clipping");//窗口名称
	glutDisplayFunc(myDisplay);//自定义函数绘图
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	myInit();
	glutMainLoop();//消息循环相应
	return 0;
}

