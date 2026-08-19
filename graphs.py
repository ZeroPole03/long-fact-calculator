import csv
import os
import matplotlib.pyplot as plt


CSV_FILE = "results/comparison.csv"
OUTPUT_DIR = "results/plots"

os.makedirs(OUTPUT_DIR, exist_ok=True)


# Lists for accuracy analysis
n_all = []
relative_error = []

# Lists for execution-time analysis
n_time = []
custom_time = []
lgamma_time = []


with open(CSV_FILE, newline="") as csvfile:
    reader = csv.DictReader(csvfile)

    for row in reader:
        n = int(row["N"])

        custom = float(row["custom_time"])
        reference = float(row["lgamma_time"])
        error = float(row["relative_error_percent"])

        # Keep all cases for accuracy analysis
        n_all.append(n)
        relative_error.append(error)

        # Only include valid non-zero timing measurements
        if custom > 0 and reference > 0:
            n_time.append(n)
            custom_time.append(custom)
            lgamma_time.append(reference)


# ============================================================
# Execution Time Comparison
# ============================================================

plt.figure(figsize=(8, 5))

plt.plot(
    n_time,
    custom_time,
    marker="o",
    label="Custom algorithm"
)

plt.plot(
    n_time,
    lgamma_time,
    marker="o",
    label="lgamma() reference"
)

plt.xscale("log")
plt.yscale("log")

plt.xlabel("N")
plt.ylabel("Average execution time (s)")
plt.title("Execution Time Comparison")

plt.grid(True, which="both", linestyle="--", alpha=0.5)
plt.legend()

plt.tight_layout()

plt.savefig(
    os.path.join(OUTPUT_DIR, "execution_time.png"),
    dpi=300
)

plt.close()


# ============================================================
# Relative Error
# ============================================================

plt.figure(figsize=(8, 5))

plt.plot(
    n_all,
    relative_error,
    marker="o"
)

plt.xscale("log")
plt.yscale("log")

plt.xlabel("N")
plt.ylabel("Relative error (%)")
plt.title("Relative Error of the Custom Factorial Approximation")

plt.grid(True, which="both", linestyle="--", alpha=0.5)

plt.tight_layout()

plt.savefig(
    os.path.join(OUTPUT_DIR, "relative_error.pdf"),
    dpi=300
)

plt.close()


print("Plots generated successfully:")
print(f"  {OUTPUT_DIR}/execution_time.pdf")
print(f"  {OUTPUT_DIR}/relative_error.pdf")