/*-----------------------------------------------
 * Author: Yue Chen
 * Date: 2.13.2023
 * Description: The class have a set of methods that can draw 
 * lines and triangles
 ----------------------------------------------*/

#ifndef canvas_H_
#define canvas_H_

#include <string>
#include <vector>
#include "image.h"

namespace agl
{
   struct Vertice {
      int x;
      int y;
   };

   enum PrimitiveType {UNDEFINED, LINES, TRIANGLES};
   class Canvas
   {
   public:
      Canvas(int w, int h);
      virtual ~Canvas();

      // Save to file
      void save(const std::string& filename);

      // Draw primitives with a given type (either LINES or TRIANGLES)
      // For example, the following draws a red line followed by a green line
      // begin(LINES);
      //    color(255,0,0);
      //    vertex(0,0);
      //    vertex(100,0);
      //    color(0,255,0);
      //    vertex(0, 0);
      //    vertex(0,100);
      // end();
      void begin(PrimitiveType type);
      void end();

      // Get the width of the canvas
      int getW();
      // Get the height of the canvas
      int getH();
      // Set the pixel RGB color at index (x, y) 
      void set(int x, int y, Pixel pixel);

      // Specifiy a vertex at raster position (x,y)
      // x corresponds to the column; y to the row
      void vertex(int x, int y);

      // Specify a color. Color components are in range [0,255]
      void color(unsigned char r, unsigned char g, unsigned char b);

      // Fill the canvas with the given background color
      void background(unsigned char r, unsigned char g, unsigned char b);

      // Draw a line from vertice 1 to vertice 2
      void drawLine(Vertice v1, Vertice v2);

      void drawLineLow(Vertice v1, Vertice v2, Pixel color1, Pixel color2);

      void drawLineHigh(Vertice v1, Vertice v2, Pixel color1, Pixel color2);

      // Draw a triangle according to three vertices
      void drawTriangle(Vertice v1, Vertice v2, Vertice v3);


   private:
      std::vector<Vertice> _positions;
      PrimitiveType _type;
      Pixel _color;
      Image _canvas;
      std::vector<Pixel> _colors;
   };
}

#endif


