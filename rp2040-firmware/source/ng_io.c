#include "ng_io.h"
#include "ng_mem.h"
#include "ng_utils.h"
#include "core/memory.h"
#include <stdio.h>

#ifdef PICO_NEO6502
#include "core/backend/neo6502/neo6502.h"
#endif
// see backends

bool _keyboard_connected = false;
bool _mouse_connected = false;
// in locked state the currentstate will not be cleared by clear_state
bool input_locked = false;

// runtime data to work on
mouse_report_t mouse_report;

gamepad_state_t gamepad_down[GAMEPAD_MAX_DEVICES]={0};
gamepad_state_t gamepad_pressed[GAMEPAD_MAX_DEVICES] = {0};
gamepad_state_t gamepad_released[GAMEPAD_MAX_DEVICES] = {0};

#define KBDMAP_FLAG_INUSE (1 << 0)

keyboard_environment_t kenv = {0};

// TODO :debuggin remove
// uint32_t calls_clearstate = 0;
// uint32_t calls_update = 0;
// ---

bool was_set = false;

static void update_mouse(){
    *mm_mouse_x = mouse_report.mouse_x;
    *mm_mouse_y = mouse_report.mouse_y;
    *mm_mouse_btn_state = mouse_report.mouse_btn_state;
    *mm_mouse_btn_state_pressed = mouse_report.mouse_btn_state_pressed;
    *mm_mouse_btn_state_released = mouse_report.mouse_btn_state_released;
    *mm_mouse_wheel = mouse_report.mouse_wheel;
}

static void update_keymapping(keyboard_mapping_t *keymap)
{
    keymap->key_pressed = 0;
    keymap->key_released = 0;
    keymap->key_down = 0;

    //calls_update++;
    // keymap->key_down=0;
    for (int i = 0; i < 8; i++)
    {
        uint8_t keycode = keymap->keycodes[i];

        if (keycode == 0)
        {
            continue;
        }
        uint8_t bit = (1 << i);
        // TODO: this can be more optimizied but I want it getting started! Let's start with overengineering later
        if (flags_isset(keymap->flags, KEYBMAP_FLAG_SCAN_KEY_PRESSED) && io_keyboard_is_pressed(keycode))
        {
            keymap->key_pressed |= bit;
            was_set = true;
        }
        if (flags_isset(keymap->flags, KEYBMAP_FLAG_SCAN_KEY_DOWN) && io_keyboard_is_down(keycode))
        {
            keymap->key_down |= bit;
        }
        if (flags_isset(keymap->flags, KEYBMAP_FLAG_SCAN_KEY_RELEASED) && io_keyboard_is_released(keycode))
        {
            keymap->key_released |= bit;
        }
    }
}

static void update_all_keymappings()
{
    keyboard_mapping_t *keymap = kenv.keyboardmappings;
    uint8_t count = kenv.keyboardmapping_amount;
    while (count--)
    {
        update_keymapping(keymap);
        keymap++;
    }
}

void io_lock_input(bool lock_it) {
    input_locked=lock_it;
}

void io_init(void)
{
    for (int i = 0; i < GAMEPAD_MAX_DEVICES; i++)
    {
        gamepad_down[i]=(gamepad_state_t){0};
        gamepad_pressed[i]=(gamepad_state_t){0};
        gamepad_released[i]=(gamepad_state_t){0};
    }
    io_backend_init();
}



bool io_gamepad_is_active(uint8_t gamepad_id)
{
    assert(gamepad_id < GAMEPAD_MAX_DEVICES);
    bool result = bit_is_set_some(*mm_gamepad_info, (1 << gamepad_id));
    if (gamepad_id == 0 && result == false)
    {
        return false;
    }
    return result;
}

void io_after_tick(void)
{
    io_backend_after_tick();

}

static void update_gamepad(void){
    for (int i = 0; i < GAMEPAD_MAX_DEVICES; i++)
    {
        if (io_gamepad_is_active(i))
        {
            mm_gamepad_pressed[i] = gamepad_pressed[i];
            mm_gamepad_released[i] = gamepad_released[i];
            mm_gamepad_down[i] = gamepad_down[i];
        }
    } 
}

// clear pressed/released-states
static void io_input_clear_state_gamepad(void)
{
    for (int i = 0; i < GAMEPAD_MAX_DEVICES; i++)
    {
        if (io_gamepad_is_active(i))
        {
            gamepad_pressed[i] = (gamepad_state_t){0};
            gamepad_released[i] = (gamepad_state_t){0};
        }
    }
}

static void io_input_clear_state_keyboard(void){
    // clear keyboard
    keyboard_mapping_t *keymap = kenv.keyboardmappings;
    uint8_t count = kenv.keyboardmapping_amount;
    while (count--)
    {
        if (keymap->key_pressed > 0)
        {
            int a = 0;
        }
        keymap->key_pressed = 0;
        keymap->key_released = 0;
        keymap->key_down = 0;
        keymap++;
    }
    
    *mm_keyboard_last_pressed_char = 0;
    *mm_keyboard_last_pressed_keycode = 0;
}


static void io_input_clear_state_mouse(void){
    // clear mouse
    mouse_report.mouse_btn_state_pressed = 0;
    mouse_report.mouse_btn_state_released = 0;
    mouse_report.mouse_wheel = 0;
}

void io_before_tick(void)
{
    // clear keyboard state
    io_input_clear_state_keyboard();
    update_all_keymappings();

    update_mouse();
    io_input_clear_state_mouse();
    
    update_gamepad();
    io_input_clear_state_gamepad();

    
    io_backend_clear_state();
}

/// @brief register keyboardmappings to the system
/// @param address
/// @param amount
void io_keyboardmapping_register(keyboard_mapping_t *address, uint8_t amount)
{
    assert(kenv.keyboardmappings == NULL && "KEYBOARDMAPPINGS ALREADY SET! UNREGISTER and REGISTER again if you need to change");
    kenv.keyboardmappings = address;
    kenv.keyboardmapping_amount = amount;
}

/// @brief Unregisters the current keyboard mapping!
void io_keyboardmapping_unregister()
{
    ASSERT_STRICT(kenv.keyboardmappings != NULL && "NO KEYBOARD MAPPINGS REGISTERED! NO NEED TO UNREGISTER!");
    kenv.keyboardmapping_amount = 0;
    kenv.keyboardmappings = NULL;
}