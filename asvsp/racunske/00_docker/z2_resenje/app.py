from flask import Flask
import os
import socket

app = Flask(__name__)

@app.route("/")
def hello():

    html = "<h3>Pozdrav, {name}!</h3>" \
           "<b>Container ID:</b> {hostname}<br/>"
    return html.format(name=os.getenv("PERSON", "student"), hostname=socket.gethostname())

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=80)

