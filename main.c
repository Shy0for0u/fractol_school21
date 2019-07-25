/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgorold- <dgorold-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 17:18:26 by dgorold-          #+#    #+#             */
/*   Updated: 2019/04/29 19:02:35 by dgorold-         ###   ########.fr       */
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
	fcl->max_iter = 30;
	fcl->width = 400;
	fcl->height = 400;
    fcl->ms.movex = 0;
    fcl->ms.movey = 0;
}

int 			color_func(int it, float off)
{
	float		res;

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

void            foo(t_fcl *fcl, float oldim, float oldre, float *newre, float *newim)
{

    *newre = oldre * oldre - oldim * oldim + fcl->ms.prev_x / fcl->height * 0.5;
    *newim = 2 * oldre * oldim + fcl->ms.prev_y / fcl->width * 0.5;
}

int            julia(t_fcl *fcl, int y, int x)
{
    int 	i;
    float	newre;
    float	newim;
    float	oldre;
    float	oldim;

    newre = (1.5 * x - fcl->width * 0.75) / (0.5 * fcl->zoom * fcl->width) + fcl->ms.movex;
    newim = (y - fcl->height * 0.5) / (0.5 * fcl->zoom * fcl->height) + fcl->ms.movey;
    i = 0;
    while ((i < fcl->max_iter) && ((newre * newre + newim * newim) <= 4))
    {
        oldre = newre;
        oldim = newim;
        foo(fcl, oldim, oldre, &newre, &newim);
        i++;
    }
    return (i);
}

//void			*fractol(t_fcl *fcl)
void            *fractol(void *thread)
{
//    int 	    i;
    int       iteration;
    int         x;
    int         y;
    t_thread    *new;

    new = thread;
    y = new->index * HEIGHT / THREADS;


//    y = 0;
    while (y++ < HEIGHT / THREADS * (new->index + 1))
    {
        x = 0;
        while (x++ < WIDTH)
        {
            iteration = julia(new->foo0, y, x);
            if (iteration < new->foo0->max_iter)
                set_color(iteration, x, y, new->foo0);
        }
    }
    return (NULL);
}

void            pthread(t_fcl *fcl)
{
    t_thread    *thread;
    int         i = 0;

    thread = (t_thread*)malloc(sizeof(t_thread) * THREADS);

    while (i < THREADS)
    {
        thread[i].index = i;
        thread[i].foo0 = fcl;
        pthread_create(&(thread[i].point), NULL, fractol, (void*)&thread[i]);
        i++;
    }
    while (i--)
        pthread_join(thread[i].point, NULL);
    free(thread);
}

void			regular_works(t_fcl *fcl)
{
    mlx_clear_window(fcl->mlx_ptr, fcl->win_ptr);
//    fractol(fcl);
    pthread(fcl);
// https://github.com/pbondoer/42-fractol/blob/master/src/image.c

    mlx_put_image_to_window(fcl->mlx_ptr, fcl->win_ptr, fcl->img.img_pntr, 0, 0);
}

int				deal_key(int key, t_fcl *fcl)
{
    if (key == 65307)
        exit(0);
    if (key == 100 && fcl->ms.stopmove)
    {
        fcl->ms.stopmove = 0;
    }
    if (key == 122)
    {
        fcl->ms.stopmove = 1;
    }
    return (0);
}

int				mouse_press(int button, int x, int y, t_fcl *fcl) // opencl
{
//    (void)x;
//    (void)y;
    if (button == 4)
        fcl->zoom = fcl->zoom + 1;
    if (button == 5)
        fcl->zoom = fcl->zoom - 1;
    return (0);
}

int 			mouse_move(int x, int y, t_fcl *fcl)
{
//    (void)x;
//    (void)y;
    fcl->ms.prev_x = x;
    fcl->ms.prev_y = y;
    regular_works(fcl);
    return (0);
}

static void		init_image(t_fcl fcl)
{
    fcl.mlx_ptr = mlx_init();
    fcl.img.img_pntr = mlx_new_image(fcl.mlx_ptr, fcl.width, fcl.height);
    fcl.img.data = mlx_get_data_addr(fcl.img.img_pntr,
                                     &fcl.img.bpp, &fcl.img.s_l, &fcl.img.endian);

    fcl.win_ptr = mlx_new_window(fcl.mlx_ptr, fcl.width, fcl.height, fcl.file);
    mlx_hook(fcl.win_ptr, 6, PointerMotionMask, mouse_move, &fcl);
    mlx_hook(fcl.win_ptr, 2, 5, deal_key, &fcl);
    mlx_hook(fcl.win_ptr, 4, 5, mouse_press, &fcl);
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