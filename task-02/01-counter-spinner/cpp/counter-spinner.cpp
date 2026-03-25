#include <stdlib.h>

#include "harness.h"

volatile double var1 = 42;
volatile double var2 = 1;
volatile double var3 = 2;
volatile double var4 = 3;
volatile double var5 = 4;
volatile double var6 = 5;

class fp_instructions : public workload {
public:
    const char *name () override { return "floating_point_instructions"; }
    int execute () override { return (int) (var1 + var2 + var3 + var4 + var5 + var6); }
};

class branch_instructions_mispredicted : public workload {
private:
    // binary representation of some number 
    unsigned int pseudoRandomState = 0b0110'1100'1001'1110'1001'1100'1011'0101u;
    volatile int acc = 0;

public:
    const char *name () override { return "branch_instructions_mispredicted"; }
    int execute () override {
        pseudoRandomState ^= pseudoRandomState << 9;
        pseudoRandomState ^= pseudoRandomState >> 13;
        pseudoRandomState ^= pseudoRandomState << 3;

        if (pseudoRandomState & 1u) acc ^= 0b10101010;
        else            acc ^= 0b01010101;

        if (pseudoRandomState & 2u) acc ^= 0b01010101;
        else                    acc ^= 0b10101010;

        return acc;
    }
};

class l2_data_cache_misses : public workload {
private:
    static const int N = 1024 * 1024 * 2 / sizeof(int);
    volatile int *data;
    unsigned idx = 0;

public:
    l2_data_cache_misses() {
        data = new volatile int[N];
        for (int i = 0; i < N; ++i) {
            data[i] = i;
        }
    }

    ~l2_data_cache_misses() override {
        delete[] data;
    }

    const char *name () override { return "l2_data_cache_misses"; }

    int execute () override {
        constexpr int STRIDE = 1024 / sizeof(int);

        idx = (idx + STRIDE) % N;
        int v = data[idx] + 1;
        data[idx] = v;

        return v;
    }
};

int main(int argc, char *argv []) {
    harness_init ();
    // harness_run (new fp_instructions (), "PAPI_FP_OPS");
    harness_run (new branch_instructions_mispredicted (), "PAPI_BR_MSP");
    harness_run (new l2_data_cache_misses (), "PAPI_L2_DCM");
    harness_done ();

    return (0);
}
