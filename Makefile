CC = gcc -std=c99 -w
SHELL = /bin/bash
FILELOC = ./inputfiles

test: cleanresults filecreate timer blocksize prefetchsize filecache directpointer merge

cleanresults:
	rm -f output/* inputfile/* temp_output/*

filecreate: filecreate.exe
	-./filecreate.exe

timer: timer.exe
	-./timer.exe > output/timer.txt

blocksize: block_size.exe
	@for (( i = 0 ; i < 100; i = i + 1 )) do ./block_size.exe $(FILELOC)/file_20_mb.txt > temp_output/blocksize$$i.txt;  done
	
prefetchsize: prefetch_size.exe
	-./prefetch_size.exe $(FILELOC)/file_50_mb.txt 1  > output/prefetchsize_file1MB.txt
	-./prefetch_size.exe $(FILELOC)/file_50_mb.txt 2  > output/prefetchsize_file2MB.txt
	-./prefetch_size.exe $(FILELOC)/file_50_mb.txt 4  > output/prefetchsize_file4MB.txt

filecache: filecache_size.exe
	-./filecache_size.exe $(FILELOC)/file_200_mb.txt > output/filecache_size.txt
	
directpointer: direct_pointer.exe
	rm -Rf $(FILELOC)/direct_pointer_*.txt
	@for (( i = 0 ; i < 10; i = i + 1 )) do ./direct_pointer.exe $(FILELOC)/direct_pointer_$$i.txt > temp_output/direct_pointer_$$i.txt;  done

merge: merge.exe
	-./merge.exe

filecreate.exe: file_create.c
	$(CC) -o $@ $<
	chmod 0755 filecreate.exe

timer.exe: timer.c
	$(CC) -o $@ $<
	chmod 0755 timer.exe

block_size.exe: block_size.c
	$(CC) -o $@ $<
	chmod 0755 block_size.exe

prefetch_size.exe: prefetch_size.c
	$(CC) -o $@ $<
	chmod 0755 prefetch_size.exe

filecache_size.exe: filecache_size.c
	$(CC) -o $@ $<
	chmod 0755 filecache_size.exe

direct_pointer.exe: direct_pointer.c
	$(CC) -o $@ $<
	chmod 0755 direct_pointer.exe

merge.exe: merge.c
	$(CC) -o $@ $<
	chmod 0755 merge.exe

