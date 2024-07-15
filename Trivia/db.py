import sqlite3
from requests import get
import html

# Connect to SQLite database
conn = sqlite3.connect('TriviaDB.db')
cursor = conn.cursor()

# Create table if not exists
cursor.execute("CREATE TABLE IF NOT EXISTS Questions (id INTEGER PRIMARY KEY, question TEXT, correct TEXT, wrong1 TEXT, wrong2 TEXT, wrong3 TEXT);")

# Clear the table
cursor.execute("DELETE FROM Questions")

data = get("https://opentdb.com/api.php?amount=10&difficulty=easy&type=multiple").json()

for ID, question_data in enumerate(data['results'], start=1):
    # Properly handle special characters in SQL query
    question = html.unescape(question_data['question'])
    correct_answer = html.unescape(question_data['correct_answer'])
    wrong_answers = [html.unescape(answer) for answer in question_data['incorrect_answers']]
    command = "INSERT INTO Questions (id, question, correct, wrong1, wrong2, wrong3) VALUES (?, ?, ?, ?, ?, ?);"
    cursor.execute(command, (ID, question, correct_answer, *wrong_answers))

# Commit changes to the database
conn.commit()

conn.close()  # Close connection when done
