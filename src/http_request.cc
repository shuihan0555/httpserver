/*************************************************************************
  > File Name: http_request.cc
  > Author: xyz
  > Mail: xiao13149920@foxmail.com 
  > Created Time: Fri 03 Feb 2017 01:16:43 PM CST
 ************************************************************************/
#include<unistd.h>
#include<string.h>
#include<string>
#include<stdlib.h>
#include<rapidjson/document.h>
#include<rapidjson/writer.h>
#include<rapidjson/rapidjson.h>
#include<rapidjson/stringbuffer.h>

#include"http_request.h"
#include"tnode_adapter.h"
#include"xlogger.h"

namespace snetwork_xservice_market_solutiongateway {
/**** HttpRequest begin ****/
http_parser_settings HttpRequest::m_settings;
HttpRequest::HttpRequest() {
	m_settings.on_message_begin = OnMessageBegin;
	m_settings.on_headers_complete = OnHeadersComplete;
	m_settings.on_message_complete = OnMessageComplete;
	m_settings.on_url = OnUrl;
	m_settings.on_status = OnStatus;
	m_settings.on_header_field = OnHeaderField;
	m_settings.on_header_value = OnHeaderValue;
	m_settings.on_body = OnBody;
}

void HttpRequest::FreeWriteReq(uv_write_t* req) {
	write_req_t* wr = (write_req_t*) req;
	free(wr->buf.base);
	wr->buf.base = nullptr;
	//free(wr);
}

void HttpRequest::WriteCb(uv_write_t* req, int status) {
	if (status) {
		XERROR("Write error %s\n", uv_strerror(status));
	}

	FreeWriteReq(req);
}

/* callback of begin of parser*/
int HttpRequest::OnMessageBegin(http_parser* parser) {

	http_request_t* http_request = (http_request_t*)parser->data;
	http_request->header_lines = 0;
//what will happend
	return 0;
}

int HttpRequest::OnMessageComplete(http_parser *parser) {
	http_request_t* http_request = (http_request_t*)parser->data;

	return 0;
}

int HttpRequest::OnHeadersComplete(http_parser* parser) {

	http_request_t* http_request = (http_request_t*)parser->data;
	const char* method = http_method_str(http_method(parser->method));

	http_request->method = (char*)malloc(sizeof(method));
	strncpy(http_request->method, method, strlen(method));
	http_request->method [strlen(method)] = 0;
//return directory
	return 0;
}

int HttpRequest::OnUrl(http_parser* parser, const char* buf, size_t len) {
	http_request_t* http_request = (http_request_t*)parser->data;

	http_request->url = (char*)malloc(len+1);
	if (http_request->url == nullptr) {
		XERROR("malloc failure");

		return -1;
	}

	strncpy((char*) http_request->url, buf, len);
	http_request->url[len] = 0;

	return 0;
}

int HttpRequest::OnStatus(http_parser* parser, const char* buf, size_t len) {

	return 0;
}

int HttpRequest::OnHeaderField(http_parser* parser, const char* buf, size_t len) {
	http_request_t* http_request = (http_request_t*)parser->data;
	http_header_t* header = &http_request->headers[http_request->header_lines];

	header->field = (char*)malloc(len+1);
	if (header->field == nullptr) {
		XERROR("malloc failure");

		return -1;
	}

	memset((char*)header->field, 0, len);
	strncpy((char*) header->field, buf, len);
	//header->field[len] = 0;
	header->field_length = len;

	return 0;
}

int HttpRequest::OnHeaderValue(http_parser* parser, const char* buf, size_t len) {
	http_request_t* http_request = (http_request_t*)parser->data;
	http_header_t* header = &http_request->headers[http_request->header_lines];

	header->value_length = len;
	header->value = (char*)malloc(len+1);
	if (header->value == nullptr) {
		XERROR("malloc failure");

		return -1;
	}

	memset((char*)header->value, 0, len+1);
	strncpy((char*) header->value, buf, len);

	++http_request->header_lines;

	return 0;
}

int HttpRequest::OnBody(http_parser* parser, const char* buf, size_t len) {
	http_request_t* http_request = (http_request_t*)parser->data;

	http_request->body = (char*)malloc(len+1);
	if (http_request->body == nullptr) {
		XERROR("malloc failure");

		return -1;
	}
	strncpy(http_request->body, buf, len);
	http_request->body[len] = 0;

	return 0;
}

/**** HttpRequest end ****/
}

