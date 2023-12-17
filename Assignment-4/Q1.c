#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t forks[5];
pthread_mutex_t bowls;
pthread_mutex_t eat_lock;
int meals_eaten[5] = {0};

#define MAX_MEALS 3
// Initial logic to solve the dining philosophers problem is taken from OSTEP Chapter 31
void eating(int pn) {
    if (meals_eaten[pn] >= MAX_MEALS) {
        printf("Philosopher %d has reached the maximum meals.\n", pn);
        return;
    }

    meals_eaten[pn]++;
    printf("Philosopher %d is eating (%d meals eaten)\n", pn, meals_eaten[pn]);
    sleep(2);
}

void thinking(int pn) {
    printf("Philosopher %d is thinking\n", pn);
    sleep(1);
}

int left(int pn) {
    return pn;
}

int right(int pn) {
    return (pn + 1) % 5;
}

void* philosopher(void* arg) {
    int philosopher_number = *((int*)arg);
    int left_fork = left(philosopher_number);
    int right_fork = right(philosopher_number);

    thinking(philosopher_number);

    pthread_mutex_lock(&forks[left_fork]);
    pthread_mutex_lock(&forks[right_fork]);

    if (philosopher_number < 3) {
        eating(philosopher_number);
    }

    pthread_mutex_unlock(&forks[right_fork]);
    pthread_mutex_unlock(&forks[left_fork]);

    while (1) {
        thinking(philosopher_number);

        pthread_mutex_lock(&forks[left_fork]);
        pthread_mutex_lock(&forks[right_fork]);

        eating(philosopher_number);

        pthread_mutex_unlock(&forks[right_fork]);
        pthread_mutex_unlock(&forks[left_fork]);

        sleep(1);

        if (meals_eaten[philosopher_number] >= MAX_MEALS) {
            printf("Philosopher %d has reached the maximum number of meals.\n", philosopher_number);
            break; // Break the loop if the maximum meals are reached
        }
    }

    return NULL;
}

int main() {
    pthread_t philosophers[5];
    int philosopher_numbers[5];

    pthread_mutex_init(&bowls, NULL);
    for (int i = 0; i < 5; ++i) {
        pthread_mutex_init(&forks[i], NULL);
    }

    for (int i = 0; i < 5; ++i) {
        philosopher_numbers[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_numbers[i]);
    }

    for (int i = 0; i < 5; ++i) {
        pthread_join(philosophers[i], NULL);
    }

    return 0;
}
