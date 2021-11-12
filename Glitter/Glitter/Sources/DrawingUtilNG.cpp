#include <stdio.h>
#include <algorithm>    // std::max
//#include "GL\glut.h"

#include "fssimplewindow.h"
//#include "GraphicFont.h"

#include "DrawingUtilNG.h"


using namespace DrawingUtilNG;

float DrawingUtilNG::vectorLength(vertexF v0)
{
	return sqrt(v0.x * v0.x + v0.y * v0.y + v0.z * v0.z);
}

vertexF DrawingUtilNG::getUnitVector(vertexF v0)
{
	return getUnitVector({ 0., 0., 0. }, v0);
}

vertexF DrawingUtilNG::getUnitVector(vertexF v1, vertexF v2)
{
	vertexF vector = { (v2.x - v1.x) , (v2.y - v1.y) , (v2.z - v1.z) };

	float length = vectorLength(vector);

	vector = { vector.x / length, vector.y / length, vector.z / length };

	return vector;
}

vertexF DrawingUtilNG::vectorSum(vertexF v1, vertexF v2)
{
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

vertexF DrawingUtilNG::vectorSum(vertexF v1, vertexF v2, vertexF v3)
{
	return { v1.x + v2.x + v3.x, v1.y + v2.y + v3.y, v1.z + v2.z + v3.z };
}

vertexF DrawingUtilNG::scalarProduct(vertexF v0, float scalar)
{
	return { v0.x * scalar, v0.y * scalar, v0.z * scalar };
}

float DrawingUtilNG::dotProduct(vertexF v1, vertexF v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vertexF DrawingUtilNG::crossProduct(vertexF v1, vertexF v2)
{
	return {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

vertexF DrawingUtilNG::rotateVector(vertexF v, vertexF k, float theta)
{
	double cosTheta = cos(theta * atan(1.) / 45.);
	double sinTheta = sin(theta * atan(1.) / 45.);

	// make sure k is a unit vector
	k = getUnitVector(k);

	// using Rodrigues' Rotation Formula
	vertexF firstTerm = scalarProduct(v, cosTheta);
	vertexF secondTerm = scalarProduct(crossProduct(k, v), sinTheta);
	vertexF thirdTerm = scalarProduct(k, dotProduct(k, v) * (1 - cosTheta));

	return vectorSum(firstTerm, secondTerm, thirdTerm);
}

void DrawingUtilNG::drawCircle(double centerX, double centerY,
	double radius, bool filled)
{
	float x, y, angle;
	float radianConvert = atan(1.) / 45.;
	radius = fabs(radius);

	// adapt the number of segments based on radius size
	int stepSize = 1;
	if (radius < 10)
		stepSize = 3;
	else if (radius < 200)
		stepSize = round((3. - 1.) / (10. - 200.) * (radius - 200.) + 1.);

	stepSize *= 6;  // always want stepSize to be a factor of 360

	if (filled)
		glBegin(GL_POLYGON);
	else
		glBegin(GL_LINE_LOOP);

	for (int i = 0; i < 360; i += stepSize) {
		angle = i * radianConvert;
		x = cos(angle) * radius + centerX;
		y = sin(angle) * radius + centerY;
		glVertex2d(x, y);
	}
	glEnd();
}

void DrawingUtilNG::drawRectangle(double x, double y, int sizeX, int sizeY, bool filled)
{
	if (filled)
		glBegin(GL_POLYGON);
	else
		glBegin(GL_LINE_LOOP);

	glVertex2i(x, y);
	glVertex2i(x + sizeX, y);
	glVertex2i(x + sizeX, y + sizeY);
	glVertex2i(x, y + sizeY);

	glEnd();
}


bool DrawingUtilNG::hsv2rgb(double H, double S, double V, double& red, double& green, double& blue)
{
	// using formula from:
	// https://www.rapidtables.com/convert/color/hsv-to-rgb.html

	// adjust H to be 0 <= H <= 360
	H = fabs(fmod(H, 360.));  // note that H=360 is the same as H=0

	double C = V * S;
	double X = C * (1 - fabs(fmod(H / 60., 2) - 1));
	double m = V - C;
	double Rprime, Gprime, Bprime;

	if (H < 0.) {  // unlikely due to adjustment above
		return false;  //  <<< possible exit from function
	}
	else if (H < 60.) {
		Rprime = C;
		Gprime = X;
		Bprime = 0.;
	}
	else if (H < 120.) {
		Rprime = X;
		Gprime = C;
		Bprime = 0.;
	}
	else if (H < 180.) {
		Rprime = 0.;
		Gprime = C;
		Bprime = X;
	}
	else if (H < 240.) {
		Rprime = 0.;
		Gprime = X;
		Bprime = C;
	}
	else if (H < 300.) {
		Rprime = X;
		Gprime = 0.;
		Bprime = C;
	}
	else if (H < 360.) {
		Rprime = C;
		Gprime = 0.;
		Bprime = X;
	}
	else
		return false;  //  <<< possible exit from function

	red = Rprime + m;
	green = Gprime + m;
	blue = Bprime + m;

	return true;
}

/*
void DrawingUtilNG::writeHelvetica(std::string outString, int locX, int locY, int size)
{
	void *fontCode;
	switch (size) {
	case 10: fontCode = GLUT_BITMAP_HELVETICA_10;
		break;
	case 12: fontCode = GLUT_BITMAP_HELVETICA_12;
		break;
	case 18: fontCode = GLUT_BITMAP_HELVETICA_18;
		break;
	default: fontCode = GLUT_BITMAP_HELVETICA_10;
		break;
	}

	glRasterPos2i(locX, locY);
	for (int i = 0; i < outString.length(); i++) {
		glutBitmapCharacter(fontCode,
			outString.substr(i, 1).c_str()[0]);
	}
}
void DrawingUtilNG::writeTimesRoman(std::string outString, int locX, int locY, int size)
{
	void *fontCode;
	switch (size) {
	case 10: fontCode = GLUT_BITMAP_TIMES_ROMAN_10;
		break;
	case 24: fontCode = GLUT_BITMAP_TIMES_ROMAN_24;
		break;
	default: fontCode = GLUT_BITMAP_TIMES_ROMAN_10;
		break;
	}

	glutInitWindowPosition(45, 76);
	glRasterPos2i(locX, locY);
	for (int i = 0; i < outString.length(); i++) {
		glutBitmapCharacter(fontCode,
			outString.substr(i, 1).c_str()[0]);
	}
}
*/
void DrawingUtilNG::coverAndFade(int x1, int y1, int x2, int y2, int totalT, int currT,
	int r, int g, int b)
{
	if (currT > totalT)
		currT = totalT;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int temp = currT / totalT * 255;
	glColor4ub(r, g, b, currT / (double)totalT * 255);
	//glColor4ub(0, 0, 0, 180);
	drawRectangle(x1, y1, x2, y2, true);

	glDisable(GL_BLEND);

}

void DrawingUtilNG::drawCube(vertexF v1, vertexF v2, bool sixColors)
{
	drawCube(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, sixColors);
}

void DrawingUtilNG::drawCube(double x1, double y1, double z1,
	double x2, double y2, double z2, bool sixColors/*, GraphicFont* aFont*/)
{
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 1);

	glBegin(GL_QUADS);

	if (sixColors)
		glColor3ub(0, 0, 255);  // blue face
	glVertex3d(x1, y1, z1);
	glVertex3d(x2, y1, z1);
	glVertex3d(x2, y2, z1);
	glVertex3d(x1, y2, z1);



	if (sixColors)
		glColor3ub(0, 255, 0);  // green
	glVertex3d(x1, y1, z2);
	glVertex3d(x2, y1, z2);
	glVertex3d(x2, y2, z2);
	glVertex3d(x1, y2, z2);

	if (sixColors)
		glColor3ub(0, 255, 255); // cyan
	glVertex3d(x1, y1, z1);
	glVertex3d(x2, y1, z1);
	glVertex3d(x2, y1, z2);
	glVertex3d(x1, y1, z2);

	if (sixColors)
		glColor3ub(255, 0, 0);  // red
	glVertex3d(x1, y2, z1);
	glVertex3d(x2, y2, z1);
	glVertex3d(x2, y2, z2);
	glVertex3d(x1, y2, z2);

	if (sixColors)
		glColor3ub(255, 0, 255); // magenta
	glVertex3d(x1, y1, z1);
	glVertex3d(x1, y2, z1);
	glVertex3d(x1, y2, z2);
	glVertex3d(x1, y1, z2);

	if (sixColors)
		glColor3ub(255, 255, 0);  // yellow
	glVertex3d(x2, y1, z1);
	glVertex3d(x2, y2, z1);
	glVertex3d(x2, y2, z2);
	glVertex3d(x2, y1, z2);

	glEnd();

	glDisable(GL_POLYGON_OFFSET_FILL);

	// font tester (needs GraphicFont.h)
	//if (aFont != NULL) {
	//	aFont->setColorHSV(30, 1, 1);
	//	//aFont->drawText3D("  ONE is the loneliest number",
	//	aFont->drawText3D(" 1ne",
	//		{ (float)x2, (float)y1, (float)z2 }, { (float)x2, (float)y2, (float)z2 }, { (float)x2, (float)y1, (float)z1 }, .15, 10.);

	//	aFont->drawText3D(" 2wo",
	//		{ (float)x2, (float)y1, (float)z1 }, { (float)x2, (float)y2, (float)z1 }, { (float)x1, (float)y1, (float)z1 }, .15, 10.);

	//	aFont->drawText3D(" 6ix",
	//		{ (float)x1, (float)y1, (float)z1 }, { (float)x1, (float)y2, (float)z1 }, { (float)x1, (float)y1, (float)z2 }, .15, 10.);

	//	aFont->drawText3D(" 5ive",
	//		{ (float)x1, (float)y1, (float)z2 }, { (float)x1, (float)y2, (float)z2 }, { (float)x2, (float)y1, (float)z2 }, .15, 10.);

	//	aFont->drawText3D(" 3ee",
	//		{ (float)x1, (float)y2, (float)z1 }, { (float)x2, (float)y2, (float)z1 }, { (float)x1, (float)y2, (float)z2 }, .15, 10.);

	//	aFont->drawText3D(" 4r",
	//		{ (float)x2, (float)y1, (float)z2 }, { (float)x2, (float)y1, (float)z1 }, { (float)x1, (float)y1, (float)z2 }, .15, 10.);


}

