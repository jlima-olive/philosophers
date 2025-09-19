/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_action_funcs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namejojo <namejojo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:59:13 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/19 02:50:07 by namejojo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int go_eat(t_philo *philo)
{
	grab_spoon(philo);
	if (wait_to_talk(philo))
		return (drop_spoon(philo), /* start_dying(philo),  */1);
	if (last_time_ate(philo) > philo->time_to_die || all_alive(philo) == 0)
		return (drop_spoon(philo), /* start_dying(philo),  */1);
	philo->eating = 1;
	pthread_mutex_lock(&philo->eating_mutex);
	gettimeofday(&philo->lta, NULL);
	pthread_mutex_unlock(&philo->eating_mutex);
	printf("%ld %d has taken a fork\n", total_time() / KILO, philo->nbr);
	printf("%ld %d has taken a fork\n", total_time() / KILO, philo->nbr);
	printf("%ld %d is eating\n", total_time() / KILO, philo->nbr);
	let_other_talk(philo);
	if (better_usleep(philo, philo->time_to_eat))
		return (drop_spoon(philo), /* start_dying(philo),  */1);
	drop_spoon(philo);
	philo->times_ate++;
	philo->eating = 0;
	pthread_mutex_lock(&philo->eating_mutex);
	gettimeofday(&philo->lta, NULL);
	pthread_mutex_unlock(&philo->eating_mutex);
	return (0);
}

int go_think(t_philo *philo)
{
	long time;

	time = philo->time_to_eat - last_time_ate(philo);
	if (last_time_ate(philo) > philo->time_to_die || wait_to_talk(philo))
		return (/* start_dying(philo),  */1);
	printf("%ld %d is thinking\n", total_time() / KILO, philo->nbr);
	let_other_talk(philo);
	if (time > 0 && better_usleep(philo, 0))
		return (time);
	return (0);
}

int go_sleep(t_philo *philo)
{
	if (last_time_ate(philo) > philo->time_to_die || wait_to_talk(philo))
		return (/* start_dying(philo),  */1);
	if (all_alive(philo))
		printf("%ld %d is sleeping\n", total_time() / KILO, philo->nbr);
	else
		return (/* start_dying(philo),  */1);
	let_other_talk(philo);
	if (better_usleep(philo, philo->time_to_sleep))
		return (/* start_dying(philo),  */1);
	return (0);
}

void grab_spoon(t_philo *philo)
{
	pthread_mutex_lock(philo->spoon1);
	pthread_mutex_lock(philo->spoon2);
	// printf("%d grabbed the spoons\n", philo->nbr);
}

void drop_spoon(t_philo *philo)
{
	pthread_mutex_unlock(philo->spoon2);
	pthread_mutex_unlock(philo->spoon1);
	// printf("%d dropped the spoons\n", philo->nbr);
}
