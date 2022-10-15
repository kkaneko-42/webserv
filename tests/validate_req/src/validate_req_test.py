import socket
import unittest
import glob
import os

TEST_CASES_DIR="./cases/"

def req(raw_req):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.settimeout(5)
    sock.connect(("127.0.0.1", 4242))
    sock.send(raw_req.encode())
    rcv_data = sock.recv(1024)
    sock.close()
    return rcv_data

def get_status(rcv_data):
    line = rcv_data.split("\r\n")[0]
    status = line.split()[1]
    return int(status)

def load_test_cases():
    def path_to_status(path):
        filename = os.path.basename(path)
        status = filename.split("_")[0]
        return int(status)

    cases = []
    files = glob.glob(TEST_CASES_DIR + "*")
    for f in files:
        cases += [{"status": path_to_status(f), "path": f}]
    return cases

class ReqValidationTest(unittest.TestCase):
    def test_all(self):
        cases = load_test_cases()

        for case in cases:
            path = case["path"]
            status = case["status"]
            with open(path) as f:
                print(f"Testing {path}...")
                raw_req = f.read()
                resp = req(raw_req.replace("\r\n", "\n").replace("\n", "\r\n"))
                self.assertEqual(status, get_status(str(resp)))

if __name__ == "__main__":
    # print(load_test_cases())
    # print(get_status("HTTP/1.1 200 OK\r\naaaaa"))
    unittest.main()
