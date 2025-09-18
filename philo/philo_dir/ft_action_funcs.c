/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_action_funcs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42lisba.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:59:13 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/18 15:02:25 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int go_eat(t_philo *philo, int time_to_die)
{
	long time;

	pthread_mutex_lock(philo->info_mutex);
	time = philo->info->time_to_eat;
	pthread_mutex_unlock(philo->info_mutex);
	grab_spoon(philo);
	if (wait_to_talk(philo, time_to_die))
		return (1);
	if (last_time_ate(philo) > time_to_die || all_alive(philo) == 0)
		return (let_other_talk(philo), 1);
	printf("%ld %d has taken a fork\n", total_time() / KILO, philo->nbr);
	printf("%ld %d has taken a fork\n", total_time() / KILO, philo->nbr);
	printf("%ld %d is eating\n", total_time() / KILO, philo->nbr);
	let_other_talk(philo);
	if (better_usleep(philo, time, 1))
		return (1);
	increment_eating(philo);
	drop_spoon(philo);
	pthread_mutex_lock(&philo->eat_mutex);
	gettimeofday(&philo->lta, NULL);
	pthread_mutex_unlock(&philo->eat_mutex);
	return (0);
}

int go_think(t_philo *philo, long time_to_die)
{
	long time;

	pthread_mutex_lock(philo->info_mutex);
	time = philo->info->time_to_eat - last_time_ate(philo);
	pthread_mutex_unlock(philo->info_mutex);
	if (last_time_ate(philo) > time_to_die || wait_to_talk(philo, time_to_die))
		return (start_dying(philo), 1);
	if (all_alive(philo))
		printf("%ld %d is thinking\n", total_time() / KILO, philo->nbr);
	else
		return (start_dying(philo), 1);
	let_other_talk(philo);
	if (better_usleep(philo, time, 0))
		return (start_dying(philo), 1);
	return (0);
}

int go_sleep(t_philo *philo, long time_to_die)
{
	long time;

	pthread_mutex_lock(philo->info_mutex);
	time = philo->info->time_to_sleep;
	pthread_mutex_unlock(philo->info_mutex);
	if (last_time_ate(philo) > time_to_die || wait_to_talk(philo, time_to_die))
		return (start_dying(philo), 1);
	if (all_alive(philo))
		printf("%ld %d is sleeping\n", total_time() / KILO, philo->nbr);
	else
		return (start_dying(philo), 1);
	let_other_talk(philo);
	if (better_usleep(philo, time, 0))
		return (start_dying(philo), 1);
	return (0);
}

void grab_spoon(t_philo *philo)
{
	pthread_mutex_lock(&philo->grab_spoons);
	pthread_mutex_lock(&philo->eat_mutex);
	pthread_mutex_lock(&philo->left_eat_mutex);
	while (philo->spoon == 1 || *philo->left_spoon == 1)
	{
		pthread_mutex_unlock(&philo->left_eat_mutex);
		pthread_mutex_unlock(&philo->eat_mutex);
		usleep(10);
		if (all_alive(philo) == 0)
			return ;
		pthread_mutex_lock(&philo->eat_mutex);
		pthread_mutex_lock(&philo->left_eat_mutex);
	}
	philo->spoon = 1;
	*philo->left_spoon = 1;
	pthread_mutex_unlock(&philo->left_eat_mutex);
	pthread_mutex_unlock(&philo->eat_mutex);
	pthread_mutex_unlock(&philo->grab_spoons);
}

void drop_spoon(t_philo *philo)
{
	pthread_mutex_lock(&philo->grab_spoons);
	pthread_mutex_lock(&philo->eat_mutex);
	pthread_mutex_lock(&philo->left_eat_mutex);
	philo->spoon = 0;
	*philo->left_spoon = 0;
	pthread_mutex_unlock(&philo->eat_mutex);
	pthread_mutex_unlock(&philo->left_eat_mutex);
	pthread_mutex_unlock(&philo->grab_spoons);
}
