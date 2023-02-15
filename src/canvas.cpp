#include "canvas.h"
#include <cassert>
#include "math.h"

using namespace std;
using namespace agl;



Canvas::Canvas(int w, int h) : _canvas(w, h)
{
}

Canvas::~Canvas()
{
}

void Canvas::save(const std::string& filename)
{
   _canvas.save(filename);
}

void Canvas::begin(PrimitiveType type)
{
   _type = type;
}

void Canvas::end()
{
   if(_type == LINES ){
      while(!_positions.empty()){
         Vertice v2 = _positions.back();
         _positions.pop_back();
         Vertice v1 = _positions.back();
         _positions.pop_back();
         drawLine(v1,v2);
      }
   }else if(_type == TRIANGLES){
      while(!_positions.empty()){
         Vertice v3 = _positions.back();
         _positions.pop_back();
         Vertice v2 = _positions.back();
         _positions.pop_back();
         Vertice v1 = _positions.back();
         _positions.pop_back();
         drawTriangle(v1,v2,v3);
      }
   }
   _type = UNDEFINED;
}

int Canvas::getW(){
   return _canvas.width();
}

int Canvas::getH(){
   return _canvas.height();
}

void Canvas::set(int x, int y, Pixel pixel){
   _canvas.set(y,x,pixel);
}

void Canvas::vertex(int x, int y)
{
   _positions.push_back({x,y});
   if(_positions.size()!=_colors.size()){
      _colors.push_back(_color);
   }
}

void Canvas::color(unsigned char r, unsigned char g, unsigned char b)
{
   _color = {r,g,b};
   _colors.push_back(_color);
}


void Canvas::background(unsigned char r, unsigned char g, unsigned char b)
{
   Pixel color = {r,g,b};
   for(int i=0;i<getW();i++){
      for(int j=0;j<getH();j++){
         set(i,j,color);
      }
   }
}

//implementing Bresenham Algorithm
void Canvas::drawLine(Vertice v1, Vertice v2){
   //access the colors 
   Pixel color2 = _colors.back();
   _colors.pop_back();
   Pixel color1 = _colors.back();
   _colors.pop_back();

   int W = v2.x-v1.x;
   int H = v2.y-v1.y;
   if(std::abs(H)<std::abs(W)){
      if(v1.x>v2.x){
         Vertice temp = v1;
         v1 = v2;
         v2 = temp;

         Pixel tempColor = color1;
         color1 = color2;
         color2 = tempColor;
      }
      drawLineLow(v1,v2,color1,color2);
   }else{
      if(v1.y>v2.y){
         Vertice temp = v1;
         v1 = v2;
         v2 = temp;

         Pixel tempColor = color1;
         color1 = color2;
         color2 = tempColor;
      }
      drawLineHigh(v1,v2,color1,color2);
   }
}

//to draw a line whose |slope| < 1
void Canvas::drawLineLow(Vertice v1, Vertice v2, Pixel color1, Pixel color2){
   
   float t = 0;//help to decide the color
   Pixel color;

   int y = v1.y;
   int W = v2.x-v1.x;
   int H = v2.y-v1.y;
   int dy = 1;
   if(H<0){
      dy = -1;
      H = -H;
   }
   int F = 2*H-W;
   for(int x = v1.x; x < v2.x; x++){
      //calculate the color
      unsigned char r = color1.r * (1-t/W)+ color2.r * (t/W);
      unsigned char g = color1.g * (1-t/W)+ color2.g * (t/W);  
      unsigned char b = color1.b * (1-t/W)+ color2.b * (t/W);
      color = {r,g,b};    
      set(x,y,color);
      if(F>0){
         y+=dy;
         F+=2*(H-W);
      }else{
         F+=(2*H);
      }
      t++;
   }
}

//to draw a line whose |slope| >= 1
void Canvas::drawLineHigh(Vertice v1, Vertice v2, Pixel color1, Pixel color2){

   float t = 0;//help to decide the color
   Pixel color;

   int x = v1.x;
   int W = v2.x-v1.x;
   int H = v2.y-v1.y;
   int dx = 1;
   if(W<0){
      dx = -1;
      W = -W;
   }
   int F = 2*W-H;
   for(int y = v1.y; y < v2.y; y++){
      //calculate the color
      unsigned char r = color1.r * (1-t/H)+ color2.r * (t/H);
      unsigned char g = color1.g * (1-t/H)+ color2.g * (t/H);  
      unsigned char b = color1.b * (1-t/H)+ color2.b * (t/H);
      color = {r,g,b};    
      set(x,y,color);
      if(F>0){
         x+=dx;
         F+=2*(W-H);
      }else{
         F+=(2*W);
      }
      t++;
   }
}

void Canvas::drawTriangle(Vertice a, Vertice b, Vertice c){
   //access the colors
   Pixel color3 = _colors.back();
   _colors.pop_back();
   Pixel color2 = _colors.back();
   _colors.pop_back();
   Pixel color1 = _colors.back();
   _colors.pop_back();

   //sort three vertices to make them in counter-clock order
   //find the midpoint m between a and b
   float mx = (a.x+b.x)/2.0;
   float my = (a.y+b.y)/2.0;
   //calculate the vector am
   float amx = mx - a.x;
   float amy = my - a.y;
   //calculate the vector ac
   float acx = c.x - a.x;
   float acy = c.y - a.y;
   //swap b and c if the cross product of am and ac is less than 0
   if((amx*acy-amy*acx)<0){
      Vertice temp = c;
      c = b;
      b = temp;

      Pixel tempColor = color3;
      color3 = color2;
      color2 = tempColor;
   } 
   
   //calculate the bounding box
   int xmax = std::max(a.x,b.x);
   xmax = std::max(xmax,c.x);
   int xmin = std::min(a.x,b.x);
   xmin = std::min(xmin,c.x);
   int ymax = std::max(a.y,b.y);
   ymax = std::max(ymax,c.y);
   int ymin = std::min(a.y,b.y);
   ymin = std::min(ymin,c.y);


   int fa = (b.y-c.y)*a.x - (b.x-c.x)*a.y - c.x*b.y + c.y*b.x;
   int fb = (c.y-a.y)*b.x - (c.x-a.x)*b.y - a.x*c.y + c.x*a.y;
   int fc = (a.y-b.y)*c.x - (a.x-b.x)*c.y - b.x*a.y + a.x*b.y;
   int fa_1 = (b.y-c.y)*-1 - (b.x-c.x)*-1 - c.x*b.y + c.y*b.x;
   int fb_1 = (c.y-a.y)*-1 - (c.x-a.x)*-1 - a.x*c.y + c.x*a.y;
   int fc_1 = (a.y-b.y)*-1 - (a.x-b.x)*-1 - b.x*a.y + a.x*b.y;


   for(int y = ymin; y < ymax; y++){
      for(int x = xmin; x < xmax; x++){
         float aa = ((b.y-c.y)*x - (b.x-c.x)*y - c.x*b.y + c.y*b.x)/(float)fa;
         float bb = ((c.y-a.y)*x - (c.x-a.x)*y - a.x*c.y + c.x*a.y)/(float)fb;
         float cc = ((a.y-b.y)*x - (a.x-b.x)*y - b.x*a.y + a.x*b.y)/(float)fc;
         if(aa >= 0 && bb >= 0 && cc >= 0){
            //avoiding overlaps
            if((aa>0||fa_1*fa > 0)&&(bb>=0||fb_1*fb >= 0)&&(cc>=0||fc_1*fc >=0)){
               //calculate the color
               unsigned char r = aa*color1.r + bb*color2.r + cc*color3.r;
               unsigned char g = aa*color1.g + bb*color2.g + cc*color3.g;
               unsigned char b = aa*color1.b + bb*color2.b + cc*color3.b;
               Pixel color = {r,g,b};
               set(x,y,color);
            }
         }
      }
   }
}

