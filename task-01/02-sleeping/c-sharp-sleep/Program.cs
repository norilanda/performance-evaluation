using System.Diagnostics;

int defaultIterations = 400;
int defaultSleepMs = 1;

RunSleepTest(defaultIterations, defaultSleepMs);

static void RunSleepTest(int iterations, int sleepMs)
{
    int warmUpIterations = iterations;

    long[] timeStamps = new long[iterations];
    long[] warmUpTimeStamps = new long[warmUpIterations];
    TimeSpan[] deltas = new TimeSpan[iterations - 1];
    TimeSpan[] warmUpDeltas = new TimeSpan[warmUpIterations - 1];

    // Warm-Up
    for (int i = 0; i < warmUpIterations; i++)
    {
        warmUpTimeStamps[i] = Stopwatch.GetTimestamp();
        Thread.Sleep(sleepMs);
    }

    // Actual Test
    for (int i = 0; i < iterations; i++)
    {
        timeStamps[i] = Stopwatch.GetTimestamp();
        Thread.Sleep(sleepMs);
    }

    // Calculate Deltas
    for (int i = 0; i < iterations - 1; i++)
    {
        deltas[i] = Stopwatch.GetElapsedTime(timeStamps[0], timeStamps[i + 1]);
        warmUpDeltas[i] = Stopwatch.GetElapsedTime(warmUpTimeStamps[0], warmUpTimeStamps[i + 1]);
    }

    var filePath = $"sleep-results-{sleepMs}ms.csv";
    var filePathWarmUp = $"sleep-results-warmup-{sleepMs}ms.csv";

    using (StreamWriter writer = new(filePath))
    {
        for (int i = 0; i < iterations - 1; ++i)
        {
            writer.WriteLine(deltas[i].TotalMilliseconds);
        }
    }


    using (StreamWriter writer = new(filePathWarmUp))
    {
        for (int i = 0; i < iterations - 1; ++i)
        {
            writer.WriteLine(warmUpDeltas[i].TotalMilliseconds);
        }
    }
}
