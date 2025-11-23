python3 generate_files.py
g++ -std=c++20 src/*.cpp -I src -o build/a.out -pthread
./build/a.out
python3 compare_results.py