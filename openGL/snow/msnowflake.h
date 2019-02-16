#pragma once
#include<GL/freeglut.h>
#include<iostream>
#include<time.h>

#define PI 3.141592653589793
#define MAXCLICK 30

struct point2 {
	GLfloat x;
	GLfloat y;
};


struct point3 {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

struct colorRGB {
	GLfloat red;
	GLfloat green;
	GLfloat blue;
};
typedef struct Point2Node{
	Point2Node() :next(NULL),anticlockwise(1.0f) {}
	point2 p2;
	Point2Node* next;
	float anticlockwise;
	colorRGB clrgb;
}*P2NP,P2N,P2NH;

typedef struct Point3Node {
	Point3Node() :next(NULL), anticlockwise(1.0f) {}
	point3 p3;
	Point3Node* next;
	float anticlockwise;
	colorRGB clrgb;
}*P3NP, P3N, P3NH;


/*
* design the flake shape
W_
H1//   _ H2
_____//___//__
L1    L2   L3
*/
void shapeflake(point2* shape, float scale = 1.0f) {
	GLfloat L1 = 10.0*scale, L2 = 3.0*scale, L3 = 8.0*scale, H1 = 6.0*scale, H2 = 3.0*scale, W = 2.0*scale, Off1 = 4.0*scale, Off2 = 2.0*scale, AngH = 2.0*scale, AngW = 1.2*scale;
	shape[0] = { 0, 0 };
	shape[1] = { L1, 0 };
	shape[2] = { L1 + Off1, H1 };
	shape[3] = { L1 + Off1 + W,H1 };
	shape[4] = { L1 + W, 0 };
	shape[5] = { L1 + W + L2, 0 };
	shape[6] = { L1 + W + L2 + Off2, H2 };
	shape[7] = { L1 + W + L2 + Off2 + W, H2 };
	shape[8] = { L1 + W + L2 + W, 0 };
	shape[9] = { L1 + W + L2 + W + L3, 0 };
	shape[10] = { L1 + W + L2 + W + L3 + AngH, -AngW };
}

point2 inline operator + (const point2 &a, const point2& b) {
	return { a.x + b.x, a.y + b.y };
}

point3 inline operator + (const point3 &a, const point3& b) {
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

point2 inline operator * (float k, const point2& a) {
	return { a.x * k,a.y *k };
}

point3 inline operator * (float k, const point3& a) {
	return { a.x * k,a.y *k, a.z*k };
}

point2 inline rotateZ(const point2& p, float Ang) {
	return { p.x*cos(Ang) - p.y*sin(Ang), p.y*cos(Ang) + p.x*sin(Ang) };
}

void inline rotateZCenter(point2& p, float Ang, point2& center) {
	p = p + -1 * center;
	p = rotateZ(p, Ang);
	p = p + center;
}

double CalFrequency()
{
	static int count;
	static double freq;
	static clock_t last, current;
	double timegap;

	++count;
	/*current = glutGet(GLUT_ELAPSED_TIME);
	if (current - last > 1000) {
	freq = count * 1000 / (current - last);
	last = current;
	count = 0;
	}*/
	if (count <= 50)
		return freq;
	count = 0;
	last = current;
	current = clock();
	timegap = (double)(current - last) / CLK_TCK;
	freq = 50.0 / timegap;
	return freq;
}

inline double getRandValue(double a, double b) {
	return ((double)rand() / RAND_MAX)*(b - a) + a;
}