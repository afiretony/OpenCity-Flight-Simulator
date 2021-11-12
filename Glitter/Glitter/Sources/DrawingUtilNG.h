#pragma once
#include <string>

//#include "GraphicFont.h"
//class GraphicFont;   // needed because GraphicFont.h includes DrawingUtilNG.h

namespace DrawingUtilNG {
	// this can be used as a vertex coordinates or a 3-D vector definition
	//     (mathematical/physics vector, not data structure vector)
	struct vertexF {
		float x, y, z;
	};

	// vector functions, hopefully unambiguously named
	float vectorLength(vertexF v0); 
	vertexF getUnitVector(vertexF v0);
	vertexF getUnitVector(vertexF v1, vertexF v2);
	vertexF vectorSum(vertexF v1, vertexF v2);
	vertexF vectorSum(vertexF v1, vertexF v2, vertexF v3);
	vertexF scalarProduct(vertexF v0, float scalar);
	float dotProduct(vertexF v1, vertexF v2);
	vertexF crossProduct(vertexF v1, vertexF v2);
	
	// rotate vector v by theta degrees about vector k
	// uses Rodrigues' Rotation Formula (see Wikipedia)
	vertexF rotateVector(vertexF v, vertexF k, float theta); 

	// draws common shapes, filled in or not
	void drawCircle(double centerX, double centerY, double radius, bool filled);
	void drawRectangle(double x, double y, int sizeX, int sizeY, bool filled);

	//converts a color model from HSV (degrees, 0-1., 0-1.) to RGB (3 values 0-1.
	bool hsv2rgb(double H, double S, double V,
		double &red, double &green, double &blue);

	//// quick writing with fancy fonts, but requires GLUT library
	//void writeHelvetica(std::string outString, int locX, int locY, int size);
	//void writeTimesRoman(std::string outString, int locX, int locY, int size);

	// draws a box (colored rgb) that increases opacity based on the ratio of currT/totalT
	void coverAndFade(int x1, int y1, int x2, int y2, int totalT, int currT,
		int r = 0, int g = 0, int b = 0);

	// draw 3D shapes

	void drawCube(vertexF v1, vertexF v2, bool sixColors = false);
	void drawCube(double x1, double y1, double z1,
		double x2, double y2, double z2, bool sixColors = false/*, GraphicFont* aFont = NULL*/);

};
