#include "graphics.h"
#include "drawing.h"

#include <GL/glut.h>
#include <Math.h>
#include <cfloat>
#include <cmath>

using namespace std;

#define PI 3.14159265f
#define CIRCLE_SLICES 200;

const colorType HIGHLIGHT_COLOR = WHITE;

const double TRANSFORM_GISMO_SCALE = 25.0;



bool highlight = false;

bool getHighlight()
{
  return highlight;
}

void setHighlight(bool h)
{
  if (h) setColor(HIGHLIGHT_COLOR);
  highlight = h;
}


colorType intToColor(int item)
{
  switch ( item ) {
    case 0 : return BLACK   ;
    case 1 : return BLUE    ;
    case 2 : return GREEN   ;
    case 3 : return CYAN    ;
    case 4 : return RED     ;
    case 5 : return MAGENTA ;
    case 6 : return YELLOW  ;
    case 7 : return WHITE   ;
    case 8 : return GRAY    ;
   default : return BLACK   ;
  }
}


void setColor( colorType color )
{
  if (!highlight)
    switch ( color )
    {
      case BLACK   : glColor3f( 0.0, 0.0, 0.0 ); break;
      case BLUE    : glColor3f( 0.0, 0.0, 1.0 ); break;
      case GREEN   : glColor3f( 0.0, 1.0, 0.0 ); break;
      case CYAN    : glColor3f( 0.0, 1.0, 1.0 ); break;
      case RED     : glColor3f( 1.0, 0.0, 0.0 ); break;
      case MAGENTA : glColor3f( 1.0, 0.0, 1.0 ); break;
      case YELLOW  : glColor3f( 1.0, 1.0, 0.0 ); break;
      case WHITE   : glColor3f( 1.0, 1.0, 1.0 ); break;
      case GRAY    : glColor3f( 0.5, 0.5, 0.5 ); break;
    }
  else if (highlight)
	  return;
}



TransformStack transformStack;

TransformStack::TransformStack()
{
	theStack.push(new Matrix());
}

void TransformStack::push(Matrix* transform)
{
	theStack.push(transform);
}

void TransformStack::pop()
{
	theStack.pop();
}

Matrix* TransformStack::top()
{

	return theStack.top();
}


void gPush(Matrix* transform)
{
	transformStack.push(transformStack.top()->multiply(transform));
}

void gPop()
{
	transformStack.pop();
}

void drawLine(double x0, double y0, double x1, double y1)
{
	drawLine(new Vector(x0, y0), new Vector(x1, y1));
}

void drawLine(Vector* p0, Vector* p1)
{
	Vector* q0 = transformStack.top()->multiply(p0);
	Vector* q1 = transformStack.top()->multiply(p1);

	glBegin(GL_LINES);
	glVertex2i((*q0)[0], (*q0)[1]);
	glVertex2i((*q1)[0], (*q1)[1]);
	glEnd();
	glFlush();
}

void drawRectangle(double x0, double y0, double x1, double y1)

{
	drawLine(new Vector(x0,y0), new Vector(x1,y0));
	drawLine(new Vector(x1, y0), new Vector(x1, y1));
	drawLine(new Vector(x1, y1), new Vector(x0, y1));
	drawLine(new Vector(x0, y1), new Vector(x0, y0));

}

void drawCircle(double x0, double y0, double x1, double y1)
{
	double xDif = x1 - x0;
	double yDif = y1 - y0;
	double rad = sqrt(xDif*xDif + yDif*yDif);
	drawCircle(x0, y0, rad);
}

void drawCircle(double cX, double cY, double radius)
{
	GLdouble theta, delta;
	delta = 2 * PI / CIRCLE_SLICES;
	for (theta = 0; theta < 2 * PI; theta += delta) {
		Vector* point1 = new Vector(cX + radius * cos(theta), cY + radius * sin(theta));
		Vector* point2 = new Vector(cX + radius * cos(theta + delta), cY + radius * sin(theta + delta));
			drawLine(point1, point2);
	}
	
}

void drawPolygon(const list<Vector*>& vertices, bool close)
{
	list<Vector*>::const_iterator iter;
	glBegin(GL_LINE_LOOP);
	iter = vertices.begin();
	while (iter != vertices.end()) {
		Vector* p = *iter;
		double x = (*p)[0];
		double y = (*p)[1];
		glVertex2d(x, y);
		
		iter++;
	}
	glEnd();
}

void drawTransformGismo()
{
  double halfSize = TRANSFORM_GISMO_SCALE/2.0;
  drawRectangle(-halfSize,-halfSize,halfSize,halfSize);
}
