#include <pthread.h>
#include <stdio.h>
#include "caltrain.h"

void station_init(struct station *station) {
	// initialize seat mutex
	pthread_mutex_init(&station->seat_mutex, NULL);
	pthread_mutex_init(&station->passen_mutex, NULL);

	// initialize the two conditions
	pthread_cond_init(&station->cond_wait_passen, NULL);
	pthread_cond_init(&station->cond_avail_seats, NULL);

	station->curr_avail_seats = 0;
	station->curr_wait_passen = 0;
}

void station_load_train(struct station *station, int count) {
	station->curr_avail_seats = count;
	printf("seats from load  %d ,  curr_avail_seats  %d\n", count,
			station->curr_avail_seats);

	// if there are any free seats , notify all passengers
	if (station->curr_avail_seats > 0)
		pthread_cond_broadcast(&station->cond_avail_seats);

	while (station->curr_wait_passen > 0 && station->curr_avail_seats > 0) {
		pthread_mutex_lock(&station->passen_mutex);

//		printf("Here\n");
		pthread_cond_wait(&station->cond_wait_passen, &station->passen_mutex);

//		printf("HHHEEERRRREEE\n");

//		if (station->curr_avail_seats == 0)
//			return;

		pthread_mutex_unlock(&station->passen_mutex);
	}

	// the train is waiting till no passengers are waiting
	// nor chairs are available
//	while (station->curr_avail_seats > 0 && station->curr_wait_passen > 0) {
//
//	}

	return;
}

void station_wait_for_train(struct station *station) {
	// lock the seat mutex
	pthread_mutex_lock(&station->seat_mutex);

	// increment current waiting passengers
	station->curr_wait_passen++;

	// wait for available seat and release the mutex
	while (station->curr_avail_seats == 0)
		pthread_cond_wait(&station->cond_avail_seats, &station->seat_mutex);

	// when the waiting is done , decrement the seats
//	station->curr_avail_seats--;

	// release the mutex
	pthread_mutex_unlock(&station->seat_mutex);
}

void station_on_board(struct station *station) {
	pthread_mutex_lock(&station->seat_mutex);

	station->curr_avail_seats--;
	station->curr_wait_passen--;

	pthread_mutex_unlock(&station->seat_mutex);

//	printf("Free seats %d , Waiting passengers %d\n", station->curr_avail_seats,
//			station->curr_wait_passen);

//	if (station->curr_wait_passen == 0 || station->curr_avail_seats == 0)
	pthread_cond_signal(&station->cond_wait_passen);
}
