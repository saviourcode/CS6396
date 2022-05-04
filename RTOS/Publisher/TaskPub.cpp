// #include "Publisher.h"
#include "../cpp-httplib/httplib.h"
#include <iostream>
#include <string>
using namespace std;

void postdata(httplib::Client &client, std::string data) 
{
  while (true)
  {
    auto res = client.Post("/pub", data, "text/plain");
    if (res)
    {
      std::cout << res->status << std::endl;
      std::cout << res->body << std::endl;
    }
    else
    {
      std::cout << res.error() << std::endl;
    }
    sleep(2);
  }
}
int main()
{
  auto scheme_host_port = "http://localhost:1234";
  httplib::Client cli1(scheme_host_port);
  httplib::Client cli2(scheme_host_port);
  httplib::Client cli3(scheme_host_port);
  // create threads to run the clients concurrently
  std::thread t1(postdata, std::ref(cli1), "{\"topic\":\"topic1\",\"message\":\"t1\"}");
  std::thread t2(postdata, std::ref(cli2), "{\"topic\":\"topic1\",\"message\":\"t2\"}");
  std::thread t3(postdata, std::ref(cli3), "{\"topic\":\"topic1\",\"message\":\"t3\"}");
  t1.join();
  t2.join();
  t3.join();
  return 0;
}