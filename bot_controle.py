import map_storage
import serial   # library for port
import time   #library for time delays
from command_string import CommandString as CS


class BotControl:
    def __init__(self):
        self.map = map_storage
        self.usb_connection = serial.Serial('/dev/ttyUSB0', 9600)  # port for rasspberry pi, on windows can not work
        self.scanner_thread = None  # TODO thread to get data from scanner, compute them and map update
        self.cont = True

    def start(self):
        while self.cont:
            usb = self.usb_connection
            command = self.get_command()
            self.perform_command(command)
            usb.open  # open usb connection(port)

    def perform_command(self, command, usb):
        print command
        sequence_of_bits = command.get_command_bit_sequence()
        if command.command_name == "rotate":
            x = "2"   # the number by which Arduino understands that she will need to turn
            usb.write(x.encode())   # sending command numbers to arduino
            time.sleep(1)  # 1 second pause for correct operation
            self.perform_rotate(sequence_of_bits)
        if command.command_name == "move_forward":
            x = "3"  # the number by which Arduino understands that she will need to go right before the obstacle
            usb.write(x.encode())  # sending command numbers to arduino
            self.perform_moving_forward(sequence_of_bits)
        if command.command_name == "quit":
            self.cont = False

    def perform_rotate(self, sequence_of_bits, usb):
        angle = sequence_of_bits[1]
        usb.write(angle.encode())
        print("performing rotation on %s degrees" % angle)


        # TODO usb_connection.send_command(sequence_of_bits)

    def perform_moving_forward(self, sequence_of_bits, usb):
        distance = sequence_of_bits[1]
        print("performing moving on %s" % distance)
        while True:
            data = usb.readline()
            print(data)
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
