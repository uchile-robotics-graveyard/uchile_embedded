#!/usr/bin/python

__author__ = 'gdiaz'

# ROS INTERFACE

"""Provides a high level interface over ROS to generate emotions on Bender Robot.
It use methods provided by FacialExpressions class (non ROS hardware interface). See Documentation.
"""
import sys
import math
import rospy
sys.path.append('/home/hernan/fuerte_workspace/bender/bender_head_arduino/src')
from threading import Thread

from std_msgs.msg import Empty
from bender_head_arduino.msg import ExpressionCommand
#from bender_head_arduino.msg import Emotion

# Use HW interface
from head_hw_controller import HeadHWController
from servos_hw import ServosHW
from eye_emotions import EyeEmotion
from facial_gestures import FacialGestures
from facial_expressions import FacialExpressions
#from dynamixel_driver.dynamixel_io import DynamixelIO
from dynamixel_io import DynamixelIO

DEV_ID = 16

class ROSFacialExpressions:
    def __init__(self, dxl_io, controller_namespace, port_namespace):
        # Only argument stuff
        self.running = False
        self.dxl_io = dxl_io
        self.controller_namespace = controller_namespace
        self.port_namespace = port_namespace
        self.hw_controller = HeadHWController(dxl_io, dev_id = DEV_ID)
        self.servos_hw = ServosHW(self.hw_controller)
        self.eyes = EyeEmotion(self.hw_controller)
        self.facial_gestures = FacialGestures(self.servos_hw)
        self.facial_expressions = FacialExpressions(self.eyes, self.facial_gestures)
        self.expression_state = ExpressionCommand()
        #self.joystick_msg = ExpressionCommand()
        
    def initialize(self):
        # Get params and allocate msgs
        self.state_update_rate = rospy.get_param(self.controller_namespace + '/rate', 2)
        self.expression_state.expression = "defaulf"

    def start(self):
        # Create subs, services, publishers, threads
        self.running = True
		#subscribers
        self.command_sub = rospy.Subscriber(self.controller_namespace + '/expression_command', ExpressionCommand, self.process_command)
        self.expressionsList_sub = rospy.Subscriber(self.controller_namespace + '/expression_list', Empty, self.list_expressions)
        #self.joystick_sub = rospy.Subscriber(self.controller_namespace + '/joystick_face_command', Emotion, self.joystick_cmd)
       #publishers
        self.state_pub = rospy.Publisher(self.controller_namespace + '/expression_state', ExpressionCommand)
        Thread(target=self.update_state).start()

    def stop(self):
        self.running = False
        self.command_sub.unregister()
        self.expressionsList_sub.unregister()
        #self.joystick_sub.unregister()
        self.state_pub.unregister()

    def process_command(self, msg):
        if (msg.expression == "surprised"):
            self.facial_expressions.surprised()
        elif (msg.expression == "angry"):
            self.facial_expressions.angry()
        elif (msg.expression == "happy"):
            self.facial_expressions.happy()
        elif (msg.expression == "sad"):
            self.facial_expressions.sad()
        elif (msg.expression == "veryHappy"):
            self.facial_expressions.veryHappy()
        elif (msg.expression == "default"):
            self.facial_expressions.default()
        elif (msg.expression == "apagado"):
            self.facial_expressions.apagado()
        elif (msg.expression == "id"):
            device_id = self.hw_controller.get_state(3)
            print("device_id = "+str(device_id))

    def joystick_cmd(self, msg):
        if (msg.Order == "changeFace"):
            self.joystick_msg.expression = msg.Action
            self.command_sub.publish(self.joystick_msg)

    def list_expressions(self, msg):
        print("------------------------------------------------------------")
        print("Expressions Availables:\n")
        print("surprised\nangry\nhappy\nsad\nveryHappy\ndefault\napagado\n")
        print("------------------------------------------------------------")

    def update_state(self):
        rate = rospy.Rate(self.state_update_rate)
        while self.running and not rospy.is_shutdown():
            current_expression = self.facial_expressions.get_state()

            #update the data state
            self.expression_state.expression = current_expression

            #publish the state
            self.state_pub.publish(self.expression_state)
            rate.sleep()

if __name__ == '__main__':
    rospy.init_node('expressions_controller')
    dxl = DynamixelIO('/dev/ttyUSB1', baudrate = 115200)
    expressions = ROSFacialExpressions(dxl, 'expressions', 'left')
    expressions.initialize()
    expressions.start()
    rospy.spin()
    expressions.stop()
