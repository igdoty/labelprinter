#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
//#include <rapidjson/rapidjson.h>
//#include <rapidjson/reader.h>
//#include <rapidjson/document.h>
//#include <rapidjson/stringbuffer.h>
//#include <rapidjson/writer.h>
#include <iostream>
#include <string>
#include <string.h>
#include <curl/curl.h>


using namespace std;

size_t write_to_string(void *ptr, size_t size, size_t nmemb, std::string stream)
{
    size_t realsize = size * nmemb;
    std::string temp(static_cast<const char*>(ptr), realsize);
    stream.append(temp);
    return realsize;
}

int main(){
    
    CURL *curl;
    CURLcode res;
    
    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* get a curl handle */
    curl = curl_easy_init();
    if(curl) {
        Json::Value values;
        Json::Reader reader;
        
        
        /* First set the URL that is about to receive our POST. */
        curl_easy_setopt(curl, CURLOPT_URL, "http://admin.touchofmodern.local:3000/shipments/scan_label");
        
        curl_easy_setopt(curl, CURLOPT_USERPWD,"igdoty@gmail.com:password");
        
        /* Now specify the POST data */
        string response;
        
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "id=123123");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);


        
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        
        /* Check for errors */
        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }
        
        reader.parse(response, values);
        bool success = reader.parse(response, values,false);
        
        cout<<response<<endl;
        if(!success){
            cout<<"Parse Error"<<endl;
            return 1;
        }
        
        cout<<values.toStyledString()<<endl;
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    
    return 0;
}