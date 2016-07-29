#!/usr/bin/python

import time

# NON ROS HARDWARE INTERFACE

"""In this class possible colors of the LEDs Bender's eyes representing their emotions are defined.
This is done by "set_eye_colors (eye, rgb_colors)" method of the "HeadHWController" class. For more details see Documentation.
Seven standard "emotion methods" for eyes's colors have been given.
Modify to create new emotions based on LEDs colors."""

# Use HW controller
from head_hw_controller import HeadHWController

class EyeEmotion(object):
	def __init__(self, hw_controller):
		self.hw_controller = hw_controller

	def surprised(self):
		black = [0,0,0]
		red = [3,0,0]
		green = [0,3,0]
		blue = [0,0,3]
		rgb_colors = [blue,blue,blue,red,red,red,black,black,black,blue,red,blue,green,green,green,green]
		self.hw_controller.set_eye_colors("left", rgb_colors)
		self.hw_controller.set_eye_colors("right", rgb_colors)

	def angry(self):
		black = [0,0,0]
		red = [3,0,0]
		green = [0,3,0]
		blue = [0,0,3]
		yellow = [3,3,0]
		rgb_colors = [red,red,red,red,yellow,yellow,black,black,black,red,black,black,green,green,green,green]
		self.hw_controller.set_eye_colors("left", rgb_colors)
		self.hw_controller.set_eye_colors("right", rgb_colors)
		
	def sad(self):
		black = [0,0,0]
		blue = [0,0,3]
		rgb_colors = [black,black,black,blue,black,blue,black,blue,black,blue,black,blue,black,black,black,black]
		self.hw_controller.set_eye_colors("left", rgb_colors)
		self.hw_controller.set_eye_colors("right", rgb_colors)

	def happy(self):
		black = [0,0,0]
		red = [3,0,0]
		green = [0,3,0]
		blue = [0,0,3]
		yellow = [3,3,0]
		rgb_colors = [black,black,black,black,yellow,yellow,black,black,black,black,yellow,yellow,yellow,yellow,black,black]
		self.hw_controller.set_eye_colors("left", rgb_colors)
		self.hw_controller.set_eye_colors("right", rgb_colors)

	def reset(self):
		black = [0,0,0]
		rgb_colors = [black,black,black,black,black,black,black,black,black,black,black,black,black,black,black,black]
		self.hw_controller.set_eye_colors("left", rgb_colors)
		self.hw_controller.set_eye_colors("right", rgb_colors)

	def color_palette1(self):
		led = 0
		rgb_colors_l = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
		rgb_colors_r = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
		for g in range(4):
			for b in range(4):
				rgb_colors_l[led] = [0,g,b]
				led+= 1
		led=0
		for g in range(4):
			for b in range(4):
				rgb_colors_r[led] = [1,g,b]
				led+= 1
		self.hw_controller.set_eye_colors("left", rgb_colors_l)
		self.hw_controller.set_eye_colors("right", rgb_colors_r)

	def color_palette2(self):
		led = 0
		rgb_colors_l = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
		rgb_colors_r = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
		for g in range(4):
			for b in range(4):
				rgb_colors_l[led] = [2,g,b]
				led+= 1
		led=0
		for g in range(4):
			for b in range(4):
				rgb_colors_r[led] = [3,g,b]
				led+= 1
		self.hw_controller.set_eye_colors("left", rgb_colors_l)
		self.hw_controller.set_eye_colors("right", rgb_colors_r)

if __name__ == '__main__':
	DEV_ID = 1
	dxl = DynamixelIO('/dev/ttyUSB0', baudrate = 115200)
	hw_controller = HeadHWController(dxl, dev_id = DEV_ID)
	eyes = EyeEmotion(hw_controller)
	while True:
		eyes.surprised()
		time.sleep(2)
		eyes.angry()
		time.sleep(2)
		eyes.happy()
		time.sleep(2)
		eyes.color_palette1()
		time.sleep(2)
		eyes.color_palette2()
		time.sleep(2)
		eyes.reset()
		time.sleep(2)
