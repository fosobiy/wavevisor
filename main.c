
#include <sys/ksys.h>
#include <kosgl.h>

#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "SysCall.h"


struct {
	int x, y;
	int dx, dy;
} win;

static GLuint* TexObj;

KOSGLContext cgl;

void DrawGL() {
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glTranslatef(0.0, 0.0, -600.0);
	glRotatef(0.0, 1.0, 0.0, 0.0);
	glRotatef(0.0, 0.0, 1.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-300.0,  256.0, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-300.0, -256.0, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f( 300.0, -256.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f( 300.0,  256.0, 0.0);
	glEnd();

	kosglSwapBuffers();
}

int winx_size = 600;
int winy_size = 512;

void reshape() {
	glViewport(0, 0, winx_size, winy_size - 20);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (GLfloat)winx_size / winy_size, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void disabletgl() {
	kosglDestroyContext(cgl);
}

void end_this_task_m1 () {
	int foo = -1;
	__asm__ ("movl %0, %%eax" : "m" (foo));
    __asm__ ("int $0x40");
}

char * title = "TinyGL in KolibriOS";

void draw_window(void) {
	_ksys_start_draw();
	_ksys_create_window(win.x, win.y, win.dx, win.dy, title, 0, 3);
	_ksys_end_draw();
}



struct rgb_pixel {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

struct rgb_pixel palette[512];
double spectrum[610][512];

double linear(double x, double start, double end) {
	if (x < start)
		return 0;
	else if (x > end)
		return 1;
	else
		return (x - start) / (end - start);
}

double getR(double value) {
	return linear(value, 25.0/200.0, 140.0/200.0);
}
double getG(double value) {
	return linear(value, 120.0/200.0, 180.0/200.0);
}
double getB(double value) {
	return linear(value, 0.75, 1.0) + (linear(value, 0, 57.0/200.0) - linear(value, 63.0/200.0, 120.0/200.0))*0.5;
}

double maxf(double a, double b) {
	return a > b ? a : b;
}
double minf(double a, double b) {
	return a < b ? a : b;
}


void main(void) {

	win.x = 100;
	win.y = 100;
	win.dx = 600;
	win.dy = 512;

	draw_window();

	cgl = kosglCreateContext(0, 0);
	kosglMakeCurrent( 0, 20, win.dx, win.dy - 20, cgl );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);


	
	for (int i = 0; i < 512; ++i) {
		palette[i].r = getR(i/512.0)*255;
		palette[i].g = getG(i/512.0)*255;
		palette[i].b = getB(i/512.0)*255;
	}

	
	for (int i = 0; i < 600; i++) {
		for (int j = 0; j < 512; j++) 
			spectrum[i][j] = (rand() * (1.0f / RAND_MAX)) * 2.0 + 10.0;
	}


	
	int width = 600, height = 512;
	unsigned char * texture;
	texture = malloc(512 * 600 * 3);

	
	double maxValue = 0;
	for (int x_ = 0; x_ < 600; ++x_) {
		for (int y_ = 0; y_ < 512; ++y_) {
			maxValue = maxf(maxValue, spectrum[x_][y_]);
		}
	}
	
	for (int x_ = 0; x_ < 600; ++x_) {
		for (int y_ = 0; y_ < 512; ++y_) {
			double value = maxf(0.0, spectrum[x_][y_] / maxValue);
			value = (1 - minf(-log(value), 12.0)/12.0) * 512;
			
			texture[((512 - y_ - 1) * 600 * 3 + (x_) * 3) + 0] = palette[(int)value].r;
			texture[((512 - y_ - 1) * 600 * 3 + (x_) * 3) + 1] = palette[(int)value].g;
			texture[((512 - y_ - 1) * 600 * 3 + (x_) * 3) + 2] = palette[(int)value].b;
		}
	}
	
	

	/* Setup texturing */
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

	/* generate texture object IDs */
	glGenTextures(1, TexObj);
	glBindTexture(GL_TEXTURE_2D, *TexObj);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0,  GL_RGB, GL_UNSIGNED_BYTE, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, *TexObj);
	glEnable(GL_TEXTURE_2D);


	SysCall3(66, 1, 1);

	reshape();

	do {
		DrawGL();

		switch(_ksys_check_event()) {
			case 1:
				draw_window();
				reshape();
				break;
			case 2: break;
			case 3:
				free(texture);
				disabletgl();
				end_this_task_m1();
				return;
		}
	} while(1);
	
	disabletgl();
	end_this_task_m1();
	return;
}































