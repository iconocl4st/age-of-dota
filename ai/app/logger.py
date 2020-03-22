
class Logger:
    def __init__(self, out_file):
        self.out_file = out_file

    def log(self, msg):
        # print(msg)
        self.out_file.write(msg + "\n")
        self.out_file.flush()
