/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adouay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 16:12:01 by adouay            #+#    #+#             */
/*   Updated: 2022/06/05 17:18:45 by adouay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdio.h>

typedef struct s_fracrol{

	void	*mlx;
	void	*mlx_win;
	double	min_r;
	double	max_r;
	double	min_i;
	double	max_i;
}	t_fractol;

typedef struct s_img{

	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_img;

void	my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	render_pixel(t_img *img, double c_r, double c_i, int x, int y)
{
	double	z_r = 0;
	double	z_i = 0;
	double	i = 0;
	double	tmp;

	while ((z_r*z_r + z_i*z_i) < 4 && i < 80)
	{
		tmp = z_r;
		z_r = z_r*z_r - z_i*z_i + c_r;
		z_i = 2 * z_i * tmp + c_i;
		i++;
	}
	if (i == 80)
		my_mlx_pixel_put(img, x, y, 0x00000000);
	else
		my_mlx_pixel_put(img, x, y, 0xFFFFFF + (i * 255 / 80));
	return ;
}

void	boucle(t_img *img, t_fractol *fract, int width, int height)
{
	int		x;
	int		y;
	double	c_r;
	double	c_i;

	y = 0;
	while (y < height)
	{
		x = 0;
		while (x < width)
		{
			c_r = fract->min_r + x * (fract->max_r - fract->min_r) / width;
			c_i = fract->min_i + y * (fract->max_i - fract->min_i) / height;
			render_pixel(img, c_r, c_i, x, y);
			x++;
		}
		y++;
	}
	return ;
}

int	zoom(int keycode, t_fractol *fract)
{
	if (keycode == 4)
	{	
	//	fract->min_r = fract->max_r + 0.5 * (fract->min_r - fract->max_r);
	//	fract->max_r = fract->max_r + ((fract->min_r - fract->max_r) - 0.5 * (fract->min_r - fract->max_r)) / 2;
	//	fract->min_i = fract->max_i + 0.5 * (fract->min_i - fract->max_i);
	//	fract->max_i = fract->max_i + ((fract->min_i - fract->max_i) - 0.5 * (fract->min_i - fract->max_i)) / 2;
		printf("SCROLL UP\n");
	}
	if (keycode == 5)
	{
	//	fract->min_r = fract->max_r + 2 * (fract->min_r - fract->max_r);
	//	fract->max_r = fract->max_r + ((fract->min_r - fract->max_r) - 2 * (fract->min_r - fract->max_r)) / 2;
    //	fract->min_i = fract->max_i + 2 * (fract->min_i - fract->max_i);
	//	fract->max_i = fract->max_i + ((fract->min_i - fract->max_i) - 2 * (fract->min_i - fract->max_i)) / 2;
		printf("SCROLL DOWN\n");
	}
	return (0);
}

int	main(void)
{
	int			width;
	int			height;
	t_fractol	fract;
	t_img		img;

	width = 1920;
	height = 1080;
	fract.mlx = mlx_init();
	fract.mlx_win = mlx_new_window(fract.mlx, width, height, "Hello World!");
	img.img = mlx_new_image(fract.mlx, 1920, 1080);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	fract.min_r = -2.2;
	fract.max_r = 1.0;
	fract.min_i = -1.0;
	fract.max_i = 1.0;
	boucle(&img, &fract, width, height);
	mlx_mouse_hook(fract.mlx_win, zoom, &fract);
	mlx_put_image_to_window(fract.mlx, fract.mlx_win, img.img, 0, 0);
	mlx_loop(fract.mlx);
	return (0);
}
