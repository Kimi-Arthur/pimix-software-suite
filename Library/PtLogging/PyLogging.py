import sys

class Logger:
    output = sys.stdout
    @classmethod
    def set_output(cls, file_name):
        if output != sys.stdout:
            output.close()
        if file_name is None:
            output = sys.stdout
        else:
            output = open(file_name)

    @classmethod
    def log(cls, content, type='INFO'):
        
        for line in content.splitlines():
            output.write(log_pattern, )

    @classmethod
    def info(cls, content):
        cls.log(content, "INFO")

    @classmethod
    def err(cls, content):
        cls.log(content, "ERROR")

    @classmethod
    def warn(cls, content):
        cls.log(content, "WARN")

    @classmethod
    def fatal(cls, content):
        cls.log(content, "FATAL")

    @classmethod
    def debug(cls, content):
        cls.debug(content, "DEBUG")
