#include <pthread.h>
#include <stdio.h>
#include "caltrain.h"

void station_init(struct station *station) {
	// initialize seat mutex
	pthread_mutex_init(&station->mutex, NULL);

	// initialize the two conditions
	pthread_cond_init(&station->cond_wait_passen, NULL);
	pthread_cond_init(&station->cond_avail_seats, NULL);

	station->avail_seats = 0;
	station->wait_passen = 0;
	station->on_passen = 0;
}

void station_load_train(struct station *station, int count) {
	station->avail_seats = count;

	printf("seats from load  %d ,  avail_seats  %d\n", count,
			station->avail_seats);

	// if there are any free seats , notify all passengers
	if (station->avail_seats > 0)
		pthread_cond_broadcast(&station->cond_avail_seats);

	pthread_mutex_lock(&station->mutex);
	while (station->wait_passen > 0 && station->avail_seats > 0) {

//		printf("Here\n");
		pthread_cond_wait(&station->cond_wait_passen, &station->mutex);

//		printf("HHHEEERRRREEE\n");
	}
	pthread_mutex_unlock(&station->mutex);
}

void station_wait_for_train(struct station *station) {
	// lock the seat mutex
	pthread_mutex_lock(&station->mutex);

	// increment current waiting passengers
	station->wait_passen++;

	// wait for available seat and release the mutex
	while (station->avail_seats == station->on_passen)
		pthread_cond_wait(&station->cond_avail_seats, &station->mutex);

	station->on_passen++;

	printf("After wait_passen %d\n", station->wait_passen);

	// release the mutex
	pthread_mutex_unlock(&station->mutex);
}

void station_on_board(struct station *station) {
	pthread_mutex_lock(&station->mutex);

	station->avail_seats--;
	station->wait_passen--;
	station->on_passen--;

	pthread_mutex_unlock(&station->mutex);

//	printf("Free seats %d , Waiting passengers %d\n", station->avail_seats,
//			station->wait_passen);

//	if (station->wait_passen == 0 || station->avail_seats == 0)
	pthread_cond_signal(&station->cond_wait_passen);
}
