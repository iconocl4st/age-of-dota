#  https://stackoverflow.com/questions/6886283/how-i-can-i-lazily-read-multiple-json-values-from-a-file-stream-in-python


import math

class JsonTokens:
    BEGIN_OBJECT = '{'
    END_OBJECT = '}'
    BEGIN_ARRAY = '['
    END_ARRAY = ']'
    NULL = 'null'
    EOF = 'eof'
    TRUE = 'true'
    FALSE = 'false'
    COLON = ':'
    STRING = '"'
    NUMBER = 'number'
    COMMA = ','

def _parse_utf8(byte_data, emit):
    # TODO:
    raise Exception("Not parsing utf yet...")
    # left = 0
    # num = 0
    #
    # def _utf8(byte_data):
    #     if (byte_data & 0xc0) != 0x80:
    #         raise Exception("Invalid byte in UTF-8 character: 0x" + byte_data.toString(16))
    #
    #     left = left - 1
    #
    #     num |= (byte_data & 0x3f) << (left * 6)
    #     if left:
    #         return _utf8
    #     return emit(num)
    #
    # if 0xc0 <= byte_data < 0xe0:  # 2-byte UTF-8 Character
    #     left = 1
    #     num = (byte_data & 0x1f) << 6
    #     return _utf8
    #
    # if 0xe0 <= byte_data < 0xf0:  # 3-byte UTF-8 Character
    #     left = 2
    #     num = (byte_data & 0xf) << 12
    #     return _utf8
    #
    # if 0xf0 <= byte_data < 0xf8:  # 4-byte UTF-8 Character
    #     left = 3
    #     num = (byte_data & 0x07) << 18
    #     return _utf8
    #
    # raise Exception("Invalid byte in UTF-8 string: 0x" + str(byte_data))


# Nestable state machine for hex escaped characters
def _parse_hex(reader):
    # TODO
    raise Exception("Not parsing hex yet....")
    # left = 4
    # num = 0
    #
    # def _hex(byte_data):
    #     nonlocal num, left
    #
    #     if 0x30 <= byte_data < 0x40:
    #         i = byte_data - 0x30
    #     elif 0x61 <= byte_data <= 0x66:
    #         i = byte_data - 0x57
    #     elif 0x41 <= byte_data <= 0x46:
    #         i = byte_data - 0x37
    #     else:
    #         raise Exception("Expected hex char in string hex escape")
    #
    #     left -= 1
    #     num |= i << (left * 4)
    #
    #     if left:
    #         return _hex
    #     return emit(num)
    #
    # return _hex

def _parse_constant(expected, reader):
    for i in range(len(expected)):
        found = reader.read()
        if expected[i] != found:
            raise Exception("Unexpected character, expected: " + str(expected[i]) + ", but found: " + str(found))

def _parse_string(reader):
    string = ""
    while True:
        byte_data = reader.read()
        if byte_data == 0x22:  # "
            return string

        if byte_data == 0x5c:  # \
            string += _parse_escape(reader)

        if byte_data & 0x80:  # UTF-8 handling
            string += _parse_utf8(byte_data, on_char_code)

        if byte_data < 0x20:  # ASCII control character
            raise Exception("Unexpected control character: 0x" + str(byte_data))

        string += chr(byte_data)

def _parse_escape(byte_data):
    if byte_data == 0x22 or byte_data == 0x5c or byte_data == 0x2f:  # " \ /
        return chr(byte_data)

    if byte_data == 0x62:  # b
        return "\b"

    if byte_data == 0x66:  # f
        return "\f"

    if byte_data == 0x6e:  # n
        return "\n"

    if byte_data == 0x72:  # r
        return "\r"

    if byte_data == 0x74:  # t
        return "\t"

    if byte_data == 0x75:  # u
        return _parse_hex(reader)

    #def on_char_code(char_code):
    #    nonlocal string
    #    string += chr(char_code)
    #    return _string

    raise Exception("Unexpected escape: " + chr(byte_data))

def _is_number_char(byte_data):
    part_of_float = (
            byte_data == 0x2e  # .
            or byte_data == 0x45  # E
            or byte_data == 0x65  # e
    )
    return (
            part_of_float
            or 0x30 <= byte_data <= 0x39 # 0-9
            or byte_data == 0x2d  # -
            or byte_data == 0x2b  # +
    ), part_of_float

def _parse_number(byte_data, reader):
    string = "" + chr(byte_data)
    is_integer = True
    while True:
        byte_data = reader.peek()
        is_num, is_float = _is_number_char(byte_data)
        if is_float:
            is_integer = False
        if is_num:
            string += chr(reader.read())
        else:
            break
    if is_integer:
        return int(string)
    else:
        return float(string)

class JsonTokener:
    TRUE = [0x72, 0x75, 0x65]
    FALSE = [0x61, 0x6c, 0x73, 0x65]
    NULL = [0x75, 0x6c, 0x6c]

    def __init__(self, reader):
        self.reader = reader
        self.value = None
        self.token = None

    def next(self):
        byte_data = self.reader.read()

        while (
                byte_data == 0x09 or # horizontal tab
                byte_data == 0x0a or # new line
                byte_data == 0x0d or # carriage return
                byte_data == 0x20 or # data line escape
                byte_data == 0x40 # space
        ):
            byte_data = self.reader.read()

        # import pdb; pdb.set_trace()

        # print("Next token is a " + chr(byte_data))

        if not byte_data:
            self.token = JsonTokens.EOF
            self.value = None
            return self.token

        if _is_number_char(byte_data)[0]:
            self.token = JsonTokens.NUMBER
            self.value = _parse_number(byte_data, self.reader)
            return self.token

        if byte_data == 0x22:  # "
            self.token = JsonTokens.STRING
            self.value = _parse_string(self.reader)
            return self.token

        if byte_data == 0x3a:  # :
            self.token = JsonTokens.COLON
            self.value = None
            return self.token

        if byte_data == 0x7b:  # {
            self.token = JsonTokens.BEGIN_OBJECT
            self.value = None
            return self.token

        if byte_data == 0x7d:  # }
            self.token = JsonTokens.END_OBJECT
            self.value = None
            return self.token

        if byte_data == 0x5b:  # [
            self.token = JsonTokens.BEGIN_ARRAY
            self.value = None
            return self.token

        if byte_data == 0x5d:  # ]
            self.token = JsonTokens.END_ARRAY
            self.value = None
            return self.token

        if byte_data == 0x2c:  # ,
            self.token = JsonTokens.COMMA
            self.value = None
            return self.token

        if byte_data == 0x74:  # t
            _parse_constant(JsonTokener.TRUE, self.reader)
            self.token = JsonTokens.TRUE
            self.value = None
            return self.token

        if byte_data == 0x66:  # f
            _parse_constant(JsonTokener.FALSE, self.reader)
            self.token = JsonTokens.FALSE
            self.value = None
            return self.token

        if byte_data == 0x6e:  # n
            _parse_constant(JsonTokener.NULL, self.reader)
            self.token = JsonTokens.NULL
            self.value = None
            return self.token

        raise Exception("Did not see expected character: " + byte_data)

class LookAhead:
    def __init__(self, reader):
        self.reader = reader
        self.peeked = False
        self.next = None

    def peek(self):
        if self.peeked:
            return self.next

        self.next = self.read()
        self.peeked = True
        return self.next


    def read(self):
        if self.peeked:
            r = self.next
            self.peeked = False
            self.next = None
            return r
        return self.reader.read()


class OpenedObject:
    def __init__(self, reader):
        self.reader = reader
    def __enter__(self):
        pass
    def __exit__(self, exc_type, exc_val, exc_tb):
        self.reader.read_end_object()


class JsonParser:
    def __init__(self, tokenizer):
        self.tokenizer = tokenizer

    def _skip_comma(self):
        if self.tokenizer.token == JsonTokens.COMMA:
            self.tokenizer.next()

    def _expect(self, token, allow_nulls=False):
        if token == JsonTokens.NULL and allow_nulls:
            return
        if token != self.tokenizer.token:
            raise Exception("Expected: " + token + ", found: " + self.tokenizer.token)

    def _expect_key(self, key):
        if key != self.tokenizer.value:
            raise Exception("Wrong key, expected: " + key + ", found: " + self.tokenizer.value)

    def _skip_key(self, key):
        if key is None:
            return
        self._expect(JsonTokens.STRING)
        self._expect_key(key)

        self.tokenizer.next()
        self._expect(JsonTokens.COLON)

        self.tokenizer.next()

    def read_begin_object(self, key=None, allow_null=False):
        self.tokenizer.next()
        self._skip_comma()
        self._skip_key(key)
        self._expect(JsonTokens.BEGIN_OBJECT, allow_null)
        return OpenedObject(self)

    def read_end_object(self):
        self.tokenizer.next()
        self._expect(JsonTokens.END_OBJECT)

    def read_begin_array(self, key=None, allow_null=False):
        self.tokenizer.next()
        self._skip_comma()
        self._skip_key(key)
        self._expect(JsonTokens.BEGIN_ARRAY, allow_null)

    def is_end_array(self, alternative_token, allow_null=False):
        self.tokenizer.next()
        self._skip_comma()
        if self.tokenizer.token == JsonTokens.END_ARRAY:
            return True
        elif self.tokenizer.token == alternative_token:
            return False
        elif self.tokenizer.token == JsonTokens.NULL and allow_null:
            return False
        else:
            raise Exception("uh oh, expected: end array or " + alternative_token + ", found: " + self.tokenizer.token)

    def read_number(self, key=None, allow_null=False):
        self.tokenizer.next()
        self._skip_comma()
        self._skip_key(key)
        self._expect(JsonTokens.NUMBER, allow_null)
        return self.tokenizer.value

    def read_string(self, key=None, allow_null=False):
        self.tokenizer.next()
        self._skip_comma()
        self._skip_key(key)
        self._expect(JsonTokens.STRING, allow_null)
        return self.tokenizer.value

    def read_boolean(self, key=None, allow_null=False):
        self.tokenizer.next()
        self._skip_comma()
        self._skip_key(key)
        if self.tokenizer.token == JsonTokens.TRUE:
            return True
        elif self.tokenizer.token == JsonTokens.FALSE:
            return False
        elif self.tokenizer.token == JsonTokens.NULL and allow_null:
            return None
        else:
            raise Exception("Expected boolean")


class SocketOutputWriter:
    def __init__(self, socket):
        self.client_socket = socket
        self.output_buffer = bytearray()

    def write(self, some_string):
        self.output_buffer.extend(some_string.encode("latin-1"))

    def flush(self):
        # print("Sending " + str(self.output_buffer))
        self.client_socket.send(self.output_buffer)
        self.output_buffer = bytearray()


class FileOutputWriter:
    def __init__(self, output_file):
        self.output_file = output_file

    def write(self, some_string):
        self.output_file.write(some_string)

    def flush(self):
        self.output_file.flush()


class JsonTextWriter:
    def __init__(self, output_writer):
        self.requires_comma = False
        self.indentation = 0
        self.output_writer = output_writer

    def _write(self, some_string):
        self.output_writer.write(some_string)

    def _indent(self):
        for i in range(self.indentation):
            self._write('\t')

    def _write_comma(self):
        if not self.requires_comma:
            return
        self._write(',\n')
        self.requires_comma = False

    def _write_key(self, key):
        if key is None:
            return

        self._write("\"")
        self._write(key)
        self._write("\": ")

    def write_begin_array(self, key=None):
        self._write_comma()
        self._indent()
        self._write_key(key)
        self._write("[\n")
        self.indentation += 1
        self.requires_comma = False

    def write_end_array(self):
        self.indentation -= 1
        self._write('\n')
        self._indent()
        self._write(']')
        self.requires_comma = True

    def write_begin_object(self, key=None):
        self._write_comma()
        self._indent()
        self._write_key(key)
        self._write("{\n")
        self.indentation += 1
        self.requires_comma = False

    def write_end_object(self):
        self.indentation -= 1
        self._write('\n')
        self._indent()
        self._write('}')
        self.requires_comma = True

    def write_boolean(self, key=None, value=None):
        self._write_comma()
        self._indent()
        self._write_key(key)
        if value is None:
            raise Exception("booleans cannot be null")
        if value:
            self._write("true")
        else:
            self._write("false")
        self.requires_comma = True

    def write_string(self, key=None, value=None):
        self._write_comma()
        self._indent()
        self._write_key(key)
        if value is None:
            raise Exception("strings cannot be null")
        self._write('"')
        self._write(value)
        self._write('"')
        self.requires_comma = True

    def write_number(self, key=None, value=None):
        self._write_comma()
        self._indent()
        self._write_key(key)
        if value is None:
            raise Exception("numbers cannot be null")
        self._write(str(value))
        self.requires_comma = True

    def flush(self):
        self.output_writer.flush()


class JsonSplitWriter:
    def __init__(self, writer1, writer2):
        self.writer1 = writer1
        self.writer2 = writer2

    def write_begin_array(self, key=None):
        self.writer1.write_begin_array(key=key)
        self.writer2.write_begin_array(key=key)

    def write_end_array(self):
        self.writer1.write_end_array()
        self.writer2.write_end_array()

    def write_begin_object(self, key=None):
        self.writer1.write_begin_object(key=key)
        self.writer2.write_begin_object(key=key)

    def write_end_object(self):
        self.writer1.write_end_object()
        self.writer2.write_end_object()

    def write_boolean(self, key=None, value=None):
        self.writer1.write_boolean(key=key, value=value)
        self.writer2.write_boolean(key=key, value=value)

    def write_string(self, key=None, value=None):
        self.writer1.write_string(key=key, value=value)
        self.writer2.write_string(key=key, value=value)

    def write_number(self, key=None, value=None):
        self.writer1.write_number(key=key, value=value)
        self.writer2.write_number(key=key, value=value)

    def flush(self):
        self.writer1.flush()
        self.writer2.flush()
