#include "GameCommon.h"
#include "GameFrame.h"
#include "GlobalUserData.h"
#include "GrabKeyboard.h"
#include "UpdateManager.h"
#include "GameData.h"
void SendClientRequest(ClientRequest clientRequest, const Json::Value& param)
{
	if (g_pGameFrame)
	{
		g_pGameFrame->OnClientRequest(clientRequest, param);
	}
}

void SendGameTip(const std::string& strTip)
{
	Json::Value param;	
	param["strTip"] = strTip;
	SendClientRequest(ClientRequest::kSendTip, param);
}




void SendTableTip(const std::string& strTip)
{
	Json::Value param;
	param["strTip"] = strTip;
	SendClientRequest(ClientRequest::kSendTableTip, param);
}

void PopTipWithBt2(const std::string& strContent, const std::string& strBt1, const std::string& strBt2, std::function<void(unsigned)> cbBt)
{
	if (g_pGameFrame)
	{
		g_pGameFrame->PopTipLayerWithBt2(strContent, strBt1, strBt2, cbBt);
	}
}

void PopTipWithBt1(const std::string& strContent, const std::string& strBt, std::function<void(unsigned)> cbBt)
{
	if (g_pGameFrame)
	{
		g_pGameFrame->PopTipLayerWithBt1(strContent, strBt, cbBt);
	}
}

void RegistEscPressCb(cocos2d::Ref *target, std::function<void(cocos2d::Ref *pTarget)> cb)
{
	if (g_pGameFrame)
	{
		auto pKeyPressGrabber = g_pGameFrame->GetEscKeyGrabber();
		if (pKeyPressGrabber)
		{
			pKeyPressGrabber->Push(target, cb);
		}
	}

}

void UnRegistEscPressCb(cocos2d::Ref *target)
{
	if (g_pGameFrame)
	{
		auto pKeyPressGrabber = g_pGameFrame->GetEscKeyGrabber();
		if (pKeyPressGrabber)
		{
			pKeyPressGrabber->Remove(target);
		}
	}
}

void SendNonUIMsg(const Json::Value& param)
{
	if (g_pGameFrame)
	{
		g_pGameFrame->PushNonUIMsg(param);
	}
}

void SendNetworkChangedMsg(int status)
{
    Json::Value param;
    param["cmd"] = NonUI_NetworkChanged;
    param["status"] = status;
    SendNonUIMsg(param);
}

void setUpdateLoadPro(int pro)
{
	if (g_pUpdateManager)
	{
		//g_pUpdateManager->updatePro(pro) ;
        g_pUpdateManager->newUpdatePro(pro);
	}
}

void setNewVersion(std::string version)
{
	//if (g_pUpdateManager)
	//{
	//	g_pUpdateManager->updateVersionCode(version) ;
	//}
	GameData::getInstance()->setNewVersionCode(version) ;
}

void setUpdateContent(std::string content)
{
	//if (g_pUpdateManager)
	//{
	//	g_pUpdateManager->showUpdateInfo(content) ;
	//}
	GameData::getInstance()->setUpdateContent(content) ;
}
