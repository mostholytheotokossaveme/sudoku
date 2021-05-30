#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "jpeglib.h"

unsigned char* jpg_load(const char* a, int* w, int*h, int* c, long *ts) {
  FILE* f, *fr;
  if ((f = fopen(a, "rb")) == NULL) {
	fprintf(stderr, "couldn't open %s\n", a);
	exit(errno);
  }

  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr err;
  jpeg_std_error(&err);
  cinfo.err = &err;
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, f);
  jpeg_read_header(&cinfo, 1);
  (void)jpeg_start_decompress(&cinfo);

  int rs;
  rs = cinfo.output_width * cinfo.output_components;
  printf("oc: %d\n", cinfo.output_components);
  *w = cinfo.output_width;
  *h = cinfo.output_height;
  *c = cinfo.output_components;
  j_common_ptr cptr;
  *(void**)&cptr = (void*)&cinfo;

  JSAMPARRAY buffer;
  JSAMPLE *image_buffer;
  JSAMPLE *ptr;
  const size_t SCL_CT = 10;
  buffer = (*cinfo.mem->alloc_sarray)	(cptr, JPOOL_IMAGE, rs, SCL_CT);
  long ibs = 3 + sizeof(JSAMPLE) * rs * cinfo.output_height;
  image_buffer = malloc(ibs);
  image_buffer[ibs-1] = 3;
  image_buffer[ibs-2] = 2;
  image_buffer[ibs-3] = 1;
  *ts = ibs - 3;
  printf("TS: %'.2f (kb) \tW: %d\tH:%d\n", (*ts) / 1024.d, cinfo.output_width, cinfo.output_height);
  if (ibs > 3)
	  image_buffer[ibs - 4] = 100;
  ptr = image_buffer;
  size_t lread;
  while(cinfo.output_scanline < cinfo.output_height) {
    if((lread = jpeg_read_scanlines(&cinfo, buffer, SCL_CT))) {
	int idx;
	for(idx=0;idx<lread;idx++) {
	  JSAMPLE* r = buffer[idx];
	  memcpy(ptr, r, rs);
	  ptr = ptr + rs;
	}
    }
  }

  assert(ibs < 4 || image_buffer[ibs-4] != 100);
  assert(image_buffer[ibs-1] == 3);
  assert(image_buffer[ibs-2] == 2);
  assert(image_buffer[ibs-3] == 1);

  fclose(f);
  assert(sizeof(JSAMPLE) == 1);
  return image_buffer;

}

