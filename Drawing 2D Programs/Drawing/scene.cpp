#include <iostream>
#include <algorithm>
#include <GL/glut.h>
#include "scene.h"


int TransformNode::count = 0;
map<int, TransformNode*> TransformNode::lookUpTable;

TransformNode::TransformNode(TransformNode* p)
: parent(p), shapeNode(NULL), mat(new Matrix()), selected(false), identifier(count++)
{
	lookUpTable[identifier] = this;
}

TransformNode::TransformNode(TransformNode* p, ShapeNode* s, Matrix* t)
: parent(p), shapeNode(s), mat(t), selected(false), identifier(count++)
{
	lookUpTable[identifier] = this;
}



TransformNode::~TransformNode()
{
	list<TransformNode*>::const_iterator iter;
	for (iter = children.begin(); iter != children.end(); iter++) {
		delete (*iter);
	}
}

Matrix* TransformNode::CWT() {
	if (parent == NULL)
		return mat;
	else {
		
		 return parent->CWT()->multiply(mat);
	}
	
}

void TransformNode::translate(double deltaX, double deltaY)
{
	Matrix* temp = Matrix::translation(deltaX, deltaY);
	Matrix* cWorldTrans = CWT();
	Matrix* invCwt = cWorldTrans->getInverse();
	
	Matrix* firstProd = mat->multiply(invCwt);
	Matrix* secondProd = firstProd->multiply(temp);
	Matrix* product = secondProd->multiply(cWorldTrans);
	
	delete mat;
	mat = product;
}

void TransformNode::rotate(double theta)
{
	Matrix* temp = Matrix::rotation(theta);
	Matrix* cWorldTrans = CWT();
	Matrix* invCwt = cWorldTrans->getInverse();

	Matrix* firstProd = mat->multiply(invCwt);
	Matrix* secondProd = firstProd->multiply(temp);
	Matrix* product = secondProd->multiply(cWorldTrans);

	delete mat;
	mat = product;
	
}

void TransformNode::shear(double shearXY, double shearYX)
{
	Matrix* temp = Matrix::shearing(shearXY,shearYX);
	Matrix* cWorldTrans = CWT();
	Matrix* invCwt = cWorldTrans->getInverse();

	Matrix* firstProd = mat->multiply(invCwt);
	Matrix* secondProd = firstProd->multiply(temp);
	Matrix* product = secondProd->multiply(cWorldTrans);

	delete mat;
	mat = product;
}

void TransformNode::scale(double scaleX, double scaleY)
{
	Matrix* temp = Matrix::scaling(scaleX,scaleY);
	Matrix* cWorldTrans = CWT();
	Matrix* invCwt = cWorldTrans->getInverse();

	Matrix* firstProd = mat->multiply(invCwt);
	Matrix* secondProd = firstProd->multiply(temp);
	Matrix* product = secondProd->multiply(cWorldTrans);

	delete mat;
	mat = product;
}

void TransformNode::draw(bool displayHelpers) const
{	
	bool currentHighlightStatus = getHighlight();

	if (selected)
		setHighlight(true);
	gPush(mat);
	glPushName(identifier);
	if (shapeNode != NULL)
		shapeNode->draw();

	list<TransformNode*>::const_iterator iter;
	for (iter = children.begin(); iter != children.end(); iter++) {
		(*iter)->draw(displayHelpers);
		
	}

	setHighlight(currentHighlightStatus);
	glPopName();
	gPop();

}

TransformNode* TransformNode::getParent() const 
{ 
   return parent;
}

void TransformNode::setParent(TransformNode* p) 
{ 
	parent = p;
}

void TransformNode::changeParent(TransformNode* newParent)
{
	parent = newParent;
}

void TransformNode::groupObjects(set<TransformNode*>& groupMembers)
{
	TransformNode* newT = new TransformNode(this);
	set<TransformNode*>::const_iterator iter;

	for (iter = groupMembers.begin(); iter != groupMembers.end(); iter++) 
		addChild(*iter);

}

Matrix* TransformNode::getTransform() const
{
   return mat;
}

TransformNode* TransformNode::clone() const
{
	return NULL;
}

void TransformNode::addChild(TransformNode* child)
{
	children.push_back(child);
}

void TransformNode::removeChild(TransformNode* child)
{
	children.pop_back();
}

TransformNode* TransformNode::firstChild() const
{
	return NULL;
	list<TransformNode*>::const_iterator iter;
	iter = children.begin();
		return (*iter);
		
}

TransformNode* TransformNode::lastChild() const
{
	list<TransformNode*>::const_iterator iter;
	iter = children.end();
	return (*iter);
}

TransformNode* TransformNode::nextChild(TransformNode* child) const
{
	list<TransformNode*>::const_iterator iter;

	list<TransformNode*>::const_iterator index;

	for (iter = children.begin(); iter != children.end(); iter++) {
		if ((*iter) == child)
			index = iter;
	}

	if (index == children.end())
		throw new exception_ptr;
	
	else {
		index++;
		return *index;
	}
		

}

TransformNode* TransformNode::previousChild(TransformNode* child) const
{
	list<TransformNode*>::const_iterator iter;

	list<TransformNode*>::const_iterator index;

	for (iter = children.begin(); iter != children.end(); iter++) {
		if ((*iter) == child)
			index = iter;
	}

	if (index == children.begin())
		throw new exception_ptr;

	else {
		index--;
		return *index;
	}
}


void TransformNode::select() 
{
	selected = true;
}

void TransformNode::deSelect() 
{
	selected = false;
}

TransformNode* TransformNode::nodeLookup(int identifier)
{
	return lookUpTable[identifier];
}


ShapeNode::ShapeNode(colorType c) 
	: color(c)
{
}

void ShapeNode::setTransformNode(TransformNode* tn)
{
	shapeTran = tn;
}

TransformNode* ShapeNode::getTransformNode()
{
	return shapeTran;
}

Line::Line(double xx0, double yy0, double xx1, double yy1, colorType c)
	:ShapeNode(c), x0(xx0), y0(yy0), x1(xx1), y1(yy1)
{
}


ShapeNode* Line::clone() const
{
   return NULL;
}

void Line::draw() const
{
	setColor(color);
	drawLine(x0, y0, x1, y1);
}



Rectangle::Rectangle(double xx0, double yy0, double xx1, double yy1, colorType c)
	: ShapeNode(c), x0(xx0), y0(yy0), x1(xx1), y1(yy1)
{
}


ShapeNode* Rectangle::clone()  const
{
   return NULL;
}

void Rectangle::draw() const
{
	setColor(color);
	drawRectangle(x0, y0, x1, y1);

}

Circle::Circle(double ccX, double ccY, double r, colorType c) 
	: ShapeNode(c), cX(ccX), cY(ccY), radius(r)

{}

ShapeNode* Circle::clone() const
{
   return NULL;
}

void Circle::draw() const
{
	setColor(color);
	drawCircle(cX, cY, radius);
}


Polygon::Polygon(const list<Vector*>& vs, colorType c) 
	: ShapeNode(c)
{
	for (list<Vector*>::const_iterator iter = vs.begin(); iter != vs.end(); iter++) {
		Vector* vp = *iter;
		vertices.push_back(new Vector(*vp));
			
	}
}

Polygon::~Polygon()
{

}

ShapeNode* Polygon::clone() const
{
   return NULL;
}

void Polygon::draw() const
{
	setColor(color);
	drawPolygon(vertices);
}

