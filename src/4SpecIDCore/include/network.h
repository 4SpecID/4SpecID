#ifndef NETWORK_H
#define NETWORK_H
#include <curl/curl.h>
#include <string>
namespace ispecid{ namespace network{
    void prepare_network();
    std::string get_page(const char* url);
}}
#endif
