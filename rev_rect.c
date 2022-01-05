/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   revision.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iderighe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/30 14:15:06 by iderighe          #+#    #+#             */
/*   Updated: 2022/01/05 15:49:49 by iderighe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
	float	la;
	float	lo;
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
	if (((x < form->x || (form->x + form->la < x)) || (y < form->y))
		|| (form->y + form->lo < y))
		return (0);
	if (((x - form->x < 1.00000000) || ((form->x + form->la) -x < 1.00000000))
		|| ((y - form->y < 1.00000000 || ((form->y + form->lo) - y < 1.00000000))))
		return (2);
	return (1);
}

void	ft_put_shapes_to_win(t_set *set, char **win, t_form *form)
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
			what = ft_rectangle(j, i, form);
if (what)
	printf("what = [%d]\n", what);
			if ((form->type == 'r' && what == 2) || (form->type == 'R' && what))
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

	while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &form.type, &form.x, &form.y, &form.la, &form.lo, &form.cara)) == 6)
	{
		if (form.la <= 0 || form.lo <= 0 || (form.type != 'r' && form.type != 'R'))
			return (0);
		ft_put_shapes_to_win(set, win, &form);
	}
	if (ret != -1)
		return (0);
	return (1);
}

char	*ft_windows(FILE *file, t_set *set)
{
	char	*win_tmp;
	int		i;

	if (fscanf(file, "%d %d %c\n", &set->w, &set->h, &set->bg) != 3)
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
		return (ft_error("Error: arguments\n"));
	if (!(file = fopen(av[1], "r")))
		return (ft_error("Error: Operation file corrupted\n"));
	if (!(win = ft_windows(file, &set)))
		return (ft_free_all(file, NULL, "Error: Operation file corrupted\n"));
	if (!(ft_get_shapes(file, &set, &win)))
		return (ft_free_all(file, win, "Error: Operation file corrupted\n"));
	ft_draw(&set, win);
	ft_free_all(file, win, NULL);
	return (0);
}
