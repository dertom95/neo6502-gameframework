.segment "CODE"

    .org $1000 ; Start address

Loop:
    LDA #$95 ; Load the value 95 into the accumulator
    STA $21F6 ; Store the accumulator value at address $2000

    JMP Loop ; Jump back to the start of the loop