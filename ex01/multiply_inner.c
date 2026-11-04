#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

int N;
int P;
int **a;
int **b;
int **c;

pthread_barrier_t bar_matmull_init;
pthread_mutex_t mutex_matmull = PTHREAD_MUTEX_INITIALIZER;

// TODO: paralelizati operatia din comentariul din functie
// in interiorul functiei respective
void *thread_function(void *arg) {
    ll thread_id = *(int *)arg;

    /*
    for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                    for (k = 0; k < N; k++) {
                            c[i][j] += a[i][k] * b[k][j];
                    }
            }
    }
    */

    ll start = (ll)thread_id * N / P;
    ll end = (ll)(thread_id + 1) * N / P;
    

    // Ca sa nu cumva sa spuna stim noi cine ca am serializat algoritmul
    for (ll i = start; i < end; i += 1) {
        for (ll j = 0; j < N; j += 1) {
            c[i][j] = 0;
        }
    }

    pthread_barrier_wait(&bar_matmull_init);

    for (ll i = 0; i < N; i++) {
        for (ll j = 0; j < N; j++) {
            ll aux = 0;
            for (ll k = start; k < end; k++) {
                aux += a[i][k] * b[k][j];
            }
            pthread_mutex_lock(&mutex_matmull);
            c[i][j] += aux;
            pthread_mutex_unlock(&mutex_matmull);
        }
    }

    pthread_exit(NULL);
}

void get_args(int argc, char **argv) {
    if (argc < 3) {
        printf("Numar insuficient de parametri: ./program N P\n");
        exit(1);
    }

    N = atoi(argv[1]);
    P = atoi(argv[2]);
    // ca sa functioneze pentru orice valori
    if (N < 1 || N > 1500) {
        printf("Valoare nevalida pentru N");
        exit(1);
    }
    if (P < 1) {
        P = 1;
    } else if (P > N) {
        P = N;
    }
}

void init() {
    a = malloc(sizeof(int *) * N);
    b = malloc(sizeof(int *) * N);
    c = malloc(sizeof(int *) * N);

    if (a == NULL || b == NULL || c == NULL) {
        printf("Eroare la malloc!");
        exit(1);
    }

    int i, j;
    for (i = 0; i < N; i++) {
        a[i] = malloc(sizeof(int) * N);
        b[i] = malloc(sizeof(int) * N);
        c[i] = malloc(sizeof(int) * N);

        if (a[i] == NULL || b[i] == NULL || c[i] == NULL) {
            printf("Eroare la malloc!");
            exit(1);
        }
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            c[i][j] = 0;

            a[i][j] = 7 + (i + j) % 100;
            b[i][j] = 50 + (i + j) % 100;
        }
    }
}

void print(int **mat) {
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%i\t", mat[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int i;

    get_args(argc, argv);
    init();
    
    // fprintf(stderr, "ok");


    pthread_t tid[P];
    int thread_id[P];

    pthread_barrier_init(&bar_matmull_init, NULL, P);
    // pthread_mutex_init(&mutex, NULL);
    // pthread_mutex_unlock(&mutex_matmull);

    for (i = 0; i < P; i++) {
        thread_id[i] = i;
        pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
    }

    for (i = 0; i < P; i++) {
        pthread_join(tid[i], NULL);
    }

    print(c);

    return 0;
}
