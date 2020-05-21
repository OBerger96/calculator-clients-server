#define STR_SIZE (150)
#define DECIMAL (10)
#define WAITING_TIME (60)

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/random.h>

// *********************************************************************************************************************************
// *********************************************************************************************************************************
// 														Function declaration
// *********************************************************************************************************************************
// *********************************************************************************************************************************

//Read from to_srv.txt file and write the message to to_client_XXXXX.txt file
void sigusr1Handler(int sig);

//Print "Time Out" message 
void alrmHandler(int sig);

//kill all the children processes
void killAllChildrenProcessese();

// Appends a copy of the str2 string to the str1 string.
void myStrcat(char* str1, char* str2);

// Returns length of the string.
int myStrlen(char* str);

//Reverse the string
void myReverseString(char* str);

// Converts a string and returns it's value of type int
int myAtoi(char* str);

// Converts an int and returns it's value of type string
void myItoa(int num, char * str, int base);

// Fills memory with a '\0'.
void myMemset(char* str);

//
void readFromToServerFile(char* to_client_path,pid_t* pid, int* leftOperand, int* myOperator, int* rightOperand);

//
int calculate (int* leftOperand, int* myOperator, int* rightOperand);

//Write the result to to_client_XXXXX.txt file
void writeToClientFile(char* to_client_path, int* result);

//Read data (client's pid & expression) from to_srv.txt file
void myReadLine(int fd_to_server, char * buffer);

// Close & delete to_client_XXXXX.txt file
void deleteToClientFile(pid_t pid_client);

//Clean up all the data if something goes wrong
void cleanUp();

// *********************************************************************************************************************************
// *********************************************************************************************************************************
//															Main.
// *********************************************************************************************************************************
// *********************************************************************************************************************************

int childCounter = 0;

int main(int argc, char* argv[])
{

	// Check the number of arguments
	if (argc != 1)
	{
		printf("\nERROR_FROM_EX2\n");
		//printf("Error occurred with the number of arguments: the number is uncorrect...");
		exit(-1);
	}

	//Delete to_srv.txt file
	unlink("to_srv.txt");

	printf("\nServer: End of stage 5.c\n");

	//	Signal registration
	signal(SIGUSR1, sigusr1Handler);
	signal(SIGALRM, alrmHandler);

	alarm(WAITING_TIME);


	do
	{
		pause();
	} while (1);


	printf("\nServer is done\n");
	exit(0);
}



void sigusr1Handler(int sig)
{

	pid_t pid_server = 0;

	if ((pid_server = fork()) < 0)
	{
		// Failed to create child process
		cleanUp();
	}
	//Child process
	if (pid_server == 0)
	{
		printf("\nServer: End of stage 5.f\n");

		char to_client_path[STR_SIZE] = "to_client_";
		
		pid_t pid_client = 0;

		int leftOperand = 0;
		int myOperator=0;
		int rightOperand = 0;
		int result = 0;

		printf("\nServer: End of stage 5.b\n");

		//Read and asign data from to_srv.txt file
		readFromToServerFile(to_client_path, &pid_client, &leftOperand, &myOperator, &rightOperand);
		
		printf("\nServer: End of stage 5.g\n");

		//Calculate the client's expression
		result = calculate(&leftOperand, &myOperator, &rightOperand);

		//Write the result to the to_client_XXXXX.txt file
		writeToClientFile(to_client_path, &result);
		
		if (kill(pid_client, SIGUSR1) == -1)
		{
			deleteToClientFile(pid_client);
			cleanUp();
		}


		printf("\nServer: End of stage 5.i\n");
		printf("\nServer: End of stage 5.a\n");

		exit(0);
	}

	//Parent process
	if (pid_server > 0)
	{
		//	Signal registration
		signal(SIGUSR1, sigusr1Handler);

		// Restart the timer
		alarm(WAITING_TIME);

		childCounter++;
		wait(NULL);
		//printf("\nparent kill child\n");
		childCounter--;
		return;
	}
}

void alrmHandler(int sig)
{
	// Ignores SIGUSR1 from now all
	signal(SIGUSR1, SIG_IGN);

	killAllChildrenProcessese();

	printf("\nThe server was closed because no service request was received for the last 60 seconds\n");
	printf("\nServer is done\n");
	exit(0);
}

void killAllChildrenProcessese()
{		
	
	signal(SIGUSR1, SIG_IGN);

	for (int i = 0; i < childCounter; i++)
	{
		if (wait(NULL) == -1)
		{

			printf("\nERROR_FROM_EX2\n");
		//	printf("Error occurred attempting to kill child process...\n");
			exit(-1);
		}
	}

	childCounter = 0;
}

void myStrcat(char* str1, char* str2)
{
	char* start = str1;

	while (*str1 != '\0')
	{
		str1++;
	}

	while (*str2 != '\0')
	{
		*str1 = *str2;
		str1++;
		str2++;
	}

	*str1 = '\0';
}

int myStrlen(char* str)
{
	int len = 0;

	while (str[len] != '\0')
	{
		len++;
	}

	return (len);
}

void myReverseString(char* str)
{
	int l;
	int i;
	char * begin_ptr;
	char * end_ptr;
	char temp;

	l = myStrlen(str);

	// Set the begin_ptr and end_ptr 
	begin_ptr = str;
	end_ptr = str;

	for (i = 0; i < l - 1; i++) { end_ptr++; }

	// Swap the char from start and end 
	// index using begin_ptr and end_ptr 
	for (i = 0; i < l / 2; i++) {

		temp = *end_ptr;
		*end_ptr = *begin_ptr;
		*begin_ptr = temp;

		// update pointers positions 
		begin_ptr++;
		end_ptr--;
	}
}

int myAtoi(char* str)
{
	int result = 0;
	int sign = 1;
	int i = 0;

	if (str == NULL) { return 0; }

	// If number is negative, then update sign 
	if (str[0] == '-') {
		sign = -1;
		i++;
	}

	for (; str[i] != '\0'; ++i)
		result = result * 10 + str[i] - '0';

	return sign * result;
}

void myItoa(int num, char * str, int base)
{
	int i = 0;
	int isNegative = 0;

	if (num == 0)
	{
		str[i++] = '0';
		str[i] = '\0';
		//return str;
	}

	if (num < 0 && base == 10)
	{
		isNegative = 1;
		num = -num;
	}

	// Process individual digits 
	while (num != 0)
	{
		int rem = num % base;
		str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num = num / base;
	}

	// If number is negative, append '-' 
	if (isNegative == 1) { str[i++] = '-'; }

	str[i] = '\0'; // Append string terminator 

	// Reverse the string 
	myReverseString(str);
}

void myMemset(char* str)
{

	while (*str)
	{
		*str++ = '\0';
	}
}

void readFromToServerFile(char* to_client_path, pid_t* pid, int* leftOperand, int* myOperator, int* rightOperand)
{
	char to_srv_path[STR_SIZE] = "to_srv.txt";
	char buffer[STR_SIZE] = { 0 };

	int fd_to_server = -1;

	if ((fd_to_server = open(to_srv_path, O_RDONLY, 0666)) < 0)
	{
		cleanUp();
	}

	//Read client's pid
	myReadLine(fd_to_server, buffer);

	*pid = myAtoi(buffer);

	// Creates the path to the to_client_XXXXX.txt file
	myStrcat(to_client_path, buffer);
	if (to_client_path[0] == '\0')
	{
		close(fd_to_server);
		cleanUp();
	}

	myStrcat(to_client_path, ".txt");
	if (to_client_path[0] == '\0')
	{
		close(fd_to_server);
		cleanUp();
	}

	myMemset(buffer);

	//Read left Operand
	myReadLine(fd_to_server, buffer);
	*leftOperand = myAtoi(buffer);
	myMemset(buffer);

	//Read operator
	myReadLine(fd_to_server, buffer);
	*myOperator = myAtoi(buffer);
	myMemset(buffer);

	//Read right Operand
	myReadLine(fd_to_server, buffer);
	*rightOperand = myAtoi(buffer);
	myMemset(buffer);

	close(fd_to_server);

	if (unlink(to_srv_path) < 0)
	{
		cleanUp();
	}
}

int calculate(int* leftOperand, int* myOperator, int* rightOperand)
{
	switch (*myOperator)
	{
	case 1:
		return (*leftOperand + *rightOperand);
	case 2:
		return (*leftOperand - *rightOperand);
	case 3:
		return (*leftOperand * *rightOperand);
	case 4:
		if (*rightOperand == 0)
		{
			printf("\nError occurred attempting to divide by zero...\n");
			exit(0);
		}
		return (*leftOperand / *rightOperand);
	}
}

void writeToClientFile(char* to_client_path, int* result)
{
	int fd_to_client = -1;
	char buffer[STR_SIZE] = { 0 };

	if ((fd_to_client = open(to_client_path, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) 
	{  
		cleanUp();
	}

	printf("\nServer: End of stage 5.h\n");

	myItoa(*result, buffer, DECIMAL);

	if (write(fd_to_client, buffer, myStrlen(buffer)) < 0)
	{
		close(fd_to_client);
		cleanUp();
	}

	close(fd_to_client);
}

void myReadLine(int fd_to_server, char * buffer)
{
	char c;
	int i = 0;
	while (1)
	{
		if (read(fd_to_server, &c, sizeof(c)) < 0)
		{
			close(fd_to_server);
			cleanUp();
		}
		if (c == '\n') { buffer[i] = '\0'; break; }

		buffer[i++] = c;
	}

}

void cleanUp()
{
	printf("\nERROR_FROM_EX2\n");
	killAllChildrenProcessese();
	exit(-1);
}

void deleteToClientFile(pid_t pid_client)
{
	char to_client_path[STR_SIZE] = "to_client_";
	char pid_str[STR_SIZE] = { 0 };

	myItoa(pid_client, pid_str, DECIMAL);

	// Creates the path to the to_client_XXXX.txt file
	myStrcat(to_client_path, pid_str);

	if (to_client_path[0] == '\0')
	{
		cleanUp();
	}

	myStrcat(to_client_path, ".txt");

	if (to_client_path[0] == '\0')
	{
		cleanUp();
	}

	unlink(to_client_path);
}
