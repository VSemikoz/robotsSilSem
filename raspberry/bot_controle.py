import map_storage
import serial
import threading
import time
from raspberry.scaner_thread import ScannerThread
from command_string import CommandString as CS

UTF8 = "utf-8"


class BotControl:
    def __init__(self):
        self.map = map_storage
        self.usb_connection = serial.Serial('/dev/ttyUSB0', 9600)
        self.scanner = ScannerThread()
        self.cont = True

    def initialize(self):
        self.usb_connection.open()
        self.scanner.start_scanner()
        scanner_thread = threading.Thread(target=self.scanner.scanner_thread, args=(self.usb_connection,))
        scanner_thread.start()
        scanner_thread.join()

    def start(self):
        self.initialize()
        while self.cont:
            command = self.get_command()
            self.perform_command(command)

    def perform_command(self, command):
        sequence_of_bits = command.get_command_bit_sequence()
        if command.command_name == "rotate" and len(command.parameters) == 1:
            print("performing rotation on %s degrees" % command.parameters[0])
            self.perform_rotate(sequence_of_bits)
        if command.command_name == "move_forward" and len(command.parameters) == 1:
            print("performing moving on %s" % command.parameters[0])
            self.perform_moving_forward(sequence_of_bits)
        if command.command_name == "quit":
            self.scanner.start_scanner()
            self.cont = False

    def perform_rotate(self, sequence_of_bits):
        self.usb_connection.write(sequence_of_bits.encode())# example_sequence: 1/90
        time.sleep(.1)
        # TODO usb_connection.send_command(sequence_of_bits)

    def perform_moving_forward(self, sequence_of_bits):
        self.usb_connection.write(sequence_of_bits.encode()) #example_sequence: 2/20
        time.sleep(.1)
        # TODO usb_connection.send_command(sequence_of_bits)

    @staticmethod
    def get_command():
        while True:
            command_list = str(raw_input("Input command: ")).split(" ")  # example: rotate 90
            cs = CS(command_list)
            if cs.command_is_available():
                break
            else:
                print("invalid input")
        return cs
