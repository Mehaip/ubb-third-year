python3 src/scripts/generate_files.py
g++ -std=c++20 src/*.cpp src/utils/*.cpp -I src -I src/data_structures -I src/utils -o build/a.out -lsqlite3 -pthread
./build/a.out
python3 src/scripts/compare_results.py