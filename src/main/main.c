#include "philosophers.h"

void *f_hilo(void *args) 
{
	t_philosophers	*vars;

	vars = (t_philosophers *) args;
	printf("%d, %d, %d\n", vars->time_to_die, vars->time_to_eat, vars->time_to_sleep);
	printf("hola luci te quiero =)\n\n");
	return (NULL);
}

int	verify_args(int argc, char **argv)
{
	if (argc != 3 && argc != 4)
	{
		write(2, "the number of arguments is incorrect\n", 38);
		return (1);
	}
	while (*argv)
	{
		if (ft_arr_isdigit(*argv) == 1)
		{
			write(2, "the type of arguments are incorrect\n", 37);
			return (1);
		}
		argv ++;
	}
	return (0);
}


int	create_philo_struct(int argc, char **argv, t_philosophers	*vars)
{
	int				err;
	t_ldictionary	*id_fork;
	int				n_threads;

	err = 0;
	(*vars).time_to_die = ft_atoi_chetao(argv[1], &err);
	if (err)
		return (1);
	(*vars).time_to_eat = ft_atoi_chetao(argv[2], &err);
	if (err)
		return (1);
	if (argc == 4)
		(*vars).time_to_sleep = ft_atoi_chetao(argv[3], &err);
	if (err)
		return (1);
	n_threads = ft_atoi_chetao(argv[0], &err);
	id_fork = (void *)malloc(sizeof(t_ldictionary));
	while (n_threads --)
	{
		l_dictionary_add_back(&id_fork, l_dictionary_new((pthread_t )-2, 0));
	}
	return (0);
}

int	create_threads(int threads, t_philosophers	*vars)
{
	int				creacion;
	int				counter;
	int				aux;
	int				espera;

	aux = threads;
	counter = 0;
	while (threads)
	{
		creacion = pthread_create(&(vars->id_fork->key), NULL, f_hilo, vars);
		if (creacion)
			return (write(2, "pthread_create fail\n", 21), 1);
		counter ++;
		threads --;
	}
	while (aux --)
	{
		espera = pthread_join(vars->id_fork->key, NULL);
		if (espera)
			return (write(2, "wait pthread fail\n", 19), 1);
		vars->id_fork = vars->id_fork->next;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_philosophers	vars;
	int				num_threads;
	int				err;

	if (verify_args(argc - 1, (argv + 1)))
		return (1);
	if (create_philo_struct(argc - 1, argv + 1, &vars))
		return (write(2, "atoi fail\n", 11), 1);
	num_threads = ft_atoi_chetao(argv[1], &err);
	if (err)
		return (write(2, "atoi fail\n", 11), 1);
	if (create_threads(num_threads, &vars))
		return (1);
	return (0);
}
