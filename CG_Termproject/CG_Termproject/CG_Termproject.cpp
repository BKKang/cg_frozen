#include <windows.h>
#include <glut.h>
#include <stdio.h>

GLuint LoadTexture(const char* filename){
	GLuint texture;
	int width, height;
	unsigned char* data;
	FILE * file;

	file = fopen(filename, "rb");

	if (file == NULL){printf("no"); return 0;}
	width = 600;
	height = 400;
	data = (unsigned char *)malloc(width * height* 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	for(int i=0; i<width * height; ++i){
		int index = i*3;
		unsigned char B, R;
		B = data[index];
		R = data[index+2];

		data[index] = R;
		data[index+2] = B;
	}
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );


	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
	free( data );

	return texture;
}
void display(void){
	glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3f(0.0, 0.0, 1.0);
	glLoadIdentity();
	gluLookAt(10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	//성 보이는 크기 조절
	glScalef(0.12, 0.12, 0.12);
	
	glEnable(GL_CULL_FACE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	GLfloat light_pos[4] = {1000.0f, 1000.0f, 1000.0f, 1.0f};
	GLfloat light_dif[4] = {0.2,0.2,0.2,1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
	
	GLUquadricObj *base = gluNewQuadric();
	
	gluQuadricDrawStyle(base, GLU_FILL);
	gluQuadricNormals(base, GLU_SMOOTH);
	gluQuadricTexture(base, GL_TRUE);

	GLuint texID;
	texID = LoadTexture("iceTex2.bmp");

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
	
	/*2nd floor*/
	glTranslatef(0.0, 0.0, 30.0); //height
	gluDisk(base, 0, 20, 6, 1);

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

	glTranslatef(-10.0, 10.0, 0.0);

	/*1st floor*/
	glTranslatef(0.0, 0.0, -30.0); //height of 1st floor + ground floor
	
	/*ground floor*/
	gluCylinder(base, 30, 25, 10, 20, 1);
	glTranslatef(0, 0, 10);
	gluDisk(base, 0, 25, 20, 1);
	
	gluCylinder(base, 20, 20, 20, 6, 1);

	glFlush();
}

void reshape(int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600,600);
	glutCreateWindow("example");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}