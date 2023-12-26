import subprocess
import os
import time
import sys

GREEN = '\033[92m'
RED = '\033[91m'
ENDC = '\033[0m'

#TODO timeout detection

class Tester:
    def __init__(self, compressor_path):
        self.absolute_path = os.path.dirname(__file__)
        self.compressor_path = os.path.join(self.absolute_path, compressor_path)

    def failed(self, file_path, testname, retcode = None, stderr=None):
        print(RED+"FAILED"+ENDC, file_path, "({})".format(testname))
        if retcode != None:
            print('return code: {}'.format(retcode))
        if stderr != None:
            print('stderr: ')
            print(stderr.decode('ASCII'))
        return 1

    def runAll(self, dir_path):
        for root, _, files in os.walk(dir_path):
            for filename in files:
                file_path = os.path.join(root, filename)
                self.run(file_path)

    def run(self, file_path):
        absoluteFilePath = os.path.join(self.absolute_path, file_path)
        
        start = time.time()
        stdoutRedirect = subprocess.PIPE
        #stdoutRedirect = sys.stdout

        #compression
        file_stats = os.stat(file_path)
        print('{0:42} {1}B'.format(file_path, file_stats.st_size))
        process = subprocess.Popen([self.compressor_path, '-c', absoluteFilePath, 'temp.compressed'], stdout=stdoutRedirect, stderr=subprocess.PIPE) 
        stream_out, stream_err = process.communicate()
        if process.returncode != 0:
            return self.failed(file_path, 'COMPRESSION', process.returncode, stream_err)

        #decompression
        process = subprocess.Popen([self.compressor_path, '-d', 'temp.compressed', 'temp.decompressed'], stdout=stdoutRedirect, stderr=subprocess.PIPE) 
        stream_out, stream_err = process.communicate()
        if process.returncode != 0:
            return self.failed(file_path, 'DECOMPRESSION', process.returncode, stream_err)

        #compare
        process = subprocess.Popen(['cmp', absoluteFilePath, 'temp.decompressed'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stream_out, stream_err = process.communicate()
        output = stream_out.decode('ASCII')

        elapsed = time.time() - start

        if output != "":
            return self.failed(file_path, 'COMPARISSON')

        file_stats_after = os.stat('temp.compressed')
        
        print('{} {}ms (deflated {}%)'.format(GREEN+"PASSED"+ENDC,
                                                    round(elapsed*1000, 2),
        round(100 - (100*file_stats_after.st_size)/file_stats.st_size, 2)
        ))
        #print('{0} {1:50}\t{2}ms'.format(GREEN+"PASSED"+ENDC, file_path, round(elapsed*1000, 2)))

tester = Tester("../build/byte-compressor")
tester.runAll("files")
#tester.run("files/21_total_5_unique.txt")
#tester.run("files/1000_digits.bin")
#tester.run("files/1000_lowercase.bin")
#tester.run("files/1000_printable.bin")
#tester.run("files/iliad.txt")
#tester.run("files/10000_all.bin")
##tester.run("files/lena.tiff")
##tester.run("files/mozart_symphony_40.wav")
#tester.run("files/img/hdr.pgm")
