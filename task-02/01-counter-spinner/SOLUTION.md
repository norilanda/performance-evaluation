## Platform
- CPU model: AMD Ryzen 5 PRO 4650G with Radeon Graphics
- OS: Gentoo Linux, kernel 6.12.77
- PAPI: 7.1.0.0

## Workloads & Counters

### 1. EXAMPLE: Floating-Point Operations (PAPI_FP_OPS)
Description: Simple double arithmetic; compiler should recognize FP ops easily.
Code: Six volatile double additions per execute().
Score: 5.06 instructions/event

### 1. Branch Mispredict (PAPI_BR_MSP)
Description: Conditional branch instructions mispredicted

Code: the implemented code is inside the `branch_instructions_mispredicted` class. Firstly it changes the `pseudoRandomState` so it is not easy to predict it with lightweight operations (integer shifts and XORs). Then the first least significant bit is used in the first if and the second in the second one. Each branch changes the `acc` variable and the final value is returned.


### 2. L2 Data Cache Misses (PAPI_L2_DCM)
Description: Level 2 data cache misses

Code: the implemented code is inside the `l2_data_cache_misses` class.
...

### 3. Address Cache Misses (PAPI_TLB_DM)
Description: Data translation lookaside buffer misses
...

## Known Issues
...