// Copyright 2021, Aline Normoyle, alinen
/*-----------------------------------------------
 * Author: Yue Chen
 * Date: 2.8.2023
 * Description: This is the file where the functions from image.h 
 * are being implemented. It can load images, save images, and do 
 * modifications to images
 ----------------------------------------------*/

#include "image.h"

#include <cassert>
#include <cmath>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace agl {


Image::Image():_width(0),_height(0){
  _data = new Pixel[0];
  //std::cout<<"default width: "<<_width<<", defult height: "<<_height<<std::endl;
}

Image::Image(int width, int height):_width(width),_height(height){
  _data = new Pixel[width*height];
  _channel = 3;
  //std::cout<<"width: "<<_width<<", height: "<<_height<<std::endl;
}

Image::Image(const Image& orig):_width(orig._width),_height(orig._height),_channel(orig._channel){
  _data = new Pixel[_width*_height];
  memcpy(_data,orig._data,_width*_height*3);
  //std::cout<<"copy width: "<<_width<<", copy height: "<<_height<<std::endl;
}

Image& Image::operator=(const Image& orig) {
  if (&orig == this) {
    return *this;
  }
  _width=orig.width();
  _height=orig.height();
  _channel=orig._channel;
  memcpy(_data,orig._data,_width*_height*3);
  //std::cout<<"assignment op "<<_width<<","<<_height<<std::endl;
  return *this;
}

Image::~Image() {
  free();
  //std::cout<<"destructor: "<<_width<<","<<_height<<std::endl;
}

int Image::width() const { 
  return _width;
}

int Image::height() const {
  return _height;
}

unsigned char* Image::data() const {
   return (unsigned char*)_data;
}

void Image::set(int width, int height, unsigned char* data) {
  free();
  _height = height;
  _width = width;
  _data = new Pixel[_width*_height];
  memcpy(_data, data, _height*_width*3);
} 

bool Image::load(const std::string& filename, bool flip) {
  unsigned char* data = stbi_load(filename.c_str(), &_width , &_height, &_channel, 3);
  if(!data){
    return false;
  }
  set(_width,_height,data);
  stbi_image_free(data);
  return true;
}

bool Image::save(const std::string& filename, bool flip) const {
  unsigned char* data = (unsigned char*) _data;
  if(!data){
   return false;
  }
  stbi_write_png(filename.c_str(),_width,_height,_channel,data,_width*_channel);
  return true;
}

Pixel Image::get(int row, int col) const {
  return _data[row*width()+col];
}

void Image::set(int row, int col, const Pixel& color) {
  _data[row*width()+col] = color;
}


Pixel Image::get(int i) const
{
  return _data[i];
}

void Image::set(int i, const Pixel& c)
{
  _data[i] = c;
}

Image Image::resize(int w, int h) const {
  Image result(w, h);
  for( int i = 0; i < h; i++ ){
    for( int j = 0; j < w; j++ ){
    int r = (_height-1)*i/(h-1);
    int c = (_width-1)*j/(w-1); 
    result.set(i,j,get(r,c));
    }
  }
  return result;
}

Image Image::flipHorizontal() const {
  Image result(_height, _width);
  for(int i=0; i<_height; i++){
    for(int j=0; j<_width; j++){
      result.set(i,_width-1-j,get(i,j));
    }
  }
  return result;
}

Image Image::flipVertical() const {
  Image result(_height, _width);
  for(int i=0; i< _height; i++){
    for(int j=0; j< _width; j++){
      result.set(_height-1-i,j,get(i,j));
    }
  }
  return result;
}

Image Image::rotate90() const {
  Image result(_height, _width);
  return result;
}

Image Image::subimage(int startx, int starty, int w, int h) const {
  Image sub(w, h);
  for(int i=0; i<h; i++){
    for(int j=0; j<w; j++){
      sub.set(i,j,get(startx+i,starty+j));
    }
  }
  return sub;
}

void Image::replace(const Image& image, int startx, int starty) {
  for(int i=0; i<image.height(); i++){
    for(int j=0; j<image.width(); j++){
      set(startx+i,starty+j,image.get(i,j));
    }
  }
}

Image Image::swirl() const {
  Image result(_width, _height);
  Pixel temp;
  for(int i=0; i< _height; i++){
    for(int j=0; j< _width; j++){
      temp = {get(i,j).g,get(i,j).b,get(i,j).r};
      result.set(i,j,temp);
    }
  }  
  return result;
}

Image Image::add(const Image& other) const {
  Image result(_width, _height);
  for(int i=0; i< _height; i++){
    for(int j=0; j< _width; j++){
      int r = (int)get(i,j).r+(int)other.get(i,j).r;
      unsigned char red = std::min(r,255);
      int g = (int)get(i,j).g+other.get(i,j).g;
      unsigned char green = std::min(g,255);
      int b = (int)get(i,j).b+other.get(i,j).b;
      unsigned char blue = std::min(b,255);
      result.set(i,j,{red,green,blue});
    }
  }
  return result;
}

Image Image::subtract(const Image& other) const {
  Image result(_width, _height);
  for(int i=0; i< _height; i++){
    for(int j=0; j< _width; j++){
      int r = (int)get(i,j).r-(int)other.get(i,j).r;
      unsigned char red = std::max(r,0);
      int g = (int)get(i,j).g-other.get(i,j).g;
      unsigned char green = std::max(g,0);
      int b = (int)get(i,j).b-other.get(i,j).b;
      unsigned char blue = std::max(b,0);
      result.set(i,j,{red,green,blue});
    }
  }
  return result;
}

Image Image::multiply(const Image& other) const {
  Image result(_width, _height);
  for(int i=0; i< _height; i++){
    for(int j=0; j< _width; j++){
      unsigned char red = (int)get(i,j).r*(int)other.get(i,j).r/255;
      unsigned char green = (int)get(i,j).g*(int)other.get(i,j).g/255;
      unsigned char blue = (int)get(i,j).b*(int)other.get(i,j).b/255;
      result.set(i,j,{red,green,blue});
    }
  }
  return result;
}

Image Image::difference(const Image& other) const {
  Image result(_width, _height);
  for(int i=0; i< _height; i++){
    for(int j=0; j< _width; j++){
      unsigned char red = std::abs((int)get(i,j).r-(int)other.get(i,j).r);
      unsigned char green = std::abs((int)get(i,j).g-(int)other.get(i,j).g);
      unsigned char blue = std::abs((int)get(i,j).b-(int)other.get(i,j).b);
      result.set(i,j,{red,green,blue});
    }
  }
  return result;
}

Image Image::lightest(const Image& other) const {
  Image result(_width, _height);
  for(int i=0; i< _height; i++){
    for(int j=0; j< _width; j++){
      unsigned char red = std::max((int)get(i,j).r,(int)other.get(i,j).r);
      unsigned char green = std::max((int)get(i,j).g,(int)other.get(i,j).g);
      unsigned char blue = std::max((int)get(i,j).b,(int)other.get(i,j).b);
      result.set(i,j,{red,green,blue});  
    }
  }
  return result;
}

Image Image::darkest(const Image& other) const {
  Image result(_width, _height);
  for(int i=0; i< _height; i++){
    for(int j=0; j< _width; j++){
      unsigned char red = std::min((int)get(i,j).r,(int)other.get(i,j).r);
      unsigned char green = std::min((int)get(i,j).g,(int)other.get(i,j).g);
      unsigned char blue = std::min((int)get(i,j).b,(int)other.get(i,j).b);
      result.set(i,j,{red,green,blue});           
    }
  }
  return result;
}

Image Image::gammaCorrect(float gamma) const {
   gamma = 1/gamma;
   Image result(_width, _height);
   for(int i=0; i< _height; i++){
      for(int j=0; j< _width; j++){
        unsigned char red = get(i,j).r ;
        unsigned char green = get(i,j).g;
        unsigned char blue = get(i,j).b;
        red = std::pow((float)red/255,gamma) * 255;
        green = std::pow((float)green/255,gamma) * 255;
        blue = std::pow((float)blue/255,gamma) * 255;
        Pixel myPixel ={red,green,blue};
        result.set(i,j,myPixel);
      }
   }
   return result;
}

Image Image::alphaBlend(const Image& other, float alpha) const {
   Image result(_width, _height);
   for(int i=0; i< _height; i++){
      for(int j=0; j< _width; j++){
        unsigned char red = (1-alpha)*(get(i,j).r)+alpha*(other.get(i,j).r) ;
        unsigned char green = (1-alpha)*(get(i,j).g)+alpha*(other.get(i,j).g) ;
        unsigned char blue = (1-alpha)*(get(i,j).b)+alpha*(other.get(i,j).b) ; 
        Pixel myPixel ={red,green,blue};
        result.set(i,j,myPixel);
      }
   }
   return result;
}

Image Image::invert() const {
   Image image(_width, _height);
   for(int i=0; i< _height; i++){
      for(int j=0; j< _width; j++){
        unsigned char red = 255-(int)get(i,j).r;
        unsigned char green = 255-(int)get(i,j).g;
        unsigned char blue = 255-(int)get(i,j).b;
        Pixel myPixel ={red,green,blue};
        image.set(i,j,myPixel);
      }
   }   
   return image;
}

Image Image::grayscale() const {
  Image result(_width, _height);
  for(int i=0; i< _height; i++){
    for(int j=0; j< _width; j++){
      unsigned char red = get(i,j).r ;
      unsigned char green = get(i,j).g;
      unsigned char blue = get(i,j).b;
      unsigned char average = (red+green+blue)/3;
      Pixel myPixel ={average,average,average};
      result.set(i,j,myPixel);
    }
  }
  return result;
}

Image Image::colorJitter(int size) const {
  Image image(0, 0);
  
  return image;
}

Image Image::bitmap(int size) const {
   Image image(0, 0);
   
   return image;
}

void Image::fill(const Pixel& c) {
  }

void Image::free(){
  delete[] _data;
}

}  // namespace agl

