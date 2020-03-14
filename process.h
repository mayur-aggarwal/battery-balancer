#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>

//#define DEBUG
#define MAX_VOLTAGE 13.0
#define MIN_VOLTAGE 9.0

int compare_float(float x, float y);
int set_voltage(float *v1, float *v2, float *v3);
int set_resistance(unsigned int res1, unsigned int res2, unsigned int res3);
void start_simulation();
void get_current();
void get_voltage();
void get_switchstate();
#endif