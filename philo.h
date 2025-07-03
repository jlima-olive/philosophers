/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:19:47 by jlima-so          #+#    #+#             */
/*   Updated: 2025/07/03 20:40:27 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define __USE_XOPEN


typedef struct s_philo
{
	int				nbr_of_philosophers;
	int				notepme;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	pthread_mutex_t	start_banquet;
}	t_philo;

typedef struct s_list
{
	int				p_nbr;
	struct s_list	*left;
	struct s_list	*right;
	pthread_mutex_t	fork;
	pthread_mutex_t	talky_talk;
	t_philo			info;
}	t_list;

// in str goes the string to turn into a float
float	ft_atof(const char *str);
// in str goes the string to turn into an int
int		ft_atoi(const char *str);

/* content is the content to add to the newly created list left and are the 
left and right nodes to connect to if htere is no node to conect to use NULL */
t_list	*ft_lstnew(t_list *left, t_list *right, int nbr, t_philo info);
// lst is the address of the head of the list to delete, limit is how many nodes to delete
void	ft_lstclear(t_list **lst, int nbr);

// writes number nbr into file descriptor fd 
void	ft_putnbr_fd(int nbr, int fd);
// writes string str into file descriptor fd 
void	ft_putstr_fd(char *str, int fd);


#endif 