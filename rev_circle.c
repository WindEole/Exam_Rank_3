/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rev_circle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iderighe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 15:50:34 by iderighe          #+#    #+#             */
/*   Updated: 2022/01/06 12:51:25 by iderighe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct s_set
{
	int		w;
	int		h;
	char	bg;
}	t_set;

typedef struct s_form
{
	char	type;
	float	x;
	float	y;
	float	radius;
	char	cara;
}	t_form;

int	ft_error(char *s)
{
	int	i;

	i = -1;
	while (s[++i])
		write(1, &s[i], 1);
	return (1);
}

int	ft_clear(FILE *file, char *win, char *s)
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

void	ft_draw(t_set *set, char *win)
{
	int	i;

	i = 0;
	while (i < (set->h))
	{
		write(1, win + (i * set->w), set->w);
		write(1, "\n", 1);
		i++;
	}
}

int	ft_circle(float x, float y, t_form *form)
{
	float	distance;

	distance = sqrtf(powf((x - form->x), 2)  + powf((y - form->y), 2));
	if (distance <= form->radius)
	{
		if ((form->radius - distance) < 1.00000000)
			return (2);
		return (1);
	}
	return (0);
}

void	ft_put_shape_to_win(t_set *set, t_form *form, char *win)
{
	
	int	i;
	int	j;
	int	what;

	i = 0;
	while (i < set->h)
	{
		j = 0;
		while (j < set->w)
		{
			what = ft_circle((float)j, (float)i, form);
			if ((form->type == 'c' && what == 2) || (form->type == 'C' && what))
				win[(i * set->w) + j] = form->cara;
			j++;
		}
		i++;
	}
}

int	ft_get_shapes(FILE *file, char *win, t_set *set)
{
	t_form	form;
	int		r;

	while ((r = fscanf(file, "%c %f %f %f %c\n", &form.type, &form.x, &form.y, &form.radius, &form.cara)) == 5)
	{
		if (form.radius <= 0.00000000 || (form.type != 'c' && form.type != 'C'))
			return (0);
		ft_put_shape_to_win(set, &form, win);
	}
	if (r != -1)
		return (0);
	return (1);
}

char	*ft_window(FILE *file, t_set *set)
{
	char	*win;
	int		i;

	if (fscanf(file, "%d %d %c\n", &set->w, &set->h, &set->bg) != 3)
		return(NULL);
	if (set->w <= 0 || set->h <= 0 || set->w > 300 || set->h > 300)
		return(NULL);
	win = (char *)malloc(sizeof(*win) * (set->w * set->h));
	if (win == NULL)
		return (NULL);
	i = 0;
	while (i < (set->w * set->h))
		win[i++] = set->bg;
	return (win);
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
		return (ft_error("Error: arguments\n"));
	if (!(file = fopen(av[1], "r")))
		return (ft_error("Error: Operation file corrupted\n"));
	if (!(win = ft_window(file, &set)))
		return (ft_clear(file, NULL, "Error: Operation file corrupted\n"));
	if (!(ft_get_shapes(file, win, &set)))
		return (ft_clear(file, win, "Error: Operation file corrupted\n"));
	ft_draw(&set, win);
	ft_clear(file, win, NULL);
	return (0);
}
