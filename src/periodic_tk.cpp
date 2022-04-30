// C Headers
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <curl/curl.h>
// Litmus RT Header
#include <litmus.h>
// C++ Headers
#include <iostream>
#include <string>
using namespace std;

// #define PERIOD ms2ns(10)
// #define DEADLINE ms2ns(10)
// #define EXEC_COST ms2ns(1)
#define PERIOD ms2ns(100)
#define DEADLINE ms2ns(100)
#define EXEC_COST ms2ns(10)
#define CALL(exp)                                     \
    do                                                \
    {                                                 \
        int ret;                                      \
        ret = exp;                                    \
        if (ret != 0)                                 \
            fprintf(stderr, "%s failed: %m\n", #exp); \
        else                                          \
            fprintf(stderr, "%s ok.\n", #exp);        \
    } while (0)

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string *data)
{
    data->append((char *)ptr, size * nmemb);
    return size * nmemb;
}

class Curl
{
public:
    Curl()
    {
        curl = curl_easy_init();
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/stock/AMZN");
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.47.0");
            curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
            curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_string);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_string);

            // curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, response_code);
            // curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, elapsed);
            // curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, url);
        }
    }

    void cleanup()
    {
        if (curl)
            curl_easy_cleanup(curl);
        curl = NULL;
    }

    void fetchData()
    {
        response_string = "";
        curl_easy_perform(curl);
    }

    std::string getResponse()
    {
        fetchData();
        return response_string;
    }

private:
    CURL *curl;
    std::string response_string;
    std::string header_string;
    char *url;
    long response_code;
    double elapsed;
};

int i = 0;

int job(Curl &c)
{
    cout << c.getResponse() << endl;
    i++;
    if (i >= 10)
        return 1;
    return 0;
}

int main()
{
    int do_exit;
    struct rt_task params;
    CALL(init_litmus());
    init_rt_task_param(&params);
    params.exec_cost = EXEC_COST;
    params.period = PERIOD;
    params.relative_deadline = DEADLINE;
    CALL(set_rt_task_param(gettid(), &params));

    Curl c;
    CALL(task_mode(LITMUS_RT_TASK));
    CALL(wait_for_ts_release());

    // Do real-time stuff
    do
    {
        sleep_next_period();
        do_exit = job(c);
        // printf("%d\n", do_exit);
    } while (!do_exit);

    CALL(task_mode(BACKGROUND_TASK));

    return 0;
}
