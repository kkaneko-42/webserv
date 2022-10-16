package main

import (
	"io/ioutil"
	"net/http"
	"testing"
)

func TestServerName(t *testing.T) {
	type in struct {
		url  string
		host string
	}
	type want struct {
		fileName string
	}
	tests := []struct {
		in   in
		want want
	}{
		// 4242
		{in{"http://web:4242/", ""}, want{"./html/hoge/index.html"}},
		{in{"http://web:4242/", "hoge.com"}, want{"./html/hoge/index.html"}},
		{in{"http://web:4242/", "fuga.com"}, want{"./html/fuga/index.html"}},
		{in{"http://web:4242/", "piyo.com"}, want{"./html/piyo/index.html"}},
		// 4243
		{in{"http://web:4243/", ""}, want{"./html/hoge4243/index.html"}},
		{in{"http://web:4243/", "hoge.com"}, want{"./html/hoge4243/index.html"}},
		{in{"http://web:4243/", "fuga.com"}, want{"./html/hoge4243/index.html"}},
		{in{"http://web:4243/", "piyo.com"}, want{"./html/hoge4243/index.html"}},
		{in{"http://web:4243/", "foo.com"}, want{"./html/foo/index.html"}},
	}

	for _, tt := range tests {
		t.Run("test", func(t *testing.T) {
			req, err := http.NewRequest(http.MethodGet, tt.in.url, nil)
			if err != nil {
				t.Fatal(err)
			}
			if tt.in.host != "" {
				req.Host = tt.in.host
			}

			resp, err := http.DefaultClient.Do(req)
			if err != nil {
				t.Fatal(err)
			}

			gotBytes, err := ioutil.ReadAll(resp.Body)
			if err != nil {
				t.Fatal(err)
			}
			gotBody := string(gotBytes)

			wantBytes, err := ioutil.ReadFile(tt.want.fileName)
			if err != nil {
				t.Fatal(err)
			}
			wantBody := string(wantBytes)

			if gotBody != wantBody {
				t.Errorf("%s (Host:%s) want %s",
					tt.in.url,
					tt.in.host,
					tt.want.fileName,
				)
			}
		})
	}

}
