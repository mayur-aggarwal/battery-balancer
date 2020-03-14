/**
 * @file	process.c
 * @author	Mayur Aggarwal
 * @date	Jun 28, 2016
 * @brief	cell balancing simulation
 */
#include "process.h"
#include <pthread.h>

typedef unsigned int uint_32;

/*	cell data Structure */
typedef struct cellbalancing
{
	float b1_v;
	float b2_v;
	float b3_v;
	float b1_i;
	float b2_i;
	float b3_i;
	uint_32 r1;
	uint_32 r2;
	uint_32 r3;
	uint_32 main_load;
}Celldata;

struct State
{
	int S1;
	int S2;
	int S3;
}state;

Celldata tCelldata;

/*	Global Variables */
float max_volt;
float current;
float current_per_sec;
float volt_per_sec;
int flag1,flag2,flag3;
int cell1,cell2,cell3;
int isSetResistanceDone;
int isSetVoltageDone;
int isSetLoad;
int stopSimulation=0;
int cell1_thresh,cell2_thresh,cell3_thresh;

/*	thread ID's */
pthread_t pstart = 0;
pthread_t pmonitor = 0;
pthread_t pdisplay = 0;

/**
  * @fn		void reset_all(void))
  * @brief	reset all global vars
  *			
  * @param	void
  * @return	void
  * @warning	
  */
void reset_all(void)
{
	memset(&tCelldata,0,sizeof(tCelldata));
	cell1_thresh=cell2_thresh=cell3_thresh=0;
	flag1=flag2=flag3=0;
	cell1=cell2=cell3=0;
	stopSimulation=isSetLoad=isSetVoltageDone=isSetResistanceDone=0;
	current_per_sec = volt_per_sec=0.0;
	max_volt = 0.0;
	current = 0.0;
}

/**
  * @fn		int is_nan(float x)
  * @brief	checks for Not a Number
  *			
  * @param	float
  * @return	int
  * @warning	
  */
int is_nan(float x)
{
	return x!=x;
}

/**
  * @fn		int compare_float(float x, float y)
  * @brief	compare float number
  *			
  * @param	float x
  * @param	float y
  * @return	int
  * @warning	
  */
int compare_float(float x, float y)
{
	if (is_nan(y))
		return !is_nan(x);
	return x < y;
}
/**
  * @fn		int set_voltage(unsigned int load)
  * @brief	set voltage value
  *			
  * @param	float *v1
  * @param	float *v2
  * @param	float *v3
  * @return	int
  * @warning	
  */
int set_voltage(float *v1, float *v2, float *v3)
{
	tCelldata.b1_v = *v1;
	tCelldata.b2_v = *v2;
	tCelldata.b3_v = *v3;
	isSetVoltageDone = 1;
	return 1;
}

/**
  * @fn		int set_resistance(unsigned int res1, unsigned int res2, unsigned int res3)
  * @brief	set resistance value
  *			
  * @param	unsigned int res1
  * @param	unsigned int res2
  * @param	unsigned int res3
  * @return	int
  * @warning	
  */
int set_resistance(unsigned int res1, unsigned int res2, unsigned int res3)
{
	tCelldata.r1 = res1;
	tCelldata.r2 = res2;
	tCelldata.r3 = res3;
	isSetResistanceDone = 1;
	return 1;
}

/**
  * @fn		int set_load(unsigned int load)
  * @brief	set load value
  *			
  * @param	unsigned int
  * @return	int
  * @warning	
  */
int set_load(unsigned int load)
{
	tCelldata.main_load = load;
	isSetLoad = 1;
	return 1;
}

#ifdef DEBUG
/**
  * @fn		int calculate_total_current(void)
  * @brief	calculate total current in whole battery pack
  *			
  * @param	void
  * @return	int
  * @warning	
  */
int calculate_total_current(void)
{
	int i1 = 0, i2 = 0, i3 = 0;
	int total_i = 0;
	tCelldata.b1_i = tCelldata.b1_v / tCelldata.r1;
	tCelldata.b2_i = tCelldata.b2_v / tCelldata.r2;
	tCelldata.b3_i = tCelldata.b3_v / tCelldata.r3;
	total_i = tCelldata.b1_i + tCelldata.b2_i + tCelldata.b3_i;
	return total_i;
}
#endif

/**
  * @fn		void discharge_curve_per_sec(float *v, float *i)
  * @brief	discharge_curve_per_sec, update voltage and current value
  *			
  * @param	float *v
  * @param	float *i
  * @return	void
  * @warning	
  */
void discharge_curve_per_sec(float *v, float *i)
{
	*v = *v - volt_per_sec;
	*i = *i - current_per_sec;
}


/**
  * @fn		void find_max_volt_current(void)
  * @brief	find_max_volt_current
  *			
  * @param	void
  * @return	void
  * @warning	
  */
void find_max_volt_current(void)
{
#ifdef DEBUG
	printf("[%s]\n",__FUNCTION__);
#endif
	if(tCelldata.b1_v == tCelldata.b2_v && tCelldata.b2_v == tCelldata.b3_v )
	{
		printf("Cells voltages are Equal \n");
	}
	else if(cell1_thresh && cell2_thresh && cell3_thresh)
	{
		;
	}
	else if(tCelldata.b1_v > tCelldata.b2_v && tCelldata.b1_v > tCelldata.b3_v)
	{
		max_volt = tCelldata.b1_v;
		current = max_volt / tCelldata.r1;
		current_per_sec = current / 3600; //per sec
		volt_per_sec = current_per_sec * tCelldata.r1;
		tCelldata.b1_i = tCelldata.b1_v / tCelldata.r1;
		state.S1 = 1;
		flag1=1;
		cell1 = 1;
#ifdef DEBUG
		printf("state S1 = %d\n",state.S1);
	//	discharge_curve_per_sec(&tCelldata.b1_v,&tCelldata.b1_i);
		printf("max_volt V1 = %f volt_per_sec = %f current_per_sec = %f\n",max_volt,volt_per_sec,current_per_sec);
#endif
	}
	else if(tCelldata.b2_v > tCelldata.b3_v && tCelldata.b2_v > tCelldata.b1_v)
	{
		max_volt = tCelldata.b2_v;
		current = max_volt / tCelldata.r2;
		current_per_sec = current / 3600; //per sec
		volt_per_sec = current_per_sec * tCelldata.r2;
		tCelldata.b2_i = tCelldata.b2_v / tCelldata.r2;
		state.S2 = 1;
		flag2=1;
		cell2 = 1;
#ifdef DEBUG
		printf("state S2 = %d\n",state.S2);
		printf("max_volt V2 = %f volt_per_sec = %f current_per_sec = %f\n",max_volt,volt_per_sec,current_per_sec);
#endif
	}
	else
	{
		max_volt = tCelldata.b3_v;
		current = max_volt / tCelldata.r3;
		current_per_sec = current / 3600; //per sec
		volt_per_sec = current_per_sec * tCelldata.r3;
		tCelldata.b3_i = tCelldata.b3_v / tCelldata.r3;
		state.S3 = 1;
		flag3=1;
		cell3 = 1;
#ifdef DEBUG
		printf("state S3 = %d\n",state.S3);
		//discharge_curve_per_sec(&tCelldata.b3_v,&tCelldata.b3_i);
		printf("max_volt V3 = %f current_per_sec = %f volt_per_sec = %f \n",max_volt,current_per_sec,volt_per_sec);
#endif
	}
}

/**
  * @fn		void monitor(void)
  * @brief	monitor thread which checks for voltage for each battery
  *			
  * @param	void
  * @return	void
  * @warning	
  */
void monitor(void) 
{
#ifdef DEBUG
	printf("[%s] Enter..\n",__FUNCTION__);
#endif
	while(1)
	{
		if(flag1 && (tCelldata.b1_v <= tCelldata.b2_v || tCelldata.b1_v <= tCelldata.b3_v))
		{
#ifdef DEBUG
			printf("1 battery reached near 2 or 3rd battery level\n");
			get_voltage();
#endif
			find_max_volt_current();
			usleep(100);
			flag1 = 0;
		}
		if(flag2 && (tCelldata.b2_v <= tCelldata.b1_v || tCelldata.b2_v <= tCelldata.b3_v))
		{
#ifdef DEBUG
			printf("2 battery reached near 1 or 3rd battery level\n");
			get_voltage();
#endif
			find_max_volt_current();
			usleep(100);
			flag2 = 0;
		}
		if(flag3 && (tCelldata.b3_v <= tCelldata.b1_v || tCelldata.b3_v <= tCelldata.b2_v))
		{
#ifdef DEBUG
			printf("3 battery reached near 1 or 2nd battery level\n");
			get_voltage();
#endif
			find_max_volt_current();
			usleep(100);
			flag3 = 0;
		}
		
		if((!cell1_thresh && tCelldata.b1_v <= MIN_VOLTAGE) || (!cell2_thresh && tCelldata.b2_v <= MIN_VOLTAGE) || (!cell3_thresh && tCelldata.b3_v <= MIN_VOLTAGE))
		{
			stopSimulation = 1;
#ifdef DEBUG
			printf("Cell1 = %d Cell2 = %d Cell3 = %d\n",cell1_thresh,cell2_thresh,cell3_thresh); 
			get_voltage();
#endif
			printf("Battery Reached Min Voltage, Stopping Simulation ...\n");
			state.S1 = 0;
			state.S2 = 0;
			state.S3 = 0;
			
			reset_all();
			
			break;
		}
		if(cell1_thresh && cell2_thresh && cell3_thresh)
		{
			stopSimulation = 1;
			printf("Battery is below Min Voltage, Stopping Simulation ...\n");
			state.S1 = 0;
			state.S2 = 0;
			state.S3 = 0;
			
			reset_all();
			
			break;
		}
		usleep(100);
	}
#ifdef DEBUG
	printf("%s --> Exit\n",__FUNCTION__);
#endif
}

/**
  * @fn		void start(void)
  * @brief	start thread which make discharge curve and update the voltage and current for each battery every sec
  *			
  * @param	void
  * @return	void
  * @warning	
  */
void start(void)
{
#ifdef DEBUG
	printf("[%s] Enter..\n",__FUNCTION__);
#endif
	if(tCelldata.b1_v <= MIN_VOLTAGE)
	{
		cell1_thresh = 1;
	}
	if(tCelldata.b2_v <= MIN_VOLTAGE)
	{
		cell2_thresh = 1;
	}
	if(tCelldata.b3_v <= MIN_VOLTAGE)
	{
		cell3_thresh = 1;
	}
	find_max_volt_current();
	while(1)
	{
		if(cell1)
			discharge_curve_per_sec(&tCelldata.b1_v,&tCelldata.b1_i);
		if(cell2)
			discharge_curve_per_sec(&tCelldata.b2_v,&tCelldata.b2_i);
		if(cell3)
			discharge_curve_per_sec(&tCelldata.b3_v,&tCelldata.b3_i);
		sleep(1);	

		if(stopSimulation)
			break;
		
	}
#ifdef DEBUG
	printf("%s --> Exit\n",__FUNCTION__);
#endif
}

#ifdef DEBUG
/**
  * @fn		void display(void)
  * @brief	display all values for debug purpose
  *			
  * @param	void
  * @return	void
  * @warning	
  */
void display(void)
{
	while(1)
	{
		printf("\n-------Result-------\n");
		get_voltage();
		get_current();
		get_switchstate();
		sleep(5);
		if(stopSimulation)
			break;
	}
	printf("%s --> Exit\n",__FUNCTION__);
}
#endif

/**
  * @fn		void start_simulation()
  * @brief	starts simulation
  *			
  * @param	void
  * @return	void
  * @warning	
  */
void start_simulation(void)
{
	if(isSetVoltageDone && isSetResistanceDone && isSetLoad)
	{
		pthread_create(&pstart, NULL, (void *) start, NULL );
		if(pstart)
			pthread_detach(pstart);
		usleep(100);
		pthread_create(&pmonitor, NULL, (void *) monitor, NULL );
		if(pmonitor)
			pthread_detach(pmonitor);
#ifdef DEBUG
		pthread_create(&pdisplay, NULL, (void *) display, NULL );
		if(pdisplay)
			pthread_detach(pdisplay);	
#endif
	}
	else
	{
		printf("First set Voltage, load and Resistance values..\n");
	}
}

/**
  * @fn		void get_current()
  * @brief	get each battery current
  *			
  * @param	void
  * @return	void
  * @warning	
  */
void get_current()
{
	printf("B1: %f \nB2: %f \nB3: %f\n",tCelldata.b1_i,tCelldata.b2_i,tCelldata.b3_i);
}

/**
  * @fn		void get_voltage()
  * @brief	get each battery voltage
  *			
  * @param	void
  * @return	void
  * @warning	
  */
void get_voltage()
{
	printf("B1: %f \nB2: %f \nB3: %f\n",tCelldata.b1_v,tCelldata.b2_v,tCelldata.b3_v);
}

/**
  * @fn		void get_switchstate()
  * @brief	get battery switch state
  *			
  * @param	void
  * @return	void
  * @warning	
  */
void get_switchstate()
{
	if(state.S1)
		printf("B1: ON\n");
	else
		printf("B1: OFF\n");
	if(state.S2)
		printf("B2: ON\n");
	else
		printf("B2: OFF\n");
	if(state.S3)
		printf("B3: ON\n");
	else
		printf("B3: OFF\n");
}