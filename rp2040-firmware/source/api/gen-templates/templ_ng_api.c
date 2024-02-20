/*block:gluecode|file:ng_api.c|overwrite*/

#include /*name:rip|auto|echo:"ng_api.h"*/"templ_ng_api.h"/*endname*/

const volatile uint8_t* call_function = (uint8_t*)MM_FUNC_CALL;

/*block:function*/// returns: /*name:return_type*/void/*endname*/ f-grp:/*name:function_group*/1/*endname*/ f-id:/*name:function_id*/2/*endname*/
/*name:signature*/void gfx_set_palette_from_assset_t(uint8_t some_byte)/*endname*/{
    uint8_t result_code;
    /*name:name|pre:call_|post:_t*/somedata_t/*endname*/* func_data;
    func_data = (/*name:name|pre:call_|post:_t*/somedata_t/*endname*/*)(MEMORY_MAP_CALL_BUFFER_BEGIN);
    func_data->hdr.func_type = /*name:function_group*/1/*endname*/;
    func_data->hdr.func_id = /*name:function_id*/1/*endname*/;
/*block:parameter*/    func_data->/*name:name*/some_byte/*endname*/=/*name:name*/some_byte/*endname*/;
/*endblock:parameter*/    
    result_code = *call_function;
    /*name:if|auto|if:@return_type!='void',return *((@return_type*)(MEMORY_MAP_CALL_BUFFER_BEGIN));,*//*endname*/
}   
/*endblock:function*/

/*endblock:gluecode*/
