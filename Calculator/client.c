#define NUM_OF_TRIES (10)
#define STR_SIZE (150)
#define DECIMAL (10)
#define WAITING_TIME (30)


#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/random.h>

// *********************************************************************************************************************************
// *********************************************************************************************************************************
// 														Function declaration
// *********************************************************************************************************************************
// *********************************************************************************************************************************

// Returns random number (0-5) as the client's latency
int getLatency();

// Converts a string and returns it's value of type int
int myAtoi(char* str);

// Converts an int and returns it's value of type string
void myItoa(int num, char * str ,int base);

// Returns length of the string.
int myStrlen(char* str);

// Determines whether a string represents an integer or not (1->valid number, 0->not)
int isValidNumber(char* str);

// Appends a copy of the str2 string to the str1 string.
void myStrcat(char* str1, char* str2);

//Reverse the string
void myReverseString(char* str);

// Read from to_client_XXXXX.txt file and prints result message
void sigusr1Handler(int sig);

//Prints 'TimeOut' message 
void alrmHandler(int sig);

////Write data (client's pid & expression) to to_srv.txt file
void writeToServerFile(int argc, char* argv []);


// *********************************************************************************************************************************
// *********************************************************************************************************************************
//															Main.
// *********************************************************************************************************************************
// *********************************************************************************************************************************

int main(int argc, char* argv[])
{
	// Check the number of arguments
	if (argc != 5)
	{
		printf("\nERROR_FROM_EX2\n");
		//printf("Error occurred with the number of arguments: the number is uncorrect...");
		exit(-1);
	}

	//Check if all the arguments are valid numbers
	for (int i = 1; i < argc; i++)
	{
		if (isValidNumber(argv[i]) == 0)
		{
			printf("\nERROR_FROM_EX2\n");
			//printf("Error occurred with the argument %d is not a valid number...\n", i);
			exit(-1);
		}
	}

	// Check if the operator is valid
	if ((myAtoi(argv[3]) < 1) || (myAtoi(argv[3]) > 4))
	{
		printf("\nERROR_FROM_EX2\n");
		//printf("Error occurred : wrong operator\n");
		exit(-1);
	}

	//	Signal registration
	signal(SIGUSR1, sigusr1Handler);
	signal(SIGALRM, alrmHandler);

	// The client sleeps for 1-5 seconds (try to synchronize with other clients)
	sleep(getLatency());

	printf("\nClient #%d: End of stage 5.b\n", getpid());

	writeToServerFile(argc, argv);



	// Sends signal to the server
	if (kill(myAtoi(argv[1]), SIGUSR1) == -1)
	{
		// Delete to_srv.txt file
		unlink("to_srv.txt");
		printf("\nERROR_FROM_EX2\n");
		//printf("Error occurred: attempting to send SIGUSR1 to the server\n");
		exit(-1);
	}

	printf("\nClient #%d: End of stage 5.d\n", getpid());
	printf("\nClient #%d: End of stage 5.a\n",getpid());

	alarm(WAITING_TIME);

	pause();

	exit(0);
}


// *********************************************************************************************************************************
// *********************************************************************************************************************************
//														Function Definition.
// *********************************************************************************************************************************
// *********************************************************************************************************************************

int getLatency()
{
	int randomNum = 0;
	if (getrandom(&randomNum, sizeof(randomNum), 0) < 0)
	{
		printf("\nERROR_FROM_EX2\n");
		//printf("Error occurred: attempting to get random number...\n");
		exit(-1);
	}
	if (randomNum < 0) { randomNum *= -1; }
	return ((randomNum % 5) + 1);
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

void myItoa(int num, char * str ,int base)
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

int myStrlen(char* str)
{
	int len = 0;

	while (str[len] != '\0')
	{
		len++;
	}

	return (len);
}

int isValidNumber(char* str)
{
	if (str == NULL) { return 0; }

	int sizeOfStr = myStrlen(str);
	int i = 0;

	// Checking for negative sign "-" could be added: ASCII value 45.
	if (str[0] == '-') {i++;}

	// ASCII value of 0 = 48, 9 = 57. So if value is outside of numeric range then fail.
	for (; i < sizeOfStr; i++)
	{
		if ((str[i] < 48) || str[i] > 57) { return 0; }
	}

	return 1;
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

void sigusr1Handler(int signal)
{
	//signal(SIGUSR1, sigusr1Handler);

	alarm(0);

	char to_client_path[STR_SIZE] = "to_client_";
	char pid_str[STR_SIZE] = { 0 };
	char result[STR_SIZE] = { 0 };

	int fd_to_client = -1;

	pid_t pid = getpid();

	myItoa(pid, pid_str, DECIMAL);

	// Creates the path to the to_client_XXXX.txt file
	myStrcat(to_client_path, pid_str);

	if (to_client_path[0] == '\0')
	{
		printf("\nERROR_FROM_EX2\n");
		//printf("Error occurred attempting to concatenate...\n");
		exit(-1);
	}

	myStrcat(to_client_path, ".txt");

	if (to_client_path[0] == '\0')
	{
		printf("\nERROR_FROM_EX2\n");
		//printf("Error occurred attempting to concatenate...\n");
		exit(-1);
	}

	if ((fd_to_client = open(to_client_path, O_RDONLY, 0666)) < 0) 
	{ 
		unlink(to_client_path);
		printf("\nERROR_FROM_EX2\n");
		//printf("Error occurred attempting to open to_client.txt file...\n");
		exit(-1);
	}

	if (read(fd_to_client, result, STR_SIZE) < 0)
	{
		close(fd_to_client);
		unlink(to_client_path);
		printf("\nERROR_FROM_EX2\n");
		//printf("Error occurred attempting to read from to_client.txt file...\n");
		exit(-1);
	}

	printf("\nThe result for client #%d is: %s\n", pid, result);

	close(fd_to_client);
	if (unlink(to_client_path) < 0)
	{
		printf("\nERROR_FROM_EX2\n");
		//printf("Error occurred attempting to remove  to_client_XXXXX.txt file...\n");
		exit(-1);
	}

	printf("\nClient #%d: End of stage 5.j\n", getpid());


}

void alrmHandler(int signal)
{
	//Delete to_srv.txt file
	unlink("to_srv.txt");
	printf("\nClient closed because no response was received from the server for 30 seconds\n");
}

void writeToServerFile(int argc, char* argv[])
{
	// Variables
	int fd_to_server = -1;

	pid_t my_pid = getpid();

	char to_srv_path[STR_SIZE] = "to_srv.txt";
	char my_pid_str[STR_SIZE] = { 0 };

	for (int i = 0; i < NUM_OF_TRIES; i++)
	{
		if ((fd_to_server = open(to_srv_path, O_WRONLY | O_CREAT |O_EXCL, 0666)) < 0) { sleep(getLatency()); }
		else { break; }
	}

	printf("\nClient #%d: End of stage 5.e\n", getpid());

	//Coudln't open to_srv.txt file for 10 tries
	if (fd_to_server < 0)
	{
		printf("\nERROR_FROM_EX2\n");
		//printf("Error occurred attempting to open to_srv.txt file...\n");
		exit(-1);
	}

	myItoa(my_pid, my_pid_str, DECIMAL);

	//Write to to_srv.txt file the client's pid and client's expression (each parameter in new line ("\n"))

	if (write(fd_to_server, my_pid_str, myStrlen(my_pid_str)) < 0)
	{
		close(fd_to_server);

		printf("\nERROR_FROM_EX2\n");
		//printf("Error occurred attempting to write to_srv.txt file...\n");
		exit(-1);
	}

	if (write(fd_to_server, "\n", myStrlen("\n")) < 0)
	{
		close(fd_to_server);

		printf("\nERROR_FROM_EX2\n");
		//printf("Error occurred: attempting to write to_srv.txt file...\n");
		exit(-1);
	}

	for (int i = 2; i < argc; i++)
	{
		if (write(fd_to_server, argv[i], myStrlen(argv[i])) < 0)
		{
			close(fd_to_server);

			printf("\nERROR_FROM_EX2\n");
			//printf("Error occurred attempting to write to_srv.txt file...\n");
			exit(-1);
		}

		if (write(fd_to_server, "\n", myStrlen("\n")) < 0)
		{
			close(fd_to_server);

			printf("\nERROR_FROM_EX2\n");
			//printf("Error occurred: attempting to write to_srv.txt file...\n");
			exit(-1);
		}
	}

	close(fd_to_server);
	fd_to_server = -1;
}
