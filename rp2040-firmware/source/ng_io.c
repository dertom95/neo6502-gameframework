#include "ng_io.h"
#include "ng_mem.h"
#include "ng_utils.h"
#include "core/memory.h"

#ifdef PICO_NEO6502
# include "core/backend/neo6502/neo6502.h"
#endif
// see backends

bool _keyboard_connected = false;
bool _mouse_connected = false;

#define KBDMAP_FLAG_INUSE (1 << 0)



keyboard_environment_t kenv={0};

bool was_set = false;

static void update_keymapping(keyboard_mapping_t* keymap){
  //keymap->key_down=0;
  for (int i=0;i<8;i++){
    uint8_t keycode = keymap->keycodes[i];
    if (keycode==0){
      continue;
    }
    uint8_t bit = (128>>i);
    // TODO: this can be more optimizied but I want it getting started! Let's start with overengineering later
    if (flags_isset(keymap->flags,KEYBMAP_FLAG_SCAN_KEY_PRESSED) && io_keyboard_is_pressed(keycode)){
      keymap->key_pressed |= bit;
      was_set=true;
    }
    if (flags_isset(keymap->flags,KEYBMAP_FLAG_SCAN_KEY_DOWN) && io_keyboard_is_down(keycode)){
      keymap->key_down |= bit;
    }
    if (flags_isset(keymap->flags,KEYBMAP_FLAG_SCAN_KEY_RELEASED) && io_keyboard_is_released(keycode)){
      keymap->key_released |= bit;
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
    for (int i=0;i<GAMEPAD_MAX_DEVICES;i++){
        gamepad_state_t* current_state = &mm_gamepad_down[i];
    }
    io_backend_init();
}

void io_before_tick(void)
{
    //TODO: implement some kind of mapping to only update on request/memoryread (but ensure that is not done a 2mhz)
    update_all_keymappings();
}



bool io_gamepad_is_active(uint8_t gamepad_id) {
    assert(gamepad_id < GAMEPAD_MAX_DEVICES);
    bool result = bit_is_set_some(*mm_gamepad_info,(1 << gamepad_id));
    if (gamepad_id == 0 && result==false){
        return false;
    }
    return result;
}

void io_after_tick(void)
{
    io_backend_after_tick();

}

// clear pressed/released-states
void io_input_clear_states(void) {
    // clear gamepad
    for (int i=0;i<GAMEPAD_MAX_DEVICES;i++){
        if (io_gamepad_is_active(i)){
            gamepad_state_t* current_state = &mm_gamepad_down[i];
            mm_gamepad_pressed[i] = (gamepad_state_t){0};
            mm_gamepad_released[i] = (gamepad_state_t){0};
        }
    }

    // clear keyboard
    keyboard_mapping_t* keymap = kenv.keyboardmappings;
    uint8_t count = kenv.keyboardmapping_amount;
    while(count--){
        if (keymap->key_pressed > 0){
            int a=0;
        }
        keymap->key_pressed = 0;
        keymap->key_released = 0;
        keymap++;
    }    

    // clear mouse
    *mm_mouse_btn_state_pressed = 0;

    io_backend_clear_state();
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