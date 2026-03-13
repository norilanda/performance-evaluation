using System.Diagnostics;

Console.WriteLine($"Stopwatch frequency: {Stopwatch.Frequency} ticks per second");
Console.WriteLine($"Is the timer based on the high-resolution performance counter: {Stopwatch.IsHighResolution}");

int iterations = 1000;
int warmUpIterations = iterations;

long[] timeStamps = new long[iterations];
long[] warmUpTimeStamps = new long[warmUpIterations];
TimeSpan[] deltas = new TimeSpan[iterations - 1];
TimeSpan[] warmUpDeltas = new TimeSpan[warmUpIterations - 1];

// Warm-Up
for (int i = 0; i < warmUpIterations; i++)
{
    warmUpTimeStamps[i] = Stopwatch.GetTimestamp();
}

// Test

for (int i = 0; i < iterations; i++)
{
    timeStamps[i] = Stopwatch.GetTimestamp();
}

for (int i = 0; i < iterations - 1; i++)
{
    deltas[i] = Stopwatch.GetElapsedTime(timeStamps[0], timeStamps[i + 1]);
    warmUpDeltas[i] = Stopwatch.GetElapsedTime(warmUpTimeStamps[0], warmUpTimeStamps[i + 1]);
}

// Write results to file
string filePath = "c-sharp-timer-results-nanoseconds.csv";
string filePathWarmUp = "c-sharp-timer-results-nanoseconds-warmup.csv";

using (StreamWriter writer = new(filePath))
{
    for (int i = 0; i < iterations - 1; ++i)
    {
        writer.WriteLine(deltas[i].TotalNanoseconds);
    }
}


using (StreamWriter writer = new(filePathWarmUp))
{
    for (int i = 0; i < iterations - 1; ++i)
    {
        writer.WriteLine(warmUpDeltas[i].TotalNanoseconds);
    }
}

Console.WriteLine();