import subprocess
import os

GREEN = '\033[92m'
RED = '\033[91m'
ENDC = '\033[0m'

#TODO timeout detection, print return codes

class Tester:
    def __init__(self, compressorPath):
        self.absolutePath = os.path.dirname(__file__)
        self.compressorPath = os.path.join(self.absolutePath, compressorPath)

    def run(self, filePath):
        absoluteFilePath = os.path.join(self.absolutePath, filePath)
        # print("<<TESTER>> : BEGIN COMPRESSION")
        retcode = subprocess.call([self.compressorPath, '-c', absoluteFilePath, 'temp.compressed'], stdout=subprocess.DEVNULL) 
        if retcode != 0:
            print(RED+"FAILED"+ENDC, filePath, "(COMPRESSION)")
            return    
        # print("<<TESTER>> : BEGIN DECOMPRESSION")
        retcode = subprocess.call([self.compressorPath, '-d', 'temp.compressed', 'temp.decompressed'], stdout=subprocess.DEVNULL) 
        if retcode != 0:
            print(RED+"FAILED"+ENDC, filePath, "(DECOMPRESSION)")
            return    
        # print("<<TESTER>> : BEGIN COMPARISSON")
        p = subprocess.Popen(['cmp', absoluteFilePath, 'temp.decompressed'], stdout=subprocess.PIPE)
        p.wait()
        p.stdout.flush()
        output = p.stdout.read().decode('ASCII')
        if output == "":
           print(GREEN+"PASSED"+ENDC, filePath)
        else:
            print(RED+"FAILED"+ENDC, filePath, "(COMPARISSON)")

tester = Tester("../build/byte-compressor")
tester.run("files/22_total_5_unique.txt")
tester.run("files/iliad.txt")
tester.run("files/1000_digits.bin")
tester.run("files/1000_lowercase.bin")
tester.run("files/1000_printable.bin")
tester.run("files/10000_all.bin")
tester.run("files/lena.tiff")
tester.run("files/mozart_symphony_40.wav")
