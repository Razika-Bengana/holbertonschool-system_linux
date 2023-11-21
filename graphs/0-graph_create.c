#include "graphs.h"

/**
 * graph_create - creates a graph structure and initializes it
 *
 * The function dynamically allocates memory for a new graph structure,
 * initializes its member 'nb_vertices' to 0, indicating that the graph
 * currently contains no vertices, and sets the 'vertices' pointer to NULL,
 * meaning the graph has no adjacency list at this point.
 *
 * Return: A pointer to the newly allocated graph_t structure if the allocation
 * was successful, or NULL if the memory allocation failed.
 */

graph_t *graph_create(void)
{
        graph_t *graph = (graph_t *)malloc(sizeof(graph_t));

        if (graph == NULL)
        {
            return (NULL);
        }

        graph->nb_vertices = 0;
        graph->vertices = NULL;

        return (graph);
}
