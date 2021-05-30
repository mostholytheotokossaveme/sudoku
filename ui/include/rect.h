#ifndef RECT_H
#define RECT_H
typedef int coord_size_t;
typedef unsigned short ushort;
typedef struct {
  coord_size_t x;
  coord_size_t y;
  coord_size_t w;
  coord_size_t h;
} rect_t;

typedef struct {
  coord_size_t x;
  coord_size_t y;
} point_t;

rect_t rect_create			(coord_size_t x,coord_size_t y,coord_size_t w,coord_size_t h);
rect_t rect_square			(coord_size_t x,coord_size_t y,coord_size_t sz);
rect_t* rect_split			(rect_t* sq, coord_size_t w, coord_size_t h, ushort *sct);
short  rect_point		        (rect_t* r, point_t* p);
#endif
