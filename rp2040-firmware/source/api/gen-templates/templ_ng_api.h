/*block:gluecode|file:ng_api.h|overwrite*/

#ifndef __NG_API_H_
#define __NG_API_H_

#include "../ng_config.h"

typedef struct call_header_t {
    uint8_t func_type;
    uint8_t func_id;
} call_header_t;


#define MOUSE_BUTTON_LEFT      (1 << 0) /* Left button */
#define MOUSE_BUTTON_RIGHT     (1 << 1) /* Right button */
#define MOUSE_BUTTON_MIDDLE    (1 << 2) /* Middle button */
#define MOUSE_BUTTON_BACKWARD  (1 << 3) /* Backward button, */
#define MOUSE_BUTTON_FORWARD   (1 << 4) /* Forward button */

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
