arm-none-eabi-objcopy -I ihex --output-target=binary Debug/NyanPrinter.hex Debug/NyanPrinter.bin
st-flash write Debug/NyanPrinter.bin 0x8000000
