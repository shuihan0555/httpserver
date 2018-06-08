/*************************************************************************
	> File Name: solution_config.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Thu 06 Apr 2017 11:55:06 AM CST
 ************************************************************************/
#include"marketsolution_config.h"
#include<gflags/gflags.h>

namespace snetwork_xservice_xflagger {

DEFINE_bool(screen, false, "printf message on terminator");
DEFINE_bool(color, true, "message color");
DEFINE_string(loggerDir,"../logs", "logger directory");
//DEFINE_string(loggerDir,"SolutionGateway", "logger directory");
DEFINE_string(loggerFile,"marketsolutiongateway", "logger name");
DEFINE_string(solutionID,"2", "Market Solution Gateway ID");
DEFINE_int32(solutionPort, 5001, "http connection port");

/* rabbitmq */
DEFINE_string(mqVHost, "/demo", "rabbitmq vhost");
DEFINE_string(mqExchange, "t_node", "rabbitmq exchange");
DEFINE_string(mqExchangeGroup, "t_node_grp", "rabbitmq exchange group");
DEFINE_string(mqAddress, "localhost", "rabbitmq IP address");
DEFINE_string(mqPort, "5672", "rabbitmq port");
DEFINE_string(mqUser, "demo", "rabbitmq user");
DEFINE_string(mqPassword, "demo", "rabbitmq password");

/* marketagent */
DEFINE_string(marketAgent, "test", "marketagent name");


MSGXConfig::MSGXConfig() {

}

MSGXConfig::~MSGXConfig() {
}

void MSGXConfig::Init(void) {
	/* solution */
	Screen(FLAGS_screen);
	Color(FLAGS_color);
	LoggerDir(FLAGS_loggerDir);
	LoggerFile(FLAGS_loggerFile);
	SolutionID(FLAGS_solutionID);
	SolutionPort(FLAGS_solutionPort);

	/* rabbitmq */
	VHost(FLAGS_mqVHost);
	Exchange(FLAGS_mqExchange);
	ExchangeGroup(FLAGS_mqExchangeGroup);
	Address(FLAGS_mqAddress);
	Port(FLAGS_mqPort);
	User(FLAGS_mqUser);
	Password(FLAGS_mqPassword);
	
	/* marketagent */
	MarketAgent(FLAGS_marketAgent);
}


std::ostream& operator<<(std::ostream&os, const MSGXConfig& r) {
	os<<"screen:\t"<<r.screen
		<<"\n"<<"color:\t"<<r.color
		<<"\n"<<"loggerDir:\t"<<r.loggerDir
		<<"\n"<<"loggerFile:\t"<<r.loggerFile
		<<"\n"<<"solutionID:\t"<<r.solutionID
		<<"\n"<<"solutionPort:\t"<<r.solutionPort
		<<"\n"<<"mqVHost:\t"<<r.mqVHost
		<<"\n"<<"mqExchange:\t"<<r.mqExchange
		<<"\n"<<"mqExchangeGroup:\t"<<r.mqExchangeGroup
		<<"\n"<<"mqAddress:\t"<<r.mqAddress
		<<"\n"<<"mqPort:\t"<<r.mqPort
		<<"\n"<<"mqUser:\t"<<r.mqUser
		<<"\n"<<"mqPassword:\t"<<r.mqPassword
		<<"\n"<<"marketAgent:\t"<<r.marketAgent
		<<"\n";

	return os;
}

/**MSGXFlagger begin**/
MSGXFlagger* MSGXFlagger::m_instance = nullptr;
std::mutex MSGXFlagger::m_mutex;

MSGXFlagger::MSGXFlagger() : m_config(nullptr) {
}

MSGXFlagger* MSGXFlagger::GetInstance(void) {
	if (m_instance == nullptr) {
		std::lock_guard<std::mutex> lk(m_mutex);
		if (m_instance == nullptr) {
			m_instance = new MSGXFlagger;
		}
	}

	return m_instance;
}

MSGXFlagger::~MSGXFlagger() {
	if (m_config != nullptr) {
		delete m_config;
		m_config = nullptr;
	}
}

void MSGXFlagger::FlagXConfig(XConfig* cfg) {
	do {
		if (m_config == nullptr) {
			m_config = cfg;
			cfg->Init();

			break;
		}
	}while(false);
}
/**MSGXFlagger end**/

}
