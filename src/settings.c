#include <sudoku3/settings.h>
#include <assert.h>
static settings_t _settings;

void settings_set(settings_t* s){
 assert(s);
 _settings = *s;
}

settings_t* settings_get(){
 return &_settings;
}
