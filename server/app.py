from flask import Flask, request, jsonify
from flask_cors import CORS
import subprocess
import os

app = Flask(__name__)
CORS(app)

from flask import send_from_directory

@app.route('/')
def home():
    return send_from_directory('../frontend', 'index.html')

@app.route('/check', methods=['POST'])
def check():

    if 'file' in request.files:
        file = request.files['file']
        filepath = "input.txt"
        file.save(filepath)
    else:
        text = request.data.decode()
        filepath = "input.txt"
        with open(filepath, "w") as f:
            f.write(text)

    result = subprocess.run(
        ["../backend/spellchecker", filepath],
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

    return jsonify({"errors": data})

if __name__ == "__main__":
    app.run(debug=True)