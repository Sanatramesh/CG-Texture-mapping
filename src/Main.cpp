#include <iostream>
#include "model.h"
#include "view.h"
#include "controller.h"

using namespace std;

View view;
Model model, model1;

/*
 * main: initialize glut window
 */
int main(int argc,char* argv[])
{
	if (argc != 3){
		std::cout<<"Usage: ./assignment3 <ply1 filename> <ply2 filename>"<<std::endl;
		exit(-1);
	}
	Controller controller;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(400, 400); //Set the window size
	glutInitWindowPosition(50,50);
	//Create the window
	glutCreateWindow("Assignment 3: Texture Mapping");
	controller.run(argv);
	return 0;	
}

