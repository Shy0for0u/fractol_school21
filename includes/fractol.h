/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgorold- <dgorold-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 17:19:10 by dgorold-          #+#    #+#             */
/*   Updated: 2019/04/29 18:37:41 by dgorold-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_FRACTOL_H
# define FRACTOL_FRACTOL_H
# include "../libft/includes/libft.h"
# include "/usr/local/include/mlx.h"
# include "fcntl.h"
# include "math.h"

typedef struct s_image
{
	void		*img_pntr;
	char		*data;
	int			s_l; // size line
	int         bpp;
	int			endian;

}				t_image;

typedef struct	s_color
{
	char 		b;
	char 		g;
	char 		r;
}				t_color;

typedef struct	s_mouse
{
	double 		prev_x;
	double 		prev_y;
	int 		stopmove;
}				t_mouse;

typedef struct	s_fractol
{
	double		zoom;
	char 		*file;
	double 		step;
	int 		max_iter;
	char		*image_data;
	void		*mlx_ptr;
	void		*win_ptr;
	double		factor;
	int			width;
	int			height;
	t_mouse		ms;
	t_image		img;
}				t_fcl;

#endif //FRACTOL_FRACTOL_H
