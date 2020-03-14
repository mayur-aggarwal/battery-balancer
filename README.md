# Battery Balancing Algorithm
## *_Description_*

Today, batteries are the backbone technology of all the Portable electronic devices around us. For optimal operation of a battery pack, the cells constituting the battery pack should all be around the same voltage. Usually, such battery packs have a charge cycle and a discharge cycle. I have implemented a battery balancing algorithm during the discharge cycle.
The algorithm will switch the switches S1, S2, and S3 on/off at appropriate timings such that the voltage of the B1, B2, B3 remains roughly the same throughout the duration of operation of the battery. (B1, B2, B3 are the cells which constitute the battery pack)

<p align="center">
  <img src="https://github.com/mayur-aggarwal/battery-balancer/blob/master/battery_switch_load.png">
</p>
<p align="center">Figure 1: Battery with switches.</p>

## *_Assumptions_*
1.	MAX_VOLTAGE = 13.0 V, MIN_VOLTAGE = 9.0V
2.	3 batteries connected in parallel.
3.	To start simulation, 3 battery voltages, resistance value (1 ohm to 1K ohm) and Load value is must to set.
4.	Stop balancing the cells once battery reach min voltage and simulation will be stopped.
5.	On based of cell voltage and resistance, current/sec calculated in AH.
6.	V = IR --> I = V/R -->Ic = I/3600 (1 hour = 60 min, 1min = 60 sec, 1 sec = 60*60 = 3600) AH
7.	Voltage required to serve the load per sec = Ic * R

## *Implementation of simulator which consists of following API's.*
1. Seven API's have been developed
	i.	setInitialCellVoltage v1 v2 v2 (for ex) setInitialCellVoltage 10.5 12.4 11.7)
	ii.	setResistanceVal r1 r2 r3 (for ex) setResistanceVal 10 10 10)
	iii.	setLoad load (for ex) setLoad 100)
	iv.	startSimulation : Start simulation.
	v.	getVoltage : will show each battery voltage
	vi.	getSourceCurrent : will show each battery current
	vii.	getSwitchState: will show each battery switch state
2.	Reading input (string) through fgets() and then extracting tokens from string by using strtok_r.
3.	Comparing string “API which is inputted” and processing.
4.	When voltage, resistance or load value is set through API, it is been set to global structure.
5.	Two threads have been created when simulation is started.
6.	Checking for highest voltage among three batteries, turning it ON and then voltage and current is subtracted from the calculated voltage required per sec and current to serve the load in while(1) till the highest voltage selected comes equal or less than to any other battery in other thread.  
7.	Second thread is to monitor constantly the voltage drop per sec. If selected voltage comes less than or equal to any other 2 batteries than it will switch ON that battery along with current one. Similar way last battery will also take decision to turn ON or remain OFF based on discharge curve and its own voltage. 
8.	Both threads are detachable hence once thread is exited all resources will be free.
9.	Simulation will be stopped once min voltage is reached and turn OFF all 3 switches including reset all global vars. Battery pack will stop serving the load. Need to set the voltage, resistor and load values to start again the simulation.

<p align="center">
  <img src="https://github.com/mayur-aggarwal/battery-balancer/blob/master/battery_balancer_flowchart.png">
</p>
<p align="center">Figure 2: Battery Balancer Flowchart.</p>

### _How to run:_
1.	Run ./simulation executable file, can only be run on unix/linux platform.
2.	For compilation, fetch all *.c, *h & make file and then execute “make all” command.
3.	To clean, execute “make clean”.
5.	Executable binary will be formed by name "simulation”.
