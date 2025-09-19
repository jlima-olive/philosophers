/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namejojo <namejojo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:24:31 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/18 22:17:18 by namejojo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	init_philo(int ac, char **av, t_philo *philo)
{
	philo->nbr = 1;
	philo->nbr_of_philo = ft_atoi(av[1]);
	philo->time_to_die = ft_atoi(av[2]) * KILO;
	philo->time_to_eat = ft_atoi(av[3]) * KILO;
	philo->time_to_sleep = ft_atoi(av[4]) * KILO;
	philo->notepme = -1;
	if (ac > 5)
		philo->notepme = ft_atoi(av[5]);
	return (0);
}

int	exit_message(t_philo *philo, int ac)
{
	write(2, "invalid number of philosophers\n",
		  (philo->nbr_of_philo <= 0) * 32);
	write(2, "invalid time to die\n", (philo->time_to_die <= 0) * 21);
	write(2, "invalid time to eat\n", (philo->time_to_eat <= 0) * 21);
	write(2, "invalid time to sleep\n", (philo->time_to_sleep <= 0) * 23);
	if (ac > 5)
		write(2, "invalid number of times each philosopher must eat \n",
			  (philo->notepme <= 0) * 52);
	return ((philo->nbr_of_philo <= 0) + (philo->time_to_die <= 0) +
				(philo->time_to_eat <= 0) + (philo->time_to_sleep <= 0) +
				(philo->notepme <= 0) && ac > 5);
}

void	*hypervisor(void *var)
{
	t_philo	*philo;

	philo = (t_philo *)var;
	while (1)
	{
		if (last_time_ate(philo) >= philo->time_to_die)
		{
			// printf("\t\t\t%ld %d died\n", total_time() / KILO, philo->nbr);
			sem_wait(philo->talk_perms);
			printf("\t\t\t%ld %d died\n", total_time() / KILO, philo->nbr);
			exit(sem_post(philo->dead));
		}
	}
	return (NULL);
}

void	*run_code(t_philo *philo)
{
	pthread_t	thread_id;

	if (philo->nbr_of_philo == 1)
	{
		usleep(philo->time_to_die);
		printf("%ld 1 died\n", total_time() / KILO);
		return (NULL);
	}
	sem_wait(philo->info);
	gettimeofday(&philo->lta, NULL);
	sem_post(philo->info);
	if (pthread_create(&thread_id, NULL, hypervisor, philo))
		exit(sem_post(philo->dead));
	pthread_detach(thread_id);
	while (1)
	{
		go_eat(philo);
		if (philo->ammout_eaten == philo->notepme)
			sem_post(philo->eaten_sem);
		go_sleep(philo);
		go_think(philo);
	}
	return (NULL);
}

int	unlink_all_sem(void)
{
	sem_unlink("/eaten_sem");
	sem_unlink("/talk_perms");
	sem_unlink("/dead");
	sem_unlink("/spoons");
	sem_unlink("/getting_spoons");
	sem_unlink("/info");
	sem_unlink("/0");
	sem_unlink("/1");
	sem_unlink("/2");
	sem_unlink("/3");
	sem_unlink("/4");
	sem_unlink("/5");
	sem_unlink("/6");
	sem_unlink("/7");
	sem_unlink("/8");
	sem_unlink("/9");
	return (1);
}

void	*wait_to_close(void *var)
{
	int		ind;
	t_philo *philo;

	philo = (t_philo *)var;
	ind = philo->nbr_of_philo;
	while (ind--)
		sem_wait(philo->eaten_sem);
	kill(0, SIGKILL);
	sem_close(philo->talk_perms);
	sem_close(philo->dead);
	sem_close(philo->spoons);
	sem_close(philo->getting_spoons);
	unlink_all_sem();
	return (NULL);
}

/* sem_t	*get_info(t_philo *philo, int ind, int total)
{
	char	str[3];
	
	str[0] = '/';
	str[1] = '0' + ind / 10;
	str[2] = '\0';
	sem_unlink(str);
	if (ind / 10 != total / 10)
		philo->info = sem_open("/eaten_sea", O_CREAT, 0660, total / 10);
	if (ind / 10 != total / 10)
		philo->info = sem_open("/eaten_sea", O_CREAT, 0660, total / 10);
	if (philo->info == SEM_FAILED)
		return (unlink_all_sem(), NULL);
	return (philo->info);
} */

void	init_infosophers(t_philo *philo, sem_t	**info)
{
	pthread_t	thread_id;
	int			pid[200];
	int			ind;

	ind = -1;
	total_time();
	while (++ind < philo->nbr_of_philo)
	{
		philo->info = info[ind % 10];
		pid[ind] = fork();
		if (pid[ind] == -1)
		{
			kill(0, SIGKILL);
			exit(0);
		}
		if (pid[ind] == 0)
			run_code(philo);
		philo->nbr++;
	}
	if (pthread_create(&thread_id, NULL, wait_to_close, philo))
		exit(sem_post(philo->dead));
	pthread_detach(thread_id);
	// printf("check1\n");
	sem_wait(philo->dead);
	kill(0, SIGKILL);
	// printf("check2\n");
}

int	open_all_sem(t_philo *philo)
{
	philo->talk_perms = \
	sem_open("/talk_perms", O_CREAT, 0660, 1);
	if (philo->talk_perms == SEM_FAILED)
		return (1);
	philo->dead = \
	sem_open("/dead", O_CREAT, 0660, 0);
	if (philo->dead == SEM_FAILED)
		return (unlink_all_sem());
	philo->spoons = \
	sem_open("/spoons", O_CREAT, 0660, philo->nbr_of_philo);
	if (philo->spoons == SEM_FAILED)
		return (unlink_all_sem());
	philo->getting_spoons = \
	sem_open("/getting_spoons", O_CREAT, 0660, philo->nbr_of_philo / 2 + 1);
	if (philo->getting_spoons == SEM_FAILED)
		return (unlink_all_sem());
	philo->eaten_sem = \
	sem_open("/eaten_sem", O_CREAT, 0660, 0);
	if (philo->eaten_sem == SEM_FAILED)
		return (unlink_all_sem());
	return (0);
}

sem_t	**open_sem(void)
{
	sem_t	**info;
	char	str[3];
	int		ind;

	str[0] = '/';
	info = malloc(sizeof(sem_t *) * 10);
	str[2] = '\0';
	ind = -1;
	while (++ind < 10)
	{
		str[1] = '0' + ind / 10;
		sem_unlink(str);
		info[ind / 10] = sem_open(str, O_CREAT, 0660, 1);
	}
	return (info);
}

int	main(int ac, char **av)
{
	t_philo	philo;
	sem_t	**info;

	if (ac != 5 && ac != 6)
		return (write(2, "invalid number of arguments\n", 28));
	if (init_philo(ac, av, &philo))
		return (1);
	if (exit_message(&philo, ac))
		return (1);
	printf("nbr_of_philo:%d\n", philo.nbr_of_philo);
	printf("time_to_die:%d\n", philo.time_to_die / KILO);
	printf("time_to_eat:%d\n", philo.time_to_eat / KILO);
	printf("time_to_sleep:%d\n", philo.time_to_sleep / KILO);
	printf("notepme:%d\n", philo.notepme);
	info = open_sem();
	unlink_all_sem();
	if (open_all_sem(&philo))
		return (1);
	printf("\nstarting now\n");
	init_infosophers(&philo, info);
	sem_close(philo.eaten_sem);
	sem_close(philo.talk_perms);
	sem_close(philo.info);
	sem_close(philo.dead);
	sem_close(philo.spoons);
	sem_close(philo.getting_spoons);
	unlink_all_sem();
	return (0);
}

