import serial
import time

# Open UART connection
ser = serial.Serial('COM3', 9600)

time.sleep(2)  # Give time for the connection to stabilize

# Send a flag message
ser.write(b"ON")  # Send flag "ON" or any message
