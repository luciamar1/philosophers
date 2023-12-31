/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_create.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucia-ma <lucia-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 19:08:33 by lucia-ma          #+#    #+#             */
/*   Updated: 2023/12/19 20:22:34 by lucia-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	actions(t_2link_circ_list *vars)
{
	if (im_dead_(vars))
		return (1);
	if (eating(vars))
	{
		return (1);
	}
	if (im_dead_(vars))
		return (1);
	if (sleeping(vars))
		return (1);
	if (im_dead_(vars))
		return (1);
	if (thinking(vars))
		return (1);
	return (0);
}

void	number_actions(t_2link_circ_list *vars)
{
	int	n_times;

	pthread_mutex_lock(vars->mutex_all_sit);
	*(vars->all_sit) = (*(vars->all_sit)) + 1;
	pthread_mutex_unlock(vars->mutex_all_sit);
	if (vars->routine.number_of_times >= 0)
	{
		if (im_dead_(vars))
			return ;
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
		if (im_dead_(vars))
			return ;
		while (1)
			if (actions(vars))
				break ;
	}
}

void	*f_hilo(void *args)
{
	t_2link_circ_list	*vars;

	vars = (t_2link_circ_list *) args;
	pthread_mutex_lock(&(vars->mutex.t_start_eating));
	gettimeofday(&(vars->start_eating), NULL);
	pthread_mutex_unlock(&(vars->mutex.t_start_eating));
	pthread_mutex_lock(&(vars->mutex.id));
	if (is_impar(vars->id_fork.id))
	{
		ft_usleep(200, vars);
	}
	pthread_mutex_unlock(&(vars->mutex.id));
	gettimeofday(&(vars->born_philo), NULL);
	number_actions(vars);
	return (NULL);
}

void	error_pthread_create(t_2link_circ_list *vars, int counter, \
	pthread_t *threads)
{
	int	wait_pthread;

	printf("counter %d\n", counter);
	while (counter)
	{
		printf("limiando\n");
		wait_pthread = pthread_join(threads[counter], NULL);
		if (wait_pthread)
		{
			printf("peta\n");
			pthread_mutex_lock((vars->mutex_im_dead));
			if (*(vars->dead) == 0)
				*(vars->dead) = 1;
			pthread_mutex_unlock((vars->mutex_im_dead));
			perror("wait pthread fail");
			return ;
		}
		counter --;
	}
}

int	create_threads(int n_threads, t_2link_circ_list *vars, pthread_t *threads)
{
	int	counter;
	int	aux;
	int	espera;

	aux = ((counter = 0), n_threads);
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
