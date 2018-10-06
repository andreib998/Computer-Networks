import sys
import struct
# import time
# import random
from multiprocessing import Process

class Server:
    def __init__(self, socket):
        self.socket = socket

    def client_handler(self, conn, address):
        result = {'sum': 0, 'values': []}
        amount = conn.recv(4)
        amount = struct.unpack('i', amount)[0]
        for _ in range(0, amount):
            data = conn.recv(4)
            unpacked_data = int(struct.unpack('i', data)[0])
            result['values'].append(unpacked_data)
            result['sum'] += unpacked_data

        conn.send(struct.pack('i', result['sum']))

        # delay = random.randint(1,10)
        # time.sleep(delay)
        # print('[+] Client disconected: {0} [computed in {1} sec]'.format(address[0], delay))
        print('[+] Client disconected: {0}'.format(address[0]))
        conn.close()

    def run(self):
        while True:
            conn, address = self.socket.accept()
            print('[+] Client connected: {0}'.format(address[0]))
            prc = Process(target = self.client_handler, args = (conn, address ))
            prc.start()
