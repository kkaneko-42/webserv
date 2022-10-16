from urllib import request
import requests
import unittest

IP="localhost"
PAGES_DIR="./pages"

class ErrorPagesTest(unittest.TestCase):

    def test_default_error_page(self):
        url = "http://" + IP + ":4242"

        # POST is not allowed(405)
        r = requests.post(url)
        self.__assert_response(r, 405, PAGES_DIR + "/default.html")

        # resource is not found(404)
        r = requests.get(url + "/non-existent_file.html")
        self.__assert_response(r, 404, PAGES_DIR + "/default.html")

    def test_setting_one_status(self):
        url = "http://" + IP + ":4243"

        # POST is not allowed(405)
        r = requests.post(url)
        self.__assert_response(r, 405, PAGES_DIR + "/405_only.html")

        # resource is not found(404)
        r = requests.get(url + "/non-existent_file.html")
        self.__assert_response(r, 404, PAGES_DIR + "/inmemory_error_page.html")

    def test_setting_multiple_status(self):
        url = "http://" + IP + ":4244"

        # POST is not allowed(405)
        r = requests.post(url)
        self.__assert_response(r, 405, PAGES_DIR + "/404_or_405.html")

        # resource is not found(404)
        r = requests.get(url + "/non-existent_file.html")
        self.__assert_response(r, 404, PAGES_DIR + "/404_or_405.html")

        # bad request(400),
        # because header field name has invalid char '@'
        h = {"Connection@": "close"}
        r = requests.get(url + "/index.html", headers=h)
        self.__assert_response(r, 400, PAGES_DIR + "/inmemory_error_page.html")

    def __assert_response(self, resp, status, contents_filepath):
        self.assertEqual(status, resp.status_code)
        with open(contents_filepath) as f:
            expected = f.read()
            self.assertEqual(expected, resp.text)

if __name__ == "__main__":
    unittest.main()
