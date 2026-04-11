from flask import Flask, request, send_from_directory
from flask_cors import CORS
import subprocess

app = Flask(__name__)
CORS(app)

@app.route('/')
def home():
    return send_from_directory('../frontend', 'index.html')

@app.route('/check', methods=['POST'])
def check():
    text = request.data.decode()

    with open("input.txt", "w") as f:
        f.write(text)

    result = subprocess.run(
        ["../backend/spellchecker", "input.txt"],
        capture_output=True,
        text=True
    )

    lines = result.stdout.strip().split("\n")
    data = []

    for line in lines:
        if "|" in line:
            parts = line.split("|")
            if len(parts) == 3:
                data.append({
                    "word": parts[0],
                    "type": parts[1],
                    "suggestion": parts[2]
                })

    return {"errors": data}

app.run(debug=True)