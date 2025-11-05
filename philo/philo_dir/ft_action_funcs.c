/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_action_funcs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:59:13 by jlima-so          #+#    #+#             */
/*   Updated: 2025/11/05 13:27:01 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_strlen(char *str)
{
	int	ind;

	ind = 0;
	while (str[ind])
		ind++;
	return (ind);
}

int	any_dead(t_philo *philo)
{
	int	ret;
	
	pthread_mutex_lock(philo->dead_mutex);
	ret = *philo->dead;
	pthread_mutex_unlock(philo->dead_mutex);
	return (ret);
}

int	go_eat(t_philo *philo)
{
	if (grab_spoon(philo))
		return (1);
	philo->eating = 1;
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->dead == 0)
	{
		use_single_syscal(total_time() / KILO, philo->nbr, " is eating\n");
		pthread_mutex_unlock(philo->dead_mutex);
	}
	else
		return (drop_spoon(philo), pthread_mutex_unlock(philo->dead_mutex), 1);
	if (better_usleep(philo, philo->time_to_eat))
		return (drop_spoon(philo), 1);
	drop_spoon(philo);
	pthread_mutex_lock(&philo->gettime);
	gettimeofday(&philo->lta, NULL);
	pthread_mutex_unlock(&philo->gettime);
	philo->eating = 0;
	philo->times_ate++;
	if (philo->times_ate == philo->notepme)
		(*philo->end_sim)++;
	return (0);
}

int	go_sleep(t_philo *philo)
{
	// if (philo->nbr % 2)
		// usleep(100);
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->dead == 0)
	{
		use_single_syscal(total_time() / KILO, philo->nbr, " is sleeping\n");
		pthread_mutex_unlock(philo->dead_mutex);
	}
	else
		return (pthread_mutex_unlock(philo->dead_mutex), 1);
	if (better_usleep(philo, philo->time_to_sleep))
		return (1);
	return (0);
}

int go_think(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->dead == 0)
	{
		use_single_syscal(total_time() / KILO, philo->nbr, " is thinking\n");
		pthread_mutex_unlock(philo->dead_mutex);
	}
	else
		return (pthread_mutex_unlock(philo->dead_mutex), 1);
	// if (better_usleep(philo, KILO))
		// return (1);
	return (0);
}

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

void drop_spoon(t_philo *philo)
{
	pthread_mutex_unlock(philo->spoon1);
	pthread_mutex_unlock(philo->spoon2);
}
