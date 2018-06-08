/*************************************************************************
	> File Name: define.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Fri 03 Feb 2017 11:33:28 AM CST
 ************************************************************************/
#ifndef SOLUTIONGATEWAY_DEFINE_H_
#define SOLUTIONGATEWAY_DEFINE_H_
//// for HttpSolutionGateway xlogger
//#define SOLUTIONGATEWAY_LOGGER		"HttpSolutionGateway"
//#define SOLUTIONGATEWAY_LOGGER_DIR	"./tmp/logger/HttpSolutionGateway"
#include "xlogger.h"
// for uv_tcp connection
#define SOLUTIONGATEWAY_BACKLOG	128

#define MAX_HTTP_HEADERS 20
#define URL_MARK_DATA	"/md"

#if defined(KEEP_ALIVE_TEST)
#define SOLUTIONGATEWAY_PORT	5001
#define RESPONSE \
	"Connection: Keep-Alive\r\n" \
	"Content-Length: "
#else
#define SOLUTIONGATEWAY_PORT	5001
#define RESPONSE \
	"Content-Length: "
#endif

// for http parser
#include<uv.h>
#include <time.h>
#include "http_parser.h"
#include <chrono>
typedef struct {
	const char* field;
	const char* value;
	//char* field;
	//char* value;
	size_t field_length;
	size_t value_length;
} http_header_t;

typedef struct {
	uv_write_t req;

	uv_stream_t stream;

	http_parser parser;
	char* url;
	char* method;
	int header_lines;
	http_header_t headers[MAX_HTTP_HEADERS];
	char* body;
	struct sockaddr_in peer_name;
#if defined(FUNCTIONTIME)
	unsigned long long accept;
	unsigned long long read;
	unsigned long long parse_end;
	unsigned long long query_begin;
	unsigned long long query_end;
	unsigned long long end;
#endif
} http_request_t;

typedef struct {
	uv_write_t req;
	uv_buf_t buf;
}write_req_t;

// tool
template<typename T>
inline void FREE(T** t) {
	free(*t);
	*t = nullptr;
}

template<typename T>
inline void DELETE(T** t) {
	delete(*t);
	*t = nullptr;
}


inline long GetMicroSecond(void) {
	return std::chrono::system_clock::now().time_since_epoch()/std::chrono::microseconds(1);
}

#if defined (__i386__)
static __inline__ unsigned long long GetClockCycleCount(void)
{
        unsigned long long int x;
        __asm__ volatile("rdtsc":"=A"(x));
        return x;
}
#elif defined (__x86_64__)
static __inline__ unsigned long long GetClockCycleCount(void)
{
	unsigned hi,lo;
	__asm__ volatile("rdtsc":"=a"(lo),"=d"(hi));
	return ((unsigned long long)lo)|(((unsigned long long)hi)<<32);
}
#endif

#if defined(FUNCTIONTIME)
#define  CPUFREQUENCY (3.20*1000000000)

static void PRINT(const http_request_t* http_request) {
	XINFO("Http Request Time(read - accept): ",
		(http_request->read - http_request->accept)*1.0/ CPUFREQUENCY);
	XINFO("Http Parse Time(parse_end - read): ",
		(http_request->parse_end - http_request->read)*1.0 / CPUFREQUENCY);
	XINFO("Market Query Time(query_end - query_begin): ",
		(http_request->query_end - http_request->query_begin)*1.0 / CPUFREQUENCY);
	XINFO("Market Respone Time(end - query_end): ",
		(http_request->end - http_request->query_end)*1.0 /CPUFREQUENCY);
}
#endif

#endif //SOLUTIONGATEWAY_DEFINE_H_

