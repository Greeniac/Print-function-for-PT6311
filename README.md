Code to control the VFD driver PT6311
The idea of this code is keeping it so basic wich any person can use it easly.
This code allow to do the control of a board of a old DVD player using the buttons and Driver.
You can reuse any board with this driver if you applly a reverse engineering to chekc and adapt
the connections of the board in face you.

This code is related with the: https://youtu.be/Kt1JU3t7QJg



HOW TO USE:

1- Define the pins

2- You might have to change the letter/numbers if your display uses diffrent bits for each segments, if you need to, try the designs from these links:

https://github.com/emsyscode/PT6311/blob/master/PT6311_6grids_arduino_v3.ino   

https://github.com/emsyscode/SGNE-SO791GC-or-PT6311

https://github.com/emsyscode/PT6312-arduino-clock/blob/master/PT6312_code_youtube_18072019.cpp

https://github.com/emsyscode/PT6311-_Philips-_LX3700/blob/main/PT6311_philips_LX3700D-22S


3- Flash!

edit vfdprint(); in Setup() to display whatever you want, note that the function only takes strings that are capital letters and numbers, small letters and symbols are not implemented 


