#include "glut.h"
#include <math.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <string>
#include "Point2D.h"
#include <vector>

#define EYEDROP 2
#define FILL 1
#define DRAW 0
using namespace std;

const int W = 1200;
const int H = 800;
const double PI = 3.14;
const int numberOfPixels = W * H * 3;

// simulates Frame Buffer
unsigned char pixels[H][W][3] = { 0 }; // 3 is for RGB

double offset = 0;
int old_x = 0, old_y = 0;
int count_clicks = 0;
int color_limit = 120;
int brush_size = 1;
int paint_state = DRAW;
unsigned char new_color[3], replace_color[3], border_color[3] = {0,0,0}, brush_color[3] = { 0,0,0 };

// canvas parameters
int active_width = W - color_limit - 1;
int active_height = H;

// images saved count
int numSaved = 0;

void init()
{
	int i, j, slot = 5;
	glClearColor((GLclampf)0, (GLclampf)0, (GLclampf)0.3, (GLclampf)0);// color of window background
	glOrtho(-1, 1, -1, 1, -1, 1); // defines the main axis and their range

	for (i = 0; i < H; i++)
		for (j = color_limit + 1; j < W; j++)
		{
			// White canvas
			pixels[i][j][0] = (unsigned char)(255);		// red
			pixels[i][j][1] = (unsigned char)(255);	// green
			pixels[i][j][2] = (unsigned char)(255);		// blue
		}
	
	// color pallete
	for (i = 0; i < H; i++)
		for (j = 0; j <= 20; j++)
		{
			pixels[i][j][0] = (int)(255 * (1 - i / (double)H)) / slot * slot; // red
			pixels[i][j][1] = (int)(255 * (0.5 - i / (double)H)) / slot * slot; // green
			pixels[i][j][2] = (int)(0 * (1 - i / (double)H)) / slot * slot; // blue
		}
	for (i = 0; i < H; i++)
		for (j = 20; j <= 40; j++)
		{
			pixels[i][j][0] = (int)(255 * (i / (double)H)) / slot * slot; // red
			pixels[i][j][1] = (int)(255 * (0.5 - i / (double)H)) / slot * slot; // green
			pixels[i][j][2] = (int)(255 * (1 - i / (double)H)) / slot * slot; // blue
		}
	for (i = 0; i < H; i++)
		for (j = 40; j <= 60; j++)
		{
			pixels[i][j][0] = (int)(0) / slot * slot; // red
			pixels[i][j][1] = (int)(255 * (0.5 - i / (double)H)) / slot * slot; // green
			pixels[i][j][2] = (int)(255 * (i / (double)H)) / slot * slot; // blue
		}
	for (i = 0; i < H; i++)
		for (j = 60; j <= 80; j++)
		{
			pixels[i][j][0] = (int)(255 * (0.5 - i / (double)H)) / slot * slot; // red
			pixels[i][j][1] = (int)(255 * (1 - i / (double)H)) / slot * slot; // green // green
			pixels[i][j][2] = (int)(255) / slot * slot; // blue
		}
	for (i = 0; i < H; i++)
		for (j = 80; j <= 100; j++)
		{
			pixels[i][j][0] = (int)(255 * (1 - i / (double)H)) / slot * slot; // red
			pixels[i][j][1] = (int)(255 * (i / (double)H)) / slot * slot; // green // green
			pixels[i][j][2] = (int)(255 * (0.5 - i / (double)H)) / slot * slot; // blue
		}
	for (i = 0; i < H; i++)
		for (j = 100; j <= 120; j++)
		{
			pixels[i][j][0] = (int)(254 * (0.5 - i / (double)H)) / slot * slot + 1; // red
			pixels[i][j][1] = (int)(254 * (0.5 - i / (double)H)) / slot * slot + 1; // green
			pixels[i][j][2] = (int)(254 * (0.5 - i / (double)H)) / slot * slot + 1; // blue
		}
	for (i = 0; i < H; i++)
		for (j = color_limit; j <= color_limit + 1; j++)
		{
			pixels[i][j][0] = 0; // red
			pixels[i][j][1] = 0; // green
			pixels[i][j][2] = 0; // blue
		}

}

void DrawLine(int startX, int startY, int endX, int endY)
{
	int i, j, tmp;
	double a, b;

	// check valid points
	if (startX >= W || startX < 0 || endX >= W || endX < 0 || startY >= H || startY < 0 || endY >= H || endY < 0)
	{
		return;
	}

	if (abs(startX - endX) > abs(startY - endY))
	{
		a = (endY - startY) / (double)(endX - startX);
		b = startY - a * startX;

		if (startX > endX)
		{
			tmp = startX;
			startX = endX;
			endX = tmp;
		}
		for (j = startX; j <= endX; j++)
		{
			i = (int) (a * j + b);
			pixels[i][j][0] = brush_color[0];
			pixels[i][j][1] = brush_color[1];
			pixels[i][j][2] = brush_color[2];
		}
	}
	else // deltaX < deltaY
	{
		a = (double)(endX - startX)/(endY - startY) ;
		b = startX - a * startY;
		if (startY > endY)
		{
			tmp = startY;
			startY = endY;
			endY = tmp;
		}
		for (i = startY; i < endY; i++)
		{
			j =(int) (a * i + b);
			pixels[i][j][0] = brush_color[0];
			pixels[i][j][1] = brush_color[1];
			pixels[i][j][2] = brush_color[2];
		}

	}
}

// color the current pressed pixel (plus brush_size pixels arround it)
void PaintPixel(int x, int y, unsigned char newColor[3])
{
	// check valid points
	if (x + brush_size - 1 >= W || x - brush_size + 1 < 0 || y + brush_size - 1 >= H || y - brush_size + 1 < 0)
	{
		return;
	}

	// color current pixel
	pixels[y][x][0] = newColor[0];
	pixels[y][x][1] = newColor[1];
	pixels[y][x][2] = newColor[2];

	// color pixels around it
	for (int s = 1; s < brush_size; s++) {
		pixels[y + s][x][0] = newColor[0];
		pixels[y + s][x][1] = newColor[1];
		pixels[y + s][x][2] = newColor[2];

		pixels[y - s][x][0] = newColor[0];
		pixels[y - s][x][1] = newColor[1];
		pixels[y - s][x][2] = newColor[2];

		pixels[y][x + s][0] = newColor[0];
		pixels[y][x + s][1] = newColor[1];
		pixels[y][x + s][2] = newColor[2];

		pixels[y][x - s][0] = newColor[0];
		pixels[y][x - s][1] = newColor[1];
		pixels[y][x - s][2] = newColor[2];
	}

	
}

// compare pixel's color to another color
bool IsTheSameColor(int x, int y, unsigned char otherColor[3])
{
	return pixels[y][x][0] == otherColor[0] && pixels[y][x][1] == otherColor[1] && pixels[y][x][2] == otherColor[2];
}


// recursive implementation of flood filling - will not work on large segments!!!
void FloodFillRec(int x, int y, unsigned char newColor[3], unsigned char borderColor[3])
{
	// 1. paint pixel in new color
	PaintPixel(x, y, newColor);
	// 2. check UP
	if (y < H-1 && !IsTheSameColor(x, y + 1, newColor) && !IsTheSameColor(x, y + 1, borderColor))
		FloodFillRec(x, y + 1, newColor, borderColor);
	// 3. check RIGHT
	if (x<W-1 && !IsTheSameColor(x + 1, y, newColor) && !IsTheSameColor(x+ 1, y , borderColor))
		FloodFillRec(x + 1, y, newColor, borderColor);
	// 4. check DOWN
	if (y > 0 && !IsTheSameColor(x, y - 1, newColor) && !IsTheSameColor(x, y - 1, borderColor))
		FloodFillRec(x, y - 1, newColor, borderColor);
	// 5. check LEFT
	if (x > 0 && !IsTheSameColor(x- 1, y , newColor) && !IsTheSameColor(x - 1, y, borderColor))
		FloodFillRec(x- 1, y , newColor, borderColor);
}

// iterative implementation of flood filling - resolves recursive method problem :)
void FloodFillIter(int x, int y, unsigned char newColor[3], unsigned char borderColor[3])
{
	vector <Point2D*> stackOfPoints;
	Point2D* pCurrent;
	int row, col;

	replace_color[0] = pixels[y][x][0];
	replace_color[1] = pixels[y][x][1];
	replace_color[2] = pixels[y][x][2];

	stackOfPoints.push_back(new Point2D(x, y));

	while (!stackOfPoints.empty())
	{
		pCurrent = stackOfPoints.back();
		stackOfPoints.pop_back();
		row = pCurrent->getY();
		col = pCurrent->getX();
		if (col > 91) {
			PaintPixel(col, row, newColor);
		}

		// UP
		if (col > 91 && row < H - 1 && !IsTheSameColor(col, row + 1, newColor) && IsTheSameColor(col, row + 1, replace_color))
			stackOfPoints.push_back(new Point2D(col, row + 1));
		// RIGHT
		if (col > 91 && col < W - 1 && !IsTheSameColor(col + 1, row, newColor) && IsTheSameColor(col + 1, row, replace_color))
			stackOfPoints.push_back(new Point2D(col + 1, row));
		// DOWN
		if (col > 91 && row > 0 && !IsTheSameColor(col, row - 1, newColor) && IsTheSameColor(col, row - 1, replace_color))
			stackOfPoints.push_back(new Point2D(col, row - 1));
		// LEFT
		if (col > 91 && !IsTheSameColor(col - 1, row, newColor) && IsTheSameColor(col - 1, row, replace_color))
			stackOfPoints.push_back(new Point2D(col - 1, row));

	}
	
}

void saveImage() {

	FILE *imageFile;
	int x, y;

	numSaved++;

	imageFile = fopen("image.pgm", "wb");
	if (imageFile == NULL) {
		printf("FILE NOT OPENED!!!");
	}

	(void)fprintf(imageFile, "P6\n%d %d\n255\n", active_width, active_height);

	for (y = H-1; y >= H-active_height; y--) {
		for (x = W-active_width; x < W; x++) {
			
			static unsigned char color[3];
			color[0] = pixels[y][x][0];
			color[1] = pixels[y][x][1];
			color[2] = pixels[y][x][2];
			(void)fwrite(color, 1, 3, imageFile);
		}
	}

	fclose(imageFile); 
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	
	// draws the matrix pixels
	glDrawPixels(W, H, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	glutSwapBuffers(); // show all
}

void idle()
{
	glutPostRedisplay(); // sends a message to the window to run display function
}

void mouse(int button, int state, int x, int y)
{	

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (old_x < 0 || old_y < 0) {
			old_x = x;
			old_y = H - y; // y axis points downwards
		}

		if (x <= color_limit) // pick color
		{
			new_color[0] = pixels[H - y][x][0];
			new_color[1] = pixels[H - y][x][1];
			new_color[2] = pixels[H - y][x][2];

			brush_color[0] = new_color[0];
			brush_color[1] = new_color[1];
			brush_color[2] = new_color[2];
		}
		else // draw
		{			
			if (paint_state == FILL) {
				if (brush_size > 1) {
					int tmp_size = brush_size;
					brush_size = 1;
					FloodFillIter(x, H - y, new_color, border_color);
					brush_size = tmp_size;
				}
				else {
					FloodFillIter(x, H - y, new_color, border_color);
				}
			}
		}

	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		// Recursive version:
		//FloodFillRec(x, H - y, new_color, border_color);

		// Iterative version:
		FloodFillIter(x, H - y, new_color, border_color);
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		old_x = -1;
		old_y = -1;

		if (paint_state == EYEDROP) {
			new_color[0] = pixels[H - y][x][0];
			new_color[1] = pixels[H - y][x][1];
			new_color[2] = pixels[H - y][x][2];

			brush_color[0] = new_color[0];
			brush_color[1] = new_color[1];
			brush_color[2] = new_color[2];
		}
	}
}

void drawShape(int x, int y)
{
	if (x > color_limit && old_x > color_limit)
	{
		DrawLine(old_x, old_y, x, H - y);
		for (int s = 1; s < brush_size; s++) {
			DrawLine(old_x, old_y + s, x, H - y + s);
			DrawLine(old_x, old_y - s, x, H - y - s);
			DrawLine(old_x + s, old_y, x + s, H - y);
			DrawLine(old_x - s, old_y, x - s, H - y);
		}
		PaintPixel(x, H - y, brush_color);
	}

	old_x = x;
	old_y = H - y;
}

void menu(int choice)
{
	switch (choice)
	{
	case 0: // size-up brush

		saveImage();
		break;

	case 1: // size-up brush

		if (brush_size < 20) {
			brush_size += 1;
		}
		paint_state = DRAW;
		break;

	case 2:	// size-down brush

		if (brush_size > 1) {
			brush_size -= 1;
		}
		paint_state = DRAW;
		break;
	
	case 3:	// fill

		paint_state = FILL;
		break;
	
	case 4:	// draw

		paint_state = DRAW;
		break;

	case 5:	// eyedrop

		paint_state = EYEDROP;
		break;

	case 6:	// restart

		for (int i = 0; i < H; i++)
			for (int j = color_limit + 1; j < W; j++)
			{
				// White canvas
				pixels[i][j][0] = (unsigned char)(255);		// red
				pixels[i][j][1] = (unsigned char)(255);		// green
				pixels[i][j][2] = (unsigned char)(255);		// blue
			}
		break;
	}
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("Paint2.0");

	glutDisplayFunc(display);	// display is a name of refresh function
	glutIdleFunc(idle);			// idle is a function that runs in background
	glutMouseFunc(mouse);		// handle clicking events
	glutMotionFunc(drawShape);	// handle mouse dragging events

	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Save", 0);
	glutAddMenuEntry("(+) Brush", 1);
	glutAddMenuEntry("(-) Brush", 2);
	glutAddMenuEntry("Fill", 3);
	glutAddMenuEntry("Draw", 4);
	glutAddMenuEntry("Eyedrop", 5);
	glutAddMenuEntry("Restart", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();
	
	glutMainLoop();
	return 0;
}