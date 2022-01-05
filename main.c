/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iderighe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/28 14:14:59 by iderighe          #+#    #+#             */
/*   Updated: 2021/12/30 10:56:33 by iderighe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> // write
#include <stdio.h> // fopen, fread, fscanf, fclose
#include <stdlib.h> // malloc, calloc, realloc, free
#include <string.h> // memset
#include <math.h> // powf, sqrtf

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[1;34m"
# define PURPLE "\033[1;35m"
# define CYAN "\033[1;36m"
# define GREY "\033[1;37m"
# define RESET "\033[0m"

typedef struct s_set
{
	int		w;
	int		h;
	char	bg;
}	t_set;

typedef struct s_form
{
	char	type; // minipaint : r ou R / micro_paint : c ou C
	float	x;    // position horizontale du coin haut gauche du rectangle
	float	y;    // position horizontale du coin haut gauche du rectangle
//	float	d;    // diametre du cercle (micro_paint)
	float	l;    // largeur du rectangle (minipaint)
	float	h;    // hauteur du rectangle (minipaint)
	char	cara; // caractere utilise pour le dessin
}	t_form;

int	ft_free_all(FILE *file, char *win, char *s)
{
	int	i;

	if (file)
		fclose(file);
	if (win)
		free(win);
	if (s)
	{
		i = -1;
		while (s[++i])
			write(1, &s[i], 1);
	}
	return (1);
}

int	ft_error(char *s, int ret)
{
	int	i;

	i = -1;
	while (s[++i])
		write(1, &s[i], 1);
	return (ret);
}

void	ft_draw(t_set *set, char *win)
{
	int	i;

	i = 0;
	while (i < set->h)
	{
		write(1, win + (i * set->w), set->w);
		write(1, "\n", 1);
		i++;
	}
}

int	ft_rectangle(float x, float y, t_form *form)
{
	if (((x < form->x || (form->x + form->l < x)) || (y < form->y))
		|| (form->y + form->h < y))
		return (0);
	if (((x - form->x < 1.00000000) || ((form->x + form->l) - x < 1.00000000))
		|| ((y - form->y < 1.00000000 || ((form->y + form->h) - y < 1.00000000))))
		return (2);
	return (1);
}

//////////    IF CIRCLE     //////////////

/*int	ft_circle(float x, float y, t_form *form)
{
	float	distance;

	distance = sqrtf(powf(x - form->x, 2.) + powf(y - form->y, 2.));
	if (distance <= form->d)
	{
		if ((form->d - distance) < 1.00000000)
			return (2);
		return (1);
	}
	return (0);
}*/

void	ft_put_shapes_to_win(t_set *set, char **win, t_form *form)
{
	int	i;
	int	j;
	int	res;

	i = 0;
	while (i < set->h)
	{
		j = 0;
		while (j < set->w)
		{
			res = ft_rectangle(j, i, form);
//			ret = ft_circle((float)j, (float)y, form);    //// IF CIRCLE
			if ((form->type == 'r' && res == 2) || (form->type == 'R' && res))
				(*win)[(i * set->w) + j] = form->cara;
			j++;
		}
		i++;
	}
}

int	ft_get_shapes(FILE *file, t_set *set, char **win)
{
	t_form	form;
	int		ret;

	while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &form.type, &form.x, &form.y, &form.l, &form.h, &form.cara)) == 6)
	{
		if (form.l <= 0 || form.h <= 0 || (form.type != 'r' && form.type != 'R'))
			return (0);
		ft_put_shapes_to_win(set, win, &form);
	}
	if (ret != -1)
		return (0);
	return (1);
}

char	*ft_window(FILE *file, t_set *set)
{
	char	*win_tmp;
	int		i;

	if (fscanf(file, "%d %d %c\n", &set->w, &set->h, &set->bg) != 3
	|| fscanf(file, "%d %d %c\n", &set->w, &set->h, &set->bg) == -1)
		return (NULL);
	if (set->w <= 0 || set->w > 300 || set->h <= 0 || set->h > 300)
		return (NULL);
	win_tmp = (char *)malloc(sizeof(*win_tmp) * (set->w * set->h));
	if (win_tmp == NULL)
		return (NULL);
	i = 0;
	while (i < (set->w * set->h))
		win_tmp[i++] = set->bg;
	return (win_tmp);
}

int	main(int ac, char **av)
{
	FILE	*file;
	t_set	set;
	char	*win;

	set.w = 0;
	set.h = 0;
	set.bg = 0;
	win = NULL;
	if (ac != 2)
		return (ft_error("Error: arguments\n", 1));
	if (!(file = fopen(av[1], "r")))
		return (ft_error("Error: Operation file corrupted\n", 1));
	if (!(win = ft_window(file, &set)))
		return (ft_free_all(file, NULL, "Error: Operation file corrupted\n"));
	if (!(ft_get_shapes(file, &set, &win)))
		return (ft_free_all(file, win, "Error: Operation file corrupted\n"));
	ft_draw(&set, win);
	ft_free_all(file, win, NULL);
	return (0);
}
