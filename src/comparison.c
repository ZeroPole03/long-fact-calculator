// comparison.c
// Alan Rodriguez Bojorjes
// Comparison between a custom large factorial approximation
// and the lgamma() mathematical reference.
//
// Compile with:
// clang -O2 -Wall -Wextra -o comparison src/comparison.c -lm

#include <stdio.h>
#include <math.h>
#include <time.h>

#define NUM_TESTS 6

volatile double benchmark_sink = 0.0;


/*
 * Structure used to represent:
 *
 *      n! ~= coef x 10^exp
 */
typedef struct {
    double coef;
    int n;
    int exp;
} longfact;


/*
 * Custom factorial approximation.
 *
 * The mantissa is continuously normalized to keep
 * the intermediate result in a manageable range.
 */
longfact detLF(int n)
{
    longfact lf;
    lf.n = n;

    if (n <= 1) {
        lf.coef = 1.0;
        lf.exp = 0;
        return lf;
    }

    double actual = (double)n;
    int cont = 0;

    for (int k = 1; k < n; k++) {

        double anterior = (double)(n - k);

        actual *= anterior;

        while (actual >= 10.0) {
            actual *= 0.1;
            cont++;
        }
    }

    lf.coef = actual;
    lf.exp = cont;

    return lf;
}


/*
 * Reference implementation using:
 *
 *      log10(n!) = lgamma(n + 1) / ln(10)
 */
longfact lgamma_factorial(int n)
{
    longfact lf;
    lf.n = n;

    if (n <= 1) {
        lf.coef = 1.0;
        lf.exp = 0;
        return lf;
    }

    double log_fact = lgamma((double)n + 1.0);

    double log10_fact = log_fact / log(10.0);

    int exponent = (int)floor(log10_fact);

    double coefficient =
        pow(10.0, log10_fact - exponent);

    lf.coef = coefficient;
    lf.exp = exponent;

    return lf;
}


/*
 * High-resolution monotonic timer.
 */
double get_time_seconds(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (double)ts.tv_sec +
           (double)ts.tv_nsec * 1e-9;
}


/*
 * Benchmark the custom algorithm.
 */
double benchmark_custom(int n, int repetitions)
{
    double start = get_time_seconds();

    for (int i = 0; i < repetitions; i++) {

        longfact result = detLF(n);

        benchmark_sink +=
            result.coef + (double)result.exp;
    }

    double end = get_time_seconds();

    return (end - start) / repetitions;
}


/*
 * Benchmark the lgamma() reference.
 */
double benchmark_lgamma(int n, int repetitions)
{
    double start = get_time_seconds();

    for (int i = 0; i < repetitions; i++) {

        longfact result = lgamma_factorial(n);

        benchmark_sink +=
            result.coef + (double)result.exp;
    }

    double end = get_time_seconds();

    return (end - start) / repetitions;
}


/*
 * Select a reasonable number of repetitions
 * depending on the size of N.
 */
int get_repetitions(int n)
{
    if (n <= 100)
        return 100000;

    if (n <= 1000)
        return 10000;

    if (n <= 10000)
        return 1000;

    if (n <= 100000)
        return 100;

    return 10;
}


int main(void)
{
    int test_values[NUM_TESTS] = {
        10,
        100,
        1000,
        10000,
        100000,
        1000000
    };

    FILE *file;

    file = fopen(
        "results/comparison.csv",
        "w"
    );

    if (file == NULL) {
        printf(
            "Error: could not create "
            "results/comparison.csv\n"
        );

        return 1;
    }


    fprintf(
        file,
        "N,repetitions,"
        "custom_coefficient,"
        "custom_exponent,"
        "lgamma_coefficient,"
        "lgamma_exponent,"
        "exponent_difference,"
        "coefficient_relative_error,"
        "relative_error_percent,"
        "custom_time,"
        "lgamma_time,"
        "time_ratio\n"
    );


    printf(
        "\n"
        "============================================\n"
        " LARGE FACTORIAL APPROXIMATION COMPARISON\n"
        "============================================\n"
    );


    for (int i = 0; i < NUM_TESTS; i++) {

        int n = test_values[i];

        int repetitions =
            get_repetitions(n);


        /*
         * Calculate both results once.
         */
        longfact custom =
            detLF(n);

        longfact reference =
            lgamma_factorial(n);


        /*
         * Benchmark both implementations.
         */
        double custom_time =
            benchmark_custom(
                n,
                repetitions
            );

        double lgamma_time =
            benchmark_lgamma(
                n,
                repetitions
            );


        /*
         * Accuracy comparison.
         */
        int exponent_difference =
            custom.exp - reference.exp;

        double coefficient_relative_error =
            fabs(
                custom.coef -
                reference.coef
            ) /
            fabs(reference.coef);

        double relative_error_percent =
            coefficient_relative_error *
            100.0;


        /*
         * Time comparison.
         */
        double time_ratio = 0.0;

        if (lgamma_time > 0.0) {

            time_ratio =
                custom_time /
                lgamma_time;
        }


        /*
         * Print results.
         */
        printf(
            "\n"
            "N = %d\n",
            n
        );

        printf(
            "Repetitions: %d\n",
            repetitions
        );

        printf(
            "--------------------------------------------\n"
        );


        printf(
            "Custom algorithm:\n"
        );

        printf(
            "  %.12f x 10^%d\n",
            custom.coef,
            custom.exp
        );

        printf(
            "  Average time: %.12e s\n",
            custom_time
        );


        printf(
            "\n"
            "lgamma() reference:\n"
        );

        printf(
            "  %.12f x 10^%d\n",
            reference.coef,
            reference.exp
        );

        printf(
            "  Average time: %.12e s\n",
            lgamma_time
        );


        printf(
            "\n"
            "Comparison:\n"
        );

        printf(
            "  Exponent difference: %d\n",
            exponent_difference
        );

        printf(
            "  Coefficient relative error: %.12e\n",
            coefficient_relative_error
        );

        printf(
            "  Relative error: %.12e %%\n",
            relative_error_percent
        );


        if (lgamma_time > 0.0) {

            printf(
                "  Custom/reference "
                "time ratio: %.3f\n",
                time_ratio
            );
        }


        printf(
            "--------------------------------------------\n"
        );


        /*
         * Save to CSV.
         */
        fprintf(
            file,
            "%d,%d,"
            "%.15e,%d,"
            "%.15e,%d,"
            "%d,"
            "%.15e,"
            "%.15e,"
            "%.15e,"
            "%.15e,"
            "%.15e\n",

            n,
            repetitions,

            custom.coef,
            custom.exp,

            reference.coef,
            reference.exp,

            exponent_difference,

            coefficient_relative_error,

            relative_error_percent,

            custom_time,

            lgamma_time,

            time_ratio
        );
    }


    fclose(file);


    printf(
        "\n"
        "Results saved to:\n"
        "results/comparison.csv\n"
    );


    printf(
        "\n"
        "Benchmark sink: %.6f\n",
        benchmark_sink
    );


    return 0;
}