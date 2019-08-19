#!/usr/bin/env python3

from PIL import Image

tag = input("Please enter filename tag (e.g. \"start\" or \"loop\") > ")								# will be added to filename
motor_dir = int(input("Please specify motor direction for forward movement (either 1 or 0) > "))		# forward direction depends on wiring of the motor
image_filename = input("Please specify filename of desired image > ")
image = Image.open(image_filename)

if not image.width == 384:
	print("Wrong image width, must be 384 px.")
	exit()

pixels = image.load()
bytes = 0

# start source file
sourcefile = open("image_" + tag + ".c", "w+")
sourcefile.write("// Automatically generated source file for NyanPrinter.\n// This file contains image data from: " + image_filename + "\n\n")
sourcefile.write("#include \"stm32f10x.h\"\n\n")
sourcefile.write("const uint8_t image_" + tag + "[] = {\n")

# every byte also contains some printer control data: motor direction (bit 6) and latch status (bit 7)
# latch is not controlled via DMA right now, so it does not appear in the image data
# motor direction will be initialized to forward, but will be changed when coloring one line in multiple steps
base_byte = motor_dir << 6
base_byte_bw = (not motor_dir) << 6			# base byte for backwards motor movement

# bits 0 - 5 will contain the actual image data
# because every line is 384 pixels wide, there are 64 bytes per line
# all bit 0 of these 64 bytes represent the first 64 pixels in this line
# bit 1 represents the second 64 pixels and so on
# pixel to bit assignment is weird due to the shifting of 6 parallel image lines (refer to printer datasheet)

# we will need this in every line, it is used to convert the integer list to binary string array
to_binary_string = lambda a: str(bin(a))
s = "\t" + "%s, " * 64 + "\n"

# go through each line
for l in range(0, image.height):

	# print("Analyzing line " + str(l))
	# we can color 64 pixels at once, so once we counted 64 black pixels we need to end the data block and return to the same position in the next pass
	black_pixels = 0					# number of black pixels colored so far
	x = 0							# current pixel in that line
	current_bytes = [base_byte] * 64	# buffer for current data block

	# go through all pixels for this line
	while x < 384:
		
		# check current pixel for being black and if so, increase counter and set specific bit in byte array
		if pixels[x, l]:
			black_pixels = black_pixels + 1
			current_bytes[x % 64] |= 1 << int(x / 64)

		# check if we have to terminate this data block
		if black_pixels == 64:
			# print("Starting new data block for line " + str(l))
			sourcefile.write(s % tuple(map(to_binary_string, current_bytes)))		# write current bytes as new data block to source file
			bytes = bytes + 64
			current_bytes = [base_byte] * 60 + [base_byte_bw] * 4					# re-initialize for subsequent data blocks with inverted motor direction in the end of transmission (4 bytes for now, technically 1 should suffice)
			black_pixels = 0
		
		x = x + 1
	
	# print all left over pixels to source file
	if black_pixels > 0:
		sourcefile.write(s % tuple(map(to_binary_string, current_bytes)))			# write current bytes as new data block to source file
		bytes = bytes + 64


sourcefile.write("};\n\n")
sourcefile.write("uint16_t image_" + tag + "_length = " + str(bytes) + ";")
sourcefile.close()

# write header file
headerfile = open("image_" + tag + ".h", "w+")
headerfile.write("// Automatically generated header file for NyanPrinter.\n// This file contains image data from: " + image_filename + "\n\n")
headerfile.write("#ifndef IMAGE_" + tag.upper() + "_H\n")
headerfile.write("#define IMAGE_" + tag.upper() + "_H\n\n")
headerfile.write("#include \"stm32f10x.h\"\n")
headerfile.write("extern const uint8_t image_" + tag + "[" + str(bytes) + "];\n")
headerfile.write("extern uint16_t image_" + tag + "_length;\n\n")
headerfile.write("#endif //define IMAGE_" + tag.upper() + "_H")
headerfile.close()

print("Done")
