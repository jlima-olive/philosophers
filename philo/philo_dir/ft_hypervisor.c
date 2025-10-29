/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hypervisor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:57:02 by jlima-so          #+#    #+#             */
/*   Updated: 2025/10/29 16:57:45 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	hypervise(t_philo *philo, long ttd)
{
	int	*dead;
	int	print;

	dead = philo->dead;
	ttd = philo->time_to_die;
	while (*philo->init == 0);
	usleep(1000);	
	while (1)
	{
		pthread_mutex_lock(philo->dead_mutex);
		if (last_time_ate(philo) > ttd || *philo->dead)
		{
			if (*dead == 0)
				*dead = philo->nbr;
			print = *philo->dead;
			pthread_mutex_unlock(philo->dead_mutex);
			printf("philo%d\n\t\t\t%ld > %ld, dead %d\n",philo->nbr, last_time_ate(philo), ttd, *philo->dead);
			pthread_mutex_lock(philo->talk_mutex);
			printf("%ld %d died\n", total_time() / KILO, print);
			pthread_mutex_unlock(philo->talk_mutex);
			
			return ;
		}
		pthread_mutex_unlock(philo->dead_mutex);
		philo = philo->right;
	}
}
