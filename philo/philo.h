/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:19:47 by jlima-so          #+#    #+#             */
/*   Updated: 2025/10/28 12:32:11 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#define __USE_XOPEN

#ifndef MEGA
#define MEGA 1000000
#endif

#ifndef KILO
#define KILO 1000
#endif

typedef struct s_info
{
	int				nbr_of_philo;
	int				notepme;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				talk_perms;
	int				all_alive;
	pthread_mutex_t	talk_mutex;
	pthread_mutex_t	alive_mutex;
} t_info;

typedef struct s_philo
{
	int				nbr_of_philo;
	int				notepme;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				*talk_perms;
	int				*all_alive;
	_Atomic int		eating;
	int				nbr;
	struct s_philo	*left;
	struct s_philo	*right;
	struct timeval	lta;
	pthread_mutex_t	eating_mutex;
	pthread_mutex_t	*alive_mutex;
	pthread_mutex_t	*talk_mutex;
	pthread_mutex_t	*spoon1;
	pthread_mutex_t	*spoon2;
	_Atomic int		times_ate;
} t_philo;

// in str goes the string to turn into a float
float ft_atof(const char *str);
// in str goes the string to turn into an int
int ft_atoi(const char *str);

/* content is the content to add to the newly created list left and are the
left and right nodes to connect to if htere is no node to conect to use NULL */
t_philo *ft_philonew(t_philo *left, t_philo *right, int nbr, t_info *info);
void ft_philoclear(t_philo *philo);

// writes number nbr into file descriptor fd
void ft_putnbr_fd(int nbr, int fd);
// writes string str into file descriptor fd
void ft_putstr_fd(char *str, int fd);

void	hypervise(t_philo *philo, long ttd);
t_philo *init_philo_and_mutex(int nbr, t_info *info);
int better_usleep(t_philo *philo, long time_to_sleep);
int wait_to_eat(t_philo *philo);
int wait_to_talk(t_philo *philo);
int go_sleep(t_philo *philo);
int go_think(t_philo *philo);
int check_times_ate(t_philo *philo, int value);
int go_eat(t_philo *philo);
void increment_eating(t_philo *philo);
void let_other_talk(t_philo *philo);
long last_time_ate(t_philo *philo);
void grab_spoon(t_philo *philo);
void drop_spoon(t_philo *philo);
// void start_dying(t_philo *philo);
int all_alive(t_philo *philo);
long total_time(void);

int init_info(int ac, char **av, t_info *info);
int exit_message(t_info *info, int ac);

#endif