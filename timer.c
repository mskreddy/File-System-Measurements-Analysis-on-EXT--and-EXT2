/********************************************************************/
/********************************************************************/
/*Filename 	:  timer.c                                          */
/*Description	:  This program implements the functionality for    */
/*                 comparing performance of rdtsc with gettimeofday */
/*Input         :  None                                             */
/*Output	:  Returns the access time taken for each timer.    */
/*Refernces	: http://en.wikipedia.org/wiki/Time_Stamp_Counter   */ 
/*              http://linux.die.net/man/2/gettimeofday/            */
/********************************************************************/
/********************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "rdtsc.h"
#include <sys/time.h>

/*****This is the user defined function to retrieve the CPU processor speed of the system on which**/
/******this program is executed, it is determined using the system command "/proc/cpuinfo" ********/
float get_cpu_clock_speed ()
{
  FILE* fp;
  char buffer[1024000];
  size_t bytes_read;
  char* match;
  float clock_speed;

/*************Execute the command /proc/cpuinfo and write the content into a file*****************/
  system("cat /proc/cpuinfo > out.txt");
  fp=fopen("out.txt","r");
/******************************Read the contents from file into a buffer**************************/
  bytes_read = fread (buffer, 1, sizeof (buffer), fp);
/**********************************close the file pointer*****************************************/ 
 fclose (fp);

/************************************If there is no data in the buffer,terminate******************/
  if (bytes_read == 0 || bytes_read == sizeof (buffer))
    return 0;
  buffer[bytes_read] = '\0';

/*****************************Indentify for the line which contains "cpu MHz"*********************/
  match = strstr (buffer, "cpu MHz");
  if (match == NULL)
    return 0;

/***********************************Parse the line to extract the value of clock speed************/
  sscanf (match, "cpu MHz : %f", &clock_speed);
/*************************************return the clock speed and exit*****************************/ 
  return clock_speed;
}


/***************************************main function*********************************************/
int main(int argc, char * argv[])
{
/********************************Declaration of user defined variables****************************/
	int i=0;
  	struct timeval start,end;
/*******************************Execute user defined function to retrieve cpu speed***************/
 	float mhz=(get_cpu_clock_speed());
/*******************************start the rdtsc call with sleep of 10 seconds*********************/	
	unsigned long long before = rdtsc();
	sleep(10);		
	unsigned long long after  = rdtsc();
/********************calculate the elapsed time to perform this operation in microsecs************/
	double final=(after-before)/(mhz);
	printf("Time of rdtsc %lf microseconds\n",final);
/*******************************start the gettimeofday call with sleep of 10 seconds*********************/		
  	gettimeofday(&start, NULL);
	sleep(10);
  	gettimeofday(&end, NULL);
/********************calculate the elapsed time to perform this operation in microsecs************/
	printf("Time of gettimeofday %ld microseconds\n", ((end.tv_sec * 1000000 + end.tv_usec)
		  - (start.tv_sec * 1000000 + start.tv_usec)));

  	return 0;
}

