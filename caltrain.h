#include <pthread.h>

struct station {
	pthread_mutex_t mutex;
	pthread_cond_t train_cond, passeng_cond;
	int curr_avail_seats, curr_wait_passen, free_seats, is_train_wait;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
