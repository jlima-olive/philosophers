/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:24:31 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/05 22:37:19 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


/*
void	*run_code(void *arg)
{
	int				ind;
	t_philo			*philo;
	struct timeval	time;
	struct timeval	last_time;
	long int		total_time;

	philo = (t_philo *)arg;
	// if (philo->p_nbr != philo->info->nbr_of_philo)
	// 	usleep(200);
	// else
	// {
	// 	pthread_mutex_lock(&philo->info->start_banquet);
	// 	usleep(200);
	// }
	// pthread_mutex_unlock(&philo->info->start_banquet);
	gettimeofday(&last_time, NULL);
	total_time = 0;
	ind = 0;
	while (1 && ++ind)
	{
		gettimeofday(&time, NULL);
		total_time += (philo->info->time_to_eat) * (philo->p_nbr % 2 - 1 != 0);
		if (ft_eating(philo, total_time , time ))
			break ;
		total_time += (philo->info->time_to_eat);
		last_time = time;
		gettimeofday(&time, NULL);
		// total_time += philo->info->time_to_eat;
		// total_time += ((1000000 * (time.tv_usec < last_time.tv_usec) + time.tv_usec - last_time.tv_usec));
		if (ft_thinking(philo))
			break ;
		last_time = time;
		gettimeofday(&time, NULL);
		// total_time += ((1000000 * (time.tv_usec < last_time.tv_usec) + time.tv_usec - last_time.tv_usec));
		if (ft_sleeping(philo))
			break ;
		last_time = time;
	}
	return (NULL);
}
*/

long	total_time(void)
{
	static int		 		flag;
	static struct timeval	start;
	struct timeval			curr;
	long					ret;

	if (flag == 0)
		gettimeofday(&start, NULL);
	flag++;
	gettimeofday(&curr, NULL);
	ret = MEGA * (curr.tv_sec - start.tv_sec) + curr.tv_usec - start.tv_usec;
	return (ret);
}

long	last_time_ate(t_philo *philo)
{
	struct timeval	curr;
	long			ret;	

	gettimeofday(&curr, NULL);
	ret = MEGA * curr.tv_sec + curr.tv_usec;
	ret = ret - (MEGA * philo->lta.tv_sec  + philo->lta.tv_usec);
	return (ret / KILO);
}

int	int_info(int ac, char **av, t_info *info)
{
	info->nbr_of_philo = ft_atoi(av[1]);
	info->time_to_die = ft_atoi(av[2]);
	info->time_to_eat = ft_atoi(av[3]);
	info->time_to_sleep = ft_atoi(av[4]);
	info->notepme = -1;
	if (ac > 5)
		info->notepme = ft_atoi(av[5]);
	info->continue_sim = 1;
	return (0);
}

int	exit_message(t_info *info)
{
	write(2, "invalid number of philosophers\n", \
		(info->nbr_of_philo <= 0) * 32);
	write(2, "invalid time to die\n", (info->time_to_die <= 0) * 21);
	write(2, "invalid time to eat\n", (info->time_to_eat <= 0) * 21);
	write(2, "invalid time to sleep\n", (info->time_to_sleep <= 0) * 23);
	write(2, "invalid number of times each philosopher must eat \n", \
		(info->notepme <= 0) * 52);
	return ((info->nbr_of_philo <= 0) + (info->time_to_die <= 0) + \
		(info->time_to_eat <= 0) + (info->time_to_sleep <= 0) + \
		(info->notepme <= 0));
}

t_philo	*init_philo_and_mutex(int nbr, t_info *info, pthread_mutex_t *info_mutex)
{
	t_philo			*philo;
	t_philo			*head;
	int				ind;

	philo = ft_philonew(NULL, NULL, 1, info);
	philo->info_mutex = info_mutex;
	if (philo == NULL)
		return (NULL);
	head = philo;
	ind = 1;
	while (++ind < nbr)
	{
		philo->right = ft_philonew(philo, NULL, ind, info);
		if (philo->right == NULL)
			return (ft_philoclear(&head, ind - 1), NULL);
		philo->right->info_mutex = info_mutex;
		philo = philo->right;
	}
	philo->right = ft_philonew(philo, head, nbr, info);
	if (philo->right == NULL)
		return (ft_philoclear(&head, ind - 1), NULL);
	philo->right->info_mutex = info_mutex;
	head->left = philo->right;
	return (head);
}

int	init_infosophers(t_info *info, pthread_mutex_t	*info_mutex)
{
	int				ind;
	t_philo			*philo;
	pthread_t		*nof;

	nof = malloc(info->nbr_of_philo * sizeof(pthread_t));
	if (nof == NULL)
		return (1);
	philo = init_philo_and_mutex(info->nbr_of_philo, info, info_mutex);
	if (philo == NULL)
		return (free(nof), 1);
	/* while (philo->nbr != philo->info->nbr_of_philo)
	{
		printf("philo->nbr=%d\n", philo->nbr);
		philo = philo->right;
	}
	printf("%ld %ld\n", philo->lta.tv_sec, philo->lta.tv_usec);
	return (0); */
	ind = -1;
	while (++ind < info->nbr_of_philo)
	{
		if (pthread_create(nof + ind, NULL, run_code, philo))
			return (ft_philoclear(philo, info->nbr_of_philo), free(nof), 1);
		pthread_detach(nof[ind]);
		philo = philo->right;
	}
	if (pthread_create(nof + ind, NULL, hypervise, philo))
		return (work(), ft_philoclear(philo, info->nbr_of_philo), free(nof));
	pthread_mutex_lock(info_mutex);
	while(info->continue_sim)
	{
		pthread_mutex_unlock(info_mutex);
		usleep(100);
		pthread_mutex_lock(info_mutex);
	}
	pthread_mutex_unlock(info_mutex);

	return (free(nof), ft_lstclear(&philo, info->nbr_of_philo), 0);
}

int	main(int ac, char **av)
{
 	pthread_mutex_t	info_mutex;
	t_info			info;

	if (ac != 5 && ac != 6)
		return (write(2, "invalid number of arguments\n", 28));
	if (int_info(ac, av, &info))
		return (1);
	if (exit_message(&info))
		return (1);
	if (pthread_mutex_init(&info_mutex, NULL))
		return (1);
	printf("nbr_of_philo:%d\n", info.nbr_of_philo);
	printf("time_to_die:%d\n", info.time_to_die);
	printf("time_to_eat:%d\n", info.time_to_eat);
	printf("time_to_sleep:%d\n", info.time_to_sleep);
	printf("notepme:%d\n", info.notepme);
	printf("\nstarting now\n");
	init_infosophers(&info, &info_mutex);
	pthread_mutex_destroy(&info_mutex);
	return (0);
}
