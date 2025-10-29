/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hypervisor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:57:02 by jlima-so          #+#    #+#             */
/*   Updated: 2025/10/29 23:42:55 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	hypervise(t_philo *philo, long ttd)
{
	int	print;

	ttd = philo->time_to_die;
	while (*philo->init == 0);
	usleep(500);
	while (1)
	{
		pthread_mutex_lock(philo->dead_mutex);
		if (last_time_ate(philo) > ttd)
		{
			*philo->dead = philo->nbr;
			print = *philo->dead;
			pthread_mutex_lock(philo->talk_mutex);
			pthread_mutex_unlock(philo->dead_mutex);
			use_single_syscal(total_time() / KILO, print, " is dead\n");
			pthread_mutex_unlock(philo->talk_mutex);
			return ;
		}
		pthread_mutex_unlock(philo->dead_mutex);
		philo = philo->right;
	}
}
