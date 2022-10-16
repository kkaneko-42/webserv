from urllib import request
import requests
import unittest

IP="localhost"
PAGES_DIR="./pages"

def assert_response(method, url, status, filepath):
    r = method(url)
    

class ErrorPagesTest(unittest.TestCase):
    def __assert_response(self, resp, status, contents_filepath):
        self.assertEqual(status, resp.status_code)
        with open(contents_filepath) as f:
            expected = f.read()
            self.assertEqual(expected, resp.text)

    def test_default_error_page(self):
        url = "http://" + IP + ":4242"
        # method is not allowed
        r = requests.delete(url)
        self.__assert_response(r, 405, PAGES_DIR + "/default.html")
        # resource is not found
        r = requests.get(url + "/hogehoge.html")
        self.__assert_response(r, 404, PAGES_DIR + "/default.html")

    def test_setting_one_status(self):
        url = "http://" + IP + ":4243"
        # get method is not allowed
        r = requests.get()
        self.assertEqual(405, r.status_code)
