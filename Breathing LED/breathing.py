#!/bin/env python3

# Johannes Herold, 2019
#
# This tools calculates distinct PWM values to fade a breathing LED using a microcontroller
# The breathing equation used is:
#
# (exp(-cos(x))-1/e)*(255/(e-1/e))
#
# The equation is based on thi article by Sean Voisen: https://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
# Have a look at the graph: https://www.wolframalpha.com/input/?i=%28exp%28-cos%28x%29%29-1%2Fe%29*%28255%2F%28e-1%2Fe%29%29


from math import cos, e, pi, exp


# get values from user

period = float(input("Breathing period? (seconds, float possible)"))
fps = int(input("Frames per second? (integer)"))
total_frames = int(period * fps)
pwm_min = int(input("Minimal PWM value? (integer)"))
pwm_max = int(input("Maximal PWM value? (integer)"))
pwm_range = pwm_max - pwm_min
if pwm_range <= 0 or period <= 0 or total_frames == 0:
	print("Faulty values")
	exit()

output_list = []

for x in range (0, total_frames):
	output_list.append(int((exp(-cos(2*pi*(x/total_frames)))-1/e)*(255/(e-1/e))))

print(output_list)
