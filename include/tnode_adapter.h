/*************************************************************************
	> File Name: tnode_adapter.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Mon 06 Feb 2017 06:04:58 PM CST
 ************************************************************************/
#ifndef SOLUTIONGATEWAY_TNODE_ADAPTER_H_
#define SOLUTIONGATEWAY_TNODE_ADAPTER_H_
#include<atomic>
#include<vector>
#include<mutex>

#include"tnode.h"
#include"tnodeconfig_v0.h"

#include"marketdata.h"
#include"marketdataagent.h"

#include"define.h"

/*TNodeAministrator begin ****/
class TNodeAministrator {
	public:
		~TNodeAministrator();
	private:
		TNodeAministrator();

		TNodeAministrator(const TNodeAministrator& r) = delete;
		TNodeAministrator& operator=(const TNodeAministrator& r) = delete;
		TNodeAministrator(TNodeAministrator&& r) = delete;
		TNodeAministrator& operator=(TNodeAministrator&& r) = delete;

	public:
		static TNodeAministrator* GetInstance(void);

	public:
		snetwork_signalengine_marketdata::MarketDataAgent* GetMarketDataAgent(void) const {
			return m_agent;
		}

		snetwork_xservice_tnode::TNodeChannel GetChannel(void);
		void PublishGroup(const char* group, const char* event, const char* msg, unsigned int len, unsigned short tag);

	private:
		static TNodeAministrator* m_instance;
		static std::mutex m_mutex;
		std::vector<snetwork_xservice_tnode::TNodeChannel*> m_channel;
		snetwork_signalengine_marketdata::MarketDataAgent* m_agent;
};
/*TNodeAministrator end ****/

#endif //SOLUTIONGATEWAY_TNODE_ADAPTER_H_

