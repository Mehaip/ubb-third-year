#!/usr/bin/env python3
"""Compare sequential vs parallel outputs for 10x10 and 1000x1000 matrices."""

import sys

def read_matrix(filename):
    """Read matrix from output file."""
    try:
        with open(filename, 'r') as f:
            lines = f.readlines()
            n, m = map(int, lines[0].strip().split())
            matrix = []
            for i in range(1, n + 1):
                row = list(map(int, lines[i].strip().split()))
                matrix.append(row)
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
    
    n1, m1, mat1 = read_matrix(file1)
    n2, m2, mat2 = read_matrix(file2)
    
    if mat1 is None or mat2 is None:
        print("FAILED: Could not read one or both files")
        return False
    
    if n1 != n2 or m1 != m2:
        print(f"FAILED: Dimensions differ - ({n1}x{m1}) vs ({n2}x{m2})")
        return False
    
    print(f"Dimensions: {n1}x{m1}")
    
    differences = 0
    first_diff = None
    
    for i in range(n1):
        for j in range(m1):
            if mat1[i][j] != mat2[i][j]:
                differences += 1
                if first_diff is None:
                    first_diff = (i, j, mat1[i][j], mat2[i][j])
    
    if differences == 0:
        print("PASSED: Matrices are identical!")
        return True
    else:
        print(f"FAILED: {differences} differences found")
        print(f"First difference at [{first_diff[0]}][{first_diff[1]}]: {first_diff[2]} vs {first_diff[3]}")
        return False

def main():
    print("="*50)
    print("Output Comparison - Small Matrices (10, 1000)")
    print("="*50)
    
    results = []
    
    # Compare 10x10
    results.append(compare_matrices(
        "results/cpp/sequential/output10.txt",
        "results/cpp/parallel/output10.txt",
        "10x10 Matrix"
    ))
    
    # Compare 1000x1000
    results.append(compare_matrices(
        "results/cpp/sequential/output1000.txt",
        "results/cpp/parallel/output1000.txt",
        "1000x1000 Matrix"
    ))
    
    # Summary
    print(f"\n{'='*50}")
    print("SUMMARY")
    print('='*50)
    
    names = ["10x10", "1000x1000"]
    all_passed = True
    for name, passed in zip(names, results):
        status = "PASS" if passed else "FAIL"
        print(f"  {name}: {status}")
        if not passed:
            all_passed = False
    
    print('='*50)
    if all_passed:
        print("All tests PASSED!")
    else:
        print("Some tests FAILED!")
    
    return 0 if all_passed else 1

if __name__ == "__main__":
    sys.exit(main())
