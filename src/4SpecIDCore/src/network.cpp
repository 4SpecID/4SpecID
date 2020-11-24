#include "network.h"
#include <cstring>
#include <stdexcept>
#include <string>

namespace ispecid {
namespace network {

void prepare_network() { curl_global_init(CURL_GLOBAL_ALL); }

static size_t writeCallback(void *contents, size_t size, size_t nmemb,
                            void *userp) {
  size_t realsize = size * nmemb;
  std::string *mem = static_cast<std::string *>(userp);
  mem->append(static_cast<char *>(contents), realsize);
  return realsize;
}

std::string get_page(const char *url) {
  CURL *curl_handle = curl_easy_init();
  std::string data;
  if (curl_handle != nullptr) {
    CURLcode res;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA,
                     static_cast<void *>(&data));
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    res = curl_easy_perform(curl_handle);
    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
      fprintf(stderr, "Url failed: %s\n", url);
      curl_easy_cleanup(curl_handle);
      throw std::runtime_error("Can't connect to www.boldsystems.org to access "
                               "nearest neighbors details. Please check your "
                               "internet connection and  / or try again later");
    }
  } else {
    curl_easy_cleanup(curl_handle);
    throw std::runtime_error("Can't connect to www.boldsystems.org to access "
                             "nearest neighbors details. Please check your "
                             "internet connection and  / or try again later");
  }
  curl_easy_cleanup(curl_handle);
  return data;
}

} // namespace network
} // namespace ispecid
