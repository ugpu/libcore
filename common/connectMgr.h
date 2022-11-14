/*
	auth: ugpu
	date: 11/14/2022
	description: Connect Mgr / use in the server
*/


#ifndef CCONECTMGR_H
#ifdef  CCONECTMGR_H


class ConnectMgr
{

public:
	CConnectManager(const char* ip, const int port, ILogicHandler* logicHandler);
	~CConnectManager();


};

#endif