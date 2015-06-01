/********************************************************************/
/********************************************************************/
/*Filename 	:  direct_pointer                                   */
/*Description	:  This program implements the functionality for    */
/*                 determining the number of direct pointers        */
/*Input         :  files created in the code                        */
/*Output	:  Returns the access time taken for varying reads  */
/*Note:         :  Ensure that cache is not flushed on successive   */
/*                 reads                                            */
/*                                                                  */   
/*Refernces	: http://linux.die.net/man/2/posix_fadvise          */ 
/*              http://unix.stackexchange.com/questions/43279/      */
/*              disabling-readahead-with-hdparm-or-posix-fadv-random*/
/*              http://en.wikipedia.org/wiki/Ext2                   */
/*              http://www.mcs.anl.gov/~kazutomo/rdtsc.h            */ 
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
  printf("Error in opening the file: out.txt\n");
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
/*************************************Declaration of user defined variables*************************/
	--argc;argv++;
	if(argc == 1)
        {
     	int i=0;
	char buffer[4 * KB];
	int size = 0;
	char output[200];
	char outputarr[100000];
	int blocks_read=0;

/********execute the user defined function to retrieve the cpu speed and convert to GHz***********/    
	float mhz=(fetch_processor_clock_speed());
/*****************create a file, the file name is provided as command line argument***************/			
	int fd = open(argv[0],O_WRONLY | O_CREAT);
/******any error occurred during the process of creation of file will ****************************/
/***********************************cause the program to terminate********************************/					
	if(fd < 0)
	{
		printf("Error in creating the file");
		return 1;
	}
	
/********************************write 4K bytes of data into the newly created file**************/
	while(size < 100 * KB)
	{
/**********************start of rdtsc()**********************************************************/
		unsigned long long before = rdtsc();
/*****************write certain num of bytes of data to the file to buffer***********************/	       				
		write(fd,buffer,4 * KB);
		fsync(fd);

/*********************************end of rdtsc()**************************************************/		   			
		unsigned long long after = rdtsc();
/******************if offset greater than or equal to 512, calculate the time*********************/
/**********************************elapsed between the reads**************************************/				
		double elapsed=(after-before)/mhz;
/**************************************print the block number and elapsed time********************/
		blocks_read=blocks_read+1;
		sprintf(output,"%d %f" , blocks_read , elapsed);
		outputarr[i]=puts(output);
		i++;
		size += 4 * KB;
	}
/**************************************Close the file descriptor**********************************/	
	close(fd);
	return 0;
}
else
{
printf("Insufficient arguments\n");
}
}
