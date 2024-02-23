/*block:gluecode|file:ng_api.h|overwrite*/

#ifndef __NG_API_H_
#define __NG_API_H_

#include "../ng_config.h"
#include "../../ng_io.h"
#include "../../ng_gfx.h"

#define MM_SB           MEMORY_MAP_START    // memory-location where the memory-mapping starts
#define MM_KEYSET       (MM_SB + 0x01)      // uint8
#define MM_MOUSE_X      (MM_SB + 0x02)      // uint16 
#define MM_MOUSE_Y      (MM_SB + 0x04)      // uint16
#define MM_MOUSE_BTN    (MM_SB + 0x06)      // uint8
#define MM_MOUSE_WHEEL  (MM_SB + 0x07)      // int8
#define MM_FUNC_CALL    (MM_SB + 0x08)      // uint8 

#define MOUSE_BUTTON_LEFT      (1 << 0) ///< Left button
#define MOUSE_BUTTON_RIGHT     (1 << 1) ///< Right button
#define MOUSE_BUTTON_MIDDLE    (1 << 2) ///< Middle button
#define MOUSE_BUTTON_BACKWARD  (1 << 3) ///< Backward button,
#define MOUSE_BUTTON_FORWARD   (1 << 4) ///< Forward button,

#define PALETTE_SIZE 255
#define COL_TRANSPARENT 0
#define COL_BLACK    1
#define COL_BLUE_DARK   2
#define COL_VIOLETTE 3
#define COL_GREEN_DARK    4
#define COL_ORANGE_DARK   5
#define COL_BROWNGREY 6
#define COL_LIGHTGREY 7
#define COL_WHITE     8
#define COL_RED       9  
#define COL_ORANGE   10
#define COL_YELLOW   11
#define COL_GREEN    12
#define COL_BLUE_LIGHT 13
#define COL_GREY 14
#define COL_PINK 15
#define COL_PEACH 16

typedef struct call_header_t {
    uint8_t func_type;
    uint8_t func_id;
} call_header_t;


/*block:function-group*/

// function grp: /*name:id*/0/*endname*/

/*block:function*/ // returns: /*name:return_type*/void/*endname*/ f-grp:/*name:function_group*/1/*endname*/ f-id:/*name:function_id*/2/*endname*/;
/*name:return_type*/void/*endname*/ /*name:signature*/ do_somedata(uint8_t some_byte)/*endname*/;
/*endblock:function*/

/*block:function*/// returns: /*name:return_type*/void/*endname*/ f-grp:/*name:function_group*/1/*endname*/ f-id:/*name:function_id*/2/*endname*/
typedef struct /*name:name|pre:call_|post:_t*/somedata_t/*endname*/ {
    call_header_t hdr;
/*block:parameter*/    /*name:type|if:@type.endswith("*"),uint16_t,@type*/uint8_t/*endname*/ /*name:name*/some_byte/*endname*/;
/*endblock:parameter*/} /*name:name|pre:call_|post:_t*/somedata_t/*endname*/;
/*endblock:function*/

/*endblock:function-group*/

/*endblock:gluecode*/

#endif
