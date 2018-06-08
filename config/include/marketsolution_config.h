/*************************************************************************
	> File Name: market_solution_config.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Thu 06 Apr 2017 10:40:46 AM CST
 ************************************************************************/
#ifndef SOLUTION_CONFIG_H_
#define SOLUTION_CONFIG_H_
#include"xconfig.h"
#include<mutex>
#include<ostream>

namespace snetwork_xservice_xflagger {
/* marketsolutiongateway config
 * logger directory
 * logger filename
 * solution ID
 * rabbit mq: ip, port
 * */
class MSGXConfig : public XConfig {
	public:
		MSGXConfig();
		virtual ~MSGXConfig();

		MSGXConfig(const MSGXConfig& r) = delete;
		MSGXConfig& operator=(const MSGXConfig& r) = delete;
		MSGXConfig(MSGXConfig&& r) = delete;
		MSGXConfig& operator=(MSGXConfig&& r) = delete;

		virtual void Init(void) override;

	public:
		/*setters */
		void Screen(bool flag) {
			screen = flag;
		}
		bool Screen(void) const {
			return screen;
		}

		void Color(bool flag) {
			color = flag;
		}
		bool Color(void) const {
			return color;
		}

		void LoggerDir(const std::string& loggerDir) {
			this->loggerDir = loggerDir;
		}
		const std::string& LoggerDir(void) const {
			return loggerDir;
		}

		void LoggerFile(const std::string& loggerFile) {
			this->loggerFile = loggerFile;
		}
		const std::string& LoggerFile(void) {
			return loggerFile;
		}

		void SolutionID(const std::string& solutionID) {
			this->solutionID = solutionID;
		}
		const std::string& SolutionID(void) const {
			return solutionID;
		}

		void SolutionPort(int port) {
			this->solutionPort = port;
		}
		int SolutionPort(void) const {
			return solutionPort;
		}

		/* rabbitmq */
		void VHost(const std::string& vhost) {
			mqVHost = vhost;
		}
		const std::string& VHost(void) const {
			return mqVHost;
		}

		void Exchange(const std::string& exchang) {
			mqExchange = exchang;
		}
		const std::string& Exchange(void) const {
			return mqExchange;
		}

		void ExchangeGroup(const std::string& exchangeGroup) {
			mqExchangeGroup = exchangeGroup;
		}
		const std::string& ExchangeGroup(void) const {
			return mqExchangeGroup;
		}
		
		void Address(const std::string& address) {
			mqAddress = address;
		}
		const std::string& Address(void) const {
			return mqAddress;
		}

		void Port(const std::string& port) {
			mqPort = port;
		}
		const std::string& Port(void) const {
			return mqPort;
		}

		void User(const std::string& user) {
			mqUser = user;
		}
		const std::string& User(void) const {
			return mqUser;
		}

		void Password(const std::string& password) {
			mqPassword = password;
		}
		const std::string& Password(void) const {
			return mqPassword;
		}
		
		void MarketAgent(const std::string& agent) {
			marketAgent = agent;
		}
		
		const std::string& MarketAgent(void) {
			return marketAgent;
		}

		friend std::ostream& operator<<(std::ostream& os, const MSGXConfig& r);

	private:
		/* solution */
		bool screen;
		bool color;
		std::string loggerDir;
		std::string loggerFile;
		std::string solutionID;
		int solutionPort;

		/* rabbitmq */
		std::string mqVHost; 
		std::string mqExchange; 
		std::string mqExchangeGroup;
		std::string mqAddress;
		std::string mqPort;
		std::string mqUser;
		std::string mqPassword;
		
		/* market */
		std::string marketAgent;
};

class MSGXFlagger : public XFlagger {
	private:
		MSGXFlagger();
		virtual ~MSGXFlagger();

		MSGXFlagger(const MSGXFlagger& r) = delete;
		MSGXFlagger& operator=(const MSGXFlagger& r) = delete;
		MSGXFlagger(MSGXFlagger&& r) = delete;
		MSGXFlagger& operator=(MSGXFlagger&& r) = delete;

	public:
		virtual void FlagXConfig(XConfig* cfg) override ;
		virtual XConfig* GetConfig(void) const override {
			return m_config;
		}
		
	public:
		static MSGXFlagger* GetInstance(void);
		
	private:
		XConfig* m_config;
		static MSGXFlagger* m_instance;
		static std::mutex m_mutex;
};
}

#endif

