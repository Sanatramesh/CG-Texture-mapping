#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
#include <SOIL.h>

class Point{
	int n;
	float x, y, z;
	float normx, normy, normz;
	float r, s;
public:
	Point();
	Point(float x_coor, float y_coor, float z_coor);
	float getX();
	void setX(float x_coor);
	float getY();
	void setY(float y_coor);
	float getZ();
	void setZ(float z_coor);
	float getNormX();
	float getNormY();
	float getNormZ();
	float getTexR();
	float getTexS();
	void print();
	void normNormal();
	void textureMapCylinder();
	void textureMapSphere();
	void setNormal(float normx, float normy, float normz);
	void addNormal(float normx, float normy, float normz);
};

class Polygon{
	int no;
	std::vector<Point*> vertices;
	float normx, normy, normz;
public:
	Polygon();
	void addVertex(Point *p);
	Point* getPointAt(int i);
	void printVertices();
	void drawPolygon();
	void computeNormal();
	void normalize();
	void computeTexSphere();
	void computeTexCylinder();
};

class Model{
	std::vector<Point*> points; // List of input points
	std::vector<Polygon*> polygons; // List of polygons
	float minx, miny, minz;
	float maxx, maxy, maxz;
	int width, height;
	unsigned char *image;
public:
	Model();
	void render();
	void drawAxis();
	void drawObject();
	void readObject(char* fname);
	void computeNormals();
	void drawBoundingBox();
	void drawFloor(float size);
	void textureMapCylinder();
	void textureMapSphere();
	void readTexture(char* texname, GLuint *tex);
};

#endif
