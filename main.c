/**
 * @file	main.c
 * @author	Mayur Aggarwal
 * @date	Jun 28, 2016
 * @brief	simulation command-line interface (CLI)
 */

#include "process.h"

#define BUF_SIZE 1024
#define MAX_TOKENS 5


int main(int argc, char *argv[]) 
{
	char consoleCmd[BUF_SIZE]="";
	char *tokens[MAX_TOKENS];
	char *tmpToken;
	char *psaveTok = NULL;
	
	int i = 0;

	printf( "===============================================\n");
	printf( "	  Simulation user guide (API List):	\n" );
	printf( "===============================================\n");
	printf( "- setInitialCellVoltage : <<setInitialCellVoltage [V1] [V2] [V3]>>\n" );
	printf( "- setResistanceVal      : <<setResistanceVal [R1] [R2] [R3]>> \n" );
	printf( "- setLoad               : <<setLoad [load val]>>\n" );
	printf( "- startSimulation       : <<startSimulation>>\n" );
	printf( "- getVoltage            : <<getVoltage>>\n" );
	printf( "- getSourceCurrent      : <<getSourceCurrent>>\n" );
	printf( "- getSwitchState        : <<getSwitchState>>\n" );
	while(1) 
	{
		printf( "CMD : ");
		memset(consoleCmd, 0x00, sizeof(consoleCmd));
		fgets(consoleCmd, BUF_SIZE, stdin);	// read the input
		if ( 0 < strlen( consoleCmd ) ) 
		{
			consoleCmd[strlen( consoleCmd )-1] = 0x00;
        }
		else 
		{
		 	usleep(5000);
            continue;
        }
		i = 0;
		psaveTok = NULL;
		tmpToken = strtok_r( consoleCmd, " ", &psaveTok);
		tokens[i] = tmpToken;
		/*	Below code is to check the command line argument supplied with API	*/
		while ( NULL != tmpToken )
        {
			tmpToken = strtok_r( NULL, " ", &psaveTok);
            i++;
            tokens[i] = tmpToken; 
        }
		if(!strcmp(consoleCmd, "setInitialCellVoltage") ) 
		{
			float V1 = 0,V2 = 0,V3 = 0;
			int ret = -1;
			if(tokens[1] == NULL || tokens[2] == NULL || tokens[3] == NULL) 
			{
				printf("ex) setInitialCellVoltage [V1] [V2] [V3]\n");	// any arg is missing then continue
				continue;
			}
			sscanf (tokens[1], "%f", &V1);
#ifdef DEBUG
			ret = compare_float(V1,MAX_VOLTAGE);
			if(!ret)
			{
				printf ("Error 1:Voltage should be less than 13V and more than 9 V. Pls Input again"); 
				continue;
			}
			ret = compare_float(V1,MIN_VOLTAGE);
			if(ret)
			{
				printf ("Error 1:Voltage should be less than 13V and more than 9 V. Pls Input again"); 
				continue;
			}					
			printf("V1 [%f] ",V1);
#endif			
			sscanf (tokens[2], "%f", &V2);
#ifdef DEBUG
			ret = compare_float(V2,MAX_VOLTAGE);
			if(!ret)
			{
				printf ("Error 2:Voltage should be less than 13V and more than 9 V. Pls Input again"); 
				continue;
			}
			ret = compare_float(V2,MIN_VOLTAGE);
			if(ret)
			{
				printf ("Error 2:Voltage should be less than 13V and more than 9 V. Pls Input again"); 
				continue;
			}
			printf("V2 [%f] ",V2);
#endif
			
			sscanf (tokens[3], "%f", &V3);
#ifdef DEBUG
			ret = compare_float(V3,MAX_VOLTAGE);
			if(!ret)
			{
				printf ("Error 3:Voltage should be less than 13V and more than 9 V. Pls Input again"); 
				continue;
			}
			ret = compare_float(V3,MIN_VOLTAGE);
			if(ret)
			{
				printf ("Error 3:Voltage should be less than 13V and more than 9 V. Pls Input again"); 
				continue;
			}
		//	V3 = ((int)(V3 * 100 + .5) / 100.0);	// to round-off the float value
			printf("V3 [%f]\n",V3);
#endif
			ret = set_voltage(&V1, &V2, &V3);
			if(!ret)
			  printf("Failed to set the voltage value\n");
		}
		else if(!strcmp(consoleCmd, "setLoad") ) 
		{
			unsigned int load = 0;
			int ret = -1;
			if(tokens[1] == NULL) 
			{
				printf("ex) setLoad [load]\n");
				continue;
			}
			load = atoi(tokens[1]);
#ifdef DEBUG			
			printf("load [%d]\n",load);
#endif
			ret = set_load(load);
			if(!ret)
			  printf("Failed to set the setLoad value\n");
		}
		else if(!strcmp(consoleCmd, "setResistanceVal") ) 
		{
			unsigned int R1 = 0,R2 = 0,R3 = 0;
			int ret = -1;
			if(tokens[1] == NULL || tokens[2] == NULL || tokens[3] == NULL) 
			{
				printf("ex) setResistanceVal [R1] [R2] [R3]\n");
				continue;
			}
			R1 = atoi(tokens[1]);	
			R2 = atoi(tokens[2]);
			R3 = atoi(tokens[3]);
#ifdef DEBUG
			printf("R1 [%d]",R1);
			printf("R2 [%d]",R2);
			printf("R3 [%d]\n",R3);
#endif
			ret = set_resistance(R1, R2, R3);
			if(!ret)
			  printf("Failed to set the resistance value\n");
		}
		else if(!strcmp(consoleCmd, "startSimulation") ) 
		{
			start_simulation();
		}
		else if(!strcmp(consoleCmd, "getSourceCurrent") ) 
		{
			get_current();
		}
		else if(!strcmp(consoleCmd, "getVoltage"))
		{
			get_voltage();
		}
		else if(!strcmp(consoleCmd, "getSwitchState"))
		{
			get_switchstate();
		}
		else if(!strcmp(consoleCmd, "help") ) 	// print all the API's
		{
			printf( "- setInitialCellVoltage : <<setInitialCellVoltage [V1] [V2] [V3]>>\n" );
			printf( "- setResistanceVal      : <<setResistanceVal [R1] [R2] [R3]>> \n" );
			printf( "- setLoad               : <<setLoad [load val]>>\n" );
			printf( "- startSimulation       : <<startSimulation>>\n" );
			printf( "- getVoltage            : <<getVoltage>>\n" );
			printf( "- getSourceCurrent      : <<getSourceCurrent>>\n" );
			printf( "- getSwitchState        : <<getSwitchState>>\n" );
		}
		else if(!strcmp(consoleCmd, "exit") ) 	//exit the simulation
		{
			break;
		}
		else
		{
			;
		}
	}
	return 0;
}
