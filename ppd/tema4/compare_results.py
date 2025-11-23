
def parse_file(filename):
    results = {}
    with open(filename, "r") as f:
        for line in f:
            line = line.strip()
            if line:
                parts = line.split(",")
                student_id = int(parts[0])
                grade = int(parts[1])
                results[student_id] = grade
    return results

seq_results = parse_file("files/rezultate.txt")
par_results = parse_file("files/rezultate_p.txt")

# Compare
if len(seq_results) != len(par_results):
    print(f"Different number of students: sequential={len(seq_results)}, parallel={len(par_results)}")
else:
    print(f"Both have {len(seq_results)} students")

all_match = True
for student_id, grade in seq_results.items():
    if student_id not in par_results:
        print(f"Student {student_id} missing in parallel results")
        all_match = False
    elif par_results[student_id] != grade:
        print(f"Student {student_id}: sequential={grade}, parallel={par_results[student_id]}")
        all_match = False

for student_id in par_results:
    if student_id not in seq_results:
        print(f"Student {student_id} extra in parallel results")
        all_match = False

if all_match:
    print("CORECT!")
else:
    print("Results do NOT match!")
