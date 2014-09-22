#ifndef HTTPCLIENT_H
#define  HTTPCLIENT_H
#include <iostream>
#include <string>
#include <vector>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;

class HttpClientNode : public CCNode
{
public:

	~HttpClientNode();
	static HttpClientNode  * sharedHttpClient();
	void addHttpClient(CCNode *parent);
	void requestUserId();
	void writeUserId(CCHttpClient *sender, CCHttpResponse *response);
	void loginReq();

	void eventSend(int eventId, string ex1 = "", string ex2 = "", string ex3 = "");
	string getPlatformInf();
private:
	HttpClientNode();
	static HttpClientNode *m_httpNode;

};
#endif