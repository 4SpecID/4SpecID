#pragma once
#include <curl/curl.h>
#include <string>
namespace ispecid{ 
    namespace network{
        using CurlShPtr = std::shared_ptr<CURLSH>;
        using CurlPtr = std::shared_ptr<CURL>;
        class HttpEngine{
            public:
            /*
            ParseResponse : (stringstream) -> Response
            The Response should be a pointer created and return by the ParseResponse callback
            However, it can encapsulate any type.
            */
            explicit HttpEngine(const std::string&);
            ~HttpEngine();
            CurlPtr initHandle();
            std::stringstream performRequest(CurlPtr, const std::string&);
            
            private:
            void createSharedHandle();
            int deleteSharedHandle();
            std::string m_host;
            CURLSH* m_shared_curl;
        };
    void prepare_network();
    std::string get_page(const char* url);
}}
