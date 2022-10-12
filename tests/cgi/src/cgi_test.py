import requests
import unittest
import os
import shutil

LOG_DIR="./log"
ENV_DIR="./env"
URL="http://localhost:4242"
CGI_DIR = ENV_DIR + "/cgi"
CHUNK_HEADER = {"Transfer-Encoding": "chunked"}

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
        methods = [requests.get, requests.post]
        
        for t in tests:
            for func in methods:
                path_info = t["path_info"]
                query_string = t["query_string"]
                r = func(URL + f"/cgi/print_env.py{path_info}?{query_string}")

                self.assertEqual(200, r.status_code)
                expected = f"PATH_INFO: {path_info}\nQUERY_STRING: {query_string}\n"
                self.assertEqual(expected, r.text)

    def test_stdin(self):
        

if __name__ == "__main__":
    unittest.main()
