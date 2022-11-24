import serial
import time
import io
import os


if __name__ == "__main__":
    try:
        ser = serial.Serial('/dev/tty.usbmodem14101')
    except Exception:
        print("Cannot open serial port")

    recording = []

    while True:
        os.system('cls' if os.name == 'nt' else 'clear')
        print("Record, play, loop, save, or load? (Enter R, P, L, S or LD): ", end="", flush=True)
        action = input()
        if action == 'R':
            recording.clear()
            print("Beginning 5 second recording in 3...", end="", flush=True)
            time.sleep(1)
            print("2...", end="", flush=True)
            time.sleep(1)
            print("1...", end="", flush=True)
            time.sleep(1)
            print("GO!")
            ser.write(b'R')

            start = time.time()
            ser.read().decode()
            byte = ser.read().decode()

            while byte != '3':
                recording.append(byte)
                print(round(time.time() - start, 3), "s", end="\r", flush=True)
                byte = ser.read().decode()

            print("\033[2K\033[1G", end="")
            print("DONE!")

        elif action == 'P' or action == 'L':
            print("Playback speed slower or faster? (Enter S or F): ", end="", flush=True)
            type = input()
            print("Type in playback speed factor (1-9x): ", end="", flush=True)
            factor = input()
            if action == 'P':
                print("Playing back recording in 3...", end="", flush=True)
            else:
                print("Looping back recording in 3...", end="", flush=True)
            time.sleep(1)
            print("2...", end="", flush=True)
            time.sleep(1)
            print("1...")
            time.sleep(1)

            ser.write(bytes(action, 'utf-8'))
            ser.write(bytes(action, 'utf-8'))
            ser.write(bytes(factor, 'utf-8'))

            ser.read().decode()
            if action == 'P':
                start = time.time()
                for byte in recording:
                    if byte == '1':
                        ser.write(b'1')
                    else:
                        ser.write(b'0')
                    print(round(time.time() - start, 3), "s", end="\r", flush=True)
            else:
                while True:
                    start = time.time()
                    for byte in recording:
                        if byte == '1':
                            ser.write(b'1')
                        else:
                            ser.write(b'0')
                        print(round(time.time() - start, 3), "s", end="\r", flush=True)
                    ser.read().decode()

            ser.read().decode()
            print("\033[2K\033[1G", end="")
            print("DONE!")

        elif action == 'S':
            print("Saving recording...", end="", flush=True)
            with open("recording.txt", "w") as file:
                for byte in recording:
                    file.write(byte + "\n")
            print("DONE!")

        elif action == 'LD':
            recording.clear()
            print("Loading recording...", end="", flush=True)
            with open("recording.txt", "r") as file:
                for line in file:
                    recording.append(line.rstrip())
            print("DONE!")

        time.sleep(1)