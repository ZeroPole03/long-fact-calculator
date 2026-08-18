# Large Factorial Approximation in C

## Project Description

This project implements a numerical algorithm in C for approximating extremely large factorials without directly storing the complete result in memory.

The algorithm represents a factorial using a normalized coefficient and a decimal exponent:

```text
n! ≈ coefficient × 10^exponent
```

Instead of allowing the intermediate factorial value to grow indefinitely, the algorithm continuously normalizes the result whenever its magnitude exceeds 10. Each decimal scaling operation is recorded in an exponent counter.

This approach makes it possible to approximate factorials whose exact numerical representation would be impractical to store using standard floating-point or integer data types.

The implementation has been tested with very large values, including:

```text
1,000,000!
```

The project also measures the execution time required to perform the calculation.

---

## Mathematical Principle

A factorial is defined as:

```text
n! = n × (n - 1) × (n - 2) × ... × 2 × 1
```

Factorials grow extremely quickly.

For example:

```text
10! = 3,628,800
```

However, as `n` increases, the number of digits grows rapidly. Storing the exact value eventually becomes impossible using standard numerical data types such as `int`, `long`, or `double`.

Instead of storing the complete factorial, this algorithm uses a scientific-notation-like representation:

```text
n! ≈ coef × 10^exp
```

where:

- `coef` is the normalized coefficient.
- `exp` stores the accumulated decimal exponent.

---

## Algorithm

The factorial calculation begins with:

```text
actual = n
```

The remaining factors are multiplied sequentially:

```text
actual = n × (n - 1) × (n - 2) × ... × 1
```

After each multiplication, the algorithm checks whether the current value exceeds 10.

Whenever:

```text
actual ≥ 10
```

the value is normalized by multiplying it by:

```text
0.1
```

and the decimal exponent counter is incremented:

```text
actual = actual × 0.1
exp = exp + 1
```

This process is repeated until:

```text
1 ≤ actual < 10
```

The final factorial approximation is stored as:

```text
n! ≈ actual × 10^exp
```

---

## Data Structure

The factorial approximation is represented using the following structure:

```c
struct longfact {
    double coef;
    int n;
    int exp;
};
```

The members represent:

- `coef`: normalized factorial coefficient.
- `n`: factorial input value.
- `exp`: decimal exponent.

For example:

```text
100! ≈ 9.332621 × 10^157
```

would be represented approximately as:

```text
coef = 9.332621
exp  = 157
```

---

## Implementation

The main approximation function is:

```c
struct longfact detLF(int n)
```

The function calculates the factorial sequentially while preventing the intermediate result from growing indefinitely.

The normalization process is implemented using:

```c
while (actual >= 10) {
    actual *= 0.1f;
    cont++;
}
```

Each iteration moves the decimal point one position to the left and increments the exponent counter.

At the end of the calculation:

```c
lf.exp = cont;
lf.coef = actual;
```

The factorial can then be represented as:

```text
n! ≈ coef × 10^exp
```

---

## Example

For a factorial such as:

```text
10!
```

the exact value is:

```text
10! = 3,628,800
```

The algorithm represents the result approximately as:

```text
10! ≈ 3.628800 × 10^6
```

The same principle can be extended to much larger factorials.

For example, the algorithm can approximate:

```text
1,000,000!
```

without attempting to store the complete integer value.

---

## Features

- Large factorial approximation
- Scientific notation representation
- Continuous decimal normalization
- Dynamic exponent tracking
- Constant memory usage
- Standard C implementation
- Execution time measurement
- Support for extremely large factorial inputs
- No external libraries required

---

## Time Measurement

The program measures the execution time using the C standard library:

```c
clock_t t;
```

The execution time is calculated using:

```c
double time_taken = ((double)t) / CLOCKS_PER_SEC;
```

This allows the computational cost of large factorial calculations to be evaluated.

---

## Requirements

The project uses only standard C libraries:

```c
#include <stdio.h>
#include <time.h>
```

No external dependencies are required.

---

## Compilation

Using `gcc`:

```bash
gcc -O3 factorial.c -o factorial
```

Run the program with:

```bash
./factorial
```

---

## Example Execution

The program requests the factorial input:

```text
de que numero desea obtener el factorial
```

For example:

```text
1000000
```

The program calculates the approximation and displays a result similar to:

```text
1000000! ~= coefficient × 10^exponent
```

The exact coefficient may vary slightly depending on floating-point precision.

The program also reports the execution time:

```text
fact() tardo X.XXXXXX segundos en ejecutarse
```

---

## Computational Approach

The algorithm does not calculate or store the complete integer representation of the factorial.

Instead, it maintains only:

```text
coefficient
exponent
```

This keeps memory usage effectively constant with respect to the size of the factorial result.

The computational complexity of the current implementation is approximately:

```text
O(n)
```

with respect to the number of factorial multiplications.

---

## Limitations

This implementation is designed as a numerical approximation rather than an arbitrary-precision factorial calculator.

Because the calculation uses the `double` data type, floating-point rounding errors may accumulate during very large calculations.

The result should therefore be interpreted as:

```text
n! ≈ coef × 10^exp
```

rather than an exact integer representation.

The algorithm is particularly useful when the order of magnitude of a factorial is more important than its complete decimal representation.

---

## Possible Improvements

Several improvements could be explored in future versions:

- Higher precision using `long double`
- Logarithmic factorial estimation
- Comparison with Stirling's approximation
- Arbitrary-precision arithmetic
- Parallel factorial decomposition
- SIMD optimization
- Multi-threaded multiplication
- Error analysis for large values of `n`
- Comparison against mathematical libraries

---

## Concepts Demonstrated

This project demonstrates:

- C programming
- Structures
- Iterative algorithms
- Numerical approximation
- Scientific notation
- Floating-point arithmetic
- Factorial computation
- Algorithmic complexity
- Execution time measurement
- Numerical scaling and normalization

---

## Author

**Alan Rodríguez Bojorjes**

Master's Program in Electronics Engineering  
Universidad Autónoma de San Luis Potosí

---

## Notes

The purpose of this project is to explore a simple numerical representation that allows extremely large factorials to be approximated without requiring arbitrary-precision integer storage.

The algorithm continuously rescales the intermediate result and tracks the corresponding decimal exponent, allowing factorials with extremely large magnitudes to be represented using a compact structure.