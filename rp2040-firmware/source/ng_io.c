#include "ng_io.h"

#ifdef PICO_NEO6502
# include "core/backend/neo6502/neo6502.h"
#endif
// see backends

void io_init(void)
{
  neo6502_usb_init();
}

void io_update(void)
{
  neo6502_usb_update();
}