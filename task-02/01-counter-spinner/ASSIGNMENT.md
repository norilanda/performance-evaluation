# Task: Spin Hardware Counters

## Overview

Write code that generates specific counter events as frequently as possible.

We have prepared a harness for your code that collects the counter values and computes a score.
The score is the number of instructions executed to fire one event, lower is better.
The best score in each event category will receive a grading bonus :-)

Since counter availability is platform dependent, you need to tailor the counter choice to your platform.
The following list refers to counters available in the student lab, pick at least 3 counters from
different rows, or their reasonable equivalens on different platforms:

- A counter for some special subset of instructions (vectorized, floating point ...) such as `INST_RETIRED:X87'
- A counter for some branch misprediction event, such as `PAPI_BR_MSP`
- A counter for some data cache miss event, such as `PAPI_L3_TCM`
- A counter for some address cache miss event, such as `PAPI_TLB_DM` or `PAPI_TLB_IM`
- A counter for some interesting operating system event, such as
  `perf::PERF_COUNT_SW_PAGE_FAULTS`
  or `perf::PERF_COUNT_SW_CPU_MIGRATIONS`
  or `perf::PERF_COUNT_SW_CONTEXT_SWITCHES`

## Example

Assume you want to generate the `PAPI_LD_INS` event, which counts the load instructions.
Start by inheriting from the `workload` class and overriding the `execute ()` method.
The new method will obviously need to load data, standard variable accesses should
do the trick.

### C++

```cpp
#include <stdlib.h>
#include "harness.h"

volatile int var1 = 1;
volatile int var2 = 2;
volatile int var3 = 3;
volatile int var4 = 4;
volatile int var5 = 5;
volatile int var6 = 6;

class load_instructions : public workload {
public:
    int execute() {
        return (var1 + var2 + var3 + var4 + var5 + var6);
    }
};

int main () {
    harness_init ();
    harness_run (new load_instructions (), "PAPI_LD_INS");
    harness_done();
    return (0);
}

```

### Java

```java
public class LoadInstructions extends Workload {
    public static volatile int VAR1 = 1;
    public static volatile int VAR2 = 2;
    public static volatile int VAR3 = 3;
    public static volatile int VAR4 = 4;
    public static volatile int VAR5 = 5;
    public static volatile int VAR6 = 6;

    @Override
    public int execute() {
        return (VAR1 + VAR2 + VAR3 + VAR4 + VAR5 + VAR6);
    }
}
```

## Internals

The actual measurement is implemented as follows:

```pseudo
for i in 1..10000:
    start-counters ()
    for j in 1..1000
        workload.execute ()
    done
    stop-and-store-counters ()
done
```

Please make sure your solution completes in reasonable time (at most seconds).

## Debugging

You may observe a situation where a counter reported as available by PAPI will not work.
Should this happen, please replace the PAPI counter in question with a native counter of
similar semantics (for example use `FP_ARITH:SCALAR_SINGLE` instead of `PAPI_SP_OPS`) and
mention the issue in `SOLUTION.md`.

## Plotting

```shell
Rscript counter-spinner-plot.r out.csv out.pdf
```


## What to Submit

Your `workload` classes, with a script for building the executable.

The data file with the measurement results you have collected, and the PDF report.

A summary `SOLUTION` document describing your platform and the rationale behind your workloads.
