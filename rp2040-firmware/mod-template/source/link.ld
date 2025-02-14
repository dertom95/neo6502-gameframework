MEMORY {
    zp : ORIGIN = __rc31 + 1, LENGTH = 0x100 - (__rc31 + 1)
    user_ram (rw) : ORIGIN = 0x0200, LENGTH = 0xfe00
}

REGION_ALIAS("c_readonly", user_ram)
REGION_ALIAS("c_writeable", user_ram)

SECTIONS { INCLUDE c.ld }

__rc0 = 0x00;
INCLUDE imag-regs.ld
ASSERT(__rc0 == 0x00, "Inconsistent zero page map.")
ASSERT(__rc31 == 0x1f, "Inconsistent zero page map.")

__stack = 0;

OUTPUT_FORMAT { 
    SHORT(ORIGIN(user_ram))
    /*SHORT(__data_end - 0x0200)*/
    TRIM(user_ram)
}
