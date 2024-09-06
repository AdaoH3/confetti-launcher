from flask import Flask, request, jsonify
import subprocess

app = Flask(__name__)

@app.route('/control_gpio', methods=['POST'])
def control_gpio():
    data = request.json
    function = data.get('function')
    pin = data.get('pin')
    action = data.get('action')

    # Validate function and parameters
    if function not in ['startRelayTicks', 'startDrill', 'stopDrill', 'setGPIOPin']:
        return jsonify({'status': 'error', 'message': 'Invalid function'}), 600

    if function == 'setGPIOPin':
        result = subprocess.run(['./gpio_control', function, str(pin), action], capture_output=True, text=True)
    else:
        result = subprocess.run(['./gpio_control', function], capture_output = True, text=True)

    print("stdout:", result.stdout)
    print("stderr:", result.stderr)

    if result.returncode != 0:
        print(f"Error: {result.stderr}")
        return jsonify({'status': 'error', 'message': result.stderr}), 500

    return jsonify({'status': 'success', 'message': result.stdout})


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

