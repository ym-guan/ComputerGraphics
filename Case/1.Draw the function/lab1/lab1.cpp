#include <GL/glut.h>
#include <math.h>
const int screenWidth = 640;
const int screenHeight = 480;
GLdouble A, B, C, D;
void myInit() {//改函数用来设置坐标系点大小等
	glClearColor(1.0, 1.0, 1.0, 0.0);	//背景颜色为白
	glColor3f(0.0f, 0.0f, 0.0f);		//画图颜色为黑
	glPointSize(2.0);					//点的大小为2×2像素
	glMatrixMode(GL_PROJECTION);		//设置投影矩阵
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)screenWidth, 0.0, (GLdouble)screenHeight);
	//设置比例变换和平移值
	A = screenWidth / 40.0;
	B = 20.0 * A;
	C = screenHeight / 2.0;
	D = C;
}

void myDisplay()//回调函数
{
	glClear(GL_COLOR_BUFFER_BIT);//清空色彩缓存区,清屏
	glBegin(GL_POINTS);
	for (GLdouble x = -20.0;x < 20.0;x += 0.005) {
		GLdouble func = sin(x) / x;
		glVertex2d(A * x + B, C * func + D);
	}
	glEnd();
	glFlush();//送所有输出到显示设备
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);//初始化glut  （可不写默认）
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//显示模式初始化 RGB显示模式，单缓冲模式
	glutInitWindowPosition(100, 100);//窗口窗口位置  （可不写默认）
	glutInitWindowSize(640, 480);//窗口大小  （可不写默认）
	glutCreateWindow("Draw the function");//窗口名称
	glutDisplayFunc(myDisplay);//自定义函数绘图
	myInit();
	glutMainLoop();//消息循环相应
	return 0;
}

