import subprocess
import os
import time
import sys

GREEN = '\033[92m'
RED = '\033[91m'
ENDC = '\033[0m'

#TODO timeout detection

class Tester:
    def __init__(self, compressorPath):
        self.absolutePath = os.path.dirname(__file__)
        self.compressorPath = os.path.join(self.absolutePath, compressorPath)

    def failed(self, filePath, testname, retcode = None, stderr=None):
        print(RED+"FAILED"+ENDC, filePath, "({})".format(testname))
        if retcode != None:
            print('return code: {}'.format(retcode))
        if stderr != None:
            print('stderr: ')
            print(stderr.decode('ASCII'))
        return 1

    def run(self, filePath):
        absoluteFilePath = os.path.join(self.absolutePath, filePath)
        
        start = time.time()
        stdoutRedirect = subprocess.PIPE
        #stdoutRedirect = sys.stdout

        #compression
        process = subprocess.Popen([self.compressorPath, '-c', absoluteFilePath, 'temp.compressed'], stdout=stdoutRedirect, stderr=subprocess.PIPE) 
        stream_out, stream_err = process.communicate()
        if process.returncode != 0:
            return self.failed(filePath, 'COMPRESSION', process.returncode, stream_err)

        #decompression
        process = subprocess.Popen([self.compressorPath, '-d', 'temp.compressed', 'temp.decompressed'], stdout=stdoutRedirect, stderr=subprocess.PIPE) 
        stream_out, stream_err = process.communicate()
        if process.returncode != 0:
            return self.failed(filePath, 'DECOMPRESSION', process.returncode, stream_err)
            
        #compare
        process = subprocess.Popen(['cmp', absoluteFilePath, 'temp.decompressed'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stream_out, stream_err = process.communicate()
        output = stream_out.decode('ASCII')

        elapsed = time.time() - start

        if output != "":
            return self.failed(filePath, 'COMPARISSON')

        print('{0} {1:50}\t{2}ms'.format(GREEN+"PASSED"+ENDC, filePath, round(elapsed*1000, 2)))

tester = Tester("../build/byte-compressor")
tester.run("files/22_total_5_unique.txt")
tester.run("files/1000_digits.bin")
tester.run("files/1000_lowercase.bin")
tester.run("files/1000_printable.bin")
tester.run("files/iliad.txt")
tester.run("files/10000_all.bin")
#tester.run("files/lena.tiff")
#tester.run("files/mozart_symphony_40.wav")
