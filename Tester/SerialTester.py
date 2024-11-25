import serial.tools
import serial.tools.list_ports as port_list
import serial
import random
from time import sleep

# Define DataID as a dictionary
DataID = {
    "OIL_PRESSURE": 0x01,
    "OIL_TEMPERATURE": 0x02,
    "FUEL_FLOW": 0x03,
    "FUEL": 0x04,
    "EGT": 0x05,
    "TORQUE": 0x06,
    "INDICATED_POWER": 0x07,
    "FRICTIONAL_POWER": 0x08,
    "THERMAL_EFFICIENCY": 0x09,
    "AIR_FUEL_RATIO": 0x0A,
    "MOTOR_SPEED": 0x0B,
    "OUTPUT_AIR_SPEED": 0x0C,
    "VIBRATION": 0x0D,
    "BODY_TEMP": 0x0E,
    "AIR_TEMP": 0x0F,
    "OIL_PRESSURE_SENSOR_ERROR": 0x11,
    "OIL_TEMPERATURE_SENSOR_ERROR": 0x12,
    "FUEL_FLOW_SENSOR_ERROR": 0x13,
    "FUEL_SENSOR_ERROR": 0x14,
    "EGT_SENSOR_ERROR": 0x15,
    "TORQUE_SENSOR_ERROR": 0x16,
    "INDICATED_POWER_SENSOR_ERROR": 0x17,
    "FRICTIONAL_POWER_SENSOR_ERROR": 0x18,
    "THERMAL_EFFICIENCY_SENSOR_ERROR": 0x19,
    "AIR_FUEL_RATIO_SENSOR_ERROR": 0x1A,
    "MOTOR_SPEED_SENSOR_ERROR": 0x1B,
    "OUTPUT_AIR_SPEED_SENSOR_ERROR": 0x1C,
    "VIBRATION_SENSOR_ERROR": 0x1D,
    "BODY_TEMP_SENSOR_ERROR": 0x1E,
    "AIR_TEMP_SENSOR_ERROR": 0x1F,
}

# Define allInfo as a dictionary
allInfo = {
    0x01: (False, "OIL PRESSURE", 0, 1000),
    0x02: (False, "OIL TEMPERATURE", 0, 400),
    0x03: (False, "FUEL FLOW", 0, 800),
    0x04: (False, "FUEL", 0, 800),
    0x05: (False, "EGT", 0, 400),
    0x06: (False, "TORQUE", 0, 400),
    0x07: (False, "INDICATED POWER", 0, 400),
    0x08: (False, "Frictional Power", 0, 400),
    0x09: (False, "Thermal efficiency", 0, 100),
    0x0A: (False, "Air-Fuel ratio", 0, 20),
    0x0B: (False, "MOTOR SPEED", 0, 1000),
    0x0C: (False, "OUTPUT AIR SPEED", 0, 1000),
    0x0D: (False, "VIBRATION", 0, 100),
    0x0E: (False, "BODY TEMP", 0, 400),
    0x0F: (False, "AIR TEMP", 0, 400),
    0x11: (True, "OIL PRESSURE SENSOR ERROR", 0, 1),
    0x12: (True, "OIL TEMPERATURE SENSOR ERROR", 0, 1),
    0x13: (True, "FUEL FLOW SENSOR ERROR", 0, 1),
    0x14: (True, "FUEL SENSOR ERROR", 0, 1),
    0x15: (True, "EGT SENSOR ERROR", 0, 1),
    0x16: (True, "TORQUE SENSOR ERROR", 0, 1),
    0x17: (True, "INDICATED POWER SENSOR ERROR", 0, 1),
    0x18: (True, "Frictional Power SENSOR ERROR", 0, 1),
    0x19: (True, "Thermal efficiency SENSOR ERROR", 0, 1),
    0x1A: (True, "Air-Fuel ratio SENSOR ERROR", 0, 1),
    0x1B: (True, "MOTOR SPEED SENSOR ERROR", 0, 1),
    0x1C: (True, "OUTPUT AIR SPEED SENSOR ERROR", 0, 1),
    0x1D: (True, "VIBRATION SENSOR ERROR", 0, 1),
    0x1E: (True, "BODY TEMP SENSOR ERROR", 0, 1),
    0x1F: (True, "AIR TEMP SENSOR ERROR", 0, 1),
}

index = 0

#define parameters here
TOTAL_PACKETS = 1000
BAD_CHECKSUM_INTERVAL = 17
NOISE_IN_PACKET_INTERVAL = 19
MISS_ONE_FIELD_IN_PACKET = True
SEND_DATA_IN_BYTES = True
SEND_DATA_RANDOM_DATA = False

class PacketData:
    def __init__(self,id = 0x1, miss = False):
        self.reserve = 0
        self.id = id
        isError,name,min,max = allInfo[self.id]
        value = None
        global index
        if SEND_DATA_RANDOM_DATA:
            value = random.randint(min,max)
        else:
            if isError:
                max = 2
            value = index % (max - min) + min
        if miss and MISS_ONE_FIELD_IN_PACKET:
            value = max+1
        self.factor = random.randint(0,100)
        if isError:
            self.factor = 1
        self.data = value*self.factor
    def tobytes(self) -> bytearray:
        return bytearray([self.id,self.reserve])+bytearray(self.data.to_bytes(4,'little'))+bytearray(self.factor.to_bytes(4,'little'))
        

class Packet:
    data : list[PacketData]
    def __init__(self,msgCounter = 0, badchecksum = False) -> None:
        self.data = []
        self.msgCounter = msgCounter
        self.badchecksum = badchecksum
        self.__fill()
    def __fill(self) -> None:
        allID = list(allInfo.keys())
        badID = random.choice(allID)
        print(f"msg counter:{self.msgCounter} - bad ID:{badID}")
        for id in allID:
            self.data.append(PacketData(id,id==badID))
    def __calculate_checksum(self,data: bytearray) -> int:
        checksum = 0
        if self.badchecksum:
            checksum = 1
        for byte in data:
            checksum += byte
            checksum %= 0x10000
        return checksum

    def tobytes(self) -> bytearray:
        header = [0xa5]*4
        footer = [0x55]
        idN = [len(self.data)]
        result = bytearray()
        result+=bytearray([self.msgCounter])
        result+=bytearray(idN)
        for packetdata in self.data:
            result+=packetdata.tobytes()
        
        result+=bytearray(self.__calculate_checksum(result).to_bytes(2,'little'))
        result = bytearray(header) + result + bytearray(footer)
        return result
        

    


def open_port(portName : serial.tools.list_ports_common.ListPortInfo) -> serial.Serial:
    ser = serial.Serial()
    ser.baudrate = 115200
    ser.port = portName.name
    ser.parity = serial.PARITY_ODD
    ser.stopbits = serial.STOPBITS_ONE
    ser.open()
    if ser.is_open:
        return ser
    return None


def run_test(ser : serial.Serial):
    for i in range(TOTAL_PACKETS):
        global index
        index = i
        msgCounter = i % 256
        packet = Packet(msgCounter,(i%BAD_CHECKSUM_INTERVAL)==0)
        bytes = packet.tobytes()
        if (i % NOISE_IN_PACKET_INTERVAL) == 0:
            bytes.insert(len(bytes), random.randint(0,255))
        if SEND_DATA_IN_BYTES:
            for byte in bytes:
                ser.write([byte])
        else:
            ser.write(bytes)
            sleep(20/1000) 

def run():
    print("choose a com port:")
    ports = list(port_list.comports())
    for i,p in enumerate(ports):
        print (i,"-",p)
    index = int(input("enter index:"))
    if index < 0 or index >= len(ports):
        print("bad index")
        return
    ser = open_port(ports[index])
    if ser:
        run_test(ser)
        ser.close()
    else:
        print("could not open port")
    
    


if __name__ == "__main__":
    while True:
        run()
