/*block:gluecode|file:ng_api.c|overwrite*/

#include /*name:rip|auto|echo:"ng_api.h"*/"templ_ng_api.h"/*endname*/

const volatile uint8_t* mem_call_function = (uint8_t*)MM_FUNC_CALL;
const volatile uint8_t* mem_call_result   = (uint8_t*)MEMORY_MAP_CALLRETURN_BUFFER_BEGIN;

/*block:function-group*/

/*block:function*/// returns: /*name:return_type*/void/*endname*/ f-grp:/*name:function_group*/1/*endname*/ f-id:/*name:function_id*/2/*endname*/
/*name:return_type*/void/*endname*/ /*name:signature*/gfx_set_palette_from_assset_t(uint8_t some_byte)/*endname*/{
    uint8_t result_code;
    /*name:name|pre:call_|post:_t*/somedata_t/*endname*/* func_data;
    func_data = (/*name:name|pre:call_|post:_t*/somedata_t/*endname*/*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = /*name:function_group|auto|echo:%s,@function-group.id*/1/*endname*/;
    func_data->hdr.func_id = /*name:function_id*/1/*endname*/;
/*block:parameter*/    func_data->/*name:name*/some_byte/*endname*/=/*name:type|if:@type.endswith("*"),(uint16_t),*//*endname*/ /*name:name*/some_byte/*endname*/;
/*endblock:parameter*/    
    result_code = *mem_call_function;
    // TODO: resultcode some error checking?!

    {
        /*name:if|auto|if:@return_type!='void',@return_type call_result = (@return_type)*mem_call_result;,*//*endname*/
        /*name:if|auto|if:@return_type!='void',return call_result;,*//*endname*/
    }
}   
/*endblock:function*/

/*endblock:function-group*/

/*endblock:gluecode*/
