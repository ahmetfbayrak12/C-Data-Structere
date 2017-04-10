#variables
CC=gcc

CFLAG=-c -Wall


start: exp2
	$(CC) exp2.c -o exp2

exp2: exp2.c
	$(CC) $(CFLAG) exp2.c

#./exp2.o I=input_filename O=output_filename

run:
	./exp2 $(I) $(O)
#clear	
clean:
	rm -rf *o exp2	
	