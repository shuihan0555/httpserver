/*************************************************************************
  > File Name: connection.cc
  > Author: xyz
  > Mail: xiao13149920@foxmail.com 
  > Created Time: Fri 03 Feb 2017 12:00:04 PM CST
 ************************************************************************/
#include<unistd.h>
#include<string.h>
#include<string>
#include<stdlib.h>
#include <arpa/inet.h> /*for ntohl*/
#include<rapidjson/document.h>
#include<rapidjson/writer.h>
#include<rapidjson/rapidjson.h>
#include<rapidjson/stringbuffer.h>
#include <stdio.h>
#include <bits/stdio2.h>
#include <define.h>
#include"connection.h"
#include"http_request.h"
#include"tnode_adapter.h"
#include "xlogger.h"
#include "marketsolution_config.h"
#include "marketsolution_config.h"
using  namespace snetwork_xservice_xflagger;

namespace snetwork_xservice_market_solutiongateway {
/* Tcp Server begin */
uv_loop_t* UVTcpServer::m_loop = nullptr;
HttpRequest* UVTcpServer::m_settings = nullptr;
uv_tcp_t UVTcpServer::m_server;

UVTcpServer::UVTcpServer() {
	Init();
}

UVTcpServer::~UVTcpServer() {
	if (m_settings != nullptr) {
		delete m_settings;
		m_settings = nullptr;
	}

	int r = uv_loop_close(m_loop);
	if (r == UV_EBUSY)
	{
		XERROR("uv_loop_close error %s\n", uv_strerror(r));	/* code */
	} else {
		free(m_loop);
	}
}

int UVTcpServer::Init(void) {
	m_settings = new HttpRequest();
	m_loop = uv_default_loop();
	uv_tcp_init(m_loop, &m_server);

	MSGXConfig* msgxConfig = dynamic_cast<MSGXConfig*>(MSGXFlagger::GetInstance()->GetConfig());

	uv_ip4_addr("0.0.0.0", msgxConfig->SolutionPort(), &m_addr);
	uv_tcp_bind(&m_server, (const struct sockaddr*)&m_addr, 0);

	//uv_tcp_keepalive((uv_tcp_t*)&m_server, 1, 60);
	//uv_tcp_simultaneous_accepts((uv_tcp_t*)&m_server, 1);

	int r = uv_listen((uv_stream_t*)&m_server, SOLUTIONGATEWAY_BACKLOG, OnNewConnection);
	if (r) {
		XERROR("Listen error %s\n", uv_strerror(r));

		return 1;
	}

	return 0;
}

/*http connect begin */
void UVTcpServer::OnNewConnection(uv_stream_t* server, int status) {
	if (status < 0) {
		XERROR("New connection error %s\n", uv_strerror(status));

		return;
	}

#if defined(DEBUG)
	XINFO("on_new_connection\n");
#endif

	http_request_t* http_request = (http_request_t*)malloc(sizeof(http_request_t));
	if (http_request == nullptr) {
		XERROR("malloc fail");

		return;
	}
	memset(http_request, 0, sizeof(http_request_t));

	uv_tcp_init(server->loop, (uv_tcp_t*)&http_request->stream);
	/* set references so we can use our http_request in http_parser and libuv */
	http_request->stream.data = http_request;
	http_request->parser.data = http_request;

	if (uv_accept(server, (uv_stream_t*)&http_request->stream) == 0) {

#if defined(FUNCTIONTIME)
		http_request->accept = GetClockCycleCount();
#endif
		http_parser_init(&http_request->parser, HTTP_REQUEST);
		uv_read_start((uv_stream_t*)&http_request->stream, UVTcpServer::AllocBuffer, UVTcpServer::EchoRead);
#if defined(DEBUG)
	XINFO("uv_read_start end\n");
#endif
	} else {
		uv_close((uv_handle_t*)&http_request->stream, UVTcpServer::OnClose);
#if defined(DEBUG)
	XINFO("uv_close end\n");
#endif
	}
#if defined(DEBUG)
	XINFO("uv_*** end\n");
#endif
}


void UVTcpServer::EchoRead(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf) {
	http_request_t* http_request = (http_request_t*)handle->data;
	if (nread < 0) {
		if (nread != UV_EOF) {
			XERROR("Read error %s\n", uv_err_name(nread));
		}
		free(buf->base);
		/*when client close positive, here will be call too*/
		uv_close((uv_handle_t*)handle, UVTcpServer::OnClose);

		return;
	}


#if defined(FUNCTIONTIME)
	http_request->read = GetClockCycleCount();
#endif
	size_t parsed = http_parser_execute(&http_request->parser, m_settings->GetSettings(), buf->base, nread);

	if (parsed < nread) {
		XERROR("parse error\n");
		uv_close((uv_handle_t*)handle, UVTcpServer::OnClose);
		//free(&http_request);
	} else {
		if (strncmp(http_request->method, "POST", 4) != 0) {
			uv_close((uv_handle_t*)handle, UVTcpServer::OnClose);
		} else {


#if defined(FUNCTIONTIME)
		http_request->parse_end = GetClockCycleCount();
#endif
		ParserJson(http_request);
		//g_threadPool->enqueue(UVTcpServer::ParserJson, http_request);
		}
	}

	free(buf->base);
}

void UVTcpServer::ParserJson(http_request_t* http_request) {
	/*parser http body */
	rapidjson::Document d;
	http_request_t* request = http_request;
	if (strncmp(request->url, URL_MARK_DATA, strlen(URL_MARK_DATA)) == 0) {
		/*mark data(real time or history data)*/

		d.Parse(http_request->body);
		if (d.HasParseError()) {
			XERROR("parser json error\n");

			return;
		}

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("symbols")) == d.MemberEnd()) || !it->value.IsArray()) {
#if defined(DEBUG)
            XINFO("field no exit or type error");
#endif

			return;
		}
		//const rapidjson::GenericArray::Array & a = it->value.GetArray();//type error
		//const rapidjson::Value& a = d["symbols"];
		rapidjson::SizeType  size = it->value.GetArray().Size();
		if (size <= 0) {
			return;
		}

		std::string content = "{\"symbols\":[";
		std::string symbols = "symbols";

#if defined(FUNCTIONTIME)
		http_request->query_begin = GetClockCycleCount();
#endif
		/* getmark data for library */
		for (rapidjson::SizeType i=0; i<size; ++i) {
			std::string price;
			TNodeAministrator::GetInstance()->GetMarketDataAgent()->
					GetMarketData(it->value.GetArray()[i].GetString(), price);
			content += price;
			content += ",";
		}

#if defined(FUNCTIONTIME)
		http_request->query_end = GetClockCycleCount();
#endif
		content[content.length()-1] = ']';
		content += "}";
#if 1
		/*respone http request*/
		write_req_t* req = (write_req_t*)malloc(sizeof(write_req_t));
		std::string headers;
		for (int i=0; i<request->header_lines; ++i) {
			if (strncmp(request->headers[i].field, "Content-Type", request->headers[i].field_length) == 0) {
				headers += std::string(request->headers[i].field, request->headers[i].field_length);
				headers += ": ";
				headers += std::string(request->headers[i].value, request->headers[i].value_length);
				headers +="\r\n";
				break;
			}
		}
		char* response = NULL;
		asprintf(&response, "%s%s%s%zu%s%s", "HTTP/1.1 200 OK\r\n", headers.c_str(), RESPONSE, content.length(), "\r\n\r\n", content.c_str());
		//asprintf(&response, "%s %s", RESPONSE, "hello world\n");
		req->buf = uv_buf_init(response, strlen(response));

		//uv_write((uv_write_t*)&http_request->req, (uv_stream_t*)&http_request->stream, &req->buf, 1, NULL);
		uv_write((uv_write_t*)&req->req, (uv_stream_t*)&http_request->stream, &req->buf, 1, WriteCb);
#endif
#if 0
		/*respone http request*/
		write_req_t* req = (write_req_t*)malloc(sizeof(write_req_t) * 5);
		char** response = (char**)malloc(sizeof(char) * 5);
		for(int i = 0 ; i < 5; i++) {
		std::string headers;
		for (int j=0; j<request->header_lines; ++j) {
			if (strncmp(request->headers[i].field, "Content-Type", request->headers[i].field_length) == 0) {
				headers += std::string(request->headers[i].field, request->headers[i].field_length);
				headers += ": ";
				headers += std::string(request->headers[i].value, request->headers[i].value_length);
				headers +="\r\n";
				break;
			}
		}

		asprintf(&response[i], "%s%s%s%zu%s%s", "HTTP/1.1 200 OK\r\n", headers.c_str(), RESPONSE, content.length(), "\r\n\r\n", content.c_str());
		//asprintf(&response, "%s %s", RESPONSE, "hello world\n");

				req[i].buf = uv_buf_init(response[i], strlen(response[i]));

		uv_write((uv_write_t*)&req[i].req, (uv_stream_t*)&http_request->stream, &req[i].buf, 1,  NULL);
		}

#endif

#if defined(FUNCTIONTIME)
		http_request->end = GetClockCycleCount();
		PRINT(http_request);
#endif
        /*server close positive*/
        //uv_close((uv_handle_t*)&http_request->stream, UVTcpServer::OnClose);
	}
}
/*http connect end */


void UVTcpServer::AllocBuffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
	buf->base = (char*) malloc(suggested_size);
	memset(buf->base, 0, suggested_size);
	buf->len = suggested_size;
}

void UVTcpServer::FreeWriteReq(uv_write_t* req) {
	write_req_t* wr = (write_req_t*) req;
	free(wr->buf.base);
	wr->buf.base = nullptr;
	free(wr);
}

void UVTcpServer::WriteCb(uv_write_t* req, int status) {
	if (status) {
		XERROR("Write error %s\n", uv_strerror(status));
	}

	FreeWriteReq(req);
}

void UVTcpServer::OnClose(uv_handle_s* handle) {
	http_request_t* http_request = (http_request_t*)handle->data;
		FREE(&http_request->url);
		FREE(&http_request->method);
		FREE(&http_request->body);
		for (int i=0; i<http_request->header_lines; ++i) {
			FREE((void**)&http_request->headers[i].field);
			FREE((void**)&http_request->headers[i].value);
		}
		FREE(&http_request);
#if defined(DEBUG)
	XINFO("disconnection\n");
#endif
}

int UVTcpServer::Run(void) {
	return uv_run(m_loop, UV_RUN_DEFAULT);
}

}
