#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPBasicCredentials.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/reader.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <iostream>
#include <string>
#include <string.h>
#include <curl/curl.h>


using namespace Poco::Net;
using namespace Poco;
using namespace std;

size_t write_to_string(void *ptr, size_t size, size_t nmemb, std::string stream)
{
    size_t realsize = size * nmemb;
    std::string temp(static_cast<const char*>(ptr), realsize);
    stream.append(temp);
    return realsize;
}

int main(){
//    try {
        //Using POCO
        
//        // prepare session
//        const URI uri("http://admin.touchofmodern.local:3000");
//        HTTPClientSession session("admin.touchofmodern.com");
////        HTTPClientSession session(uri.getHost(), uri.getPort());
//        
////        HTTPRequest req(HTTPRequest::HTTP_POST, "/shipments/scan_label");
//        HTTPRequest req(HTTPRequest::HTTP_POST, "/shipments/scan_label", HTTPMessage::HTTP_1_0);
//        req.setContentType("application/x-www-form-urlencoded\r\n");
//        req.setKeepAlive(true);
//
//        HTTPBasicCredentials cred("igdoty@gmail.com", "password");
//        cred.authenticate(req);
//        
//        
//        std::string reqBody("id=123123");
//        req.setContentLength( reqBody.length() );
//        session.sendRequest(req) << reqBody;
//        
//        req.write(std::cout);
//        
//        HTTPResponse res;
//        std::istream& iStr = session.receiveResponse(res);  // get the response from server
//        StreamCopier::copyStream(iStr, cout);
//        
//
//
//        
//    } catch (Exception &ex) {
//        cerr<< ex.displayText() <<endl;
//        return -1;
//    }
    
    CURL *curl;
    CURLcode res;
    
    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* get a curl handle */
    curl = curl_easy_init();
    if(curl) {
        /* First set the URL that is about to receive our POST. */
        curl_easy_setopt(curl, CURLOPT_URL, "http://admin.touchofmodern.local:3000/shipments/scan_label");
        
        curl_easy_setopt(curl, CURLOPT_USERPWD,"igdoty@gmail.com:password");
        
        /* Now specify the POST data */
        string response;
        
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "id=123123");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        
        const char* ch = response.c_str();
        
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        
        cout<<response<<endl;
        rapidjson::Document document;
        if(document.Parse<0>(ch).HasParseError()){
            cout<<"Parse Error"<<endl;
            return 1;
        }
        
        if (document["label"].IsObject()) {
            rapidjson::StringBuffer sb;
            rapidjson::Writer<rapidjson::StringBuffer> writer( sb );
            document[ "label" ].Accept( writer );
            std::cout << sb.GetString() << std::endl;
        }        
//        printf("%s\n", document["label"].GetString());
           curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return 0;
}