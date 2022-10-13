import requests
import unittest
import os
import shutil

LOG_DIR="./log"
ENV_DIR="./env"
URL="http://localhost:4242"
UPLOAD_DIR = ENV_DIR + "/upload"
HEADERS = {"Transfer-Encoding": "chunked"}

def create_chunked_body(arr):    
    data = ""
    for e in arr:
        data += f"{hex(len(e))[2:]}\r\n{e}\r\n"
    data += "0\r\n\r\n"
    return data

class ChunkPostTest(unittest.TestCase):
    def __init__(self, *args, **kwargs):
        super(ChunkPostTest, self).__init__(*args, **kwargs)
        shutil.rmtree(UPLOAD_DIR)
        os.mkdir(UPLOAD_DIR)

    def test_chunk_post(self):
        expected = ["aaaaaaaaaaaaaaaaaaaa", "vvvvv", "c"]
        data = create_chunked_body(expected)
        r = requests.post(URL + "/post/chunk_create.txt", headers=HEADERS, data=data)

        self.assertEqual(201, r.status_code)
        with open(UPLOAD_DIR + "/chunk_create.txt") as f:
            actual = f.read()
            self.assertEqual(''.join(expected), actual)

    def test_chunk_no_data(self):
        data = "0\r\n\r\n"
        r = requests.post(URL + "/post/chunk_no_data.txt", headers=HEADERS, data=data)

        self.assertEqual(201, r.status_code)
        with open(UPLOAD_DIR + "/chunk_no_data.txt") as f:
            actual = f.read()
            self.assertEqual("", actual)

    def test_one_chunked(self):
        expected = ["hello, world"]
        data = create_chunked_body(expected)
        r = requests.post(URL + "/post/one_chunked.txt", headers=HEADERS, data=data)

        self.assertEqual(201, r.status_code)
        with open(UPLOAD_DIR + "/one_chunked.txt") as f:
            actual = f.read()
            self.assertEqual(''.join(expected), actual)

    def test_has_empty_chunk(self):
        cases = [
            ["", "hogehoge", "fuga"],
            ["hogehoge", "", "fuga"],
            ["hogehoge", "fuga", ""]
        ]

        for case in cases:
            data = create_chunked_body(case)
            r = requests.post(URL + "/post/chunk_has_empty_chunk.txt", headers=HEADERS, data=data)
            self.assertEqual(400, r.status_code)

    def test_no_eof(self):
        s = "hello, world"
        data = f"{hex(len(s))[2:]}\r\n{s}\r\n"
        r = requests.post(URL + "/post/ko.txt", headers=HEADERS, data=data)
        self.assertEqual(400, r.status_code)

    def test_length_too_short(self):
        s = "hello, world"
        data = f"{hex(len(s) - 1)[2:]}\r\n{s}\r\n0\r\n\r\n"
        r = requests.post(URL + "/post/ko.txt", headers=HEADERS, data=data)
        self.assertEqual(400, r.status_code)

    def test_length_too_long(self):
        s = "hello, world"
        data = f"{hex(len(s) + 1)[2:]}\r\n{s}\r\n0\r\n\r\n"
        r = requests.post(URL + "/post/ko.txt", headers=HEADERS, data=data)
        self.assertEqual(400, r.status_code)

    def test_invalid_eof(self):
        s = "hello, world"
        data = f"{hex(len(s))[2:]}\r\n{s}\r\n0\r\n"
        r = requests.post(URL + "/post/ko.txt", headers=HEADERS, data=data)
        self.assertEqual(400, r.status_code)

    def test_no_CR_in_body(self):
        s = "hello, world"
        data = f"{hex(len(s))[2:]}\n{s}\n0\r\n\r\n"
        r = requests.post(URL + "/post/ko.txt", headers=HEADERS, data=data)
        self.assertEqual(400, r.status_code)

    def test_no_LF_in_body(self):
        s = "hello, world"
        data = f"{hex(len(s))[2:]}\r{s}\r0\r\n\r\n"
        r = requests.post(URL + "/post/ko.txt", headers=HEADERS, data=data)
        self.assertEqual(400, r.status_code)

    def test_no_CR_in_eof(self):
        s = "hello, world"
        data = f"{hex(len(s))[2:]}\r\n{s}\r\n0\n\n"
        r = requests.post(URL + "/post/ko.txt", headers=HEADERS, data=data)
        self.assertEqual(400, r.status_code)

    def test_no_LF_in_eof(self):
        s = "hello, world"
        data = f"{hex(len(s))[2:]}\r\n{s}\r\n0\r\r"
        r = requests.post(URL + "/post/ko.txt", headers=HEADERS, data=data)
        self.assertEqual(400, r.status_code)

if __name__ == "__main__":
    unittest.main()
