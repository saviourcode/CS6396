// #include "Publisher.h"
#include "../cpp-httplib/httplib.h"
#include <iostream>
#include<string>
using namespace std;
int main()
{
  auto scheme_host_port = "http://localhost:1234";
  
  httplib::Client cli(scheme_host_port);
  for(;;){
  auto data = "{ \"hi\" : \"12\" }";

  if (auto res = cli.Post("/pub", data, "text/plain"))
  {
    cout << res->status << endl;
    cout << res->get_header_value("Content-Type") << endl;
    cout << res->body << endl;
  }
  else
  {
    cout << "error code: " << res.error() << std::endl;
  }
  }
}