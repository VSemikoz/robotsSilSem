AVAILABLE_COMMANDS = {"quit": 0,
                      "rotate": 1,
                      "move_forward": 2}  # TODO


class CommandString:
    def __init__(self, command_list):
        self.command_name = command_list[0]
        self.parameters = command_list[1:]

    def get_command_bit_sequence(self):
        encode_command = str(AVAILABLE_COMMANDS[self.command_name]).encode()
        encode_params = '#'.encode().join([str(param).encode() for param in self.parameters])
        return encode_command + "/".encode() + encode_params #xample: comand_number/param1#param2#param3...

    def command_is_available(self):
        return True if self.command_name in AVAILABLE_COMMANDS.keys() else False
