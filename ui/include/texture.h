#ifndef TEXTURE_H
#define TEXTURE_H

struct texture {
   unsigned char* tx;
   int w;
   int h;
   long ts;
   int rs;
   int c;
   int dpt;
   void *src;
};

typedef struct texture texture_t;

texture_t* texture_from_file(char *fn);
#endif
