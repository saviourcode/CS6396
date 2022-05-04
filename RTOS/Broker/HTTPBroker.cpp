#include "../cpp-httplib/httplib.h"
#include<string>
#include<iostream>
using namespace httplib;
std::string dump_headers(const Headers &headers) {
  std::string s;
  char buf[BUFSIZ];

  for (auto it = headers.begin(); it != headers.end(); ++it) {
    const auto &x = *it;
    snprintf(buf, sizeof(buf), "%s: %s\n", x.first.c_str(), x.second.c_str());
    s += buf;
  }

  return s;
}

std::string log(const Request &req, const Response &res) {
  std::string s;
  char buf[BUFSIZ];

  s += "================================\n";

  snprintf(buf, sizeof(buf), "%s %s %s", req.method.c_str(),
           req.version.c_str(), req.path.c_str());
  s += buf;

  std::string query;
  for (auto it = req.params.begin(); it != req.params.end(); ++it) {
    const auto &x = *it;
    snprintf(buf, sizeof(buf), "%c%s=%s",
             (it == req.params.begin()) ? '?' : '&', x.first.c_str(),
             x.second.c_str());
    query += buf;
  }
  snprintf(buf, sizeof(buf), "%s\n", query.c_str());
  s += buf;

  s += dump_headers(req.headers);

  s += "--------------------------------\n";

  snprintf(buf, sizeof(buf), "%d %s\n", res.status, res.version.c_str());
  s += buf;
  s += dump_headers(res.headers);
  s += "\n";

  if (!res.body.empty()) { s += res.body; }

  s += "\n";

  return s;
}
int main()
{
    Server svr;

    svr.Get("/hi", [](const Request &req, Response &res)
            { res.set_content("Hello World!", "text/plain"); });

    svr.Get("/body-header-param", [](const Request &req, Response &res)
            {
    if (req.has_header("Content-Length")) {
      auto val = req.get_header_value("Content-Length");
    }
    if (req.has_param("key")) {
      auto val = req.get_param_value("key");
    }
    res.set_content(req.body, "text/plain"); });

    svr.Get("/stop", [&](const Request &req, Response &res)
            { svr.stop(); });

    svr.Post("/pub", [](const Request &req, Response &res) {
        auto body = dump_headers(req.headers);
        std::cout << req.body << std::endl;
        res.set_content("Successfully Published the data!", "text/plain");
    });

    svr.set_logger([](const Request &req, const Response &res)
                   { printf("%s", log(req, res).c_str()); });
    svr.listen("localhost", 1234);
}