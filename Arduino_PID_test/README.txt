-----------------------------------------------------------------------------------
---------------------------	Arduino_PID_test.ino 	---------------------------
-----------------------------------------------------------------------------------


=== 	QUICK GUIDE 	===

1. Download and install Arduino IDE.
 	- The folder structre is setup for Arduion IDE. But you can use other IDEs such as Atom. But you'll have to
 	  edit the folder structure yourself

2. Compile code (If errors: See Troubleshoot)

3. Select right COM-port and download file to mikrocontroller.
 	- You'll need to use Arduino UNO in for this setup to work correctly.

4. Open SerialPlotter or SerialMonitor.
   Here you can see ProcessValue (A0 on the circuit board).





=== 	TROUBLESHOOT 	===
Compiler error: Libraries wont link properly or similar issues.
Try installing the library files inside your global Arduino libraries folder. This can usually be located at:
--This PC
----Documents
------Arduino
--------libraries

Remmeber also to tell Arduino to look for Global libraries instead of local by editing the following
at line 2 and 3 in Arduino_PID_test.ino:

#include "ProcessShield.h" 		--> 	#include <ProcessShield.h> 	
#include "Arduino_FreeRTOS.h" 		-->	#include <Arduino_FreeRTOS.h> 