import os
import shutil
from datetime import datetime
import tkinter as tk
from tkinter import *
import tkinter.font as tkfont

import time

window= tk.Tk()
window.geometry("300x200")
window.configure(bg = "#A7A7A7")
window.title("Workspace Cleaner")
window.resizable(False,False)

#grabs date and time info to name new folder for Logs
dateinfo = str(datetime.now())
dateinfo_corrected = dateinfo.replace(" ", "_").replace("-", "_").replace(":", "_").replace(".", "_")

#path of the folder to store the Logs and date stamped folders
loginfofolder = 'C:/RCMLogs'
datestampedlogfolder= 'C:/RCMLogs'+ '/' + dateinfo_corrected
newlogdestination= 'C:/RCMLogs'+ '/' + dateinfo_corrected + '/'+ 'Log'

#folder to be moved 
source_dir = ('C:/RCMworkspace/logs')

#folder to be deleted
workspace_folder = ('C:/RCMworkspace')

def CheckForWorkspaceFolder():
	if os.path.exists(workspace_folder) == True:
		pass
	else:
		statusbar.config(text = "Workspace Folder Not Found")
		window.mainloop()

def MakeNewDirectory():
	if os.path.exists(datestampedlogfolder) == False:
		os.makedirs(datestampedlogfolder)
	else:
		statusbar.config(text = "Workspace Folder Already Moved")

def MoveLogToDirectory():
	if os.path.exists(newlogdestination) == False:
		shutil.move(source_dir, datestampedlogfolder)
	else:
		statusbar.config(text = "Workspace Folder Already Moved")

def DeleteWorkspaceFolder():
	if os.path.exists(workspace_folder) == True:
		shutil.rmtree(workspace_folder)
		statusbar.config(text = "Log Folder Move Complete")
		window.after(3000, window.destroy)
		
	else:
		statusbar.config(text = "Workspace Folder Already Moved")

def MoveLogFolder():
	#see if there is a workspace folder to delete
	CheckForWorkspaceFolder()

	#generates a folder to store the datestamped folders
	if not os.path.exists(loginfofolder):
		os.makedirs(loginfofolder)
	else:
		print('Folder already exists')

	#makes the datestamped folder to store log files
	MakeNewDirectory()

	#moves the files to a datestamped folder
	MoveLogToDirectory()

	#deletes workspace folder
	DeleteWorkspaceFolder()

folder_move_button = tk.Button(
	text="Delete Workspace",
	bg='#D2F6D0',
	fg='#013A65',
	font=("arial", 12),
	command=MoveLogFolder,
	width = 15,
	height = 2,
	)

statusbar = tk.Label(window, text="", bd=1, relief=tk.SUNKEN, anchor=tk.W)

statusbar.pack(side=tk.BOTTOM, fill=tk.X)

folder_move_button.place(x=80, y=100)

window.mainloop()

