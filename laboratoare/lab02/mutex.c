#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 2

int a = 0;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void *f(void *arg) {
    (void)arg;

    pthread_mutex_lock(&m);
    a += 2;
    pthread_mutex_unlock(&m);

    pthread_exit(NULL);
}

int main(void) {
    int i, r;
    void *status;
    pthread_t threads[NUM_THREADS];
    int arguments[NUM_THREADS];

    for (i = 0; i < NUM_THREADS; i++) {
        arguments[i] = i;
        r = pthread_create(&threads[i], NULL, f, &arguments[i]);

        if (r) {
            printf("Eroare la crearea thread-ului %d\n", i);
            exit(-1);
        }
    }

    for (i = 0; i < NUM_THREADS; i++) {
        r = pthread_join(threads[i], &status);

        if (r) {
            printf("Eroare la asteptarea thread-ului %d\n", i);
            exit(-1);
        }
    }

    printf("a = %d\n", a);

    return 0;
}
