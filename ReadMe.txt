										ReadMe.txt
									=============================	
Project Name 
=======================
Operating System Project4 - File System Measurements

Team Members
=======================
Sai Kiran Reddy Malikireddy
Kiranmayi Karri

Overview
=======================
In this project, we implemented functionality for measuring the parameters of file system. This functionality has been tested on LINUX machine.
For any further details , please refer to "OS_Project4.pdf"

Note: rdtsc.h file should be included in the folder from which the below codes are going to be executed. rdtsc.h is available in zip file.
 
To compile the codes 
=======================
use makefile (we have used gcc compiler)
'make' command

Note : Please ensure that the complete folder(not only the codes) is dumped while issuing make command, as redirected all the input/output files to the
respective folders.

In case, of any issue with make, please follow the below steps:
Step 1 :
Clean the previous execution results using 
	rm -f output/* inputfile/* temp_output/*
Step 2: Compilation and execution of the codes
File creation : Input file generation
		gcc -o filecreate file_create.c
	    ./filecreate.exe
Timer : Comparison of timers
		gcc -o timer timer.c
	    ./timer.exe > output/timer.txt

blocksize:
		gcc -o block_size block_size.c
	@for (( i = 0 ; i < 100; i = i + 1 )) do ./block_size.exe inputfiles/file_20_mb.txt > temp_output/blocksize$$i.txt;  done
	
prefetchsize: 
		gcc -o prefetch_size prefetch_size.c
		./prefetch_size.exe inputfiles/file_50_mb.txt 1  > output/prefetchsize_file1MB.txt
		./prefetch_size.exe inputfiles/file_50_mb.txt 2  > output/prefetchsize_file2MB.txt
		./prefetch_size.exe inputfiles/file_50_mb.txt 4  > output/prefetchsize_file4MB.txt

filecache: 
		gcc -o filecache_size filecache_size.c
		./filecache_size.exe inputfiles/file_200_mb.txt > output/filecache_size.txt

directpointer: 
		gcc -o direct_pointer direct_pointer.c
	rm -Rf inputfiles/direct_pointer_*.txt
	@for (( i = 0 ; i < 10; i = i + 1 )) do ./direct_pointer.exe inputfiles/direct_pointer_$$i.txt > temp_output/direct_pointer_$$i.txt;  done

merge: 
		gcc -o merge merge.c
		./merge.exe

To execute the codes 
=======================
Execution commands have been included in the make file. 

Note : 
Before the execution of prefetch_size.c program, clear the cache's using linux commands.
Before the every execution of the filecache_size.c program the file system should be unmounted and mounted to clear the file cache.
Ensure that direct_pointer.c program are executed on Ext2 file system as the direct pointer block concept is unavailable on Ext4.  

Results
=======================
Our test results are kept in 'Results' Folder. (Though the results are generated in output folder as said above, when we issue make command, the older files 
will be cleared)

Input files will be generated automatically and stored in 'inputfiles folder'.
All the output files will be stored at outputs folder.

Codes included :
=======================
timer.c - program to compare various timers to confirm the accuracy.
Inputs : not required 
Output : Can be found in /output/timer.txt

file_create.c - program to create input files which are to be used by the further programs(example: 1MB,20MB,30MB....files)
Inputs : not applicable
Ouputs : These files will be created and routed to /inputfiles folder.

block_size.c - program to measure the block size of filesystem.
Inputs : file_20_mb.txt
Outputs: 100 files with block sizes will be stored in temporary folder.

prefetch_size.c - program to measure prefetch size of file system.
Inputs : file_50_mb.txt 1 - first argument is the file name and 2 argument is the number of mega bytes of data.
Ouputs : 3 files with prefetch sizes. Can be found in /output/prefetch_size_1.txt, /output/prefetch_size_2.txt, /output/prefetch_size_4.txt

fiecache_size.c - program to measure file cache size of file system.
Inputs : file_200_mb.txt 
Ouputs : Can be found in /output/filecache_size.txt 

direct_pointer : program to calculate the filecache size.
Inputs :  will be generated using the program itself.
Outputs: As this program is repeated for 10 times to get the accuracy.

merge.c - program to merge the generated input files (buffer and direct pointers) and calculate the average.

test_blk.awk - this is awk script to combine all the block size output files into one and generate an average of these files.
test_dir.awk - this is awk script to combine all the direct pointer files into one and generate an average of these files.


Outputs File Description: 
=========================
blocksize_average.txt -  contains results of block size, first column are number of bytes read and second column represents time in microseconds.
prefetchsize_file1MB.txt -  contains results of prefetch size, first column are number of kilobytes read and second column represents time in microseconds.
filecache_size.txt -  contains results of file cache size, first column are number of blocks read and second column represents time in microseconds.
blocksize_average.txt -  contains results of number of direct pointer blocks, first column are number of blocks and second column represents time in microseconds.
