import requests
import unittest

LOG_DIR="./log"
ENV_DIR="./env"
URL="http://localhost:4242"

class GetTest(unittest.TestCase):
    # ファイルを直接GETする
    def test_file(self):
        r = requests.get(URL + "/get/index.html")

        self.assertEqual(200, r.status_code)
        with open(ENV_DIR + "/index.html") as f:
            expected = f.read()
            self.assertEqual(expected, r.text)

    # index.html != "", autoindex == trueなるディレクトリをgetする
    def test_dir_index_and_autoindex(self):
        r = requests.get(URL + "/get/")

        self.assertEqual(200, r.status_code)
        with open(ENV_DIR + "/index.html") as f:
            expected = f.read()
            self.assertEqual(expected, r.text)

    # index.htmlが設定されたdirをGETする
    def test_dir_index(self):
        r = requests.get(URL + "/get/dir/")

        self.assertEqual(200, r.status_code)
        with open(ENV_DIR + "/dir/index.html") as f:
            expected = f.read()
            self.assertEqual(expected, r.text)

    # autoindexがonのdirをGETする
    def test_autoindex(self):
        r = requests.get(URL + "/get/dir/listing/")

        self.assertEqual(200, r.status_code)

    # autoindexもindex.htmlも設定されていないdirをGETする
    def test_error_dir(self):
        r = requests.get(URL + "/get/dir/no_index/")

        self.assertEqual(200, r.status_code)
        with open(ENV_DIR + "/dir/index.html") as f:
            expected = f.read()
            self.assertEqual(expected, r.text)

    # 存在しないファイルをGET
    def test_non_existent_file(self):
        r = requests.get(URL + "/get/hogwbuoeugbowuef.txt")
        self.assertEqual(404, r.status_code)

    # GETが認められていないlocationをGET
    def test_not_allowed(self):
        r = requests.get(URL + "/deny_all/")

        self.assertEqual(405, r.status_code)
        with open(ENV_DIR + "/40x.html") as f:
            expected = f.read()
            self.assertEqual(expected, r.text)

if __name__ == "__main__":
    unittest.main()
