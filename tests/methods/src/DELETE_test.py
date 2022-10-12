import requests
import unittest
import os
import shutil

ENV_DIR="./env"
URL="http://localhost:4242"
DELETE_DIR = ENV_DIR + "/delete"

class DeleteTest(unittest.TestCase):
    def __init__(self, *args, **kwargs):
        super(DeleteTest, self).__init__(*args, **kwargs)
        if os.path.isfile(DELETE_DIR + "/chmod000.txt"):
            os.chmod(DELETE_DIR + "/chmod000.txt", 0o755)
        if os.path.exists(DELETE_DIR):
            shutil.rmtree(DELETE_DIR)
        os.mkdir(DELETE_DIR)
        os.mkdir(DELETE_DIR + "/dir")
        with open(DELETE_DIR + "/chmod000.txt", 'w') as f:
            f.write("hoge\n")
        os.chmod(DELETE_DIR + "/chmod000.txt", 0o000)

    # ファイルを直接delete
    def test_file(self):
        with open(DELETE_DIR + "/ok.txt", 'w') as f:
            f.write("body\n")
        r = requests.delete(URL + "/delete/ok.txt")
        self.assertEqual(204, r.status_code)
        self.assertEqual(False, os.path.exists(DELETE_DIR + "/ok.txt"))

    # ディレクトリをdelete
    def test_dir(self):
        r = requests.delete(URL + "/delete/dir")
        self.assertEqual(404, r.status_code)
        self.assertEqual(True, os.path.exists(DELETE_DIR + "/dir"))

    # TODO: 下のやり方だと消せる。rm chmod000.txtもできたので、なにか工夫が必要
    # ファイル権限がない
    # def test_file_permission(self):
    #     r = requests.delete(URL + "/delete/chmod000.txt")
    #     self.assertIn(r.status_code, [403, 404])
    #     self.assertEqual(True, os.path.exists(DELETE_DIR + "/chmod000.txt"))

    # method not allowed
    def test_method_not_allowed(self):
        with open(ENV_DIR + "/tmp_delete.txt", 'w') as f:
            f.write("body\n")
        r = requests.delete(URL + "/deny_all/tmp_delete.txt")
        self.assertEqual(405, r.status_code)
        self.assertEqual(True, os.path.exists(ENV_DIR + "/tmp_delete.txt"))

if __name__ == "__main__":
    unittest.main()
