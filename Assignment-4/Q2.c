#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


sem_t boardable_mutex, end_journey_mutex, boardedcountmutex, fullyunboarded, fullyboarded; // can passenger board yet , has journey finished , can adjust boardedcount variable without interthread conflict , have all passengers unboarded , has car capacity been fully boarded
int passengers_boarded = 0;
int car_capacity;
int total_passengers;

void load() {
    printf("Car is loading passengers.\n");
    sleep(1);
}

void unload() {
    printf("Car is unloading passengers.\n");
    sleep(1);
}

void board(int passenger_id) {
    printf("Passenger %d is boarding.\n", passenger_id);
    sem_wait(&boardedcountmutex);
    passengers_boarded++;
    if(passengers_boarded == car_capacity){
        sem_post(&fullyboarded);
    }
    sem_post(&boardedcountmutex);
    sleep(1);
}

void offboard(int passenger_id) {
    printf("Passenger %d is getting off.\n", passenger_id);
    sem_wait(&boardedcountmutex);
    passengers_boarded--;
    if(passengers_boarded == 0){
        sem_post(&fullyunboarded);
    }
    sem_post(&boardedcountmutex);
    sleep(1);

}
void* car(void* args) {
    while (1) {
        load();
        for (int i = 0; i < car_capacity; ++i) {
            sem_post(&boardable_mutex);
        }
	sem_wait(&fullyboarded);
        printf("Car is on the ride.\n");
        sleep(2);
	unload();
        for (int i = 0; i < car_capacity; ++i) {
            sem_post(&end_journey_mutex);
        }
	sem_wait(&fullyunboarded);
    }
}

void* passenger(void* args) {
    int passenger_id = *((int*)args);
    while (1) {
	sem_wait(&boardable_mutex);
	board(passenger_id);
    sem_wait(&end_journey_mutex);
    offboard(passenger_id);
    }
}


int main() {
    sem_init(&boardable_mutex, 0, 0);
    sem_init(&end_journey_mutex, 0, 0);
    sem_init(&boardedcountmutex,0,1);
    printf("Enter car capacity: ");
    scanf("%d", &car_capacity);

    printf("Enter total number of passengers: ");
    scanf("%d", &total_passengers);
    if(total_passengers < car_capacity){
        printf("Total passengers cannot be less than car capacity.\n");
        return 0;
    }
    pthread_t car_thread, passenger_threads[total_passengers];
    int passenger_ids[total_passengers];
    pthread_create(&car_thread, NULL, car, NULL);
    for (int i = 0; i < total_passengers; ++i) {
        passenger_ids[i] = i + 1;
        pthread_create(&passenger_threads[i], NULL, passenger, &passenger_ids[i]);
    }
    for (int i = 0; i < total_passengers; ++i) {
        pthread_join(passenger_threads[i], NULL);
    }
    pthread_join(car_thread, NULL);
    sem_destroy(&boardable_mutex);
    sem_destroy(&end_journey_mutex);

    return 0;
}