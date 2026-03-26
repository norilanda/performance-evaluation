## Platform
- CPU model: AMD Ryzen 5 PRO 4650G with Radeon Graphics
- OS: Gentoo Linux, kernel 6.12.77
- PAPI: 7.1.0.0

## False Sharing Results
Measurements were collected with 10 runs for each distance value (`0`, `1`, `64`) on CPUs `0` and `1`:

- `./false-sharing 0 1` (true sharing)
- `./false-sharing 1 1` (false sharing)
- `./false-sharing 64 1` (no sharing)

The program prints results for both threads. The table below reports per-distance median values from the collected runs.

| Distance | Scenario | Median TOT_INS | Median TOT_CYC | Median cyc/iter | Median CPI | Median L2_DCM | Median L1_DCA/iter |
|---|---|---:|---:|---:|---:|---:|---:|
| 0 | True sharing | 8,000,313 | 31,434,400 | 31.4345 | 3.9295 | 159,854 | 6.20491 |
| 1 | False sharing | 8,000,313 | 31,910,268 | 31.9105 | 3.9885 | 166,226 | 6.43213 |
| 64 | No sharing | 8,000,307 | 8,003,654 | 8.0035 | 1.0000 | 32.5 | 3.00039 |

### Why this proves false sharing

1. `distance = 1` is significantly slower than `distance = 64`:
  - `cyc/iter`: `31.9105` vs `8.0035` (about 4.0x higher)
  - `CPI`: `3.9885` vs `1.0000` (about 4.0x higher)

2. Work amount is effectively identical:
  - `TOT_INS` stays around `8.0M` in all cases.
  - Therefore, the large cycle increase for `distance = 1` is not caused by more executed work.

3. Memory/cache traffic indicators increase in the sharing cases:
  - `L2_DCM` and `L1_DCA/iter` are much higher for `distance = 1` than for `distance = 64`.
  - This is consistent with cache-line ping-pong and coherence overhead when two threads write different words in the same cache line.

4. `distance = 0` (true sharing) is also slower than `distance = 64`, but the key false-sharing evidence is `distance = 1` vs `distance = 64`, because those runs execute the same algorithm with only memory placement changed.

Conclusion: the measurements demonstrate false sharing, as the `distance = 1` case has much higher cost per instruction and per iteration while instruction counts remain unchanged.

## Builing and running
### Running the experiments
```
make
mkdir -p results
for d in 0 1 64; do
  : > "results/d${d}.log"
  for r in $(seq 1 10); do
    ./false-sharing "$d" 1 >> "results/d${d}.log"
  done
done
```

```
median() { sort -n | awk '{a[NR]=$1} END{if(NR==0){print "n/a"; exit} if(NR%2){print a[(NR+1)/2]} else {print (a[NR/2]+a[NR/2+1])/2}}'; }

for d in 0 1 64; do
  f="results/d${d}.log"
  echo "distance=$d"
  echo -n "  TOT_INS median: "; grep -o 'TOT_INS=[0-9.]*' "$f" | cut -d= -f2 | median
  echo -n "  TOT_CYC median: "; grep -o 'TOT_CYC=[0-9.]*' "$f" | cut -d= -f2 | median
  echo -n "  cyc/iter median: "; grep -o 'cyc/iter=[0-9.]*' "$f" | cut -d= -f2 | median
  echo -n "  CPI median: "; grep -o 'CPI=[0-9.]*' "$f" | cut -d= -f2 | median
  echo -n "  L2_DCM median: "; grep -o 'L2_DCM=[0-9.]*' "$f" | cut -d= -f2 | median
  echo -n "  L1_DCA/iter median: "; grep -o 'L1_DCA/iter=[0-9.]*' "$f" | cut -d= -f2 | median
done
```