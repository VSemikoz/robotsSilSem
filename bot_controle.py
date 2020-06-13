import map_storage
from command_string import CommandString as CS


class BotControl:
    def __init__(self):
        self.map = map_storage
        self.usb_connection = None  # TODO
        self.scanner_thread = None  # TODO thread to get data from scanner, compute them and map update
        self.cont = True

    def start(self):
        while self.cont:
            command = self.get_command()
            self.perform_command(command)

    def perform_command(self, command):
        print command
        sequence_of_bits = command.get_command_bit_sequence()
        if command.command_name == "rotate":
            self.perform_rotate(sequence_of_bits)
        if command.command_name == "move_forward":
            self.perform_moving_forward(sequence_of_bits)
        if command.command_name == "quit":
            self.cont = False

    def perform_rotate(self, sequence_of_bits):
        angle = sequence_of_bits[1]
        print("performing rotation on %s degrees" % angle)

        # TODO usb_connection.send_command(sequence_of_bits)

    def perform_moving_forward(self, sequence_of_bits):
        distance = sequence_of_bits[1]
        print("performing moving on %s" % distance)
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
