import random
import os

# Get project root (2 levels up from this script)
script_dir = os.path.dirname(os.path.abspath(__file__))
project_root = os.path.dirname(os.path.dirname(script_dir))

# Ensure directories exist
input_dir = os.path.join(project_root, "files", "input")
output_dir = os.path.join(project_root, "files", "output")
os.makedirs(input_dir, exist_ok=True)
os.makedirs(output_dir, exist_ok=True)

for proj in range(1, 11):
    num_students = random.randint(80, 200)
    students = list(range(1, 201))  # All 200 IDs
    random.shuffle(students)
    selected = students[:num_students]

    filepath = os.path.join(input_dir, f"proiect{proj}.txt")
    with open(filepath, "w") as f:
        for student_id in selected:
            grade = random.randint(-1, 10)
            f.write(f"{student_id},{grade}\n")

print("Generated 10 project files in files/input/")
