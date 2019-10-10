#ifndef FIGURE_H
#define FIGURE_H

#include <list>
#include <map>
#include <set>
#include "matrix.h"
#include "graphics.h"

using namespace std;

class TransformNode; 
class ShapeNode;


class TransformNode
{
  public:
   TransformNode(TransformNode* parent);
   TransformNode(TransformNode* parent, ShapeNode* shapeNode, Matrix* transform);
   ~TransformNode();
   void translate(double deltaX, double deltaY);
   void rotate(double theta);
   void shear(double shearXY, double shearYX);
   void scale(double scaleX, double scaleY);
   virtual void draw(bool displayHelpers) const;
   TransformNode* getParent() const;
   void setParent(TransformNode* parent);
   void changeParent(TransformNode* newParent);
   void groupObjects(set<TransformNode*>& groupMembers);
   Matrix* getTransform() const;
   virtual TransformNode* clone() const;
   void addChild(TransformNode* child);
   void removeChild(TransformNode* child);
   TransformNode* firstChild() const;
   TransformNode* lastChild() const;
   TransformNode* nextChild(TransformNode* child) const;
   TransformNode* previousChild(TransformNode* child) const;
   Matrix* CWT();
   void select();
   void deSelect();
   static TransformNode* nodeLookup(int identifier);
   static int count;
  private:
	  TransformNode* parent;
	  list<TransformNode*> children;
	  Matrix* mat;
	  bool selected;
	  int identifier;
	  static map<int, TransformNode*> lookUpTable;
  protected:
	ShapeNode* shapeNode;
	  
};

class ShapeNode
{
  public:
   ShapeNode(colorType color);
   virtual void draw() const = 0;
   void setTransformNode(TransformNode* transformNode);
   TransformNode* getTransformNode();
   virtual ShapeNode* clone() const = 0;
  private:
	  TransformNode* shapeTran;
  protected:
	  colorType color;
	  
};

class Line : public ShapeNode
{
  public:
   Line(double x0, double y0, double x1, double y1, colorType c);
   virtual ShapeNode* clone() const;
   virtual void draw() const;
  private:
	  double x0, y0, x1, y1;
};

class Rectangle : public ShapeNode
{
  public:
   Rectangle(double x0, double y0, double x1, double y1, colorType c);
   virtual ShapeNode* clone() const;
   virtual void draw() const;
  private:
	  double x0, y0, x1, y1;
};


class Circle : public ShapeNode
{
  public:
   Circle(double cX, double cY, double radius, colorType c);
   virtual ShapeNode* clone() const;
   virtual void draw() const;
  private:
	  double cX, cY, radius;
};

class Polygon : public ShapeNode
{
  public:
   Polygon(const list<Vector*>& vertices, colorType c);
   ~Polygon();
   virtual ShapeNode* clone() const;
   virtual void draw() const;
  private:
	   list<Vector*> vertices;
};

#endif
