/********************************************************************/
/********************************************************************/
/*Filename 	:  block_size.c		                            */
/*Description	:  This program implements the functionality for    */
/*                 determining the block size for a file system.    */
/*Input         :  Arg1 - file_20_mb.txt(file used for read access) */
/*Output	:  Returns the access time taken for varying reads  */
/*Note:         :  This code handles avoiding of prefetch mechanism */
/*                                                                  */   
/*Refernces	: http://www.linfo.org/get_block_size.html          */ 
/*                http://linux.die.net/man/8/mkfs.ext3              */
/*                http://linux.die.net/man/2/posix_fadvise          */ 
/*                http://unix.stackexchange.com/questions/43279/    */
/*              disabling-readahead-with-hdparm-or-posix-fadv-random*/
/*		  http://www.mcs.anl.gov/~kazutomo/rdtsc.h          */
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
  printf("Error in opening the file : out.txt\n");
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
	--argc;++argv;	
	if(argc == 1)
{	
	int i=0;
	char output[200];
	char outputarr[1000];
	unsigned char buffer[KB];
	int min_blocksize = 512;

/********execute the user defined function to retrieve the cpu speed and convert to GHz***********/	

	float mhz=(fetch_processor_clock_speed());

/*****************open file, the file name is provided as command line argument*******************/	
	int fd = open(argv[0],O_RDONLY);

/******any error occurred during the process of opening will cause the program to terminate*******/	
	if(fd < 0)
		{
		printf("Error while opening the file : file not found");
		return 1;
		}
/***************This is the user defined function in order to delete the cached data**************/
/***************so that it doesn't impact the calculation of block size***************************/
/***************further details can be referred in the project report*****************************/
	posix_fadvise(fd,0,0,POSIX_FADV_DONTNEED);
        
/***************This is the user defined function in order to disable the pre fetching mechanism **/
/***************so that it doesn't impact the calculation of block size***************************/
/***************further details can be referred in the project report*****************************/

	posix_fadvise(fd,0,0,POSIX_FADV_RANDOM);


/*********set the position of file pointer at the random location using lseek system call*********/		
	lseek(fd,0,SEEK_SET);
	
/******************access the file by reading the data in units of 512 bytes for each time********/
/*******************here we are reading 512 bytes at a time as we assumed the minimum block size**/
/*********************may be 512 bytes************************************************************/	
	
	for(int j = 0; j < 50 * KB ; j = j+512 )
	{
/**********************start of rdtsc() **********************************************************/		
	unsigned long long before = rdtsc();
	
/***************************read 512 bytes of data from the file to buffer************************/	       
	read(fd,buffer,min_blocksize);
/*********************************end of rdtsc()**************************************************/		   
	unsigned long long after = rdtsc();

/******************if offset greater than or equal to 512, calculate the time*********************/
/**********************************elapsed between the reads**************************************/
	if(j != 0)
	{
            double elapsed=(after-before)/mhz;
/*********************************Write the offset value and time elapsed into a array************/			
	    sprintf(output,"%d %f" , j , elapsed);
	    outputarr[i]=puts(output);
            i++;
	}
	}
/******************************************close the file*****************************************/	
close(fd);
return 0;
}
else
{
printf("Insufficient number of arguments\n");
}
}
