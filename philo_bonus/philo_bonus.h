/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namejojo <namejojo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:19:47 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/23 13:58:56 by namejojo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

# include <signal.h>
# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <semaphore.h>
# include <fcntl.h>
#define __USE_XOPEN

#ifndef MEGA
#define MEGA 1000000
#endif

#ifndef KILO
#define KILO 1000
#endif

typedef struct s_philo
{
	int 			nbr_of_philo;
	int 			notepme;
	int 			time_to_die;
	int 			time_to_eat;
	int 			time_to_sleep;
	sem_t			*eaten_sem;
	sem_t			*talk_perms;
	sem_t			*dead;
	sem_t			*spoons;
	sem_t			*getting_spoons;
	int				nbr;
	struct timeval	lta;
	_Atomic int		times_ate;
	_Atomic int		eating;
} t_philo;

// in str goes the string to turn into a float
float ft_atof(const char *str);
// in str goes the string to turn into an int
int ft_atoi(const char *str);

int		better_sleep(long time_to_sleep);
int		init_philo(int ac, char **av, t_philo *philo);
int		exit_message(t_philo *philo, int ac);
long 	last_time_ate(t_philo *philo);
void 	grab_spoon(t_philo *philo);
void 	drop_spoon(t_philo *philo);
void	go_sleep(t_philo *philo);
void	go_think(t_philo *philo);
void	go_eat(t_philo *philo);
void	*hypervisor(void *var);
long	total_time(void);

#endif