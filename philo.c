/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:24:31 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/10 15:28:18 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long total_time(void)
{
	static int		flag;
	static struct	timeval start;
	struct			timeval curr;
	long ret;

	if (flag == 0)
	{
		gettimeofday(&start, NULL);
		flag++;
	}
	gettimeofday(&curr, NULL);
	ret = MEGA * (curr.tv_sec - start.tv_sec) + curr.tv_usec - start.tv_usec;
	return (ret);
}

long last_time_ate(t_philo *philo)
{
	struct	timeval curr;
	long	ret;

	gettimeofday(&curr, NULL);
	ret = MEGA * curr.tv_sec + curr.tv_usec;
	ret = ret - (MEGA * philo->lta.tv_sec + philo->lta.tv_usec);
	return (ret);
}

void set_eating(t_philo *philo, int value)
{
	// if (philo->nbr == 9)
		// printf("\t\t\t1 lock\n");
	pthread_mutex_lock(&philo->eat_mutex);
	philo->eating = value;
	pthread_mutex_unlock(&philo->eat_mutex);
	// if (philo->nbr == 9)
		// printf("\t\t\t1 unlock\n");
}

void start_dying(t_philo *philo)
{
	set_eating(philo, 0);
	pthread_mutex_lock(philo->info_mutex);
	if (philo->info->all_alive == 0)
	{
		pthread_mutex_unlock(philo->info_mutex);
		exit(0);
	}
	philo->info->all_alive = 0;
	while (philo->info->talk_perms == 0)
	{
		pthread_mutex_unlock(philo->info_mutex);
		pthread_mutex_lock(philo->info_mutex);
	}
	pthread_mutex_unlock(philo->info_mutex);
	printf("%ld %d died", total_time() / KILO, philo->nbr);
	exit(0);
}

int int_info(int ac, char **av, t_info *info)
{
	info->nbr_of_philo = ft_atoi(av[1]);
	info->time_to_die = ft_atoi(av[2]) * KILO;
	info->time_to_eat = ft_atoi(av[3]) * KILO;
	info->time_to_sleep = ft_atoi(av[4]) * KILO;
	info->notepme = -1;
	if (ac > 5)
		info->notepme = ft_atoi(av[5]);
	info->all_alive = 1;
	return (0);
}

int exit_message(t_info *info)
{
	write(2, "invalid number of philosophers\n",
		  (info->nbr_of_philo <= 0) * 32);
	write(2, "invalid time to die\n", (info->time_to_die <= 0) * 21);
	write(2, "invalid time to eat\n", (info->time_to_eat <= 0) * 21);
	write(2, "invalid time to sleep\n", (info->time_to_sleep <= 0) * 23);
	write(2, "invalid number of times each philosopher must eat \n",
		  (info->notepme <= 0) * 52);
	return ((info->nbr_of_philo <= 0) + (info->time_to_die <= 0) +
			(info->time_to_eat <= 0) + (info->time_to_sleep <= 0) +
			(info->notepme <= 0));
}

t_philo *init_philo_and_mutex(int nbr, t_info *info, pthread_mutex_t *info_mutex)
{
	t_philo *philo;
	t_philo *head;
	int ind;

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
			return (ft_philoclear(head), NULL);
		philo->right->info_mutex = info_mutex;
		philo = philo->right;
	}
	philo->right = ft_philonew(philo, head, nbr, info);
	if (philo->right == NULL)
		return (ft_philoclear(head), NULL);
	philo->right->info_mutex = info_mutex;
	head->left = philo->right;
	return (head);
}

int all_alive(t_philo *philo)
{
	int ret;

	pthread_mutex_lock(philo->info_mutex);
	ret = philo->info->all_alive;
	pthread_mutex_unlock(philo->info_mutex);
	return (ret);
}

void allow_to_eat(t_philo *philo)
{
	// if (philo->left->nbr == 9)
		// printf("\t\t\t21 lock\n");
	pthread_mutex_lock(&philo->left->eat_mutex);
	// if (philo->right->nbr == 9)
		// printf("\t\t\t31 lock\n");
	pthread_mutex_lock(&philo->right->eat_mutex);
	while (philo->left->eating || philo->right->eating)
	{
		pthread_mutex_unlock(&philo->left->eat_mutex);
		// if (philo->left->nbr == 9)
			// printf("\t\t\t22 unlock\n");
		pthread_mutex_unlock(&philo->right->eat_mutex);
		// if (philo->right->nbr == 9)
			// printf("\t\t\t32 unlock\n");
		usleep(20);
		// if (philo->left->nbr == 9)
			// printf("\t\t\t23 lock\n");
		pthread_mutex_lock(&philo->left->eat_mutex);
		// if (philo->right->nbr == 9)
			// printf("\t\t\t33 lock\n");
		pthread_mutex_lock(&philo->right->eat_mutex);
	}
	// if (philo->right->nbr == 9)
		// printf("\t\t\t24 unlock\n");
	pthread_mutex_unlock(&philo->left->eat_mutex);
	// if (philo->right->nbr == 9)
		// printf("\t\t\t34 unlock\n");
	pthread_mutex_unlock(&philo->right->eat_mutex);
	// if (philo->nbr == 9)
		// printf("\t\t\t4 lock\n");
	pthread_mutex_lock(&philo->eat_mutex);
	philo->waiting_to_eat = 0;
	// philo->eating = 1;
	pthread_mutex_unlock(&philo->eat_mutex);
	// if (philo->nbr == 9)
		// printf("\t\t\t4 unlock\n");
}

void allow_one_to_eat(t_philo *philo, int flag, int group)
{
	if (flag)
	{
		// if (philo->left->left->nbr == 9)
			// printf("\t\t\t51 lock\n");
		pthread_mutex_lock(&philo->left->left->eat_mutex);
	}
	// if (philo->left->nbr == 9)
		// printf("\t\t\t61 lock\n");
	pthread_mutex_lock(&philo->left->eat_mutex);
	// if (philo->right->nbr == 9)
		// printf("\t\t\t71 lock\n");
	pthread_mutex_lock(&philo->right->eat_mutex);
	group = (philo->left->eating || (philo->left->left->eating && flag));
	while (group || philo->right->eating)
	{
		if (flag)
		{
			pthread_mutex_unlock(&philo->left->left->eat_mutex);
			// if (philo->left->left->nbr == 9)
				// printf("\t\t\t52 unlock\n");
		}
		pthread_mutex_unlock(&philo->left->eat_mutex);
		// if (philo->left->nbr == 9)
			// printf("\t\t\t62 unlock\n");
		pthread_mutex_unlock(&philo->right->eat_mutex);
		// if (philo->right->nbr == 9)
			// printf("\t\t\t72 unlock\n");
		usleep(20);
		// printf("%d cant eat\n", philo->nbr);
		if (flag)
		{
			// if (philo->left->left->nbr == 9)
				// printf("\t\t\t53 lock\n");
			pthread_mutex_lock(&philo->left->left->eat_mutex);
		}
		// if (philo->left->nbr == 9)
			// printf("\t\t\t63 lock\n");
		pthread_mutex_lock(&philo->left->eat_mutex);
		// if (philo->right->nbr == 9)
			// printf("\t\t\t73 lock\n");
		pthread_mutex_lock(&philo->right->eat_mutex);
		group = (philo->left->eating || (philo->left->left->eating && flag));
	}
	if (flag)
	{
		pthread_mutex_unlock(&philo->left->left->eat_mutex);
		// if (philo->left->left->nbr == 9)
			// printf("\t\t\t54 unlock\n");
	}
	pthread_mutex_unlock(&philo->left->eat_mutex);
	// if (philo->left->nbr == 9)
		// printf("\t\t\t64 unlock\n");
	pthread_mutex_unlock(&philo->right->eat_mutex);
	// if (philo->right->nbr == 9)
		// printf("\t\t\t74 unlock\n");
	// if (philo->nbr == 9)
		// printf("\t\t\t81 lock\n");
	pthread_mutex_lock(&philo->eat_mutex);
	philo->waiting_to_eat = 0;
	pthread_mutex_unlock(&philo->eat_mutex);
	// if (philo->nbr == 9)
		// printf("\t\t\t81 unlock\n");
}

void eat_perm(t_philo *philo, const t_info *info, int flag)
{
	static int last;

	if (philo->nbr == 1)
	{
		allow_one_to_eat(philo, flag, 0);
			// printf("\t\tallowing %d to eat\n", philo->nbr);
		// allow_to_eat(philo);
		philo->eating = 1;
		philo = philo->right->right;
		while (philo->nbr < info->nbr_of_philo && philo->nbr != 1)
		{
			// printf("\t\tallowing %d to eat\n", philo->nbr);
			allow_to_eat(philo);
			philo = philo->right->right;
		}
	}
	else
	{
		while (philo->nbr <= info->nbr_of_philo - 1 - flag)
		{
			// printf("\t\t%d AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA lock\n", philo->nbr <= info->nbr_of_philo - 1);
			// printf("\t\tallowing %d to eat\n", philo->nbr);
			allow_to_eat(philo);
			// printf("\t\t%d AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA unlock\n", philo->nbr <= info->nbr_of_philo - 1);
			philo = philo->right->right;
		}
		if (last && flag)
			philo = philo->right;
		// printf("\t\tallowing %d to eat\n", philo->nbr);
		allow_to_eat(philo);
		last = !last;
	}
}

void hypervise(t_philo *philo, const t_info *info, pthread_mutex_t *info_mutex)
{
	int flag;

	flag = philo->left->nbr;
	if (flag == 1)
		return ;
	flag = flag % 2;
	pthread_mutex_lock(info_mutex);
	while (info->all_alive)
	{
		pthread_mutex_unlock(info_mutex);
		eat_perm(philo, info, flag);
		usleep(500);
		eat_perm(philo->right, info, flag);
		usleep(500);
		pthread_mutex_lock(info_mutex);
	}
	pthread_mutex_unlock(info_mutex);
}

void wait_to_eat(t_philo *philo, long time_to_die)
{
	// if (philo->nbr == 9)
		// printf("\t\t\t51 lock\n");
	pthread_mutex_lock(&philo->eat_mutex);
	while (philo->waiting_to_eat)
	{
		// printf("%d eat loop\n", philo->nbr);
		pthread_mutex_unlock(&philo->eat_mutex);
		// if (philo->nbr == 9)
			// printf("\t\t\t51 unlock\n");
		if (last_time_ate(philo) > time_to_die)
		{
			// printf("philo=%d\n", philo->nbr);
			// printf("%d->eating=%d\n", philo->right->nbr, philo->right->eating);
			// printf("%d->eating=%d\n", philo->left->nbr, philo->left->eating);
			start_dying(philo);
		}
		if (all_alive(philo) == 0)
			exit(0);
		// if (philo->nbr == 9)
			// printf("\t\t\t52 lock\n");
		pthread_mutex_lock(&philo->eat_mutex);
	}
	philo->waiting_to_eat = 1;
	pthread_mutex_unlock(&philo->eat_mutex);
	// if (philo->nbr == 9)
		// printf("\t\t\t52 unlock\n");
}

void wait_to_talk(t_philo *philo, long time_to_die)
{
	pthread_mutex_lock(philo->info_mutex);
	while (philo->info->talk_perms == 0)
	{
		// printf("talk loop\n");
		pthread_mutex_unlock(philo->info_mutex);
		usleep(25);
		if (last_time_ate(philo) > time_to_die)
			start_dying(philo);
		if (all_alive(philo) == 0)
			exit(0);
		pthread_mutex_lock(philo->info_mutex);
	}
	philo->info->talk_perms = 0;
	pthread_mutex_unlock(philo->info_mutex);
}

void let_other_talk(t_philo *philo)
{
	pthread_mutex_lock(philo->info_mutex);
	philo->info->talk_perms = 1;
	pthread_mutex_unlock(philo->info_mutex);
}

void better_usleep(t_philo *philo, long time_to_sleep, int flag)
{
	long time;

	time = total_time();
	while (total_time() - time < time_to_sleep - 100)
	{
		// printf("%d %ld - %ld < %ld\n", philo->nbr, total_time(), time, time_to_sleep - 100);
		usleep(100);
		if (all_alive(philo) == 0)
		{
			set_eating(philo, 0);
			exit(0);
		}
		if (flag == 0 && last_time_ate(philo) > philo->info->time_to_die)
			start_dying(philo);
	}
	time_to_sleep = time_to_sleep - (total_time() - time);
	if (time_to_sleep > 0)
		usleep(time_to_sleep);
}

void go_eat(t_philo *philo, int time_to_die)
{
	long	time;

	pthread_mutex_lock(philo->info_mutex);
	time = philo->info->time_to_eat;
	pthread_mutex_unlock(philo->info_mutex);
	wait_to_eat(philo, time_to_die);
	if (last_time_ate(philo) > time_to_die)
	{
		set_eating(philo, 0);
		start_dying(philo);
	}
	if (all_alive(philo) == 0)
	{
		set_eating(philo, 0);
		exit(0);
	}
	wait_to_talk(philo, time_to_die);
	set_eating(philo, 1);
	printf("%ld %d has taken a fork\n", total_time() / KILO, philo->nbr);
	printf("%ld %d has taken a fork\n", total_time() / KILO, philo->nbr);
	printf("%ld %d is eating for %ldml\n", total_time() / KILO, philo->nbr, time / KILO);
	let_other_talk(philo);
	better_usleep(philo, time, 1);
	set_eating(philo, 0);
	gettimeofday(&philo->lta, NULL);
}

int	check_times_ate(t_philo *philo, int value)
{
	int	ret;

	while (philo->nbr != 1)
		philo = philo->left;
	philo = philo->left;
	pthread_mutex_lock(&philo->left->eat_mutex);
	pthread_mutex_lock(&philo->eat_mutex);
	ret = (philo->times_ate >= value) && (philo->left->times_ate >= value);
	pthread_mutex_unlock(&philo->left->eat_mutex);
	pthread_mutex_unlock(&philo->eat_mutex);
	return (ret);
}

void	go_think(t_philo *philo, long time_to_die)
{
	long	time;

	pthread_mutex_lock(philo->info_mutex);
	time = philo->info->time_to_eat - last_time_ate(philo);
	pthread_mutex_unlock(philo->info_mutex);
	wait_to_talk(philo, time_to_die);
	printf("%ld %d is thinking for %ldml\n", total_time() / KILO, philo->nbr, time / KILO);
	let_other_talk(philo);
	better_usleep(philo, time, 0);
}

void	go_sleep(t_philo *philo, long time_to_die)
{
	long	time;

	pthread_mutex_lock(philo->info_mutex);
	time = philo->info->time_to_sleep;
	pthread_mutex_unlock(philo->info_mutex);
	wait_to_talk(philo, time_to_die);
	printf("%ld %d is sleeping for %ldml\n", total_time() / KILO, philo->nbr, time / KILO);
	let_other_talk(philo);
	better_usleep(philo, time, 0);
}

void	*run_code(void *var)
{
	t_philo	*philo;
	int		time_to_die;
	int		notepme;

	philo = (t_philo *)var;
	if (philo->info->nbr_of_philo == 1)
	{
		usleep(philo->info->time_to_die);
		return (printf("%ld 1 died\n", total_time() / KILO), NULL);
	}
	pthread_mutex_lock(philo->info_mutex);
	notepme = philo->info->notepme;
	time_to_die = philo->info->time_to_die;
	pthread_mutex_unlock(philo->info_mutex);
	gettimeofday(&philo->lta, NULL);
	while (all_alive(philo))
	{
		go_eat(philo, time_to_die);
		if (notepme != -1 && check_times_ate(philo, notepme))
			exit(0);
		go_sleep(philo, time_to_die);
		go_think(philo, time_to_die);
	}
	return (NULL);
}

int init_infosophers(t_info *info, pthread_mutex_t *info_mutex)
{
	int			ind;
	t_philo		*philo;
	pthread_t	*nof;

	nof = malloc(info->nbr_of_philo * sizeof(pthread_t));
	if (nof == NULL)
		return (1);
	philo = init_philo_and_mutex(info->nbr_of_philo, info, info_mutex);
	if (philo == NULL)
		return (free(nof), 1);
	// while (philo->nbr != philo->info->nbr_of_philo)
	// {
	// 	printf("philo->nbr=%d\n", philo->nbr);
	// 	philo = philo->right;
	// }
	// printf("philo->nbr=%d\n", philo->nbr);
	// return (0);
	ind = -1;
	total_time();
	while (++ind < info->nbr_of_philo)
	{
		if (pthread_create(nof + ind, NULL, run_code, philo))
		{
			return (ft_philoclear(philo), free(nof), 1);
		}
		// pthread_detach(nof[ind]);
		philo = philo->right;
	}
	// sleep(1);
	hypervise(philo, info, info_mutex);
	while (ind-- > 0)
	{
		pthread_join(nof[ind], NULL);
		printf("closing %d\n", ind);	
	}
	return (free(nof), ft_philoclear(philo), 0);
}

int main(int ac, char **av)
{
	pthread_mutex_t info_mutex;
	t_info info;

	if (ac != 5 && ac != 6)
		return (write(2, "invalid number of arguments\n", 28));
	if (int_info(ac, av, &info))
		return (1);
	if (exit_message(&info))
		return (1);
	if (pthread_mutex_init(&info_mutex, NULL))
		return (1);
	printf("nbr_of_philo:%d\n", info.nbr_of_philo);
	printf("time_to_die:%d\n", info.time_to_die / KILO);
	printf("time_to_eat:%d\n", info.time_to_eat / KILO);
	printf("time_to_sleep:%d\n", info.time_to_sleep / KILO);
	printf("notepme:%d\n", info.notepme);
	printf("\nstarting now\n");
	init_infosophers(&info, &info_mutex);
	pthread_mutex_destroy(&info_mutex);
	printf("ending now\n");
	return (0);
}
