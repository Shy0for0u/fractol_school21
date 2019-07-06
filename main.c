/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgorold- <dgorold-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 17:18:26 by dgorold-          #+#    #+#             */
/*   Updated: 2019/07/06 23:15:38 by dgorold-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/fractol.h"

void		alert(int key)
{
	if (key == 1)
	{
		ft_putstr("usage: ./fractol name_of_map");
	}
}

static void		initialization(t_fcl *fcl, char *file)
{
	fcl->ms.prev_y = 0;
	fcl->ms.prev_x = 0;
	fcl->ms.stopmove = 0;
	fcl->zoom = 1;
	fcl->step = 0.5;
	fcl->file = file;
	fcl->max_iter = 300;
	fcl->width = 800;
	fcl->height = 800;
}

int 			color_func(int it, double off)
{
	double		res;

	res = cos(sqrt(it) + off);
	return (255 * res * res);
}

void			pixel_to_img(t_fcl *fcl, int x, int y, t_color c)
{
	int 		index;

	index = x * 4 + fcl->img.s_l * y;
	fcl->img.data[index] = c.b;
	fcl->img.data[index + 1] = c.g;
	fcl->img.data[index + 2] = c.r;
}

void			set_color(int iter, int x, int y, t_fcl *fcl)
{
	t_color		color;

	color.r = 0;
	color.g = 0;
	color.b = 0;
	if (iter < fcl->max_iter)
	{
		color.r = (char)color_func(iter, 0);
		color.g = (char)color_func(iter, 0.1 * fcl->step);
		color.b = (char)color_func(iter, 0.2 * fcl->step);
	}
	pixel_to_img(fcl, x, y, color);
}

void			julia(t_fcl *fcl)
{
	double	cre;
	double	cim;

	double	newre;
	double	newim;
	double	oldre;
	double	oldim;
	double	movex;
	double	movey;

	int 	i;

	int x;
	int y;
	y = 0;
	movex = 0;
	movey = 0;
	cre = 2 * fcl->ms.prev_x / fcl->height;
	cim = 2 * fcl->ms.prev_y / fcl->width;
	while (y < fcl->height)
	{
		x = 0;
		while (x < fcl->width)
		{
			newre = 1.5 * (x - (double)fcl->width / 2) / (0.5 * fcl->zoom * fcl->width) + movex;
			newim = (y - (double)fcl->height / 2) / (0.5 * fcl->zoom * fcl->height) + movey;
			i = 0;
			while (i < fcl->max_iter)
			{
				if ((newre * newre + newim * newim) > 4)
					break ;
				oldre = newre;
				oldim = newim;
				newre = oldre * oldre - oldim * oldim + cre;
				newim = 2 * oldre * oldim + cim;

				i++;
			}
			set_color(i, x, y, fcl);
			x++;
		}
		y++;
	}
}

void			regular_works(t_fcl *fcl)
{
	if (!ft_strcmp(fcl->file, "julia"))
		julia(fcl);
	mlx_put_image_to_window(fcl->mlx_ptr, fcl->win_ptr, fcl->img.img_pntr, 0, 0);
}

int				deal_key(int key, t_fcl *fcl)
{
	fcl->img.data = mlx_get_data_addr(fcl->img.img_pntr,
										&fcl->img.bpp, &fcl->img.s_l, &fcl->img.endian);
	if (key == 53)
		exit(0);
	if (key == 46 && fcl->ms.stopmove)
	{
		fcl->ms.stopmove = 0;
	}
	if (key == 46)
	{
		fcl->ms.stopmove = 1;
	}
	regular_works(fcl);
	return (0);
}

int				mouse_press(int button, int x, int y, t_fcl *fcl)
{
	(void)x;
	(void)y;
	fcl->img.data = mlx_get_data_addr(fcl->img.img_pntr,
									  &fcl->img.bpp, &fcl->img.s_l, &fcl->img.endian);
	if (button == 4)
		fcl->zoom = fcl->zoom + 1;
	if (button == 5)
		fcl->zoom = fcl->zoom - 1;
	regular_works(fcl);
	return (0);
}

int 			mouse_move(int x, int y, t_fcl *fcl)
{
//	fcl->img.data = mlx_get_data_addr(fcl->img.img_pntr,
//									  &fcl->img.bpp, &fcl->img.s_l, &fcl->img.endian);
	t_mouse		*mouse;

	mouse = &fcl->ms;
	if ((x < 0 || x >= fcl->width || y < 0 || y >= fcl->height) && fcl->ms.stopmove == 0)
	{
		if (x >= fcl->width)
			x = fcl->width;
		if (x >= fcl->height)
			x = fcl->height;
		mouse->prev_x = x;
		mouse->prev_y = y;
//		mlx_clear_window(fcl->mlx_ptr, fcl);
		julia(fcl);
	}
	return (0);
}

static void		init_image(t_fcl fcl)
{
	fcl.mlx_ptr = mlx_init();
	fcl.img.img_pntr = mlx_new_image(fcl.mlx_ptr, fcl.width, fcl.height);
	fcl.img.data = mlx_get_data_addr(fcl.img.img_pntr,
									   &fcl.img.bpp, &fcl.img.s_l, &fcl.img.endian);

	fcl.win_ptr = mlx_new_window(fcl.mlx_ptr, fcl.width, fcl.height, fcl.file);
	mlx_hook(fcl.win_ptr, 2, 5, deal_key, &fcl);
	mlx_hook(fcl.win_ptr, 4, 5, mouse_press, &fcl);
	mlx_hook(fcl.win_ptr, 6, 5, mouse_move, &fcl);
//	mlx_mouse_hook(fcl.win_ptr, mouse_move, &fcl);
	mlx_loop(fcl.mlx_ptr);
}

int				main(int argc, char **argv)
{
	t_fcl	fcl;

	if (argc == 2)
	{
		initialization(&fcl, argv[1]);
		init_image(fcl);
	}
	else
		alert(1);
	return (0);
}