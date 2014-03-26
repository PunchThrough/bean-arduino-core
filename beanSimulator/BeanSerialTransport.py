import os
import serial # requires pip install
from serial.tools import list_ports 
import logging
import sys
from enum import Enum  # requires pip install


class Bean_Serial_Transport:
    SOF_BYTE = 0x7E
    EOF_BYTE = 0x7F
    ESC_BYTE = 0x7D
    ESC_SOF_BYTE = 0x5E
    ESC_EOF_BYTE = 0x5F
    ESC_ESC_BYTE = 0x5D

    MSG_ID_SERIAL_DATA        = 0x00, 0x00
    MSG_ID_BT_SET_ADV         = 0x05, 0x00
    MSG_ID_BT_SET_CONN        = 0x05, 0x02
    MSG_ID_BT_SET_LOCAL_NAME  = 0x05, 0x04
    MSG_ID_BT_SET_PIN         = 0x05, 0x06
    MSG_ID_BT_SET_TX_PWR      = 0x05, 0x08
    MSG_ID_BT_GET_CONFIG      = 0x05, 0x10
    MSG_ID_BT_ADV_ONOFF       = 0x05, 0x12
    MSG_ID_BT_SET_SCRATCH     = 0x05, 0x14
    MSG_ID_BT_GET_SCRATCH     = 0x05, 0x15
    MSG_ID_BT_RESTART         = 0x05, 0x20
    MSG_ID_BL_CMD             = 0x10, 0x00
    MSG_ID_BL_FW_BLOCK        = 0x10, 0x01
    MSG_ID_BL_STATUS          = 0x10, 0x02
    MSG_ID_CC_LED_WRITE       = 0x20, 0x00
    MSG_ID_CC_LED_WRITE_ALL   = 0x20, 0x01
    MSG_ID_CC_LED_READ_ALL    = 0x20, 0x02
    MSG_ID_CC_ACCEL_READ      = 0x20, 0x10
    MSG_ID_AR_SET_POWER_INT   = 0x30, 0x00
    MSG_ID_AR_GET_CONFIG      = 0x30, 0x06
    MSG_ID_AR_POWER_OFFON     = 0x30, 0x10
    MSG_ID_DB_LOOPBACK        = 0xFE, 0x00
    MSG_ID_DB_COUNTER         = 0xFE, 0x01

    ParserStates = Enum("WAITING_FOR_SOF",
                        "GETTING_LENGTH",
                        "GETTING_MAJOR_TYPE",
                        "GETTING_MINOR_TYPE",
                        "GETTING_MESSAGE_BODY",
                        "GETTING_EOF")



    def __init__(self):
        self.serial_port = None
        self.parser_init_done = False
        self.callbacks_serial = []
        self.reset_parser()
        self.message_handlers = {}

    def add_serial_callback(self, function):
        self.callbacks_serial.append(function)


    def reset_parser(self):
        self.parser_state = self.ParserStates.WAITING_FOR_SOF
        self.parser_length = 0
        self.parser_message_buffer = []
        self.parser_message_type = []
        self.parser_init_done = True
        self.parser_escaping = False


    def parser(self):        
        if(self.serial_port == None or self.serial_port.isOpen() == False):
            return

        while(self.serial_port.inWaiting() > 0):
            byte = map(ord, self.serial_port.read(1))[0]

            #logging.error(byte)

            if(self.parser_escaping == False and byte == self.ESC_BYTE):
                # should anything else have escaping?
                if(self.parser_state == self.ParserStates.GETTING_MESSAGE_BODY):
                    self.parser_escaping = True
                    continue

            # reset the parser and handle the new message
            if(self.parser_escaping == False and byte == self.SOF_BYTE):
                self.reset_parser()

            if(self.parser_escaping == True):
                self.parser_escaping = False
                byte = byte | 0x20

            if(self.parser_state == self.ParserStates.WAITING_FOR_SOF):
                if(byte == self.SOF_BYTE):
                    self.parser_state = self.ParserStates.GETTING_LENGTH
#                    logging.debug("SOF --> LEN")
            
            elif(self.parser_state == self.ParserStates.GETTING_LENGTH):
                self.parser_length = byte
                self.parser_state = self.ParserStates.GETTING_MAJOR_TYPE
#                logging.debug("LEN --> MAJ")

            elif(self.parser_state == self.ParserStates.GETTING_MAJOR_TYPE):
                self.parser_message_type = []
                self.parser_length -= 1
                self.parser_message_type.append(byte)
                self.parser_state = self.ParserStates.GETTING_MINOR_TYPE
#                logging.debug("MAJ --> MIN")

            elif(self.parser_state == self.ParserStates.GETTING_MINOR_TYPE):
                self.parser_length -= 1
                self.parser_message_type.append(byte)
                self.parser_state = self.ParserStates.GETTING_MESSAGE_BODY
#                logging.debug("MIN --> BODY")

            elif(self.parser_state == self.ParserStates.GETTING_MESSAGE_BODY):

                if(self.parser_message_type[0] == self.MSG_ID_SERIAL_DATA[0] and
                   self.parser_message_type[1] == self.MSG_ID_SERIAL_DATA[1]):
                    for callback in self.callbacks_serial:
                        callback(chr(byte))
                else:
                    self.parser_message_buffer.append(byte)

                self.parser_length -= 1
                if(self.parser_length == 0):
                    self.parser_state = self.ParserStates.GETTING_EOF
#                    logging.debug("BODY --> EOF")

            elif(self.parser_state == self.ParserStates.GETTING_EOF):
                if(byte != self.EOF_BYTE):
                    
                    continue
                if(self.parser_message_type[0] != self.MSG_ID_SERIAL_DATA[0] or
                   self.parser_message_type[1] != self.MSG_ID_SERIAL_DATA[1]):
                    self.handle_message(tuple(self.parser_message_type), self.parser_message_buffer)

                self.reset_parser()




    def serial_ports(self):
        """
        Returns a generator for all available serial ports
        """
        if os.name == 'nt':
            # windows
            for i in range(256):
                try:
                    s = serial.Serial(i)
                    s.close()
                    yield 'COM' + str(i + 1)
                except serial.SerialException:
                    pass
        else:
            # unix
            for port in list_ports.comports():
                yield port[0]

    def get_available_serial_ports(self):
      ports = list(self.serial_ports())
      logging.info("All system serial ports: ")
      logging.info(ports)
      return ports

    def open_port(self, port, baudrate):
        if(self.serial_port != None):
            self.serial_port.close
        self.reset_parser()
        self.serial_port = serial.Serial(port, baudrate, timeout=0)
        self.serial_port.flushInput()
        logging.info("transport opened serial port: %s to baud: %d", port, baudrate  )

    def close_port(self):
        if(self.serial_port != None):
            self.serial_port.close
            self.serial_port = None
            logging.info("transport closed port")

    def log_port(self):
        if(self.serial_port != None):
            logging.info("serial port: " + self.serial_port.portstr)
        else:
            logging.info("No serial port needed to close.");

    def escape_buffer(self, buffer):
        escaped = []
        for i in buffer:
            if i == self.SOF_BYTE:
                escaped.extend([self.ESC_BYTE, self.ESC_SOF_BYTE])
            elif i == self.EOF_BYTE:
                escaped.extend([self.ESC_BYTE, self.ESC_EOF_BYTE])
            elif i == self.ESC_BYTE:
                escaped.extend([self.ESC_BYTE, self.ESC_ESC_BYTE])
            else:
                escaped.append(i)
        return escaped

    def build_message(self, message_type, buffer):
        message = [self.SOF_BYTE]
        message_body = []
        message_body.extend(message_type)
        message_body.extend(buffer)
        message.append(len(message_body))
        message.extend(self.escape_buffer(message_body))
        message.append(self.EOF_BYTE)
        return message

    def send_message(self, message_type, buffer):
        message = self.build_message(message_type, buffer)
        self.serial_port.write(message)

    def add_handler(self, message, handler):
        try:
            self.message_handlers[message]
        except KeyError:
            self.message_handlers[message] = []
        
        logging.debug("Adding handler to message type:" + str(message) + ", " + str(handler))
        self.message_handlers[message].append(handler)

    def handle_message(self,message_type, message_body):    
        handlers = []

        if(not isinstance(message_type, tuple)):
            message_type = tuple(message_type)

        try:
            handlers = self.message_handlers[message_type]
        except KeyError:
            return

        for handler in handlers:
            handler(message_body)

    def remove_handler(self, message, handler):
        handlers = []
        try:
            handlers = self.message_handlers[message]
        except KeyError:
            return

        handlers.remove(handler)


if __name__ == '__main__':
    #Simple testing down here
    logging.basicConfig(stream=sys.stderr, level=logging.DEBUG)

    transport = Bean_Serial_Transport()
    port = transport.get_available_serial_ports()[0]
    transport.open_port(port, 57600)
    transport.log_port()
    transport.close_port()

    logging.debug(transport.escape_buffer([1,2,3,4,5,6]))
    logging.debug(transport.escape_buffer([Bean_Serial_Transport.SOF_BYTE,
                                          2,3,4,5,
                                          Bean_Serial_Transport.EOF_BYTE]))
    
    logging.debug(transport.escape_buffer([Bean_Serial_Transport.SOF_BYTE,
                                          Bean_Serial_Transport.ESC_BYTE,
                                          Bean_Serial_Transport.ESC_EOF_BYTE,
                                          Bean_Serial_Transport.EOF_BYTE]))

    logging.debug(transport.build_message(transport.MSG_ID_SERIAL_DATA, [1,2]))
    logging.debug(transport.build_message(transport.MSG_ID_SERIAL_DATA, 'hello'))

    def simple_function(message):
        print message

    transport.add_handler(Bean_Serial_Transport.MSG_ID_BL_STATUS, simple_function)
    
