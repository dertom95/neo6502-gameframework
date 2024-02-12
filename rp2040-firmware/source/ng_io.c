#include "ng_io.h"

#ifdef PICO_NEO6502
# include "core/backend/neo6502/neo6502.h"
#endif
// see backends

int16_t mouse_x=0;
int16_t mouse_y=0;
uint8_t mouse_btn_state=0;
int8_t  mouse_wheel=0;
uint8_t keyboard_last_pressed_keycode = 0;
char    keyboard_last_pressed_key = 0;

bool _keyboard_connected = false;
bool _mouse_connected = false;


void io_init(void)
{
  io_backend_init();
}

void io_update(void)
{
  io_backend_update();
}