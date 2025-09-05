/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:19:47 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/05 22:35:40 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#define __USE_XOPEN

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
	int				talk_perms;
	int				all_alive;
	int				continue_sim;
	long			totaltime;
}	t_info;

typedef struct s_philo
{
	int				nbr;
	struct timeval	lta;
	struct s_philo	*left;
	struct s_philo	*right;
	pthread_mutex_t	eat_mutex;
	int				eating;
	pthread_mutex_t	*info_mutex;
	t_info			*info;
}	t_philo;

// in str goes the string to turn into a float
float	ft_atof(const char *str);
// in str goes the string to turn into an int
int		ft_atoi(const char *str);

/* content is the content to add to the newly created list left and are the 
left and right nodes to connect to if htere is no node to conect to use NULL */
t_philo	*ft_philonew(t_philo *left, t_philo *right, int nbr, t_info *info);
// philo is the address of the head of the list to delete, limit is how many nodes to delete
void	ft_philoclear(t_philo **lst, int nbr);

// writes number nbr into file descriptor fd 
void	ft_putnbr_fd(int nbr, int fd);
// writes string str into file descriptor fd 
void	ft_putstr_fd(char *str, int fd);


#endif 