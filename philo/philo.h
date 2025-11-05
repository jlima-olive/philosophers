/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:19:47 by jlima-so          #+#    #+#             */
/*   Updated: 2025/11/05 13:36:30 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# define __USE_XOPEN

# ifndef MEGA
#  define MEGA 1000000
# endif

# ifndef KILO
#  define KILO 1000
# endif

typedef struct s_info
{
	int				nbr_of_philo;
	int				notepme;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				dead;
	pthread_mutex_t	dead_mutex;
	_Atomic int		init;
	_Atomic int		end_sim;
}	t_info;

typedef struct s_philo
{
	int				times_ate;
	int				nbr_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				notepme;
	int				nbr;
	int				*dead;
	pthread_mutex_t	*dead_mutex;
	_Atomic int		eating;
	_Atomic int		*init;
	_Atomic int		*end_sim;
	struct s_philo	*left;
	struct s_philo	*right;
	struct timeval	lta;
	pthread_mutex_t	holder_spoon1;
	pthread_mutex_t	*spoon1;
	pthread_mutex_t	*spoon2;
	pthread_mutex_t	gettime;
}	t_philo;

// in str goes the string to turn into a float
float	ft_atof(const char *str);
// in str goes the string to turn into an int
int		ft_atoi(const char *str);

/* content is the content to add to the newly created list left and are the
left and right nodes to connect to if htere is no node to conect to use NULL */
t_philo	*ft_philonew(t_philo *left, t_philo *right, int nbr, t_info *info);
void	ft_philoclear(t_philo *philo);

// writes number nbr into file descriptor fd
void	ft_putnbr_fd(int nbr, int fd);
// writes string str into file descriptor fd
void	ft_putstr_fd(char *str, int fd);

t_philo	*init_philo_and_mutex(t_info *info);
void	hypervise(t_philo *ph, long ttd, int _Atomic *init, int _Atomic *end);
int		better_usleep(t_philo *philo, long time_to_sleep);
int		go_sleep(t_philo *philo);
int		go_think(t_philo *philo);
int		go_eat(t_philo *philo);
long	last_time_ate(t_philo *philo);
int		grab_spoon(t_philo *philo);
void	drop_spoon(t_philo *philo);
int		any_dead(t_philo *philo);
long	total_time(void);
void	use_single_syscal(int time, int nbr, char *msg);

int		init_info(int ac, char **av, t_info *info);
int		exit_message(t_info *info, int ac);
int		ft_strlen(char *str);

#endif