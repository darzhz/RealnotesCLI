#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cJSON.h"

// Structure to hold the fetched data and its size
struct MemoryStruct {
    char *data;
    size_t size;
};

// Write callback function to store received data
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t total_size = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->data, mem->size + total_size + 1);
    if (ptr == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return 0;
    }

    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, total_size);
    mem->size += total_size;
    mem->data[mem->size] = '\0';

    return total_size;
}
void displayArt(){
    char *art2 = "\n|  _ \\| ____|  / \\  | |   | \\ | |/ _ |_   _| ____/ ___|  \\ \\   / |___ /   \n| |_) |  _|   / _ \\ | |   |  \\| | | | || | |  _| \\___ \\   \\ \\ / /  |_ \\  \n|  _ <| |___ / ___ \\| |___| |\\  | |_| || | | |___ ___) |   \\ V /  ___) | \n|_| \\_|_____/_/   \\_|_____|_| \\_|\\___/ |_| |_____|____/     \\_/  |____/  \n";
    char *art = "welcome to ＲＥＡＬＮＯＴＥＳ Ｖ３";
    printf("%s",art2);
    printf("%s\n",art);
}
void fetch(char *url){
    CURL *curl;
    CURLcode res;
    struct MemoryStruct mem;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        // URL to fetch JSON data from
        printf("fetching url: %s",url);

        // Set the initial memory size to 0
        mem.data = NULL;
        mem.size = 0;

        // Loop to resend request if the server doesn't respond
        int retry_count = 3;
        while (retry_count > 0) {
            // Set the URL to fetch
            curl_easy_setopt(curl, CURLOPT_URL, url);
            //setiting useragent as glitch doesnt forward
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.64.1");

            // Set the callback function to write received data
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &mem);

            // Perform the request
            res = curl_easy_perform(curl);
            // Check for errors and handle server response codes
            long response_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            // if (res != CURLE_OK || response_code != 200) {
            //     fprintf(stderr, "Failed to fetch data. Retrying...\n");
            //     retry_count--;
            //     continue;
            // }

            // Display the fetched JSON data
           // printf("JSON Data:\n%s\n", mem.data);
            cJSON *root = cJSON_Parse(mem.data);
            if (root == NULL) {
                fprintf(stderr, "Failed to parse JSON data.(invalid code)\n");
            } else {
                //Extract required fields
                cJSON *url = cJSON_GetObjectItem(root, "data")->child->next;
                cJSON *text = cJSON_GetObjectItem(root, "data")->child;
                cJSON *last_updated = cJSON_GetObjectItem(root, "data")->child->next->next;

                // Display formatted output
                printf("\nurl: realnotes.glitch.me/%s\n", url->valuestring);
                printf("data: \n --------------------Data starts Here---------------------\n\n%s\n\n--------------------Data ends Here-----------------------\n", text->valuestring);
                printf("last_updated: %s\n", last_updated->valuestring);

                // Cleanup cJSON
                cJSON_Delete(root);
            }

            // Clean up and exit the loop
            curl_easy_cleanup(curl);
            break;
        }

        // Free allocated memory
        free(mem.data);
    }

    // Cleanup global libcurl resources
    curl_global_cleanup();

}
int main()
{
    char url[256];
    int ch = 1;
    while(ch==1){
            displayArt();
            printf("SELECT ANY OPTION :\n\t\tpress 1 to Search\n\t\tpress any other number to quit\n");
            scanf("%d",&ch);
            if(ch == 1){
                printf("Enter a URL: ");
                scanf("%255s", url);
                char fullUrl[512];
                snprintf(fullUrl, sizeof(fullUrl), "https://testreal.glitch.me/api/%s", url);
                fetch(fullUrl);
            }else{
                printf("\033[2J");
                break;
            }
    }
    //fetch("https://testreal.glitch.me/api/hello");
    return 0;
}

