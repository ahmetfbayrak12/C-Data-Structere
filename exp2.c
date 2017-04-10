/* @Author
 * Student Name: Ahmet Faruk BAYRAK
 * Student ID : 21426716
 * Date: 23.11.2016 */


#include<stdio.h>
#include<stdlib.h>

#include<math.h>
#include<string.h>
#include<ctype.h>

typedef struct queue {			//queue for result
	int capacity;
	int size;
	int front;
	int rear;
	char **final_result;
} queue;
//function prototypes
int read_data(char *,char *);
int calculate(int,int,queue *Q,char *);
int integer_store(char *,queue *Q);
int syntax(char *,int);
void push_operand(char *,int);
void push_operator(char *,int);
int isintdigit(char *,int);
int isoperator(char *,int);
int ostack_count(void);
int opstack_count(void);
void evaluate(void);
int pows(int);
void pop_opstack(void);
int pop_ostack(void);
void display(int,queue *Q,char *);
int hex_store(char *,queue *Q);
int ishexdigit(char *,int);
void convert_hex_to_decimal(char[],int);
void convert_decimal_to_hex(char [],int);
void push_hex_operand(char ,int);
queue * createqueue(int);
void enqueue(queue *Q , char *);
int count_file_characters(char *);

struct ostack {                      //stack for operand
	int os_data;
	struct ostack *links;
}*operand = NULL;

struct opstack {					//stack for operator
	char ops_data;
	struct opstack *link;
}*operators = NULL;



int main(int argc, char *argv[] )  { 						//put input and output file in argument
   char *input_filename,*output_filename;
   if( argc == 3 ) {
   	input_filename = argv[1];
   	output_filename = argv[2];
	read_data(input_filename,output_filename);
}
   else if( argc > 3 ) {
      printf("Too many arguments supplied.\n");
   }
   else {
      printf("incomplete argument supplied.\n");
   }
}

int read_data(char *input_filename,char *output_filename) {                               //read_inputfile();
	FILE *input_file;
	int count=0;								//locate the line in the file
	char *line;
	/*count the number where print is called*/
	int p_count=0;
	queue *Q = createqueue(100);				//create queue
	int line_size;
	line_size=count_file_characters(input_filename);					        
	line = (char *)malloc(line_size * sizeof(char));                      
	input_file = fopen(input_filename,"r");   		//retrieve the content of input.txt
	if(input_file == NULL) {
		printf("\nThere is no input file");     
		return(0);								
	} else {
		while((fgets(line, line_size, (FILE *)input_file)) != NULL) {  //read an entire line

			count++;
			if(line[0] == 'c') {                    //check if the instruction is calculate or print
				calculate(count,line_size,Q,input_filename);			
			} else if(line[0] == 'p'){
				p_count++;
				display(p_count,Q,output_filename);						
			}
		}
		fclose(input_file);
	}
	return 0;
}

int calculate(int count,int line_size,queue *Q,char *input_filename) {		// calculate executed
	/*point the line of the file*/
	int point,hex_in_decimal;
	char *line,*test;
	char integer_result[] = "integer ";
	char hex_result[] = "hex ";
	char result[50];
	line = (char *)malloc(line_size * sizeof(char));
	test = (char *)malloc(line_size * sizeof(char));
	operators = NULL;
	operand = NULL;
	FILE *input_file2;
	input_file2 = fopen(input_filename,"r");
	for(point = 0 ; point < count ; point++) {				//reading the line determined by count
		fgets(line, line_size, (FILE *)input_file2);
	}
		
	/*separating words*/
	test=strtok(line," ");
	/*going through each word*/
	while(test != NULL) {
		/*check first word*/
		if((strcmp(test,"calculate")) == 0) {
			test=strtok(NULL," ");
			/*check for hex*/
			if((strcmp(test,"hex")) == 0) {
				/*get hex expression*/
				test=strtok(NULL," ");
				/*calculate hex expression*/
				if(hex_store(test,Q) != 0) {					//if no error in hex calculation
					hex_in_decimal=operand->os_data;
					if(hex_in_decimal < 0) {
						hex_in_decimal=0-hex_in_decimal;
						strcat(hex_result,"-");
					}
					sprintf(result,"%X",hex_in_decimal);
					//strupr(result);
					strcat(hex_result,result);
					enqueue(Q,hex_result);
				}
			}
			/*check for integer*/
			else if((strcmp(test,"integer")) == 0) {
				/*get integer expression*/
				test=strtok(NULL," ");
				/*calculate integer expression*/
				if(integer_store(test,Q) != 0 ) {				//if no error in integer calculation
					sprintf(result,"%d",operand->os_data);
					strcat(integer_result,result);
					enqueue(Q,integer_result);
				}
			}
			/*if wrong instruction*/
			else {
				enqueue(Q,"error");
				fclose(input_file2);
				return 0;
			}
		} else
			test=strtok(NULL," ");
	}
	fclose(input_file2);
	return 0;
}


int integer_store(char *test,queue *Q) {		//for integer problem
	int index;
	int lp,rp;						//counter for left and right parentheses ()
	lp=0;
	rp=0;
	if(syntax(test,1) == 1) {						//operator syntax error				
		enqueue(Q,"error");
		return 0;
	} else {
		for(index=1; index<strlen(test)-2; index++) {	//scan each character
			if( ((test[index] == ')') && (index == strlen(test)-3) && (lp > rp+1))) {	//if parentheses not equal
				enqueue(Q,"error");
				return 0;
			
			}else if(test[1] == '-' && index == 1){
					index++;
					push_operand(test,index);
			} else if(test[index] == '(') {					
				lp++;
				if(syntax(test,index) != 1)
				push_operator(test,index);
				else{
					enqueue(Q,"error");
				}
			} else if(test[index] == ')') {					
				rp++;
				if(syntax(test,index) != 1 && rp <= lp) {
					push_operator(test,index);
				} else {
					enqueue(Q,"error");
					return 0;
				}
			} else if(isintdigit(test,index) == 1) {	
				push_operand(test,index);
			} else if(isoperator(test,index) == 1) {	
				if(syntax(test,index) != 1)
				{
						push_operator(test,index);
				}
				else{
					enqueue(Q,"error");
					return 0;
				}
			}

			else {							//other symbols
				enqueue(Q,"error");
				return 0;
			}
		}
		if(test[1] != '(') {						//if no brackets used in the beginning
			while (opstack_count() != 0) {
				evaluate();
			}
		} else {									//if brackets used in the beginning
			while (operators->ops_data != '(') {
				evaluate();
			}
		}
		return 1;
	}

}

int syntax(char *test,int index) {
	if(index == 1) {	//test at start
		if(test[1] == ')' || test[1] == '*' || test[1] == '/' || (test[1] == '(' && test[2] == ')') || (test[1] == '+' && test[2] == '+') || (test[1] == '-' && test[2] == '-') || (test[1] == '+' && test[2] == '*') || (test[1] == '+' && test[2] == '/') || (test[1] == '-' && test[2] == '*') || (test[1] == '-' && test[2] == '/') ) {
			return 1;
		} else
			return 0;
	} else if(test[index] == '(' && test[index+1] ==')')
		return 1;
	else if(test[index] == ')' && test[index+1] =='(')
		return 1;
	else if(((test[index] == '*' && test[index+1] == '+' && isintdigit(test,index+2) ==0 ) || (test[index] == '*' && test[index+1] == '-' && isintdigit(test,index+2) == 0 ) || (test[index] == '/' && test[index+1] == '+' && isintdigit(test,index+2) == 0 ) || (test[index] == '/' && test[index+1] == '-' && isintdigit(test,index+2) == 0 ) || (test[index] == '*' && test[index+1] == '*' ) || (test[index] == '/' && test[index+1] == '/' ) || (test[index] == '+' && test[index+1] == '*' ) || (test[index] == '+' && test[index+1] == '/' ) || (test[index] == '-' && test[index+1] == '*' ) || (test[index] == '-' && test[index+1] == '/' )))
		return 1;
	else if((test[index] == '+' || test[index] == '-') && (test[index+1] == '+' || test[index+1] == '-') && (test[index+2] == '+' || test[index+2] == '-'))
	return 1;
	else
		return 0;
}

void push_operand(char *test,int index) {			//push operand in the operand stack
	struct ostack *temp;
	int top_number,negative_position;
	temp = (struct ostack*)malloc(sizeof(struct ostack));  					
	if(index == 2 && test[1] == '-') {								//making the operand negative if found at the start after - sign
		temp->os_data = -(test[index]%48);									//converting char into int
		temp->links = operand;
		operand = temp;
	} else if((isintdigit(test,index-1) == 1)) {							//if series of numbers
		temp = operand;
		top_number = temp->os_data;
		operand=operand->links;
		if(top_number<0)
			temp->os_data = (top_number*10)-(test[index]%48);				//putting the series of numbers as a single one
		else
			temp->os_data = (top_number*10)+(test[index]%48);
		temp->links = operand;
		operand = temp;
	}
	/*converting into negative numbers if found any*/
	else if((test[index-1] == '+' || test[index-1] == '-') && opstack_count() != 0 && operators->ops_data =='-') {
		operators->ops_data = '+';
		temp->os_data = -(test[index]%48);
		temp->links = operand;
		operand = temp;
	} else {
		temp->os_data = (test[index]%48);
		temp->links = operand;
		operand = temp;
	}
}


void push_operator(char *test,int index) {                    //push operator in the operator stack
	struct opstack *hold;
	struct ostack *temp;
	int negative;
	hold = (struct opstack*)malloc(sizeof(struct opstack));  					//new stack
	if(opstack_count() > 0) {													//if stack is not empty
		if(test[index] == ')') {
			if(operators->ops_data == '(' && opstack_count() > 1) {
				pop_opstack();
			} else {
				hold->ops_data = test[index];
				hold->link = operators;
				operators = hold;
				evaluate();
				if(opstack_count() != 0 && operators->ops_data == '-')
				{
					operand->os_data = 0 - operand->os_data;
					operators->ops_data = '+';
				}					
			}
		}
		else if( (operators->ops_data == '/' || operators->ops_data == '*') && (test[index-1] != '/' || test[index-1] != '*') && test[index] == '-' ) {
			evaluate();
			hold->ops_data = test[index];
			hold->link = operators;
			operators = hold;
		}
		else if( (operators->ops_data == '/' || operators->ops_data == '*') && test[index-1] != '/' && test[index-1] != '*' ) {
			evaluate();
			hold->ops_data = test[index];
			hold->link = operators;
			operators = hold;
		}
		//performing the properties of - sign
		else if(test[index] == '-') {
			if(test[index-1] == '-' && operators->ops_data == '-') {
				pop_opstack();
				hold->ops_data = '+';
				hold->link = operators;
				operators = hold;
			} else if(test[index-1] == '+' && operators->ops_data == '+') {
				pop_opstack();
				hold->ops_data = test[index];
				hold->link = operators;
				operators = hold;
			} else {
				hold->ops_data = test[index];
				hold->link = operators;
				operators = hold;
			}
		} 
		//performing the properties of + sign
		else if(test[index] == '+') {							
			if(test[index-1] != '+' && test[index-1] != '-' && (test[index-1] != '*' && test[index-1] != '/')) {
				hold->ops_data = test[index];
				hold->link = operators;
				operators = hold;
			}
		} else if(test[index] == '(') {
			hold->ops_data = test[index];
			hold->link = operators;
			operators = hold;
		} else {
			hold->ops_data = test[index];
			hold->link = operators;
			operators = hold;
		}
	} else {				//if empty stack
		hold->ops_data = test[index];
		hold->link = operators;
		operators = hold;
	}
}

int isintdigit(char *test,int index) {     //checking for integer
	if(test[index] == '0' || test[index] == '1' || test[index] == '2' || test[index] == '3' || test[index] == '4' || test[index] == '5' || test[index] == '6' || test[index] == '7' || test[index] == '8' || test[index] == '9')
		return 1;
	else
		return 0;
}

int isoperator(char *test,int index) {      //checking for operators
	if(test[index] == '+' || test[index] == '-' || test[index] == '*' || test[index] == '/')
		return 1;
	else
		return 0;
}
int ostack_count(){							//count operand stack
	int count = 0;
	struct ostack *temp;
	temp = operand;
	while (temp != NULL) {
		temp = temp->links;
		count++;
	}
	return count;
}
int opstack_count() {                  		//count operator stack
	int count = 0;
	struct opstack *temp;
	temp = operators;
	while (temp != NULL) {
		temp = temp->link;
		count++;
	}
	return count;
}

void evaluate() {							//actual calculation
	struct ostack *temp;
	int calculation_test=0;
	int o1,o2;         //operands for calculation
	if(opstack_count() != 0) {
		temp = (struct ostack*)malloc(sizeof(struct ostack));
		switch(operators->ops_data) {									//calculation as per the operator
			case '/':
				o1=pop_ostack();
				o2 = pop_ostack();
				o1 = o2/o1;
				pop_opstack();
				calculation_test++;
				break;
			case '*':
				o1=pop_ostack();
				o2 = pop_ostack();
				o1 = o2*o1;
				pop_opstack();
				calculation_test++;
				break;
			case '+':
				o1=pop_ostack();
				o2 = pop_ostack();
				o1 = o2+o1;
				pop_opstack();
				calculation_test++;
				break;
			case '-':
				o1=pop_ostack();
				o2 = pop_ostack();
				o1 = o2-o1;
				pop_opstack();
				calculation_test++;
				break;
			case ')':
				pop_opstack();
				while(operators->ops_data != '(') 			//continue until ( is seen
					evaluate();
				if(opstack_count() > 1)
					pop_opstack();
				break;
			default :
				break;
		}
		if(calculation_test != 0) {
			temp->os_data = o1;
			temp->links = operand;
			operand = temp;
		}
	}
}

int pop_ostack() {										//remove top element of operand stack
	struct ostack *temp;
	int top_data;
	if (operand != NULL) {
		temp = operand;
		top_data = temp->os_data;
		operand = operand->links;
		free(temp);
		return top_data;
	} else
		return 0;
}

void pop_opstack() {									//remove top element of operators stack
	struct opstack *temp;
	if (operators != NULL) {
		temp = operators;
		operators = operators->link;
		free(temp);
	}
}

void display(int p_count,queue *Q,char *output_filename) {				//put the result in the output.txt file and pop queue
	FILE *output_file;
	int point;
	char *result;
	char new_line[]="\n";
	if(p_count == 1) {
		output_file=fopen(output_filename,"w");        //output.txt is the name of output file which will be made in the same directory
		while(Q->size != 0) {
			result=Q->final_result[Q->front];
			printf("%s\n",result);
			fputs(result,output_file);
			fputs(new_line,output_file);
			Q->size--;
			Q->front++;
			if(Q->front==Q->capacity) {
				Q->front=0;
			}
		}
		fclose(output_file);
	} else {
		output_file=fopen(output_filename,"a");        
		while(Q->size != 0) {
			result=Q->final_result[Q->front];
			printf("%s\n",result);
			fputs(result,output_file);
			fputs(new_line,output_file);
			Q->size--;
			Q->front++;
			if(Q->front==Q->capacity) {
				Q->front=0;
			}
		}
		fclose(output_file);
	}
}

int hex_store(char *test,queue *Q) {					//for hex problem
	int index,negative=0;
	int lp = 0,rp = 0 ,hexdigit_counter,decimal_counter;
	char hex_list[50];							
	if(syntax(test,1) == 1) {
		return 0;
		enqueue(Q,"error");
		return 0;
	} else {
		for(index=1; index<strlen(test)-2; index++) {
			if( ((test[index] == ')') && (index == strlen(test)-3) && (lp > rp+1))) {
				enqueue(Q,"error");
				return 0;
			} else if(test[index] == '(') {
				lp++;
				if(syntax(test,index) != 1)
				push_operator(test,index);
				else{
					enqueue(Q,"error");
					return 0;
				}
			} else if(test[index] == ')') {
				rp++;
				if(syntax(test,index) != 1 && rp <= lp) {
					push_operator(test,index);
				} else {
					enqueue(Q,"error");
					return 0;
				}
			} else if(ishexdigit(test,index) == 1) {	
				hexdigit_counter = 0;
				if(test[1] == '-' && index == 2 ) {			//- sign at the beginning
					negative = 1;
				}
				if( (test[index-1] == '+' || test[index-1] == '-') && opstack_count() != 0 && operators->ops_data =='-') { //negative in between
					negative=2;
				}
				hex_list[hexdigit_counter] = toupper(test[index]);
				hexdigit_counter++;
				while(ishexdigit(test,index+1) != 0) {				//collecting all hex digits
					index++;
					hex_list[hexdigit_counter] = toupper(test[index]);
					hexdigit_counter++;
				}
				convert_hex_to_decimal(hex_list,hexdigit_counter);		//converting hex digits into decimal
				decimal_counter = strlen(hex_list)+1;
				for(hexdigit_counter = 0; hexdigit_counter < strlen(hex_list); hexdigit_counter++) {
					decimal_counter--;
					if(negative == 1) {
						push_hex_operand(hex_list[hexdigit_counter],0);
						negative = 0;
					} else if(negative == 2) {
						push_hex_operand(hex_list[hexdigit_counter],-2);
						negative = 0;
					} else if(strlen(hex_list) == 1 || hexdigit_counter == 0) {
						push_hex_operand(hex_list[hexdigit_counter],-1);
					} else {
						push_hex_operand(hex_list[hexdigit_counter],decimal_counter);
					}
			}
			} else if(isoperator(test,index) == 1) {	
				if(syntax(test,index) != 1)
				{
						push_operator(test,index);
				}
				else{
					enqueue(Q,"error");
					return 0;
				}
			}	

			else {
				enqueue(Q,"error");
				return 0;
			}
			
		}
		if(test[1] != '(') {
			while (opstack_count() != 0) {
				evaluate();
			}
		} else {
			while (operators->ops_data != '(') {
				evaluate();
			}
		}
		return 1;
	}

}

int ishexdigit(char *test,int index) { 				//checking hex digit
	char hexchar;
	hexchar = toupper(test[index]);
	if(hexchar == '0' || hexchar == '1' || hexchar == '2' || hexchar == '3' || hexchar == '4' || hexchar == '5' || hexchar == '6' || hexchar == '7' || hexchar == '8' || hexchar == '9' || hexchar == 'A' || hexchar == 'B' || hexchar == 'C' || hexchar == 'D' || hexchar == 'E' || hexchar == 'F')
		return 1;
	else
		return 0;
}

void convert_hex_to_decimal(char hex_list[50],int hexdigit_counter) {		// Converting hex into decimal number
	int i,j,k,decimal_number=0;
	int temp_counter=0,power=0;
	char hex_check[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
	                      '9', 'A', 'B', 'C', 'D', 'E', 'F'
	                     };
	for(i=hexdigit_counter-1; i >= 0; i--) {			
		for(j=0; j<16; j++) {
			if(hex_list[i] == hex_check[j]) {
				decimal_number += j*pows(power);
				//decimal_number += j*(16^power); this is not needed 
			}
		}
		power++;
	}
	sprintf(hex_list,"%d",decimal_number);
}

int pows(int power)
{
	int result = 1;
	 while (power != 0)
    {
        result *= 16;
        --power;
    }
    return result;
}

void push_hex_operand(char test,int decimal_counter) {				//push hex digits in operand stack
	struct ostack *temp;
	int top_number,negative_position;
	temp = (struct ostack*)malloc(sizeof(struct ostack));  					
	if(decimal_counter == 0) {								        //making the operand negative if found at the start
		temp->os_data = -(test%48);									//converting char into int
		temp->links = operand;
		operand = temp;
	} else if(decimal_counter > 0) {							    //if series of numbers
		if(operand != NULL) {
			temp = operand;
			top_number = temp->os_data;
			operand=operand->links;
			if(top_number<0)
				temp->os_data = (top_number*10)-(test%48);
			else
				temp->os_data = (top_number*10)+(test%48);
			temp->links = operand;
			operand = temp;
		} else {
			temp->os_data =(test%48);
			temp->links = operand;
			operand = temp;
		}
	}
	/*converting into negative numbers if found any*/
	else if(decimal_counter == -2) {
		operators->ops_data = '+';
		temp->os_data = -(test%48);
		temp->links = operand;
		operand = temp;
	} else {
		temp->os_data = (test%48);
		temp->links = operand;
		operand = temp;
	}
}

queue * createqueue(int maxElements) {
	/* Create a queue */
	queue *Q;
	Q = (queue *)malloc(sizeof(queue));
	Q->final_result = (char**)malloc(sizeof(char*)*maxElements);
	Q->size = 0;
	Q->capacity = maxElements;
	Q->front = 0;
	Q->rear = -1;
	return Q;

}
void enqueue(queue *Q , char *result) {
	/*insert result*/
	if(Q->size != Q->capacity) {
		Q->size++;
		Q->rear = Q->rear + 1;
		Q->final_result[Q->rear] = (char *) malloc((sizeof result + 1)* sizeof(char));
		strcpy(Q->final_result[Q->rear], result);
	}
}

int count_file_characters(char *input_filename)  	//get the size of the file
{
	int count = 0;   
    FILE *input_file;  
    int ch;
    input_file = fopen(input_filename, "r");
    while (1) {
        ch = fgetc(input_file);
        if (ch == EOF)
            break;
        ++count;
    }
	fclose(input_file);
    return (count);
}
