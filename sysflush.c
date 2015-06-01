/*******************************************************************/
/*******************************************************************/
/*Filename 	:  sysflush.c                                      */
/*Description	:  This program executes the some linux commands to*/
/*                 clear the caches before the execution.          */
/*Input         :  Not applicable                                  */
/*Output	:  Clears the caches                               */
/*Note:         :                                                  */
/*Refernces	: https://www.cs.rutgers.edu/~pxk/416/notes/       */ 
/*                c-tutorials/gettime.html                         */
/*                http://linux.die.net/man/3/system                */
/*                http://www.mcs.anl.gov/~kazutomo/rdtsc.h         */ 
/*******************************************************************/
/*******************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void main()
{
/****************************Commands to clear caches*****************/

		//system("sudo blockdev --flushbufs /dev/sda1");
		//system("sudo hdparm --F /dev/sda1");
		//system("sudo sh -c echo 3>/proc/sys/vm/drop_caches");
}


