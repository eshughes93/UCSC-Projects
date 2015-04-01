/***************************************
 * Evan Hughes [eshughes]
 * Hadley Black [hablack]
 * cmps109 - Wesley Mackey - Spring 2014
 * assignment 4 - oop inheritance
 ***************************************/

// header file for shapes using inheritance

#ifndef __SHAPE_H__
#define __SHAPE_H__

#include <iomanip>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
using namespace std;

#include "rgbcolor.h"

//
// Shapes constitute a single-inheritance hierarchy, summarized
// here, with the superclass listed first, and subclasses indented
// under their immediate superclass.
//
// shape
//    text
//    ellipse
//       circle
//    polygon
//       rectangle
//          square
//       diamond
//       triangle
//          right_triangle
//          isosceles
//          equilateral
//

class shape;
typedef struct {GLfloat xpos; GLfloat ypos; } vertex;
typedef vector<vertex> vertex_list;
typedef shared_ptr<shape> shape_ptr; 

//
// Abstract base class for all shapes in this system.
//

class shape {
   friend ostream& operator<< (ostream& out, const shape&);
   private:
      shape (const shape&) = delete; // Prevent copying.
      shape& operator= (const shape&) = delete; // Prevent copying.
   protected:
      inline shape(); // Only subclass may instantiate.
   public:
      virtual ~shape() {}
      virtual void draw (const vertex&, const rgbcolor&) const = 0;
      virtual void show (ostream&) const;
};


//
// Class for printing text.
//

class text: public shape {
   protected:
      void* font = nullptr;
      // GLUT_BITMAP_8_BY_13
      // GLUT_BITMAP_9_BY_15
      // GLUT_BITMAP_HELVETICA_10
      // GLUT_BITMAP_HELVETICA_12
      // GLUT_BITMAP_HELVETICA_18 
      // GLUT_BITMAP_TIMES_ROMAN_10
      // GLUT_BITMAP_TIMES_ROMAN_24
      string textdata;
   public:
      virtual ~text(){}
      text (const string& font, const string& textdata);
      virtual void draw (const vertex&, const rgbcolor&) const override;
      virtual void show (ostream&) const override;
};

//
// Classes for ellipse and circle.
//

class ellipse: public shape {
   protected:
      vertex dimension;
   public:
      ellipse (GLfloat width, GLfloat height);
      virtual ~ellipse(){}
      virtual void draw (const vertex&, const rgbcolor&) const override;
      virtual void show (ostream&) const override;
};

class circle: public ellipse {
   public:
      circle (GLfloat diameter);
};

//
// Class polygon.
//

class polygon: public shape {
   protected:
      const vertex_list vertices;
   public:
      polygon (const vertex_list& vertices);
      virtual ~polygon(){}
      virtual void draw (const vertex&, const rgbcolor&) const override;
      virtual void show (ostream&) const override;
};


//
// Class rectangle -
//

class rectangle: public polygon {
   private:
      GLfloat width, height;
   public:
      virtual ~rectangle(){}
      rectangle (GLfloat width, GLfloat height);
      virtual void draw(const vertex&, const rgbcolor&) const override;
};

//
// Class square -
//

class square: public rectangle {
   private:
      GLfloat width;
   public:
      square (GLfloat width);
};

//
// Class diamond -
//

class diamond: public polygon {
   private:
      GLfloat width, height;
   public:
      virtual ~diamond(){}
      virtual void draw(const vertex&, const rgbcolor&) const override;
      diamond (const GLfloat width, const GLfloat height);
};

//
// Class triangle -
//

class triangle: public polygon {
   public:
      virtual ~triangle(){}
      virtual void draw(const vertex&, const rgbcolor&) const override;
      triangle(const vertex_list& vertices);
};

//
// Class right triangle -
//

class right_triangle: public triangle {
   private:
      GLfloat width;
      GLfloat height;
   public:
      virtual ~right_triangle(){}
      virtual void draw(const vertex&, const rgbcolor&) const override;
      right_triangle(const GLfloat width, const GLfloat height);
};

//
// Class isosceles triangle -
//

class isoceles: public triangle{
   private:
      GLfloat width;
      GLfloat height;
   public:
      virtual ~isoceles(){}
      virtual void draw(const vertex&, const rgbcolor&) const override;
      isoceles(const GLfloat width, const GLfloat height);
};

//
// Class equilateral triangle -
//

class equilateral: public triangle{
   private:
      GLfloat width;
   public:
      virtual ~equilateral(){}
      virtual void draw(const vertex&, const rgbcolor&) const override;
      equilateral(const GLfloat width);
};

ostream& operator<< (ostream& out, const shape&);

#endif

