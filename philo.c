/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:24:31 by jlima-so          #+#    #+#             */
/*   Updated: 2025/07/04 16:35:13 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>


int	ft_eating(t_list *philo, unsigned long long total_time, struct timeval last_time)
{
	struct timeval	time;

	while (philo->fork == 0 || philo->left->fork == 0)
	{
		
	}
	pthread_mutex_lock(&philo->left->fork_prot);
	pthread_mutex_lock(&philo->fork_prot);	
	philo->fork = 0;
	philo->left->fork = 0;
	pthread_mutex_unlock(&philo->fork_prot);
	pthread_mutex_unlock(&philo->left->fork_prot);
	while (philo->info.talky_talk == 0)
	{
		
	}
	pthread_mutex_lock(&philo->info.talky_talk_prot);
	philo->info.talky_talk = 0;
	pthread_mutex_unlock(&philo->info.talky_talk_prot);
	gettimeofday(&time, NULL);
	total_time += (time.tv_usec - last_time.tv_usec);
	// printf("%ld\n", time.tv_usec - last_time.tv_usec);
	printf("numbers %lld %d is eating for %dms\n", total_time, philo->p_nbr, philo->info.time_to_eat);
	usleep(philo->info.time_to_eat);
	philo->info.talky_talk = 1;
	philo->left->fork = 1;
	philo->fork = 1;
	return (0);
}

int	ft_thinking(t_list *philo)
{
	usleep(philo->info.time_to_sleep);
	return (0);
}

int	ft_sleeping(t_list *philo)
{
	usleep(philo->info.time_to_sleep);
	return (0);
}


void	*run_code(void *arg)
{
	t_list			*philo;
	struct timeval	time;
	struct timeval	last_time;
	unsigned long long			total_time;
	int				ind;

	philo = (t_list *)arg;
	if (philo->p_nbr != philo->info.nbr_of_philosophers)
		usleep(20);
	else
	{
		pthread_mutex_lock(&philo->info.start_banquet);
		usleep(20);
	}
	pthread_mutex_unlock(&philo->info.start_banquet);
	gettimeofday(&last_time, NULL);
	total_time = 0;
	ind = 0;
	while (1 && ++ind)
	{
		gettimeofday(&time, NULL);
		total_time += (time.tv_usec - last_time.tv_usec);
		// printf("%ld\n", time.tv_usec - last_time.tv_usec);
		if (ft_eating(philo, total_time, time))
			break ;
		last_time = time;
		gettimeofday(&time, NULL);
		total_time += (time.tv_usec - last_time.tv_usec);
		// printf("%ld\n", time.tv_usec - last_time.tv_usec);
		if (ft_thinking(philo))
			break ;
		last_time = time;
		gettimeofday(&time, NULL);
		total_time += (time.tv_usec - last_time.tv_usec);
		// printf("%ld\n", time.tv_usec - last_time.tv_usec);
		if (ft_sleeping(philo))
			break ;
		last_time = time;
	}
	return (NULL);
}

t_list	*init_fork_prot(int nbr, t_philo info)
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
		philo->right = ft_lstnew(philo, NULL, ind, info);
		if (philo->right == NULL)
			return (ft_lstclear(&head, ind - 1), NULL);
		philo = philo->right;
	}
	philo->right = ft_lstnew(philo, head, nbr, info);
	if (philo->right == NULL)
		return (ft_lstclear(&head, ind - 1), NULL);
	head->left = philo->right;
	return (head);
}

int	init_philosophers(t_philo info)
{
	int				ind;
	t_list			*philo;
	pthread_t		*nof;

	nof = malloc(info.nbr_of_philosophers * sizeof(pthread_t));
	if (nof == NULL)
		return (1);
	philo = init_fork_prot(info.nbr_of_philosophers, info);
	if (philo == NULL)
		return (free(nof), 1);
	ind = -1;
	while (++ind < info.nbr_of_philosophers)
	{
		if (pthread_create(nof + ind, NULL, run_code, philo))
			return (1);
		philo = philo->right;
	}
	while(ind--)
		pthread_join(nof[ind], NULL);
	return (free(nof), ft_lstclear(&philo, info.nbr_of_philosophers), 0);
}

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

int	main(int ac, char **av)
{
	t_philo			info;

	if (ac != 6)
		return (write(2, "invalid number of arguments\n", 28));
	info.nbr_of_philosophers = ft_atoi(av[1]);
	info.time_to_die = ft_atoi(av[2]);
	info.time_to_eat = ft_atoi(av[3]);
	info.time_to_sleep = ft_atoi(av[4]);
	info.notepme = ft_atoi(av[5]);
	info.talky_talk = 1;
	if (exit_message(info))
		return (1);
	if (pthread_mutex_init(&info.talky_talk_prot, NULL))
		return (1);
	if (pthread_mutex_init(&info.start_banquet, NULL))
		return (1);
	init_philosophers(info);
}

// timestamp_in_ms X has taken a fork_prot
// timestamp_in_ms X is eating
// timestamp_in_ms X is sleeping
// timestamp_in_ms X is thinking
// timestamp_in_ms X died
