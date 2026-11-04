#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef long long ll;
#define INF LLONG_MAX
#define NEGINF LLONG_MIN

#define P ((ll)4)

#define MOD ((ll)1337)
#define SEED ((int)1337)

ll *arr;
ll n;
int dbg = 0;

pthread_t threads[P];
ll thread_ids[P];

pthread_barrier_t bar;

void *thread_function(void *arg) {
    ll tid = *(ll *)arg;

    ll res;
    if (tid == 0) {
        res = INF;
        for (ll i = 0; i < n; i++) {
            if (arr[i] < res) {
                res = arr[i];
            }
        }
    } else if (tid == 1) {
        res = NEGINF;
        for (ll i = 0; i < n; i++) {
            if (arr[i] > res) {
                res = arr[i];
            }
        }
    } else if (tid == 2) {
        res = 0;
        for (ll i = 0; i < n; i++) {
            res += arr[i];
        }
    } else if (tid == 3) {
        res = 1;
        for (ll i = 0; i < n; i++) {
            res = (arr[i] % MOD) * res % MOD;
        }
    }

    // fprintf(stderr, "%lld\n", tid);
    // pthread_barrier_wait(&bar);

    char text[P][20] = {"minim", "maxim", "suma", "produs"};

    for (ll i = 0; i < P; i++) {
        if (tid == i) {
            fputs(text[i], stdout);
            printf(" = %lld\n", res);
        }
        pthread_barrier_wait(&bar);
    }

    return NULL;
    // pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Specificati dimensiunea array-ului\n");
        exit(-1);
    }

    n = atoi(argv[1]);
    if (argc > 2) {
        dbg = atoi(argv[2]);
    }

    pthread_barrier_init(&bar, NULL, P);

    srand(SEED);

    arr = malloc(n * sizeof(ll));
    for (ll i = 0; i < n; i++) {
        arr[i] = rand() % 100;
    }
    if (dbg) {
        fprintf(stderr, "dbg on\n");
        for (ll i = 0; i < n; i++) {
            fprintf(stderr, "%lld", arr[i]);
            if (i != n - 1) {
                fprintf(stderr, " ");
            } else {
                fprintf(stderr, "\n");
            }
        }
    }

    for (ll i = 0; i < P; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    for (ll i = 0; i < P; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // cleanup
    pthread_barrier_destroy(&bar);
    free(arr);

    return 0;
}
