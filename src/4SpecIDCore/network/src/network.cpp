#include "network/network.h"
#include <cstring>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

namespace ispecid
{
  namespace network
  {
    size_t BodyResponseWriteCallback(void *buffer, size_t size, size_t nitems,
                                     void *userdata)
    {
      std::stringstream *responseBodyStream =
          static_cast<std::stringstream *>(userdata);

      *responseBodyStream << static_cast<char *>(buffer);
      return nitems * size;
    }
    void StringStreamToLinesVector(std::stringstream &ss,
                                   std::vector<std::string> &lines)
    {
      for (std::string line; std::getline(ss, line);)
      {
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());
        lines.emplace_back(line);
      }
    }

    HttpEngine::HttpEngine(const std::string &host) : m_host(host)
    {
      curl_global_init(CURL_GLOBAL_ALL);
      createSharedHandle();
    }

    HttpEngine::~HttpEngine()
    {
      deleteSharedHandle();
    }

    void HttpEngine::createSharedHandle()
    {
      if (!m_shared_curl)
      {
        // shared handel is caching connection, so we can reuse it with
        // many curl_easy handles
        m_shared_curl = curl_share_init();
        curl_share_setopt(m_shared_curl, CURLSHOPT_SHARE, CURL_LOCK_DATA_CONNECT);
        curl_share_setopt(m_shared_curl, CURLSHOPT_SHARE,
                          CURL_LOCK_DATA_SSL_SESSION);
        curl_share_setopt(m_shared_curl, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
      }
    }

    int HttpEngine::deleteSharedHandle()
    {
      int errorCode = -1;
      if (m_shared_curl)
      {
        errorCode = static_cast<int>(curl_share_cleanup(m_shared_curl));
        m_shared_curl = nullptr;
      }
      return errorCode;
    }

    CurlPtr HttpEngine::initHandle()
    {
      CURL *curl_handle = curl_easy_init();
      createSharedHandle();
      curl_easy_setopt(curl_handle, CURLOPT_SHARE, m_shared_curl);
      curl_easy_setopt(curl_handle, CURLOPT_URL, m_host.c_str());
      curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
      curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);

      // return shared_ptr with auto cleanup function
      CurlPtr ptr(curl_handle, curl_easy_cleanup);
      return ptr;
    }

    std::stringstream HttpEngine::performRequest(CurlPtr handle, const std::string &url)
    {
      std::stringstream stream;
      std::string full_url = m_host + url;

      curl_easy_setopt(handle.get(), CURLOPT_URL, full_url.c_str());
      curl_easy_setopt(handle.get(), CURLOPT_USERAGENT, "libcurl-agent/1.0");
      curl_easy_setopt(handle.get(), CURLOPT_WRITEFUNCTION, BodyResponseWriteCallback);
      curl_easy_setopt(handle.get(), CURLOPT_WRITEDATA, &stream);
      CURLcode res = curl_easy_perform(handle.get());

      if (res != CURLE_OK)
      {
        deleteSharedHandle();
        return stream;
      }

      return stream;
    }

    static size_t writeCallback(void *contents, size_t size, size_t nmemb,
                                void *userp)
    {
      size_t realsize = size * nmemb;
      std::string *mem = static_cast<std::string *>(userp);
      mem->append(static_cast<char *>(contents), realsize);
      return realsize;
    }

    std::string get_page(const char *url)
    {
      CURL *curl_handle = curl_easy_init();
      std::string data;
      if (curl_handle != nullptr)
      {
        CURLcode res;
        curl_easy_setopt(curl_handle, CURLOPT_URL, url);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA,
                         static_cast<void *>(&data));
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        res = curl_easy_perform(curl_handle);
        if (res != CURLE_OK)
        {
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));
          fprintf(stderr, "Url failed: %s\n", url);
          curl_easy_cleanup(curl_handle);
          throw std::runtime_error("Can't connect to www.boldsystems.org to access "
                                   "nearest neighbors details. Please check your "
                                   "internet connection and  / or try again later");
        }
      }
      else
      {
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
