# Large Factorial Approximation in C

A numerical experiment in C for approximating extremely large factorials using a normalized coefficient and decimal exponent.

The project implements a custom iterative algorithm capable of representing values such as:

```text
1,000,000!
```

without storing the complete integer in memory.

The implementation also includes a benchmarking and validation program that compares the custom algorithm against the standard mathematical function `lgamma()`.

---

## Project Description

Factorials grow extremely quickly. Even relatively small factorials exceed the range of standard numerical types, making it impractical to store the complete integer using types such as `int`, `long`, or `double`.

This project uses a scientific-notation-like representation:

```text
n! ≈ coefficient × 10^exponent
```

Instead of allowing the intermediate factorial value to grow indefinitely, the algorithm continuously normalizes the result whenever its magnitude exceeds `10`.

Each normalization step contributes to a decimal exponent counter.

This makes it possible to represent extremely large factorials using only:

* A floating-point coefficient.
* An integer decimal exponent.

For example:

```text
100! ≈ 9.332621544394 × 10^157
```

and:

```text
1,000,000! ≈ 8.263931690787 × 10^5565708
```

The complete decimal representation of `1,000,000!` contains more than five million digits, but the algorithm stores only a coefficient and exponent.

---

## Mathematical Principle

A factorial is defined as:

```text
n! = n × (n - 1) × (n - 2) × ... × 2 × 1
```

For example:

```text
10! = 3,628,800
```

As `n` increases, the number of digits grows rapidly.

Instead of storing the complete result, this project represents the factorial as:

```text
n! ≈ coef × 10^exp
```

where:

* `coef` is the normalized coefficient.
* `exp` is the accumulated decimal exponent.

The coefficient is maintained approximately within the interval:

```text
1 ≤ coef < 10
```

---

## Algorithm

The calculation begins with:

```text
actual = n
```

The remaining factors are multiplied sequentially:

```text
actual = n × (n - 1) × (n - 2) × ... × 1
```

After each multiplication, the algorithm checks whether the intermediate value exceeds `10`.

Whenever:

```text
actual ≥ 10
```

the value is normalized:

```text
actual = actual × 0.1
```

while the exponent counter is incremented:

```text
exp = exp + 1
```

This process continues until:

```text
1 ≤ actual < 10
```

The final approximation is represented as:

```text
n! ≈ actual × 10^exp
```

---

## Data Structure

The result is stored using the following structure:

```c
struct longfact {
    double coef;
    int n;
    int exp;
};
```

The members represent:

* `coef`: normalized factorial coefficient.
* `n`: factorial input value.
* `exp`: decimal exponent.

For example:

```text
100! ≈ 9.332621544394 × 10^157
```

can be represented as:

```text
coef = 9.332621544394
exp  = 157
```

---

## Project Structure

```text
longfact/
│
├── src/
│   ├── longfact.c
│   └── comparison.c
│
├── results/
│   ├── comparison.csv
│   └── plots/
│       ├── execution_time.png
│       └── relative_error.png
│
├── docs/
│
├── README.md
└── graphs.py
```

### `longfact.c`

Contains the original implementation of the factorial approximation algorithm.

The main function is:

```c
struct longfact detLF(int n)
```

It calculates the factorial iteratively while continuously normalizing the intermediate result.

### `comparison.c`

Validates and benchmarks the custom implementation.

The program compares:

```text
Custom normalized factorial algorithm
```

against:

```text
lgamma()
```

The comparison includes:

* Normalized coefficient.
* Decimal exponent.
* Exponent difference.
* Coefficient relative error.
* Relative error percentage.
* Average execution time.
* Execution-time ratio.

The results are exported to:

```text
results/comparison.csv
```

### `graphs.py`

Reads the comparison results and generates plots for:

* Execution time.
* Relative error.

The generated figures are stored in:

```text
results/plots/
```

---

## Validation Against `lgamma()`

The project uses the mathematical identity:

```text
ln(n!) = lgamma(n + 1)
```

to generate a reference approximation.

The reference representation is obtained from:

```text
log10(n!) = lgamma(n + 1) / ln(10)
```

The decimal exponent is then calculated as:

```text
exponent = floor(log10(n!))
```

and the normalized coefficient is obtained from the fractional part of the logarithm.

This provides an independent numerical reference for evaluating the custom algorithm.

---

## Test Results

The custom algorithm was tested for the following values:

```text
N = 10
N = 100
N = 1,000
N = 10,000
N = 100,000
N = 1,000,000
```

The results show that the custom algorithm correctly reproduces the decimal exponent for all tested values.

For example:

|         N | Custom exponent | Reference exponent | Relative error |
| --------: | --------------: | -----------------: | -------------: |
|        10 |               6 |                  6 | 1.96 × 10⁻¹³ % |
|       100 |             157 |                157 | 2.11 × 10⁻¹² % |
|     1,000 |            2567 |               2567 | 1.49 × 10⁻¹⁰ % |
|    10,000 |           35659 |              35659 | 5.24 × 10⁻¹⁰ % |
|   100,000 |          456573 |             456573 |  7.28 × 10⁻⁹ % |
| 1,000,000 |         5565708 |            5565708 |  2.69 × 10⁻⁷ % |

Even for:

```text
1,000,000!
```

the algorithm reproduces the correct decimal order of magnitude, with a very small relative error in the normalized coefficient.

---

## Execution Time Comparison

The benchmarking program compares the execution time of the custom algorithm against the standard `lgamma()` implementation.

The custom algorithm performs approximately:

```text
O(n)
```

operations because it explicitly iterates through all factorial terms:

```text
n × (n - 1) × ... × 1
```

In contrast, `lgamma()` is a highly optimized mathematical library implementation and is significantly faster for large values of `n`.

The purpose of this comparison is not to outperform `lgamma()`, but to validate the accuracy and computational behavior of a simple direct normalization approach.

The execution-time results illustrate the expected difference between:

```text
Direct iterative multiplication
```

and:

```text
Optimized mathematical library functions
```

---

## Results

### Execution Time

The execution-time comparison is generated from:

```text
results/comparison.csv
```

and saved as:

```text
results/plots/execution_time.png
```

The plot uses logarithmic scales to visualize the difference in execution time across multiple orders of magnitude.

The `1,000,000!` `lgamma()` timing result is excluded from the timing comparison when the measured value reaches the timer resolution limit.

### Relative Error

The numerical accuracy is visualized in:

```text
results/plots/relative_error.png
```

The relative error remains very small throughout the tested range, although floating-point rounding errors gradually accumulate as the number of multiplications increases.

---

## Example

For:

```text
10!
```

the exact result is:

```text
10! = 3,628,800
```

The algorithm produces:

```text
10! ≈ 3.628800 × 10^6
```

For:

```text
1,000,000!
```

the algorithm produces approximately:

```text
1,000,000! ≈ 8.263931690787 × 10^5565708
```

The reference calculation using `lgamma()` gives:

```text
1,000,000! ≈ 8.263931668545 × 10^5565708
```

The decimal exponent is identical in both cases.

---

## Features

* Large factorial approximation.
* Scientific notation representation.
* Continuous decimal normalization.
* Dynamic exponent tracking.
* Constant memory usage with respect to factorial size.
* Standard C implementation.
* Validation against `lgamma()`.
* Relative error analysis.
* Execution-time benchmarking.
* CSV result generation.
* Python-based result visualization.
* Approximation of factorials up to at least `1,000,000!`.

---

## Requirements

### C Compiler

The project can be compiled using Clang:

```text
clang
```

### C Libraries

The implementation uses standard C libraries:

```c
#include <stdio.h>
#include <time.h>
#include <math.h>
```

### Python

Python is required only for generating plots.

The graphing script requires:

```text
matplotlib
```

Install it with:

```bash
python3 -m pip install matplotlib
```

---

## Compilation

### Compile the factorial approximation program

Using Clang:

```bash
clang -O3 src/longfact.c -o longfact
```

Run:

```bash
./longfact
```

### Compile the comparison program

```bash
clang -O3 src/comparison.c -o comparison -lm
```

Run:

```bash
./comparison
```

The comparison results will be saved to:

```text
results/comparison.csv
```

---

## Generating the Plots

After running the comparison program:

```bash
python3 graphs.py
```

The generated plots will be stored in:

```text
results/plots/
```

Expected output:

```text
results/plots/execution_time.png
results/plots/relative_error.png
```

---

## Computational Complexity

The current implementation performs one multiplication for each factorial term.

Therefore, the computational complexity is approximately:

```text
Time complexity: O(n)
```

The algorithm stores only a fixed number of variables regardless of the number of digits in the factorial result.

Therefore:

```text
Space complexity: O(1)
```

This refers to the memory required by the approximation algorithm itself.

---

## Limitations

This project is designed as a numerical approximation rather than an arbitrary-precision factorial calculator.

The implementation uses:

```text
double
```

for the normalized coefficient.

As a result, floating-point rounding errors accumulate during very large calculations.

The result should therefore be interpreted as:

```text
n! ≈ coef × 10^exp
```

rather than as the exact integer value of `n!`.

Additionally, the current normalization method may require multiple scaling operations after a multiplication, although the intermediate value is continuously kept within a manageable numerical range.

The algorithm is primarily intended as an exploration of numerical scaling, approximation, and large-number representation.

---

## Possible Improvements

Future versions could explore:

* `long double` precision.
* Alternative normalization strategies.
* Logarithmic accumulation.
* Stirling's approximation.
* Arbitrary-precision arithmetic.
* Parallel factorial decomposition.
* SIMD optimization.
* Multi-threaded multiplication.
* More detailed error analysis.
* Comparison with GMP or MPFR.
* Higher-resolution benchmarking.
* Hardware performance counters.

---

## Concepts Demonstrated

This project demonstrates:

* C programming.
* Structures.
* Iterative algorithms.
* Numerical approximation.
* Scientific notation.
* Floating-point arithmetic.
* Numerical scaling.
* Normalization techniques.
* Factorial computation.
* Algorithmic complexity.
* Benchmarking.
* Numerical validation.
* CSV data generation.
* Data visualization.

---

## Author

**Alan Rodríguez Bojorjes**

Master's Candidate in Electronics Engineering
Expected graduation: September 2026

Universidad Autónoma de San Luis Potosí

---

## Notes

The purpose of this project is to explore a simple numerical representation that allows extremely large factorials to be approximated without requiring arbitrary-precision integer storage.

The algorithm continuously rescales the intermediate result and tracks the corresponding decimal exponent, allowing factorials with millions of decimal digits to be represented using a compact structure.

The benchmarking section was added to evaluate both the numerical accuracy of the method and its computational cost relative to the standard mathematical library implementation based on `lgamma()`.
