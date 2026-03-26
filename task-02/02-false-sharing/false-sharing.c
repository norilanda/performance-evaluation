#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>
#include <inttypes.h>
#include <papi.h>

#define LOOPS 1000000
#define EVENT_COUNT 4

static const int EVENT_CODES[EVENT_COUNT] = {
    PAPI_TOT_CYC,
    PAPI_TOT_INS,
    PAPI_L2_DCM,
    PAPI_L1_DCA
};

static const char *EVENT_NAMES[EVENT_COUNT] = {
    "TOT_CYC",
    "TOT_INS",
    "L2_DCM",
    "L1_DCA"
};


typedef struct {
    int index;
    int cpu;
    uint64_t timestamp;
    long long counters[EVENT_COUNT];
} run_info_t;

static void run_info_print(run_info_t *info) {
    printf("CPU%2d [ %10llu ]", info->cpu, (unsigned long long) info->timestamp);

    for (int i = 0; i < EVENT_COUNT; i++) {
        if (info->counters[i] >= 0) {
            printf(" %s=%lld", EVENT_NAMES[i], info->counters[i]);
        } else {
            printf(" %s=n/a", EVENT_NAMES[i]);
        }
    }

    if (info->counters[0] >= 0) {
        double cycles_per_iter = (double) info->counters[0] / (double) LOOPS;
        printf(" cyc/iter=%.3f", cycles_per_iter);
    } else {
        printf(" cyc/iter=n/a");
    }

    if (info->counters[0] > 0 && info->counters[1] > 0) {
        double cpi = (double) info->counters[0] / (double) info->counters[1];
        printf(" CPI=%.3f", cpi);
    } else {
        printf(" CPI=n/a");
    }

    if (info->counters[3] >= 0) {
        double evt3_per_iter = (double) info->counters[3] / (double) LOOPS;
        printf(" %s/iter=%.6f", EVENT_NAMES[3], evt3_per_iter);
    } else {
        printf(" %s/iter=n/a", EVENT_NAMES[3]);
    }
}


static volatile int *FALSE_SHARING_VARIABLE;

static pthread_barrier_t barrier;

static void papi_fail(int ret, const char *msg) {
    fprintf(stderr, "%s: %s\n", msg, PAPI_strerror(ret));
    exit(1);
}


static inline uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi) : : );
    return ((uint64_t) hi << 32 | (uint64_t) lo);
}

static void *worker(void *arg) {
    run_info_t *info = arg;
    int event_set = PAPI_NULL;
    int enabled_events[EVENT_COUNT];
    int enabled_count = 0;
    long long measured[EVENT_COUNT];

    for (int i = 0; i < EVENT_COUNT; i++) {
        info->counters[i] = -1;
    }

    int ret = PAPI_register_thread();
    if (ret != PAPI_OK) {
        papi_fail(ret, "PAPI_register_thread failed");
    }

    ret = PAPI_create_eventset(&event_set);
    if (ret != PAPI_OK) {
        papi_fail(ret, "PAPI_create_eventset failed");
    }

    for (int i = 0; i < EVENT_COUNT; i++) {
        ret = PAPI_add_event(event_set, EVENT_CODES[i]);
        if (ret == PAPI_OK) {
            enabled_events[enabled_count++] = i;
            info->counters[i] = 0;
        }
    }

    pthread_barrier_wait(&barrier);

    if (enabled_count > 0) {
        ret = PAPI_start(event_set);
        if (ret != PAPI_OK) {
            papi_fail(ret, "PAPI_start failed");
        }
    }

    uint64_t begin = rdtsc();
    for (int i = 0; i < LOOPS; i++) {
        FALSE_SHARING_VARIABLE[info->index] += i;
    }
    uint64_t end = rdtsc();

    if (enabled_count > 0) {
        ret = PAPI_stop(event_set, measured);
        if (ret != PAPI_OK) {
            papi_fail(ret, "PAPI_stop failed");
        }
        for (int i = 0; i < enabled_count; i++) {
            info->counters[enabled_events[i]] = measured[i];
        }
    }

    info->timestamp = end - begin;

    ret = PAPI_cleanup_eventset(event_set);
    if (ret != PAPI_OK) {
        papi_fail(ret, "PAPI_cleanup_eventset failed");
    }

    ret = PAPI_destroy_eventset(&event_set);
    if (ret != PAPI_OK) {
        papi_fail(ret, "PAPI_destroy_eventset failed");
    }

    ret = PAPI_unregister_thread();
    if (ret != PAPI_OK) {
        papi_fail(ret, "PAPI_unregister_thread failed");
    }

    return NULL;
}


static void pthread_create_with_affinity(pthread_t *thread,
        void *(*worker) (void *),
        void *arg,
        int affinity) {

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    cpu_set_t cpus;
    CPU_ZERO(&cpus);
    CPU_SET(affinity, &cpus);

    pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpus);

    pthread_create(thread, &attr, worker, arg);
}

int main(int argc, char *argv[]) {
    int ret = PAPI_library_init(PAPI_VER_CURRENT);
    if (ret != PAPI_VER_CURRENT) {
        if (ret > 0) {
            fprintf(stderr, "PAPI version mismatch: got %d, expected %d\n", ret, PAPI_VER_CURRENT);
            return 1;
        }
        papi_fail(ret, "PAPI_library_init failed");
    }

    ret = PAPI_thread_init((unsigned long (*)(void)) pthread_self);
    if (ret != PAPI_OK) {
        papi_fail(ret, "PAPI_thread_init failed");
    }

    ret = PAPI_set_cmp_granularity(PAPI_GRN_THR, 0);
    if (ret != PAPI_OK) {
        papi_fail(ret, "PAPI_set_cmp_granularity(PAPI_GRN_THR) failed");
    }

    if (argc == 1) {
        fprintf(stderr, "Usage: %s distance CPU [CPU [CPU ... ]]\n", argv[0]);
        return 1;
    }

    int count = argc - 2;

    int distance = atoi(argv[1]);

    int *cpus = malloc(sizeof(int) * count);
    for (int i = 0; i < count; i++) {
        cpus[i] = atoi(argv[i + 2]);
    }

    run_info_t *info_one = malloc(sizeof(run_info_t) * count);
    run_info_t *info_two = malloc(sizeof(run_info_t) * count);

    FALSE_SHARING_VARIABLE = memalign(64, sizeof(int) * 256);

    for (int i = 0; i < count; i++) {
        pthread_t thread_one, thread_two;

        info_one[i].index = 0;
        info_one[i].cpu = 0;

        info_two[i].index = distance;
        info_two[i].cpu = cpus[i];


        pthread_barrier_init(&barrier, NULL, 2);

        pthread_create_with_affinity(&thread_one, worker, &info_one[i], 0);
        pthread_create_with_affinity(&thread_two, worker, &info_two[i], cpus[i]);

        pthread_join(thread_one, NULL);
        pthread_join(thread_two, NULL);

        pthread_barrier_destroy(&barrier);
    }

    for (int i = 0; i < count; i++) {
        run_info_print(&info_one[i]);
        printf("  :  ");
        run_info_print(&info_two[i]);
        puts("");
    }

    return 0;
}
