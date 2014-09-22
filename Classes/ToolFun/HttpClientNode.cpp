#include "HttpClientNode.h"
#include "ToolsFun.h"
#include "JniCall.h"
const char *gVersionCode = "mm_chtzj_320X480_1.0.0";
#define HTTP_HEADER ""

HttpClientNode *HttpClientNode::m_httpNode = NULL;
HttpClientNode::HttpClientNode()
{

}

HttpClientNode::~HttpClientNode()
{
	LD("HttpClientNode::~HttpClientNode()");
}

HttpClientNode  * HttpClientNode::sharedHttpClient()
{
	if(!m_httpNode)
	{
		m_httpNode = new HttpClientNode();
		m_httpNode->autorelease();
		m_httpNode->retain();
	}

	return m_httpNode;
}

void HttpClientNode::requestUserId()
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	return;
#endif
	int userId = CCUserDefault::sharedUserDefault()->getIntegerForKey("lemiUserId", 0);

	if(userId == 0)
	{
		CCHttpRequest* request = new CCHttpRequest();
		request->setUrl("http://114.215.129.247:11101/create");
		request->setRequestType(CCHttpRequest::kHttpPost);
		request->setResponseCallback(this, httpresponse_selector(HttpClientNode::writeUserId));
		// write the post data
		string bufferStr = getPlatformInf(); // including \0, the strings after \0 should not be cut in response
		request->setRequestData(bufferStr.c_str(), bufferStr.size());

		request->setTag("POST Binary test");
		CCHttpClient::getInstance()->send(request);
		request->release();
	}
	else
	{
		//  sendRegid();
		loginReq();
	}
}

void HttpClientNode::writeUserId(CCHttpClient *sender, CCHttpResponse *response)
{
	if(!response)
	{
		return;
	}

	std::string tmpString;
	std::vector<char> *buffer = response->getResponseData();

	for(unsigned int i = 0; i < buffer->size(); i++)
	{
		char tmp[5];
		sprintf(tmp, "%c", (*buffer)[i]);
		tmpString.append(tmp);
	}

	int userId = atoi(tmpString.c_str());
	CCUserDefault::sharedUserDefault()->setIntegerForKey("lemiUserId", userId);
	LD("writeUserId");
	//sendRegid();
}

void HttpClientNode::loginReq()
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	return;
#endif

	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl("http://114.215.129.247:11101/login");
	request->setRequestType(CCHttpRequest::kHttpPost);
	//request->setResponseCallback(this, NULL);
	// write the post data
	int userId = CCUserDefault::sharedUserDefault()->getIntegerForKey("lemiUserId");
	char buffer[33];
	sprintf(buffer, "%d", userId);
	string bufferSend;
	bufferSend.append("data={\"")
	.append("roleId")
	.append("\":\"")
	.append(string(buffer))
	.append("\"}");
	LD("bufferSend = %s", bufferSend.c_str());
	request->setRequestData(bufferSend.c_str(), bufferSend.size());

	request->setTag("POST Binary test");
	CCHttpClient::getInstance()->send(request);
	request->release();
}

void HttpClientNode::eventSend(int eventId, string ex1, string ex2, string ex3)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	return;
#endif
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl("http://114.215.129.247:11101/event");
	request->setRequestType(CCHttpRequest::kHttpPost);
	//request->setResponseCallback(this, NULL);
	// write the post data
	int userId = CCUserDefault::sharedUserDefault()->getIntegerForKey("lemiUserId");
	char buffer[33];
	sprintf(buffer, "%d", userId);
	string bufferSend;
	char eventIdStr[33];
	sprintf(eventIdStr, "%d", eventId);
	bufferSend.append("data={\"")
	.append("roleId")
	.append("\":\"")
	.append(string(buffer))
	.append("\",\"")
	.append("eventId")
	.append("\":\"")
	.append(eventIdStr)
	.append("\",\"")
	.append("ext1")
	.append("\":\"")
	.append(ex1.c_str())
	.append("\",\"")
	.append("ext2")
	.append("\":\"")
	.append(ex2.c_str())
	.append("\",\"")
	.append("ext3")
	.append("\":\"")
	.append(ex3.c_str())
	.append("\"}");
	//LD("bufferSend = %s",bufferSend.c_str());
	CCLog("bufferSend = %s\n", bufferSend.c_str());
	request->setRequestData(bufferSend.c_str(), bufferSend.size());

	request->setTag("POST Binary test");
	CCHttpClient::getInstance()->send(request);
	request->release();
}

string HttpClientNode::getPlatformInf()
{
	string buffer;
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	buffer.append("data={\"")
	.append("deviceId")
	.append("\":\"")
	.append("win32_test")
	.append("\",\"")
	.append("version")
	.append("\":\"")
	.append("win7_64")
	.append("\"}");
#else

	CCLog("%s\t%d\n", __FUNCTION__, __LINE__);
	string deveiceInf = CCUserDefault::sharedUserDefault()->getStringForKey("deviceId", "default");

	if(deveiceInf == "default")
	{
		deveiceInf = JniCall::sharedJniCall()->getDeviceInf();
		CCUserDefault::sharedUserDefault()->setStringForKey("deviceId", deveiceInf.c_str());
	}

	string versionInf = CCUserDefault::sharedUserDefault()->getStringForKey("versionInf", "default");

	if(versionInf == "default")
	{
		versionInf = JniCall::sharedJniCall()->getSystemVersion();
		CCUserDefault::sharedUserDefault()->setStringForKey("versionInf", versionInf.c_str());
	}

/*
	string gVersionCode = CCUserDefault::sharedUserDefault()->getStringForKey("gVersionCode", "default");

	if(gVersionCode == "default")
	{
		gVersionCode = JniCall::sharedJniCall()->getGameVersionCode();
		CCUserDefault::sharedUserDefault()->setStringForKey("gVersionCode", gVersionCode.c_str());
	}*/

	string gPhone = CCUserDefault::sharedUserDefault()->getStringForKey("gPhone", "default");

	if(gPhone == "default")
	{
		gPhone = JniCall::sharedJniCall()->getPhone();
		CCUserDefault::sharedUserDefault()->setStringForKey("gPhone", gPhone.c_str());
	}

	string gCorp = CCUserDefault::sharedUserDefault()->getStringForKey("gCorp", "default");

	if(gCorp == "default")
	{
		gCorp = JniCall::sharedJniCall()->getCorp();
		CCUserDefault::sharedUserDefault()->setStringForKey("gPhone", gCorp.c_str());
	}

	//  string position = JniCall::sharedJniCall()->getLocation();

	buffer.append("data={\"")
	.append("deviceId")
	.append("\":\"")
	.append(deveiceInf.c_str())
	.append("\",\"")
	.append("version")
	.append("\":\"")
	.append(versionInf.c_str())
	.append("\",\"")
	.append("gameVer")
	.append("\":\"")
	.append(gVersionCode)
	.append("\",\"")
	.append("phone")
	.append("\":\"")
	.append(gPhone.c_str())
	.append("\",\"")
	.append("corp")
	.append("\":\"")
	.append(gCorp.c_str())
	.append("\"}");
#endif
	CCLog("buffer = %s", buffer.c_str());
	return buffer;
}
