import subprocess
import os

GREEN = '\033[92m'
RED = '\033[91m'
ENDC = '\033[0m'

# class Tester:
    # def __init__(self, compiler_path, interpreter_path, progs_dir, inputs_dir, outputs_dir):
    #     self.absolute_path = os.path.dirname(__file__)
    #     self.compiler_path = os.path.join(self.absolute_path, compiler_path)
    #     self.compiled_src_filename =  os.path.join(self.absolute_path, "temp.ifjcode")
    #     self.compiled_output_filename =  os.path.join(self.absolute_path, "temp.out")
    #     self.progs_dir = progs_dir
    #     self.inputs_dir = inputs_dir
    #     self.outputs_dir = outputs_dir
    #     self.interpreter_path = os.path.join(self.absolute_path, interpreter_path)

    # def run_compiler(self, prog_file_name, input_file_name, output_file_name):
    #     prog_file_path = os.path.join(self.absolute_path, self.progs_dir+prog_file_name)
    #     output_file_path = os.path.join(self.absolute_path, self.outputs_dir+output_file_name)

    #     compiler_output_file = open(self.compiled_src_filename, 'w')
    #     compiler_input_file = open(prog_file_path, 'r')
    #     compiler_process = subprocess.Popen([self.compiler_path], stdin=compiler_input_file, stdout=compiler_output_file)

    #     compiler_process.wait()

    #     compiler_input_file.close()

    #     compiler_output_file.flush()
    #     compiler_output_file.close()

    # def run_interpreter(self, prog_file_name, input_file_name, output_file_name):
    #     interpreter_output_file = open(self.compiled_output_filename, 'w')
    #     if input_file_name == None:
    #         p = subprocess.Popen([self.interpreter_path, os.path.join(self.absolute_path, "temp.ifjcode")], stdin=subprocess.PIPE, stdout=interpreter_output_file)
    #         p.wait()
    #     else:
    #         interpreter_input_filepath = os.path.join(self.absolute_path, self.inputs_dir+input_file_name)
    #         interpreter_input_file = open(interpreter_input_filepath, 'r')
    #         p = subprocess.Popen([self.interpreter_path, os.path.join(self.absolute_path, "temp.ifjcode")], stdin=interpreter_input_file, stdout=interpreter_output_file)
    #         p.wait()
    #         interpreter_input_file.close()

    #     interpreter_output_file.flush()
    #     interpreter_output_file.close()


    # def run_diff(self, prog_file_name, input_file_name, output_file_name):
    #     expected_output_filename =  os.path.join(self.absolute_path, self.outputs_dir + output_file_name)
    #     p = subprocess.Popen(["diff", self.compiled_output_filename, expected_output_filename], stdout=subprocess.PIPE)
    #     p.wait()
    #     p.stdout.flush()
    #     output = p.stdout.read().decode('ASCII')
    #     if output == "":
    #         print(GREEN+"PASSED   "+ENDC+"{", prog_file_name, "|",input_file_name, "|",output_file_name,"}")
    #     else:
    #         print(RED+"FAILED   "+ENDC+"{", prog_file_name, "|",input_file_name, "|",output_file_name,"}\n", output)


    # def run(self, prog_file_name, input_file_name, output_file_name):
    #     self.run_compiler(prog_file_name, input_file_name, output_file_name)
    #     self.run_interpreter(prog_file_name, input_file_name, output_file_name)
    #     self.run_diff(prog_file_name, input_file_name, output_file_name)

# tester = Tester("../main", "../../ic22int", "progs/", "inputs/", "outputs/")
# tester.run("hello_world", None, "hello_world.out")
# tester.run("expr1", None, "expr1.out")
# tester.run("expr2", None, "expr2.out")
# tester.run("reads_write", "reads_write.in", "reads_write.out")
# tester.run("expr3", None, "expr3.out")

1. run compression on a file
2. run decompression on a result file
3. compare files by cmp 