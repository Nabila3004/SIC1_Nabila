from flask import Flask, request, jsonify

app = Flask(_name_)

@app.route('/', methods=['POST'])
def receive_data():
    data = request.json
    if data:
        temperature = data.get('temperature')
        humidity = data.get('humidity')
        temp_category = data.get('tempCategory')
        hum_category = data.get('humCategory')
        print(f"Received data: Temperature={temperature} ({temp_category}), Humidity={humidity} ({hum_category})")
        response = {
            "status": "success",
            "data_received": {
                "temperature": temperature,
                "temp_category": temp_category,
                "humidity": humidity,
                "hum_category": hum_category
            }
        }
        return jsonify(response), 200
    else:
        response = {
            "status": "failure",
            "message": "No data received"
        }
        return jsonify(response), 400

if _name_ == '_main_':
    app.run(host='0.0.0.0', port=5000)