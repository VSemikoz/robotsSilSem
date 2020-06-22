import map_storage
import serial
import time
from command_string import CommandString as CS

UTF8 = "utf-8"

class BotControl:
    def __init__(self):
        self.map = map_storage
        self.usb_connection = serial.Serial('/dev/ttyUSB0', 9600)
        self.scanner_thread = None  # TODO thread to get data from scanner, compute them and map update
        self.cont = True

    def initialize(self):
        self.usb_connection.open()

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
