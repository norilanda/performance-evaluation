# A Quick Introduction To R

## Help

```r
help (mean)
```

## Vectors

Vectors of numbers.
Indices start at 1.

```r
x <- c (2, 3, 4, 3, 3, 2, 5, 1, 2, 3)
```

Generate 5 numbers from normal distribution with mean of 10 and SD of 2 (N (10, 4)).

```r
x <- rnorm (5, 10, 2)
```

## Viewing Data

```r
print (x)
head (x)
x
```

## Reading Data from Files

```r
install.packages ('tidyverse')
library ('tidyverse')
x <- read_csv ('https://d3s.mff.cuni.cz/files/teaching/nswi131/labs/01/data.csv.gz')
```

## Plotting with Plain R

Assume `x` is a vector of numbers.

### Simple Plot

```r
plot (x)
plot (x, type='l') # Lines instead of dots
```

### Boxplot

Extremely useful to get a quick overview of what the data looks like (center and variability).
Can also be used for roughly comparing alternatives.

```r
boxplot (x)
```

### Histogram

To view the distribution (frequencies of individual values):

```r
hist (x)
```

### Zooming

```r
plot (x [2:20])
plot (x, xlim=c (9,11), ylim=c (3,8))
```

### Colors and Legend

```r
x1 <- rnorm (50, 10, 2)
x2 <- rnorm (50, 14, 2)
x3 <- rnorm (50, 18, 2)
plot (x1, ylim=c (0, max (c (x1,x2,x3))), col='blue')
points (x2, col='red')
points (x3, col='green')
legend ('topright', legend=c ('x1', 'x2', 'x3'),
    col=c ('blue', 'red', 'green'), pch=20)
```

## Plotting with Tidyverse GGPlot

```r
library ('tidyverse')
x <- read_csv ('https://d3s.mff.cuni.cz/files/teaching/nswi131/labs/01/data.csv.gz')
```

The value of `x` is of type tibble, which is a specific type of a table.

```r
x
```

```text
# A tibble: 117,971 x 4
   benchmark     score  loop   run
   <chr>         <dbl> <dbl> <dbl>
 1 alpha     461331349     1     1
 2 alpha     197597673     2     1
 3 alpha     180138333     3     1
 4 alpha     174773075     4     1
 5 alpha     328616794     5     1
 6 alpha     390904361     6     1
 7 alpha     138603295     7     1
 8 alpha     133475760     8     1
 9 alpha     153667583     9     1
10 alpha     131513684    10     1
# … with 117,961 more rows
```

### Plot One Run

```r
xx <- x |> filter (benchmark == 'alpha', run == 1)
```

Note that `xx` is again a tibble.

```r
ggplot (xx, aes (x=loop, y=score)) + geom_point ()
```

The `ggplot` function creates a plot object with given data as source.
The `aes` information provides defaults for data dependent values.
The `geom` functions add actual output to the plot object.

### More Runs with Colors

```r
xx <- x |> filter (benchmark == 'alpha')
ggplot (xx, aes (x=loop, y=score, color=run)) + geom_line (alpha=0.5)
```

### Boxplots

Factors are category types where each category has a name.
Internally factors are represented as integers.
Beware mixing factors with different levels.

```r
xx <- x |> filter (benchmark == 'alpha')
ggplot (xx, aes (x=factor (run), y=score)) + geom_boxplot ()
```

### Histograms

```r
xx <- x |> filter (benchmark == 'alpha')
ggplot (xx, aes (x=score)) + geom_histogram ()
ggplot (xx, aes (x=score, color=factor (run))) + geom_histogram (bins=1000)
```

### Facets

```r
ggplot (x, aes (x=score)) + geom_histogram (bins=1000) + facet_wrap (vars (benchmark))
ggplot (x, aes (x=score)) + geom_histogram (bins=1000) + facet_wrap (vars (benchmark), scale = 'free')
```
