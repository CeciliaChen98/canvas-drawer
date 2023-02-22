#include <iostream>
#include "canvas.h"
#include <time.h>
using namespace std;
using namespace agl;

int main(int argc, char** argv)
{
   Canvas drawer(640, 380);
   srand (time(NULL));

   drawer.background(255,255,255);
   drawer.begin(CIRCLES);
   for(int i =0; i<40; i++){
      int randr = rand()%60+150;
      int randb = rand()%30+180;
      int randg = rand()%30+180;
      int randx = rand()%640;
      int randy = rand()%380;
      int randradius = rand()%70;
      drawer.color(randr,randg,randb);
      drawer.vertex(randx,randy);
      drawer.radius(randradius);
      drawer.color(randr+10,randg,randb);
      drawer.vertex(randx,randy);
      drawer.radius(randradius+10);
      drawer.color(randr+20,randg,randb);
      drawer.vertex(randx,randy);
      drawer.radius(randradius+20);
   }   
   drawer.end();
   drawer.save("circles.png");

   drawer.background(255,255,255);
   drawer.begin(RECTANGLES);
   for(int i =0; i<60; i++){
      int randr = rand()%60+150;
      int randb = rand()%100+100;
      int randg = rand()%180;
      int randx = rand()%640;
      int randy = rand()%380;
      drawer.color(randr,randg,randb);
      drawer.vertex(randx,randy);
   }   
   drawer.end();
   drawer.save("rectangles.png");

   drawer.background(255,255,255);
   drawer.begin(STARS);
   for(int i =0; i<40; i++){
      int randr = rand()%30+180;
      int randb = rand()%40+180;
      int randg = rand()%100+100;
      int randx = rand()%640;
      int randy = rand()%380;
      int randradius = rand()%10+20;
      drawer.color(200,randg,randb);
      drawer.vertex(randx,randy);
      drawer.radius(randradius);
      drawer.color(200,randg,randb-10);
      drawer.vertex(randx,randy);
      drawer.radius(randradius+15);
      drawer.color(200,randg-10,randb-20);
      drawer.vertex(randx,randy);
      drawer.radius(randradius+30);
   }   
   drawer.end();
   drawer.save("stars.png");

   drawer.background(0,0,0);
   drawer.begin(LINES);
   for(int i =0; i<900; i++){
      int randr = rand()%60+150;
      int randb = rand()%100+100;
      int randg = rand()%100+100;
      int randx = rand()%640;
      int randy = rand()%380;
      drawer.color(randr,randg,randb);
      drawer.vertex(randx,randy);
   }  
   drawer.end();
   drawer.save("lines.png");

   drawer.background(255,255,255);
   drawer.begin(TRIANGLES);
   for(int i =0; i<900; i++){
      int randr = rand()%60+150;
      int randb = rand()%100+100;
      int randg = rand()%100+100;
      int randx = rand()%640;
      int randy = rand()%380;
      drawer.color(randr,randg,randb);
      drawer.vertex(randx,randy);
   }  
   drawer.end();
   drawer.save("triangles.png");

   drawer.background(0,0,0);
   drawer.begin(POINTS);
   for(int i =0; i<1500; i++){
      int randr = rand()%60+150;
      int randb = rand()%100+100;
      int randg = rand()%100+100;
      int randx = rand()%640;
      int randy = rand()%380;
      drawer.color(randr,randg,randb);
      drawer.vertex(randx,randy);
   }  
   drawer.end();
   drawer.save("points.png");



   drawer.background(255,255,255);
   drawer.begin(FLOWERS);
   for(int i =0; i<50; i++){
      int randp = rand()%6+15;
      int randr = rand()%20+120;
      int randb = rand()%20+150;
      int randx = rand()%640;
      int randy = rand()%380;
      int randradius = rand()%80+30;
      drawer.setPetal(randp);
      drawer.color(randr,150,randb);
      drawer.vertex(randx,randy);
      drawer.radius(randradius);
   }
   drawer.end();
   drawer.save("flowers.png");

   Image circles;
   circles.load("../images/circles.png");
   Image flowers;
   flowers.load("../images/flowers.png");
   //flowers=flowers.invert();
   Image work = circles.lightest(flowers);
   work.save("my_art_work1.png");

   Image bacteria;
   bacteria.load("../images/bacteria.png");
   Image bacteria2;
   bacteria2.load("../images/bacteria2.png");
   work = bacteria.swirl().swirl();
   work = work.multiply(bacteria2);
   Image background = bacteria.subimage(0,0,320,380);
   background = background.invert();
   work.replace(background,0,0);
   work.save("my_art_work2.png");

}

