/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spoons.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 13:35:38 by jlima-so          #+#    #+#             */
/*   Updated: 2025/11/05 13:37:20 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	grab_spoon(t_philo *philo)
{
	pthread_mutex_lock(philo->spoon2);
	pthread_mutex_lock(philo->spoon1);
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->dead)
	{
		pthread_mutex_unlock(philo->dead_mutex);
		pthread_mutex_unlock(philo->spoon1);
		pthread_mutex_unlock(philo->spoon2);
		return (1);
	}
	use_single_syscal(total_time() / KILO, philo->nbr, " has taken a fork\n");
	use_single_syscal(total_time() / KILO, philo->nbr, " has taken a fork\n");
	pthread_mutex_unlock(philo->dead_mutex);
	return (0);
}

void	drop_spoon(t_philo *philo)
{
	pthread_mutex_unlock(philo->spoon1);
	pthread_mutex_unlock(philo->spoon2);
}
