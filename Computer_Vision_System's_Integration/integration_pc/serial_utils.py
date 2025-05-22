import serial

_ser = None

def init_serial(port="COM4", baud=9600):
    """
    Call once at the very start to open the port with default (blocking) behavior.
    """
    global _ser
    if _ser is None:
        try:
            _ser = serial.Serial(port, baud)   # no timeout argument
            print(f"[serial_utils] Opened {port}@{baud}")
        except Exception as e:
            print(f"[serial_utils] Failed to open {port}: {e}")
    return _ser

def send_uart_flag(flag):
    """
    Use this everywhere to send a flag over that one shared port.
    """
    if _ser is None:
        raise RuntimeError("serial not initialized — call init_serial() first")
    _ser.write(f"{flag}".encode())
    print(f"[serial_utils] UART SENT -> {flag}")
