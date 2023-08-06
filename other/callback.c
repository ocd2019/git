#include <stdio.h>

typedef int (*callback)(int digital, char *character);

int callback_function(int number, char *string)
{
	printf("callback_function:%d --> %s\n", number, string);

	return 0;
}

int register_callback(int number, callback function)
{
	char *src = "hello world";

	function(number, src);

	return 0;
}

int main(int argc, char *argv[])
{
	register_callback(888888, callback_function);

	return 0;
}

