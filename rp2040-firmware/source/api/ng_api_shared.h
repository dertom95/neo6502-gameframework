#ifndef __NG_API_MAPPING_H_
#define __NG_API_MAPPING_H_

typedef struct keyboard_mapping_t {
  uint8_t key_state;
  uint8_t keycodes[8];
  uint8_t flags;
} keyboard_mapping_t;

#endif