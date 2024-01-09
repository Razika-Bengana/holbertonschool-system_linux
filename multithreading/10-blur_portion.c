#include "multithreading.h"

void blur_portion(blur_portion_t const *portion)
{
	size_t end_x = portion->x + portion->w, end_y = portion->y + portion->h;
	size_t x, y;
	float weight;
	size_t kx, ky;
	int pixel_x, pixel_y, adjusted_kx, adjusted_ky;
	pixel_t current_pixel, *blurred_pixel;

	for (y = portion->y; y < end_y; y++)
	{
		for (x = portion->x; x < end_x; x++)
		{
			float totalR = 0, totalG = 0, totalB = 0, totalWeight = 0;

			for (ky = 0; ky < portion->kernel->size; ky++)
			{
				adjusted_ky = ky - portion->kernel->size / 2;
				for (kx = 0; kx < portion->kernel->size; kx++)
				{
					adjusted_kx = kx - portion->kernel->size / 2;
					pixel_x = x + adjusted_kx;
					pixel_y = y + adjusted_ky;

					if (pixel_x >= 0 && pixel_x < (int)portion->img->w &&
					pixel_y >= 0 && pixel_y < (int)portion->img->h)
					{
						current_pixel = portion->img->pixels
							[pixel_y * portion->img->w + pixel_x];
						weight = portion->kernel->matrix[ky][kx];
						totalR += current_pixel.r * weight;
						totalG += current_pixel.g * weight;
						totalB += current_pixel.b * weight;
						totalWeight += weight;
					}
				}
			}
			blurred_pixel = &portion->img_blur->pixels[y * portion->img_blur->w + x];
			blurred_pixel->r = totalR / totalWeight;
			blurred_pixel->g = totalG / totalWeight;
			blurred_pixel->b = totalB / totalWeight;
		}
	}
}
