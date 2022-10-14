import requests
import unittest
import os
import shutil

LOG_DIR="./log"
ENV_DIR="./env"
URL="http://localhost:4242"
CGI_DIR = ENV_DIR + "/cgi"
CHUNK_HEADER = {"Transfer-Encoding": "chunked"}
METHODS = [requests.get, requests.post]

def create_chunked_body(arr):    
    data = ""
    for e in arr:
        data += f"{hex(len(e))[2:]}\r\n{e}\r\n"
    data += "0\r\n\r\n"
    return data

class CgiTest(unittest.TestCase):
    def test_env(self):
        tests = [
            {
                "path_info": "/path/info",
                "query_string": "value=42&str=hogehoge",
            },
            {
                "path_info": "/",
                "query_string": "value=42&str=hogehoge",
            },
            {
                "path_info": "",
                "query_string": "value=42&str=hogehoge"
            },
            {   
                "path_info": "/path/info",
                "query_string": ""
            },
        ]

        for t in tests:
            for func in METHODS:
                path_info = t["path_info"]
                query_string = t["query_string"]
                # python cgi test
                r = func(URL + f"/cgi/print_env.py{path_info}?{query_string}")
                self.assertEqual(200, r.status_code)
                expected = f"PATH_INFO: {path_info}\nQUERY_STRING: {query_string}\n"
                self.assertEqual(expected, r.text)

                # sh cgi test
                r = func(URL + f"/cgi/print_env.sh{path_info}?{query_string}")
                self.assertEqual(200, r.status_code)
                expected = f"PATH_INFO: {path_info}\nQUERY_STRING: {query_string}\n"
                self.assertEqual(expected, r.text)

    def test_stdin(self):
        cases = [
            "hello, world", "c"
        ]

        for data in cases:
            for func in METHODS:
                r = func(URL + "/cgi/print_stdin.py", data=data)
                self.assertEqual(200, r.status_code)
                self.assertEqual(data, r.text)

    def test_chunked(self):
        arr = ["hogehoge", "fuga", "piyo", "c"]
        header = {"Transfer-Encoding": "chunked"}
        data = create_chunked_body(arr)

        for func in METHODS:
            r = func(URL + "/cgi/print_stdin.py", data=data, headers=header)
            self.assertEqual(200, r.status_code)
            self.assertEqual(''.join(arr), r.text)

    def test_cgi_script_fail(self):
        for func in METHODS:
            r = func(URL + "/cgi/fail.py")
            self.assertEqual(400, r.status_code)

if __name__ == "__main__":
    unittest.main()
