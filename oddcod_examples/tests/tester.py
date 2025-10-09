import subprocess
import os
import time
import sys

GREEN = '\033[92m'
RED = '\033[91m'
ENDC = '\033[0m'

TEMP_COMPRESS_FILE_NAME = "temp.compress"
TEMP_DECOMPRESS_FILE_NAME = "temp.decompress"

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
        base_path = os.path.dirname(os.path.abspath(__file__))
        abs_dir_path = os.path.join(base_path, dir_path)

        for root, _, files in os.walk(abs_dir_path):
            for filename in files:
                file_path = os.path.join(root, filename)
                self.run(file_path, os.path.relpath(file_path, start=base_path))

        if os.path.exists(TEMP_COMPRESS_FILE_NAME):
            os.remove(TEMP_COMPRESS_FILE_NAME)
        if os.path.exists(TEMP_DECOMPRESS_FILE_NAME):
            os.remove(TEMP_DECOMPRESS_FILE_NAME)


    def run(self, file_path, test_name):
        absoluteFilePath = os.path.join(self.absolute_path, file_path)
        
        start = time.time()
        stdoutRedirect = subprocess.PIPE
        #stdoutRedirect = sys.stdout

        #compression
        file_stats = os.stat(file_path)
        print('{0:32} {1:10}B'.format(test_name, file_stats.st_size), end='\t')
        process = subprocess.Popen([self.compressor_path, '-c', absoluteFilePath, TEMP_COMPRESS_FILE_NAME], stdout=stdoutRedirect, stderr=subprocess.PIPE) 
        try:
            stream_out, stream_err = process.communicate(timeout=3)
            if process.returncode != 0:
                return self.failed(file_path, 'COMPRESSION', process.returncode, stream_err)
        except subprocess.TimeoutExpired:
            return self.failed(file_path, 'COMPRESSION timeout', None, None)
        
        #decompression
        process = subprocess.Popen([self.compressor_path, '-d', TEMP_COMPRESS_FILE_NAME, TEMP_DECOMPRESS_FILE_NAME], stdout=stdoutRedirect, stderr=subprocess.PIPE) 
        try:
            stream_out, stream_err = process.communicate(timeout=3)
            if process.returncode != 0:
                return self.failed(file_path, 'DECOMPRESSION', process.returncode, stream_err)
        except subprocess.TimeoutExpired:
            return self.failed(file_path, 'DECOMPRESSION timeout', None, None)

        #compare
        process = subprocess.Popen(['cmp', absoluteFilePath, TEMP_DECOMPRESS_FILE_NAME], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stream_out, stream_err = process.communicate()
        output = stream_out.decode('ASCII')

        elapsed = time.time() - start

        if output != "":
            return self.failed(file_path, 'COMPARISSON')

        file_stats_after = os.stat(TEMP_COMPRESS_FILE_NAME)
        
        print('{} {}ms (deflated {}%)'.format(GREEN+"PASSED"+ENDC,
                                                    round(elapsed*1000, 2),
        round(100 - (100*file_stats_after.st_size)/file_stats.st_size, 2)
        ))

tester = Tester("../build/file-compression")
tester.runAll("both")