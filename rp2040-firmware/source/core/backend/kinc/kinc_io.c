#ifdef __KINC__

/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 No0ne (https://github.com/No0ne)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "kinc_ng.h"

#include "../../../ng_io.h"
#include "../../../ng_gfx.h"
#include "../../memory.h"

#include <kinc/input/keyboard.h>
#include <kinc/input/mouse.h>
#include <kinc/input/gamepad.h>

extern keyboard_environment_t kenv;

#define KEYBOARD_REPORT_MAX_KEYCODES 6

extern uint16_t window_width, window_height;
extern float    window_factor_x, window_factor_y;

typedef struct hid_keyboard_report_t
{
  uint8_t modifier;   
  uint8_t keycode_amount;
  uint8_t keycode[KEYBOARD_REPORT_MAX_KEYCODES]; 
} hid_keyboard_report_t;

hid_keyboard_report_t kb_pressed;
hid_keyboard_report_t kb_down;
hid_keyboard_report_t kb_released;



uint8_t HID2KINC_0[]={
KINC_KEY_UNKNOWN             , //0x00
KINC_KEY_UNKNOWN                 ,    //0x01
KINC_KEY_UNKNOWN                 ,    //0x02
KINC_KEY_UNKNOWN                 ,    //0x03
KINC_KEY_A                     ,    //0x04
KINC_KEY_B                     ,    //0x05
KINC_KEY_C                     ,    //0x06
KINC_KEY_D                     ,    //0x07
KINC_KEY_E                     ,    //0x08
KINC_KEY_F                     ,    //0x09
KINC_KEY_G                     ,    //0x0A
KINC_KEY_H                     ,    //0x0B
KINC_KEY_I                     ,    //0x0C
KINC_KEY_J                     ,    //0x0D
KINC_KEY_K                     ,    //0x0E
KINC_KEY_L                     ,    //0x0F
KINC_KEY_M                     ,    //0x10
KINC_KEY_N                     ,    //0x11
KINC_KEY_O                     ,    //0x12
KINC_KEY_P                     ,    //0x13
KINC_KEY_Q                     ,    //0x14
KINC_KEY_R                     ,    //0x15
KINC_KEY_S                     ,    //0x16
KINC_KEY_T                     ,    //0x17
KINC_KEY_U                     ,    //0x18
KINC_KEY_V                     ,    //0x19
KINC_KEY_W                     ,    //0x1A
KINC_KEY_X                     ,    //0x1B
KINC_KEY_Y                     ,    //0x1C
KINC_KEY_Z                     ,    //0x1D
KINC_KEY_1                     ,    //0x1E
KINC_KEY_2                     ,    //0x1F
KINC_KEY_3                     ,    //0x20
KINC_KEY_4                     ,    //0x21
KINC_KEY_5                     ,    //0x22
KINC_KEY_6                     ,    //0x23
KINC_KEY_7                     ,    //0x24
KINC_KEY_8                     ,    //0x25
KINC_KEY_9                     ,    //0x26
KINC_KEY_0                     ,    //0x27
KINC_KEY_RETURN                 ,    //0x28
KINC_KEY_ESCAPE                ,    //0x29
KINC_KEY_BACKSPACE             ,    //0x2A
KINC_KEY_TAB                   ,    //0x2B
KINC_KEY_SPACE                 ,    //0x2C
KINC_KEY_SUBTRACT                 ,    //0x2D
KINC_KEY_EQUALS                 ,    //0x2E
KINC_KEY_OPEN_BRACKET          ,    //0x2F
KINC_KEY_CLOSE_BRACKET        ,    //0x30
KINC_KEY_BACK_SLASH             ,    //0x31
KINC_KEY_UNKNOWN              ,    //0x32
KINC_KEY_SEMICOLON             ,    //0x33
KINC_KEY_UNKNOWN            ,    //0x34  // TODO: KINC_KEY_APOSTROPHE
KINC_KEY_UNKNOWN                 ,    //0x35 // TODO: KINC_KEY_GRAVE
KINC_KEY_COMMA                 ,    //0x36
KINC_KEY_PERIOD                ,    //0x37
KINC_KEY_SLASH                 ,    //0x38
KINC_KEY_CAPS_LOCK             ,    //0x39
KINC_KEY_F1                    ,    //0x3A
KINC_KEY_F2                    ,    //0x3B
KINC_KEY_F3                    ,    //0x3C
KINC_KEY_F4                    ,    //0x3D
KINC_KEY_F5                    ,    //0x3E
KINC_KEY_F6                    ,    //0x3F
KINC_KEY_F7                    ,    //0x40
KINC_KEY_F8                    ,    //0x41
KINC_KEY_F9                    ,    //0x42
KINC_KEY_F10                   ,    //0x43
KINC_KEY_F11                   ,    //0x44
KINC_KEY_F12                   ,    //0x45
KINC_KEY_PRINT_SCREEN          ,    //0x46
KINC_KEY_SCROLL_LOCK           ,    //0x47
KINC_KEY_PAUSE                 ,    //0x48
KINC_KEY_INSERT                ,    //0x49
KINC_KEY_HOME                  ,    //0x4A
KINC_KEY_PAGE_UP               ,    //0x4B
KINC_KEY_DELETE                ,    //0x4C
KINC_KEY_END                   ,    //0x4D
KINC_KEY_PAGE_DOWN             ,    //0x4E
KINC_KEY_RIGHT           ,    //0x4F
KINC_KEY_LEFT            ,    //0x50
KINC_KEY_DOWN            ,    //0x51
KINC_KEY_UP              ,    //0x52
KINC_KEY_NUM_LOCK              ,    //0x53
KINC_KEY_DIVIDE         ,    //0x54 // TODO: NUMPAD-Commands mapped to main-keyboard keys
KINC_KEY_MULTIPLY       ,    //0x55
KINC_KEY_SUBTRACT       ,    //0x56
KINC_KEY_ADD            ,    //0x57
KINC_KEY_RETURN          ,    //0x58
KINC_KEY_NUMPAD_1              ,    //0x59
KINC_KEY_NUMPAD_2              ,    //0x5A
KINC_KEY_NUMPAD_3              ,    //0x5B
KINC_KEY_NUMPAD_4              ,    //0x5C
KINC_KEY_NUMPAD_5              ,    //0x5D
KINC_KEY_NUMPAD_6              ,    //0x5E
KINC_KEY_NUMPAD_7              ,    //0x5F
KINC_KEY_NUMPAD_8              ,    //0x60
KINC_KEY_NUMPAD_9              ,    //0x61
KINC_KEY_NUMPAD_0              ,    //0x62
KINC_KEY_COMMA        ,    //0x63   // TODO: NUMPAD_DECIMAL => MAINKEYBOARD-komma
KINC_KEY_UNKNOWN              ,    //0x64  // TODO: EUROPE2
KINC_KEY_UNKNOWN           ,    //0x65 // TODO: KINC_KEY_APPLICATION
KINC_KEY_UNKNOWN                 ,    //0x66  // TODO: KINC_KEY_POWER
KINC_KEY_UNKNOWN          ,    //0x67 // TODO: KINC_KEY_KEYPAD_EQUAL
KINC_KEY_F13                   ,    //0x68
KINC_KEY_F14                   ,    //0x69
KINC_KEY_F15                   ,    //0x6A
KINC_KEY_F16                   ,    //0x6B
KINC_KEY_F17                   ,    //0x6C
KINC_KEY_F18                   ,    //0x6D
KINC_KEY_F19                   ,    //0x6E
KINC_KEY_F20                   ,    //0x6F
KINC_KEY_F21                   ,    //0x70
KINC_KEY_F22                   ,    //0x71
KINC_KEY_F23                   ,    //0x72
KINC_KEY_F24                   ,    //0x73
KINC_KEY_EXECUTE               ,    //0x74
KINC_KEY_HELP                  ,    //0x75
KINC_KEY_HOME                 ,    //0x76  // Is MENU==KINC_HOME?
KINC_KEY_SELECT                ,    //0x77
KINC_KEY_UNKNOWN                 ,    //0x78  //TODO: KINC_KEY_STOP  
KINC_KEY_UNKNOWN                 ,    //0x79  //TODO: KINC_KEY_AGAIN 
KINC_KEY_UNKNOWN                 ,    //0x7A  //TODO: KINC_KEY_UNDO  
KINC_KEY_UNKNOWN                 ,    //0x7B  //TODO: KINC_KEY_CUT   
KINC_KEY_UNKNOWN                 ,    //0x7C  //TODO: KINC_KEY_COPY  
KINC_KEY_UNKNOWN                 ,    //0x7D  //TODO: KINC_KEY_PASTE 
KINC_KEY_UNKNOWN                 ,    //0x7E  //TODO: KINC_KEY_FIND  
KINC_KEY_UNKNOWN                 ,    //0x7F  //TODO: KINC_KEY_MUTE  
KINC_KEY_VOLUME_UP             ,    //0x80
KINC_KEY_VOLUME_DOWN           ,    //0x81
KINC_KEY_UNKNOWN   ,    //0x82 //TODO: KINC_KEY_LOCKING_CAPS_LOCK  
KINC_KEY_UNKNOWN   ,    //0x83 //TODO: KINC_KEY_LOCKING_NUM_LOCK   
KINC_KEY_UNKNOWN   ,    //0x84 //TODO: KINC_KEY_LOCKING_SCROLL_LOCK
KINC_KEY_COMMA          ,    //0x85
KINC_KEY_UNKNOWN     ,    //0x86 // TODO: KINC_KEY_KEYPAD_EQUAL_SIG
KINC_KEY_UNKNOWN     ,    //0x87 // TODO: KINC_KEY_KANJI1          
KINC_KEY_UNKNOWN     ,    //0x88 // TODO: KINC_KEY_KANJI2          
KINC_KEY_UNKNOWN     ,    //0x89 // TODO: KINC_KEY_KANJI3          
KINC_KEY_UNKNOWN     ,    //0x8A // TODO: KINC_KEY_KANJI4          
KINC_KEY_UNKNOWN     ,    //0x8B // TODO: KINC_KEY_KANJI5          
KINC_KEY_UNKNOWN     ,    //0x8C // TODO: KINC_KEY_KANJI6          
KINC_KEY_UNKNOWN     ,    //0x8D // TODO: KINC_KEY_KANJI7          
KINC_KEY_UNKNOWN     ,    //0x8E // TODO: KINC_KEY_KANJI8          
KINC_KEY_UNKNOWN     ,    //0x8F // TODO: KINC_KEY_KANJI9          
KINC_KEY_UNKNOWN     ,    //0x90 // TODO: KINC_KEY_LANG1           
KINC_KEY_UNKNOWN     ,    //0x91 // TODO: KINC_KEY_LANG2           
KINC_KEY_UNKNOWN     ,    //0x92 // TODO: KINC_KEY_LANG3           
KINC_KEY_UNKNOWN     ,    //0x93 // TODO: KINC_KEY_LANG4           
KINC_KEY_UNKNOWN     ,    //0x94 // TODO: KINC_KEY_LANG5           
KINC_KEY_UNKNOWN     ,    //0x95 // TODO: KINC_KEY_LANG6           
KINC_KEY_UNKNOWN     ,    //0x96 // TODO: KINC_KEY_LANG7           
KINC_KEY_UNKNOWN     ,    //0x97 // TODO: KINC_KEY_LANG8           
KINC_KEY_UNKNOWN     ,    //0x98 // TODO: KINC_KEY_LANG9           
KINC_KEY_UNKNOWN     ,    //0x99 // TODO: KINC_KEY_ALTERNATE_ERASE 
KINC_KEY_UNKNOWN     ,    //0x9A // TODO: KINC_KEY_SYSREQ_ATTENTION
KINC_KEY_CANCEL                ,    //0x9B
KINC_KEY_CLEAR                 ,    //0x9C
KINC_KEY_UNKNOWN                 ,    //0x9D // TODO: KINC_KEY_PRIOR
KINC_KEY_RETURN                ,    //0x9E
KINC_KEY_SEPARATOR             ,    //0x9F
KINC_KEY_UNKNOWN           ,    //0xA0 // TODO: KINC_KEY_OUT        
KINC_KEY_UNKNOWN           ,    //0xA1 // TODO: KINC_KEY_OPER       
KINC_KEY_UNKNOWN           ,    //0xA2 // TODO: KINC_KEY_CLEAR_AGAIN
KINC_KEY_UNKNOWN           ,    //0xA3 // TODO: KINC_KEY_CRSEL_PROPS
KINC_KEY_EXSEL                 ,    //0xA4
};

uint8_t HID2KINC_1[] = {
KINC_KEY_CONTROL      ,        // 0xE0  // TODO: No distiction between left and right control in kinc?
KINC_KEY_SHIFT       ,        // 0xE1
KINC_KEY_ALT         ,        // 0xE2
KINC_KEY_UNKNOWN         ,        // 0xE3 // KINC_KEY_GUI_LEFT
KINC_KEY_CONTROL    ,        // 0xE4
KINC_KEY_SHIFT      ,        // 0xE5
KINC_KEY_ALT        ,        // 0xE6
KINC_KEY_UNKNOWN        ,        // 0xE7   // TODO: KINC_KEY_GUI_RIGHT
};

// THIS mapping is actually pointless....for a lot of inputs
uint8_t KINC2_HID[]={
HID_KEY_NONE     ,//0
HID_KEY_NONE        ,//1   // Android //TODO: HID_KEY_BACK
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE      ,//3 // TODO: HID_KEY_CANCEL
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE        ,//6
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_BACKSPACE   ,//8
HID_KEY_TAB         ,//9
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE       ,//12
HID_KEY_ENTER      ,//13
HID_KEY_NONE       , // NO KINC VALUE
HID_KEY_NONE       ,// NO KINC VALUE
HID_KEY_SHIFT_LEFT       ,//16
HID_KEY_CONTROL_LEFT    ,//17
HID_KEY_ALT_LEFT         ,//18
HID_KEY_PAUSE       ,//19
HID_KEY_CAPS_LOCK   ,//20
HID_KEY_NONE        ,//21
HID_KEY_NONE        ,//22
HID_KEY_NONE       ,//23
HID_KEY_NONE       ,//24
HID_KEY_NONE       ,//25
HID_KEY_NONE       ,//25
HID_KEY_ESCAPE      ,//27
HID_KEY_NONE     ,//28
HID_KEY_NONE ,//29
HID_KEY_NONE     ,//30
HID_KEY_NONE ,//31
HID_KEY_SPACE       ,//32
HID_KEY_PAGE_UP     ,//33
HID_KEY_PAGE_DOWN   ,//34
HID_KEY_END         ,//35
HID_KEY_HOME        ,//36
HID_KEY_ARROW_LEFT        ,//37
HID_KEY_ARROW_UP          ,//38
HID_KEY_ARROW_RIGHT       ,//39
HID_KEY_ARROW_DOWN        ,//40
HID_KEY_SELECT      ,//41
HID_KEY_PRINT_SCREEN       ,//42
HID_KEY_EXECUTE     ,//43
HID_KEY_PRINT_SCREEN,// 44
HID_KEY_INSERT      ,//45
HID_KEY_DELETE      ,//46
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_0           ,//48      
HID_KEY_1           ,//49
HID_KEY_2           ,//50
HID_KEY_3           ,//51
HID_KEY_4           ,//52
HID_KEY_5           ,//53
HID_KEY_6           ,//54
HID_KEY_7           ,//55
HID_KEY_8           ,//56
HID_KEY_9           ,//57
HID_KEY_SEMICOLON      ,//58 //TODO: COLON
HID_KEY_SEMICOLON   ,//59
HID_KEY_COMMA   ,//60 // TODO: lessthan
HID_KEY_EQUAL      ,//61
HID_KEY_NONE,// 62  // TODO: greater than
HID_KEY_SLASH,// 63 // TODO: questionmark
HID_KEY_2          ,//64 // TODO: @
HID_KEY_A           ,//65
HID_KEY_B           ,//66
HID_KEY_C           ,//67
HID_KEY_D           ,//68
HID_KEY_E           ,//69
HID_KEY_F           ,//70
HID_KEY_G           ,//71
HID_KEY_H           ,//72
HID_KEY_I           ,//73
HID_KEY_J           ,//74
HID_KEY_K           ,//75
HID_KEY_L           ,//76
HID_KEY_M           ,//77
HID_KEY_N           ,//78
HID_KEY_O           ,//79
HID_KEY_P           ,//80
HID_KEY_Q           ,//81
HID_KEY_R           ,//82
HID_KEY_S           ,//83
HID_KEY_T           ,//84
HID_KEY_U           ,//85
HID_KEY_V           ,//86
HID_KEY_W           ,//87
HID_KEY_X           ,//88
HID_KEY_Y           ,//89
HID_KEY_Z           ,//90
HID_KEY_NONE         ,//91 //TODO: windows
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE,// 93 // TODO: HID_KEY_CONTEXT_MENU
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE       ,//95 // TODO: HID_KEY_SLEEP
HID_KEY_KEYPAD_0    ,//96
HID_KEY_KEYPAD_1    ,//97
HID_KEY_KEYPAD_2    ,//98
HID_KEY_KEYPAD_3    ,//99
HID_KEY_KEYPAD_4    ,//100
HID_KEY_KEYPAD_5    ,//101
HID_KEY_KEYPAD_6    ,//102
HID_KEY_KEYPAD_7    ,//103
HID_KEY_KEYPAD_8    ,//104
HID_KEY_KEYPAD_9    ,//105
HID_KEY_NONE    ,//106 // *
HID_KEY_EQUAL         ,//107 //TODO: +
HID_KEY_SEPARATOR   ,//108
HID_KEY_MINUS    ,//109

// HID_KEY_DECIMAL     ,//110
// HID_KEY_DIVIDE      ,//111
HID_KEY_NONE     ,//110
HID_KEY_NONE      ,//111

HID_KEY_F1          ,//112
HID_KEY_F2          ,//113
HID_KEY_F3          ,//114
HID_KEY_F4          ,//115
HID_KEY_F5          ,//116
HID_KEY_F6          ,//117
HID_KEY_F7          ,//118
HID_KEY_F8          ,//119
HID_KEY_F9          ,//120
HID_KEY_F10  ,              //121
HID_KEY_F11  ,              //122
HID_KEY_F12  ,              //123
HID_KEY_F13  ,              //124
HID_KEY_F14  ,              //125
HID_KEY_F15  ,              //126
HID_KEY_F16  ,              //127
HID_KEY_F17  ,              //128
HID_KEY_F18  ,              //129
HID_KEY_F19  ,              //130
HID_KEY_F20  ,              //131
HID_KEY_F21  ,              //132
HID_KEY_F22  ,              //133
HID_KEY_F23  ,              //134
HID_KEY_F24  ,              //135
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NUM_LOCK          , //144
HID_KEY_SCROLL_LOCK       , //145

//HID_KEY_WIN_OEM_FJ_JISHO  , //146
//HID_KEY_WIN_OEM_FJ_MASSHOU, //147
//HID_KEY_WIN_OEM_FJ_TOUROKU, //148
//HID_KEY_WIN_OEM_FJ_LOYA   , //149
//HID_KEY_WIN_OEM_FJ_ROYA   ,//150
HID_KEY_NONE , //146
HID_KEY_NONE, //147
HID_KEY_NONE, //148
HID_KEY_NONE , //149
HID_KEY_NONE ,//150

HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE

//HID_KEY_CIRCUMFLEX         ,//160
//HID_KEY_EXCLAMATION        ,//161
//HID_KEY_DOUBLE_QUOTE       ,//162
//HID_KEY_HASH               ,//163
//HID_KEY_DOLLAR             ,//164
//HID_KEY_PERCENT            ,//165
//HID_KEY_AMPERSAND          ,//166
//HID_KEY_UNDERSCORE         ,//167
HID_KEY_NONE       ,//160
HID_KEY_NONE       ,//161
HID_KEY_NONE       ,//162
HID_KEY_NONE       ,//163
HID_KEY_NONE       ,//164
HID_KEY_NONE       ,//165
HID_KEY_NONE       ,//166
HID_KEY_NONE       ,//167

//HID_KEY_OPEN_PAREN         ,//168
//HID_KEY_CLOSE_PAREN        ,//169
//HID_KEY_ASTERISK           ,//170
//HID_KEY_PLUS               ,//171
//HID_KEY_PIPE               ,//172
//HID_KEY_HYPHEN_MINUS       ,//173
//HID_KEY_OPEN_CURLY_BRACKET ,//174
//HID_KEY_CLOSE_CURLY_BRACKET,// 175
//HID_KEY_TILDE              ,//176
HID_KEY_NONE  ,//168
HID_KEY_NONE  ,//169
HID_KEY_NONE  ,//170
HID_KEY_NONE  ,//171
HID_KEY_NONE  ,//172
HID_KEY_NONE  ,//173
HID_KEY_NONE  ,//174
HID_KEY_NONE  ,// 175
HID_KEY_NONE  ,//176


HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE

//HID_KEY_VOLUME_MUTE        ,//181
HID_KEY_NONE        ,//181

HID_KEY_VOLUME_DOWN        ,//182
HID_KEY_VOLUME_UP          ,//183
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_COMMA              ,//188
HID_KEY_PERIOD             ,//190
HID_KEY_SLASH              ,//191

//HID_KEY_BACK_QUOTE         ,//192
HID_KEY_NONE         ,//192

HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE
HID_KEY_NONE, // NO KINC VALUE

// HID_KEY_OPEN_BRACKET       ,//219
// HID_KEY_BACK_SLASH         ,//220
// HID_KEY_CLOSE_BRACKET      ,//221
// HID_KEY_QUOTE              ,//222
// HID_KEY_NONE, // NO KINC VALUE
// HID_KEY_META               ,//224
// HID_KEY_ALT_GR             ,//225
// HID_KEY_NONE, // NO KINC VALUE
// HID_KEY_WIN_ICO_HELP       ,//227
// HID_KEY_WIN_ICO_00         ,//228
// HID_KEY_NONE, // NO KINC VALUE
// HID_KEY_WIN_ICO_CLEAR      ,//230
// HID_KEY_NONE, // NO KINC VALUE
// HID_KEY_NONE, // NO KINC VALUE
// HID_KEY_WIN_OEM_RESET      ,//233
// HID_KEY_WIN_OEM_JUMP       ,//234
// HID_KEY_WIN_OEM_PA1        ,//235
// HID_KEY_WIN_OEM_PA2        ,//236
// HID_KEY_WIN_OEM_PA3        ,//237
// HID_KEY_WIN_OEM_WSCTRL     ,//238
// HID_KEY_WIN_OEM_CUSEL      ,//239
// HID_KEY_WIN_OEM_ATTN       ,//240
// HID_KEY_WIN_OEM_FINISH     ,//241
// HID_KEY_WIN_OEM_COPY       ,//242
// HID_KEY_WIN_OEM_AUTO       ,//243
// HID_KEY_WIN_OEM_ENLW       ,//244
// HID_KEY_WIN_OEM_BACK_TAB   ,//245
// HID_KEY_ATTN               ,//246
// HID_KEY_CRSEL              ,//247
// HID_KEY_EXSEL              ,//248
// HID_KEY_EREOF              ,//249
// HID_KEY_PLAY               ,//250
// HID_KEY_ZOOM               ,//251
// HID_KEY_NONE, // NO KINC VALUE
// HID_KEY_PA1                ,//253
// HID_KEY_WIN_OEM_CLEAR      ,//254

HID_KEY_NONE   ,//219
HID_KEY_NONE   ,//220
HID_KEY_NONE   ,//221
HID_KEY_NONE   ,//222
HID_KEY_NONE    , // NO KINC VALUE
HID_KEY_NONE   ,//224
HID_KEY_NONE   ,//225
HID_KEY_NONE    , // NO KINC VALUE
HID_KEY_NONE   ,//227
HID_KEY_NONE   ,//228
HID_KEY_NONE    , // NO KINC VALUE
HID_KEY_NONE   ,//230
HID_KEY_NONE    , // NO KINC VALUE
HID_KEY_NONE    , // NO KINC VALUE
HID_KEY_NONE   ,//233
HID_KEY_NONE   ,//234
HID_KEY_NONE   ,//235
HID_KEY_NONE   ,//236
HID_KEY_NONE   ,//237
HID_KEY_NONE   ,//238
HID_KEY_NONE   ,//239
HID_KEY_NONE   ,//240
HID_KEY_NONE   ,//241
HID_KEY_NONE   ,//242
HID_KEY_NONE   ,//243
HID_KEY_NONE   ,//244
HID_KEY_NONE   ,//245
HID_KEY_NONE   ,//246
HID_KEY_NONE   ,//247
HID_KEY_NONE   ,//248
HID_KEY_NONE   ,//249
HID_KEY_NONE   ,//250
HID_KEY_NONE   ,//251
HID_KEY_NONE    , // NO KINC VALUE
HID_KEY_NONE   ,//253
HID_KEY_NONE   ,//254
};


uint8_t hid2backend(int keycode){
  if (keycode <=0xA4){
    return HID2KINC_0[keycode];
  }
  else if (keycode >=0xe0 && keycode <=0xe7){
    return HID2KINC_1[keycode-0xe0];
  } else {
    ASSERT_STRICT(false && "UNKNOWN KEYCODE!");
    return KINC_KEY_UNKNOWN;
  }
}

uint8_t backend2hid(int keycode) {
  return KINC2_HID[keycode];
}


float mouse_factor;

#include <stdio.h>

static void keyboard_pressed(int _key, void* data)
{
  printf("pressed: %d\n",_key)  ;
  uint8_t key = backend2hid((uint8_t)_key);
  kb_pressed.keycode[kb_pressed.keycode_amount++]=key;
  kb_down.keycode[kb_down.keycode_amount++]=key;
}

static void keyboard_released(int _key, void* data)
{
  printf("released: %d\n",_key)  ;
  uint8_t key = backend2hid((uint8_t)_key);
 
  kb_released.keycode[kb_released.keycode_amount++]=key;
  for (int i=0;i<kb_down.keycode_amount;i++){
    if (kb_down.keycode[i]==key){
      if (i < kb_down.keycode_amount-1){
        // not last keycode in list?
        kb_down.keycode[i]=kb_down.keycode[kb_down.keycode_amount-1];
      } else {
        kb_down.keycode[i]=0;
      }
      kb_down.keycode_amount--;
    }
  }
}

static void callback_mouse_moved(int window, int x, int y, int dx, int dy,void* data){
  *mm_mouse_x=x*window_factor_x;
  *mm_mouse_y=y*window_factor_y;
}

static void callback_mouse_button_pressed(int window,int button,int x,int y,void* data){
  *mm_mouse_btn_state |= (1 << button);
}

static void callback_mouse_button_released(int window,int button,int x,int y,void* data){
  *mm_mouse_btn_state &= ~(1<<button);
}

static void callback_mouse_wheel_updated(int window,int delta, void* data){
  *mm_mouse_wheel = delta;
}

typedef struct {
    uint32_t identifier;
    uint8_t axis_lr;       // Axis for left-right control
    uint8_t axis_ud;       // Axis for up-down control
    int8_t lr_invert;      // -1 if left is positive, 1 if right is positive
    int8_t ud_invert;      // -1 if up is positive, 1 if down is positive
    uint8_t button_map[10]; // Mapping of button numbers to expected buttons
} gamepad_mapping_t;

gamepad_mapping_t gamepad_mappings[]={
    {  // CSL PS3-Controller clone
        .identifier=4065002617,
        .axis_lr=0,
        .lr_invert=1,
        .axis_ud=1,
        .ud_invert=-1,
        .button_map={
            [0]=GP_BTN_TOP,
            [1]=GP_BTN_RIGHT,
            [2]=GP_BTN_BOTTOM,
            [3]=GP_BTN_LEFT,
            [4]=GP_BTN_REAR_LEFT,
            [5]=GP_BTN_REAR_RIGHT,
            [8]=GP_BTN_SELECT,
            [9]=GP_BTN_START
        }
    },
    {  // MicronTek / Tracer Glider
        .identifier=3362886800,
        .axis_lr=0,
        .lr_invert=1,
        .axis_ud=1,
        .ud_invert=-1,
        .button_map={
            [0]=GP_BTN_TOP,
            [1]=GP_BTN_RIGHT,
            [2]=GP_BTN_BOTTOM,
            [3]=GP_BTN_LEFT,
            [4]=GP_BTN_REAR_LEFT,
            [5]=GP_BTN_REAR_RIGHT,
            [8]=GP_BTN_SELECT,
            [9]=GP_BTN_START
        }
    },    
    { // XBox X/S
        .identifier=3367356690,
        .axis_lr=6,
        .lr_invert=1,
        .axis_ud=7,
        .ud_invert=-1,
        .button_map={
            [3]=GP_BTN_TOP,
            [1]=GP_BTN_RIGHT,
            [0]=GP_BTN_BOTTOM,
            [2]=GP_BTN_LEFT,
            [4]=GP_BTN_REAR_LEFT,
            [5]=GP_BTN_REAR_RIGHT,
            [6]=GP_BTN_SELECT,
            [7]=GP_BTN_START
        }
    },
    { // Logitech RumblePad 2
        .identifier=422644551,
        .axis_lr=0,
        .lr_invert=1,
        .axis_ud=1,
        .ud_invert=-1,
        .button_map={
            [3]=GP_BTN_TOP,
            [2]=GP_BTN_RIGHT,
            [1]=GP_BTN_BOTTOM,
            [0]=GP_BTN_LEFT,
            [4]=GP_BTN_REAR_LEFT,
            [5]=GP_BTN_REAR_RIGHT,
            [8]=GP_BTN_SELECT,
            [9]=GP_BTN_START
        }
    },
};

gamepad_mapping_t* gamepad2mapping[GAMEPAD_MAX_DEVICES];

uint32_t hash_function(const char* str1) {
    const uint32_t prime1 = 31;
    const uint32_t prime2 = 37;
    uint32_t hash = 0;
    int i;

    for (i = 0; str1[i] != '\0' && i < 10; i++) {
        hash = hash * prime1 + (uint32_t)str1[i];
    }

    return hash;
}

#define GP_REPORT_TYPE_AXIS (1 << 0)
#define GP_REPORT_TYPE_BTN  (1 << 1)
typedef struct kinc_gamepad_report_t {
    uint8_t flags;
    uint8_t number;
    int8_t value;
} kinc_gamepad_report_t;

/*
void gamepad_device_tracer_glider(gamepad_registration_t* gamepad_registration, void* _report){
    kinc_gamepad_report_t* report = (kinc_gamepad_report_t*)_report;
    gamepad_state_t* gamepad_state = &mm_gamepad_state[gamepad_registration->gamepad_idx];

    if (bit_is_set_some(report->flags,GP_REPORT_TYPE_AXIS)){
        uint8_t axis = report->number;
        if (axis == 0){
            // left<->right
            if (report->value==-1){
                gamepad_state->controls |= GP_D_LEFT;
            }
            else if (report->value==1){
                gamepad_state->controls |= GP_D_RIGHT;
            }
            else {
                gamepad_state->controls &= ~(GP_D_RIGHT | GP_D_LEFT);
            }
        }
        else if (axis == 1){
            // up<->down
            // left<->right
            if (report->value==-1){
                gamepad_state->controls |= GP_D_DOWN;
            }
            else if (report->value==1){
                gamepad_state->controls |= GP_D_UP;
            }
            else {
                gamepad_state->controls &= ~(GP_D_UP | GP_D_DOWN);
            }
        }
    }
    else if (bit_is_set_some(report->flags,GP_REPORT_TYPE_BTN)){
        if (report->value==1){
            switch (report->number){
                case 0: gamepad_state->buttons |= GP_BTN_TOP; break;
                case 1: gamepad_state->buttons |= GP_BTN_RIGHT; break;
                case 2: gamepad_state->buttons |= GP_BTN_BOTTOM; break;
                case 3: gamepad_state->buttons |= GP_BTN_LEFT; break;
                case 4: gamepad_state->buttons |= GP_BTN_REAR_LEFT; break;
                case 5: gamepad_state->buttons |= GP_BTN_REAR_RIGHT; break;
                case 8: gamepad_state->buttons |= GP_BTN_SELECT; break;
                case 9: gamepad_state->buttons |= GP_BTN_START; break;
            }
        } else {
            switch (report->number){
                case 0: gamepad_state->buttons &= ~GP_BTN_TOP; break;
                case 1: gamepad_state->buttons &= ~GP_BTN_RIGHT; break;
                case 2: gamepad_state->buttons &= ~GP_BTN_BOTTOM; break;
                case 3: gamepad_state->buttons &= ~GP_BTN_LEFT; break;
                case 4: gamepad_state->buttons &= ~GP_BTN_REAR_LEFT; break;
                case 5: gamepad_state->buttons &= ~GP_BTN_REAR_RIGHT; break;
                case 8: gamepad_state->buttons &= ~GP_BTN_SELECT; break;
                case 9: gamepad_state->buttons &= ~GP_BTN_START; break;
            }
        }
    }
}*/




void gamepad_device_handler(gamepad_mapping_t* mapping, int gamepad_idx, void* _report) {
    kinc_gamepad_report_t* report = (kinc_gamepad_report_t*)_report;
    gamepad_state_t* gamepad_state = &mm_gamepad_down[gamepad_idx];

    if (bit_is_set_some(report->flags, GP_REPORT_TYPE_AXIS)) {
        uint8_t axis = report->number;

        if (axis == mapping->axis_lr) {
            // Handle left-right axis
            if (report->value * mapping->lr_invert == -1) {
                gamepad_state->controls |= GP_D_LEFT;
                mm_gamepad_pressed->controls |= GP_D_LEFT;
            } else if (report->value * mapping->lr_invert == 1) {
                gamepad_state->controls |= GP_D_RIGHT;
                mm_gamepad_pressed->controls |= GP_D_RIGHT;
            } else {
                mm_gamepad_released->controls |= (GP_D_LEFT | GP_D_RIGHT) & (gamepad_state->controls);
                gamepad_state->controls &= ~(GP_D_LEFT | GP_D_RIGHT);
            }
        } else if (axis == mapping->axis_ud) {
            // Handle up-down axis
            if (report->value * mapping->ud_invert == -1) {
                gamepad_state->controls |= GP_D_UP;
                mm_gamepad_pressed->controls |= GP_D_UP;
            } else if (report->value * mapping->ud_invert == 1) {
                gamepad_state->controls |= GP_D_DOWN;
                mm_gamepad_pressed->controls |= GP_D_DOWN;
            } else {
                mm_gamepad_released->controls |= (GP_D_UP | GP_D_DOWN) & (gamepad_state->controls);
                gamepad_state->controls &= ~(GP_D_UP | GP_D_DOWN);
            }
        }
    } else if (bit_is_set_some(report->flags, GP_REPORT_TYPE_BTN)) {
        uint8_t button = report->number;

        if (button < sizeof(mapping->button_map)) {
            uint32_t mapped_button = mapping->button_map[button];

            if (report->value == 1) {
                // Button pressed
                gamepad_state->buttons |= mapped_button;
                mm_gamepad_pressed->buttons |= mapped_button;
            } else {
                // Button released
                gamepad_state->buttons &= ~mapped_button;
                mm_gamepad_released->buttons |= mapped_button;
            }
        }
    }
}




static gamepad_mapping_t* gamepad_find_mapping(int gamepad){
    const char* vendor = kinc_gamepad_vendor(gamepad);
    const char* product = kinc_gamepad_product_name(gamepad);    
    uint32_t gamepad_identifier = hash_function(product);
    for (int i=0;i<(sizeof(gamepad_mappings) / sizeof(gamepad_mappings[0]));i++){
        gamepad_mapping_t* mapping = &gamepad_mappings[i];
        if (mapping->identifier == gamepad_identifier){
            return mapping;
        }
    }
    // COULDN't FIND GAMEPAD
    return NULL;
}

static void callback_gamepad_connected(int gamepad,void* userdata){
    const char* vendor = kinc_gamepad_vendor(gamepad);
    const char* product = kinc_gamepad_product_name(gamepad);
    uint32_t hash = hash_function(product);
    printf("Connected: %s | %s => %u\n",vendor,product,hash);
    gamepad_mapping_t* mapping = gamepad_find_mapping(gamepad);
    if (mapping == NULL ){
        return;
    }
    gamepad2mapping[gamepad] = mapping;
    *mm_gamepad_info |= (1 << gamepad);
}

static void callback_gamepad_disconnected(int gamepad,void* userdata){
    gamepad2mapping[gamepad]=NULL;
    *mm_gamepad_info &= ~(1 << gamepad);
}

gamepad_state_t gamepad_state_current;

static void callback_gamepad_axiscallback(int gamepad,int axis, float value, void* userdata){
    printf("gamepad:%d axis:%d value:%f\n",gamepad,axis,value);
    gamepad_mapping_t* mapping = gamepad2mapping[gamepad];
    if (mapping==NULL){
        return;
    }
    kinc_gamepad_report_t report = {
        .flags = GP_REPORT_TYPE_AXIS,
        .number = axis,
        .value = (int8_t)value
    };
    gamepad_device_handler(mapping,gamepad,(void*)&report);
}

static void callback_gamepad_buttoncallback(int gamepad, int button, float value, void* userdata){
    printf("gamepad:%d btn:%d value:%f\n",gamepad,button,value);
    gamepad_mapping_t* mapping = gamepad2mapping[gamepad];
    if (mapping==NULL){
        return;
    }    
    kinc_gamepad_report_t report = {
        .flags = GP_REPORT_TYPE_BTN,
        .number = button,
        .value = (int8_t)value
    };
    gamepad_device_handler(mapping,gamepad,(void*)&report);  
}

void io_backend_init()
{
  kinc_keyboard_set_key_down_callback(&keyboard_pressed, NULL);
  kinc_keyboard_set_key_up_callback(&keyboard_released, NULL);

// void kinc_mouse_set_press_callback(void (*value)(int /*window*/, int /*button*/, int /*x*/, int /*y*/, void * /*data*/), void *data) {
// 	mouse_press_callback = value;
// 	mouse_press_callback_data = data;
// }

// void kinc_mouse_set_release_callback(void (*value)(int /*window*/, int /*button*/, int /*x*/, int /*y*/, void * /*data*/), void *data) {
// 	mouse_release_callback = value;
// 	mouse_release_callback_data = data;
// }

// void kinc_mouse_set_move_callback(void (*value)(int /*window*/, int /*x*/, int /*y*/, int /*movement_x*/, int /*movement_y*/, void * /*data*/), void *data) {

    kinc_mouse_set_move_callback(&callback_mouse_moved,NULL);
    kinc_mouse_set_press_callback(&callback_mouse_button_pressed, NULL);
    kinc_mouse_set_release_callback(&callback_mouse_button_released, NULL);
    kinc_mouse_set_scroll_callback(&callback_mouse_wheel_updated, NULL);

    kinc_gamepad_set_connect_callback(callback_gamepad_connected, NULL);
    kinc_gamepad_set_disconnect_callback(callback_gamepad_disconnected,NULL);
    kinc_gamepad_set_axis_callback(callback_gamepad_axiscallback, NULL);
    kinc_gamepad_set_button_callback(callback_gamepad_buttoncallback, NULL);
}


void io_backend_before_tick(void){
}

void io_backend_after_tick()
{
  kb_pressed=(hid_keyboard_report_t){0};
  kb_released=(hid_keyboard_report_t){0};
  *mm_mouse_wheel = 0;
}

// look up new key in previous keys
bool find_key_in_report(hid_keyboard_report_t const *report, uint8_t keycode)
{
  for(uint8_t i=0; i<report->keycode_amount; i++)
  {
    if (report->keycode[i] == keycode)  {
      return true;
    }
  }

  return false;
}

bool io_keyboard_is_pressed(uint8_t keycode){
  bool result = find_key_in_report(&kb_pressed,keycode);
  return result;
}

bool io_keyboard_is_down(uint8_t keycode){
  return find_key_in_report(&kb_down,keycode);
}

bool io_keyboard_is_released(uint8_t keycode){
  return find_key_in_report(&kb_released,keycode);
}

#endif