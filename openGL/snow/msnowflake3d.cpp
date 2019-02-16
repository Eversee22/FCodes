#include<iostream>
#include<cmath>
#include"msnowflake.h"
#include"commcamop.h"

using namespace std;

/*global control*/

//
const float origw = 800.f;
float width = origw, height = origw * 9 / 16, Far = 100.0f;

//snow flake shape
point2 shape[11];

//random snowflakes
const int randpn = 30;
point3 randPos[randpn];

//
/*P3NH clickph;
int clickpn;*/ 

//
float deltx = 0.05f; //fall speed
float deltAng = 0.0030f; //rotate speed
float rotAngCon;

//
const float origflakescale = 0.1f;

//paint mode-1:plane,0:line
int mode = 1;

//snowflake color
const colorRGB origcl = { 0.6f,0.6f,1.0f };
colorRGB color = origcl;
bool clched = false;

//timing
float deltaTime = 0.0f;

//light-0:point,1:spot,2:direct,3:ambient
bool el0 = true;
bool el1 = false;
bool el2 = false;
bool el3 = false;

/*global control*/

void init(void) {
	glClearColor(0.1f, 0.1f, 0.2f, 0.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glMatrixMode(GL_PROJECTION);
	//gluOrtho2D(0.0, width, 0.0, height);
	//glOrtho(0.0, width, 0.0, height, 0.0f, -far);

	//initiate the random snowflakes
	srand((unsigned int)time(NULL));
	for (int i = 0; i < randpn; i++) {
		float px = getRandValue(-100.0, 100.0);
		float py = getRandValue(150.0,170.0);
		float pz = getRandValue(-100.0, 100.0);
		randPos[i] = { px,py,pz};
	}
	//shapeflake(shape,1.5f);

}

//void inline drawflake(const point2* cp) {
//	glBegin(GL_LINE_STRIP);
//	for (int i = 0; i < 11; i++)
//		glVertex2d(cp[i].x, cp[i].y);
//	glEnd();
//}

void inline drawSnowflake3d(const point3 (* snform)[11]) {
	//glBegin(GL_LINE_STRIP);
	if(mode == 1)
		glBegin(GL_POLYGON);
	else if(mode == 0) 
		glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 12; i++)
		for (int j = 0; j < 11; j++)
			glVertex3f(snform[i][j].x, snform[i][j].y, snform[i][j].z);
	glEnd();
	
}

void rotateNdraw3d(const point3* basep,float Ang, point3 &rotCenter, point3(* snform)[11],int up) {
	point2 curp[11];
	static float sinAng = sin(Ang), cosAng = cos(Ang);
	//float sinAngCon = sin(rotAngCon), cosAngCon = cos(rotAngCon);
	for (int i = 0; i < 11; i++)
		curp[i] = { basep[i].x,basep[i].y };
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 11; j++) {
			curp[j] = { curp[j].x - rotCenter.x, curp[j].y - rotCenter.y }; //translate
			curp[j] = { curp[j].x*cosAng - curp[j].y*sinAng, curp[j].y*cosAng + curp[j].x*sinAng }; //rotate
			curp[j] = { curp[j].x + rotCenter.x, curp[j].y + rotCenter.y }; //translate back
			//rotateZCenter(curp[j], Ang, rotCenter);
		}
		/*for (int k = 0; k < 11; k++)
			buffer[i][k] = curp[k];*/
		//drawflake(curp);
		//cout << "draw flake over " << i + 1 << endl;
		//printf("draw flake%d over\n", i + 1);
		for (int j = 0; j < 11; j++)
			if (up==0)
				snform[up + i * 2][j] = { curp[j].x,curp[j].y,rotCenter.z };
			else
				snform[up + i * 2][j] = { curp[10 - j].x,curp[10 - j].y,rotCenter.z };
	}
	/*for (int i = 0; i < 6; i++)
		drawflake(buffer[i]);*/
}


/*
* design the flake shape
      W_
    H1//   _ H2
_____//___//__
L1    L2   L3
*/

void drawSnowflake3d(point3 cp,colorRGB color = { 0.6f,0.6f,1.0f },float anticlockwise = 1.0f) {
	glColor3f(color.red,color.green,color.blue);
	//point2 shape[11];
	point3 basep[2][11];
	point3 snform[12][11];
	static float rotAng = PI / 3;
	point3 rotCenter;
	shapeflake(shape, origflakescale*origw/width);
	rotCenter = { cp.x + shape[10].y / tan(rotAng / 2),cp.y + shape[10].y,cp.z };

	//left
	for (int i = 0; i < 11; i++) {
		basep[0][i] = { cp.x + shape[i].x, cp.y + shape[i].y ,cp.z };
	}
	//right
	for (int i = 0; i < 10; i++) {
		basep[1][i] = { basep[0][i].x, basep[0][i].y + 2 * shape[10].y ,basep[0][i].z };
	}
	basep[1][2].y = basep[1][3].y = basep[1][2].y - 2 * shape[2].y;
	basep[1][6].y = basep[1][7].y = basep[1][6].y - 2 * shape[6].y;
	basep[1][10] = basep[0][10];

	rotateNdraw3d(basep[0], rotAng, rotCenter, snform, 1);
	rotateNdraw3d(basep[1], rotAng, rotCenter, snform, 0);
	float sinAngCon = sin(anticlockwise*rotAngCon), cosAngCon = cos(anticlockwise*rotAngCon);
	for(int i = 0;i<12;i++)
		for (int j = 0; j < 11; j++) {
			snform[i][j] = snform[i][j] + (-1 * rotCenter);
			//snform[i][j] = rotateZ(snform[i][j],rotAngCon);
			snform[i][j] = { snform[i][j].x*cosAngCon - snform[i][j].y*sinAngCon, snform[i][j].y*cosAngCon + snform[i][j].x*sinAngCon };
			snform[i][j] = snform[i][j] + rotCenter;
			//rotateZCenter(snform[i][j], rotAngCon, rotCenter);
		}
	drawSnowflake3d(snform);

}

void snowfalling() {
	for (int i = 0; i < randpn; i++)
		drawSnowflake3d(randPos[i],color);
	/*for (P3NP p = clickph.next; p != NULL; p = p->next) {
		drawSnowflake3d(p->p3,p->clrgb,p->anticlockwise);
	}*/
	
}

void idleFunc() {
	
	for (int i = 0; i < randpn; ++i) {
		if (randPos[i].y > -10.0f)
			randPos[i].y -= deltx;
		else
			randPos[i] = {(GLfloat)getRandValue(-100.0,100.0),(GLfloat)getRandValue(150.0,170.0),(GLfloat)getRandValue(-100.0,100.0)};
	}

	//P3NP p = &clickph;
	//while (p->next != NULL) {
	//	if (p->next->p3.y > -20) {
	//		p->next->p3.y -= deltx;
	//		p = p->next;
	//	}
	//	else {
	//		P3NP q = p->next;
	//		p->next = q->next;
	//		q->next = NULL;
	//		delete q;
	//		clickpn--;
	//		//printf("snowflake disappear\n");
	//	}
	//}
	
	rotAngCon += deltAng;
	if (rotAngCon > 2 * PI)
		rotAngCon -= 2 * PI;
	glutPostRedisplay();
}

void display() {
	printf("FPS = %f\n", CalFrequency());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mnwCam.setModelViewMatrix();

	glPushMatrix();
	//sun light
	GLfloat sun_light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f }; 
	GLfloat sun_light_ambient[] = { 0.6f, 0.2f, 0.1f, 1.0f }; 
	GLfloat sun_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
	GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);
	//sun material
	GLfloat sun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };  //定义材质的环境光颜色
	GLfloat sun_mat_diffuse[] = { 0.9f, 0.6f, 0.2f, 1.0f };  //定义材质的漫反射光颜色
	GLfloat sun_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //定义材质的镜面反射光颜色
	GLfloat sun_mat_emission[] = { 0.9f, 0.2f, 0.1f, 1.0f };   //定义材质的辐射广颜色，为偏红色
	GLfloat sun_mat_shininess = 0.0f;
	//glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
	//glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);
	if (el0)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);
	
	//draw sun
	glPushMatrix();
	glTranslatef(0.0f, 100.0f, -10.0f);
	glutSolidSphere(3.0, 64, 32);
	//glColor3f(0.1f, 1.0f, 0.1f);
	
	//spot light
	glm::vec3 position = mnwCam.getPosition();
	glm::vec3 direction = mnwCam.getDirection();
	GLfloat spot_light_position[] = { position.x,position.y,position.z,1.0f };
	GLfloat spot_light_direction[] = { -direction.x,-direction.y,-direction.z };
	GLfloat spot_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat spot_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat spot_light_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
	//spot light material
	//GLfloat spot_mat_specular[] = { 1.0f,1.0f,1.0f,1.0f };
	//GLfloat spot_mat_diffuse[] = { 1.0f, 0.5f, 0.1f, 1.0f };
	//GLfloat spot_mat_shininess = 128.0f;
	glLightfv(GL_LIGHT1, GL_DIFFUSE, spot_light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spot_light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, spot_light_ambient);
	glLightfv(GL_LIGHT1, GL_POSITION, spot_light_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_light_direction);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0f);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 3.0f);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, spot_mat_specular);
	//glMaterialfv(GL_FRONT, GL_AMBIENT, spot_mat_diffuse);
	//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, spot_mat_shininess);
	if (el1)
		glEnable(GL_LIGHT1);
	else
		glDisable(GL_LIGHT1);

	//direction light
	float dir_light_direction[] = { 0.0f,-20.0f,10.0f,0.0f };
	GLfloat dir_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat dir_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat dir_light_ambient[] = { 0.5f,0.5f,0.5f,1.0f };
	glLightfv(GL_LIGHT2, GL_POSITION, dir_light_direction);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, dir_light_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, dir_light_specular);
	glLightfv(GL_LIGHT2, GL_AMBIENT, dir_light_ambient);
	if (el2)
		glEnable(GL_LIGHT2);
	else
		glDisable(GL_LIGHT2);

	//ambient light
	float amb_light_direction[] = { 0.0f,0.0f,0.0f,0.0f };
	GLfloat amb_light_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat amb_light_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat amb_light_ambient[] = { 0.7f,0.7f,0.7f,1.0f };
	//glLightfv(GL_LIGHT3, GL_POSITION, dir_light_direction);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, amb_light_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, amb_light_specular);
	glLightfv(GL_LIGHT3, GL_AMBIENT, amb_light_ambient);
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light_ambient);
	if (el3)
		glEnable(GL_LIGHT3);
	else
		glDisable(GL_LIGHT3);

	//draw ground
	GLfloat ground_mat_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };  //定义材质的环境光颜色
	GLfloat ground_mat_diffuse[] = { 0.1f, 0.8f, 0.5f, 1.0f };  //定义材质的漫反射光颜色
	GLfloat ground_mat_specular[] = { 0.0f, 0.0f, 0.0f, 0.0f };   //定义材质的镜面反射光颜色
	GLfloat ground_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //定义材质的辐射广颜色
	GLfloat ground_mat_shininess = 0.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, ground_mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ground_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ground_mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, ground_mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, ground_mat_shininess);

	glPopMatrix();
	glTranslatef(0.0f, -10.0f, 0.0f);
	glBegin(GL_POLYGON);
	glVertex3d(-100.0f, 0.0f, 100.0f);
	glVertex3d(100.0f, 0.0f, 100.0f);
	glVertex3d(100.0f, 0.0f, -100.0f);
	glVertex3d(-100.0f, 0.0f, -100.0f);
	glEnd();

	//draw cube
	GLfloat cube_mat_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };  //定义材质的环境光颜色
	GLfloat cube_mat_diffuse[] = { 0.1f, 0.5f, 0.9f, 1.0f };  //定义材质的漫反射光颜色
	GLfloat cube_mat_specular[] = { 1.0f, 1.0f, 1.0f, 0.0f };   //定义材质的镜面反射光颜色
	GLfloat cube_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //定义材质的辐射广颜色
	GLfloat cube_mat_shininess = 10.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, cube_mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cube_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, cube_mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, cube_mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, cube_mat_shininess);
	float cubesize = 20.0f;

	//cube1
	glTranslatef(-50.0f, cubesize/2 + 1.0f, -50.0f);
	//glColor3f(1.0f, 1.0f, 0.0f);
	glutSolidCube(cubesize);
	glTranslatef(0.0f, cubesize, 0.0f);
	glutSolidTeapot(cubesize/2);
	glTranslatef(0.0f, -cubesize, 0.0f);

	//cube2
	glTranslatef(100.0f, 0.0f, 0.0f);
	GLfloat cube_mat_diffuse1[] = { 1.0f,1.0f,0.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cube_mat_diffuse1);
	//glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidCube(cubesize);
	glTranslatef(0.0f, cubesize, 0.0f);
	glutSolidTeapot(cubesize / 2);
	glTranslatef(0.0f, -cubesize, 0.0f);

	//cube3
	glTranslatef(0.0f, 0.0f, 100.0f);
	GLfloat cube_mat_diffuse2[] = { 1.0f,0.0f,0.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cube_mat_diffuse2);
	//glColor3f(0.0f, 0.0f, 1.0f);
	glutSolidCube(cubesize);
	glTranslatef(0.0f, cubesize, 0.0f);
	glutSolidTeapot(cubesize / 2);
	glTranslatef(0.0f, -cubesize, 0.0f);

	//cube4
	glTranslatef(-100.0f, 0.0f, 0.0f);
	GLfloat cube_mat_diffuse3[] = { 0.1f,0.1f,0.1f,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cube_mat_diffuse3);
	//glColor3f(1.0f, 0.5f, 0.5f);
	glutSolidCube(cubesize);
	glTranslatef(0.0f, cubesize, 0.0f);
	glutSolidTeapot(cubesize / 2);
	glTranslatef(0.0f, -cubesize, 0.0f);

	//draw snow flake
	GLfloat sf_mat_ambient[] = { 0.5f,0.5f,0.5f,1.0f };
	GLfloat sf_mat_diffuse[] = { color.red,color.green,color.blue,1.0f };
	GLfloat sf_mat_specular[] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat sf_mat_emission[] = { 0.0f,0.0f,0.0f,1.0f };
	GLfloat sf_mat_shininess = 30.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, sf_mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, sf_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sf_mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, sf_mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, sf_mat_shininess);

	/*glRotatef(angle, 0.0f, -1.0f, 0.0f);
	glTranslatef(10.0f, 0.0f, 0.0f);

	glutSolidSphere(3.0, 40, 32);*/
	//mnwCam.setModelViewMatrix();
	glPopMatrix();
	//glScalef(0.05f, 0.05f, 0.05f);
	snowfalling();
	glutSwapBuffers();
}

//void OnMouse(int button, int state, int x, int y)
//{
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
//		if (clickpn < MAXCLICK) {
//			P3NP p = new P3N;
//			p->p3 = { (float)x,height - y,((float)rand()/RAND_MAX)*2+1 };
//			p->next = clickph.next;
//			p->anticlockwise = rand() % 2 ? 1.0f : -1.0f;
//			p->clrgb = { (float)rand() / RAND_MAX,(float)rand() / RAND_MAX ,(float)rand() / RAND_MAX };
//			//p->clrgb = { 0.5f,0.5f,0.9f };
//			clickph.next = p;
//			clickpn++;
//		}
//		//mode = 0;
//	}
//
//	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN){
//		//
//	}
//
//	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
//		//
//		mode = !mode;
//	}
//}

void OnKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
	case 'c':
		color = clched ? origcl : colorRGB{1.0f, 1.0f, 1.0f};
		clched = !clched;
		break;
	case 'R':
		if (deltAng > 0)
			deltAng = 0;
		else
			deltAng = 0.0030f;
		break;
	case 'F':
		if (deltx > 0)
			deltx = 0;
		else
			deltx = 0.05f;
		break;
	case 'm':
		mode = !mode;
		break;
	
	case 32:
		mnwCam.restore();
		break;
	//move
	case 'w':
		mnwCam.processKeyboard(BACKWARD/*,deltaTime*/);
		//Position += Front * velocity;
		break;
	case 's':
		mnwCam.processKeyboard(FORWARD/*,deltaTime*/);
		//Position -= Front * velocity;
		break;
	case 'a':
		mnwCam.processKeyboard(LEFT/*,deltaTime*/);
		//Position -= Right * velocity;
		break;
	case 'd':
		mnwCam.processKeyboard(RIGHT/*deltaTime*/);
		//Position += Right * velocity;
		break;
	//light control
	case '0':
		el0 = !el0;
		break;
	case '1':
		el1 = !el1;
		break;
	case '2':
		el2 = !el2;
		break;
	case '3':
		el3 = !el3;
		break;
	}
}
	
	

void reshape(int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, w, 0.0, h, 0.0, -Far);*/
	width = w;
	height = h;
	mnwCam.setShape(60.0f, (GLfloat)w / (GLfloat)h, 1.0f, 1000.0f);
	mnwCam.setModelViewMatrix();

	
	

}

int main(int argc, char**argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(width,height);
	glutCreateWindow("Snowflake");
	init();
	glutMouseFunc(onMouse);
	glutMotionFunc(onMouseMove);
	glutKeyboardFunc(OnKeyboard);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idleFunc);
	glutMainLoop();
	return 0;
}