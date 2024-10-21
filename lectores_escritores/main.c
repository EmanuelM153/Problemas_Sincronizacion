#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_LECTORES 5
#define NUM_ESCRITORES 2

sem_t mutex;
sem_t db;
int num_lectores = 0;

void* lector(void *arg) {
    int id = *(int *) arg;

    while (1) {
        sem_wait(&mutex);

        num_lectores++;
        if (num_lectores == 1)
            sem_wait(&db);

        sem_post(&mutex);

        printf("Lector %d está leyendo.\n", id);
        sleep(1);

        sem_wait(&mutex);

        num_lectores--;
        if (num_lectores == 0)
            sem_post(&db);

        sem_post(&mutex);

        sleep(1);
    }
    return NULL;
}

void* escritor(void *arg) {
    int id = *(int *) arg;

    while (1) {
        sem_wait(&db);

        printf("Escritor %d está escribiendo.\n", id);
        sleep(1);

        sem_post(&db);

        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t lectores[NUM_LECTORES], escritores[NUM_ESCRITORES];
    int ids_lectores[NUM_LECTORES], ids_escritores[NUM_ESCRITORES];

    sem_init(&mutex, 0, 1);
    sem_init(&db, 0, 1);

    for (int i = 0; i < NUM_LECTORES; i++) {
        ids_lectores[i] = i + 1;
        pthread_create(&lectores[i], NULL, lector, &ids_lectores[i]);
    }

    for (int i = 0; i < NUM_ESCRITORES; i++) {
        ids_escritores[i] = i + 1;
        pthread_create(&escritores[i], NULL, escritor, &ids_escritores[i]);
    }

    for (int i = 0; i < NUM_LECTORES; i++)
        pthread_join(lectores[i], NULL);
    for (int i = 0; i < NUM_ESCRITORES; i++)
        pthread_join(escritores[i], NULL);

    sem_destroy(&mutex);
    sem_destroy(&db);

    return 0;
}
