#include "multithreading.h"

task_t *create_task(task_entry_t entry, void *param)
{
	task_t *task = (task_t *)malloc(sizeof(task_t));

	if (task == NULL)
	{
		return (NULL);
	}
	task->entry = entry;
	task->param = param;
	task->status = PENDING;
	task->result = NULL;

	pthread_mutex_init(&task->lock, NULL);

	return (task);
}

void destroy_task(task_t *task)
{
	if (task != NULL)
	{
		pthread_mutex_destroy(&task->lock);
		free(task);
	}
}

void enqueue_task(task_t *task)
{
	task_node_t *node = (task_node_t *)malloc(sizeof(task_node_t));
	node->task = task;
	node->next = NULL;

	pthread_mutex_lock(&queue.lock);

	if (queue.head == NULL)
	{
		queue.head = node;
		queue.tail = node;
	}
	else
	{
		queue.tail->next = node;
		queue.tail = node;
	}
	pthread_mutex_unlock(&queue.lock);
}

task_t *dequeue_task()
{
	pthread_mutex_lock(&queue.lock);
	task_node_t *node = queue.head;
	if (node != NULL)
	{
		queue.head = node->next;
		if (queue.head == NULL)
		{
			queue.tail = NULL;
		}
	}
	pthread_mutex_unlock(&queue.lock);

	task_t *task = NULL;
	if (node != NULL)
	{
		task = node->task;
		free(node);
	}
	return (task);
}

void *exec_tasks(void *arg)
{
	(void)arg;
	while (1)
	{
		task_t *task = dequeue_task();
		if (task == NULL)
		{
			break;
		}
		pthread_mutex_lock(&task->lock);
		task->status = STARTED;
		pthread_mutex_unlock(&task->lock);

		task->result = task->entry(task->param);

		pthread_mutex_lock(&task->lock);
		task->status = SUCCESS;
		pthread_mutex_unlock(&task->lock);

		destroy_task(task);
	}
	return (NULL);
}