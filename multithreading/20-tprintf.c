#include "multithreading.h"

/* Initialize the mutex */
__attribute__((constructor))
void init_mutex(void)
{
	static pthread_mutex_t print_mutex;
	static int initialized = 0;

	if (!initialized)
	{
		pthread_mutex_init(&print_mutex, NULL);
		initialized = 1;
	}
}

/* Destroy the mutex */
__attribute__((destructor))
void destroy_mutex(void)
{
	static pthread_mutex_t print_mutex;
	pthread_mutex_destroy(&print_mutex);
}

/* Thread-safe printf function */
int tprintf(char const *format, ...)
{
	static pthread_mutex_t print_mutex;
	int ret;
	va_list args;

	va_start(args, format);

	pthread_mutex_lock(&print_mutex);
	printf("[%lu] ", pthread_self());
	ret = vfprintf(stdout, format, args);
	pthread_mutex_unlock(&print_mutex);

	va_end(args);
	return (ret);
}
