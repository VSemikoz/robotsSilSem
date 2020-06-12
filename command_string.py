AVAILABLE_COMMANDS = {"rotate": 0,
                      "move_forward": 1}  # TODO


class CommandString:
    def __init__(self, command_list):
        self.command_name = command_list[0]
        self.parameters = command_list[1:]

    def get_command_bit_sequence(self):
        return [AVAILABLE_COMMANDS[self.command_name], self.parameters]

    def command_is_available(self):
        return True if self.command_name in AVAILABLE_COMMANDS.keys() else False
