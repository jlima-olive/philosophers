/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:24:31 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/12 02:40:42 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int init_philo(int ac, char **av, t_philo *philo)
{
	philo->nbr_of_philo = ft_atoi(av[1]);
	philo->time_to_die = ft_atoi(av[2]) * KILO;
	philo->time_to_eat = ft_atoi(av[3]) * KILO;
	philo->time_to_sleep = ft_atoi(av[4]) * KILO;
	philo->notepme = -1;
	if (ac > 5)
		philo->notepme = ft_atoi(av[5]);
	return (0);
}

int exit_message(t_philo *philo, int ac)
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

void *run_code(t_philo *philo)
{
	if (philo->nbr_of_philo == 1)
	{
		usleep(philo->time_to_die);
		return (printf("%ld 1 died\n", total_time() / KILO), NULL);
	}
	gettimeofday(&philo->lta, NULL);
	while (all_alive(philo))
	{
		if (go_eat(philo))
			return (NULL);
		if (philo->notepme != -1 && check_times_ate(philo, philo->notepme))
			return (NULL);
		if (go_sleep(philo) || go_think(philo))
			return (NULL);
	}
	return (NULL);
}

int init_infosophers(t_philo *philo)
{
	int	*pid;
	int	ind;

	pid = malloc((philo->nbr_of_philo) * sizeof(int));
	if (pid == NULL)
		return (1);
	ind = -1;
	total_time();
	while (++ind < philo->nbr_of_philo)
	{
		pid[ind] = fork();
		if (pid[ind] == -1)
		{
			kill(0, SIGKILL);
			free(pid);
			exit(0);
		}
		if (pid[ind] == 0)
			run_code(philo);
	}
	printf("check1\n");
	sem_wait(philo->dead);
	kill(0, SIGKILL);
	printf("check2\n");
	return (free(pid), 0);
}

int	unlink_all_sem(void)
{
	sem_unlink("/talk_perms");
	sem_unlink("/dead");
	sem_unlink("/spoons");
	sem_unlink("/getting_spoons");
	return (1);
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
	sem_open("/getting_spoons", O_CREAT, 0660, philo->nbr_of_philo / 2);
	if (philo->getting_spoons == SEM_FAILED)
		return (unlink_all_sem());
}

int main(int ac, char **av)
{
	t_philo	philo;

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
	unlink_all_sem();
	if (open_all_sem(&philo))
		return (1);
	printf("\nstarting now\n");
	init_infosophers(&philo);
	sem_close(&philo.talk_perms);
	sem_close(&philo.dead);
	sem_close(&philo.spoons);
	sem_close(&philo.getting_spoons);
	if (unlink_all_sem());
		return (1);
	return (0);
}

