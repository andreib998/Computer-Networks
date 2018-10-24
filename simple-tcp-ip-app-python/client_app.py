import socket
import sys
from client import Client, ClientError

host = "127.0.0.1"
port = 5555

if __name__ == "__main__":
    if len(sys.argv) > 2:
        print(sys.argv[0] + " error: client.py [port]")
    elif len(sys.argv) == 1:
        port = 5555;
    else:
        try:
            port = int(sys.argv[1]);
        except ValueError as exc:
            print('Invalid port number: ', str(e))
            sys.exit()

    try:
        client = Client()
        client.run()
        client.request([4, 10, 90, 3, 1])
    except ClientError as e:
        print(e)
