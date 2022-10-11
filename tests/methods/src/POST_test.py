import requests
import unittest
import os
import shutil

LOG_DIR="./log"
ENV_DIR="./env"
URL="http://localhost:4242"
UPLOAD_DIR = ENV_DIR + "/upload"

class PostTest(unittest.TestCase):
    def __init__(self, *args, **kwargs):
        super(PostTest, self).__init__(*args, **kwargs)
        # 作成したファイルの削除
        if os.path.isfile(UPLOAD_DIR + "/chmod000.txt"):
            os.chmod(UPLOAD_DIR + "/chmod000.txt", 0o755)
        shutil.rmtree(UPLOAD_DIR)
        os.mkdir(UPLOAD_DIR)
        os.mkdir(UPLOAD_DIR + "/dir")
        os.mkdir(UPLOAD_DIR + "/hoge")
        with open(UPLOAD_DIR + "/chmod000.txt", 'w') as f:
            f.write("text\n")
        os.chmod(UPLOAD_DIR + "/chmod000.txt", 0o000)
    
    # def __del__(self):
    #     if os.path.isfile(UPLOAD_DIR + "/chmod000.txt"):
    #            os.chmod(UPLOAD_DIR + "/chmod000.txt", 0o755)

    # ファイルを作成する
    def test_post_file(self):
        expected = "aaaaa"
        r = requests.post(URL + "/post/create.txt", data=expected)

        self.assertEqual(201, r.status_code)
        with open(UPLOAD_DIR + "/create.txt") as f:
            actual = f.read()
            self.assertEqual(expected, actual)
    
    # upload_fileの中のディレクトリにpost
    def test_post_file_in_dir(self):
        expected = "aaaaa"
        r = requests.post(URL + "/post/hoge/create.txt", data=expected)

        self.assertEqual(201, r.status_code)
        with open(UPLOAD_DIR + "/hoge/create.txt") as f:
            actual = f.read()
            self.assertEqual(expected, actual)

    # 既存のファイルに追記
    def test_append(self):
        # 追記されるファイルの作成
        with open(UPLOAD_DIR + "/append.txt", 'w') as f:
            f.write("old text\n")

        expected = "hogehoge\n"
        r = requests.post(URL + "/post/append.txt", data=expected)
        self.assertEqual(200, r.status_code)
        with open(UPLOAD_DIR + "/append.txt") as f:
            actual = f.read()
            self.assertEqual("old text\nhogehoge\n", actual)

    # ディレクトリにpost
    def test_post_dir(self):
        # ディレクトリとファイル名が同一
        r = requests.post(URL + "/post/dir")
        self.assertEqual(404, r.status_code)
        # ディレクトリ自体にpost
        r = requests.post(URL + "/post/dir/")
        self.assertEqual(404, r.status_code)

    # 権限がないファイルに追記
    def test_forbidden_append(self):
        r = requests.post(URL + "/post/chmod000.txt")
        self.assertIn(r.status_code, [403, 404])

    # allow_file_uploadがoffのlocationにpost
    def test_file_upload_not_allowed(self):
        r = requests.post(URL + "/post/permission_error/ko.txt")
        self.assertIn(r.status_code, [403, 404])

    # folderが指定されていないlocationにpost
    def test_none_save_folder(self):
        filename = "/none_save_folder.txt"
        if os.path.isfile(ENV_DIR + filename):
            os.remove(ENV_DIR + filename)
        expected = "hogehoge fugafuga\n"

        r = requests.post(URL + "/post/folder_error" + filename, data=expected)
        self.assertEqual(201, r.status_code)
        with open(ENV_DIR + filename) as f:
            actual = f.read()
            self.assertEqual(expected, actual)

    # 空のcontentをpost
    def test_empty_post(self):
        r = requests.post(URL + "/post/empty.txt", data="")

        self.assertEqual(201, r.status_code)
        with open(UPLOAD_DIR + "/empty.txt") as f:
            actual = f.read()
            self.assertEqual("", actual)

    # POSTが許可されていないlocation
    def test_method_not_allowed(self):
        r = requests.post(URL + "/deny_all/deny.txt", data="forbidden")
        self.assertEqual(405, r.status_code)

if __name__ == "__main__":
    unittest.main()
