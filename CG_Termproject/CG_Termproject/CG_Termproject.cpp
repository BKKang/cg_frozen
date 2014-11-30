#include <windows.h>
#include <glut.h>
#include <stdio.h>
#include <glext.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

static int Time = 0;
static int currentTime = 0;

int height=800, y_temp[1000],x_temp[1000],z_temp[1000];

GLuint LoadTexture(const char* filename, bool alpha){
	GLuint texture;
	GLuint* data;
	int width, height;
	FILE * file;

	file = fopen(filename, "rb");

	if (file == NULL){printf("no"); return 0;}
	
	width = 160;
	height = 160;
	
	data = new GLuint[width * height];
    fread( data, width * height * sizeof(GLuint), 1, file); 
	fclose(file);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
	
	if(alpha) //for .bmp 32 bit
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else //for .bmp 24 bit
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4); 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_BGR, GL_UNSIGNED_BYTE, data);
	}

	delete(data);

	return texture;
}
void DrawBg(void){
	glDisable(GL_CULL_FACE);
	GLuint texBG[3];
	GLfloat size = 70.0;

	texBG[0] = LoadTexture("bg_right.bmp",FALSE);
	texBG[1] = LoadTexture("bg_bottom.bmp",FALSE);
	texBG[2] = LoadTexture("bg_left.bmp",FALSE);
	
	glEnable(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, texBG[0]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 0.0, size);
	glTexCoord2f(0.0, 1.0); glVertex3f(size, 0.0, size);
	glTexCoord2f(0.0, 0.0); glVertex3f(size, 0.0, 0.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texBG[1]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(size, 0.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(size, size, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0, size, 0.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texBG[2]);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.0, size, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.0, size, size);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 0.0, size);
	glEnd();
}

void DrawCastle(int texID){
	glEnable(GL_CULL_FACE);

	GLUquadricObj *base = gluNewQuadric();
	
	gluQuadricDrawStyle(base, GLU_FILL);
	gluQuadricNormals(base, GLU_SMOOTH);
	gluQuadricTexture(base, GL_TRUE);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
	
	glTranslatef(0, 0, -150.0);
	
	/*ground floor disk*/
	glTranslatef(0, 0, 10);
	gluDisk(base, 0, 25, 20, 1);
	/*2nd floor*/
	glTranslatef(0.0, 0.0, 20.0); //height
	gluDisk(base, 0, 20, 6, 1);

	/*1st floor*/
	glTranslatef(0.0, 0.0, -30.0); //height of 1st floor + ground floor

	/*ground floor*/

	gluCylinder(base, 30, 25, 10, 20, 1);
	glTranslatef(0, 0, 10);
	gluCylinder(base, 20, 20, 20, 6, 1);
}
void DrawCastleTop(int texID){
	glEnable(GL_CULL_FACE);

	GLUquadricObj *base = gluNewQuadric();
	
	gluQuadricDrawStyle(base, GLU_FILL);
	//gluQuadricNormals(base, GLU_SMOOTH);
	gluQuadricTexture(base, GL_TRUE);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
	
	//glTranslatef(40.0, 40.0, 0);
	glTranslatef(0, 0, -150.0);

	/*2nd floor pillar back,right two*/
	glTranslatef(-10.0, -10.0, 0.0); 
	gluCylinder(base, 3, 3, 15, 6, 1);
	glTranslatef(0.0, 0.0, 15.0);
	gluCylinder(base, 3, 4, 5, 20, 1);
	glTranslatef(0.0, 0.0, 5.0);
	gluCylinder(base, 5, 0, 10, 5, 1);
	glTranslatef(0.0, 0.0, -20.0);

	glTranslatef(0.0, 20.0, 0.0);
	gluCylinder(base, 2.5, 2.5, 15, 6, 1);
	glTranslatef(0.0, 0.0, 15.0);
	gluCylinder(base, 2.5, 3, 5, 20, 1);
	glTranslatef(0.0, 0.0, 5.0);
	gluCylinder(base, 4, 0, 15, 5, 1);
	glTranslatef(0.0, 0.0, -20.0);

	/*2nd floor base*/
	glTranslatef(10.0, -10.0, 0.0);
	gluCylinder(base, 11.5, 11.5, 15, 6, 1);

	/*3rd floor*/
	glTranslatef(0.0, 0.0, 15.0);//height
	gluDisk(base, 0, 11.5, 6, 1);
	
	/*3rd floor pillar back two*/
	glTranslatef(0, -8, 0);
	gluCylinder(base, 2, 2, 15, 6, 1);
	glTranslatef(0, 0, 15);
	gluCylinder(base, 2, 3, 5, 6, 1);
	glTranslatef(0, 0, 5);
	gluCylinder(base, 3.5, 0, 15, 6, 1);

	glTranslatef(-8, 8, -20);
	gluCylinder(base, 2, 2, 12, 6, 1);
	glTranslatef(0, 0, 12);
	gluCylinder(base, 3, 0, 8, 6, 1);
	glTranslatef(8, 0, -12);
	
	gluCylinder(base, 5, 5, 10, 20, 1);
	
	/*4th floor*/
	glTranslatef(0.0, 0.0, 0.0);//height 
	gluDisk(base, 0, 5, 20, 1);
	gluCylinder(base, 3, 3, 5, 6, 1);

	/*5th floor(roof)*/
	glTranslatef(0.0, 0.0, 5.0);
	gluCylinder(base, 3, 4, 6, 20, 1);
	glTranslatef(0.0, 0.0, 6.0);
	gluCylinder(base, 5, 0, 15, 20, 1);
	glTranslatef(0.0, 0.0, -11.0);

	/*3rd floor pillar front two*/
	glTranslatef(0, 8, 0);
	gluCylinder(base, 3, 3, 5, 6, 1);
	glTranslatef(0, 0, 5);
	gluCylinder(base, 3, 4, 2, 6, 1);
	glTranslatef(0, 0, 2);
	gluCylinder(base, 4, 4, 2, 6, 1);
	glTranslatef(0, 0, 2);
	gluDisk(base, 0, 4, 6, 1);
	glTranslatef(8, -8, -9);

	gluCylinder(base, 3, 3, 3, 6, 1);
	glTranslatef(0, 0, 3);
	gluCylinder(base, 3, 4, 2, 20, 1);
	glTranslatef(0, 0, 2);
	gluCylinder(base, 4, 4, 2, 20, 1);
	glTranslatef(0, 0, 2);
	gluDisk(base, 0, 4, 20, 1);
	glTranslatef(-8, 0, -7);
	
	/*2nd floor pillar front, left two*/
	glTranslatef(-10.0, 10.0, -15.0);

	glTranslatef(20.0, 0.0, 0.0);
	gluCylinder(base, 3, 3, 5, 6, 1);
	glTranslatef(0.0, 0.0, 5.0);
	gluCylinder(base, 3, 4, 3, 20, 1);
	glTranslatef(0.0, 0.0, 3.0);
	gluCylinder(base, 5, 0, 15, 5, 1);
	glTranslatef(0.0, 0.0, -8.0);

	glTranslatef(0.0, -20.0, 0.0);
	gluCylinder(base, 3, 3, 10, 6, 1);
	glTranslatef(0.0, 0.0, 10.0);
	gluCylinder(base, 3, 4, 5, 20, 1);
	glTranslatef(0.0, 0.0, 5.0);
	gluCylinder(base, 5, 0, 20, 5, 1);
	glTranslatef(0.0, 0.0, -15.0);

}
void MakeCastle(void){
	glPushMatrix();
	float temp = Time/10;
	GLuint texID;
	
	if(2000 <= currentTime && currentTime < 7000){
		glColor4f(1.0, 1.0, 1.0, 0.5);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		texID = LoadTexture("iceTex6.bmp",TRUE);
		glRotatef((GLfloat) Time, 0.0, 0.0, 1.0);
		glTranslatef(0, 0, temp);
		DrawCastle(texID);
		glDisable(GL_BLEND);
	}
	else if(7000 <= currentTime && currentTime < 12500){
		texID = LoadTexture("iceTex.bmp",TRUE);
		glTranslatef(0, 0, 100);
		DrawCastle(texID);
		
		glColor4f(1.0, 1.0, 1.0, 0.5);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		texID = LoadTexture("iceTex6.bmp",TRUE);

		glTranslatef(0, 0 ,0);
		glRotatef((GLfloat) Time, 0.0, 0.0, 1.0);
		glTranslatef(0, 0, temp);
		DrawCastleTop(texID);
		glDisable(GL_BLEND);
	}
	else if(12500 <= currentTime){
		texID = LoadTexture("iceTex.bmp",TRUE);
		
		glTranslatef(0, 0, 100);
		DrawCastle(texID);
		glTranslatef(0, 0, 170);
		DrawCastleTop(texID);
	}
	glutPostRedisplay();
	glPopMatrix();
}

void DrawSnow(void){
	glEnable(GL_CULL_FACE);
	glColor3f(0.0, 0.0, 0.0);
	GLUquadricObj *base = gluNewQuadric();
	
	gluQuadricDrawStyle(base, GLU_FILL);
	gluQuadricNormals(base, GLU_SMOOTH);
	gluQuadricTexture(base, GL_TRUE);

	GLuint texID;
	texID = LoadTexture("bg_bottom.bmp",0);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
	
	for(int i=0;i<height;i++){
		glTranslatef(x_temp[i],y_temp[i],z_temp[i]);
		glutSolidSphere(0.5,50,40);
		glTranslatef(x_temp[i]*-1,y_temp[i]*-1,z_temp[i]*-1);
	}
}
void MakeSnow(void){
	glPushMatrix();
	float temp=Time/10;
	
	glTranslatef(0, 0, -temp);
	DrawSnow();
	
	glPopMatrix();
}
void DrawFlake(void){
	glEnable(GL_CULL_FACE);
	glColor3f(1.0, 0.0, 0.0);
	GLUquadricObj *base = gluNewQuadric();
	
	gluQuadricDrawStyle(base, GLU_FILL);
	gluQuadricNormals(base, GLU_SMOOTH);
	gluQuadricTexture(base, GL_TRUE);

	GLuint texID2;
	texID2 = LoadTexture("iceTex7.bmp",0);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID2);

	glTranslatef(30, 30, 15);
	glTranslatef(10, 10, 5);
	glTranslatef(-30, -30, -20);
	
	glBindTexture(GL_TEXTURE_2D, texID2);
	glTranslatef(30,30,15);
	
	if(currentTime>0){
      gluDisk(base, 1, 2, 6, 1);
   }
   if(currentTime>400){
      gluDisk(base, 4, 6, 6, 1);
   }
   if(currentTime>800){
      gluDisk(base, 8, 10, 6, 1);
   }
   if(currentTime>1200){
      gluDisk(base, 13, 18, 6, 1);
   }
   if(currentTime>1600){
      gluDisk(base, 20, 24, 6, 1);
   }
   if(currentTime>2000){
      glTranslatef(-30,-30,-15);
   }

	/*
	gluDisk(base, 1, 2, 6, 1);
	gluDisk(base, 4, 6, 6, 1);
	gluDisk(base, 8, 10, 6, 1);
	gluDisk(base, 13, 18, 6, 1);
	gluDisk(base, 20, 24, 6, 1);
	glTranslatef(-30,-30,-15);
	*/
}
void display(void){
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0, 0.0, 1.0);
	glLoadIdentity();
	gluLookAt(9.5, 9.5, 9.5, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	
	//성 보이는 크기 조절
	glScalef(0.16, 0.16, 0.16);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	GLfloat light_pos[4] = {500.0f, 500.0f, 500.0f, 1.0f};
	GLfloat light_dif[4] = {0.35,0.35,0.35,1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);

	//배경그리기
	DrawBg();
	DrawFlake();
	MakeSnow();
	MakeCastle();

	glutSwapBuffers();
}

void reshape(int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	glMatrixMode(GL_MODELVIEW);
}
void MyTimer(int Value){
	currentTime = glutGet(GLUT_ELAPSED_TIME);	
	Time = (Time + 30) % 1800;
	glutPostRedisplay();
	glutTimerFunc(40, MyTimer, 1);
}
int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(600,600);
	glutCreateWindow("Frozen");

	
	srand(time(NULL));
	for(int i =0;i<height;i++){
		z_temp[i]=rand()%1000+1;
		y_temp[i]=rand()%80+1;
		x_temp[i]=rand()%80+1;
	}

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(30, MyTimer,1);
	glutMainLoop();

	return 0;
}