# i2c_utils.py
from smbus2 import SMBus

I2C_BUS = 1
I2C_ADDR = 0x08

# Open the bus exactly once
_bus = SMBus(I2C_BUS)

def send_i2c_flag(flag: int):
    try:
        _bus.write_i2c_block_data(I2C_ADDR, 0, [flag])
        print(f"[i2c_utils] Sent flag {flag} to ESP32")
    except Exception as e:
        print(f"[i2c_utils] Failed to send I2C flag: {e}")
