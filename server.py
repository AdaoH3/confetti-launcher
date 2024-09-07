from flask import Flask, request, jsonify
import subprocess

app = Flask(__name__)

@app.route('/trigger_relay', methods=['POST'])
def trigger_relay():
    # Get the seconds parameter from the POST REQUEST
    data = request.json
    print(f"Raw request data: {data}")
    seconds = data.get('seconds')

    str_seconds = str(seconds)
    print(f"Recieved seconds: {seconds}")

    try:
        result = subprocess.run(['./triggerRelayTime', str_seconds], capture_output=True, text=True) 
        return f"Triggered relay for {seconds} seconds.\nOutput: {result.stdout}", 200
    except subprocess.CalledProcessError as e:
       return f"Error triggering relay: {e.stderr}", 500


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

