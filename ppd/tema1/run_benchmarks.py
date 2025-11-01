#!/usr/bin/env python3

import subprocess
import sys

def run_script_and_save(script_path, output_file):
    """Run a shell script and save all output to a file."""
    print(f"Running {script_path}...")
    try:
        # Run the script and capture both stdout and stderr
        result = subprocess.run(
            ['bash', script_path],
            capture_output=True,
            text=True,
            check=False  # Don't raise exception on non-zero exit
        )

        # Combine stdout and stderr
        full_output = result.stdout + result.stderr

        # Write to output file
        with open(output_file, 'w') as f:
            f.write(full_output)

        print(f"Output saved to {output_file}")
        print(f"Exit code: {result.returncode}")

        return result.returncode

    except Exception as e:
        print(f"Error running {script_path}: {e}")
        return 1

def main():
    print("Starting benchmark runs...\n")

    # Run Java benchmark
    java_exit_code = run_script_and_save('./runjava.sh', 'javaOutputData.txt')
    print()

    # Run C++ benchmark
    cpp_exit_code = run_script_and_save('./runcpp.sh', 'cppOutputData.txt')
    print()


    print("All benchmarks completed!")
    print(f"C++ exit code: {cpp_exit_code}")
    print(f"Java exit code: {java_exit_code}")

    # Exit with error if either script failed
    if cpp_exit_code != 0 or java_exit_code != 0:
        sys.exit(1)

if __name__ == "__main__":
    main()
