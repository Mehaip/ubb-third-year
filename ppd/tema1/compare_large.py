#!/usr/bin/env python3
"""Compare sequential vs parallel outputs for 10000x10000 matrix."""

import sys

def read_matrix(filename):
    """Read matrix from output file."""
    try:
        with open(filename, 'r') as f:
            lines = f.readlines()
            n, m = map(int, lines[0].strip().split())
            matrix = []
            for i in range(1, n + 1):
                if i % 1000 == 0:
                    print(f"  Reading row {i}/{n}...", end='\r')
                row = list(map(int, lines[i].strip().split()))
                matrix.append(row)
            print(f"  Read {n} rows                    ")
            return n, m, matrix
    except FileNotFoundError:
        print(f"ERROR: File not found: {filename}")
        return None, None, None
    except Exception as e:
        print(f"ERROR reading {filename}: {e}")
        return None, None, None

def compare_matrices(file1, file2, name):
    """Compare two matrix files."""
    print(f"\n{'='*50}")
    print(f"Comparing {name}")
    print('='*50)
    
    print(f"Reading {file1}...")
    n1, m1, mat1 = read_matrix(file1)
    
    print(f"Reading {file2}...")
    n2, m2, mat2 = read_matrix(file2)
    
    if mat1 is None or mat2 is None:
        print("FAILED: Could not read one or both files")
        return False
    
    if n1 != n2 or m1 != m2:
        print(f"FAILED: Dimensions differ - ({n1}x{m1}) vs ({n2}x{m2})")
        return False
    
    print(f"Dimensions: {n1}x{m1}")
    print("Comparing values...")
    
    differences = 0
    first_diff = None
    
    for i in range(n1):
        if i % 1000 == 0:
            print(f"  Checking row {i}/{n1}...", end='\r')
        for j in range(m1):
            if mat1[i][j] != mat2[i][j]:
                differences += 1
                if first_diff is None:
                    first_diff = (i, j, mat1[i][j], mat2[i][j])
    
    print(f"  Checked {n1} rows                    ")
    
    if differences == 0:
        print("PASSED: Matrices are identical!")
        return True
    else:
        print(f"FAILED: {differences} differences found")
        print(f"First difference at [{first_diff[0]}][{first_diff[1]}]: {first_diff[2]} vs {first_diff[3]}")
        return False

def main():
    print("="*50)
    print("Output Comparison - Large Matrix (10000)")
    print("="*50)
    
    # Compare 10000x10000
    passed = compare_matrices(
        "results/cpp/sequential/output10000.txt",
        "results/cpp/parallel/output10000.txt",
        "10000x10000 Matrix"
    )
    
    # Summary
    print(f"\n{'='*50}")
    print("SUMMARY")
    print('='*50)
    print(f"  10000x10000: {'PASS' if passed else 'FAIL'}")
    print('='*50)
    
    return 0 if passed else 1

if __name__ == "__main__":
    sys.exit(main())
