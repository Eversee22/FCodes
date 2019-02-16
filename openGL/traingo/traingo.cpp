#include "skybox.h"
#include "commcamop.h"
#include "traingo.h"

//#include <glm/gtc/type_ptr.hpp>

//#include <GLFW/glfw3.h>

//const std::string CWP = "F:/C++/Visual Studio 2017/project/OpenGL/Glut1/";

std::vector<std::string> faces
{
	"right.jpg",
	"left.jpg",
	"top.jpg",
	"bottom.jpg",
	"front.jpg",
	"back.jpg"
};

//
float deltaTime = 1.0f;
LARGE_INTEGER lastFrame, currentFrame,freq;

//
float theta = 0.0f;
float deltTheta = 0.00f; //angle speed

//camera
int camNum = 0; //0:mainCamera,1:train camera

//skybox
bool nolight = false;

void init(void)
{
	/** 用户自定义的初始化过程 */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	//glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	/** 启用纹理 */
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_LIGHTING);

	/** 初始化天空 */
	loadCubemap(CWP+"skybox/", faces);
	//
	gtexId = loadTexture((CWP + "textures/grass.png").c_str(), GL_REPEAT, true);
	//rock.loadModel(CWP + "model/rock/rock.obj");
	//planet.loadModel(CWP + "model/planet/planet.obj");
	//nanosuit.loadModel(CWP + "model/nanosuit/nanosuit.obj");

	QueryPerformanceFrequency(&freq);

	/** 设置摄像机 */
	//mnwCam.setLook();

}

void reshape(int width, int height)
{
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	glViewport(0,0,width, height);
	if(camNum == 0)
		mnwCam.setShape(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 10000.0f);
	else
		trainCam.setShape(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 5000.0f);
	//mnwCam.setModelViewMatrix();
}



void display(void)
{
	/*QueryPerformanceCounter(&currentFrame);
	deltaTime = (currentFrame.QuadPart - lastFrame.QuadPart)*1.0 / freq.QuadPart;
	lastFrame = currentFrame;*/

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	trainCam.rotateY(glm::radians(theta), radius, glm::vec3(radius, 0.0f, -43.0f));
	
	if (camNum == 0) {
		mnwCam.setModelViewMatrix();
	}
	else if (camNum == 1) {
		//trainCam.setLook(320.0f, 0.0f, 45.0f, 0.0f, 0.0f, -1.0f);
		trainCam.setModelViewMatrix();
	}

	drawTrain(radius,theta);

	drawScene();

	//draw sky
	glm::vec3 pos = mnwCam.getPosition();
	createSkyBox(pos.x, pos.y, pos.z, 1000.0, 1000.0, 1000.0);

	glutSwapBuffers();
}

void onKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
	case 32:
		mnwCam.restore();
		break;
		//move
	case 'w':
		mnwCam.processKeyboard(BACKWARD,deltaTime);
		//Position += Front * velocity;
		break;
	case 's':
		mnwCam.processKeyboard(FORWARD,deltaTime);
		//Position -= Front * velocity;
		break;
	case 'a':
		mnwCam.processKeyboard(LEFT,deltaTime);
		//Position -= Right * velocity;
		break;
	case 'd':
		mnwCam.processKeyboard(RIGHT,deltaTime);
		//Position += Right * velocity;
		break;
	case 'u':
		mnwCam.processKeyboard(UP,deltaTime);
		break;
	case 'p':
		mnwCam.processKeyboard(DOWN,deltaTime);
		break;
	case 'v':
		camNum = !camNum;
		break;
	case 'z':
		deltTheta = deltTheta > 0 ? 0 : 8.0f;
		break;
	case '0':
		el0 = !el0;
		break;
	case '1':
		el1 = !el1;
		if (el1)
			glEnable(GL_LIGHT1);
		else
			glDisable(GL_LIGHT1);
		break;
	case '2':
		el2 = !el2;
		if (el2)
			glEnable(GL_LIGHT2);
		else
			glDisable(GL_LIGHT2);
		break;
	case '3':
		if (el3)
			glEnable(GL_LIGHT3);
		else
			glDisable(GL_LIGHT3);
		el3 = !el3;
		break;
	case 'l':
		nolight = !nolight;
		if (nolight)
			glDisable(GL_LIGHTING);
		else
			glEnable(GL_LIGHTING);
		break;
	}
	glutPostRedisplay();
}

void idleFunc() {
	
	QueryPerformanceCounter(&currentFrame);
	deltaTime = (currentFrame.QuadPart - lastFrame.QuadPart)*1.0 / freq.QuadPart;
	lastFrame = currentFrame;
	theta += deltTheta * deltaTime;
	glutPostRedisplay();
	printf("deltaTime:%f\n", deltaTime);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutInitWindowSize(800, 600);
	//glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) >> 1) - 400, (GetSystemMetrics(SM_CYSCREEN) >> 1) - 300);
	glutCreateWindow("Skybox");
	init();
	glutDisplayFunc(display);
	glutIdleFunc(idleFunc);
	glutReshapeFunc(reshape);
	glutMouseFunc(onMouse);
	glutMotionFunc(onMouseMove);
	glutKeyboardFunc(onKeyboard);
	
	glutMainLoop();
	return 0;
}