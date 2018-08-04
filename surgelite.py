#Unihack18. Surg.E.Lite
#OpenCV + NeoPixel set up to make low cost, adaptive light
#for surgical purposes

'''
Pseudocode:
Image processing to filter out unwanted noise
Detect purple surgical hat (for prototype)
Track purple object
Find area that is not covered by object
Objective-> Move to useful location not blocked by surgical staff 
'''

#Import required libraries

import smbus
import time
import cv2
import numpy as np
from picamera.array import PiRGBArray
from picamera import PiCamera
from gpiozero import Motor 

bus = smbus.SMBus(1)
address = 0x04 #for 12C with Arduino

ifPurple = False

ImageWidth = 320
ImageHeight = 240

camera = PiCamera()
camera.resolution = (ImageWidth, ImageHeight)
camera.framerate = 20
frameCapture = PiRGBArray(camera,size=(ImageWidth,ImageHeight))

def writeNumber(value):
    bus.write_byte(address,value)
    return -1

def readNumber():
    number = bus.read_byte(address)
    return number

for frame in camera.capture_continuous(frameCapture, format = "bgr", use_video_port = True):

    image = frame.array

    #start image processing to track object (purple mesh hat)    
    image = cv2.flip(image,0)
    image = cv2.flip(image,1)

    image = cv2.medianBlur(image,5)
    
    #Convert to HSV
    image = cv2.cvtColor(image,cv2.COLOR_BGR2HSV)    
        
    lower_red = np.array([160,100,100])
    high_red = np.array([180,255,255])

    #Threholding for red color
    mask = cv2.inRange(image,lower_red, high_red)

    result = cv2.bitwise_and(image,image,mask=mask)

    #Bounding box and Error
    img, contours, hierarchy = cv2.findContours(mask,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)

    #Constants
    frame_x = ImageWidth/2
    frame_y = ImageHeight/2
    min_area = 230
    
    for c in contours: #loop over contours array
    
        Box_Area = cv2.contourArea(c)
        if Box_Area < min_area:
            continue

        #if largest contour, do processing   
        print("Largest box drawing...")
        x,y,w,h = cv2.boundingRect(c)
        img = cv2.rectangle(img,(x,y),(x+w,y+h),(255,255,0),2)                        
        cv2.imshow("test",img)
        
        #Centre of bounding box of largest contour
        box_x = x + (w/2)
        box_y = y + (h/2)

        Error_x = frame_x - box_x
        Error_y = frame_y - box_y
        #Debugging
        print("Error_x: ", Error_x)
        print("Error_y: ", Error_y)

        #From ROI, creates variable to send (create var, 1-6)
        #Command arduino to operate Neopixel through i2c

        #Work out position of box in frame

        '''
        writeNumber(var) #var = 1,2,3,4,5,6 (regions of light)
        print("ROI detected: ",var)
        time.sleep(0.5)

        number = readNumber()
        print("I received: ", number)
        '''
    
    
    #To end streaming
    key = cv2.waitKey(5) & 0xFF
    frameCapture.truncate(0)
    if key == ord("q"):
        break


    
