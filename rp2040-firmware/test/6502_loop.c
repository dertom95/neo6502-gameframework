int main(){
 volatile   unsigned char *ptr;
    unsigned int i;

    // Assign the memory address 0x2000 to the pointer
    ptr = (unsigned char *)0x2000;

    // Loop from i=0 to i=255
    for (i = 0; i <= 255; i++) {
        // Write value of i at the memory address pointed by ptr
        *ptr = (unsigned char)i;
        
        // Print the value written
    }
}
