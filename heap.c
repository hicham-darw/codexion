#include "codexion.h"


void    insert_coder_to_heap(t_heap **heap, t_coder *coder)
{
    if (!heap || !coder)
        return;
    pthread_mutex_lock(&(*heap)->mutex_heap);

    (*heap)->coders[(*heap)->size] = coder;
    (*heap)->new_elem = (*heap)->size;
    (*heap)->size += 1;
    pthread_mutex_unlock(&(*heap)->mutex_heap);
}