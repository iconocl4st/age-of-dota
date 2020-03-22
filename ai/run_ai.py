
import sys

from app.connect import connect

if __name__ == '__main__':
    print("AI script is alive.")
    if len(sys.argv) != 3:
        print("Illegal number of arguments")
    else:
        connect(sys.argv[1], int(sys.argv[2]))
        print('AI script complete')
