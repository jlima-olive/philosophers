/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:24:31 by jlima-so          #+#    #+#             */
/*   Updated: 2025/07/03 21:02:02 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

int	exit_message(t_philo philo)
{
	write(2, "invalid number of philosophers\n", \
		(philo.nbr_of_philosophers <= 0) * 32);
	write(2, "invalid time to die\n", (philo.time_to_die <= 0) * 21);
	write(2, "invalid time to eat\n", (philo.time_to_eat <= 0) * 21);
	write(2, "invalid time to sleep\n", (philo.time_to_sleep <= 0) * 23);
	write(2, "invalid number of times each philosopher must eat \n", \
		(philo.notepme <= 0) * 52);
	return ((philo.nbr_of_philosophers <= 0) + (philo.time_to_die <= 0) + \
		(philo.time_to_eat <= 0) + (philo.time_to_sleep <= 0) + \
		(philo.notepme <= 0));
}

void	*run_code(void *arg)
{
	t_list			*philo;
	struct timeval	time;

	philo = (t_list *)arg;
	// pthread_mutex_unlock(&philo->info.start_banquet);
	// while (1)
	// {
		// ft_eating(philo);
		// ft_thinking(philo);
		// ft_sleeping(philo);
		pthread_mutex_lock(&philo->fork);
		pthread_mutex_lock(&philo->left->fork);
		pthread_mutex_lock(&philo->talky_talk);
		gettimeofday(&time, NULL);
		ft_putnbr_fd((int)time.tv_usec, 1);
		ft_putstr_fd(" ", 1);
		ft_putnbr_fd(philo->p_nbr, 1);
		ft_putstr_fd(" is eating\n", 1);
		// printf("wtf %d\n", philo->info.time_to_eat);
		fflush(stdout);
		usleep(philo->info.time_to_eat);
		fflush(stdout);
		// ft_putnbr_fd(time.tv_sec, 1);
		// ft_putnbr_fd(time.tv_sec, 1);
		pthread_mutex_unlock(&philo->talky_talk);
		pthread_mutex_unlock(&philo->fork);
		pthread_mutex_unlock(&philo->left->fork);
	// }
	return (NULL);
}

t_list	*init_fork(int nbr, pthread_mutex_t	mutex, t_philo info)
{
	t_list			*philo;
	t_list			*head;
	int				ind;

	philo = ft_lstnew(NULL, NULL, 1, info);
	if (philo == NULL)
		return (NULL);
	head = philo;
	ind = 1;
	while (++ind < nbr)
	{
		philo->talky_talk = mutex;
		philo->right = ft_lstnew(philo, NULL, ind, info);
		if (philo->right == NULL)
			return (ft_lstclear(&head, ind - 1), NULL);
		philo = philo->right;
	}
	philo->right = ft_lstnew(philo, head, nbr, info);
	if (philo->right == NULL)
		return (ft_lstclear(&head, ind - 1), NULL);
	head->left = philo->right;
	philo->right->talky_talk = mutex;
	return (head);
}

int	init_philosophers(t_philo info, pthread_mutex_t mutex)
{
	int				ind;
	t_list			*philo;
	pthread_t		*nof;

	nof = malloc(info.nbr_of_philosophers * sizeof(pthread_t));
	if (nof == NULL)
		return (1);
	philo = init_fork(info.nbr_of_philosophers, mutex, info);
	if (philo == NULL)
		return (free(nof), 1);
	ind = -1;
	printf("wtf2 %d\n", info.time_to_die);
	while (++ind < info.nbr_of_philosophers)
	{
		if (pthread_create(nof + ind, NULL, run_code, philo))
			return (1);
		pthread_detach(nof[ind]);
		philo = philo->right;
	}
	sleep(1);
	return (free(nof), ft_lstclear(&philo, info.nbr_of_philosophers), 0);
}

int	main(int ac, char **av)
{
	t_philo			info;
	pthread_mutex_t	mutex;

	if (ac != 6)
		return (write(2, "invalid number of arguments\n", 28));
	info.nbr_of_philosophers = ft_atoi(av[1]);
	info.time_to_die = ft_atoi(av[2]);
	info.time_to_eat = ft_atoi(av[3]);
	info.time_to_sleep = ft_atoi(av[4]);
	info.notepme = ft_atoi(av[5]);
	if (exit_message(info))
		return (1);
	if (pthread_mutex_init(&mutex, NULL))
		return (1);
	printf("wtf1 %d\n", info.time_to_die);
	init_philosophers(info, mutex);
}

// timestamp_in_ms X has taken a fork
// timestamp_in_ms X is eating
// timestamp_in_ms X is sleeping
// timestamp_in_ms X is thinking
// timestamp_in_ms X died
