#!/usr/bin/env python

from PIL import Image

tag = "test" 		# start or loop
motor_dir = 1       # forward direction depends on wiring of the motor and can either by 1 or 0

#image = Image.open("Source/nyan_scaled_indexed_" + tag + ".png")
image = Image.open("Source/test_image.png")

if not image.width == 384:
	print "Wrong image width, must be 384 px!"
	exit()

pixels = image.load()

# write output header
outfile = open("image_" + tag + ".h", "w+")
outfile.write("// Automatically generated header file for NyanPrinter. This file contains the image data for: " + tag + "\n\n")
outfile.write("#ifndef IMAGE_" + tag.upper() + "_H\n")
outfile.write("#define IMAGE_" + tag.upper() + "_H\n\n")
outfile.write("#include \"stm32f10x.h\"\n")
outfile.write("const uint8_t image_" + tag + "[] = {\n")

# every byte also contains some printer control data (motor direction (bit 6) and latch status (bit 7))
# motor direction is fixed for now (needs to change for complicated image data later on
# latch is not controlled via DMA right now but manually by setting a GPIO
base_byte = (motor_dir<<6)
current_pixel = 1

# go through each line
# bits 0 - 5 actually contain image data
# every line needs 64 bytes
# bit 0 of these 64 bytes represents the first 64 pixels in this line
# bit 1 stands for the second 64 pixels and so on
for l in range(0, image.height):

	#print "Analyzing line " + str(l)

	# test line for more than 64 black pixels
	black_pixels = 0
	for x in range(0, 384):
		if pixels[x, l]:
			black_pixels = black_pixels + 1
	if black_pixels > 64:
		print "More than 64 pixels in this line are black."
        
	
	for byte_number in range(0, 64):
	
		new_byte = base_byte        # copy what's always the same

		for bit_number in range(0, 6):
			if pixels[(bit_number*64 + byte_number), l]:
				new_byte |= (1<<bit_number)

		outfile.write(bin(new_byte) + ", ")
    
	outfile.write("\n")

print "Done"
outfile.write("};\n")
outfile.write("uint16_t image_" + tag + "_length = " + str(image.height * 64) + "; \t// " + str(image.height) + " lines with 64 elements per line\n\n")
outfile.write("#endif //define TEST_PICTURE_H")
outfile.close()

		

