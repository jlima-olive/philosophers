/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hypervisor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namejojo <namejojo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:57:02 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/19 02:48:24 by namejojo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void hypervise(t_philo *philo)
{
	int				*alive;
	long			ttd;
	pthread_mutex_t	*eating_mutex;
	pthread_mutex_t	*alive_mutex;

	usleep(1000);
	alive = philo->all_alive;
	ttd = philo->time_to_die;
	eating_mutex = &philo->eating_mutex;
	alive_mutex = philo->alive_mutex;
	while (1)
	{
		// pthread_mutex_lock(alive_mutex);
		// if (*alive == 0)
			// break ;
		// pthread_mutex_unlock(alive_mutex);
		// printf("\t\t%d in\n", philo->nbr);
		// pthread_mutex_lock(eating_mutex);
		// printf("\t\t%d out\n", philo->nbr);
		// if ()
		// {
			// pthread_mutex_unlock(eating_mutex);
			if (philo->eating == 0 && last_time_ate(philo) > ttd)
			{
				pthread_mutex_lock(alive_mutex);
				*alive = 0;
				pthread_mutex_unlock(alive_mutex);
				wait_to_talk(philo);
				printf("%ld %d died\n", total_time() / KILO, philo->nbr);
				return ;
			}
		// }
		// else
			pthread_mutex_unlock(eating_mutex);
		philo = philo->right;
	}
	// pthread_mutex_unlock(philo->alive_mutex);

}
