#include"msnowflake.h"
#include<iostream>
#include<cmath>
using namespace std;

//gloabal control
float width = 800.0f, height = 450.0f;
point2 shape[11];
const int randpn = 6;
point2 randPos[randpn];
float rotAngCon = 0.0f;
P2NH clickph;
int clickpn = 0;
const colorRGB ccolor = { 0.6f,0.6f,1.0f };
colorRGB color = ccolor;
bool cch = false;
int mode = 1;

void init(void) {
	glClearColor(0.1f, 0.1f, 0.2f,0.0f);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, width, 0.0, height);
	srand((unsigned)time(NULL));
	for (int i = 0; i < randpn; i++) {
		float px = getRandValue(0, width);
		float py = getRandValue(height,height+10);
		randPos[i] = { px,py };
	}
	shapeflake(shape,1.0f);
}

void inline drawflake(const point2* cp) {
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 11; i++)
		glVertex2d(cp[i].x, cp[i].y);
	glEnd();
}

void inline drawSnowflake(const point2 (* snform)[11]) {
	if(mode==0)
		glBegin(GL_LINE_STRIP);
	else if(mode==1)
		glBegin(GL_POLYGON);
	for (int i = 0; i < 12; i++)
		for (int j = 0; j < 11; j++)
			glVertex2d(snform[i][j].x, snform[i][j].y);
	glEnd();
}

void rotateNdraw(const point2* basep,float Ang, point2 &rotCenter, point2(* snform)[11],int up) {
	point2 curp[11];
	//point2 buffer[6][11];
	float sinAng = sin(Ang), cosAng = cos(Ang);
	//float sinAngCon = sin(rotAngCon), cosAngCon = cos(rotAngCon);
	for (int i = 0; i < 11; i++)
		curp[i] = basep[i];
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
				snform[up + i * 2][j] = curp[j];
			else
				snform[up + i * 2][j] = curp[10-j];
	}
	/*for (int i = 0; i < 6; i++)
		drawflake(buffer[i]);*/
}

void drawSnowflake(point2 cp,colorRGB color = { 0.6f,0.6f,1.0f },float anticlockwise = 1.0f) {
	glColor3f(color.red,color.green,color.blue);
	point2 basep[2][11];
	point2 snform[12][11];
	static float rotAng = PI / 3;
	point2 rotCenter;
	//shapeflake(shape);
	rotCenter = { cp.x + shape[10].y / tan(rotAng / 2),cp.y + shape[10].y};

	//left
	for (int i = 0; i < 11; i++) {
		basep[0][i] = { cp.x + shape[i].x, cp.y + shape[i].y };
	}
	//right
	for (int i = 0; i < 10; i++) {
		basep[1][i] = { basep[0][i].x, basep[0][i].y + 2 * shape[10].y };
	}
	basep[1][2].y = basep[1][3].y = basep[1][2].y - 2 * shape[2].y;
	basep[1][6].y = basep[1][7].y = basep[1][6].y - 2 * shape[6].y;
	basep[1][10] = basep[0][10];

	rotateNdraw(basep[0], rotAng, rotCenter, snform, 1);
	rotateNdraw(basep[1], rotAng, rotCenter, snform, 0);
	float sinAngCon = sin(anticlockwise*rotAngCon), cosAngCon = cos(anticlockwise*rotAngCon);
	for(int i = 0;i<12;i++)
		for (int j = 0; j < 11; j++) {
			snform[i][j] = snform[i][j] + (-1 * rotCenter);
			//snform[i][j] = rotateZ(snform[i][j],rotAngCon);
			snform[i][j] = { snform[i][j].x*cosAngCon - snform[i][j].y*sinAngCon, snform[i][j].y*cosAngCon + snform[i][j].x*sinAngCon };
			snform[i][j] = snform[i][j] + rotCenter;
			//rotateZCenter(snform[i][j], rotAngCon, rotCenter);
		}
	drawSnowflake(snform);

}

void snowfalling() {
	for (int i = 0; i < randpn; i++)
		drawSnowflake(randPos[i],color);
	for (P2NP p = clickph.next; p != NULL; p = p->next) {
		drawSnowflake(p->p2,p->clrgb,p->anticlockwise);
	}
	
}

void idleFunc() {
	static float deltx = 0.05f;
	static float deltAng = 0.0030f;
	for (int i = 0; i < randpn; ++i) {
		if (randPos[i].y > -10)
			randPos[i].y -= deltx;
		else
			//randPos[i] = { ((float)rand() / RAND_MAX) * (width-50) + 25,((float)rand() / RAND_MAX) * 20 + (height-20)};
			randPos[i] = { (GLfloat)getRandValue(0, width),(GLfloat)getRandValue(height,height+10) };
	}

	P2NP p = &clickph;
	while (p->next != NULL) {
		if (p->next->p2.y > -20) {
			p->next->p2.y -= deltx;
			p = p->next;
		}
		else {
			P2NP q = p->next;
			p->next = q->next;
			q->next = NULL;
			delete q;
			clickpn--;
			//printf("snowflake disappear\n");
		}
	}
	
	rotAngCon += deltAng;
	if (rotAngCon > 2 * PI)
		rotAngCon -= 2 * PI;
	glutPostRedisplay();
}

void reshape(int w, int h) {
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, w, 0.0, h);
}

void display() {
	printf("FPS = %f\n", CalFrequency());
	glClear(GL_COLOR_BUFFER_BIT);
	snowfalling();
	glFlush();
}

void OnMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		if (clickpn < MAXCLICK) {
			P2NP p = new P2N;
			p->p2 = { (float)x,height-y};
			p->next = clickph.next;
			p->anticlockwise = rand() % 2 ? 1.0f : -1.0f;
			//p->clrgb = { (float)rand() / RAND_MAX,(float)rand() / RAND_MAX ,(float)rand() / RAND_MAX };
			p->clrgb = color;
			clickph.next = p;
			clickpn++;
		}
	}

	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN){
		//
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		//
		glBegin(GL_LINE);
		glVertex2d(0, 0);
		glVertex2d(x, (height-y));
		glEnd();
		glFlush();
	}
}

void OnKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
	case 'c':
		color = cch ? ccolor : colorRGB{1.0f, 1.0f, 1.0f};
		cch = !cch;
		break;
	case 'm':
		mode = !mode;
		break;
	}
}


int main(int argc, char**argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(width,height);
	glutCreateWindow("Snowflake");
	init();
	glutMouseFunc(OnMouse);
	glutKeyboardFunc(OnKeyboard);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idleFunc);
	glutMainLoop();
	return 0;
}