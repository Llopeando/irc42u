/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   betterMlx.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 14:05:15 by ecamara           #+#    #+#             */
/*   Updated: 2023/07/17 16:03:32 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BETTERMLX_H
#define BETTERMLX_H


#include "mlx.h"
#include "extensions/MersenneTwister.h"
#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>

#define BMLX_ERROR -1
# define THREADS 11

typedef struct s_image{
	void	*img;
	int		*add;
	int		bpp;
	int		w;
	int		h;
	int		endian;
	int		*color;
	int		check;
	uint32_t width;
	uint32_t height;
}t_image;

typedef struct s_pipeline{
	t_image imgBuffer;
	void		*(*renderPass)(void *);
	void		*(*simPass)(void *);
	void		(*clean)(void *);
	pthread_t	threads[THREADS];
}t_pipeline;

typedef struct s_pipelineCreateInfo{
	void		*(*renderPass)(void *);
	void		*(*simPass)(void *);
	void		(*clean)(void *);
	uint32_t	height;
	uint32_t	width;
}t_pipelineCreateInfo;

typedef struct s_timer{
	clock_t lastTime;
	uint32_t frames;
	float fps;
}t_timer;

typedef struct s_mlx
{
	void	*mlx;
	void	*win;
	t_pipeline pipeline;
}t_mlx;

void initMlx(t_mlx *mlx, char *name, t_pipelineCreateInfo *createInfo);

void bmlx_create_img(void *mlx, t_image *image, uint32_t WIDTH, uint32_t HEIGHT);
void bmlx_create_img_xpm(void *mlx, t_image *image,  char *path,int width, int height);

void bmlx_put_img_to_img(t_image *dest, t_image *src, uint32_t idx, uint32_t size, uint32_t width);

void random_m256_init(float *mem, uint32_t size, float min, float max, int seed);
void _m256_init(float *mem, uint32_t size, float value);

//void normalize_m256_2(float *x, float *y, uint32_t max);
void normalize_m256_2(float **xt, float **yt, uint32_t max);
void normalize_2(float *x, float *y, uint32_t max);

void bmlx_set_img_color_AVX(t_image *image, int color);
void bmlx_sub_img_color_AVX(t_image *image, int color);
void bmlx_add_img_color_AVX(t_image *image, int color);
void bmlx_mul_img_color_AVX(t_image *image, double value);

void bmlx_set_img_color(t_image *image, int color);
void bmlx_sub_img_color(t_image *image, int color);
void bmlx_mul_img_color(t_image *image, float value);

void drawRect(t_image *image, int posX, int posY, int sizeX, int sizeY, int col);
void drawRectWithPer(t_image *image, int posX, int posY, int sizeX, int sizeY, uint32_t width, int bodyCol, int perCol);

#endif