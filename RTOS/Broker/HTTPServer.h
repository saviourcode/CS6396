#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "../Dependencies/cpp-httplib/httplib.h"
#include "../Dependencies/json/single_include/nlohmann/json.hpp"
#include "./ThreadSafeMap.h"

using namespace httplib;
using json = nlohmann::json;

std::string dump_headers(const Headers &headers)
{
  std::string s;
  char buf[BUFSIZ];

  for (auto it = headers.begin(); it != headers.end(); ++it)
  {
    const auto &x = *it;
    snprintf(buf, sizeof(buf), "%s: %s\n", x.first.c_str(), x.second.c_str());
    s += buf;
  }

  return s;
}

std::string log(const Request &req, const Response &res)
{
  std::string s;
  char buf[BUFSIZ];

  s += "================================\n";

  snprintf(buf, sizeof(buf), "%s %s %s", req.method.c_str(),
           req.version.c_str(), req.path.c_str());
  s += buf;

  std::string query;
  for (auto it = req.params.begin(); it != req.params.end(); ++it)
  {
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

  if (!res.body.empty())
  {
    s += res.body;
  }

  s += "\n";

  return s;
}

void http_server(ThreadSafeMap<std::string, std::string> &topics)
{
  Server svr;

  svr.set_logger([](const Request &req, const Response &res)
                 { printf("%s", log(req, res).c_str()); });

  svr.Get("/hi", [](const Request &req, Response &res)
          { res.set_content("Hello World!", "text/plain"); });

  svr.Get("/stop", [&](const Request &req, Response &res)
          { svr.stop(); });

  svr.Post("/pub",
           [&topics](const Request &req, Response &res)
           {
        auto headers = dump_headers(req.headers);
        json j = json::parse(req.body);
        std::string topic = j["topic"];
        std::string message = j["message"];
        std::cout << "topic: " << topic << std::endl;
        std::cout << "message: " << message << std::endl;
        topics.insert(topic, message);
        res.set_content("Successfully Published the data!", "text/plain"); });

  svr.listen("localhost", 1234);
}

void print_map(ThreadSafeMap<std::string, std::string> &topics)
{
  while (true)
  {
    std::cout << "========================================================" << std::endl;
    auto val = topics.get("topic1");
    for (auto x : val)
    {
      std::cout << x << std::endl;
    }
    std::cout << "========================================================" << std::endl;
  }
}