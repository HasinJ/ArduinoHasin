import serial
import pyautogui

ardSerial = serial.Serial('com7',9600)


while 1:
    checking = (ardSerial.readline())
    
    print checking
    
    if "Play/Pause" in checking:
        pyautogui.press('playpause')
        print ('done') #just to check
    if "Next" in checking:
        pyautogui.press('nexttrack')
        print ('done')
    if "Prev" in checking:
        pyautogui.press('prevtrack') 
        pyautogui.press('prevtrack') #because on spotify the first prevtrack only goes to beginning
        print('done')
    if "Cprev" in checking:
        pyautogui.press('prevtrack') #however it will go back if the song hasn't progressed too far
        print('done')
