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

int main(int argc, char *argv []) {
    harness_init ();
    harness_run (new fp_instructions (), "PAPI_FP_OPS");
    harness_done ();

    return (0);
}
