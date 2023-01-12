#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>

#define TWENTY_MB 20000000

int main(int argc,char* argv[]){
	FILE* input  = fopen(argv[1],"r");
	FILE* output = fopen(argv[2],"w+");
	
	char word_buffer[81];
	int  word_buffer_size = 81;
	
	char* buffer;
	int line_position;
	int buffer_cursor;

	int your_input_size = 0;
	sscanf(argv[3],"%d",&your_input_size);

	//set the file size == 0
	assert(ftruncate(fileno(output),0) == 0);	

	if(your_input_size > 20000000){
		printf("buffer size must be smaller than 20MB\n");
		exit(EXIT_FAILURE);
	}
	
	//unbuffered
	if(your_input_size == 0){
		printf("unbuffered\n");
		assert(setvbuf(input ,NULL,_IONBF,0) == 0);
		assert(setvbuf(output,NULL,_IONBF,0) == 0);
	}	
	//line buffered 
	else if(your_input_size == -1){
		printf("line buffered\n");
		buffer = (char*)malloc(your_input_size);
		assert(setvbuf(input ,buffer,_IOLBF,81) == 0);
		assert(setvbuf(output,buffer,_IOLBF,81) == 0);
	}	
	//fully buffered 
	else{
		printf("fully buffered size %d\n",your_input_size);
		buffer = (char*)malloc(your_input_size);
		assert(setvbuf(input ,buffer,_IOFBF,your_input_size) == 0);
		assert(setvbuf(output,buffer,_IOFBF,your_input_size) == 0);

	}
	printf("setvbuf\n");
		
	int count = 0;
	int input_int = 0;

    while(1){
        count ++;
        input_int = getc(input);

        if(input_int == EOF) {
            word_buffer[buffer_cursor]= '\0';
            fprintf(output, "%s", word_buffer);
            break;
        }

        //turn int into char 
        unsigned char input_char = (unsigned char) input_int;
        if(input_char == '\n') {
			printf("line_position:%d buffer_cursor:%d",line_position,buffer_cursor);
            if(line_position + buffer_cursor > 80){
                fseek(output, -1, SEEK_CUR); 		//delete blank
                fprintf(output,"\n");
                line_position = 0;
            }
			else{
            	word_buffer[buffer_cursor] = '\0';
            	fprintf(output, "%s\n", word_buffer);
            	buffer_cursor = 0;
            	line_position = 0;
            }
			continue;			
        }

        if(input_char == ' '){
            printf("line_position:%d bufferCursor:%d\n",line_position,buffer_cursor);
            if(line_position + buffer_cursor > 80){
                fseek(output, -1, SEEK_CUR);		//remove ' '
                fprintf(output, "\n");
                line_position = 0;
            }

            word_buffer[buffer_cursor] = ' ';
       		buffer_cursor++;
            word_buffer[buffer_cursor] = '\0';

            fprintf(output, "%s", word_buffer);
            line_position += buffer_cursor;

            // deal with next word
            buffer_cursor = 0;
            printf("linePos + bufferCursor: %d\n", line_position + buffer_cursor);
            continue;
        }

        word_buffer[buffer_cursor] = input_char;
        buffer_cursor += 1;
        
    }

    free(buffer);
	return 0;
}
