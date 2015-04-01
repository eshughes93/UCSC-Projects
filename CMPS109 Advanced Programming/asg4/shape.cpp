/***************************************
 * Evan Hughes [eshughes]
 * Hadley Black [hablack]
 * cmps109 - Wesley Mackey - Spring 2014
 * assignment 4 - oop inheritance
 ***************************************/

// implementation file for shapes using inheritance

#include <typeinfo>
#include <unordered_map>
#include <cmath>
using namespace std;

#include "shape.h"
#include "util.h"
#include "graphics.h"

static unordered_map<void*,string> fontname {
   {GLUT_BITMAP_8_BY_13       , "Fixed-8x13"    },
   {GLUT_BITMAP_9_BY_15       , "Fixed-9x15"    },
   {GLUT_BITMAP_HELVETICA_10  , "Helvetica-10"  },
   {GLUT_BITMAP_HELVETICA_12  , "Helvetica-12"  },
   {GLUT_BITMAP_HELVETICA_18  , "Helvetica-18"  },
   {GLUT_BITMAP_TIMES_ROMAN_10, "Times-Roman-10"},
   {GLUT_BITMAP_TIMES_ROMAN_24, "Times-Roman-24"},
};

static unordered_map<string,void*> fontcode {
   {"Fixed-8x13"    , GLUT_BITMAP_8_BY_13       },
   {"Fixed-9x15"    , GLUT_BITMAP_9_BY_15       },
   {"Helvetica-10"  , GLUT_BITMAP_HELVETICA_10  },
   {"Helvetica-12"  , GLUT_BITMAP_HELVETICA_12  },
   {"Helvetica-18"  , GLUT_BITMAP_HELVETICA_18  },
   {"Times-Roman-10", GLUT_BITMAP_TIMES_ROMAN_10},
   {"Times-Roman-24", GLUT_BITMAP_TIMES_ROMAN_24},
};

ostream& operator<< (ostream& out, const vertex& where) {
   out << "(" << where.xpos << "," << where.ypos << ")";
   return out;
}

shape::shape() {
   DEBUGF ('c', this);
}

// -- text ctor -- //
text::text (const string& f, const string& textdata):
      font(fontcode[f]), textdata(textdata) {
   DEBUGF ('c', this);
}

// -- ellipse ctor -- //
ellipse::ellipse (GLfloat width, GLfloat height):
                     dimension ({width, height}) {
   DEBUGF ('c', this);
}

// -- circle ctor -- //
circle::circle (GLfloat diameter): ellipse (diameter, diameter) {
   DEBUGF ('c', this);
}

// -- polygon ctor -- //
polygon::polygon (const vertex_list& vertices): vertices(vertices) {
   DEBUGF ('c', this);
}

// -- rectangle ctor -- //
rectangle::rectangle (GLfloat w, GLfloat h): 
            polygon({}), width(w), height(h) {
   DEBUGF ('c', this << "(" << width << "," << height << ")");
}

// -- square ctor -- //
square::square (GLfloat w): rectangle (w, w), width(w) {
   DEBUGF ('c', this);
}

// -- diamond ctor -- //
diamond::diamond (GLfloat w, GLfloat h): 
            polygon({}), width(w), height(h) {
   DEBUGF ('c', this << "(" << width << "," << height << ")");
}

// -- triangle ctor -- //
triangle::triangle(const vertex_list& vertices):polygon(vertices){}

// -- right triangle ctor -- //
right_triangle::right_triangle(const GLfloat w, const GLfloat h):
                  triangle({}), width(w), height(h){}

// -- isosceles triangle ctor -- //
isoceles::isoceles(const GLfloat w, const GLfloat h):triangle({}),
                    width(w), height(h){}

// -- equilateral triangle ctor -- //
equilateral::equilateral(const GLfloat w):triangle({}),
                    width(w){}
 

// -- draw function for text -- //
void text::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   glColor3ubv(color.ubvec);
   GLfloat w = glutBitmapLength (font, (GLubyte*) textdata.c_str());
   GLfloat h = glutBitmapHeight (font);
   GLfloat xpos = center.xpos - (w / 2.0);
   GLfloat ypos = center.ypos - (h / 4.0);
 
   glRasterPos2f(xpos,ypos);
  
   for (auto ch: textdata) glutBitmapCharacter (font, ch);
}

// -- draw function for ellipse -- //
void ellipse::draw (const vertex& center, const rgbcolor& color) const{
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   const GLfloat delta = 2*M_PI / 32.0;
   glColor3ubv(color.ubvec);
   glBegin(GL_LINE_LOOP);
   for(GLfloat theta = 0; theta < (2*M_PI); theta+=delta){
      GLfloat xpos = (dimension.xpos / 2.0) * cos(theta) + center.xpos;
      GLfloat ypos = (dimension.ypos / 2.0) * sin(theta) + center.ypos;
      glVertex2f(xpos,ypos);
   }
   glEnd();
}

// -- draw function for polygon -- //
void polygon::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   glColor3ubv(color.ubvec);

   GLfloat xsum, ysum, xavg, yavg = 0;
   GLfloat count = vertices.size();
   for(auto i = vertices.begin(); i != vertices.end(); ++i) {
      xsum += i->xpos;
      ysum += i->ypos;
   }

   xavg = xsum / count;
   yavg = ysum / count;
   

   glBegin(GL_LINE_LOOP);
   for(auto i = vertices.begin(); i != vertices.end(); ++i) {
      glVertex2f(i->xpos-xavg+center.xpos,i->ypos-yavg+center.ypos);
   }
   glEnd();

}

// -- draw function for triangle -- //
void triangle::draw(const vertex& center, const rgbcolor& color) const{
   polygon::draw(center, color);
}

// -- draw function for rectangle -- //
void rectangle::draw(const vertex& center,const rgbcolor& color)const {
   glBegin(GL_LINE_LOOP);
   
   glColor3ubv(color.ubvec);
   GLfloat x0 = center.xpos - (width / 2);
   GLfloat y0 = center.ypos + (height / 2);

   GLfloat x1 = center.xpos + (width / 2);
   GLfloat y1 = center.ypos + (height / 2);

   GLfloat x2 = center.xpos + (width / 2);
   GLfloat y2 = center.ypos - (height / 2);

   GLfloat x3 = center.xpos - (width / 2);
   GLfloat y3 = center.ypos - (height / 2);

   glVertex2f(x0,y0);
   glVertex2f(x1,y1);
   glVertex2f(x2,y2);
   glVertex2f(x3,y3); 

   glEnd();
}

// -- draw function for diamond -- //
void diamond::draw(const vertex& center, const rgbcolor& color) const{

   glColor3ubv(color.ubvec);

   GLfloat topx = center.xpos;
   GLfloat topy = center.ypos + (height / 2);
   GLfloat botx = center.xpos;
   GLfloat boty = center.ypos - (height / 2);
   GLfloat lefx = center.xpos - (width / 2);
   GLfloat lefy = center.ypos;
   GLfloat ritx = center.xpos + (width / 2);
   GLfloat rity = center.ypos;

   glBegin(GL_LINE_LOOP);
   glVertex2f(topx, topy);
   glVertex2f(lefx, lefy);
   glVertex2f(botx, boty);
   glVertex2f(ritx, rity);
   glEnd();
}

// -- draw function for right_triangle -- //
void right_triangle::
                 draw(const vertex& center,const rgbcolor& color)const{
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   glColor3ubv(color.ubvec);

   GLfloat x0 = center.xpos - ((2.0/3.0)*(width));
   GLfloat y0 = center.ypos - ((1.0/3.0)*(height));
   GLfloat x1 = center.xpos + ((1.0/3.0)*(width));
   GLfloat y1 = center.ypos + ((2.0/3.0)*(height));
   GLfloat x2 = center.xpos + ((1.0/3.0)*(width));
   GLfloat y2 = center.ypos - ((1.0/3.0)*(height));
 
   glBegin(GL_LINE_LOOP);
   glEnable(GL_LINE_SMOOTH);
   glVertex2f(x0,y0);
   glVertex2f(x1,y1);
   glVertex2f(x2,y2);
   glEnd();
}

// -- draw function for isosceles -- //
void isoceles::draw(const vertex& center, const rgbcolor& color) const{
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   glColor3ubv(color.ubvec);

   GLfloat x0 = center.xpos - (width/2);
   GLfloat y0 = center.ypos - (height/2);
   GLfloat x1 = center.xpos;
   GLfloat y1 = center.ypos + (height/2);
   GLfloat x2 = center.xpos + (width/2);
   GLfloat y2 = center.ypos - (height/2);\

   glBegin(GL_LINE_LOOP);
   glVertex2f(x0,y0);
   glVertex2f(x1,y1);
   glVertex2f(x2,y2);
   glEnd();
}

// -- draw function for equilateral -- //
void equilateral::
                 draw(const vertex& center,const rgbcolor& color)const{
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   glColor3ubv(color.ubvec);
   GLfloat x0 = center.xpos - (width/2);
   GLfloat y0 = center.ypos - (width/2);
   GLfloat x1 = center.xpos;
   GLfloat y1 = center.ypos + (width/2);
   GLfloat x2 = center.xpos + (width/2);
   GLfloat y2 = center.ypos - (width/2);

   glBegin(GL_LINE_LOOP);
   glVertex2f(x0,y0);
   glVertex2f(x1,y1);
   glVertex2f(x2,y2);
   glEnd();
}

// -- draw function for shape -- //
void shape::show (ostream& out) const {
   out << this << "->" << demangle (*this) << ": ";
}

void text::show (ostream& out) const {
   shape::show (out);
   out << font << "(" << fontname[font]
       << ") \"" << textdata << "\"";
}

void ellipse::show (ostream& out) const {
   shape::show (out);
   out << "{" << dimension << "}";
}

void polygon::show (ostream& out) const {
   shape::show (out);
   out << "{" << vertices << "}";
}

ostream& operator<< (ostream& out, const shape& obj) {
   obj.show (out);
   return out;
}

