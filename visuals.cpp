#include <stdio.h>     // - Just for some ASCII messages
#include <string.h>
#include <string>
#include <math.h>
#include <iostream>
#include <sstream> 
#include <fstream>
#include <time.h>
#include "gl/glut.h"   // - An interface and windows management library
#include "visuals.h"   // Header file for our OpenGL functions

model md;
static float tx = 0.0;
static float ty = 0.0;
static float rotx = 0.0;
static float roty = 0.0;
static float rotpy = 0.0;
static bool animate = false;
static bool crashed = false;
static bool passed = true;
static bool over = true;
static float rotation = 0.0;
static float sunradius = 0.4;
static float stara = 0.8;
static float starm = 0.0;
static float startable[300][3];
static float astx = 10000.0;
static float asty = 10000.0;
static float astz = 10000.0;
static float astsize = 0.05;
static int cooldown = 0.0;
static float sizebegin = 0.05;
static float sizecontinue = 0.0;
static int lifesleft = 3;
static int scr = 0;

using namespace std;

void sun()
{
	//Sphere 1
	glPushMatrix();
	glTranslatef(-80, 80, -50);
	glColor3f(0.85, 0.85, 0.10);
	glScalef(15, 15, 15);
	glutSolidSphere(1.0, 60.0, 10);
	glPopMatrix();
	//Sphere 2
	glPushMatrix();
	glTranslatef(-80, 80, -50);
	glColor4f(0.85, 0.85, 0.10, 0.15*sunradius);	// changing opacity
	glScalef(15, 15, 15);
	glutSolidSphere(sunradius, 60.0, 10);			// changing radius
	glPopMatrix();
}

void stars()
{
	for (int i = 0; i < 300; i++){
		glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef(startable[i][0] + starm, startable[i][1], startable[i][2]);	// coords from startable (filled randomly in Setup() function)
		glScalef(0.5, 0.5, 0.5);
		glutSolidSphere(1.0, 10.0, 10);
		glPopMatrix();

		glPushMatrix();
		glColor4f(1, 1, 1, 0.30 * stara);		// changing opacity
		glTranslatef(startable[i][0] + starm, startable[i][1], startable[i][2]);	// moving arround
		glScalef(0.5, 0.5, 0.5);
		glutSolidSphere(stara, 10.0, 10);		// changing radius
		glPopMatrix();
	}
}

void asteroid(){
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(astx, asty, astz);			// coords from idle() function
	glRotatef(rotation*0.5, 1, 1, 1);		// same rotation as the one for the propellers
	glScalef(astsize, astsize, astsize);	// random size from idle()
	DisplayModel(md);
	glPopMatrix();
}

void spaceship()
{
	glTranslatef(0, 0, 0);

	glScalef(0.5, 0.5, 0.5);

	//Body
	glPushMatrix();
	glColor3f(0.560784, 0.737255, 0.560784);
	glScalef(5, 20, 5);
	glutSolidSphere(3.0, 100.0, 10);
	glPopMatrix();
	
	//Cockpit
	glPushMatrix();
	glTranslatef(0, 10, 10);
	glColor4f(0.0, 1.0, 1.0, 0.5);
	glScalef(10, 25, 20);
	glutSolidSphere(1.0, 100.0, 10);
	glPopMatrix();
	
	//Wing
	glPushMatrix();
	glTranslatef(0, 0, 5);
	glColor3f(1.0, 1.0, 1.0);
	glRotatef(90, 0, 1, 0);
	glScalef(2, 2, 20);
	glutSolidSphere(3.0, 100.0, 10);
	glPopMatrix();

	//Tail
	glPushMatrix();
	glTranslatef(0, -35, 0);
	glColor3f(1.0, 0.0, 0.0);
	glScalef(2, 10, 5);
	glutSolidSphere(3.0, 100.0, 10);
	glPopMatrix();

	//Propellers
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(-50, 5, 5);
	glRotatef(rotation + 90, 0, 1, 0);
	glScalef(2, 2, 10);
	glutSolidSphere(1.0, 50.0, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(-50, 5, 5);
	glRotatef(rotation, 0, 1, 0);
	glScalef(2, 2, 10);
	glutSolidSphere(1.0, 50.0, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(50, 5, 5);
	glRotatef(rotation + 90, 0, 1, 0);
	glScalef(2, 2, 10);
	glutSolidSphere(1.0, 50.0, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(50, 5, 5);
	glRotatef(rotation, 0, 1, 0);
	glScalef(2, 2, 10);
	glutSolidSphere(1.0, 50.0, 10);
	glPopMatrix();
}

void gametext(const char *str,float size)
{
	// begin/game over text
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(-40.0, 50.0, 0.0);
	glScalef(size,size,size);
	for (int i=0;i<strlen(str);i++)
	  glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);
	glPopMatrix();

}

void lifes(const char *str, int l, float size){
	// lifes text
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(50, 60.0, 0.0);
	glScalef(size, size, size);
	for (int i = 0; i<strlen(str); i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	string sl = to_string(l);
	for (int i = 0; i < strlen(sl.c_str()); i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, sl[i]);
	glPopMatrix();
}

void score(const char *str, int scr, float size){
	// score text
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(10, 60.0, 0.0);
	glScalef(size, size, size);
	for (int i = 0; i<strlen(str); i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	string sl = to_string(scr);
	for (int i = 0; i < strlen(sl.c_str()); i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, sl[i]);
	glPopMatrix();
}

void Render()
{    
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity();

  glTranslatef(0,10,-140);

  score("Score: ", scr, 0.05);
  lifes("Lifes: ", lifesleft, 0.05);
  gametext("GAME OVER! Press p to continue", sizecontinue);
  gametext("Please press p to begin!", sizebegin);


  glRotatef(rotx, 1, 0, 0);		// camera movement x
  glRotatef(roty, 0, 1, 0);		// camera movement y

  sun();

  stars();

  asteroid();

  glRotatef(-80, 1, 0, 0);

  glTranslatef(tx, 0.0, 0.0);	// spaceship movement 
  glTranslatef(0.0, 0.0, ty);	// spaceship movement 
  glRotatef(-tx, 0, 1, 0);		// left right rotation on movement
  spaceship();


  glutSwapBuffers();        
}

//-----------------------------------------------------------

void Resize(int w, int h)
{ 
  if (h==0) h=1;
  glViewport(0,0,w,h); 

  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
 
  gluPerspective(60.0, (float)w/(float)h, 1.0, 500.0);
}

void Idle()
{
	if (crashed == true && lifesleft == 0){
		// reveal game over text
		sizecontinue = 0.05;
	}
	if (animate){
		if (lifesleft == 0){
			// continued after game over, reset stats
			lifesleft = 3;
			scr = 0;
		}

		sizebegin = 0.0;		// size of begin text -> 0
		sizecontinue = 0.0;		// size of continue text -> 0

		rotation += 0.5;		// propellers and asteroid rotation
		if (rotation == 360.0)
			rotation = 0;

		sunradius = ((double)rand() / (RAND_MAX)) + 0.4;	// changing sun radius
		stara = ((double)rand() / (RAND_MAX)) + 0.4;	// star opacity and radius
		starm += 0.1; // stars moving a little bit
		if (starm == 0.5)
			starm = 0.0;

		if (!passed){	
			// meteor hasn't passed from our spaceship yet so it's moving
			if (astx != 0){
				if (astx < 0){
					astx += 0.1;
				}
				else{
					astx -= 0.1;
				}
			}
			if (asty != 0){
				if (asty < 0){
					asty += 0.1;
				}
				else{
					asty -= 0.1;
				}
			}			
			astz += 1;

			if (astz == 0){
				// meteor at same z as our spaceship so check for collision 
				if (abs(astx - tx) <= 30 && abs(asty - ty) <= 30){
					// if collision is detected lifes = lifes -1
					lifesleft -= 1;
					if (lifesleft == 0){
						// if lifes = 0 then GAME OVER
						animate = false;
						crashed = true;
					}
				}
				else{
					// no collision score = score + 1
					scr += 1;
				}
				// now the asteroid is after our spaceship
				passed = true;
			}

		}else if(!over){
			// asteroid after our spaceship but still has to move a little bit
			astz += 1.0;
			if (astx < 0)
				astx += 0.1;
			else
				astx -= 0.1;
			if (asty < 0)
				asty += 0.1;
			else
				asty -= 0.1;
			if (astz == 500)
				over = true;
		}
		if (passed && over){
			// new meteor generation cooldown 
			cooldown -= 0.1;
		}
		if (cooldown == 0.0 && over){
			// generate meteor size and position
			astsize = 0.01 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.03 - 0.01)));
			if (rand() % 2 == 0)
				astx = (rand() % 100);
			else 
				astx = (-rand() % 100);
			if (rand() % 2 == 0)
				asty = (rand() % 100);
			else
				asty = (-rand() % 100);
			astz = (-rand() % 200) - 300;
			passed = false;
			over = false;
			cooldown = rand() % 6 + 50;
		}
	}
	glutPostRedisplay();
}

void Keyboard(unsigned char key,int x,int y)
{
	// q for quit - wasd for camera control - p for animation
	switch(key)
	{
	case 'q' : exit(0);
		break;
	case 'a' : roty-= 3.5f;
		break;
	case 'd' : roty+= 3.5f;
		break;
	case 'w': rotx += 3.5f;
		break;
	case 's': rotx -= 3.5f;
		break;
	case 'p': animate = !animate;
		break;
	default : break;
	}
	glutPostRedisplay();

}

void KeyboardExtra(int key, int x, int y)
{
	// arrow keys for spaceship control - limits spaceship movement inside camera view
	switch (key)
	{
	case GLUT_KEY_UP: 
		if(ty < 35)
			ty += 10.5f;
		break;
	case GLUT_KEY_DOWN:
		if (ty > -63)
			ty -= 10.5f;
		break;
	case GLUT_KEY_LEFT:
		if (tx > -63)
			tx -= 10.5f;
		break;
	case GLUT_KEY_RIGHT:
		if (tx < 63)
			tx += 10.5f;
		break;
	default: break;
	}
}


void Setup() 
{ 
	srand(time(NULL));

	ReadFile(&md);

	//Parameter handling
	glShadeModel (GL_SMOOTH);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1);
    
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
  
	//Set up light source
	GLfloat light_position[] = {-80.0, 90.0, -190.0, 1.0 };
	glLightfv( GL_LIGHT0, GL_POSITION, light_position);

	GLfloat ambientLight[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glFrontFace(GL_CCW);

	glClearColor(0.0f,0.0f,0.0f,1.0f);

	// fill startable with coords

	for (int i = 0; i < 300; i++){
		if (rand() % 2 == 0)
			startable[i][0] = rand() % 320;
		else
			startable[i][0] = -rand() % 320;
	}

	for (int i = 0; i < 300; i++){
		if (rand() % 2 == 0)
			startable[i][1] = rand() % 320;
		else
			startable[i][1] = -rand() % 320;
	}

	for (int i = 0; i < 300; i++){
		if (rand() % 2 == 0)
			startable[i][2] = rand() % 320;
		else
			startable[i][2] = -rand() % 320;
	}
}

void ReadFile(model *md)
{
	ifstream obj_file;
	obj_file.open("asteroid.obj");
   
	if (!obj_file.is_open())
		exit(1);
	
	string line;
	istringstream iss;
	int i = 0, j=0;

	md->vertices = 9122;
	md->faces = 18240;

	while (getline(obj_file, line)){
		string lineType;
		iss.str("");
		iss.clear();
		iss.str(line);
		iss >> lineType;

		if (lineType == "v"){
			float x = 0, y = 0, z = 0;
			iss >> x >> y >> z;
			md->obj_points[i].x = x;
			md->obj_points[i].y = y;
			md->obj_points[i].z = z;
			i++;
		}

		if (lineType == "f"){
			int x = 0, y = 0, z = 0;
			string n1, n2, n3;
			iss >> n1 >> n2 >> n3;
			n1 = n1.substr(0, n1.find("/"));
			n2 = n2.substr(0, n2.find("/"));
			n3 = n3.substr(0, n3.find("/"));
			x = atoi(n1.c_str());
			y = atoi(n2.c_str());
			z = atoi(n3.c_str());
			md->obj_faces[j].vtx[0] = x;
			md->obj_faces[j].vtx[1] = y;
			md->obj_faces[j].vtx[2] = z;
			j++;
		}
	}

   obj_file.close();
}

void DisplayModel(model md)
{

	glPushMatrix();
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < md.faces; i++)
	{
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[0]-1].x,md.obj_points[md.obj_faces[i].vtx[0]-1].y,md.obj_points[md.obj_faces[i].vtx[0]-1].z);
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[1]-1].x,md.obj_points[md.obj_faces[i].vtx[1]-1].y,md.obj_points[md.obj_faces[i].vtx[1]-1].z);
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[2]-1].x,md.obj_points[md.obj_faces[i].vtx[2]-1].y,md.obj_points[md.obj_faces[i].vtx[2]-1].z);
	}

	glEnd();
	glPopMatrix();

}