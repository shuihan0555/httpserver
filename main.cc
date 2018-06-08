/*************************************************************************
	> File Name: main.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Fri 03 Feb 2017 11:26:30 AM CST
 ************************************************************************/
#include"connection.h"
#include"xlogger.h"
#include"marketsolution_config.h"

using namespace snetwork_xservice_market_solutiongateway;
using namespace snetwork_xservice_xflagger;

int main(int argc, char* argv[]) {
#if 1
	XConfig* cfg = new MSGXConfig;
	XFlagger* flagger = MSGXFlagger::GetInstance();
	flagger->ParseCommandLine(&argc, &argv, false);
	flagger->FlagXConfig(cfg);
	MSGXConfig* msgxconfig = dynamic_cast<MSGXConfig*>(flagger->GetConfig());
	if (msgxconfig == nullptr) {
		fprintf(stderr, "config error");

		exit(1);
	}
	std::cout<<*msgxconfig<<std::endl;
	/* logger setting */

	if (!msgxconfig->Screen()) {
		//XLogger::GetObject(msgxconfig->LoggerFile().c_str())->StdErr(true)->Colour(msgxconfig->Color())->Dir(msgxconfig->LoggerDir());
		std::string fileName (msgxconfig->LoggerFile());
		fileName += msgxconfig->SolutionID();
		(*XLogger::GetObject(fileName.c_str())).StdErr(true).Colour(msgxconfig->Color()).Dir(msgxconfig->LoggerDir());
	} else {
		(*XLogger::GetObject(msgxconfig->LoggerFile().c_str())).StdErr(false).Colour(false).Dir(msgxconfig->LoggerDir());
	}
#endif

	UVTcpServer* uv = new UVTcpServer();
	int r = uv->Run();
	if(r) {
		XERROR("uv_run return %s\n", uv_strerror(r));		
	}

	delete uv;

	FREEXLOGGER;

	return 0;
}
