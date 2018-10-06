import socket
from server import Server

host = ''
port = 5555

if __name__ == "__main__":
    while True:
        try:
            ep = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            ep.bind((host, port))
            ep.listen(1)
        except socket.error as exc:
            if exc.args[0] != 48:
                raise
            print('Port', port, 'already in use')
            port += 1
        else:
            break

    print('[+] Listening for connections on port: {0}'.format(port))
    server = Server(ep)
    server.run()
