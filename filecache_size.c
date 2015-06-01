/********************************************************************/
/********************************************************************/
/*Filename 	:  filecache_size.c                                 */
/*Description	:  This program implements the functionality for    */
/*                 determining the cache size for a file system.    */
/*Input         :  Arg1 - file_200_mb.txt(file used for read access)*/
/*Output	:  Returns the access time taken for varying reads  */
/*Note:         :  Ensure that cache is not flushed on successive   */
/*                 reads.                                           */
/*                                                                  */   
/*Refernces	: http://linux.die.net/man/2/posix_fadvise          */ 
/*              http://unix.stackexchange.com/questions/43279/      */
/*              disabling-readahead-with-hdparm-or-posix-fadv-random*/
/*              http://www.mcs.anl.gov/~kazutomo/rdtsc.h            */
/*                                                                  */
/*                                                                  */ 
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
#include <stdbool.h>
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
  printf("Error in opening the file : out.txt\n");
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

/***********************************main program***************************************************/

int main(int argc, char * argv[])
{
/************************************Declaration of user defined variables***********************/
	--argc;argv++;
	if(argc == 1)
	{
	int i=0;
	int buffer_size = 4 * KB;
	int size = 0;
	char output[200];
	char outputarr[100000];
	int read_blocks = 0;

/********execute the user defined function to retrieve the cpu speed and convert to GHz***********/	
	float mhz=(fetch_processor_clock_speed());
	unsigned char *buffer=malloc(sizeof(*buffer)*buffer_size);

/*****************open file, the file name is provided as command line argument*******************/				
	int fd =open(argv[0],O_RDONLY);

/******any error occurred during the process of opening will cause the program to terminate*******/			
	if(fd < 0){
		printf("Error in opening the input file\n");
		return 1;
	}
/***************************read the file in the sequential order*********************************/
	while(true){
		int dataread = read(fd,buffer,buffer_size);
		if(dataread <= 0)
			break;
		size += dataread;
	}
		printf("Block no.""\t""Time(microsecs)""\n");
		printf("==========================================""\n");
/************now read the file in the reverse order, so that data is accessed from cache**********/	
		while(size > 4 * KB){
/**************************set the position of the file pointer, such that file accessed in*******/
/**************************reverse order *********************************************************/		
		lseek(fd,-buffer_size,SEEK_CUR);
/**********************start of rdtsc() **********************************************************/				
		unsigned long long before = rdtsc();
/*****************read certain num of bytes of data from the file to buffer***********************/	       		
		read(fd,buffer,buffer_size);
/*********************************end of rdtsc()**************************************************/		   			
		unsigned long long after= rdtsc();
/******************if offset greater than or equal to 512, calculate the time*********************/
/**********************************elapsed between the reads**************************************/		
		double elapsed=(after-before)/mhz;
		read_blocks = read_blocks + 1;		
		sprintf(output,"%d \t\t %f" ,read_blocks,elapsed );
		outputarr[i]=puts(output);
		i++;
		lseek(fd,-buffer_size,SEEK_CUR);
		size -= buffer_size;
	}

/************clear the cache, before next execution, close the file*********************************/				
	posix_fadvise(fd,0,0,POSIX_FADV_DONTNEED);
	close(fd);
	return 0;
}
else
{
printf("Insufficient arguments\n");
}
}

