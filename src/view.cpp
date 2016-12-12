#include "view.h"
#include "model.h"

int View::h = 0;
int View::w = 0;

extern Model model, model1;

float View::transx = 0.0;
float View::transy = 0.0;
float View::transz = 0.0;
float View::scale = 1;
float View::angle = 0.0;
int View::light0 = 0;
int View::light1 = 0;
int View::light2 = 0;
int View::light3 = 0;
int View::box0 = 1;
int View::box1 = 1;
int View::tex0 = 1;
int View::tex1 = 1;
GLuint View::tex[] = {0, 0, 0};
Trackball View::trackball;
unsigned char* image;
int width, height;

/*
 * initRendering: Setting up glut window, view, lights and textures
 */
void View::initRendering() {
	w = glutGet( GLUT_WINDOW_WIDTH );
	h = glutGet( GLUT_WINDOW_HEIGHT );
	glViewport(0, 0, w, h);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	
	GLfloat spot_direction[] = { 0.0, 0.0, 0.0 };
	GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	
	// Parameters for LIGHT0
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,10.0f);
	glLightf(GL_LIGHT0,GL_SPOT_EXPONENT, 50.0f);
	
	// Paramters for LIGHT1
	GLfloat light_diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular1[] = { 1.0, 1.0, 1.0, 1.0 };
	
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
	glLightf(GL_LIGHT1,GL_SPOT_CUTOFF, 10.0f);
	glLightf(GL_LIGHT1,GL_SPOT_EXPONENT, 50.0f);
	
	// Parameters for LIGHT2
	GLfloat light_ambient2[] = { .1, .1, .1, 1.0 };
	GLfloat light_diffuse2[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular2[] = { 1.0, 1.0, 1.0, 1.0 };
			
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
	
	// Parameters for LIGHT3
	GLfloat light_ambient3[] = { 1, 1, .0, 1.0 };
	GLfloat light_specular3[] = { 1.0, 1.0, 1.0, 1.0 };
	
	glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient3);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular3);
	glLightf(GL_LIGHT3,GL_SPOT_CUTOFF, 60.0f);
	glLightf(GL_LIGHT3,GL_SPOT_EXPONENT, 100.0f);
	
	// Texture initialization
	glGenTextures(1, &tex[0]);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	// Texture Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Texture Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Texture Load
	image = SOIL_load_image("../bin/wood2.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
              GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	
	char text1[] = "../bin/apple1.jpg";
	char text2[] = "../bin/wall.jpg";
	model.readTexture(text1, &tex[1]);
	model1.readTexture(text2, &tex[2]);
	
	glEnable(GL_DEPTH_TEST);	
	glClearColor(0.0, 0.0, .0, 0.0);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();    
	gluPerspective( 60.0, w / h, 0.01, 40.0 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
 * handleResize: Setting up glut window and view after window resize
 */
void View::handleResize(int wt, int ht){
	if (ht  == 0)
		ht = 1;
	
	w = wt;
	h = ht;
	glViewport(0,0, wt, ht);
	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective( 60.0, w / h, 0.1, 10.0 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
 * rotate: rotate the object using trackball
 */
void View::rotate(float oldx, float oldy, float x, float y){
	trackball.rotate((2.0*oldx - w)/(float)w,
					 (h - 2.0*oldy)/(float)h,
					 (2.0*x - w)/(float)w,
					 (h - 2.0*y)/(float)h);
}

/*
 * draw: Render the axis, model and bounding box
 */
void View::draw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat position[] = { 0.5, 1.0, 2, 1.0 };
	GLfloat position1[] = { -0.5, 1, 2, 1.0 };
	static float rotation_transform[4][4];
	GLfloat spotlight1[] = { -0.2, -0.3, -1};
	GLfloat spotlight[] = { 0.2, -0.2, -1};		
	
	float eye[] ={ .05+transx, 2+transy, 5+transz-scale, 1};
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	if (light0)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);
	if (light1){
		glEnable(GL_LIGHT1);
	}else
		glDisable(GL_LIGHT1);
	if (light2)
		glEnable(GL_LIGHT2);
	else
		glDisable(GL_LIGHT2);
	if (light3){
		glEnable(GL_LIGHT3);
	}else
		glDisable(GL_LIGHT3);
	
	trackball.rotationMatrix( rotation_transform );
	multMatrix(&eye[0], &rotation_transform[0][0]);
	GLfloat spotlight2[] = {  -eye[0], -eye[1], -eye[2]};
	gluLookAt
		(
		eye[0], eye[1], eye[2],
		0+transx, 0+transy, 0+transz,
		0, 1, 0
		);
		
	// Drawing Axises
	model.drawAxis();
	
	//Drawing two light sources
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(position[0], position[1], position[2]);
	glutWireSphere(0.025, 10, 10);
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslatef(position1[0], position1[1], position1[2]);
	glutWireSphere(0.025, 10, 10);
	glPopMatrix();
	
	// Setting Light Position
	glLightfv (GL_LIGHT0, GL_POSITION, position);
	glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, spotlight);
    glLightfv (GL_LIGHT1, GL_POSITION, position1);
    glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, spotlight1);
    glLightfv (GL_LIGHT2, GL_POSITION, position1);
    glLightfv (GL_LIGHT3, GL_POSITION, eye);
    glLightfv (GL_LIGHT3, GL_SPOT_DIRECTION, spotlight2);
    
    // Texture Mapping the Floor
	glEnable (GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	model.drawFloor(1.75);
	glDisable(GL_TEXTURE_2D);
	glDisable (GL_LIGHTING);
	
    // Rendering Model-1
    glPushMatrix();
	glTranslatef(-1, 0, 0);
	if (box0)
		model.drawBoundingBox();
	glEnable (GL_LIGHTING);
	if (tex0){
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, tex[1]);
		model.drawObject();
		glDisable(GL_TEXTURE_2D);
	}else{
		model.drawObject();
	}
	glDisable (GL_LIGHTING);
	glPopMatrix();
	
	// Rendering Model-2
	glPushMatrix();
	glTranslatef(1, 0, 0);
	if (box1)
		model1.drawBoundingBox();
	glEnable (GL_LIGHTING);
	if (tex1){
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, tex[2]);
		model1.drawObject();
		glDisable(GL_TEXTURE_2D);
	}else{
		model1.drawObject();
	}
	glDisable (GL_LIGHTING);
	glPopMatrix();
	
	glutSwapBuffers();
}

/*
 * multMatrix: compute matrix multiplication of e and rot
 */
void View::multMatrix(float *e, float *rot ){
	int i, j;
	float c[4];
	for (i=0; i<4;i++){
		c[i] = 0.0;
		for (j=0; j<4;j++){
			c[i] += e[j]*rot[4*j+i];
		}
	}
	for (i=0; i<3;i++){
		e[i] = c[i];
	}
}
