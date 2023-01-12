#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <time.h>
#include <sys/mman.h>
#include <assert.h>


long ts_to_long(struct timespec t){
	return t.tv_sec*1000000000 + t.tv_nsec;
} 

int main(int argc, char* argv[]) {
 
    int inputFd, outputFd;
    char *inputPtr, *outputPtr;

    //input read only
    inputFd = open (argv [1], O_RDONLY);
    if (inputFd == -1) {
        perror ("cannot open the file for read"); exit(1); }
 
    //output read/write（for mmap)
	outputFd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR| S_IWUSR);
    if(outputFd == -1){
        perror("canot open the file for write"); exit(1); }

    //lseek(0, seek_end)==fileSize
	ssize_t fileSize = 0,blockSize = 0;
    fileSize = lseek(inputFd, 0, SEEK_END);
	
	//remember to reset fileptr
    lseek(inputFd,0,SEEK_SET);
	ftruncate(outputFd,fileSize);

	inputPtr = mmap(NULL, fileSize, PROT_READ, MAP_SHARED , inputFd , 0);
	outputPtr = mmap(NULL, fileSize, PROT_WRITE, MAP_SHARED , outputFd , 0);
		
	struct timespec time_begin,time_end; 
	off_t data_off=0,hole_off=0,cur_off=0;
	long time_consume;	

 	clock_gettime(CLOCK_MONOTONIC,&time_begin);
	while(1){
		//case1 : data_off > hole_off 
		cur_off = lseek(inputFd,cur_off,SEEK_DATA);
		data_off = cur_off;
		//printf("data off : %-ld\n",data_off);
		cur_off = lseek(inputFd,cur_off,SEEK_HOLE);
		hole_off = cur_off;
		//printf("hole off : %-ld\n",hole_off);	
		
		//case2 : hole_off > data_off
		//actually,in this programming style,it never occurs.
		
		//the following is equal to:
		//if(lseek(outputFd,0,SEEK_CUR)==fileSize)break;
		if(cur_off == -1)
			break;
					
		blockSize = hole_off - data_off;	
		memcpy(outputPtr+data_off, inputPtr+data_off, blockSize);
		lseek(outputFd,cur_off,SEEK_SET);
		
	}
	clock_gettime(CLOCK_MONOTONIC,&time_end);
	printf("time_consume :%ld\n",ts_to_long(time_end)-ts_to_long(time_begin));
	
	assert(munmap(inputPtr,  fileSize) == 0);
    assert(munmap(outputPtr, fileSize) == 0);    
    assert(close (inputFd) == 0);
    assert(close (outputFd) == 0);

    return (EXIT_SUCCESS);
}
