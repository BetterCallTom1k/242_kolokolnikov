#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 4

typedef struct {
    int *arr;
    int  start;
    int  end;
} ThreadArg;

static long long sum = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void *worker(void *arg)
{
    ThreadArg *a = (ThreadArg *)arg;
    long long partial = 0;
    for (int i = a->start; i < a->end; i++)
        partial += a->arr[i];
    pthread_mutex_lock(&mutex);
    sum += partial;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(void)
{
    int n;
    printf("Enter N: ");
    scanf("%d", &n);

    int *arr = malloc(n * sizeof(int));
    srand((unsigned)time(NULL));
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 100;

    printf("Array: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    pthread_t threads[NUM_THREADS];
    ThreadArg args[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        args[i].arr   = arr;
        args[i].start = i * (n / NUM_THREADS);
        args[i].end   = (i == NUM_THREADS - 1) ? n : (i + 1) * (n / NUM_THREADS);
        pthread_create(&threads[i], NULL, worker, &args[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("Sum: %lld\n", sum);

    pthread_mutex_destroy(&mutex);
    free(arr);
    return 0;
}
