#include "philo.h"

void *func(void *var)
{
	if (var == NULL)
		return (NULL);
	return (NULL);
}

int main()
{
	pthread_t	thread;

	pthread_create(&thread, NULL, func, NULL);
	while (1)
	{
		write(1, "1\n", 2);
	}
	
}