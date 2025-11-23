import random

for proj in range(1, 11):
    num_students = random.randint(80, 200)
    students = list(range(1, 201))  # All 200 IDs
    random.shuffle(students)
    selected = students[:num_students]

    with open(f"files/proiect{proj}.txt", "w") as f:
        for student_id in selected:
            grade = random.randint(1, 10)
            f.write(f"{student_id},{grade}\n")

print("Generated 10 project files in files/")