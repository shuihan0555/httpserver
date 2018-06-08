/*************************************************************************
	> File Name: tnode_adapter.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Wed 08 Feb 2017 11:17:29 AM CST
 ************************************************************************/
#include<mutex>
#include<new>
#include<string.h>
#include<type_traits>
#include"tnode_adapter.h"
#include"define.h"
#include<rapidjson/document.h>
#include<rapidjson/writer.h>
#include<rapidjson/rapidjson.h>
#include<rapidjson/stringbuffer.h>
#include "marketsolution_config.h"
#include"xlogger.h"

TNodeAministrator* TNodeAministrator::m_instance = nullptr;
std::mutex TNodeAministrator::m_mutex;
//extern ThreadPool* g_threadPool;

using namespace snetwork_xservice_xflagger;
TNodeAministrator* TNodeAministrator::GetInstance(void) {
	if (m_instance == nullptr) {
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_instance == nullptr) {
			m_instance = new TNodeAministrator();
		}
	}

	return m_instance;
}

TNodeAministrator::TNodeAministrator() { 

	snetwork_xservice_tnode::TNodeConfigV0 config;

	MSGXConfig* msgxconfig = dynamic_cast<MSGXConfig*>(MSGXFlagger::GetInstance()->GetConfig());
	config.VHost(msgxconfig->VHost()).Exchange(msgxconfig->Exchange()).ExchangeGroup(msgxconfig->ExchangeGroup()).Address(msgxconfig->Address()).Port(msgxconfig->Port()).User(msgxconfig->User()).Password(msgxconfig->Password());

	snetwork_xservice_tnode::TNode tnode;
	tnode.Config(config);
	tnode.Bind(1);
	tnode.Run();


    m_agent = new snetwork_signalengine_marketdata::MarketDataAgent;
	m_agent->Init(tnode, msgxconfig->MarketAgent());
}

TNodeAministrator::~TNodeAministrator() {
  	if (m_agent != nullptr) {
		delete m_agent;
		m_agent = nullptr;
	}

	for (auto& it : m_channel) {
		delete it;
		it = nullptr;
	}
}

snetwork_xservice_tnode::TNodeChannel TNodeAministrator::GetChannel(void) {
	snetwork_xservice_tnode::TNode tnode;
	tnode.Bind(1);

	return tnode.GetChannel();
}

void TNodeAministrator::PublishGroup(const char* group, const char* event, const char* msg, unsigned int len, unsigned short tag) {
	snetwork_xservice_tnode::TNode tnode;
	tnode.Bind(1);

	snetwork_xservice_tnode::TNodeChannel channel = tnode.GetChannel();
	char* publish_msg = channel.AllocDataBuff(len);
	memcpy(publish_msg, msg, len-1);
	publish_msg[len-1] = 0;
	channel.PublishGroup(group, event, publish_msg, len, tag);
#if defined(DEBUG)
	XINFO("publish msg:<%s>, len=%d\n", msg, len);
#endif
}


/* TNodeAministrator end ****/
