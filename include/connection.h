/*************************************************************************
  > File Name: connection.h
  > Author: xyz
  > Mail: xiao13149920@foxmail.com 
  > Created Time: Fri 03 Feb 2017 12:00:28 PM CST
 ************************************************************************/
#ifndef SOLUTIONGATEWAY_CONNECTION_H_
#define SOLUTIONGATEWAY_CONNECTION_H_

#include"define.h"

namespace snetwork_xservice_market_solutiongateway {
class HttpRequest;
class UVTcpServer {
	public:
		UVTcpServer();
		~UVTcpServer();

		UVTcpServer(const UVTcpServer& r) = delete;
		UVTcpServer& operator=(const UVTcpServer& r) = delete;
		UVTcpServer(UVTcpServer&& r) = delete;
		UVTcpServer& operator=(UVTcpServer&& r) = delete;

	public:
		int Run(void);
		const uv_loop_t* GetLoop(void) const {
			return m_loop;
		}
		uv_tcp_t GetTcpSever(void) const {
			return m_server;
		}
		struct sockaddr_in GetSockaddr(void) const {
			return m_addr;
		}

	protected:
		static void OnNewConnection(uv_stream_t* server, int status);
		static void EchoRead(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf);
		static void ParserJson(http_request_t* http_request); 

		static void OnClose(uv_handle_s* handle);
		int Init(void);
		static void AllocBuffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
	public:
		static void FreeWriteReq(uv_write_t* req);
		static void WriteCb(uv_write_t* req, int status);
	private:
		static uv_loop_t*	m_loop;
		static uv_tcp_t	m_server;
		struct sockaddr_in	m_addr;
		static HttpRequest* m_settings;
};

}

#endif

