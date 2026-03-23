# Grading

## Timers [5]

- I think this is the first time ever we had an SQL timer :-) [+1]

- It is usually better to use the same code for warm up and measurement.

- You state that the SQL timer granularity is 100 ns,
  but your data shows minimum delta around 990 us ? [-1]

## Sleeping [5]

- Your plot axis labels are poorly chosen. Instead of "Sample" and
  "Timestamp Milliseconds", I would expect something like "Requested
  Sleep Time" and "Actual Sleep Time".

- It is not clear whether the overhead you report
  stays stable across multiple sleep durations.

Total **10** points.

# Hash

The repository hash used in this grading is 694a65eaa1c4f3357aa8273b6e4932ee294101a2.
