#!/bin/bash

DB_PATH="students.db"

# Check if argument is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <table_number>"
    echo "Example: $0 1    (to view proiect1)"
    echo "Table number must be between 1 and 10"
    exit 1
fi

# Validate input is a number between 1 and 10
if ! [[ "$1" =~ ^[0-9]+$ ]] || [ "$1" -lt 1 ] || [ "$1" -gt 10 ]; then
    echo "Error: Please enter a valid table number (1-10)"
    exit 1
fi

TABLE_NAME="proiect$1"

echo "=========================================="
echo "  SQLite Database Contents Viewer"
echo "  Database: $DB_PATH"
echo "  Table: $TABLE_NAME"
echo "=========================================="
echo ""

# Check if database exists
if [ ! -f "$DB_PATH" ]; then
    echo "Error: Database file not found at $DB_PATH"
    exit 1
fi

# Get row count
ROW_COUNT=$(sqlite3 "$DB_PATH" "SELECT COUNT(*) FROM $TABLE_NAME;")
echo "Total records: $ROW_COUNT"
echo ""

# Display table contents with headers
sqlite3 -column -header "$DB_PATH" "SELECT * FROM $TABLE_NAME;"
echo ""

# Show statistics for this table
CHEATER_COUNT=$(sqlite3 "$DB_PATH" "SELECT COUNT(*) FROM $TABLE_NAME WHERE grade = -1;")
echo "=========================================="
echo "Cheaters (grade=-1) in $TABLE_NAME: $CHEATER_COUNT"
echo "=========================================="
