/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hypervisor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:57:02 by jlima-so          #+#    #+#             */
/*   Updated: 2025/11/04 15:41:00 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	hypervise(t_philo *philo, long ttd)
{
	usleep(ttd - 999);
	while (1)
	{
		if (philo->eating == 0)
		{
			pthread_mutex_lock(philo->dead_mutex);
			if (last_time_ate(philo) >= ttd)
			{
				*philo->dead = philo->nbr;
				use_single_syscal(total_time() / KILO, philo->nbr, " is dead\n");
				pthread_mutex_unlock(philo->dead_mutex);
				return ;
			}
			pthread_mutex_unlock(philo->dead_mutex);
		}
		philo = philo->right;
	}
}
