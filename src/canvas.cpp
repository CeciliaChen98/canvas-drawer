#include "canvas.h"
#include <cassert>
#include <cmath>
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
         Vertex v2 = _positions.back();
         _positions.pop_back();
         Vertex v1 = _positions.back();
         _positions.pop_back();
         drawLine(v1,v2);
      }
   }else if(_type == TRIANGLES){
      while(!_positions.empty()){
         Vertex v3 = _positions.back();
         _positions.pop_back();
         Vertex v2 = _positions.back();
         _positions.pop_back();
         Vertex v1 = _positions.back();
         _positions.pop_back();
         drawTriangle(v1,v2,v3);
      }
   }else if(_type == CIRCLES){
      while(!_positions.empty()){
         Vertex v = _positions.back();
         _positions.pop_back();
         int r = _radius.back();
         _radius.pop_back();
         drawCircle(v,r);
      }
   }else if(_type == RECTANGLES){
      while(!_positions.empty()){
         Vertex v2 = _positions.back();
         _positions.pop_back();
         Vertex v1 = _positions.back();
         _positions.pop_back();
         drawRectangle(v1,v2);         
      }
   }else if(_type == STARS){
      while(!_positions.empty()){
         Vertex v = _positions.back();
         _positions.pop_back();
         int r = _radius.back();
         _radius.pop_back();
         drawStar(v,r);
      }
   }else if(_type == FLOWERS){
      while(!_positions.empty()){
         Vertex v = _positions.back();
         _positions.pop_back();
         int r = _radius.back();
         _radius.pop_back();
         int p = _petals.back();
         _petals.pop_back();
         drawFlower(v,p,r);
      }
   }else if(_type == POINTS){
      while(!_positions.empty()){
         Vertex v = _positions.back();
         _positions.pop_back();
         Pixel color = _colors.back();
         _colors.pop_back();
         set(v.x,v.y,color);
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
   if( x>=0&& x<_canvas.width() && y>=0&& y <_canvas.height()){
      _canvas.set(y,x,pixel);
   }
}

void Canvas::vertex(int x, int y)
{
   _positions.push_back({x,y});
   if(_positions.size()!=_colors.size()){
      _colors.push_back(_color);
   }  
   if(_type==FLOWERS) {
      if(_positions.size()!=_petals.size()){
         _petals.push_back(_petal);
      }
   }
}

void Canvas::radius(int r)
{
   _radius.push_back(r);
}

void Canvas::color(unsigned char r, unsigned char g, unsigned char b)
{
   _color = {r,g,b};
   _colors.push_back(_color);
}

void Canvas::setPetal(int p)
{
   _petal = p;
   _petals.push_back(p);
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
void Canvas::drawLine(Vertex v1, Vertex v2){
   //access the colors 
   Pixel color2 = _colors.back();
   _colors.pop_back();
   Pixel color1 = _colors.back();
   _colors.pop_back();

   int W = v2.x-v1.x;
   int H = v2.y-v1.y;
   if(std::abs(H)<std::abs(W)){
      if(v1.x>v2.x){
         Vertex temp = v1;
         v1 = v2;
         v2 = temp;

         Pixel tempColor = color1;
         color1 = color2;
         color2 = tempColor;
      }
      drawLineLow(v1,v2,color1,color2);
   }else{
      if(v1.y>v2.y){
         Vertex temp = v1;
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
void Canvas::drawLineLow(Vertex v1, Vertex v2, Pixel color1, Pixel color2){
   
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
void Canvas::drawLineHigh(Vertex v1, Vertex v2, Pixel color1, Pixel color2){

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

void Canvas::drawTriangle(Vertex a, Vertex b, Vertex c){
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
      Vertex temp = c;
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
   float fa_1 = (b.y-c.y)*-1 - (b.x-c.x)*-1.5 - c.x*b.y + c.y*b.x;
   float fb_1 = (c.y-a.y)*-1 - (c.x-a.x)*-1.5 - a.x*c.y + c.x*a.y;
   float fc_1 = (a.y-b.y)*-1 - (a.x-b.x)*-1.5 - b.x*a.y + a.x*b.y;


   for(int y = ymin; y < ymax; y++){
      for(int x = xmin; x < xmax; x++){
         float aa = ((b.y-c.y)*x - (b.x-c.x)*y - c.x*b.y + c.y*b.x)/(float)fa;
         float bb = ((c.y-a.y)*x - (c.x-a.x)*y - a.x*c.y + c.x*a.y)/(float)fb;
         float cc = ((a.y-b.y)*x - (a.x-b.x)*y - b.x*a.y + a.x*b.y)/(float)fc;
         if(aa >= 0 && bb >= 0 && cc >= 0){
            //avoiding overlaps
            if((aa>0||fa_1*fa > 0)&&(bb>0||fb_1*fb > 0)&&(cc>0||fc_1*fc >0)){
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

void Canvas::drawCircle(Vertex v, int r){
   int xmin = v.x-r;
   int xmax = v.x+r;
   int ymin = v.y-r;
   int ymax = v.y+r;
   Pixel color = _colors.back();
   _colors.pop_back();
   for(int x = xmin; x <= xmax; x++){
      for(int y = ymin; y<= ymax; y++){
         int distance = pow(x-v.x,2)+pow(y-v.y,2);
         if(distance < pow(r,2)){
            set(x,y,color);
         }
      }
   }
}

void Canvas::drawRectangle(Vertex v1, Vertex v2){
   //access the colors 
   Pixel color2 = _colors.back();
   _colors.pop_back();
   Pixel color1 = _colors.back();
   _colors.pop_back();

   Pixel color;
   if(v1.x>v2.x){
      Vertex temp = v1;
      v1 = v2;
      v2 = temp;
      Pixel tempC = color1;
      color1 = color2;
      color2 = tempC;
   }
   for(int i = v1.x;i <=v2.x;i++){
      float p = (i-v1.x)/(float)(v2.x-v1.x);
      unsigned char r = color1.r*(1-p)+color2.r*p;
      unsigned char g = color1.g*(1-p)+color2.g*p;   
      unsigned char b = color1.b*(1-p)+color2.b*p;  
      color = {r,g,b};
      if(v1.y <= v2.y){
         for(int j = v1.y;j<=v2.y;j++){ 
            set(i,j,color);
         }
      }else{
         for(int j = v1.y;j>=v2.y;j--){ 
            set(i,j,color);
         }
      }
   }
}

void Canvas::drawPureTriangle(Vertex a, Vertex b, Vertex c, Pixel color){
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
      Vertex temp = c;
      c = b;
      b = temp;
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
   float fa_1 = (b.y-c.y)*-1 - (b.x-c.x)*-1.5 - c.x*b.y + c.y*b.x;
   float fb_1 = (c.y-a.y)*-1 - (c.x-a.x)*-1.5 - a.x*c.y + c.x*a.y;
   float fc_1 = (a.y-b.y)*-1 - (a.x-b.x)*-1.5 - b.x*a.y + a.x*b.y;


   for(int y = ymin; y <= ymax; y++){
      for(int x = xmin; x <= xmax; x++){
         float aa = ((b.y-c.y)*x - (b.x-c.x)*y - c.x*b.y + c.y*b.x)/(float)fa;
         float bb = ((c.y-a.y)*x - (c.x-a.x)*y - a.x*c.y + c.x*a.y)/(float)fb;
         float cc = ((a.y-b.y)*x - (a.x-b.x)*y - b.x*a.y + a.x*b.y)/(float)fc;
         if(aa >= 0 && bb >= 0 && cc >= 0){
            //avoiding overlaps
            if((aa>0||fa_1*fa > 0)&&(bb>0||fb_1*fb > 0)&&(cc>0||fc_1*fc >0)){
               set(x,y,color);
            }
         }
      }
   }
}

void Canvas::drawStar(Vertex v, int r){
   //get the color
   Pixel color = _colors.back();
   _colors.pop_back();
   
   //specify the points
   float pie = 3.1415926/180;
   Vertex A = {v.x, v.y+r};
   Vertex B = {(int)(r*std::cos(18*pie))+v.x, (int)(r*std::sin(18*pie))+v.y};
   Vertex C = {(int)(r*std::cos(54*pie))+v.x, (int)(-r*std::sin(54*pie))+v.y};
   Vertex D = {(int)(-r*std::cos(54*pie))+v.x, (int)(-r*std::sin(54*pie))+v.y};
   Vertex E = {(int)(-r*std::cos(18*pie))+v.x, (int)(r*std::sin(18*pie))+v.y};
   float inner_r = (r*std::sin(18*pie))/std::cos(36*pie);
   Vertex F = {(int)(inner_r*std::cos(54*pie))+v.x, (int)(inner_r*std::sin(54*pie))+v.y};
   Vertex G = {(int)(inner_r*std::cos(18*pie))+v.x, (int)(-inner_r*std::sin(18*pie))+v.y};
   Vertex H = {v.x, (int)(v.y-inner_r)};
   Vertex K = {(int)(-inner_r*std::cos(18*pie))+v.x, (int)(-inner_r*std::sin(18*pie))+v.y};
   Vertex J = {(int)(-inner_r*std::cos(54*pie))+v.x, (int)(inner_r*std::sin(54*pie))+v.y};

   drawPureTriangle(A,v,F,color);
   drawPureTriangle(F,v,B,color);
   drawPureTriangle(B,v,G,color);
   drawPureTriangle(G,v,C,color);
   drawPureTriangle(C,v,H,color);
   drawPureTriangle(H,v,D,color);
   drawPureTriangle(D,v,K,color);
   drawPureTriangle(K,v,E,color);
   drawPureTriangle(E,v,J,color);
   drawPureTriangle(J,v,A,color);

}

void Canvas::drawFlower(Vertex v, int p, int r){
   Pixel color = _colors.back();
   _colors.pop_back();
   float pie = 3.1415926;
   for(float theta = 0; theta<2*pie; theta+=0.001){
      int x = std::cos(p*theta)*std::cos(theta)*r+v.x;
      int y = std::cos(p*theta)*std::sin(theta)*r+v.y;
      set(x,y,color);
   }
}



