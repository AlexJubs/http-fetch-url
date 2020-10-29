// this program was written by Alex Jabbour on Oct 28th, 2020
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define help_message "Usage of ./url_cli: \n --profile <int> \n		A positive integer, number of http requests (default 1) \n --url <url> \n 		URL to fetch data from, as a string (default `https://alex-worker.alexjubs.workers.dev/`) \nNote: if --profile is specified, a URL must also be specified \n Also, if the URL passed in is the same as the '/links' error, it will not work"
#define URL "https://alex-worker.alexjubs.workers.dev/links"

void fetch_url(char * url, int dump_to_console);
unsigned long rcv_bites=0;

float mean(double runtimes[], int n) {
	float sum = 0;

	for(int i = 0; i < n; i++) {
		sum+=runtimes[i];
	}
	return sum/(float)n;
}

float min(double runtimes[], int n) {
	float res = runtimes[0];
	for (int i=0; i < n; i++) {
		if (runtimes[i] <= res) res = runtimes[i];
	}
	return res;
}

float max(double runtimes[], int n) {
	float res = runtimes[0];
	for (int i=0; i < n; i++) {
		if (runtimes[i] >= res) res = runtimes[i];
	}
	return res;
}

int main( int argc, char** argv ) {
	// the default values
	char * url = "https://alex-worker.alexjubs.workers.dev/links";
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
		if (strcmp(argv[1], "--profile") == 0) {
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
	
	if (profile == 1) {
		fetch_url(url, 1);
	}

	else if (profile > 1) {
		// measuring the time taken for the request
		clock_t begin = clock();
		double runtimes[profile];

		// send N requests, and send metrics
		for (int i=0; i < profile; i++) {
			// measure the time taken
			clock_t begin = clock();
			fetch_url(url, 0);
			clock_t end = clock();
			runtimes[i] = (double)(end - begin) / CLOCKS_PER_SEC;
		}

		// collect metrics
		printf("Number of requests: %d\n", profile);
		printf("Fastest time: %f \n", min(runtimes, profile));
		printf("Slowest time: %f \n", max(runtimes, profile));
		printf("Mean time: %f \n", mean(runtimes, profile));
		printf("The size in bites of smallest response: %lu\n", rcv_bites);
		printf("The size in bites of largest response: %lu\n", rcv_bites);
	}

	return 0;
}

struct string {
  char *ptr;
  size_t len;
};

void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  rcv_bites += sizeof(nmemb);

  return size*nmemb;
}

void fetch_url(char * url, int dump_to_console)
{
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl) {
    struct string s;
    init_string(&s);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    res = curl_easy_perform(curl);

    if (dump_to_console) {
    	printf("%s\n", s.ptr);
    }
    
    free(s.ptr);

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
}
