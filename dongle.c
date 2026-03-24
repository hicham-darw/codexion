#include "codexion.h"

t_dongle    *initial_dongles(int number_of_coders)
{
    t_dongle *dongle;
    int     i;

    dongle = malloc(sizeof(t_dongle) * (number_of_coders));
    if (!dongle)
        return (NULL);
    memset(dongle, 0, sizeof(t_dongle) * (number_of_coders));
    i = 0;
    while (i < number_of_coders)
    {
        dongle[i].id = i + 1;
        dongle[i].is_taken = 0;
        pthread_mutex_init(&dongle[i].mutex_dongle, NULL);
        i += 1;
    }
    return (dongle);
}


int	take_dongles(t_coder * coder)
{
	int		ret_val;

	if (coder->left_dongle == coder->right_dongle)
	{
		pthread_mutex_lock(&coder->left_dongle->mutex_dongle);
		pthread_mutex_unlock(&coder->left_dongle->mutex_dongle);
		return (0);
	}

	if (coder->id % 2)
	{
		pthread_mutex_lock(&coder->left_dongle->mutex_dongle);
		pthread_mutex_lock(&coder->right_dongle->mutex_dongle);
	}
	else
	{
		pthread_mutex_lock(&coder->right_dongle->mutex_dongle);
		pthread_mutex_lock(&coder->left_dongle->mutex_dongle);
	}
	if (!coder->left_dongle->is_taken && !coder->right_dongle->is_taken)
	{
		coder->left_dongle->is_taken = 1;
		coder->right_dongle->is_taken = 1;
		return (1);
	}
	ret_val = 0;
	pthread_mutex_unlock(&coder->left_dongle->mutex_dongle);
	pthread_mutex_unlock(&coder->right_dongle->mutex_dongle);
	return ret_val;
}

void release_dongles(t_coder *coder)
{
    if (coder->left_dongle == coder->right_dongle)
    {
        pthread_mutex_lock(&coder->left_dongle->mutex_dongle);
        coder->left_dongle->is_taken = 0;
        pthread_mutex_unlock(&coder->left_dongle->mutex_dongle);
        return ;
    }
    if (coder->id % 2)
    {
        pthread_mutex_lock(&coder->left_dongle->mutex_dongle);
        pthread_mutex_lock(&coder->right_dongle->mutex_dongle);
    }
    else
    {
        pthread_mutex_lock(&coder->right_dongle->mutex_dongle);
        pthread_mutex_lock(&coder->left_dongle->mutex_dongle);
    }
    coder->left_dongle->is_taken = 0;
    coder->right_dongle->is_taken = 0;
    pthread_mutex_unlock(&coder->right_dongle->mutex_dongle);
    pthread_mutex_unlock(&coder->left_dongle->mutex_dongle);
}
