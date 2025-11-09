#!/usr/bin/env python3

import csv
from pathlib import Path
from collections import defaultdict
from statistics import mean

# Read the CSV file (get the most recent one)
results_dir = Path("benchmark_results")
csv_files = sorted(results_dir.glob("benchmark_*.csv"), key=lambda x: x.stat().st_mtime, reverse=True)

if not csv_files:
    print("No benchmark CSV files found!")
    exit(1)

csv_file = csv_files[0]
print(f"Processing: {csv_file}")

# Read data
# Structure: data[program][file][processes] = [times]
data = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))

with open(csv_file, 'r') as f:
    reader = csv.DictReader(f)
    for row in reader:
        program = row['Program']
        file_num = int(row['File'])
        processes = int(row['Processes'])
        time_val = float(row['Time(s)'])
        data[program][file_num][processes].append(time_val)

# Calculate statistics
stats = {}
for program in data:
    stats[program] = {}
    for file_num in data[program]:
        stats[program][file_num] = {}
        for processes in data[program][file_num]:
            times = data[program][file_num][processes]
            stats[program][file_num][processes] = {
                'mean': mean(times),
                'min': min(times),
                'max': max(times),
                'count': len(times)
            }

# Create markdown report
md_lines = []
md_lines.append("# MPI Big Number Addition - Benchmark Results\n")
md_lines.append(f"**Generated from:** `{csv_file.name}`")
md_lines.append(f"**Number of runs per configuration:** 10\n")
md_lines.append("---\n")

# File size mapping
file_sizes = {
    1: "~19 digits",
    2: "~10K digits",
    3: "~100K digits",
    4: "~10M digits"
}

# Process each file separately
all_files = sorted(set(file_num for program in data for file_num in data[program]))

for file_num in all_files:
    file_size = file_sizes.get(file_num, "Unknown")
    md_lines.append(f"## File {file_num}.txt ({file_size})\n")

    # Get sequential baseline
    baseline_time = None
    if 'sequential' in stats and file_num in stats['sequential']:
        seq_stat = stats['sequential'][file_num][1]
        baseline_time = seq_stat['mean']

        md_lines.append("### Sequential Baseline\n")
        md_lines.append(f"- **Average time:** {seq_stat['mean']:.6f}s")
        md_lines.append(f"- **Range:** {seq_stat['min']:.6f}s - {seq_stat['max']:.6f}s\n")
    else:
        md_lines.append("### Sequential Baseline\n")
        md_lines.append("No sequential data available\n")

    # Collect MPI results for this file
    mpi_results = []
    for program in sorted(stats.keys()):
        if program != 'sequential' and file_num in stats[program]:
            for processes in sorted(stats[program][file_num].keys()):
                result = stats[program][file_num][processes]
                mpi_results.append({
                    'program': program,
                    'processes': processes,
                    'mean': result['mean'],
                    'min': result['min'],
                    'max': result['max']
                })

    if mpi_results:
        md_lines.append("### MPI Implementations\n")
        md_lines.append("| Program | Processes | Mean Time (s) | Speedup vs Sequential | Efficiency |")
        md_lines.append("|---------|-----------|---------------|-----------------------|------------|")

        for result in mpi_results:
            prog = result['program'].replace('_', '\\_')
            procs = result['processes']
            mean_time = result['mean']

            if baseline_time:
                speedup = baseline_time / mean_time
                efficiency = (speedup / procs) * 100
                md_lines.append(
                    f"| {prog} | {procs} | {mean_time:.6f} | {speedup:.3f}x | {efficiency:.1f}% |"
                )
            else:
                md_lines.append(
                    f"| {prog} | {procs} | {mean_time:.6f} | N/A | N/A |"
                )
        md_lines.append("")

# Summary section
md_lines.append("---\n")
md_lines.append("## Summary\n")
md_lines.append("### Best Performing Configurations\n")

for file_num in all_files:
    # Get sequential time
    baseline_time = None
    if 'sequential' in stats and file_num in stats['sequential']:
        baseline_time = stats['sequential'][file_num][1]['mean']

    if baseline_time:
        # Find best MPI configuration
        best = None
        for program in stats:
            if program != 'sequential' and file_num in stats[program]:
                for processes in stats[program][file_num]:
                    result = stats[program][file_num][processes]
                    if best is None or result['mean'] < best['mean']:
                        best = {
                            'program': program,
                            'processes': processes,
                            'mean': result['mean']
                        }

        if best:
            speedup = baseline_time / best['mean']
            efficiency = (speedup / best['processes']) * 100
            file_size = file_sizes.get(file_num, "Unknown")

            md_lines.append(f"\n**File {file_num}.txt ({file_size}):**")
            md_lines.append(f"- Sequential: {baseline_time:.6f}s")
            md_lines.append(f"- Best MPI: `{best['program']}` with {best['processes']} processes")
            md_lines.append(f"  - Time: {best['mean']:.6f}s")
            md_lines.append(f"  - Speedup: {speedup:.3f}x")
            md_lines.append(f"  - Efficiency: {efficiency:.1f}%")

# Analysis notes
md_lines.append("\n---\n")
md_lines.append("## Analysis Notes\n")
md_lines.append("- **Speedup** is calculated as: `Sequential Time / Parallel Time`")
md_lines.append("- **Efficiency** is calculated as: `(Speedup / Number of Processes) × 100%`")
md_lines.append("- Efficiency > 100% may indicate the parallel version is unexpectedly faster (measurement variance or system effects)")
md_lines.append("- Each configuration is averaged over 10 runs")
md_lines.append("- For small problem sizes, MPI overhead dominates and sequential is faster")
md_lines.append("- For larger problem sizes (10M+ digits), parallelization should show benefits\n")

# Write to file
output_file = results_dir / "BENCHMARK_REPORT.md"
with open(output_file, 'w') as f:
    f.write('\n'.join(md_lines))

print(f"✓ Report generated: {output_file}")
