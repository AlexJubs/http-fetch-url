# My version of the cURL cli

I orginally wrote this for a coding assessment, but I found it pretty useful so I decided to modify a few things :)

This CLI tool takes an argument that is a full URL (--url). The tool will make an HTTP request to the URL and print the response directly to the console.

Another argument can be specified, N (--profile), which takes a positive integer. This tool makes that number of requests to the URL. When this parameter is specified (and > 1) we print:

- The number of requests
- The fastest time
- The slowest time
- The mean & median times
- The percentage of requests that succeeded
- Any error codes returned that weren't a success
- The size in bytes of the smallest response
- The size in bytes of the largest response

I use this to measure how fast I can perform http on different hosts. I choose C as it is a perfect systems programming language, very well optimized as a performant programming tool.

## whats next

I'm currently looking at building an algorithm to approximate the geographical location of a webserver 🧠