#include "multithreading.h"

pthread_mutex_t print_mutex;

/* Initialize the mutex */
__attribute__((constructor))
void init_mutex()
{
	pthread_mutex_init(&print_mutex, NULL);
}

/* Destroy the mutex */
__attribute__((destructor))
void destroy_mutex()
{
	pthread_mutex_destroy(&print_mutex);
}

/* Thread-safe printf function */
int tprintf(char const *format, ...)
{
	va_list args;
	va_start(args, format);
	int ret;

	pthread_mutex_lock(&print_mutex);
	printf("[%lu] ", pthread_self());
	ret = vfprintf(stdout, format, args);
	pthread_mutex_unlock(&print_mutex);

	va_end(args);
	return (ret);
}
