// comparison.c
//
// Comparison between a normalized factorial approximation
// and a reference calculation based on lgamma().
//
// Alan Rodriguez Bojorjes
// Master's Candidate in Electronics Engineering
// Universidad Autonoma de San Luis Potosi

#include <stdio.h>
#include <time.h>
#include <math.h>

struct longfact {
    double coef;
    int n;
    long long exp;
};


/*
 * Original normalized factorial algorithm.
 *
 * Represents:
 *
 *      n! ~= coef x 10^exp
 *
 * The coefficient is kept approximately
 * in the range [1, 10).
 */
struct longfact detLF(int n) {

    struct longfact lf;
    lf.n = n;

    if (n <= 1) {
        lf.exp = 0;
        lf.coef = 1.0;
        return lf;
    }

    double actual = n;
    int anterior;
    long long cont = 0;

    for (int k = 1; k < n; k++) {

        anterior = n - k;
        actual *= anterior;

        while (actual >= 10.0) {
            actual *= 0.1;
            cont++;
        }
    }

    lf.exp = cont;
    lf.coef = actual;

    return lf;
}


/*
 * Reference calculation using:
 *
 *      ln(n!) = lgamma(n + 1)
 *
 * The result is converted to:
 *
 *      n! ~= coef x 10^exp
 */
struct longfact referenceLF(int n) {

    struct longfact lf;
    lf.n = n;

    if (n <= 1) {
        lf.coef = 1.0;
        lf.exp = 0;
        return lf;
    }

    double log10_fact =
        lgamma((double)n + 1.0) / log(10.0);

    double integer_part;

    double fractional_part =
        modf(log10_fact, &integer_part);

    lf.exp = (long long)integer_part;

    lf.coef = pow(10.0, fractional_part);

    return lf;
}


/*
 * Relative error between coefficients.
 */
double relativeError(
    double calculated,
    double reference
) {

    return fabs(calculated - reference)
           / fabs(reference);
}


/*
 * Determines the number of repetitions
 * used for each benchmark.
 *
 * Larger factorials require fewer repetitions
 * because the custom algorithm is O(n).
 */
int getRepetitions(int n) {

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


/*
 * Benchmarks the custom algorithm.
 */
double benchmarkCustom(
    int n,
    int repetitions,
    struct longfact *result
) {

    clock_t start = clock();

    for (int i = 0; i < repetitions; i++) {
        *result = detLF(n);
    }

    clock_t end = clock();

    double total_time =
        (double)(end - start)
        / CLOCKS_PER_SEC;

    return total_time / repetitions;
}


/*
 * Benchmarks the lgamma() reference method.
 */
double benchmarkReference(
    int n,
    int repetitions,
    struct longfact *result
) {

    clock_t start = clock();

    for (int i = 0; i < repetitions; i++) {
        *result = referenceLF(n);
    }

    clock_t end = clock();

    double total_time =
        (double)(end - start)
        / CLOCKS_PER_SEC;

    return total_time / repetitions;
}


/*
 * Prints the comparison results.
 */
void printResult(
    int n,
    int repetitions,
    struct longfact custom,
    struct longfact reference,
    double custom_time,
    double reference_time
) {

    double error =
        relativeError(
            custom.coef,
            reference.coef
        );

    double speed_ratio = 0.0;

    if (reference_time > 0.0) {

        speed_ratio =
            custom_time / reference_time;
    }

    printf("\n");

    printf(
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
        "  %.12f x 10^%lld\n",
        custom.coef,
        custom.exp
    );

    printf(
        "  Average time: %.12e s\n",
        custom_time
    );


    printf("\n");


    printf(
        "lgamma() reference:\n"
    );

    printf(
        "  %.12f x 10^%lld\n",
        reference.coef,
        reference.exp
    );

    printf(
        "  Average time: %.12e s\n",
        reference_time
    );


    printf("\n");


    printf(
        "Comparison:\n"
    );

    printf(
        "  Exponent difference: %lld\n",
        custom.exp - reference.exp
    );

    printf(
        "  Coefficient relative error: %.12e\n",
        error
    );

    printf(
        "  Relative error: %.12e %%\n",
        error * 100.0
    );

    if (speed_ratio > 0.0) {

        printf(
            "  Custom/reference time ratio: %.3f\n",
            speed_ratio
        );
    }

    printf(
        "--------------------------------------------\n"
    );
}


int main(void) {


    int test_values[] = {
        10,
        100,
        1000,
        10000,
        100000,
        1000000
    };


    int num_tests =
        sizeof(test_values)
        / sizeof(test_values[0]);


    FILE *file =
        fopen(
            "results/comparison.csv",
            "w"
        );


    if (file == NULL) {

        printf(
            "Error: Could not create "
            "results/comparison.csv\n"
        );

        return 1;
    }


    /*
     * CSV header.
     */
    fprintf(
        file,

        "n,"
        "repetitions,"
        "custom_coefficient,"
        "custom_exponent,"
        "custom_average_time_seconds,"
        "reference_coefficient,"
        "reference_exponent,"
        "reference_average_time_seconds,"
        "exponent_difference,"
        "relative_error,"
        "time_ratio\n"
    );


    printf("\n");

    printf(
        "============================================\n"
    );

    printf(
        " LARGE FACTORIAL APPROXIMATION COMPARISON\n"
    );

    printf(
        "============================================\n"
    );


    for (
        int i = 0;
        i < num_tests;
        i++
    ) {

        int n = test_values[i];

        int repetitions =
            getRepetitions(n);


        struct longfact custom;
        struct longfact reference;


        /*
         * Benchmark custom algorithm.
         */
        double custom_time =
            benchmarkCustom(
                n,
                repetitions,
                &custom
            );


        /*
         * Benchmark reference algorithm.
         */
        double reference_time =
            benchmarkReference(
                n,
                repetitions,
                &reference
            );


        /*
         * Calculate accuracy.
         */
        double error =
            relativeError(
                custom.coef,
                reference.coef
            );


        double time_ratio = 0.0;

        if (reference_time > 0.0) {

            time_ratio =
                custom_time
                / reference_time;
        }


        /*
         * Print results.
         */
        printResult(
            n,
            repetitions,
            custom,
            reference,
            custom_time,
            reference_time
        );


        /*
         * Save results to CSV.
         */
        fprintf(
            file,

            "%d,"
            "%d,"
            "%.15e,"
            "%lld,"
            "%.15e,"
            "%.15e,"
            "%lld,"
            "%.15e,"
            "%lld,"
            "%.15e,"
            "%.15e\n",

            n,
            repetitions,

            custom.coef,
            custom.exp,
            custom_time,

            reference.coef,
            reference.exp,
            reference_time,

            custom.exp - reference.exp,

            error,

            time_ratio
        );
    }


    fclose(file);


    printf("\n");

    printf(
        "Results saved to:\n"
    );

    printf(
        "results/comparison.csv\n"
    );


    return 0;
}