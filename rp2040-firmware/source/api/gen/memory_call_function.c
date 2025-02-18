

#include "ng_api.h"
#include "../../core/memory.h"

uint8_t* call_buffer_return = &mem[MEMORY_MAP_CALLRETURN_BUFFER_BEGIN]; 

uint8_t call_function()
{
    call_header_t* header = (call_header_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];

    if (header->func_type==4){
        switch (header->func_id) {

            case 1: {
                call_asset_get_tilesheet_t* call = (call_asset_get_tilesheet_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  asset_get_tilesheet( (gfx_tilesheet_data_t*)(&mem[ call->ts_data ]) ,    call->asset_id    );

                return FUNCTION_RETURN_OK;
            }

            case 2: {
                call_asset_get_tilemap_t* call = (call_asset_get_tilemap_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  asset_get_tilemap( (gfx_tilemap_t*)(&mem[ call->tilemap ]) ,    call->asset_id    );

                return FUNCTION_RETURN_OK;
            }

        }
    }

    if (header->func_type==5){
        switch (header->func_id) {

            case 5: {
                call_audio_wav_load_t* call = (call_audio_wav_load_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                uint8_t call_result =  audio_wav_load(  call->asset_id    );

                *call_buffer_return=call_result;

                return FUNCTION_RETURN_OK;
            }

            case 1: {
                call_audio_wav_play_t* call = (call_audio_wav_play_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  audio_wav_play(  call->sound_id  ,    call->loop    );

                return FUNCTION_RETURN_OK;
            }

            case 4: {
                call_audio_wav_stop_t* call = (call_audio_wav_stop_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  audio_wav_stop(  call->sound_id    );

                return FUNCTION_RETURN_OK;
            }

            case 2: {
                call_audio_mod_play_t* call = (call_audio_mod_play_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  audio_mod_play(  call->asset_id    );

                return FUNCTION_RETURN_OK;
            }

            case 7: {
                call_audio_mod_pause_t* call = (call_audio_mod_pause_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  audio_mod_pause();

                return FUNCTION_RETURN_OK;
            }

            case 8: {
                call_audio_mod_resume_t* call = (call_audio_mod_resume_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  audio_mod_resume();

                return FUNCTION_RETURN_OK;
            }

            case 3: {
                call_audio_mod_stop_t* call = (call_audio_mod_stop_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  audio_mod_stop();

                return FUNCTION_RETURN_OK;
            }

            case 6: {
                call_audio_mod_pos_t* call = (call_audio_mod_pos_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                uint8_t call_result =  audio_mod_pos();

                *call_buffer_return=call_result;

                return FUNCTION_RETURN_OK;
            }

        }
    }

    if (header->func_type==1){
        switch (header->func_id) {

            case 12: {
                call_gfx_renderqueue_add_id_t* call = (call_gfx_renderqueue_add_id_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_renderqueue_add_id(  call->id    );

                return FUNCTION_RETURN_OK;
            }

            case 44: {
                call_gfx_renderqueue_wipe_t* call = (call_gfx_renderqueue_wipe_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_renderqueue_wipe();

                return FUNCTION_RETURN_OK;
            }

            case 1: {
                call_gfx_renderqueue_apply_t* call = (call_gfx_renderqueue_apply_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_renderqueue_apply();

                return FUNCTION_RETURN_OK;
            }

            case 16: {
                call_gfx_spritebuffer_create_t* call = (call_gfx_spritebuffer_create_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                uint8_t call_result =  gfx_spritebuffer_create( (gfx_sprite_t*)(&mem[ call->spritedata ]) ,    call->spriteamount    );

                *call_buffer_return=call_result;

                return FUNCTION_RETURN_OK;
            }

            case 21: {
                call_gfx_spritebuffer_update_t* call = (call_gfx_spritebuffer_update_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_spritebuffer_update(  call->dt  ,    call->spritebuffer_id    );

                return FUNCTION_RETURN_OK;
            }

            case 27: {
                call_gfx_sprite_apply_data_t* call = (call_gfx_sprite_apply_data_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_sprite_apply_data( (gfx_sprite_t*)(&mem[ call->sprite ])   );

                return FUNCTION_RETURN_OK;
            }

            case 17: {
                call_gfx_sprite_set_tileset_t* call = (call_gfx_sprite_set_tileset_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_sprite_set_tileset( (gfx_sprite_t*)(&mem[ call->sprite ]) ,   (gfx_tilesheet_data_t*)(&mem[ call->tsdata ]) ,    call->initial_tile_idx    );

                return FUNCTION_RETURN_OK;
            }

            case 18: {
                call_gfx_sprite_set_tileid_t* call = (call_gfx_sprite_set_tileid_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_sprite_set_tileid( (gfx_sprite_t*)(&mem[ call->sprite ]) ,    call->tile_idx    );

                return FUNCTION_RETURN_OK;
            }

            case 19: {
                call_gfx_sprite_add_animator_t* call = (call_gfx_sprite_add_animator_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                uint8_t call_result =  gfx_sprite_add_animator( (gfx_sprite_t*)(&mem[ call->sprite ]) ,   (gfx_sprite_animator_t*)(&mem[ call->animator ])   );

                *call_buffer_return=call_result;

                return FUNCTION_RETURN_OK;
            }

            case 20: {
                call_gfx_sprite_remove_extension_t* call = (call_gfx_sprite_remove_extension_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                bool call_result =  gfx_sprite_remove_extension( (gfx_sprite_t*)(&mem[ call->sprite ]) ,    call->extension_type    );

                *call_buffer_return=(uint8_t)call_result;

                return FUNCTION_RETURN_OK;
            }

            case 22: {
                call_gfx_spriteanimator_set_animation_t* call = (call_gfx_spriteanimator_set_animation_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_spriteanimator_set_animation(  call->spriteanimator_id  ,    call->anim_idx  ,    call->flags    );

                return FUNCTION_RETURN_OK;
            }

            case 23: {
                call_gfx_spriteanimator_set_animation_with_folowup_t* call = (call_gfx_spriteanimator_set_animation_with_folowup_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_spriteanimator_set_animation_with_folowup(  call->spriteanimator_id  ,    call->anim_idx  ,    call->flags  ,    call->followup_animation_idx  ,    call->followup_animation_flags    );

                return FUNCTION_RETURN_OK;
            }

            case 24: {
                call_gfx_spriteanimator_stop_t* call = (call_gfx_spriteanimator_stop_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_spriteanimator_stop(  call->spriteanimator_id    );

                return FUNCTION_RETURN_OK;
            }

            case 25: {
                call_gfx_spriteanimator_resume_t* call = (call_gfx_spriteanimator_resume_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_spriteanimator_resume(  call->spriteanimator_id    );

                return FUNCTION_RETURN_OK;
            }

            case 9: {
                call_gfx_pixelbuffer_create_t* call = (call_gfx_pixelbuffer_create_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_pixelbuffer_create( (gfx_pixelbuffer_t*)(&mem[ call->initial_data ])   );

                return FUNCTION_RETURN_OK;
            }

            case 10: {
                call_gfx_pixelbuffer_set_active_t* call = (call_gfx_pixelbuffer_set_active_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_pixelbuffer_set_active( (gfx_pixelbuffer_t*)(&mem[ call->pxbuffer ])   );

                return FUNCTION_RETURN_OK;
            }

            case 11: {
                call_gfx_pixelbuffer_get_current_t* call = (call_gfx_pixelbuffer_get_current_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                gfx_pixelbuffer_t* call_result =  gfx_pixelbuffer_get_current();

                *(uint16_t*)call_buffer_return = (uint16_t)(intptr_t)call_result-(intptr_t)mem;
                return FUNCTION_RETURN_OK;
            }

            case 13: {
                call_gfx_pixelbuffer_mount_t* call = (call_gfx_pixelbuffer_mount_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_pixelbuffer_mount( (gfx_pixelbuffer_t*)(&mem[ call->pxb ]) ,    swap16(call->destination)  ,    swap16(call->page_size)    );

                return FUNCTION_RETURN_OK;
            }

            case 33: {
                call_gfx_mount_set_page_t* call = (call_gfx_mount_set_page_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                bool call_result =  gfx_mount_set_page(  call->mount_id  ,    call->page    );

                *call_buffer_return=(uint8_t)call_result;

                return FUNCTION_RETURN_OK;
            }

            case 34: {
                call_gfx_pixelbuffer_wipe_t* call = (call_gfx_pixelbuffer_wipe_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_pixelbuffer_wipe( (gfx_pixelbuffer_t*)(&mem[ call->pxb ]) ,    call->wipe_value    );

                return FUNCTION_RETURN_OK;
            }

            case 26: {
                call_gfx_pixelbuffer_apply_data_t* call = (call_gfx_pixelbuffer_apply_data_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_pixelbuffer_apply_data( (gfx_pixelbuffer_t*)(&mem[ call->pxb ])   );

                return FUNCTION_RETURN_OK;
            }

            case 2: {
                call_gfx_set_palettecolor_t* call = (call_gfx_set_palettecolor_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_set_palettecolor(  call->color_idx  ,    swap16(call->color565)    );

                return FUNCTION_RETURN_OK;
            }

            case 3: {
                call_gfx_get_palettecolor_t* call = (call_gfx_get_palettecolor_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                uint16_t call_result =  gfx_get_palettecolor(  call->color_idx    );
                *((uint16_t*)call_buffer_return)=swap16(call_result);

                return FUNCTION_RETURN_OK;
            }

            case 4: {
                call_gfx_set_palette_from_assset_t* call = (call_gfx_set_palette_from_assset_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_set_palette_from_assset(  call->asset_id  ,    call->fill_unused_with_idx    );

                return FUNCTION_RETURN_OK;
            }

            case 5: {
                call_gfx_set_font_from_asset_t* call = (call_gfx_set_font_from_asset_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_set_font_from_asset(  call->asset_id    );

                return FUNCTION_RETURN_OK;
            }

            case 6: {
                call_gfx_get_pixel_t* call = (call_gfx_get_pixel_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                uint8_t call_result =  gfx_get_pixel(  swap16(call->x)  ,    swap16(call->y)    );

                *call_buffer_return=call_result;

                return FUNCTION_RETURN_OK;
            }

            case 7: {
                call_gfx_draw_pixel_t* call = (call_gfx_draw_pixel_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_draw_pixel(  swap16(call->x)  ,    swap16(call->y)  ,    call->color_idx    );

                return FUNCTION_RETURN_OK;
            }

            case 37: {
                call_gfx_clip_rect_t* call = (call_gfx_clip_rect_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_clip_rect(  swap16(call->x)  ,    swap16(call->y)  ,    swap16(call->w)  ,    swap16(call->h)    );

                return FUNCTION_RETURN_OK;
            }

            case 38: {
                call_gfx_clip_clear_t* call = (call_gfx_clip_clear_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_clip_clear();

                return FUNCTION_RETURN_OK;
            }

            case 39: {
                call_gfx_draw_rect_t* call = (call_gfx_draw_rect_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_draw_rect(  swap16(call->x)  ,    swap16(call->y)  ,    swap16(call->width)  ,    swap16(call->height)  ,    call->color  ,    call->flags    );

                return FUNCTION_RETURN_OK;
            }

            case 40: {
                call_gfx_draw_circle_t* call = (call_gfx_draw_circle_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_draw_circle(  swap16(call->x)  ,    swap16(call->y)  ,    swap16(call->radius)  ,    call->color  ,    call->flags    );

                return FUNCTION_RETURN_OK;
            }

            case 41: {
                call_gfx_draw_button_t* call = (call_gfx_draw_button_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_draw_button( (gfx_button_t*)(&mem[ call->btn ])   );

                return FUNCTION_RETURN_OK;
            }

            case 8: {
                call_gfx_draw_char_t* call = (call_gfx_draw_char_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_draw_char(  swap16(call->x)  ,    swap16(call->y)  ,    call->ch  ,    call->color_idx  ,    call->bg_index    );

                return FUNCTION_RETURN_OK;
            }

            case 14: {
                call_gfx_draw_text_t* call = (call_gfx_draw_text_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_draw_text(  swap16(call->x)  ,    swap16(call->y)  ,   (char*)(&mem[ call->txt ]) ,    call->color_idx  ,    call->bg_index    );

                return FUNCTION_RETURN_OK;
            }

            case 28: {
                call_gfx_tilesheet_current_set_pixel_size_t* call = (call_gfx_tilesheet_current_set_pixel_size_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_tilesheet_current_set_pixel_size(  call->px_width  ,    call->px_height    );

                return FUNCTION_RETURN_OK;
            }

            case 29: {
                call_gfx_tilesheet_current_set_t* call = (call_gfx_tilesheet_current_set_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_tilesheet_current_set( (gfx_tilesheet_data_t*)(&mem[ call->tsdata ])   );

                return FUNCTION_RETURN_OK;
            }

            case 30: {
                call_gfx_draw_tile_t* call = (call_gfx_draw_tile_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_draw_tile(  call->x  ,    call->y  ,    call->tile_number    );

                return FUNCTION_RETURN_OK;
            }

            case 31: {
                call_gfx_draw_tilemap_layer_t* call = (call_gfx_draw_tilemap_layer_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_draw_tilemap_layer(  call->x  ,    call->y  ,   (gfx_tilemap_layer_t*)(&mem[ call->tilemap ])   );

                return FUNCTION_RETURN_OK;
            }

            case 32: {
                call_gfx_load_tilemap_layer_t* call = (call_gfx_load_tilemap_layer_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_load_tilemap_layer( (gfx_tilemap_t*)(&mem[ call->tm ]) ,   (gfx_tilemap_layer_t*)(&mem[ call->out_layer ]) ,    call->layer_nr    );

                return FUNCTION_RETURN_OK;
            }

            case 42: {
                call_gfx_debug_drawinfo_pixelbuffer_t* call = (call_gfx_debug_drawinfo_pixelbuffer_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_debug_drawinfo_pixelbuffer(  swap16(call->x)  ,    swap16(call->y)  ,   (gfx_pixelbuffer_t*)(&mem[ call->pxb ]) ,    call->coltext  ,    call->col_bg    );

                return FUNCTION_RETURN_OK;
            }

            case 43: {
                call_gfx_debug_drawinfo_keyboard_t* call = (call_gfx_debug_drawinfo_keyboard_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_debug_drawinfo_keyboard(  swap16(call->x)  ,    swap16(call->y)  ,   (keyboard_mapping_t*)(&mem[ call->keyb ]) ,    call->coltext  ,    call->col_bg    );

                return FUNCTION_RETURN_OK;
            }

            case 45: {
                call_gfx_debug_drawinfo_mouse_t* call = (call_gfx_debug_drawinfo_mouse_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_debug_drawinfo_mouse(  swap16(call->x)  ,    swap16(call->y)  ,    call->coltext  ,    call->col_bg    );

                return FUNCTION_RETURN_OK;
            }

            case 46: {
                call_gfx_debug_drawinfo_gamepad_t* call = (call_gfx_debug_drawinfo_gamepad_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  gfx_debug_drawinfo_gamepad(  swap16(call->x)  ,    swap16(call->y)  ,    call->gamepad_idx  ,    call->coltext  ,    call->col_bg    );

                return FUNCTION_RETURN_OK;
            }

        }
    }

    if (header->func_type==2){
        switch (header->func_id) {

            case 8: {
                call_io_before_tick_t* call = (call_io_before_tick_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  io_before_tick();

                return FUNCTION_RETURN_OK;
            }

            case 1: {
                call_io_keyboard_is_pressed_t* call = (call_io_keyboard_is_pressed_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                bool call_result =  io_keyboard_is_pressed(  call->keycode    );

                *call_buffer_return=(uint8_t)call_result;

                return FUNCTION_RETURN_OK;
            }

            case 2: {
                call_io_keyboard_is_down_t* call = (call_io_keyboard_is_down_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                bool call_result =  io_keyboard_is_down(  call->keycode    );

                *call_buffer_return=(uint8_t)call_result;

                return FUNCTION_RETURN_OK;
            }

            case 3: {
                call_io_keyboard_is_released_t* call = (call_io_keyboard_is_released_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                bool call_result =  io_keyboard_is_released(  call->keycode    );

                *call_buffer_return=(uint8_t)call_result;

                return FUNCTION_RETURN_OK;
            }

            case 4: {
                call_io_keyboardmapping_register_t* call = (call_io_keyboardmapping_register_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  io_keyboardmapping_register( (keyboard_mapping_t*)(&mem[ call->address ]) ,    call->amount    );

                return FUNCTION_RETURN_OK;
            }

            case 5: {
                call_io_keyboardmapping_unregister_t* call = (call_io_keyboardmapping_unregister_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  io_keyboardmapping_unregister();

                return FUNCTION_RETURN_OK;
            }

            case 6: {
                call_io_gamepad_is_active_t* call = (call_io_gamepad_is_active_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                bool call_result =  io_gamepad_is_active(  call->gamepad_id    );

                *call_buffer_return=(uint8_t)call_result;

                return FUNCTION_RETURN_OK;
            }

            case 9: {
                call_io_lock_input_t* call = (call_io_lock_input_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  io_lock_input(  call->lock_it    );

                return FUNCTION_RETURN_OK;
            }

        }
    }

    if (header->func_type==6){
        switch (header->func_id) {

            case 2: {
                call_utils_random_uint16_t* call = (call_utils_random_uint16_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                uint16_t call_result =  utils_random_uint16();
                *((uint16_t*)call_buffer_return)=swap16(call_result);

                return FUNCTION_RETURN_OK;
            }

            case 1: {
                call_ng_debug_value_t* call = (call_ng_debug_value_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  ng_debug_value(  swap16(call->v1)  ,    swap16(call->v2)    );

                return FUNCTION_RETURN_OK;
            }

            case 3: {
                call_ng_debug_pointer_t* call = (call_ng_debug_pointer_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                  ng_debug_pointer( (void*)(&mem[ call->ptr ]) ,    call->data    );

                return FUNCTION_RETURN_OK;
            }

        }
    }

    return FUNCTION_RETURN_ERROR;
}

