/********************************************************************/
/********************************************************************/
/*Filename 	:  prefetch_size.c                                  */
/*Description	:  This program implements the functionality for    */
/*                 determining the prefetch size for a file system. */
/*Input         :  Arg1 - file_50_mb.txt(file used for read access) */
/*Output	:  Returns the access time taken for varying reads  */
/*Note:         :  This code handles avoiding of cache of data.     */
/*                                                                  */   
/*Refernces	: http://linux.die.net/man/2/posix_fadvise          */ 
/*                http://www.mcs.anl.gov/~kazutomo/rdtsc.h          */
/********************************************************************/
/********************************************************************/

#define _POSIX_C_SOURCE 200112L
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include "rdtsc.h"

/***********************************user defined constants****************************************/
/***************defining Kilo byte, Mega Byte, Giga Byte in bytes, for future use in the code*****/
#define KB 1024
#define MB 1048576
#define GB 1073741824  

/*****This is the user defined function to retrieve the CPU processor speed of the system on which**/
/******this program is executed, it is determined using the system command "/proc/cpuinfo" ********/
float fetch_processor_clock_speed ()
{
  FILE* fp;
  char buffer[1024000];
  size_t bytes_read;
  char* found;
  float clock_speed;

/*************Execute the command /proc/cpuinfo and write the content into a file*****************/
  system("cat /proc/cpuinfo > out.txt");
  fp=fopen("out.txt","r");

  if(fp == NULL)
  {
  printf("Error in opening the file:out.txt\n");
  return 1;  
  }
  else
  {
/******************************Read the contents from file into a buffer**************************/
  bytes_read = fread (buffer, 1, sizeof (buffer), fp);
/**********************************close the file pointer*****************************************/ 
 fclose (fp);

/************************************If there is no data in the buffer,terminate******************/
  if (bytes_read == 0 || bytes_read == sizeof (buffer))
    return 0;
  buffer[bytes_read] = '\0';

/*****************************Indentify for the line which contains "cpu MHz"*********************/
  found = strstr (buffer, "cpu MHz");
  if (found == NULL)
    return 0;

/***********************************Parse the line to extract the value of clock speed************/
  sscanf (found, "cpu MHz : %f", &clock_speed);
/*************************************return the clock speed and exit*****************************/ 
  return clock_speed;
  }
}
/*************************************end of user defined functions*********************************/

/*************************************Main function start*******************************************/
int main(int argc, char * argv[])
{
/**********************************declaration of user defined variables*************************/	
	--argc;argv++;	 
	if(argc == 2)
	{
	int i=0;
        int buffer_size = KB;
	int k = atoi(argv[1]);
	int bytecount = 0;
	char output[200];
	char outputarr[1000];

/********execute the user defined function to retrieve the cpu speed and convert to GHz***********/	

	float mhz=(fetch_processor_clock_speed());

/*****************open file, the file name is provided as command line argument*******************/	
	int fd = open(argv[0],O_RDONLY);
/******any error occurred during the process of opening will cause the program to terminate*******/	
	
	if(fd < 0){
		printf("file not found");
		return 1;
	}
/******In order to prevent prefetch / cache mechanisms, use the above user defined functions*****/	
	posix_fadvise(fd,0,0,POSIX_FADV_DONTNEED);

	unsigned char *buffer=malloc(sizeof(*buffer)*buffer_size);
	
	while(bytecount < k * MB)
	{
		read(fd,buffer,buffer_size);
		bytecount += buffer_size;
	}
/******************************issued sleep command for 1 second to complete the prefetch process***/
	sleep(1);
	
	bytecount = 0;
/***************************for each iteration read 10 kilo byte of data from the file***************/	
	buffer_size = 10 * KB;
	free(buffer);
	buffer[buffer_size];
	printf("Kilobytes" "\t" "Time(microsecs)""\n");
	printf("=====================================""\n");	
	while(bytecount < MB)
	{
		unsigned long long before = rdtsc();
		int dataread = read(fd,buffer,buffer_size);
		unsigned long long after  = rdtsc();
		double elapsed=(after-before)/mhz;   //converting cycles to microseconds;
		sprintf(output,"%d \t\t %f" , bytecount/KB,elapsed );
		outputarr[i]=puts(output); 
		i++;
		bytecount += dataread;
	}
/****************************************close the file descriptor************************************/
	close(fd);
	return 0;
}
else
{
printf("insufficient arguments\n");
}
}

