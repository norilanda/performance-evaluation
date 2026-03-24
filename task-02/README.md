# Task 2: Hardware Performance Counters

The purpose of this lab assignment is to practice measurement with hardware performance counters
and to illustrate connection between observed counter values and workload properties.

- [The counter spinner task](01-counter-spinner) is about designing workload to evaluate hardware performance counters.

- [The false sharing task](02-false-sharing) is about arguing about workload properties from observed counter values.

## Overview

The examples require the PAPI library from [https://icl.utk.edu/papi](https://icl.utk.edu/papi).
The library is available as a software package in commom Linux distributions,
and is alread installed on the lab computers.

See [the course examples repository](https://github.com/d-iii-s/teaching-performance-evaluation/tree/master/src/measurement-hardware-counters-papi) for PAPI coding examples.

### `papi_avail`

Use the `papi_avail` command to list the standard events available to PAPI.

```
Available PAPI preset and user defined events plus hardware information.
...
================================================================================
  PAPI Preset Events
================================================================================
    Name        Code    Avail Deriv Description (Note)
PAPI_L1_DCM  0x80000000  Yes   No   Level 1 data cache misses
PAPI_L1_ICM  0x80000001  Yes   No   Level 1 instruction cache misses
...
```

### `papi_native_avail`

Use the `papi_native_avail` command to list the native events available to PAPI.

```
Available native events and hardware information.
...
===============================================================================
 Native Events in Component: perf_event
===============================================================================
| ix86arch::UNHALTED_CORE_CYCLES                                               |
|            count core clock cycles whenever the clock signal on the specific |
|            core is running (not halted)                                      |
|     :e=0                                                                     |
|            edge level (may require counter-mask >= 1)                        |
|     :i=0                                                                     |
|            invert                                                            |
|     :c=0                                                                     |
|            counter-mask in range [0-255]                                     |
|     :t=0                                                                     |
|            measure any thread                                                |
|     :period=0                                                                |
|            sampling period                                                   |
|     :freq=0                                                                  |
|            sampling frequency (Hz)                                           |
|     :excl=0                                                                  |
|            exclusive access                                                  |
|     :cpu=0                                                                   |
|            CPU to program                                                    |
|     :pinned=0                                                                |
|            pin event to counters                                             |
--------------------------------------------------------------------------------
...
```
