# WSU-Vancouver-Aerospace-20-21
All code used in electornic systems aboard the rocket built by the WSU Vancouver Aerospace club for the Spaceport America Cup 2020

There are three main electrical systems in the rocket. The avionics bay is where most of the electronics are stored and where the flight computer is located to cause the seperation events. The airbreaks which can add drag to the rocket if the projected apogee is higher than our target, and the payload system which will extend an antenna in the vertical-most position once the rocket has landed.

__AV BAY__<br/>
*Components:*  <br/>
&nbsp;&nbsp;&nbsp;&nbsp;- Atlas Telemetrum<br/>
&nbsp;&nbsp;&nbsp;&nbsp;- Backup flight computer (get more details ASAP)<br/>
&nbsp;&nbsp;&nbsp;&nbsp;- Raspberry Pi 4 Model B<br/>
&nbsp;&nbsp;&nbsp;&nbsp;- 4x Raspberry Pi Camera Modules 8MP<br/>
&nbsp;&nbsp;&nbsp;&nbsp;- Arducam Multi Camera Adapter module<br/>

*Purpose:* The Atlas Telemetrum will relay with the ground station to provide live flight data using telemetry and RF. Alongside the main and backup flight computers will be a Raspberry Pi board controlling four cameras.
    
__PAYLOAD__<br/>
*Components:* <br/>
&nbsp;&nbsp;&nbsp;&nbsp;- MPU-6050 (GY-521 breakout board)<br/>
&nbsp;&nbsp;&nbsp;&nbsp;- Arduino Nano<br/>
&nbsp;&nbsp;&nbsp;&nbsp;- Stepper motor<br/>
&nbsp;&nbsp;&nbsp;&nbsp;- A4988 Stepper Motor Driver Module<br/>
&nbsp;&nbsp;&nbsp;&nbsp;- Crossband repeater<br/>
&nbsp;&nbsp;&nbsp;&nbsp;- 9V battery<br/>
&nbsp;&nbsp;&nbsp;&nbsp;- Lipo Battery Pack<br/>

*Purpose:* The payload will remain intact with the rocket during all stages fo flight and recovery. The payload will serve to take radiation and EMF sampling for the duration of the rockets flight.

__AIRBREAKS__<br/>
*Components:* <br/>
&nbsp;&nbsp;&nbsp;&nbsp;- NEMA-17 Stepper (or something similar)<br/>
&nbsp;&nbsp;&nbsp;&nbsp;- A4988 Stepper Motor Driver Module<br/>
&nbsp;&nbsp;&nbsp;&nbsp;- Barometric pressure sensor (still need to decide on type)<br/>
&nbsp;&nbsp;&nbsp;&nbsp;- Arduino Nano<br/>
&nbsp;&nbsp;&nbsp;&nbsp;- 9V battery<br/>
&nbsp;&nbsp;&nbsp;&nbsp;- Hw-125 Micro SD card adapter<br/>
&nbsp;&nbsp;&nbsp;&nbsp;- Micro SD card<br/>

*Purpose:* To slow down the rocket and increase drag if the projected apogee of the rocket is above the target apogee. Three fins will extend from the sides of the body tube and slow the velocity of the rocket. Once the rocket's projected apogee is equal to or less than the target apogee, the fins will retract back inside and the rocket will coast to its projected apogee from there.
