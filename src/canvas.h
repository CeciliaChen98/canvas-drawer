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
   struct Vertex {
      int x;
      int y;
   };

   enum PrimitiveType {UNDEFINED, LINES, TRIANGLES, CIRCLES, POINTS, RECTANGLES, STARS, FLOWERS};
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

      //Specify a radius
      // r corresponds to the radius
      void radius(int r);

      //Specify the number of petals
      void setPetal(int p);

      // Specify a color. Color components are in range [0,255]
      void color(unsigned char r, unsigned char g, unsigned char b);

      // Fill the canvas with the given background color
      void background(unsigned char r, unsigned char g, unsigned char b);

      // Draw a line from vertex 1 to vertex 2
      void drawLine(Vertex v1, Vertex v2);

      void drawLineLow(Vertex v1, Vertex v2, Pixel color1, Pixel color2);

      void drawLineHigh(Vertex v1, Vertex v2, Pixel color1, Pixel color2);

      // Draw a triangle according to three vertices
      void drawTriangle(Vertex v1, Vertex v2, Vertex v3);

      //Draw a circe according to its radius and its center position
      void drawCircle(Vertex v, int r);

      //Draw a rectangle according to two vertices
      void drawRectangle(Vertex v1, Vertex v2);

      // Draw a triangle according to three vertices
      void drawPureTriangle(Vertex v1, Vertex v2, Vertex v3, Pixel);

      //Draw a star according to its center position and its radius
      void drawStar(Vertex v, int radius);

      //Draw a flower according to its center position, radius, and its number of petals
      void drawFlower(Vertex v, int num_petals, int radius);


   private:
      std::vector<Vertex> _positions;
      std::vector<int> _radius;
      std::vector<int> _petals;
      std::vector<Pixel> _colors;
      PrimitiveType _type;
      int _petal;
      Pixel _color;
      Image _canvas;
   };
}

#endif


