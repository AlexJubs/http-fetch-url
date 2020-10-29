#include <stdio.h>
#include <curl/curl.h>
#include <string.h>

#define help_message "Usage of ./url_cli: \n --profile int \n		A positive integer (default 1) \n --url string \n 		URL to fetch data from (default `https://alex-worker.alexjubs.workers.dev/`)"

int main( int argc, char** argv ) {
	char * url;

	// parse the CLI args
	if (argc == 2) {
		if (strcmp(argv[1], "--help") == 0) {
			printf("%s\n", help_message);
			return 1;
		}
	}
	else if (argc == 3) {
		if (strcmp(argv[1], "--url") == 0) {
			// store the url in the url char
			url = strdup(argv[2]);
		}
		else {
			printf("Error: must provide a url to parse: `--url <url>`\n");
		}
	}
	else if (argc == 5) {

	}
	else printf("%s\n", help_message);

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