/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hypervisor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:57:02 by jlima-so          #+#    #+#             */
/*   Updated: 2025/11/05 13:09:14 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	hypervise(t_philo *ph, long ttd, _Atomic int *init, _Atomic int *end)
{
	usleep(200);
	*init = 1;
	usleep(ttd - 999);
	while (*end != ph->nbr_of_philo)
	{
		// printf("|%d|\n", *end);
		if (ph->eating == 0)
		{
			pthread_mutex_lock(ph->dead_mutex);
			if (last_time_ate(ph) >= ttd)
			{
				*ph->dead = ph->nbr;
				use_single_syscal(total_time() / KILO, ph->nbr, " is dead\n");
				pthread_mutex_unlock(ph->dead_mutex);
				return ;
			}
			pthread_mutex_unlock(ph->dead_mutex);
		}
		ph = ph->right;
	}
	*ph->dead = 1;
}
