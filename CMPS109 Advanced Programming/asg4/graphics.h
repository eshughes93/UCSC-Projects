/***************************************
 * Evan Hughes [eshughes]
 * Hadley Black [hablack]
 * cmps109 - Wesley Mackey - Spring 2014
 * assignment 4 - oop inheritance
 ***************************************/

// header file for window class

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <memory>
#include <vector>
using namespace std;

#include <GL/freeglut.h>

#include "rgbcolor.h"
#include "shape.h"

class object {
   private:
      shared_ptr<shape> pshape;
      vertex center;
      rgbcolor color;
      rgbcolor select_color;
      bool selected;
   public:
      // Default copiers, movers, dtor all OK.
      void center_check();
      bool get_sel() { return selected; }
      object(const shared_ptr<shape>, vertex&, rgbcolor&);
      void draw() { 
         if(selected) pshape->draw (center, select_color);
         else         pshape->draw(center,color); }
      void set_selected(bool s){ selected = s; }
      void move (GLfloat delta_x, GLfloat delta_y) {
         center.xpos += delta_x;
         center.ypos += delta_y;
      }
};

class mouse {
      friend class window;
   private:
      int xpos {0};
      int ypos {0};
      int entered {GLUT_LEFT};
      int left_state {GLUT_UP};
      int middle_state {GLUT_UP};
      int right_state {GLUT_UP};
   private:
      void set (int x, int y) { xpos = x; ypos = y; }
      void state (int button, int state);
      void draw();
};


class window {
      friend class mouse;
   private:
      static vector<object> objects;
      static size_t selected_obj;
      static mouse mus;
   private:
      static void close();
      static void entry (int mouse_entered);
      static void display();
      static void reshape (int width, int height);
      static void keyboard (GLubyte key, int, int);
      static void special (int key, int, int);
      static void motion (int x, int y);
      static void passivemotion (int x, int y);
      static void mousefn (int button, int state, int x, int y);
      static void move_selected_object(GLfloat xpos, GLfloat ypos);
      static void select_object(size_t obj);
   public:
      static int width;         // in pixels
      static int height;        // in pixels
      static int mb; //moveby value
      static GLfloat b_t; //border thickness 
      static GLfloat sel_bt;
      static rgbcolor b_c; //border color
      static void push_back(const object& obj)
      {objects.push_back(obj); cout << "pushing back " << endl;}
      static void setwidth (int width_) { width = width_; }
      static void setheight (int height_) { height = height_; }
      static void main();
      
};

#endif

