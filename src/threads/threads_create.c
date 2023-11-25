#include "philo.h"

int	im_dead_(t_2link_circ_list *vars)
{
	// printf("mutex == %p\n ", (vars->mutex_im_dead));
	pthread_mutex_lock((vars->mutex_im_dead));
	if (*(vars->dead) == 1)
	{
		//sleep(5);
		pthread_mutex_unlock((vars->mutex_im_dead));
		return (1);
	}
	pthread_mutex_unlock((vars->mutex_im_dead));
	return (0);
}

int	actions(t_2link_circ_list *vars)
{
	if (eating(vars))
		return (1);
	if (im_dead_(vars))
		return (1);
	thinking(vars);
	if (im_dead_(vars))
		return (1);
	if (sleeping(vars))
		return (1);
	if (im_dead_(vars))
		return (1);
	else
		return (0);
}

void	number_actions(t_2link_circ_list *vars)
{
	int	n_times;

	pthread_mutex_lock(vars->mutex_all_sit);
	*(vars->all_sit) = (*(vars->all_sit)) + 1;
	//printf("actions %d\n", (*(vars->all_sit)));
	pthread_mutex_unlock(vars->mutex_all_sit);
	// pthread_mutex_lock(&(vars->mutex.im_arriving));
	// vars->arriving_philos = 1;
	// pthread_mutex_unlock(&(vars->mutex.im_arriving));
	//wait_to_sit(vars);

	if (vars->routine.number_of_times >= 0)
	{
		n_times = vars->routine.number_of_times;
		while (n_times)
		{
			if (actions(vars))
				break ;
			n_times --;
		}
	}
	else
	{
		while (1)
		{
			if (actions(vars))
				break ;
		}
	}
	pthread_mutex_lock(&(vars->mutex.threads_ended));
	vars->threads_ended ++;
	pthread_mutex_unlock(&(vars->mutex.threads_ended));
}

void	*f_hilo(void *args)
{
	t_2link_circ_list	*vars;

	vars = (t_2link_circ_list *) args;
	//usleep(20000);
	pthread_mutex_lock(&(vars->mutex.t_start_eating));
	gettimeofday(&(vars->start_eating), NULL);
	pthread_mutex_unlock(&(vars->mutex.t_start_eating));
	pthread_mutex_lock(&(vars->next->mutex.id));
	if (is_impar(vars->id_fork.id))
	{
		ft_usleep(200, vars);
	}
	pthread_mutex_unlock(&(vars->next->mutex.id));
	pthread_mutex_lock(&(vars->mutex.t_born_philo));
	gettimeofday(&(vars->born_philo), NULL);
	pthread_mutex_unlock(&(vars->mutex.t_born_philo));
	number_actions(vars);
	return (NULL);
}

int	check_if_finish(t_2link_circ_list *vars)
{
	int	counter;
	int	n_philo_finished;

	n_philo_finished = 0;
	counter = vars->routine.n_philos;
	while (counter)
	{
		pthread_mutex_lock(&(vars->mutex.threads_ended));
		if (vars->threads_ended == 1)
			n_philo_finished ++;
		pthread_mutex_unlock(&(vars->mutex.threads_ended));
		vars = vars->next;
		counter --;
	}
	if (n_philo_finished == vars->routine.n_philos)
		return (1);
	return (0);
}

// void	kill_all(t_2link_circ_list *vars)
// {
// 	int	number_philos;

// 	number_philos = vars->routine.n_philos;
// 	while (number_philos)
// 	{
// 		pthread_mutex_lock(&(vars->mutex_im_dead));
// 		vars->im_dead = 1;
// 		pthread_mutex_unlock(&(vars->mutex_im_dead));
// 		vars = vars->next;
// 		number_philos--;
// 	}
// }

void	calculate_thread_death(t_2link_circ_list *vars)
{
	long long		time_elapsed;
	struct timeval	end;
	long int		time_that_philo_lived;

	while (1)
	{
		gettimeofday(&end, NULL);
		pthread_mutex_lock(&(vars->mutex.t_start_eating));
		time_elapsed = ((end.tv_sec - vars->start_eating.tv_sec) * 1000) + \
			((end.tv_usec - vars->start_eating.tv_usec) / 1000);
		pthread_mutex_unlock(&(vars->mutex.t_start_eating));
		//printf("%lld: %d\n", time_elapsed, vars->id_fork.id);
		if ((long long) vars->routine.time_to_die < time_elapsed)
		{
			pthread_mutex_lock((vars->mutex_im_dead));
			if (*(vars->dead) == 0)
				*(vars->dead) = 1;
			pthread_mutex_unlock((vars->mutex_im_dead));
			gettimeofday(&end, NULL);
			time_that_philo_lived = ((end.tv_sec - vars->born_philo.tv_sec) * 1000) + \
			((end.tv_usec - vars->born_philo.tv_usec) / 1000);
			printf("%s[%ld ms] %d is dead %s\n", RED, time_that_philo_lived, vars->id_fork.id, FN);
			break ;
		}
		if (check_if_finish(vars))
		{
			break ;
		}
		vars = vars->next;
		ft_usleep(1000, vars);
	}
}

void	wait_to_sit(t_2link_circ_list *vars)
{

	while (1)
	{
		pthread_mutex_lock((vars->mutex_all_sit));
		//printf("aaa aa %d %d\n", (*(vars->all_sit)), vars->routine.n_philos);
		if ((*(vars->all_sit)) >= vars->routine.n_philos)
		{
			//printf("mecahis\n");
			pthread_mutex_unlock((vars->mutex_all_sit));
			break;
		}
		pthread_mutex_unlock((vars->mutex_all_sit));
	}
}

int	create_threads(int n_threads, t_2link_circ_list *vars, pthread_t *threads)
{
	int					counter;
	int					aux;
	int					espera;

	counter = 0;
	aux = n_threads;
	while (aux)
	{
		if (pthread_create(&threads[counter], NULL, f_hilo, vars))
			return (perror("pthread_create fail"), 1);
		vars = vars->next;
		aux --;
		counter ++;
	}
	wait_to_sit(vars);
	calculate_thread_death(vars);
	counter = 0;
	while (n_threads)
	{
		espera = pthread_join(threads[counter], NULL);
		if (espera)
			return (perror("wait pthread fail"), 1);
		n_threads --;
		counter ++;
	}
	return (0);
}
