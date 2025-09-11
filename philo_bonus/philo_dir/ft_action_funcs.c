/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_action_funcs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:59:13 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/11 17:50:27 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int go_eat(t_philo *philo, int time_to_die)
{
	long time;

	pthread_mutex_lock(philo->info_mutex);
	time = philo->info->time_to_eat;
	pthread_mutex_unlock(philo->info_mutex);
	if (wait_to_eat(philo, time_to_die))
		return (start_dying(philo), 1);
	if (last_time_ate(philo) > time_to_die || all_alive(philo) == 0 || wait_to_talk(philo, time_to_die))
		return (drop_spoon(philo), start_dying(philo), 1);
	grab_spoon(philo);
	printf("%ld %d has taken a fork\n", total_time() / KILO, philo->nbr);
	printf("%ld %d has taken a fork\n", total_time() / KILO, philo->nbr);
	printf("%ld %d is eating\n", total_time() / KILO, philo->nbr);
	let_other_talk(philo);
	if (better_usleep(philo, time, 1))
		return (start_dying(philo), 1);
	increment_eating(philo);
	drop_spoon(philo);
	gettimeofday(&philo->lta, NULL);
	return (0);
}

int go_think(t_philo *philo, long time_to_die)
{
	long time;

	pthread_mutex_lock(philo->info_mutex);
	time = philo->info->time_to_eat - last_time_ate(philo);
	pthread_mutex_unlock(philo->info_mutex);
	if (wait_to_talk(philo, time_to_die))
		return (start_dying(philo), 1);
	printf("%ld %d is thinking\n", total_time() / KILO, philo->nbr);
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
	if (wait_to_talk(philo, time_to_die))
		return (start_dying(philo), 1);
	printf("%ld %d is sleeping\n", total_time() / KILO, philo->nbr);
	let_other_talk(philo);
	if (better_usleep(philo, time, 0))
		return (start_dying(philo), 1);
	return (0);
}

void grab_spoon(t_philo *philo)
{
	pthread_mutex_lock(&philo->eat_mutex);
	philo->spoon = 1;
	pthread_mutex_unlock(&philo->eat_mutex);
}

void drop_spoon(t_philo *philo)
{
	pthread_mutex_lock(&philo->eat_mutex);
	philo->spoon = 0;
	pthread_mutex_unlock(&philo->eat_mutex);
}
