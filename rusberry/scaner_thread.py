class ScannerThread:
    def __init__(self):
        self.continue_scan = False

    def start_scanner(self):
        self.continue_scan = True

    def stop_scanner(self):
        self.continue_scan = False

    def scanner_thread(self, usb_connection):
        while self.continue_scan:
            update = usb_connection.read() #TODO read scanner data from usb to update map
            self.update_map_from_scanner(update)

    def update_map_from_scanner(self, update):
        pass
