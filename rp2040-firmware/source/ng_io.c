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



keyboard_environment_t kenv={0};

static void update_keymapping(keyboard_mapping_t* keymap){
  keymap->key_down=0;
  keymap->key_pressed=0;
  keymap->key_released=0;

  for (int i=0;i<8;i++){
    uint8_t keycode = keymap->keycodes[i];
    if (keycode==0){
      continue;
    }
    // TODO: this can be more optimizied but I want it getting started! Let's start with overengineering later
    if (flags_isset(keymap->flags,KEYBMAP_FLAG_SCAN_KEY_PRESSED) && io_keyboard_is_pressed(keycode)){
      keymap->key_pressed |= (128>>i);
    }
    if (flags_isset(keymap->flags,KEYBMAP_FLAG_SCAN_KEY_DOWN) && io_keyboard_is_down(keycode)){
      keymap->key_down |= (128>>i);
    }
    if (flags_isset(keymap->flags,KEYBMAP_FLAG_SCAN_KEY_RELEASED) && io_keyboard_is_released(keycode)){
      keymap->key_released |= (128>>i);
    }
  }
}

static void update_all_keymappings(){
  keyboard_mapping_t* keymap = kenv.keyboardmappings;
  uint8_t count = kenv.keyboardmapping_amount;
  while(count--){
    update_keymapping(keymap);
    keymap++;
  }
}


void io_init(void)
{
  io_backend_init();
}

void io_before_tick(void)
{
  //TODO: implement some kind of mapping to only update on request/memoryread (but ensure that is not done a 2mhz)
  update_all_keymappings();
}

void io_after_tick(void)
{
  io_backend_after_tick();
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