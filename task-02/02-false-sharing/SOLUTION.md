## Platform
- CPU model: AMD Ryzen 5 PRO 4650G with Radeon Graphics
- OS: Gentoo Linux, kernel 6.12.77
- PAPI: 7.1.0.0

## False Sharing Results
Measurements were collected with one run for each distance value (`0`, `1`, `64`) on CPUs `0` and `1`:

- `./false-sharing 0 1` (true sharing)
- `./false-sharing 1 1` (false sharing)
- `./false-sharing 64 1` (no sharing)

The program prints results for both threads. The table below reports per-distance averages across the two threads.
These are single-run numbers; repeating each case and reporting median values would reduce measurement noise.

| Distance | Scenario | Avg TOT_INS | Avg TOT_CYC | Avg cyc/iter | Avg CPI | Avg L2_DCM | Avg L1_DCA/iter |
|---|---|---:|---:|---:|---:|---:|---:|
| 0 | True sharing | 8,000,312.5 | 31,126,222 | 31.127 | 3.891 | 160,323.0 | 6.318 |
| 1 | False sharing | 8,000,313.0 | 33,650,363 | 33.651 | 4.206 | 173,194.0 | 6.593 |
| 64 | No sharing | 8,000,309.5 | 8,026,958 | 8.027 | 1.003 | 79.5 | 3.001 |

### Why this proves false sharing

1. `distance = 1` is significantly slower than `distance = 64`:
  - `cyc/iter`: `33.651` vs `8.027` (about 4.2x higher)
  - `CPI`: `4.206` vs `1.003` (about 4.2x higher)

2. Work amount is effectively identical:
  - `TOT_INS` stays around `8.0M` in all cases.
  - Therefore, the large cycle increase for `distance = 1` is not caused by more executed work.

3. Memory/cache traffic indicators increase in the sharing cases:
  - `L2_DCM` and `L1_DCA/iter` are much higher for `distance = 1` than for `distance = 64`.
  - This is consistent with cache-line ping-pong and coherence overhead when two threads write different words in the same cache line.

4. `distance = 0` (true sharing) is also slower than `distance = 64`, but the key false-sharing evidence is `distance = 1` vs `distance = 64`, because those runs execute the same algorithm with only memory placement changed.

Conclusion: the measurements demonstrate false sharing, as the `distance = 1` case has much higher cost per instruction and per iteration while instruction counts remain unchanged.

## Builing and running
```
make
mkdir -p results
for d in 0 1 64; do
  ./false-sharing "$d" 1 > "results/d${d}.log"
done
```