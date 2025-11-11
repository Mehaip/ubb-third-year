# MPI Big Number Addition - Benchmark Results

**Generated from:** `benchmark_20251110_002502.csv`
**Number of runs per configuration:** 10

---

## File 1.txt (16 digits)

### Sequential Baseline

- **Average time:** 0.007383s
- **Range:** 0.004007s - 0.011503s

### MPI Implementations

| Program | Processes | Mean Time (s) | Speedup vs Sequential | Efficiency |
|---------|-----------|---------------|-----------------------|------------|
| scatter | 5 | 0.036237 | 0.204x | 4.1% |
| standard | 5 | 0.037866 | 0.195x | 3.9% |

## File 2.txt (10k digits)

### Sequential Baseline

- **Average time:** 0.011213s
- **Range:** 0.006785s - 0.015416s

### MPI Implementations

| Program | Processes | Mean Time (s) | Speedup vs Sequential | Efficiency |
|---------|-----------|---------------|-----------------------|------------|
| scatter | 5 | 0.042221 | 0.266x | 5.3% |
| scatter | 9 | 0.038498 | 0.291x | 3.2% |
| scatter | 17 | 0.043461 | 0.258x | 1.5% |
| standard | 5 | 0.037374 | 0.300x | 6.0% |
| standard | 9 | 0.036754 | 0.305x | 3.4% |
| standard | 17 | 0.039236 | 0.286x | 1.7% |

## File 3.txt (100K digits)

### Sequential Baseline

- **Average time:** 0.029489s
- **Range:** 0.025222s - 0.032094s

### MPI Implementations

| Program | Processes | Mean Time (s) | Speedup vs Sequential | Efficiency |
|---------|-----------|---------------|-----------------------|------------|
| scatter | 5 | 0.038990 | 0.756x | 15.1% |
| scatter | 9 | 0.040759 | 0.723x | 8.0% |
| scatter | 17 | 0.037754 | 0.781x | 4.6% |
| standard | 5 | 0.035982 | 0.820x | 16.4% |
| standard | 9 | 0.035024 | 0.842x | 9.4% |
| standard | 17 | 0.040693 | 0.725x | 4.3% |

## File 4.txt (10M digits)

### Sequential Baseline

- **Average time:** 1.749377s
- **Range:** 1.667268s - 1.838099s

### MPI Implementations

| Program | Processes | Mean Time (s) | Speedup vs Sequential | Efficiency |
|---------|-----------|---------------|-----------------------|------------|
| scatter | 5 | 0.048171 | 36.316x | 726.3% |
| scatter | 9 | 0.039524 | 44.261x | 491.8% |
| scatter | 17 | 0.036846 | 47.478x | 279.3% |
| scatter | 33 | 0.039272 | 44.545x | 135.0% |
| standard | 5 | 0.039466 | 44.326x | 886.5% |
| standard | 9 | 0.040797 | 42.880x | 476.4% |
| standard | 17 | 0.049727 | 35.180x | 206.9% |
| standard | 33 | 0.044375 | 39.422x | 119.5% |

---

## Summary

### Best Performing Configurations


**File 1.txt (16 digits):**
- Sequential: 0.007383s
- Best MPI: `scatter` with 5 processes
  - Time: 0.036237s
  - Speedup: 0.204x
  - Efficiency: 4.1%

**File 2.txt (10k digits):**
- Sequential: 0.011213s
- Best MPI: `standard` with 9 processes
  - Time: 0.036754s
  - Speedup: 0.305x

**File 3.txt (100K digits):**
- Sequential: 0.029489s
- Best MPI: `standard` with 9 processes
  - Time: 0.035024s
  - Speedup: 0.842x

**File 4.txt (10M digits):**
- Sequential: 1.749377s
- Best MPI: `scatter` with 17 processes
  - Time: 0.036846s
  - Speedup: 47.478x

---

## Analysis Notes

- **Speedup** = `Sequential Time / Parallel Time`
- **Efficiency** = `(Speedup / Number of Processes) × 100%`

ALTE OBSERVATII:
PENTRU CELE CARE NU AU 10MIL
17 PROCESE CEL MAI INEFICIENT
5 PROCESE CEL MAI EFICIENT
PENTRU 10MIL
17 PROCESE CEL MAI RAPID