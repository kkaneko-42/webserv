import requests
import unittest

LOG_DIR="./log/"
ENV_DIR="./env/"
URL="http://localhost:4242"

class GetTest(unittest.TestCase):
    def test_file_get(self):
        r = requests.get(URL + "/get/index.html")

        # test status code
        self.assertEqual(200, r.status_code)
        # test response body
        with open(ENV_DIR + "index.html") as f:
            expected = f.read()
            self.assertEqual(expected, r.text)

if __name__ == "__main__":
    unittest.main()
