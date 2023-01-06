// lab09.cpp : Mesh
#include <GL/glut.h>
#include <GL/GL.h>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

const int screenWidth = 960;
const int screenHeight = 640;
const int left = 480;
const int right = 480;
const int bottom = 480;
const int top = 480;
const int near = 480;
const int far = 480;

struct Point3 {
	GLdouble x = 0.0, y = 0.0, z = 0.0;
};
struct Vector3 {
	GLdouble x = 0.0, y = 0.0, z = 0.0;
};
class VertexID {
public:
	int VertIndex;//此顶点在顶点列表中的索引
	int normIndex;//顶点的法线索引
};
class Face {
public:
	int nVerts;//面片中的顶点数量
	VertexID* vert;//顶点和法线索引组成的列表
	Face() { nVerts = 0;vert = NULL; }
	~Face() { delete[]vert;nVerts = 0; }
};
class Mesh {
private:
	int numVerts;//网格中的顶点数
	Point3* pt;//3D顶点组成的数组
	int numNormals;//网格中的法线数目
	Vector3* norm;//法线数组
	int numFaces;//网格中的面片数
	Face* face;//面片列表
public:
	Mesh() { numVerts = 0;pt = NULL;numNormals = 0;norm = NULL;numFaces = 0;face = NULL; }
	~Mesh() { delete[]pt;delete[]norm;delete[]face; }
	int readMesh(char* fileName);
	void draw();
};
int Mesh::readMesh(char* fileName) {
	fstream infile;
	infile.open(fileName, ios::in);
	if (infile.fail())	return -1;//无法打开文件
	if (infile.eof())	return -1;//文件为空
	infile >> numVerts >> numNormals >> numFaces;
	pt = new Point3[numVerts];
	norm = new Vector3[numNormals];
	face = new Face[numFaces];
	//检查内存是否充足
	if (!pt || !norm || !face)	return -1;//内存分配失败
	for (int p = 0;p < numVerts;p++) {//读取顶点
		infile >> pt[p].x >> pt[p].y >> pt[p].z;
	}
	for (int n = 0;n < numNormals;n++) {//读取法线
		infile >> norm[n].x >> norm[n].y >> norm[n].z;
	}
	for (int f = 0;f < numFaces;f++) {//读取面片
		infile >> face[f].nVerts;
		face[f].vert = new VertexID[face[f].nVerts];
		for (int i = 0;i < face[f].nVerts;i++) {
			infile >> face[f].vert[i].VertIndex;
		}
		for (int i = 0;i < face[f].nVerts;i++) {
			infile >> face[f].vert[i].normIndex;
		}
	}
}
void Mesh::draw() {
	for (int f = 0;f < numFaces;f++) {//绘制每个面片
		glBegin(GL_LINE_LOOP);
		for (int v = 0;v < face[f].nVerts;v++) {//遍历每个顶点
			int in = face[f].vert[v].normIndex;//法线索引
			int iv = face[f].vert[v].VertIndex;//顶点索引
			glNormal3f(norm[in].x, norm[in].y, norm[in].z);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

void axis(double length) {//画z轴
	glPushMatrix();
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);glVertex3d(0, 0, length);
	glEnd();
	glTranslated(0, 0, length - 0.2);
	glutWireCone(0.04, 0.2, 12, 9);
	glPopMatrix();
}
void myDisplay()//回调显示函数
{
	glClear(GL_COLOR_BUFFER_BIT);//清空色彩缓存区,清屏
	glColor3d(0, 0, 0);
	axis(3);					//x-axis
	glPushMatrix();
	glRotated(90, 0, 1.0, 0);	//旋转x轴
	axis(3);					//y-axis
	glRotated(-90.0, 1, 0, 0);
	axis(3);					//z-axis
	glPopMatrix();

	char filepath[] = "D:\\Dev\\Workspace\\ComputerGraphProject\\lab9.txt";
	Mesh mesh;
	mesh.readMesh(filepath);
	mesh.draw();
	glFlush();//送所有输出到显示设备
}

void myInit() {//改函数用来设置坐标系点大小等
	glClearColor(1.0, 1.0, 1.0, 0.0);	//背景颜色为白
	glColor3f(0.0f, 0.0f, 0.0f);		//画图颜色为黑
	glMatrixMode(GL_PROJECTION);		//设置投影矩阵为当前矩阵
	glLoadIdentity();					//设置为单位阵
	glOrtho(-3.2, 3.2, -2.4, 2.4, 1, 50);	//乘以新矩阵

	glMatrixMode(GL_MODELVIEW);		//设置安放摄影机,即观察视角
	glLoadIdentity();					//设置为单位阵
	gluLookAt(2, 3, 4, 0, 1, 0, 0, 1, 0);	//乘以新矩阵
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);//初始化glut  （可不写默认）
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//显示模式初始化 RGB显示模式，单缓冲模式
	glutInitWindowPosition(100, 100);//窗口窗口位置  （可不写默认）
	glutInitWindowSize(screenWidth, screenHeight);//窗口大小  （可不写默认）
	glutCreateWindow("Mesh");//窗口名称
	glutDisplayFunc(myDisplay);//自定义函数绘图
	myInit();
	glutMainLoop();//消息循环相应
	return 0;
}

