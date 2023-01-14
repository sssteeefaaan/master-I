from os.path import join
from os import curdir
from time import sleep
from socket import socket
from os import environ

ADDRESS = environ.get("TCP_ADDRESS", "localhost")
PORT = int(environ.get("TCP_PORT", "50000"))

def main():
    filepath = join(join(curdir, "archive"), "network-traffic.csv")
    file = open(filepath)
    print(f"Opened socket tcp://{ADDRESS}:{PORT}")
    _headers = file.readline()
    for line in file:
        s = socket()
        s.connect((ADDRESS, PORT))
        s.send(line.strip().encode())
        s.close()
        sleep(1)
    file.close()
    print("Done!")

if __name__ == "__main__":
    main()