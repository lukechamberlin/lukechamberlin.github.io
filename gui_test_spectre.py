import tkinter as tk
from tkinter import *
import tkinter.font as tkfont
import serial
import time
import serial.tools.list_ports

# function to grab the serial port that the arduino is connected to
def get_ports():
    ports = serial.tools.list_ports.comports() 
    return ports

def findArduino(portsFound):
    commPort = 'None'
    numConnection = len(portsFound)
    for i in range(0,numConnection):
        port = portsFound[i]
        strPort = str(port)
        if 'USB Serial Port' in strPort: #'USB Serial Port' is set for the AZ Delivery Arduino boards and may be different for different makes
            splitPort = strPort.split(' ')
            commPort = (splitPort[0])
    return commPort                    
foundPorts = get_ports()        
connectPort = findArduino(foundPorts)

if connectPort != 'None':
    ser = serial.Serial(connectPort,baudrate = 9600, timeout=1) ## IF THERE ARE CONNECTION ISSUES COPY THIS LINE TO THE FIRST INDENT AND CHANGE
    print('Connected to ' + connectPort)						## connectPort TO THE NAME OF THE PORT THAT THE ARDUINO IS CONNECTED TO eg.'COM6'
else:
    print('Connection Issue!')
print('DONE')

#parameters of the main window to load
window= tk.Tk()
window.geometry("600x350")
window.configure(bg = "#80BBB6")
window.title("Spectre Test Device")
#window.iconbitmap("C:/Users/Luke/Desktop/python gui programs/logosoteria_7gM_icon.ico") #change if the icon location moves
window.resizable(width = False, height = False)

#Attributes the rotation directions to the values on the scale to be used by the direction sliders
SCALE_LABELS = {0: "CCW",1: "CW",}

def scale_labels1(value):
	m1_direction.config(label=SCALE_LABELS[int(value)])

def scale_labels2(value):
	m2_direction.config(label=SCALE_LABELS[int(value)])

#function that sends the data to the serial port
def m1_power_button_action():
	m1powersend = str(m1_power.get())
	m2powersend = str(m2_power.get())
	m1directionsend = str(m1_direction.get())
	m2directionsend = str(m2_direction.get())
	hundredsmovements = str(rev_entry1.get())
	tensmovements = str(rev_entry2.get())
	onesmovements = str(rev_entry3.get())
	hundredsspeed = str(speed_entry1.get())
	tensspeed = str(speed_entry2.get())
	onesspeed = str(speed_entry3.get())
	#MUST HAVE <> AS END MARKERS TO BE READ PROPERLY BY THE ARDUINO
	data = ('<' + m1powersend + m2powersend + m1directionsend + m2directionsend + hundredsmovements + tensmovements + onesmovements + hundredsspeed + tensspeed + onesspeed + '>') 
	ser.write(bytes(data, 'utf-8'))
	time.sleep(1)
	print(data)
	
## Motor 1 power slider and label
m1_power = tk.Scale(
	window,
	from_=0, 
	to = 1,
	orient=tk.HORIZONTAL,
	showvalue = True,
	sliderlength = 100,
	length  = 130
	)

m1_power_label = tk.Label(window, text = "Group 1 Power", font =("arial", 12), bg = "#80BBB6")

## Motor 1 Direction slider and label
m1_direction = tk.Scale(
	window,
	from_=min(SCALE_LABELS), 
	to = max(SCALE_LABELS),
	orient=tk.HORIZONTAL,
	showvalue = False,
	command = scale_labels1,
	sliderlength = 100,
	length  = 130,
	label = False
	)

m1_direction_label = tk.Label(window, text = "Group 1 Direction", font =("arial", 12), bg = "#80BBB6")

## Motor 2 power slider and label
m2_power = tk.Scale(
	window,
	from_=0, 
	to = 1,
	orient=tk.HORIZONTAL,
	showvalue = True,
	sliderlength = 100,
	length  = 130
	)

m2_power_label = tk.Label(window, text = "Group 2 Power", font =("arial", 12), bg = "#80BBB6")

## Motor 2 Direction slider and label
m2_direction = tk.Scale(
	window,
	from_=min(SCALE_LABELS), 
	to = max(SCALE_LABELS),
	orient=tk.HORIZONTAL,
	showvalue = False,
	command = scale_labels2,
	sliderlength = 100,
	length  = 130,
	label = False
	)

m2_direction_label = tk.Label(window, text = "Group 2 Direction", font =("arial", 12), bg = "#80BBB6")

#Creates the revolution counter input window and button
rev_counter = tk.Canvas(
	window,
	height=300,
	width=200)

#title for the cycle counter
rev_title1 = tk.Label(
	window,
	text= "Cycles"
	)

rev_title1.config(font=("arial", 12))

rev_counter.create_window(
	100,
	20, 
	window = rev_title1
	)

#counter for the speed selector
speed_title1 = tk.Label(
	window,
	text= "Speed (MS)"
	)

speed_title1.config(font=("arial", 12))

rev_counter.create_window(
	100,
	275, 
	window = speed_title1
	)

rev_button = tk.Button(
	text="Run",
	bg='green',
	fg='white',
	font=("arial", 20),
	command=m1_power_button_action,
	)

rev_counter.create_window(
	100,
	150,
	window =rev_button,
	)

## Hundreds Entry label
rev_entry1 = tk.Entry(rev_counter, font = 20)

rev_counter.create_window(
	40,
	50, 
	window=rev_entry1,
	width=15,
	height=20)

# hundreds title for revolutions
rev_title_hundreds = tk.Label(
	window,
	text= "Hundreds"
	)

rev_title_hundreds.config(font=("arial", 10))

rev_counter.create_window(
	40,
	80, 
	window = rev_title_hundreds
	)

## Tens Entry Label for revolutions
rev_entry2 = tk.Entry(rev_counter, font = 20)

rev_counter.create_window(
	100,
	50, 
	window=rev_entry2,
	width=15,
	height=20)

#tens title for revolutions
rev_title_tens = tk.Label(
	window,
	text= "Tens"
	)

rev_title_tens.config(font=("arial", 10))

rev_counter.create_window(
	100,
	80, 
	window = rev_title_tens
	)

## Ones Entry Label
rev_entry3 = tk.Entry(rev_counter, font = 20)

rev_counter.create_window(
	160,
	50, 
	window=rev_entry3,
	width=15,
	height=20)

#ones title for revolutions
rev_title_ones = tk.Label(
	window,
	text= "Ones"
	)

rev_title_ones.config(font=("arial", 10))

rev_counter.create_window(
	160,
	80, 
	window = rev_title_ones
	)

## Hundreds Entry label speed selector
speed_entry1 = tk.Entry(rev_counter, font = 20)

rev_counter.create_window(
	40,
	250, 
	window=speed_entry1,
	width=15,
	height=20)

# hundreds title for speed selector
speed_title_hundreds = tk.Label(
	window,
	text= "Hundreds"
	)

speed_title_hundreds.config(font=("arial", 10))

rev_counter.create_window(
	40,
	220, 
	window = speed_title_hundreds
	)

## Tens Entry Label speed selector
speed_entry2 = tk.Entry(rev_counter, font = 20)

rev_counter.create_window(
	100,
	250, 
	window=speed_entry2,
	width=15,
	height=20)

#tens title for revolutions
speed_title_tens = tk.Label(
	window,
	text= "Tens"
	)

speed_title_tens.config(font=("arial", 10))

rev_counter.create_window(
	100,
	220, 
	window = speed_title_tens
	)

## Ones Entry Label for revolutions
speed_entry3 = tk.Entry(rev_counter, font = 20)

rev_counter.create_window(
	160,
	250, 
	window=speed_entry3,
	width=15,
	height=20)

#ones title for revolutions for revolutions
speed_title_ones = tk.Label(
	window,
	text= "Ones"
	)

speed_title_ones.config(font=("arial", 10))

rev_counter.create_window(
	160,
	220, 
	window = speed_title_ones
	)


#placement locations for all items on the GUI (non scaleable)
m1_power.place(x=10, y=125)
m1_power_label.place(x=15, y=175)
m2_power.place(x=10,y=275)
m2_power_label.place(x=15, y=230)
m1_direction.place(x=200, y=125)
m1_direction_label.place(x=205, y= 175)
m2_direction.place(x=200, y=275)
m2_direction_label.place(x=205, y= 230)
rev_counter.place (x=350,y=25)

window.mainloop()