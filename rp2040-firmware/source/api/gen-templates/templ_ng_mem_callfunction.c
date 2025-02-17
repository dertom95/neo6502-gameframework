/*block:rip*/
#include<stdint.h>
#include"../../api/ng_config.h"
#include"templ_ng_api.h"
/*endblock:rip*/


#include "ng_api.h"
#include "../../core/memory.h"

/*block:gluecode|file:memory_call_function.c|overwrite*/

uint8_t* call_buffer_return = &mem[MEMORY_MAP_CALLRETURN_BUFFER_BEGIN]; 

uint8_t call_function()
{
    call_header_t* header = (call_header_t*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
/*block:function-group*/    
    if (header->func_type==/*name:id*/1/*endname*/){
        switch (header->func_id) {
/*block:function*/            
            case /*name:function_id*/0x01/*endname*/: {
                /*name:name|pre:call_|post:_t*/somedata_t/*endname*/* call = (/*name:name|pre:call_|post:_t*/somedata_t/*endname*/*)&mem[MEMORY_MAP_CALL_BUFFER_BEGIN];
                /*name:if|auto|if:@return_type!='void',@return_type call_result =,*//*endname*/  /*name:name*/do_somedata/*endname*/(/*block:parameter*/ /*name:type|if:@type.endswith("*"),(@type)(&mem[,*//*endname*/ /*name:if|auto|if:@type=='uint16_t',swap16(,*//*endname*/call->/*name:name*/some_byte/*endname*//*name:if|auto|if:@type=='uint16_t',),*//*endname*/ /*name:type|if:@type.endswith("*"),]),*//*endname*/ /*name:some|auto|post_n_blast:comma*//*endname*/  /*endblock:parameter*/);
                /*name:if|auto|if:@return_type=='uint16_t',*((uint16_t*)call_buffer_return)=swap16(call_result);,*//*endname*/
                /*name:if|auto|if:@return_type=='uint8_t',*call_buffer_return=call_result;,*//*endname*/
                /*name:if|auto|if:@return_type=='bool',*call_buffer_return=(uint8_t)call_result;,*//*endname*/
                /*name:if|auto|if:'*' in @return_type,*(uint16_t*)call_buffer_return = (uint16_t)(intptr_t)call_result-(intptr_t)mem;, */ /*endname*/
                return FUNCTION_RETURN_OK;
            }
/*endblock:function*/            
        }
    }
/*endblock:function-group*/    
    return FUNCTION_RETURN_ERROR;
}

/*endblock:gluecode*/