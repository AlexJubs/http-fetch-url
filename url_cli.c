// this program was written by Alex Jabbour on Oct 28th, 2020
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

#define help_message "Usage of ./url_cli: \n --profile <int> \n		A positive integer, number of http requests (default 1) \n --url <url> \n 		URL to fetch data from, as a string (default `https://alex-worker.alexjubs.workers.dev/`)"
#define URL "https://alex-worker.alexjubs.workers.dev/"

void fetch_url(char * url);

int main( int argc, char** argv ) {
	// the default values
	char * url = "https://alex-worker.alexjubs.workers.dev/";
	int profile = 1;

	// parse the CLI args
	if (argc == 2) {
		if (strcmp(argv[1], "--help") == 0) {
			printf("%s\n", help_message);
			return 1;
		}
	}
	// case where there is only 1 input which must be the url
	else if (argc >= 3) {
		if (strcmp(argv[1], "--url") == 0) {
			// store the url in the url char
			url = strdup(argv[2]);
		}
		else if (strcmp(argv[1], "--profile") == 0) {
			// store the url in the url char
			profile = *strdup(argv[2]) - '0';
		}
		// parsing url and profile at the same time
		if (argc == 5) {
			if (strcmp(argv[3], "--url") == 0) {
				// store the url in the url char
				url = strdup(argv[4]);
			}
			else if (strcmp(argv[3], "--profile") == 0) {
				// store the url in the url char
				profile = atoi(strdup(argv[4]));
			}
		}
	}
	// we can't have an odd number of inputs (argv-1)
	else if (argc%2 == 0) printf("%s\n", help_message);

	// just in case input was not parsed correctly
	if (argc > 1 && strcmp(url, URL) == 0) {
		printf("%s\n", help_message);
		return 0;
	}

	printf("profile is %d\n", profile);
	printf("url is %s\n", url);

	//fetch_url(url);

	return 0;
}

void fetch_url(char * url) {
	CURL *curl;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if( curl ) {
		curl_easy_setopt(curl, CURLOPT_URL, url );

		res = curl_easy_perform(curl);

		if( res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}

	curl_easy_cleanup(curl);
	}
}