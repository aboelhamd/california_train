#include <pthread.h>

struct station {
	pthread_mutex_t seat_mutex, passen_mutex;
	pthread_cond_t cond_wait_passen, cond_avail_seats;
	int curr_avail_seats, curr_wait_passen;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
