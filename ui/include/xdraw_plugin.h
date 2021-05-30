#ifndef XDRAW_PLUGIN_H
#define XDRAW_PLUGIN_H

typedef void* xdraw_plugin_ptr;

typedef void (*xdraw_plugin_init)(xdraw_plugin_ptr);

struct xdraw_plugin_services {
   void (*service_wh)(int*,int*);
   const void* (*service_pparam)(const char*);
} xdraw_plugin_services;

struct xdraw_plugin_ctrls {
 void (*kpress)(unsigned char,int,int);
 void (*krelease)(unsigned char,int,int);
 void (*mpress)(int,int,int);
 void (*mrelease)(int,int,int);
 void (*mmove)(int,int,int);
 void (*mbmove)(int,int,int);
} xdraw_plugin_ctrls;

struct xdraw_plugin {
 void (*display)();
 void (*init)();
 void (*cleanup)(void);
 struct xdraw_plugin_ctrls *ctrls;
 const struct xdraw_plugin_services *plugin_services;
} xdraw_plugin;

typedef void (*fp_ctrl)(int,int,int);
typedef fp_ctrl (*fp_pluginctrl)(struct xdraw_plugin*);

#endif
