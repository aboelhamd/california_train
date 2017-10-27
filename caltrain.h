#include <pthread.h>

struct station {
	pthread_mutex_t mutex;
	pthread_cond_t cond_wait_passen, cond_avail_seats;
	int avail_seats, wait_passen, on_passen;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
