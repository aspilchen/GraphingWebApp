from flask import Flask, render_template, jsonify, request
import json
import math

from algorithms import process

app = Flask(__name__)

def processGraph(jGraph, algo):
    algorithms.process(jGraph, algo)

@app.route('/')
def index():
    return render_template("index.html")

@app.route('/about')
def about():
    return render_template("about.html")

@app.route('/graph')
def graph():
    return render_template("datagraph.html")

@app.route('/calculate', methods=['GET'])
def calculate():
    algorithm = request.args.get("algo")
    inGraph = json.loads(request.args.get("graph"))
    outGraph = process(inGraph, algorithm)
    return jsonify(outGraph)

if __name__ == "__main__":
    app.run(debug='true', host='0.0.0.0', port=80)
