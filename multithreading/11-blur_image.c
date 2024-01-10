#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include "multithreading.h"


void *blur_portion_wrapper(void *arg)
{
	blur_portion_t *portion = (blur_portion_t *)arg;
	blur_portion(portion);
	return (NULL);
}

/* Blur the entire image using multiple threads */
void blur_image(img_t *img_blur, img_t const *img, kernel_t const *kernel)
{
	const int num_threads = 16;
	pthread_t threads[num_threads];
	blur_portion_t portions[num_threads];
	int i;

	/* Divide the image into portions */
	for (i = 0; i < num_threads; i++)
	{
		portions[i].img = img;
		portions[i].img_blur = img_blur;
		portions[i].kernel = kernel;
		portions[i].x = (i % 4) * (img->w / 4);
		portions[i].y = (i / 4) * (img->h / 4);
		portions[i].w = img->w / 4;
		portions[i].h = img->h / 4;

		pthread_create(&threads[i], NULL, blur_portion_wrapper, &portions[i]);
	}

	/* Wait for all threads to finish */
	for (i = 0; i < num_threads; i++)
	{
		pthread_join(threads[i], NULL);
	}
}


