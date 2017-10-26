#include <pthread.h>
#include <stdio.h>
#include "caltrain.h"

void station_init(struct station *station) {
	// initialize seat mutex
	pthread_mutex_init(&station->mutex, NULL);

	// initialize the two conditions
	pthread_cond_init(&station->train_cond, NULL);
	pthread_cond_init(&station->passeng_cond, NULL);

	station->curr_avail_seats = 0;
	station->curr_wait_passen = 0;
	station->free_seats = 0;
}

void station_load_train(struct station *station, int count) {
	station->curr_avail_seats = count;
	station->free_seats = count;
	printf("free seats from load  %d\n", count);

	// if there are any free seats , notify all passengers
	if (station->curr_avail_seats > 0)
		pthread_cond_broadcast(&station->passeng_cond);

	while (station->curr_wait_passen > 0 && station->curr_avail_seats > 0) {
		pthread_mutex_lock(&station->mutex);

//		printf("Here\n");
		pthread_cond_wait(&station->train_cond, &station->mutex);

//		printf("HHHEEERRRREEE\n");

//		if (station->curr_avail_seats == 0)
//			return;

		pthread_mutex_unlock(&station->mutex);
	}

	// the train is waiting till no passengers are waiting
	// nor chairs are available
//	while (station->curr_avail_seats > 0 && station->curr_wait_passen > 0) {
//
//	}

	printf("Load is about to return , Free seats %d , Waiting passengers %d\n",
			station->curr_avail_seats, station->curr_wait_passen);

	station_init(station);

	return;
}

void station_wait_for_train(struct station *station) {
	// lock the seat mutex
	pthread_mutex_lock(&station->mutex);

	// increment current waiting passengers
	station->curr_wait_passen++;

	printf("free seats before waiting  %d  , passengers  %d\n",
			station->free_seats, station->curr_wait_passen);

	// wait for available seat and release the mutex
	while (station->free_seats < station->curr_wait_passen)
		pthread_cond_wait(&station->passeng_cond, &station->mutex);

	// when the waiting is done , decrement the seats
//	station->curr_avail_seats--;

	// release the mutex

	station->curr_wait_passen--;
	printf("free seats after waiting  %d  , passengers  %d\n",
			station->free_seats, station->curr_wait_passen);

	pthread_mutex_unlock(&station->mutex);

}

void station_on_board(struct station *station) {
	pthread_mutex_lock(&station->mutex);

	station->curr_avail_seats--;
//	station->curr_wait_passen--;

	pthread_cond_signal(&station->train_cond);
	pthread_mutex_unlock(&station->mutex);

//	printf("Free seats %d , Waiting passengers %d\n", station->curr_avail_seats,
//			station->curr_wait_passen);

//	if (station->curr_wait_passen == 0 || station->curr_avail_seats == 0)
}
