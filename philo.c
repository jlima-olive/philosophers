/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:24:31 by jlima-so          #+#    #+#             */
/*   Updated: 2025/07/04 20:50:55 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

int	int_info(int ac, char **av, t_info *info)
{
	info->nbr_of_philosophers = ft_atoi(av[1]);
	info->time_to_die = ft_atoi(av[2]) * 1000;
	info->time_to_eat = ft_atoi(av[3]) * 1000;
	info->time_to_sleep = ft_atoi(av[4]) * 1000;
	info->notepme = -1;
	if (ac > 5)
		info->notepme = ft_atoi(av[5]);
	info->talky_talk = 1;
	info->all_alive = 1;
	info->total_time = 0;
	if (pthread_mutex_init(&info->start_banquet, NULL))
		return (1);
	if (pthread_mutex_init(&info->talky_talk_prot, NULL))
		return (1);
	return (0);
}

int	exit_message(t_info info)
{
	write(2, "invalid number of philosophers\n", \
		(info.nbr_of_philosophers <= 0) * 32);
	write(2, "invalid time to die\n", (info.time_to_die <= 0) * 21);
	write(2, "invalid time to eat\n", (info.time_to_eat <= 0) * 21);
	write(2, "invalid time to sleep\n", (info.time_to_sleep <= 0) * 23);
	write(2, "invalid number of times each philosopher must eat \n", \
		(info.notepme <= 0) * 52);
	return ((info.nbr_of_philosophers <= 0) + (info.time_to_die <= 0) + \
		(info.time_to_eat <= 0) + (info.time_to_sleep <= 0) + \
		(info.notepme <= 0));
}

int	ft_eating(t_list *philo, unsigned long total_time, struct timeval last_time)
{
	struct timeval	time;

	while (philo->fork == 0 || philo->left->fork == 0)
	{
		
	}
	pthread_mutex_lock(&philo->fork_prot);	
	pthread_mutex_lock(&philo->left->fork_prot);
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
	printf("%lu %d is eating\n", (total_time) / 1000, philo->p_nbr);
	usleep(philo->info.time_to_eat);
	philo->left->fork = 1;
	philo->fork = 1;
	philo->info.talky_talk = 1;
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
	int				ind;
	t_list			*philo;
	struct timeval	time;
	struct timeval	last_time;

	philo = (t_list *)arg;
	// if (philo->p_nbr != philo->info.nbr_of_philosophers)
	// 	usleep(200);
	// else
	// {
	// 	pthread_mutex_lock(&philo->info.start_banquet);
	// 	usleep(200);
	// }
	// pthread_mutex_unlock(&philo->info.start_banquet);
	gettimeofday(&last_time, NULL);
	ind = 0;
	while (1 && ++ind)
	{
		gettimeofday(&time, NULL);
		philo->info.total_time += (time.tv_usec);
		if (ft_eating(philo, philo->info.total_time, time))
			break ;
		last_time = time;
		gettimeofday(&time, NULL);
		philo->info.total_time += philo->info.time_to_eat;
		philo->info.total_time += ((1000000 * (time.tv_usec < last_time.tv_usec) + time.tv_usec - last_time.tv_usec));
		if (ft_thinking(philo))
			break ;
		last_time = time;
		gettimeofday(&time, NULL);
		philo->info.total_time += ((1000000 * (time.tv_usec < last_time.tv_usec) + time.tv_usec - last_time.tv_usec));
		if (ft_sleeping(philo))
			break ;
		last_time = time;
	}
	return (NULL);
}

t_list	*init_fork_prot(int nbr, t_info info)
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

void	get_order(t_list *philo)
{
	int	lcount;
	int	flag;

	flag =  philo->info.nbr_of_philosophers % 2 - 1;
	while (1)
	{
		lcount = philo->info.nbr_of_philosophers / 2;
		while (lcount-- > 0)
		{
			printf("%d\n", philo->p_nbr);
			fflush(stdout);
			philo = philo->right->right;
		}
		if (flag)
		{
			if (2 == philo->p_nbr)
				philo = philo->left;
			else if (1 == philo->p_nbr)
				philo = philo->right;
		}
		printf("\n");
		sleep(1);
	}
}

int	init_infosophers(t_info info)
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
	get_order(philo);
	ind = -1;
	while (++ind < info.nbr_of_philosophers)
	{
		if (pthread_create(nof + ind, NULL, run_code, philo))
			return (1);
		pthread_detach(nof[ind]);
		philo = philo->right;
	}
	while(philo->info.all_alive)
	{
		
	}
	return (free(nof), ft_lstclear(&philo, info.nbr_of_philosophers), 0);
}

int	main(int ac, char **av)
{
	t_info	info;

	if (ac < 5 || ac > 6)
		return (write(2, "invalid number of arguments\n", 28));
	if (int_info(ac, av, &info))
		return (1);
	if (exit_message(info))
		return (1);
	if (pthread_mutex_init(&info.talky_talk_prot, NULL))
		return (1);
	if (pthread_mutex_init(&info.start_banquet, NULL))
		return (1);
	printf("nbr_of_philosophers:%d\ntime_to_die:%d\ntime_to_eat:%d\ntime_to_sleep:%d\nnotepme:%d\n\nstarting now\n", info.nbr_of_philosophers, info.time_to_die, info.time_to_eat, info.time_to_sleep, info.notepme);
	init_infosophers(info);
}