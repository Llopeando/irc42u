#include "../betterMlx.h"
#include <immintrin.h>
#include "stdio.h"
#include <fcntl.h>
#include <stdlib.h>
#include <math.h>

void bmlx_create_img(void *mlx, t_image *image, uint32_t width, uint32_t height)
{
	image->img = mlx_new_image(mlx, width, height);
	image->add = (int *)mlx_get_data_addr(image->img,
			&image->bpp, &image->w,
			&image->endian);
	image->width = width;
	image->height = height;
}

void bmlx_create_img_xpm(void *mlx, t_image *image, char *path, int width, int height)
{
	image->img = mlx_xpm_file_to_image(mlx, path, &width, &height);
	image->add = (int *)mlx_get_data_addr(image->img,
			&image->bpp, &image->w,
			&image->endian);
	image->width = width;
	image->height = height;
}

void bmlx_set_img_color(t_image *image, int color)
{
	uint32_t size = image->width * image->height;
	for (uint32_t i = 0; i < size; i++) {
	    image->add[i] = color;
	}
}

void bmlx_sub_img_color(t_image *image, int color)
{
	uint32_t size = image->width * image->height;
	for (uint32_t i = 0; i < size; i++) {
		image->add[i] = image->add[i] - color;
	}
}

void bmlx_mul_img_color(t_image *image, float value)
{
	uint32_t size = image->width * image->height;
	for (uint32_t i = 0; i < size; i++) {
		image->add[i] = image->add[i] * value;
	}
}

void bmlx_set_img_color_AVX(t_image *image, int color)
{
	__m256i *img = (__m256i *)image->add;
	__m256i value = _mm256_set1_epi32(color);
	uint32_t size = image->width * image->height / 8;
	for (uint32_t i = 0; i < size; i++) {
	    img[i] = value;
	}
}

void bmlx_sub_img_color_AVX(t_image *image, int color)
{
	__m256i *img = (__m256i *)image->add;
	__m256i value = _mm256_set1_epi32(color);
	uint32_t size = image->width * image->height / 8;
	for (uint32_t i = 0; i < size; i++) {
		img[i] = _mm256_sub_epi32 (img[i], value);
	}
}

void bmlx_add_img_color_AVX(t_image *image, int color)
{
	__m256i *img = (__m256i *)image->add;
	__m256i value = _mm256_set1_epi32(color);
	uint32_t size = image->width * image->height / 8;
	for (uint32_t i = 0; i < size; i++) {
		img[i] = _mm256_add_epi32 (img[i], value);
	}
}

void bmlx_mul_img_color_AVX(t_image *image, double value)
{
	__m256 val = _mm256_set1_ps(value);
	uint32_t size = image->width * image->height / 8;
	__m256i *img = (__m256i *)image->add;
	for (uint32_t i = 0; i < size; i += 8) {
		img[i] = _mm256_cvtps_epi32(_mm256_mul_ps(_mm256_cvtepi32_ps(img[i]), val));
	}
}



void random_m256_init(float *mem, uint32_t size, float min, float max, int seed)
{
	mt19937_t mt;
	mt19937_init(&mt, seed);
	__m256 *mem256 = (__m256 *)mem;
	size /= 8;

	for (uint32_t i = 0; i < size; i++) {
		__m256 value = {mt19937_float_range(&mt, min, max),
						mt19937_float_range(&mt, min, max),
						mt19937_float_range(&mt, min, max),
						mt19937_float_range(&mt, min, max),
						mt19937_float_range(&mt, min, max),
						mt19937_float_range(&mt, min, max),
						mt19937_float_range(&mt, min, max),
						mt19937_float_range(&mt, min, max)};
		mem256[i] = value;
	}
}

void _m256_init(float *mem, uint32_t size, float value)
{
	__m256 *mem256 = (__m256 *)mem;

	size /= 8;
	__m256 value256 = _mm256_set1_ps(value);
	for (uint32_t i = 0; i < size; i++) {
		mem256[i] = value256;
	}
}

void initMlx(t_mlx *mlx, char *name, t_pipelineCreateInfo *createInfo)
{
	mlx->mlx = mlx_init();
	mlx->win = mlx_new_window(mlx->mlx, createInfo->width, createInfo->height, name);
	mlx->pipeline.renderPass = createInfo->renderPass;
	mlx->pipeline.simPass = createInfo->simPass;
	mlx->pipeline.clean = createInfo->clean;
	bmlx_create_img(mlx->mlx, &mlx->pipeline.imgBuffer, createInfo->width, createInfo->height);
}

__m256 square(__m256 x) {
	return _mm256_mul_ps(x, x);
}

void normalize_m256_2(float **xt, float **yt, uint32_t max)
{
	float *x = *xt;
	float *y = *yt;
	if (max % 32 != 0) {
        max = max - (max % 32);
    }
    for (uint32_t i = 0; i < max; i+=32){
        // Load 4 sets of data
        __m256 x1 = _mm256_load_ps(x + i);
        __m256 y1 = _mm256_load_ps(y + i);
        __m256 x2 = _mm256_load_ps(x + i + 8);
        __m256 y2 = _mm256_load_ps(y + i + 8);
        __m256 x3 = _mm256_load_ps(x + i + 16);
        __m256 y3 = _mm256_load_ps(y + i + 16);
        __m256 x4 = _mm256_load_ps(x + i + 24);
        __m256 y4 = _mm256_load_ps(y + i + 24);

        // Compute 4 sets of normalization
        __m256 len1 = _mm256_sqrt_ps(_mm256_add_ps(_mm256_mul_ps(x1, x1), _mm256_mul_ps(y1, y1)));
        __m256 len2 = _mm256_sqrt_ps(_mm256_add_ps(_mm256_mul_ps(x2, x2), _mm256_mul_ps(y2, y2)));
        __m256 len3 = _mm256_sqrt_ps(_mm256_add_ps(_mm256_mul_ps(x3, x3), _mm256_mul_ps(y3, y3)));
        __m256 len4 = _mm256_sqrt_ps(_mm256_add_ps(_mm256_mul_ps(x4, x4), _mm256_mul_ps(y4, y4)));
        __m256 inv_len1 = _mm256_div_ps(_mm256_set1_ps(1.0), len1);
        __m256 inv_len2 = _mm256_div_ps(_mm256_set1_ps(1.0), len2);
        __m256 inv_len3 = _mm256_div_ps(_mm256_set1_ps(1.0), len3);
        __m256 inv_len4 = _mm256_div_ps(_mm256_set1_ps(1.0), len4);
        __m256 x1_norm = _mm256_mul_ps(x1, inv_len1);
        __m256 y1_norm = _mm256_mul_ps(y1, inv_len1);
        __m256 x2_norm = _mm256_mul_ps(x2, inv_len2);
        __m256 y2_norm = _mm256_mul_ps(y2, inv_len2);
        __m256 x3_norm = _mm256_mul_ps(x3, inv_len3);
        __m256 y3_norm = _mm256_mul_ps(y3, inv_len3);
        __m256 x4_norm = _mm256_mul_ps(x4, inv_len4);
        __m256 y4_norm = _mm256_mul_ps(y4, inv_len4);

        // Store 4 sets of data
        _mm256_store_ps(x + i, x1_norm);
        _mm256_store_ps(y + i, y1_norm);
        _mm256_store_ps(x + i + 8, x2_norm);
        _mm256_store_ps(y + i + 8, y2_norm);
        _mm256_store_ps(x + i + 16,x2_norm);
		_mm256_store_ps(y + i + 16,y2_norm);
		_mm256_store_ps(x + i + 24,x2_norm);
		_mm256_store_ps(y + i + 24,y2_norm);
	}
}

void normalize_2(float *x, float *y, uint32_t max)
{
	for (uint32_t i = 0; i < max; i++){
		float len = sqrt(x[i] * x[i] + y[i] * y[i]);
		x[i] = x[i] / len;
		y[i] = x[i] / len;
	}
}

void bmlx_put_img_to_img(t_image *dest, t_image *src, uint32_t idx, uint32_t size, uint32_t width)
{
	for(uint32_t y = 0; y < size; y++)
	{
		for(uint32_t x = 0; x < size; x++)
		{
			dest->add[idx + y * width + x] = src->add[y * size + x];
		}
	}
}

void drawRect(t_image *image, int posX, int posY, int sizeX, int sizeY, int col)
{
	for (uint32_t y = 0; y < sizeY; y++)
	{
		for (uint32_t x = 0; x < sizeX; x++)
		{
			image->add[(posY + y) * image->width + (posX + x)] = col;
		}
	}
}

void drawRectWithPer(t_image *image, int posX, int posY, int sizeX, int sizeY, uint32_t width, int bodyCol, int perCol)
{
	for (uint32_t y = 0; y < sizeY; y++)
	{
		for (uint32_t x = 0; x < sizeX; x++)
		{
			image->add[(posY + y) * image->width + (posX + x)] = bodyCol;
		}
	}
	for (uint32_t j = 0; j < width; j++)
	{
		for (uint32_t x = 0; x < sizeX; x++)
		{
			image->add[(posY + j) * image->width + (posX + x)] = perCol;
			image->add[(posY + sizeY - j - 1) * image->width + (posX + x)] = perCol;
		}
	}
	for (uint32_t y = width; y < sizeY - width; y++)
	{
		for (uint32_t j = 0; j < width; j++)
		{
			image->add[(posY + y) * image->width + (posX + j)] = perCol;
			image->add[(posY + y) * image->width + (posX + sizeX - j)] = perCol;
		}
	}
}