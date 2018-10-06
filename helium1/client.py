import socket
import struct
import time
import random
import sys

class ClientError(Exception):
    def __init__(self, message, errors):
        super(ValidationError, self).__init__(message)
        self.errors = errors


class Client:
    def __init__(self):
        self.socket = None
        self.ip = None
        self.port = None

    def run(self, ip = "127.0.0.1", port = 5555):
        self.ip = ip
        self.port = port
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error as e:
            raise ClientError(str(e))

    def request(self, data):
        self.socket.connect( (self.ip, self.port) )
        bytes_sent = self.socket.send(struct.pack("i", data[0]))
        for id in range(1, len(data)):
            bytes_sent = self.socket.send(struct.pack("i", data[id]))
        response = self.socket.recv(4)
        response = struct.unpack('i', response)[0]
        print('Server responded with: ' + response.__format__('d'))
        self.socket.close()
