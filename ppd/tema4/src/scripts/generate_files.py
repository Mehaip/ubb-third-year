import random
import os
import sqlite3

# Get project root (2 levels up from this script)
script_dir = os.path.dirname(os.path.abspath(__file__))
project_root = os.path.dirname(os.path.dirname(script_dir))

# Ensure directories exist
input_dir = os.path.join(project_root, "files", "input")
output_dir = os.path.join(project_root, "files", "output")
os.makedirs(input_dir, exist_ok=True)
os.makedirs(output_dir, exist_ok=True)

# Connect to SQLite database
db_path = os.path.join(project_root, "students.db")
conn = sqlite3.connect(db_path)
cursor = conn.cursor()

# Create tables if they don't exist
for proj in range(1, 11):
    cursor.execute(f'''
        CREATE TABLE IF NOT EXISTS proiect{proj} (
            id INTEGER,
            grade INTEGER
        )
    ''')
    # Clear existing data in table
    cursor.execute(f'DELETE FROM proiect{proj}')

conn.commit()

for proj in range(1, 11):
    num_students = random.randint(80, 500)
    students = list(range(1, 501))  # All 500 IDs
    random.shuffle(students)
    selected = students[:num_students]

    filepath = os.path.join(input_dir, f"proiect{proj}.txt")
    with open(filepath, "w") as f:
        for student_id in selected:
            grade = random.randint(-1, 10)
            f.write(f"{student_id},{grade}\n")

            # Insert into database table
            cursor.execute(f'INSERT INTO proiect{proj} (id, grade) VALUES (?, ?)',
                         (student_id, grade))

    conn.commit()
    print(f"Generated proiect{proj}.txt and inserted {num_students} records into proiect{proj} table")

# Close database connection
conn.close()

print("\nGenerated 10 project files in files/input/")
print(f"Populated SQLite database at {db_path}")
