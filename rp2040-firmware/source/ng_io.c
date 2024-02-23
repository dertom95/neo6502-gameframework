#include "ng_io.h"
#include "ng_mem.h"
#include "core/memory.h"

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


#define KBDMAP_FLAG_INUSE (1 << 0)

typedef struct keyboard_environment_t {
  uint8_t keyboardmapping_amount;
  keyboard_mapping_t* keyboardmappings;
} keyboard_environment_t;

keyboard_environment_t kenv={0};

void io_init(void)
{
  io_backend_init();
}

void io_update(void)
{
  io_backend_update();
}

/// @brief register keyboardmappings to the system
/// @param address 
/// @param amount 
void io_keyboardmapping_register(keyboard_mapping_t* address, uint8_t amount){
  assert(kenv.keyboardmappings==NULL && "KEYBOARDMAPPINGS ALREADY SET! UNREGISTER and REGISTER again if you need to change");
  kenv.keyboardmappings = address;
  kenv.keyboardmapping_amount = amount;
}

/// @brief Unregisters the current keyboard mapping!
void io_keyboardmapping_unregister(){
  ASSERT_STRICT(kenv.keyboardmappings!=NULL && "NO KEYBOARD MAPPINGS REGISTERED! NO NEED TO UNREGISTER!");
  kenv.keyboardmapping_amount = 0;
  kenv.keyboardmappings = NULL;
}