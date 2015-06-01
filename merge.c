/*******************************************************************/
/*******************************************************************/
/*Filename 	:  merge.c                                         */
/*Description	:  This program combines all the output files      */
/*                 and execute programs to calculate the average   */
/*                 of block size and direct pointer usingsystem()  */
/*                 command.                                        */
/*Input         :  Takes input files from inputfile folder         */
/*                 blocksize*.txt, direct_pointer*.txt             */  
/*Output	:  Returns the file with average of files for      */
/*              :  block size and direct pointer                   */
/*Refernces	: https://www.cs.rutgers.edu/~pxk/416/notes/       */ 
/*                c-tutorials/gettime.html                         */
/*                http://linux.die.net/man/3/system                */
/*                http://www.mcs.anl.gov/~kazutomo/rdtsc.h         */ 
/*******************************************************************/
/*******************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/****************************main function***************************************/
void main()
{
/*************************Average calculation for blocksize**********************/
//command to concatenate all the 100 output files of blocksize*.txt
	system("cat temp_output/blocksize*.txt > temp_output/merge_blk.txt");
//sort the output file from the above	
	system("sort -k1 -n temp_output/merge_blk.txt > temp_output/combine_blk.txt");
//execute awk script to run calculate the avg
	system("cat test_blk.awk");
	system("bash test_blk.awk > temp_output/tmp_blk.txt");	
//sort the output file in increasing order	
	system("sort -k1 -n temp_output/tmp_blk.txt > output/blocksize_average.txt");

/*************************Average calculation for direct pointer******************/				
//command to concatenate all the 100 output files of blocksize*.txt				
	system("cat temp_output/direct_pointer*.txt > temp_output/merge_dir.txt");
//sort the output file from the above	
	system("sort -k1 -n temp_output/merge_dir.txt > temp_output/combine_dir.txt");
//execute awk script to run calculate the avg
	system("cat test_dir.awk");
	system("bash test_dir.awk > temp_output/tmp_dir.txt");	
//sort the output file in increasing order	
	system("sort -k1 -n temp_output/tmp_dir.txt > output/direct_pointer_average.txt");
	
}
