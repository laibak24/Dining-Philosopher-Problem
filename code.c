#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define N 5 // Number of philosophers

sem_t chopstick[N];

void *eat_think_repeat(void *arg) {
    int philosopher_id = *((int *)arg);
    int left_chopstick = philosopher_id;
    int right_chopstick = (philosopher_id + 1) % N;

    while (1) {
        // Thinking
        printf("Philosopher %d is thinking\n", philosopher_id);
        sleep(rand() % 3 + 1);

        // Pick up chopsticks
        sem_wait(&chopstick[left_chopstick]);
        printf("Philosopher %d picked up left chopstick %d\n", philosopher_id, left_chopstick);
        sem_wait(&chopstick[right_chopstick]);
        printf("Philosopher %d picked up right chopstick %d\n", philosopher_id, right_chopstick);

        // Eating
        printf("Philosopher %d is eating\n", philosopher_id);
        sleep(rand() % 3 + 1);

        // Put down chopsticks
        sem_post(&chopstick[left_chopstick]);
        printf("Philosopher %d put down left chopstick %d\n", philosopher_id, left_chopstick);
        sem_post(&chopstick[right_chopstick]);
        printf("Philosopher %d put down right chopstick %d\n", philosopher_id, right_chopstick);
    }

    return NULL;
}

int main() {
    syscall(335);
    int i;
    int philosopher_id[N];

    // Initialize semaphores for chopsticks
    for (i = 0; i < N; i++) {
        sem_init(&chopstick[i], 0, 1);
    }

    // Create philosopher threads
    pthread_t philosopher[N];
    for (i = 0; i < N; i++) {
        philosopher_id[i] = i;
        pthread_create(&philosopher[i], NULL, eat_think_repeat, &philosopher_id[i]);
    }

    // Join philosopher threads
    for (i = 0; i < N; i++) {
        pthread_join(philosopher[i], NULL);
    }

    // Destroy semaphores
    for (i = 0; i < N; i++) {
        sem_destroy(&chopstick[i]);
    }

    return 0;
}



