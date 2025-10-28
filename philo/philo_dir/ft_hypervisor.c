/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hypervisor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:57:02 by jlima-so          #+#    #+#             */
/*   Updated: 2025/10/28 12:35:13 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	hypervise(t_philo *philo, long ttd)
{
	int				*alive;
	pthread_mutex_t	*eating_mutex;
	pthread_mutex_t	*alive_mutex;

	if (philo->nbr_of_philo == 1)
		return ;
	usleep(1000);
	alive = philo->all_alive;
	ttd = philo->time_to_die;
	eating_mutex = &philo->eating_mutex;
	alive_mutex = philo->alive_mutex;
	while (1)
	{
			if (philo->eating == 0 && last_time_ate(philo) > ttd)
			{
				pthread_mutex_lock(alive_mutex);
				*alive = 0;
				pthread_mutex_unlock(alive_mutex);
				wait_to_talk(philo);
				printf("%ld %d died\n", total_time() / KILO, philo->nbr);
				return ;
			}
			pthread_mutex_unlock(eating_mutex);
		philo = philo->right;
	}
}
