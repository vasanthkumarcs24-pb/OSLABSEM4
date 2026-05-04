#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

#define THINKING 0
#define HUNGRY   1
#define EATING   2

int state[N];
int phil[N] = {0, 1, 2, 3, 4};

sem_t mutex;
sem_t S[N];

void test(int i) {
    if (state[i] == HUNGRY &&
        state[(i + 4) % N] != EATING &&
        state[(i + 1) % N] != EATING) {

        state[i] = EATING;
        printf("Philosopher %d takes forks %d and %d\n", i, (i + 4) % N, i);
        printf("Philosopher %d is Eating\n", i);

        sem_post(&S[i]);
    }
}


void take_forks(int i) {
    sem_wait(&mutex);

    state[i] = HUNGRY;
    printf("Philosopher %d is Hungry\n", i);

    test(i);

    sem_post(&mutex);

    sem_wait(&S[i]);
    sleep(1);
}


void put_forks(int i) {
    sem_wait(&mutex);

    state[i] = THINKING;
    printf("Philosopher %d putting forks %d and %d down\n", i, (i + 4) % N, i);
    printf("Philosopher %d is Thinking\n", i);


    test((i + 4) % N);
    test((i + 1) % N);

    sem_post(&mutex);
}


void* philosopher(void* num) {
    int i = *(int*)num;
    int j = 0;
    while (j < 5) {
        sleep(1);              // Thinking
        take_forks(i);         // Try to pick forks
        sleep(2);              // Eating
        put_forks(i);// Put forks back
        j++;
    }
}

int main() {
    pthread_t thread_id[N];

    sem_init(&mutex, 0, 1);

    for (int i = 0; i < N; i++)
        sem_init(&S[i], 0, 0);

    for (int i = 0; i < N; i++)
        pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);

    for (int i = 0; i < N; i++)
        pthread_join(thread_id[i], NULL);

    return 0;
}
