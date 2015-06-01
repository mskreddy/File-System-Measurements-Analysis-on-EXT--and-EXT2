/*******************************************************************/
/*******************************************************************/
/*Filename 	:  file_create.c                                   */
/*Description	:  This program executes the shell commands        */
/*                 to create input files of various sizes using    */
/*                 system() command.                               */
/*Input         :  None                                            */
/*Output	:  creates file and store in inputfiles folder.    */
/*Refernces	: http://linux.die.net/man/3/system                */
/*                http://www.unix.com/shell-programming-and-       */
/*                scripting/26389-creating-file-1mb-using-         */
/*                shell-command.html                               */ 
/*******************************************************************/
/*******************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <math.h>

/******************************main function**********************************/
void main()
{
/***************command to create files with 20MB,50MB and 200MB data*********/
	system("cd ./inputfiles && dd if=/dev/zero of=file_20_mb.txt bs=20M count=1");
	system("cd ./inputfiles && dd if=/dev/zero of=file_50_mb.txt bs=50M count=1");
	system("cd ./inputfiles && dd if=/dev/zero of=file_200_mb.txt bs=200M count=1");	
}
