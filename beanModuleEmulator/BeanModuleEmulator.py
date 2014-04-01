#!/usr/bin/python

#GUI Parameters
terminal_color = '#10fb72'
serial_active_color = '#1f4dbc'
serial_inactive_color = '#ff0000'

from Tkinter import *
from time import sleep
import os
import BeanSerialTransport
import logging
import numpy

logging.basicConfig(stream=sys.stderr, level=logging.DEBUG)

transport = BeanSerialTransport.Bean_Serial_Transport()


def print_to_gui(type, data):
    # line converts all numbers to chars and then
    # makes a single string of them
    # python number and char type crazyness
    data = ''.join(chr(c) for c in data)
    terminal_output.insert(END, data)
    terminal_output.yview(END)

transport.add_handler(transport.MSG_ID_SERIAL_DATA, print_to_gui)

def quit_app():
    global transport
    transport.close_port()

    global app
    app.quit()


def check_serial():
    global count
    global serial_is_active_label
    global serial_is_active

    if(transport.serial_port != None and transport.serial_port.isOpen()):
        serial_is_active_label.configure(foreground=(serial_active_color)) 
        serial_is_active.set("On")
    else:
        serial_is_active_label.configure(foreground=(serial_inactive_color)) 
        serial_is_active.set("Off")

    transport.parser()
    app.after(50, check_serial)



min_x = 500
max_x = 600
min_y = 600
max_y = 900

app = Tk()
app.wm_protocol ("WM_DELETE_WINDOW", quit_app)
app.minsize(min_x, min_y)
app.maxsize(max_x, max_y)

app.title('Bean Arduino Test Tool')


beanpic = PhotoImage(file="cutout-bean.gif")
beanpic = beanpic.subsample(8)
Label(image=beanpic).grid(row=0,sticky= N+S+E+W)
Label(text='           Bean Arduino Test Jig             ').grid(row=1)


# Initialize main container/frames

content_frame = Frame(app)
content_frame.grid(row=3, sticky=W+E+N+S)



serial_chooser_frame = Frame(content_frame, padx=10)
serial_chooser_frame.grid(row=0)

# Serial Picker
Label(serial_chooser_frame, text='Serial Port:').grid(row=0, sticky=W)
serial_chooser = StringVar()

def choose_serial(port):
    transport.close_port()
    transport.open_port(port, 57600)

serial_chooser_menu = OptionMenu(serial_chooser_frame, serial_chooser,
    *(transport.get_available_serial_ports()),
    command=choose_serial)

serial_chooser_menu.config(width=60)
serial_chooser_menu.grid(row=1, sticky=W)

serial_is_active = StringVar()
serial_is_active.set("Off")
serial_is_active_label = Label(serial_chooser_frame, textvariable=serial_is_active)
serial_is_active_label.grid(row=1,column=1)
serial_is_active_label.configure(foreground=(serial_inactive_color)) 
Label(serial_chooser_frame,
      text='___________________________________________________________________________').grid(row=2)


led_frame = Frame(content_frame, padx=10)
led_frame.grid(sticky=W)
# LED Viewer
# draw a little circle
# update the color of the circle whenever we get a message to write the LED
Label(led_frame, text='LED Color').grid(sticky=W, padx=10  )
led_canvas = Canvas(led_frame, width=200, height=50)

led_canvas.grid(sticky=W)
led_shape = led_canvas.create_oval(20,10,50,40, outline="black", fill="black", width=2)
led_color_text = led_canvas.create_text(100, 25, text= '0xFF00FF')



def RGBToHTMLColor(rgb_tuple):
    return '#%02x%02x%02x' % rgb_tuple

def RGBToString(rgb_tuple):
    return '0x%02X%02X%02X' % rgb_tuple

def led_set_color(r, g, b):
    global led_shape
    global led_canvas

    color_tuple = (r, g, b)
    color = RGBToHTMLColor(color_tuple)
    led_canvas.itemconfigure(led_shape, fill=color)
    led_canvas.itemconfigure(led_color_text, text=RGBToString((r,g,b)))



led_red = 0
led_green = 0
led_blue = 0

def led_set_color_to_current():
    global led_red
    global led_green
    global led_blue
    led_set_color(led_red, led_green, led_blue)

def handle_led_write_all(type, data):
    global led_red
    global led_green
    global led_blue
    led_red = data[0]
    led_green = data[1]
    led_blue = data[2]
    led_set_color_to_current()

def handle_led_write_single(type, data):
    global led_red
    global led_green
    global led_blue

    if(data[0] == 0):
        led_red = data[1]
    elif(data[0] == 1):
        led_green = data[1]
    elif(data[0] == 2):
        led_blue = data[1]

    led_set_color_to_current()

transport.add_handler(transport.MSG_ID_CC_LED_WRITE, handle_led_write_single)
transport.add_handler(transport.MSG_ID_CC_LED_WRITE_ALL, handle_led_write_all)


# Accelerometer View/Control
accel_frame = Frame(content_frame, padx=10)
accel_frame.grid(sticky=W)

Label(accel_frame, text='Accelerometer').grid(sticky=W, padx=10  )

accel_x_control = Scale(accel_frame, from_=(-1200), to=1200, label='X')
accel_x_control.grid(row=1, sticky=W, padx=10)

accel_y_control = Scale(accel_frame, from_=(-1200), to=1200, label='Y')
accel_y_control.grid(row=1, column=1, sticky=W, padx=10)

accel_z_control = Scale(accel_frame, from_=(-1200), to=1200, label='Z')
accel_z_control.grid(row=1, column=2, sticky=W, padx=10)

def handle_accel_read_all(type, data):
    message = numpy.array([accel_x_control.get(),
                           accel_y_control.get(),
                           accel_z_control.get()], numpy.int16)
    message.dtype = numpy.uint8
#    print(message)
    transport.send_message(transport.MSG_ID_CC_ACCEL_READ_RSP, message)

transport.add_handler(transport.MSG_ID_CC_ACCEL_READ, handle_accel_read_all)


# 'terminal' output
terminal_frame = Frame(content_frame)
terminal_frame.grid(row=100, sticky=S)

Label(terminal_frame,
      text='___________________________________________________________________________').grid(row=0)

terminal_output = Text(terminal_frame, wrap=WORD, height=6, width=75, background=(terminal_color), state='normal')
terminal_output.grid(row=1, sticky=S)
scrollbar = Scrollbar(terminal_frame, command=terminal_output.yview)
terminal_output.config(yscrollcommand=scrollbar.set)
scrollbar.grid(row=1, column=1, sticky='ns')

Button(terminal_frame, text='Clear Terminal', command=lambda: terminal_output.delete(1.0, END) ).grid(row=2, sticky=E) 


# app.after(1000, led_set_color, 0, 0xFF, 0x00)
# app.after(2000, led_set_color, 0, 0, 0xFF)
# app.after(3000, led_set_color, 0xFF, 0, 0)
app.after(4000, led_set_color_to_current)

app.after_idle(check_serial)
app.mainloop()