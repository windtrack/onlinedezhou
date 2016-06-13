#include "GameFrame.h"
#include "RpcRoutine.h"
#include "LogonLayerEx.h"
#include "GameTable.h"
#include "TextTip.h"
#include "jsonrpc/json/reader.h"
#include "Util.h"
#include "jsonrpc/json/value.h"
#include "GlobalUserData.h"
#include "PlatformSpec.h"
#include "network/HttpClient.h"
#include "GameHall.h"
#include "GameHall2.h"
#include "GrabKeyboard.h"
#include "GameMall.h"
#include "GameLoading.h"
#include "UserView.h"
#include "UserInfo.h"
#include "QuickFollowList.h" 
#include "RewardDrawingAni.h"
#include "GameTipLayer.h"
#include "VipPrivateNormal.h"
#include "GameTablePlayer.h"
#include "GameRecord.h"
#include "GameSet.h"
#include "DeZhouMagnate.h"
#include "GameCommon.h"
#include "SimpleAudioEngine.h"
#include "CustomerServices.h"
#include "GameData.h"
#include "OpenUpVip.h"
#include "ShowFamily.h"
#include "jsonrpc/json/writer.h"
#include "GameSngHall.h"
#include "GameMttHall.h"
#include "LineLabel.h"
#include "LogoLayer.h"

#include "NewbieGuideLayer.h"
#include "NewbieGuideManager.h"
#include "TaskSet.h"
#include "GameConfig.h"

#include "HttpUploader.h"
#include "SngSignup.h"
#include "GameTipsManager.h"
#include "MoveLabel.h"
#include "TrackManager.h"
#include "SngRankList.h"
#include "MttStartNotice.h"
#include "MTTMatchState.h"
#include "MttJoinMatchTips.h"
#include "MttQuitMatch.h"
#include "MttSignUpSuccess.h"
#include "base/ZipUtils.h"
#include "MttJoinMatchTips.h"
#include "GuideTips.h"
#include "PrivilegesRewards.h"
#include "GameClubHall.h"
#include "SelfClub.h"

#include "SelfFund.h"
USING_NS_CC;


#define TEST_IN 0  //是否使用测试服务器

bool GameFrame::m_tipDialogVisible = false;

enum
{
	GameTableZOrder,
	GameHallZOrder,
	GameHall2ZOrder,
	VipPrivateNormalOrder,
    LogonZOrder,
	SngSignUpZOrder,
    GameMallZOrder,
    CustomerServicesZOrder,
    GameRecordZOrder,
	TablePlayerZOrder,
	GameLoadingZOrder,

	//RewardDrawingAniZOrder,
	PassWordInputZOrder,
	TopZOrder,
	OpenUpVipZOrder,
	ShowFamilyZOrder,
	GameSetZOrder,
	ZOrder_Rule,
	GameTipLayerZOrder,
	NewbeiGuideZOrder,
    NewPlayerTaskZOrder,
    RewardDrawingAniZOrder,
	MttMatchStateZOrder,
	TextTipZOrder,
	MttSiginUpSuccess,
	MttMatchDisMiss_ZORDER,//mtt比赛解散
	MttStart_ZORDER,//mtt即将开始的提示
	MttJoin_ZORDER,//mtt20开始的提示
	SelfFund_ZORDER,
    ActRewardZORDER,
};



bool comp(const UsrWinInfo &a, const UsrWinInfo &b)
{
    return a.bIsWinner > b.bIsWinner;
}

void GameFrame::PauseHeartBeatCheck(unsigned num)
{
    if (m_gameMsgIO.check()) {
        Json::Value param;
        param["pause"] = num;
        //SendClientRequest(ClientRequest::kCallPauseHeart, param);
        m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallPauseHeart, param);
        m_gameMsgIO.Flush();
    }
}
/************************************************************************/
/* 上传头像                                                                     */
/************************************************************************/
void GameFrame::doUploadHeader(std::string& openId, std::string& openKey, int timestamp)
{
	HttpUploader* uploader = HttpUploader::getInstance();
	if (uploader) {

		HttpUploader* uploader = HttpUploader::getInstance();
		if (uploader) {
			//const char* headUrl = "http://picture.0058.com/texas_useravator.php?type=200&openid=%s&openkey=%s&timestamp=%d";
			const char* headUrl = "http://116.255.147.37:5800/texas_useravator.php?type=200&openid=%s&openkey=%s&timestamp=%d";
			char buffer[256];
			sprintf(buffer, headUrl, openId.c_str(), openKey.c_str(), timestamp);

            uploader->AddRequest(buffer, "header.jpg", "正在上传图片...", [=](bool success, std::string respData) {
                if (success) {
                    std::vector<std::string> rets;
                    stringSplit(respData, rets, "|");
                    if (rets.size() == 2) {
                        std::string imageName = rets[1];
                        //g_globalMyData.SetSkinID(0);
                        g_globalMyData.SetIconURL(imageName);
                        
                        Json::Value param;
                        param["data"] = "0";
                        param["type"] = 1;
                        SendClientRequest(ClientRequest::kEditMyInfo, param);
                    }
                }
				SendGameTip(success ? "成功上传图片！" : "上传图片失败！");
			});
		}
	}
}

void GameFrame::OnClientRequest(ClientRequest request, const Json::Value& param)
{
	switch (request)
	{
	case::ClientRequest::kCallUpdatePlayerCash:
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallUpdatePlayerCash,param);
		break ;
	case ClientRequest::kCallAllotFunToRole:
		{
			unsigned fid = JsonValueAdapter(param["clubid"]);
			unsigned touid = JsonValueAdapter(param["uid"]);
			unsigned fund = JsonValueAdapter(param["fund"]);
			std::string uname = param["uname"].asCString() ;
			std::string clubname  = param["clubname"].asCString() ;
			RPC rpc(RPCCmd::CallAllotFund, "texas_save_club_fund_log", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,fid,touid,fund,4,uname.c_str(),clubname.c_str());
			m_rpcRoutine.AddRequest(rpc);
		}
		break ;
	case ClientRequest::kCallSelfJoinClub:
		{
			RPC rpc(RPCCmd::GetAllSelfJoinClub, "texas_myclublist", g_globalMyData.m_iUserId, g_globalSession.m_szSecret);
			m_rpcRoutine.AddRequest(rpc);
		}
		break ;
	case ClientRequest::kNoticeCreatClub:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallNoticeCreatClub,param);
		}
		break ;
	case ClientRequest::kNoticeSellingClub:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallNoticeSellingClub,param);
		}
		break ;
	case ClientRequest::kNoticeQuiteClub:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallNoticeQuiteClub,param);
		}
		break ;
	case ClientRequest::kNoticeBeKickedClub:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallNoticeBeKickedClub,param);
		}
		break ;
	case ClientRequest::kNoticeJoinClub:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallNoticeJoinClub,param);
		}
		break ;

	case ClientRequest::kCallPrivateFundData:
		{
			unsigned fid = g_globalMyData.getFamilyID() ;
			RPC rpc(RPCCmd::GetPrivateFundData, "texas_private_fund", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,param["type"],param["kind"],fid);
			m_rpcRoutine.AddRequest(rpc);
		}
		break ;
	case ClientRequest::kCallPrivateRoomPushPlayer:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallPrivateRoomPushPlayer,param);
		}
		break ;
	case ClientRequest::kCallCreatPrivateRoomName:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallCrearPrivateRoomName,param);
		}
		break ;
	case ClientRequest::kCallPrivateGameController:
		{
			//con 0关闭 1开启 
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallPrivateRoomControll,param);
		}
		break ;

	case ClientRequest::kRequsetPrivateRoomInfo:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallCUrPrivateRoomInfo,param);
		}
		break ;
	case ClientRequest::kSerachPriavteRoom:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallSendPrivateRoomPassword,param);
		}
		break ;
	case ClientRequest::kRequsetIsAutoTrust:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallIsAutoTrust);
		}
		break ;
	case ClientRequest::kCallClientCancelAutoTrust:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallCancelAutoTrust);
		}
		break ;
	case ClientRequest::kCallRequestHasMatch:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallRequestHasMatch,param);
		}
		break ;
	case ClientRequest::kCallGetMttFloatMatchInfo:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallMttFloatMatchInfo);
		}
		break ;
	case ClientRequest::kCallMttRebuyCannel:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallMttRebuyCannel);
		}
		break ;
	case ClientRequest::kCallMttSingleMatchInfo:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallMttSingleMatchInfo);
		}
		break ;
	case ClientRequest::kCallMttMatchAddon : //mtt比赛重购
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallMttMatchAddon);
		}
		break ;
	case ClientRequest::kCallMttMatchRebuy : //mtt比赛重购
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallMttMatchRebuy);
		}
		break ;
	case ClientRequest::kCallMttMatchRankList:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallMttMatchRankList);
		}

		break ;
	case ClientRequest::kMttMatchQuit:

		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallMttMatchQuit,param);
            unsigned tableID = JsonValueAdapter(param["id"]);
            if (m_mttStartNotice != nullptr)
            {
                if (m_mttStartNotice->getTableID() == tableID)
                {
                    m_mttStartNotice->setVisible(false);
                }
            }
		}
		break ;
	case ClientRequest::kMttMatchSiginUp:

		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallEnterMttSiginUp,param);
		}
		break ;
	case ClientRequest::kEnterMttMatch:

		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallEnterMttMatch,param);
		}
		break ;
	case ClientRequest::kMttHallInfo:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallEnterMttHall);
		}
		break ;
	case ClientRequest::kCallGetContinuesSiginReward:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallGetContinueSiginReward);
		}

		break ;
	case ClientRequest::kCallGetSiginReward:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallGetSiginReward);
		}

		break ;

	case ClientRequest::kCallSngHallInfo:
		{
			RPC rpc(RPCCmd::GetSngHallInfo, "texas_sng_basic_info", g_globalMyData.m_iUserId, g_globalSession.m_szSecret);
			m_rpcRoutine.AddRequest(rpc);
		}
		break ;
	case ClientRequest::kCallMttHallInfo:
		{
			RPC rpc(RPCCmd::GetMTTHallInfo, "texas_mtt_basic_info", g_globalMyData.m_iUserId, g_globalSession.m_szSecret);
			m_rpcRoutine.AddRequest(rpc);
		}
		break ;
		case ClientRequest::kCallLevelTableWithOutLevel:
			{
				m_pGameTable->ShowMeHasActioned();
				m_gameMsgIO.SendGameMessage(GameMsgCmdC::LeaveTable);
				m_roomMsgIO.StopIO();
				stopVideo();
				g_globalGameSerialization.AddAtomicAction(AtomicAction::GameEnd);
				g_globalTableConfig.Clear();
				g_globalTableConfig.ClearPlayer();
			}
		break ;
		case ClientRequest::kCallExpressPower:
			{
				RPC rpc(RPCCmd::GetExpressPower, "texas_m_expressbag", g_globalMyData.m_iUserId);
				m_rpcRoutine.AddRequest(rpc);
			}
		break; ;
        case ClientRequest::kCallGuideTipsInfo:
            {
                RPC rpc(RPCCmd::GetGuideTipsInfo, "texas_get_guidance", g_globalMyData.m_iUserId, g_globalSession.m_szSecret);
                m_rpcRoutine.AddRequest(rpc);
            }
            break;
        case ClientRequest::kModifyGuideTipsInfo:
            {
                unsigned guideId = JsonValueAdapter(param["guideid"]);
                RPC rpc(RPCCmd::ModifyGuideTipsInfo, "texas_do_guidance", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, guideId);
                m_rpcRoutine.AddRequest(rpc);
            }break;
		//我的道具
		case ClientRequest::kCallUserItemList:
		{
			RPC rpc(RPCCmd::GetUserItemBagPack, "texas_m_get_backpack", g_globalMyData.m_iUserId);
			m_rpcRoutine.AddRequest(rpc);
		}

		break ;
        case ClientRequest::kCallUploadPhoto:
		{
            RPC rpc(RPCCmd::GetNewOpenIDKey, "texas_m_auth", g_globalSession.m_szOpenId);
            m_rpcRoutine.AddRequest(rpc);
		}
		break;
            
		case ClientRequest::CallReturnToLogin:
			jumpToLogin() ;
		break;
		case ClientRequest::kCallPlayAgain:
			{


				Json::Value param;
				//param["enterType"] = 0;
				param["id"] = g_globalSngTableInfo.m_tableId;
				SendClientRequest(ClientRequest::kEnterSngTable, param);

			}
			break;
	case ClientRequest::kCallLookUp:
		{
			Json::Value param;
			param["strTip"] = 5;
			SendClientRequest(ClientRequest::kGiveUp, param);
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::StandUp);
		}
		break ;

	case ClientRequest::kCallLevelTable:
		{
			g_globaMttGameInfo.clear();
			g_globalSngTableInfo.clear() ;
			m_globaPrivateRoomInfo.clean() ;

			m_pGameTable->ShowMeHasActioned();
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::LeaveTable);
			m_roomMsgIO.StopIO();
			stopVideo();
			BackToPrevView();
			g_globalGameSerialization.AddAtomicAction(AtomicAction::GameEnd);
            if (getTableType() == SngTable)
            {
                updateSNGTask();
            }
            else if (getTableType() == MttTable)
            {
                updateMTTTask();
            }
            else if (getTableType() == NormalTable)
            {
                updateNormalTask();
			} else if (getTableType() == PrivateTable)
			{
				updateNormalTask();
			}
			g_globalTableConfig.Clear();
			g_globalTableConfig.ClearPlayer();
			setTableType(NullType) ;

		}

		break ;

	case ClientRequest::kCallSngTableRankList:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallSngMatchRankList);
		}
		break ;

	case ClientRequest::kCallSngTableMangzhu:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallSngTableMangzhu);
		}
		break ;
	case ClientRequest::kGetSngHallInfo:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallSngHallInfo);
		}
		break;

		//1025德州币vip
	case ClientRequest::kCallUpdateCashAndVip:
		m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallUpdateCashAndVip);
		break ;
		

		//延长心跳检测
	case ClientRequest::kCallPauseHeart:
		/************************************************************************/
		/*  
		
		Json::Value param;
		param["pause"] = 0/1;
		SendClientRequest(ClientRequest::kCallPauseHeart, param);
		
		*/
		/************************************************************************/
		m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallPauseHeart, param);
		break ;

	case ClientRequest::CallGameFinished:
		{
			unscheduleUpdate();
			m_gameMsgIO.StopIO();
			m_roomMsgIO.StopIO();
			m_rpcRoutine.Stop();
			network::HttpClient::destroyInstance();
			g_pGameFrame = nullptr;

            TrackMgr->MobClickEnd();

			Director::getInstance()->end(); 
		}
		
		break ;

	case ClientRequest::callGameCancleFriutUI:
		{
			if(m_pGameMall != nullptr)
			{
				m_pGameMall->onCancleBuyGoodsUI(false);
			}
			if(m_pGameTable != nullptr)
			{
				m_pGameTable->OnCancleHideQuickShopUI(false);
			}

		}
		break;
	case ClientRequest::CheckVipLv:
		{
			RPC rpc(RPCCmd::CheckVipLv, "texas_vipinfo",g_globalMyData.m_iUserId, g_globalSession.m_szSecret, param["user_id"]);
			m_rpcRoutine.AddRequest(rpc);	
		}
		break ;
	case ClientRequest::kCallRoleUpdateFamilyInfo:
		m_gameMsgIO.SendGameMessage(GameMsgCmdC::NoticeRoleAboutFamilyInfo, param);
		break ;

	case ClientRequest::kCheckNewMessage:
		{
			RPC rpc(RPCCmd::CheckNewMessage, "texas_group_last_check_msg_time", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,
				param["fid"]
			);
			m_rpcRoutine.AddRequest(rpc);	
		}

		break ;
    case ClientRequest::kNewbieGuidLayer:
        {
            if (m_pNewbieGuide == nullptr)
            {
                TrackMgr->event("NewbieGuide", "开始特训");
                m_pNewbieGuide = NewbieGuideLayer::create();
                addChild(m_pNewbieGuide, NewbeiGuideZOrder);
            }
            if (m_pGameSet != nullptr)
                m_pGameSet->setVisible(false);
            m_pNewbieGuide->restart();
        }
        break;
    case ClientRequest::kJumpOverNewerGuide:
        {
            m_bJumpOverGuide = true;
        }
        break;
    case ClientRequest::kEscNewbieGuide:
        {
            m_pNewbieGuide->onMenuItemLeave();
        }
        break;
    case ClientRequest::kShowNewPlayerTask:
        {
            if (m_pNewPlayerTask == nullptr)
			{
				PopNewerTask();
			}
			m_pNewPlayerTask->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
            m_pNewPlayerTask->setVisible(true);
            m_pNewPlayerTask->UpdateTask(true);
        }
        break;
	case ClientRequest::kDealInvateJoinFamily:
		{

			RPC rpc(RPCCmd::DealInvateJoinFamily, "texas_group_invite_deal", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,
				param["add"],
				param["mid"]
				
			);
			m_rpcRoutine.AddRequest(rpc);	
		}
		break ;


	case ClientRequest::kDealApplyFamily:
		{

			RPC rpc(RPCCmd::DealApplyFamily, "texas_group_addmembers", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,
				param["group_id"],
				param["add"],
				param["user_id"],
				param["mid"]
					
			);
			m_rpcRoutine.AddRequest(rpc);	
		}
		break ;

	case ClientRequest::kSearchFamily:
		{
			RPC rpc(RPCCmd::SearchFamily, "texas_group_search", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,
				param["name"].asString());
			m_rpcRoutine.AddRequest(rpc);	
		}
		break ;

	case ClientRequest::kInviteFreinds:
		{
			RPC rpc(RPCCmd::InviteFreinds, "texas_group_invite", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,
				param["ufamilyid"],param["uid"].asString());
			m_rpcRoutine.AddRequest(rpc);	
		}
		break ;
    case ClientRequest::kCallExchangeInfo:
        {
            RPC rpc(RPCCmd::CallExchangeInfo, "texas_get_exchange_info", g_globalMyData.m_iUserId, g_globalSession.m_szSecret);
            m_rpcRoutine.AddRequest(rpc);
        }break;
    case ClientRequest::kExchangeObject:
        {
            int iConsume = JsonValueAdapter(param["consume"]);
            int exchangeId = JsonValueAdapter(param["id"]);
            int type = JsonValueAdapter(param["type"]);
            RPC rpc(RPCCmd::ExchangeObject, "texas_add_exchange", g_globalMyData.m_iUserId, exchangeId, iConsume, type);
            m_rpcRoutine.AddRequest(rpc);
        }break;
	case ClientRequest::kGetFamiliyReward:
		{
			RPC rpc(RPCCmd::GetFamiliyReward, "texas_group_reward", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,
			
			
			param["ufamilyid"]);
			m_rpcRoutine.AddRequest(rpc);	
		}
		break ;

	case ClientRequest::kChangeFamilyShaikh:
		{
			RPC rpc(RPCCmd::ChangeFamilyShaikh, "texas_group_transfer", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,
				
				
				param["group_id"],param["to_user_id"],param["code"].asString());
			m_rpcRoutine.AddRequest(rpc);	
		}
		break;

	case ClientRequest::kGetChangeShaiKhCode:
		{
			RPC rpc(RPCCmd::GetChangeShaiKhCode, "get_smscode", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, param["phone"].asString());
			m_rpcRoutine.AddRequest(rpc);	
		}
		break;

	case ClientRequest::kGetAllFriends:

		{
			RPC rpc(RPCCmd::GetAllFriends, "texas_group_invite_list", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,param["ufamilyid"]);
			m_rpcRoutine.AddRequest(rpc);
		}
		
		break ;
	case ClientRequest::kGetFamilyNoSKRole:

		{
			RPC rpc(RPCCmd::GetFamilyNoSKRole, "texas_group_members", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,param["ufamilyid"],0,0);
			m_rpcRoutine.AddRequest(rpc);
		}
		break ;

	case ClientRequest::kTo_ViceShaikh:
		{
				
			
			RPC rpc(RPCCmd::To_ViceShaikh, "texas_group_set_helper", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,
				g_globalMyData.getFamilyID(),param["user_id"],param["con"]);
			m_rpcRoutine.AddRequest(rpc);
		}
		break ;
	case ClientRequest::kMove_ViceShaikh:
		{
			RPC rpc(RPCCmd::Move_ViceShaikh, "texas_group_set_helper", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,
				g_globalMyData.getFamilyID(),param["user_id"],param["con"]);
			m_rpcRoutine.AddRequest(rpc);
		}
		break ;

	case ClientRequest::kGetFamilyRankRole:
			{
			RPC rpc(RPCCmd::GetFamilyAllRole, "texas_group_members_rank", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,param["ufamilyid"]);
			m_rpcRoutine.AddRequest(rpc);
		}

		break ;


	case ClientRequest::kEditFamliyNotice:
		{
			RPC rpc(RPCCmd::EditFamilyInfo, "texas_group_update", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,param["ufamilyid"],"notice",param["fnotice"].asString());
			m_rpcRoutine.AddRequest(rpc);
		}
		
		break ;

	case ClientRequest::kEditFamilyIcon:

		{
			RPC rpc(RPCCmd::EditFamilyInfo, "texas_group_update", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,param["ufamilyid"],"icon",param["id"].asString(),param["idindex"]);
			m_rpcRoutine.AddRequest(rpc);
		}

		break ;

	case ClientRequest::kJoinFamily:
		{
			RPC rpc(RPCCmd::JoinFamily, "texas_join_group", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,param["ufamilyid"]);
			m_rpcRoutine.AddRequest(rpc);
		}
		break ;
	case ClientRequest::kLevelFamily:
		{
			RPC rpc(RPCCmd::LevelFamily, "texas_group_info", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,param["ufamilyid"]);
			m_rpcRoutine.AddRequest(rpc);
		}
		break ;
			
			
	case ClientRequest::kPushOutFamily:
		{
	/*		param["user_id"] = m_Uid;
			param["group_id"] = g_globalMyData.getFamilyID();*/
			RPC rpc(RPCCmd::PushOutFamily, "texas_group_rm", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,param["group_id"],param["user_id"]);
			m_rpcRoutine.AddRequest(rpc);
		}
		break ;


	case  ClientRequest::kGetMyFamilyInfo:
		{
			RPC rpc(RPCCmd::GetMyFamilyInfo, "texas_group_info", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,param["ufamilyid"]);
			m_rpcRoutine.AddRequest(rpc);
		}

		break;
	case ClientRequest::kGetAllFamilyList:
		{
			RPC rpc(RPCCmd::GetFamilyList, "texas_group_list", g_globalMyData.m_iUserId, g_globalSession.m_szSecret);
			m_rpcRoutine.AddRequest(rpc);
		}
		break ;

	case ClientRequest::kTargetFamilyInfo:
		{
			RPC rpc(RPCCmd::GetTargetFamilyInfo, "texas_group_info", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, param["ufamilyid"]);
			m_rpcRoutine.AddRequest(rpc);
		}
			
		break ;
	case ClientRequest::kShowTargetFamilyInfo:
		{
			RPC rpc(RPCCmd::ShowGetTargetFamilyInfo, "texas_group_info", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, param["ufamilyid"]);
			m_rpcRoutine.AddRequest(rpc);
		}
		break ;

	//创建




	case ClientRequest::kCreatFamily:
		{
			RPC rpc(RPCCmd::CreatFamily, "texas_create_group", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, param["fname"].asString());
			m_rpcRoutine.AddRequest(rpc);	
		}
		break;
    case ClientRequest::kMttMatchState:
        {
            //EnterMTTMatchState();
        }break;

	case ClientRequest::kSendTip:
		{
			m_pTextTip->AddTip(param["strTip"].asString());
		}
		break;

	case ClientRequest::kSendTableTip:
		{
			if (m_pGameTable)
			{
				m_pGameTable->SendTip(param["strTip"].asString());
			}
		}
		break;

	case ClientRequest::kGetRegisterCode:
		{
            std::string strFlag = param["flag"].asString();
            if (strFlag == "reg")//注册获取验证码
            {
			    RPC rpc(RPCCmd::GetRegisterCode, "m_get_registercode", param["phone"].asString());
                m_rpcRoutine.AddRequest(rpc);
            }
            else if (strFlag == "reset")//重置密码获取验证码
            {
                RPC rpc(RPCCmd::GetRegisterCode, "m_get_resetcode", param["phone"].asString());
                m_rpcRoutine.AddRequest(rpc);
            }
						
		}
		break;

	case ClientRequest::kRegisit:
		{
			RPC rpc(RPCCmd::Regisit, "m_reg", param["phone"].asString(), param["pwd"].asString(), param["code"].asString());
			m_rpcRoutine.AddRequest(rpc);	
			g_globalTempData.m_strLastRegisitPhone = param["phone"].asString();
			g_globalTempData.m_strLastRegisitPwd = param["pwd"].asString();
		}
		break;

	case ClientRequest::kBindPhone:
		{
			RPC rpc(RPCCmd::BindPhone, "vmobile", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, param["phone"].asString(), param["code"].asString());
			m_rpcRoutine.AddRequest(rpc);	
		}
		break;

	case ClientRequest::kGetBindSafeCode:
		{
			RPC rpc(RPCCmd::GetBindSafeCode, "get_vmobile_code", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, param["phone"].asString());
			m_rpcRoutine.AddRequest(rpc);	
		}
		break;

	case ClientRequest::kResetPassword:
		{
			RPC rpc(RPCCmd::ResetPassword, "m_reset_pwd", param["phone"].asString(), param["pwd"].asString(), param["code"].asString());
			m_rpcRoutine.AddRequest(rpc);	
		}
		break;

	case ClientRequest::kUserLogon:
		{
			RPC rpc(RPCCmd::Logon, "texas_m_user_login", param["account"], param["password"], 1);
			// RPC rpc(RPCCmd::Logon, "texas_m_mobile_login", param["account"], param["password"], 1);
			m_rpcRoutine.AddRequest(rpc);
			
		}
		break;

	case ClientRequest::kAutoLogon:
	{
		RPC rpc(RPCCmd::AutoLogon, "texas_m_user_login", param["account"], param["password"], 1, "123456", 1);
		m_rpcRoutine.AddRequest(rpc);
	}
		break;
	case ClientRequest::kGuestLogon:
		{
			RPC rpc(RPCCmd::GuestLogon, "texas_reg_or_login", GetMobileConditionCode(), GetMobileDeviceName());
			m_rpcRoutine.AddRequest(rpc);
			g_globalConfig.SetLastLogonInfo("", "", true, false, false);
			//g_globalConfig.SetLastLogonInfo("", "", true, true, false);

			//RPC rpc(RPCCmd::GuestLogon, "texas_reg_or_login", GetMobileConditionCode(), GetMobileDeviceName());
			//m_rpcRoutine.AddRequest(rpc);
			//g_globalConfig.SetLastLogonInfo("", "", true, true, false);
		}
		break;

	case ClientRequest::kLogonBy3rd:
		{
			g_globalConfig.SetLastLogonInfo("", "", true, false, false);
			//g_globalConfig.SetLastLogonInfo("", "", true, true, false);
			unsigned u3rd = JsonValueAdapter(param["3rd"]);
			Login3rd(u3rd);
		}
		break;
    case ClientRequest::kContinueLogOn:
        {

#if OpenTicketLogon
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallContinueTicketLogin);
#else
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::ContinueLogin);
#endif
        }
        break;

	case ClientRequest::kDrawVipLoginAward:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::DrawVipLoginAward);
		}
		break;

	case ClientRequest::kDrawWaveGrilLoginAward:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::DrawWaveGrilLoginAward);
		}
		break;

	case ClientRequest::kGetHallInfo:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::GetHallInfo, param);
		}
		break;


    case ClientRequest::kGetMagnateInfo:
        {
            m_gameMsgIO.SendGameMessage(GameMsgCmdC::GetMagnateInfo, param);
        }
        break;

    case ClientRequest::kCallHideHandCard:
        {
            if (m_pGameTable != nullptr)
                m_pGameTable->HideHandCard();
        }break;
	case ClientRequest::kGetVipAllOpenInfo:
		{
			EnterVipOpenRoom();
		}
		break;	

	case ClientRequest::kStartPlayVideoInfo:
		{
			SendTableTip("只有贵族身份才能开启视频哦");
		}
		break;
	case ClientRequest::kGetVipOpenRoomInfo:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::GetVipOpenRoomInfo, param);
		}
		break;
	case ClientRequest::kSendVipOpenRoomInfo:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::SendVipOpenRoomInfo, param);
		}
		break;
	case ClientRequest::kQueryUserInfo:
		{
			//m_gameMsgIO.SendGameMessage(GameMsgCmdC::QueryUserInfo, param);
		}
		break;

	case ClientRequest::kOperateOtherPlayer:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::OperateOtherPlayer, param);
		}
		break;

    case ClientRequest::kClickChatBox:
        {
            m_pGameTable->PopChatBox();;
        }
        break;

	case ClientRequest::kTouSu:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::TouSu, param);
		}
		break;
	
	case ClientRequest::kSendTool:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::SendTool, param);
		}
		break;

	case ClientRequest::kInvitePlayer:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::InvitePlayer, param);
		}
		break;

	case ClientRequest::kEnterHall2:
		{
			EnterGameHall2();
		}
		break;
	case ClientRequest::kEnterGameSngHall:
		{
			EnterGameSngHall();
		}
		break;

	case ClientRequest::kEnterGameMttHall:
		{
			EnterGameMttHall();
		}
		break;
	case ClientRequest::kEnterTable:
		{
			setTableType(NormalTable) ;
			PopLoading(true);
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::EnterGame, param); // test
			//EnterGameTable("白手起家", 0, 6, 1, 20, 10, 5, 14);
			//TestGame();
		}
		break;

	case ClientRequest::kEnterSngTable:
		{
			m_pGameTable->ShowMeHasActioned();
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::LeaveTable);
			m_roomMsgIO.StopIO();
			stopVideo();
			g_globalGameSerialization.AddAtomicAction(AtomicAction::GameEnd);
			setTableType(SngTable) ;
			PopLoading(true);
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::EnterSngGame, param); // test
		}
		break;
	
	case ClientRequest::kEnterMttTable:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallEnterMttMatch, param); // test
		}
		break;

	case ClientRequest::kEnterPrivateTable:
		{
			setTableType(PrivateTable) ;
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallEnterRrivateRoom,param); // test
			//setTableType(PrivateTable) ;
			//PopLoading(true);
			//m_gameMsgIO.SendGameMessage(GameMsgCmdC::EnterGame, param); // test
		}
		break;

	case ClientRequest::kSendVipMessage:
		{
			SendTableTip("亲~此魔法至少侠士级别才能使用，快去商城里面开通吧!");
		}
		break;

	case ClientRequest::kJoinGame:
		{
			Json::Value param;
			param["rid"] = g_globalTableConfig.m_uAreaID;
			param["tid"] = g_globalTableConfig.m_uTableID;
			param["ifInvite"] = g_globalTableConfig.m_ifInvite ? 1 : 0;
			param["passWord"] = g_globalTableConfig.m_strPassword;
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::JoinGame, param);
		}
		break;
	case ClientRequest::kJoinTable:
		{
			bool bInvite = JsonValueAdapter(param["ifInvite"]);
			unsigned uAreaID = JsonValueAdapter(param["rid"]);
			unsigned uTableID = JsonValueAdapter(param["tid"]);
			bool bHasPwd = JsonValueAdapter(param["haspwd"]);
            unsigned uUserID = JsonValueAdapter(param["userid"]);

			unsigned tabletype = JsonValueAdapter(param["tabletype"]);

			auto funJoinGame = [this, uAreaID, uTableID, bInvite,tabletype,uUserID](const std::string& strPwd)
			{
				PopLoading(true);

				Json::Value param;
				param["rid"] = uAreaID;
				param["tid"] = uTableID;
				param["ifInvite"] = bInvite ? 1 : 0;
				g_globalTableConfig.m_ifInvite = bInvite;
				param["passWord"] = strPwd;
                param["userid"] = uUserID;
				g_globalTableConfig.m_strPassword;

				setTableType((TableType)tabletype) ;

				m_gameMsgIO.SendGameMessage(GameMsgCmdC::JoinGame, param);
			};

			if (m_pGameTable && m_pGameTable->isVisible())
			{
				if (g_globalGameSerialization.IsInGameNow() && g_globalTableConfig.GetTableMode() == TablePlaying)
				{
					PopTipLayerWithBt2("牌局尚未结束，现在加入其它游戏已下注的筹码将不会返还，是否确认加入？", "立即加入", "暂不加入", [this, bInvite, bHasPwd, funJoinGame](unsigned uBt){
						if (uBt == 0)
						{
							g_globalGameSerialization.AddAtomicAction(AtomicAction::GameEnd);
							m_pGameTable->ShowMeHasActioned();
							m_gameMsgIO.SendGameMessage(GameMsgCmdC::LeaveTable);
							m_roomMsgIO.StopIO();
							stopVideo();

							if (!bInvite && bHasPwd)
							{
								PopPassWordInputBox(funJoinGame);
							}
							else
							{
								funJoinGame("");
							}

							// PopLoading(true);

							//Json::Value param;
							//param["rid"] = uAreaID;
							//param["tid"] = uTableID;
							//param["ifInvite"] = bInvite;
							//param["passWord"] = strPwd;
							//m_gameMsgIO.SendGameMessage(GameMsgCmdC::JoinGame, param);
						}
					});
				}
				else
				{
					m_gameMsgIO.SendGameMessage(GameMsgCmdC::LeaveTable);
					m_roomMsgIO.StopIO();
					stopVideo();					
					if (!bInvite && bHasPwd)
					{
						PopPassWordInputBox(funJoinGame);
					}
					else
					{
						funJoinGame("");
					}
					//PopLoading(true);
					//m_gameMsgIO.SendGameMessage(GameMsgCmdC::JoinGame, param);
				}
			}
			else
			{
				if (!bInvite && bHasPwd)
				{
					PopPassWordInputBox(funJoinGame);
				}
				else
				{
					funJoinGame("");
				}
				//PopLoading(true);
				//m_gameMsgIO.SendGameMessage(GameMsgCmdC::JoinGame, param);
			}
		}
		break;

	case ClientRequest::kChangeTable:
		{
			if (g_globalGameSerialization.IsInGameNow() && g_globalTableConfig.GetTableMode() == TablePlaying)
				{
					PopTipLayerWithBt2("牌局尚未结束，现在换桌已下注的筹码将不会返还，是否确认换桌？", "立即换桌", "暂不换桌", [this](unsigned uBt){
						if (uBt == 0)
						{
							g_globalGameSerialization.AddAtomicAction(AtomicAction::GameEnd);
							m_pGameTable->ShowMeHasActioned();
							m_pGameTable->ResetTable();
							m_gameMsgIO.SendGameMessage(GameMsgCmdC::LeaveTable);
							m_roomMsgIO.StopIO();
							stopVideo();

							Json::Value param;
							param["enterType"] = g_globalTableConfig.m_uRoomID;
							SendClientRequest(ClientRequest::kEnterTable, param);
						}
					});
				}
				else
				{
					m_pGameTable->ResetTable();
                    m_pGameTable->SetWaitSitDownStatus(false);
					m_gameMsgIO.SendGameMessage(GameMsgCmdC::LeaveTable);
					m_roomMsgIO.StopIO();
					stopVideo();
					Json::Value param;
					param["enterType"] = g_globalTableConfig.m_uRoomID;
					//param["enterType"] = -1;
					SendClientRequest(ClientRequest::kEnterTable, param);
				}
		}
		break;

	case ClientRequest::kLeaveTable:
		{
			if (g_globalGameSerialization.IsInGameNow() && g_globalTableConfig.GetTableMode() == TablePlaying)
			{
				PopTipLayerWithBt2("牌局尚未结束，现在退出已下注的筹码将不会返还，是否确认退出？", "确认退出", "暂不退出", 
					[this](unsigned uBt){
					if (uBt == 0)
					{

						m_pGameTable->ShowMeHasActioned();
						m_gameMsgIO.SendGameMessage(GameMsgCmdC::LeaveTable);
						m_roomMsgIO.StopIO();
						stopVideo();
						BackToPrevView();
						g_globalGameSerialization.AddAtomicAction(AtomicAction::GameEnd);
						g_globalTableConfig.Clear();
						g_globalTableConfig.ClearPlayer();
					}
				});
			}
			else
			{
				auto dt = DelayTime::create(0.2);
				auto cb = CallFunc::create([=]() {
                    m_pGameTable->SetWaitSitDownStatus(false);
					m_gameMsgIO.SendGameMessage(GameMsgCmdC::LeaveTable);
					m_roomMsgIO.StopIO();
					stopVideo();
					BackToPrevView();
					g_globalTableConfig.Clear();
					g_globalTableConfig.ClearPlayer();
				});

				this->runAction(Sequence::createWithTwoActions(dt, cb));
			}			
		}
		break;
	case ClientRequest::kPopGameSet:
		{
            TrackMgr->event("PopDialog", "设置弹窗");
			bool bNeedConfirm = JsonValueAdapter(param["needconfirm"]);
			ShowGameSet(bNeedConfirm);
		}
		break;
	case ClientRequest::kStandUp:
		{
			if (g_globalTableConfig.GetTableMode() == TablePlaying)
			{				
				bool bNeedConfirm = JsonValueAdapter(param["needconfirm"]);
				if (bNeedConfirm && g_globalGameSerialization.IsInGameNow())
				{
					PopTipLayerWithBt2("牌局尚未结束，现在站起已下注的筹码将不会返还，是否确认站起？", "确认站起", "暂不站起", 
						[this](unsigned uBt){
						if (uBt == 0)
						{
							Json::Value param;
							param["strTip"] = 4;
							SendClientRequest(ClientRequest::kGiveUp, param);
							m_gameMsgIO.SendGameMessage(GameMsgCmdC::StandUp);

							g_globalGameSerialization.AddAtomicAction(AtomicAction::GameEnd);
							m_pGameTable->setPrivateGameControllButtonVisiable(true) ;
							//m_pGameTable->CleanTableAfterEnd() ;
						}
					});
				}
				else
				{
					Json::Value param;
					param["strTip"] = 5;
					SendClientRequest(ClientRequest::kGiveUp, param);
					m_gameMsgIO.SendGameMessage(GameMsgCmdC::StandUp);
					m_pGameTable->setPrivateGameControllButtonVisiable(true) ;
				}
			}
		}
		break;
	case ClientRequest::kStandUpByMttMatch:
		{
			Json::Value param;
			param["strTip"] = 5;
			SendClientRequest(ClientRequest::kGiveUp, param);
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::StandUp);
		}
		break;
	case ClientRequest::kRequestSitDown:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::RequestSitDown);
		}
		break;

	case ClientRequest::kRequestAutoSitDown:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::RequestAutoSitDown);
		}
		break;

	case ClientRequest::kLeaveHall2:
		{
			BackToPrevView();
		}
		break;
	case ClientRequest::kLeaveVipOpenRoom:
		{
			BackToPrevView();
		}
		break;
	case ClientRequest::kLeaveGameSngHall:
		{
			BackToPrevView();
		}
		break;
	case ClientRequest::kLeaveGameMttHall:
		{
			BackToPrevView();
		}
		break;

	case ClientRequest::kCaoDanDeSendGameMsg:
		{
			m_gameMsgIO.SendGameMessage(static_cast<GameMsgCmdC>(static_cast<unsigned>(JsonValueAdapter(param["cmd"]))), param["msg"]);
		}
		break;

	case ClientRequest::kAddScore:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::Bet, param);
			m_pGameTable->ShowMeHasActioned();
		}
		break;
	case ClientRequest::kGiveUp:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::FoldCard, param);
			m_pGameTable->ShowMeHasActioned();
			unsigned strTip = JsonValueAdapter(param["strTip"]);
			//if(strTip != 1 && strTip != 3 && strTip != 5  && (strTip == 2 || strTip == 4) )
			//{
				m_pGameTable->ShowMeHasFold(strTip);
//			}	
		}
		break;	
	case ClientRequest::kShowBrightBrand:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::BrightBrand, param);
		}
		break;	

	case ClientRequest::kReward:
		{
			//sng常 旁观可打赏
			if (g_globalTableConfig.GetTableMode() == TablePlaying || m_pGameTable->getTableType() == SngTable)
			{
				m_gameMsgIO.SendGameMessage(GameMsgCmdC::RewardDelaer, param);
			}
			else
			{
				SendTableTip("旁观状态下不可打赏！");
			}
		}
		break;

	case ClientRequest::kUpdateMission:
		{
			RPC rpc(RPCCmd::UpdateTask, "texas_updatetask", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, static_cast<unsigned>(JsonValueAdapter(param["task_id"])));
			m_rpcRoutine.AddRequest(rpc);
		}
		break;

	case ClientRequest::kSetAutoSupply:
		{
            if (g_globalTableConfig.GetTableMode() == TableLooking)
            {
                SendTableTip("亲~旁观状态不可以带入钱币！");
                return;
            }
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::SetAutoSupply, param);
			if(g_globalTableConfig.GetTableMode() == TablePlaying)
				SendTableTip("您补充的筹码会在本局结束后添加！");
			else
				SendTableTip("亲~旁观状态不可以带入钱币!");
		}
		break;

	case ClientRequest::kDrawAward:
		{
            //rewardflag 0:领取奖励 1：不领取跳过
			RPC rpc(RPCCmd::DrawMissionAward, "texas_get_taskreward", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, static_cast<unsigned>(JsonValueAdapter(param["missionDrawID"])),
				"webtexas", 0, static_cast<unsigned>(JsonValueAdapter(param["rewardflag"])));
			m_rpcRoutine.AddRequest(rpc);
		}
		break;

	case ClientRequest::kGetFollowList:
		{
			unsigned uFollowListType = JsonValueAdapter(param["followListType"]);
			unsigned uPage = JsonValueAdapter(param["page"]);
			RPC rpc(RPCCmd::GetFollowList, "texas_follow_list", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, uFollowListType+1, uPage, 10);
			m_rpcRoutine.AddRequest(rpc);
		}
		break;

	case ClientRequest::kGetRankList:
		{
			unsigned uWinType = JsonValueAdapter(param["winType"]);
			unsigned uRankType = JsonValueAdapter(param["rankType"]);
			unsigned uPageId = JsonValueAdapter(param["pageID"]);
			unsigned uPageCount = JsonValueAdapter(param["pageCount"]); 
			const char *szRpcCmd[] = {
				"texas_feat_rank",
				"texas_week_win_rank",
				"texas_day_win_rank",
				"texas_cost_rank"
			};
			RPCCmd rpcCmd[] =
			{
				RPCCmd::GetWeekWinPotRank, // 周战绩
				RPCCmd::GetWeekWinCashRank, // 周盈利
				RPCCmd::GetDailyWinCashRank, // 周日单局
				RPCCmd::GetCashRank, // 财富榜
			};
			RPC rpc(rpcCmd[uWinType], szRpcCmd[uWinType], g_globalMyData.m_iUserId, g_globalSession.m_szSecret, uRankType+1, uPageId, uPageCount);
			m_rpcRoutine.AddRequest(rpc);
		}
		break;
	case ClientRequest::kGetSngRankList:
		{
			unsigned uWinType = JsonValueAdapter(param["winType"]);
			unsigned uRankType = JsonValueAdapter(param["rankType"]);
			unsigned uPageId = JsonValueAdapter(param["pageID"]);
			unsigned uPageCount = JsonValueAdapter(param["pageCount"]); 
			const char *szRpcCmd[] = {
				"texas_master_zh_rank",
				"texas_master_rank",
				"texas_master_mtt_rank",
				
				//"texas_day_win_rank",
			};

			RPCCmd rpcCmd[] =
			{
				RPCCmd::GetMasterScoreRank, // 周盈利
				RPCCmd::GetSngScoreRank, // 周日单局
				RPCCmd::GetMttScoreRank, // 周战绩
			
			
			};
			RPC rpc(rpcCmd[uWinType], szRpcCmd[uWinType], g_globalMyData.m_iUserId, g_globalSession.m_szSecret, uRankType+1, uPageId, uPageCount);
			m_rpcRoutine.AddRequest(rpc); 

			
		}
		break ;
	case ClientRequest::kFollowUser:
		{
			unsigned uTUID = JsonValueAdapter(param["tid"]);
			unsigned uOp = JsonValueAdapter(param["op"]); // 1关注 0取消关注
			RPC rpc(RPCCmd::FollowUser, "texas_deal_setting", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,uTUID, "follow", uOp);
			m_rpcRoutine.AddRequest(rpc);
            if (uOp == 0)
            {   // 取消关注时更新当前页列表信息
                Json::Value param;
                param["followListType"] = 0;
                param["page"] = g_globalFollowUserInfo.m_uPageInfo[0][0];// m_uPage;
                SendClientRequest(ClientRequest::kGetFollowList, param);
            }
		}
		break;

	case ClientRequest::kPopUserDetailInfo:
		{
			unsigned uTid = JsonValueAdapter(param["tid"]);
			m_pGameHall->PopUserDetailInfo(uTid);
		}
		break;

	case ClientRequest::kGetUserDetailInfo:
		{
			unsigned uTUID = JsonValueAdapter(param["tuid"]);
			RPC rpc(RPCCmd::GetUserDetailInfo, "texas_hall_basic_info", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,uTUID);
			m_rpcRoutine.AddRequest(rpc);
		}
		break;
	case ClientRequest::kGetTableUserDetailInfo:
		{
			unsigned uTUID = JsonValueAdapter(param["tuid"]);
			RPC rpc(RPCCmd::GetTableUserDetailInfo, "texas_table_basic_info", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,uTUID);
			m_rpcRoutine.AddRequest(rpc);
		}
		break;
	//case ClientRequest::kGetMyDetailInfo:
	//	{
	//		RPC rpc(RPCCmd::GetMyDetailInfo, "texas_basic_info", g_globalMyData.m_iUserId, g_globalSession.m_szSecret,g_globalMyData.m_iUserId);
	//		m_rpcRoutine.AddRequest(rpc);
	//	}
	//	break;
		

	case ClientRequest::kPopUserAchievementInfo:
		{
			unsigned uTid = JsonValueAdapter(param["tid"]);
			RPC rpc(RPCCmd::GetAchievements, "texas_get_achievements", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, uTid);
			m_rpcRoutine.AddRequest(rpc);

			m_pGameHall->PopUserAchievementInfo(uTid);
		}
		break;

	case ClientRequest::kGetActivityInfo:
		{
			unsigned uPlatform = GetPlatformType(); // 1-ios 2-android 3-win32
			RPC rpc(RPCCmd::GetActivityInfo, "texas_activity_infos", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, GAME_ID, uPlatform/*(uPlatform == 1) ? 2 : 3)*/);
			m_rpcRoutine.AddRequest(rpc);
		}
		break;

	case ClientRequest::kGetMessageInfo:
		{
			unsigned uType = JsonValueAdapter(param["type"]);
			RPC rpc(RPCCmd::GetMessageInfo, "texas_sys_msg", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, uType + 1);
			m_rpcRoutine.AddRequest(rpc);
		}
		break;

	case ClientRequest::kGetAchievements:
		{
			//unsigned uTid = JsonValueAdapter(param["tid"]);
			//RPC rpc(RPCCmd::GetAchievements, "texas_get_achievements", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, uTid);
			//m_rpcRoutine.AddRequest(rpc);
		}
		break;
	
	case ClientRequest::kZhuXiao:
		{
#if Use_3rd_Logout
			call3rdLogout() ;

#else
			m_loginFlag = false;
			m_pGameHall->setVisible(false);
			m_gameMsgIO.StopIO();
			m_pLogonLayerEx->setVisible(true);
			g_globalMyData.Reset();	
            g_globalMyAddr.m_bHasAddr = false;
            g_globalMyAddr.m_strXingMing = "";
            g_globalMyAddr.m_strShouJi = "";
            g_globalMyAddr.m_strQQ = "";
            g_globalMyAddr.m_strEmail = "";
            g_globalMyAddr.m_strDiZhi = "";
			StopBackGroundSound(false);
#endif
			SendClientRequest(ClientRequest::kCallGuideTipsInfo);//获取功能引导信息
			g_globalMissionInfo.GetMissions(2).clear();
            g_globalFriendsSpread.m_bIsSpread = false; 
		}
		break;

	case ClientRequest::kSetGameSet:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::SetGameSet, param);
		}
		break;

	case ClientRequest::kPopMall:
		{
            TrackMgr->event("PopDialog", "商城弹窗");
			int index = JsonValueAdapter(param["idx"]) ;
			showGameMall(index);
			SendClientRequest(ClientRequest::kCallExchangeInfo);
		}
		break;

    case ClientRequest::kPopCustomerServices:
        {
            if (m_CurtomerServices == nullptr)
            {
                m_CurtomerServices = CustomerServices::create();
                m_CurtomerServices -> ignoreAnchorPointForPosition(false);
                m_CurtomerServices -> setAnchorPoint(Point(0.5,0.5));
                m_CurtomerServices->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
                addChild(m_CurtomerServices, CustomerServicesZOrder);
            }
            m_CurtomerServices->setVisible(true);
        }
        break;
    case ClientRequest::kPopDiamondBuy:
        {
            if (m_pGameMall != nullptr)
            {
                m_pGameMall->popDiamondBuy();
            }
        }break;
    case ClientRequest::kBuyGoodsByDiamond:
        {
            double dConsume = JsonValueAdapter(param["consume"]);
            unsigned goodsID = JsonValueAdapter(param["goodsID"]);
            RPC rpc(RPCCmd::BuyGoodsByDiamond, "texas_diamond_exchange", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, goodsID);
            m_rpcRoutine.AddRequest(rpc);
        }
        break;
    case ClientRequest::kPopActReward:
        {
            std::string strType = param["type"].asString();
            RPC rpc(RPCCmd::PopActReward, "texas_updataaction", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, strType);
            m_rpcRoutine.AddRequest(rpc);
        }break;
	case ClientRequest::kShowHallBgLayer:
		{
			if (m_pGameHall != nullptr)
				m_pGameHall->ShowLayerBg(true);
		}
		break;
    case ClientRequest::kOptHallBgLayer:
        {
            if (m_pGameHall != nullptr)
                m_pGameHall->ShowLayerBg(false);

			if (m_bgLayer != nullptr)
			{
				//m_bgLayer->setVisible(false);
				showBgLayer(false) ;
			}
        }
        break;
	case ClientRequest::kOptSNGHallBgLayer:
		{
			if (m_pGameSngHall!=nullptr)
			{
				m_pGameSngHall->showBlack(false) ;
			}
            if (m_bgLayer != nullptr)
			{
				//m_bgLayer->setVisible(false);
				showBgLayer(false) ;
			}
                
		}
		break;
	case ClientRequest::kOptMTTHallBgLayer:
		{
			if (m_pGameMttHall!=nullptr)
			{
				m_pGameMttHall->showBlack(false) ;
			}
			if (m_bgLayer != nullptr)
			{
				showBgLayer(false) ;
			}

		}
		break;
	case ClientRequest::kEditMyInfo:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::EditMyInfo, param);
		}
		break;
		//聊天
	case ClientRequest::kPublicChat:
		{
			//m_pGameTable->UsrChat(4, param["text"].asString());
			Json::Value paramPC;
			paramPC["sname"] = g_globalMyData.GetNickName();
			paramPC["type"] = "user";

			paramPC["tname"] = "";
			paramPC["s_stealth"] = "0";
			paramPC["vip_lv"] = g_globalMyData.GetVipLevel();
			paramPC["richLev"] = "0";
			paramPC["tuid"] = "0";
			paramPC["t_stealth"] = "0";
			paramPC["tid"] = as_integer(RoomMsgCmdC::Chat);
			paramPC["g_type"] = "null";
			paramPC["g_level"] = "null";
			paramPC["starLev"] = "null";
			paramPC["suid"] = g_globalMyData.m_iUserId;
			std::string iconUrl = g_globalMyData.GetSkinID() == 0 ? g_globalMyData.GetIconURL() : 
				g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl;
			paramPC["icon"] = iconUrl;
			paramPC["chatText"] = param["text"].asString();




			Json::Value text;
			text["nickname"] =  g_globalMyData.GetNickName();
			text["icon"] = iconUrl; //g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl;
			text["chatText"] = param["text"].asString();
			text["chairID"] = g_globalTableConfig.m_uChairID;
			text["messageType"] = 0;
			text["openid"] = g_globalMyData.m_iUserId;
			Json::FastWriter fast_writer2;
			std::string strBody = fast_writer2.write(text);
			paramPC["text"] = strBody;


			m_roomMsgIO.SendRoomMessage(RoomMsgCmdC::Chat, paramPC);
		}
		break;

	case ClientRequest::kPublicExpress:
		{
			unsigned uType = param["expresstype"].asUInt();

			if (uType == 0 
				|| (g_globalMyData.GetVipLevel() > 1 && g_globalMallInfo.m_vipInfo[g_globalMyData.GetVipLevel()-1].m_bOpenGuiZuBiaoQing)
				||  g_globalMyData.m_useExpress
				)
			{
				Json::Value paramPC;

				paramPC["vip_lv"] = g_globalMyData.GetVipLevel();
				paramPC["richLev"] = "0";
				paramPC["is_year"] = "0";//
				paramPC["starLev"] = "null";
				paramPC["tname"] = "";
				paramPC["t_stealth"] = "0";
				paramPC["g_type"] = "null";
				paramPC["s_stealth"] = "0";
				paramPC["tid"] = as_integer(RoomMsgCmdC::Chat);
				paramPC["g_level"] = "null";

				paramPC["suid"] = g_globalMyData.m_iUserId;
				paramPC["sname"] = g_globalMyData.GetNickName();
				paramPC["type"] = "user";
				paramPC["tuid"] = "0";

				std::string iconUrl = g_globalMyData.GetSkinID() == 0 ? g_globalMyData.GetIconURL() : 
					g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl;

				Json::Value text;
				text["icon"] = iconUrl; // g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl;									
				text["openid"] = g_globalMyData.m_iUserId;//								
				text["nickname"] = g_globalMyData.GetNickName();
				text["faceID"] = unsigned(JsonValueAdapter(param["expressidx"]));
				text["chairID"] = g_globalTableConfig.m_uChairID;
				text["type"] = uType;
				text["messageType"] = 1;
				Json::FastWriter fast_writer2;
				std::string strBody = fast_writer2.write(text);
				paramPC["text"] = strBody;


				m_roomMsgIO.SendRoomMessage(RoomMsgCmdC::Chat, paramPC);

			}
			else
			{
				SendTableTip("贵族表情为VIP专享，请先申请VIP！");
			}
		}
		break;

	case ClientRequest::kGetGamePlayBackList:
		{
			unsigned uPage = JsonValueAdapter(param["page"]);
			unsigned uSortType = JsonValueAdapter(param["sortType"]);
			unsigned uCountPerPage = JsonValueAdapter(param["countPerPage"]);
			RPC rpc(RPCCmd::GetGamePlayBackList, "texas_get_gamelog_list", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, uSortType, uPage+1, uCountPerPage);
			m_rpcRoutine.AddRequest(rpc);
		}
		break;

	case ClientRequest::kGetGamePlayBack:
		{
			RPC rpc(RPCCmd::GetGamePlayBack, "texas_get_onegamelog", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, param["id"].asString());
			m_rpcRoutine.AddRequest(rpc);
			PopLoading(true);

			//PopTablePlayBack(gameRecord);
		}
		break;

	case ClientRequest::kMobilePay:
		{
            PopPaymentLoading(true);
			double dPrice = JsonValueAdapter(param["price"]);
			unsigned uPayType = JsonValueAdapter(param["paytype"]);
			unsigned uID = JsonValueAdapter(param["id"]);
			std::string strName = param["name"].asString();
            m_paramPurchase["GoodsName"] = strName;
            m_paramPurchase["GoodsPrice"] = dPrice;
			if (uPayType == 0) // 银联支付
			{
				RPC rpc(RPCCmd::GetYinLianTradeID, "mupacp_get_trade_no", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, g_globalMyData.m_iUserId, 0, 0, dPrice, GAME_ID, uID);
				m_rpcRoutine.AddRequest(rpc);
                m_paramPurchase["PayType"] = "银联支付";
			}
			else if (uPayType == 1)
			{
				RPC rpc(RPCCmd::GetAliPayTradeID, "m_get_order_no", g_globalMyData.m_iUserId, g_globalMyData.m_iUserId, "malipay", dPrice, "", 0, 0, GAME_ID, uID, strName);
				m_rpcRoutine.AddRequest(rpc);
                m_paramPurchase["PayType"] = "支付宝支付";
			}
			else if (uPayType == 2) 
			{
				RPC rpc(RPCCmd::GetWeixinPayTradeID, "m_wechat_order_dz", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, g_globalMyData.m_iUserId, 0, 0, dPrice, GAME_ID, uID);
				m_rpcRoutine.AddRequest(rpc);
                m_paramPurchase["PayType"] = "微信支付";
			}
			else if (uPayType == 3) 
			{
                 m_paramPurchase["PayType"] = "第三方";
				RPC rpc(RPCCmd::Get3rdTradeID, "m_get_order_no", g_globalMyData.m_iUserId, g_globalMyData.m_iUserId, "malipay", 
					dPrice, "", 0, 0, GAME_ID, uID,strName);
				m_rpcRoutine.AddRequest(rpc);
			}
			else if (uPayType == 4) 
			{ //appstore
                m_paramPurchase["PayType"] = "appstore";
				unsigned uIndex = JsonValueAdapter(param["index"]);

				RPC rpc(RPCCmd::GetAppstoreTradeID, "m_get_order_no", g_globalMyData.m_iUserId, g_globalMyData.m_iUserId, "iap",
					dPrice, "", uIndex, 0, GAME_ID, uID,strName);
				m_rpcRoutine.AddRequest(rpc);
			}
		}
		break;


	case ClientRequest::kExit:
		{
#if Use_3rd_Exit
			call3rdExit() ;
#else

			PopTipLayerWithBt2("您确认要退出全民德州吗？", "退出游戏", "再玩一会", 
				[this](unsigned uBt){
					if (uBt == 0)
					{
						unscheduleUpdate();
						m_gameMsgIO.StopIO();
						m_roomMsgIO.StopIO();
						m_rpcRoutine.Stop();

						network::HttpClient::destroyInstance();
						g_pGameFrame = nullptr;
                        int interval = (getSystemTime() - TrackMgr->GetPreTime());
                        if (interval < 60)
                            TrackMgr->event("TotalTime", TrackMgr->GetIntervalName(interval).c_str(), 0, "秒");
                        else
                            TrackMgr->event("TotalTime", TrackMgr->GetIntervalName(interval).c_str(), 0, "分钟");
                        TrackMgr->MobClickEnd();

						Director::getInstance()->end(); 
					}
			});

#endif

	
		}
		break;
	case ClientRequest::kCheckSitdown:
		m_pGameTable->checkSitdown();
		break;

	case ClientRequest::kGameCheck:
		{
			m_gameMsgIO.SendGameMessage(GameMsgCmdC::Check_In_Game);
		}
		break;
	case ClientRequest::kGetDailyLogin:
		{
            g_globalMyData.m_continueLoginType = 0;
			RPC rpc(RPCCmd::GetTicketLogin, "texas_get_new_ticketlogin", g_globalMyData.m_iUserId, g_globalSession.m_szSecret);

			m_rpcRoutine.AddRequest(rpc);

			
			
		}
		break;
    case ClientRequest::kBindSpread:
        {
            int userID = JsonValueAdapter(param["spreadID"]);
            RPC rpc(RPCCmd::BindSpread, "texas_promoter", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, userID);
            m_rpcRoutine.AddRequest(rpc);

        }break;

    case ClientRequest::kGetMySpread:
        {
            RPC rpc(RPCCmd::GetMySpread, "texas_promoter_list", g_globalMyData.m_iUserId, g_globalSession.m_szSecret);
            m_rpcRoutine.AddRequest(rpc);

        }break;

	default:
		break;
	}
}
void GameFrame::PopTipLayerWithBt1(const std::string& strContent, const std::string& strBt, std::function<void(unsigned)> cbBt)
{
	if (m_pTipLayerWithBt1 == nullptr)
	{
		m_pTipLayerWithBt1 = GameTipLayer::create<GameTipLytBt1Cfg>();
		addChild(m_pTipLayerWithBt1, GameTipLayerZOrder);
	}
	SetLabelString(m_pTipLayerWithBt1->GetContentLabel(), strContent);
	SetLabelString(m_pTipLayerWithBt1->GetButtonLabel(0), strBt);
	m_pTipLayerWithBt1->SetBtCB(cbBt);
	m_pTipLayerWithBt1->setVisible(true);
}

void GameFrame::PopTipLayerWithBt2(const std::string& strContent, const std::string& strBt1, const std::string& strBt2, std::function<void(unsigned)> cbBt)
{
	if (m_pTipLayerWithBt2 == nullptr)
	{
		m_pTipLayerWithBt2 = GameTipLayer::create<GameTipLytBt2Cfg>();
		addChild(m_pTipLayerWithBt2, GameTipLayerZOrder);
	}
	SetLabelString(m_pTipLayerWithBt2->GetContentLabel(), strContent);
	SetLabelString(m_pTipLayerWithBt2->GetButtonLabel(0), strBt1);
	SetLabelString(m_pTipLayerWithBt2->GetButtonLabel(1), strBt2);
	m_pTipLayerWithBt2->SetBtCB(cbBt);
	m_pTipLayerWithBt2->setVisible(true);
}
GlobalKeyGrabber* GameFrame::GetEscKeyGrabber()
{
	return &m_s_escGrabber;
}


void GameFrame::PushNonUIMsg(const Json::Value& msg)
{
	m_syncDeqNonUIMsg.Push(msg);
}

bool GameFrame::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	
	//this -> ignoreAnchorPointForPosition(false);
	//this -> setAnchorPoint(Point(0.5,0.5));
	//this->setPosition(GAME_WIDTH/2,GAME_HEIGHT/2);
	m_onlineDelta = 0.0f;
	m_onlineFlags = 0;
	m_pNewbieGuide = nullptr;
    m_checkDuration = 0.0f;
    m_checkNetworkDelta = 1.0f;
    m_checkNetwork = false;
	m_tipDialogVisible = false;
    m_bJumpOverGuide = false;
	m_dlgOriginalStatus = 0;
	m_checkDialog = 0;
	m_checkTime = 0 ;
	m_loginFlag = false;
	m_bIfYaoQing = false;
	m_pOpenUpVip = nullptr;
	m_pGameTable = nullptr;
	m_pGameHall = nullptr;
	m_pGameHall2 = nullptr;
	m_pGameSngHall = nullptr ;
	m_pGameMttHall = nullptr ;

	m_pGameSet = nullptr;
	m_pGameMall = nullptr;
	m_pTablePlayer = nullptr;
	m_pGameLoading = nullptr;
	m_pRewardDrawingAni = nullptr;
	m_pTipLayerWithBt1 = nullptr;
	m_pTipLayerWithBt2 = nullptr;
	m_pVipPrivateNormal = nullptr;
    m_pGameRecord = nullptr;
	m_pShowFamily = nullptr;
	m_pPwdInput = nullptr;
    m_pPaymentLoading = nullptr;
    m_CurtomerServices = nullptr;
	m_pNewPlayerTask = nullptr;
	//m_loadingLayer = nullptr ;
	//showLoadingLayer(true) ;
	m_sngSignUp = nullptr ;
    m_bgLayer = nullptr;
    m_lastDeltaTime = 0.0f;
	m_masterRankList = nullptr ;
	m_mttStartNotice = nullptr ;
    m_pMTTMatchState = nullptr;
	m_mttMatchDisMissTips = nullptr ;
	m_mttQuitMatch = nullptr ;
	m_mttSignUpSucess = nullptr ;
	m_mttJoinMatchTips = nullptr ;
	m_showReconnect = false ;
    m_bIsRecvGameScence = false;
    m_pActivityRewards = nullptr;

	m_selfFund = nullptr ;

	showGameRun(true);
	//showUpdateManager(true) ;
    return true;
}

// test
#if TEST_IN
#define RPC_URL "http://192.168.1.201:81/api/rpc/pc.php"
//#define RPC_URL "http://192.168.1.201:81/api/rpc/pc.php"
//#define RPC_URL "http://60.174.249.98:8000/api/rpc/pc.php"
#else
#define RPC_URL   "http://www.wepoker.cn/api/rpc/pc.php"
//#define RPC_URL   "http://121.40.33.235:5800/api/rpc/pc.php"
#endif

#define USER_AGENT "User-Agent: NN100 0058/1.0"

GameFrame *g_pGameFrame = nullptr;
void GameFrame::onEnter()
{
	m_rpcRoutine.SetRpcResultProc(std::bind(&GameFrame::OnRpcResult, this, std::placeholders::_1));
	m_rpcRoutine.Start(RPC_URL, USER_AGENT);

	m_gameMsgIO.SetGameIOAbordProc(std::bind(&GameFrame::OnGameIOAbord, this));
	m_gameMsgIO.SetGameMsgProc(std::bind(&GameFrame::OnGameMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)); 
	m_roomMsgIO.SetRoomIOAbordProc(std::bind(&GameFrame::OnRoomIOAbord, this));
	m_roomMsgIO.SetRoomMsgProc(std::bind(&GameFrame::OnRoomMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	



	g_pGameFrame = this;

	Layer::onEnter();

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	EventKeyboard::KeyCode keycode = EventKeyboard::KeyCode::KEY_BACKSPACE;
#else
	EventKeyboard::KeyCode keycode = EventKeyboard::KeyCode::KEY_ESCAPE;
#endif
	m_s_escGrabber.init(this, keycode, false);

	scheduleUpdate();

	// 声音初始化
	auto pAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
	double dSoundValue, dEffectValue;
	g_globalConfig.GetSoundValue(dSoundValue, dEffectValue);
	pAudioEngine->setBackgroundMusicVolume(dSoundValue);
	pAudioEngine->setEffectsVolume(dEffectValue);

	showAutoLogin();
}



void GameFrame::onExit()
{
	Layer::onExit();
	unscheduleUpdate();

	m_gameMsgIO.StopIO();
	m_roomMsgIO.StopIO();
	m_rpcRoutine.Stop();

	network::HttpClient::destroyInstance();

	g_pGameFrame = nullptr;
}

void GameFrame::update(float delta)
{
	//if (GameData::getInstance()->getpauseNetWork())
	//{
	//	return ;
	//}

	m_rpcRoutine.ProcResult();
    
    reconnectGameServer(delta);
	m_roomMsgIO.ProcIO();
	m_gameMsgIO.ProcIO();
	updateHeartBeat(delta);
	Json::Value msg;
	if (m_syncDeqNonUIMsg.Pop(msg))
	{
		OnNonUIMsg(msg);
	}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	updateDialogVisible(delta);
#endif

	if (!GameData::getInstance()->getClearNetWorkInfo())
	{

		GameData::getInstance()->setPauseNetWorkClear(true) ;
		//forceLeaveTable();
		GameData::getInstance()->setSoundOn(true) ;
		return ;
	}
}

void GameFrame::updateHeartBeat(float dt)
{
	if (!m_loginFlag)
		return;

	m_checkTime += dt;
	if (m_checkTime >= 8)
	{
		m_checkTime = 0;
		SendClientRequest(ClientRequest::kGameCheck);
	}


	checkOnline(dt);
}

void GameFrame::OnNonUIMsg(const Json::Value& msg)
{
	unsigned uCmd = JsonValueAdapter(msg["cmd"]);
	if (uCmd == NonUICmd::NonUI_QQLoginResult)
	{
		auto& openid = msg["openid"];
		auto& token = msg["token"];
		RPC rpc(RPCCmd::QQLogon, "texas_m_qq_login", openid.asString(), token.asString(), "", "");
		m_rpcRoutine.AddRequest(rpc);
		m_pLogonLayerEx->ShowLoading();
	}
	else if (uCmd == NonUICmd::NonUI_WXLoginResult)
	{
		auto& openid = msg["openid"];
		auto& token = msg["token"];
		RPC rpc(RPCCmd::WXLogon, "texas_wechat_login", openid.asString(), token.asString(), "", "");
		m_rpcRoutine.AddRequest(rpc);
		m_pLogonLayerEx->ShowLoading();
	}

	else if (uCmd == NonUICmd::NonUI_SinaLoginResult)
	{
		auto& openid = msg["openid"];
		auto& token = msg["token"];
		RPC rpc(RPCCmd::SinaLogon, "oauth_login", openid.asString(), token.asString(), "", "","sina");
		m_rpcRoutine.AddRequest(rpc);
		m_pLogonLayerEx->ShowLoading();
	}
	else if (uCmd == NonUICmd::NonUI_3rdLoginResult)
	{

		auto& openid = msg["openid"];
		auto& token = msg["token"];
		RPC rpc(RPCCmd::The3rdLogon, "oauth_login", openid.asString(), token.asString(), "", "","360pay");
		m_rpcRoutine.AddRequest(rpc);
		m_pLogonLayerEx->ShowLoading();
	}
	else if (uCmd == NonUICmd::NonUI_MobilePayResult)
	{
        std::string strGooddsName = m_paramPurchase["GoodsName"].asCString();
        double dGoodsPrice = JsonValueAdapter(m_paramPurchase["GoodsPrice"]);
        std::string strPayType = m_paramPurchase["PayType"].asCString();
        PopPaymentLoading(false);
		auto& res = msg["res"];
		if (res == 1)
		{
            TrackMgr->event("PaySuccess", strPayType.c_str(), strGooddsName.c_str(), dGoodsPrice);
			SendClientRequest(ClientRequest::kCallUpdateCashAndVip);
            SendClientRequest(ClientRequest::callGameCancleFriutUI);
			PopTipLayerWithBt1("支付成功", "确定");
            Json::Value param;//更新每日充值任务
            param["task_id"] = 2055;
            SendClientRequest(ClientRequest::kUpdateMission, param);
            //充值活动奖励更新
            Json::Value param1;
            param1["type"] = "recharge";
            SendClientRequest(ClientRequest::kPopActReward, param1);//活动奖励更新-》针对充值
		}
		else if (res == 2)
		{
            TrackMgr->event("PayFailed", strPayType.c_str(), strGooddsName.c_str(), dGoodsPrice);
			PopTipLayerWithBt1("支付失败!", "确定");
		}
		else if (res == 3)
		{
			PopTipLayerWithBt1("支付被取消了", "确定");
		}
        
        
        
        
        
	}
	else if (uCmd == NonUI_NetworkChanged) {
		if (m_pLogonLayerEx->isVisible())
			return;

		startCheckNetwork();
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		m_gameMsgIO.StopIO();
		popupNetworkLoading(true);
#endif 
		log("test.... 1");
		
		auto& status = msg["status"];
		if (status == 1) {
			//SendGameTip("网络已连接");
		}
		else if (status == 2) {
			SendGameTip(NETWORK_DISCONNECT_STRING);
		}
	}
    else if (uCmd == NonUICmd::NonUI_AppStoreCheck)
    {
        
        
        RPC rpc(RPCCmd::CheckAppStoreProduce, "texas_pay",g_globalMyData.m_iUserId,
                g_globalSession.m_szSecret, "iap",msg["product"].asCString(),msg["orderid"].asCString(),msg["price"].asCString());
        m_rpcRoutine.AddRequest(rpc);
        
        
        
    }
}

void GameFrame::OnRpcResult(const RPC& rpc)
{
	if (rpc.m_rpcResult.empty())
	{
		OnRpcResultFailed(rpc.m_rpcCmd, rpc.m_rpcException.c_str());
	}
	else
	{
		const Json::Value& result = rpc.m_rpcResult;
		if (!result.empty())
		{
			if (static_cast<unsigned>(JsonValueAdapter(result["s"]))>=1)
			{
				OnRpcResultSuccess(rpc.m_rpcCmd, result, rpc);
			}
			else
			{
                Json::Value data = result["data"];
                if (false == data.isNull() && data.isString()) 
                {
				    OnRpcResultFailed(rpc.m_rpcCmd, result["data"].asCString());
                } 
                else 
                {
                    OnRpcResultFailed(rpc.m_rpcCmd, "获取数据失败！");
                }
			}
		}		
	}
}

void GameFrame::OnRpcResultSuccess(RPCCmd rpcID, const Json::Value& result, const RPC& rpc)
{
	const std::string& str = result.toStyledString();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	auto l = str.length();
	unsigned c = 0;
	do
	{
		char szLog[4096];
		memset(szLog, 0, 4096);
		strncpy(szLog, str.c_str()+c, 4096);
		c += 4096;
		log("%s", szLog);
	} while (c < l);
#endif


	if (rpcID == RPCCmd::CallAllotFund)
	{
		auto& msg = result["data"];
		double clubFund = JsonValueAdapter(msg["clubfund"]);
		double uFund = JsonValueAdapter(msg["myfund"]);

		unsigned uid = JsonValueAdapter(rpc.m_rpcParam[3]);
		std::string name = rpc.m_rpcParam[6].asString() ;
		double upreFund =  JsonValueAdapter(rpc.m_rpcParam[4]);
		char buff[64] ;
		sprintf(buff,"成功分配给成员%s基金%.0f",name.c_str(),upreFund) ;
		if (uid!=g_globalMyData.m_iUserId)
		{
				PopTipWithBt1(buff,"确定",nullptr) ;
		}
	


		g_globalMyFamilyInfo.m_fund = clubFund ;
		for (auto& role:g_globalMyFamilyInfo.m_allTopRank)
		{
			if (role.m_uid == uid)
			{
				role.m_hasCash = uFund ;
				m_pGameHall->updateSelfClubRoleFundInfo(clubFund,uid,uFund) ;
			}
		}

		Json::Value value;
		value["uid"] = uid ;
		value["clubid"] = rpc.m_rpcParam[2] ;
		value["clubname"] = rpc.m_rpcParam[7].asCString() ;
		value["fund"] = upreFund ;
		m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallUpdatePlayerCash,value);

		m_pGameHall->updateSelfClubRoleFundInfo(clubFund,uid,uFund) ;
	}
	else 
	if (rpcID == RPCCmd::GetAllSelfJoinClub)
	{
		g_globaSelfJoinClub.m_vecSelfJoinClub.clear();
		auto& sfamilyData = result["data"];
		for (int i=0;i<sfamilyData.size();i++)
		{		    
			FamilyInfo quickFollowInfo;
			quickFollowInfo.m_familyName = sfamilyData[i]["name"].asString();
			quickFollowInfo.m_familyID = JsonValueAdapter(sfamilyData[i]["id"]);
			quickFollowInfo.m_familylevel = JsonValueAdapter(sfamilyData[i]["lv"]);
			quickFollowInfo.m_curRoleCount = JsonValueAdapter(sfamilyData[i]["members_num"]);
			quickFollowInfo.m_maxRoleCount = JsonValueAdapter(sfamilyData[i]["max_members"]);

			quickFollowInfo.m_familyFatherID =  JsonValueAdapter(sfamilyData[i]["header"]);
			quickFollowInfo.m_isOwner = JsonValueAdapter(sfamilyData[i]["is_header"]);
		

			quickFollowInfo.m_hasJoin = true;
			std::string fUrl = sfamilyData[i]["icon"].asString() ;
			quickFollowInfo.m_familyIconID = 0 ;

			for (int j=0; j< g_globalMyData.m_FamilySkinListCount;j++)
			{

				if (fUrl == g_globalMyData.m_familySkinList[j].m_strIconUrl)
				{

					quickFollowInfo.m_familyIconID = j+1 ;
				}

			}

			quickFollowInfo.m_familyRankID=JsonValueAdapter(sfamilyData[i]["rank"]);
			quickFollowInfo.m_familyRankID =i ;
			g_globaSelfJoinClub.m_vecSelfJoinClub.push_back(quickFollowInfo);

		}

		m_pGameHall->callUpdateSelfJoinClubList();



	}else

	//私密房基金
	if (rpcID == RPCCmd::GetPrivateFundData)
	{
		GlobalFunData m_globalFundData;
		unsigned kind = JsonValueAdapter(rpc.m_rpcParam[3]) ;
		unsigned index = JsonValueAdapter(rpc.m_rpcParam[2]) ;
		
		bool ret = JsonValueAdapter(result["s"]);
		if (!ret)
		{
			return ;
		}


		auto& data  = result["data"];

		//m_globalFundData.m_clubFund =  JsonValueAdapter(data["m_clubFund"]) ;
		//m_globalFundData.m_personalRoomFund =  JsonValueAdapter(data["m_personalRoomFund"]) ;
		m_globalFundData.m_allFund =  JsonValueAdapter(data["m_allFund"]) ;

		auto& fund  = data["detail"];
		for (int j=0 ;j<fund.size(); j++)
		{
			SingleFundData data;
			data.m_index = j ;
			data.m_cash =  JsonValueAdapter(fund[j]["m_cash"]) ;


			std::string date = fund[j]["m_date"].asCString() ;
			data.m_date = date ;
			if ((index-1) != 0)
			{

				int cIndex = date.find(" ") ;
				std::string  uDate = date.substr(cIndex,date.size()) ;
				data.m_date = uDate ;
			}
		

			int type =  JsonValueAdapter(fund[j]["m_isPersonalRoom"]) ;
			data.m_isPersonalRoom = type==1?true:false ;
			data.m_roomName = fund[j]["m_roomName"].asCString() ;

			m_globalFundData.m_vecOneFundData.push_back(data);
		}

		m_globalAllFundData.m_vecAllFundData[kind-1][(index-1)] = m_globalFundData;

		if ((index-1) == 0)
		{
			//m_pVipPrivateNormal->updateFundView(0) ;
			if (m_selfFund != nullptr)
			{
				m_selfFund->updateData() ;
			}
		}
		


	}else
    if (rpcID == RPCCmd::CheckAppStoreProduce)
    {
        Json::Value param;
        param["cmd"] = NonUI_MobilePayResult;
        param["res"] = 1;
        SendNonUIMsg(param);
        
        
    }else

	//mtt大厅面板数据
	if (rpcID == RPCCmd::GetMTTHallInfo)
	{
		auto& mttinfo  = result["data"];
		UserMTTInfo detailInfo;
		detailInfo.m_uMTTBiSaiCiShu =  JsonValueAdapter(mttinfo["mttmatchcout"]);
		detailInfo.m_uMTTHuoJiangCiShu =  JsonValueAdapter(mttinfo["mttrewardcout"]);
		detailInfo.m_iZuiHaoChengJi =  JsonValueAdapter(mttinfo["mttbestrank"]);

		detailInfo.m_uMTTZhouDaShiFen =  JsonValueAdapter(mttinfo["mttweekmasterscore"]);
		detailInfo.m_iMTTDaShiPaiMing =  JsonValueAdapter(mttinfo["mttmaterrank"]);

		m_pGameHall->UpdateUserMttInfo(detailInfo) ;

		g_globalMyData.setMttMasterScore(detailInfo.m_uMTTZhouDaShiFen);
		g_globalMyData.setMttMasterRank(detailInfo.m_iMTTDaShiPaiMing);

	/*	if (m_pGameMttHall!=nullptr)
		{
			m_pGameMttHall->updateMttHallInfo() ;
		}*/

	}else
	//sng大厅面板数据
	if (rpcID == RPCCmd::GetSngHallInfo)
	{
		auto& snginfo  = result["data"];

		UserSngInfo detailInfo;
		detailInfo.m_uSNGBiSaiCiShu =  JsonValueAdapter(snginfo["sngmatchcout"]);
		detailInfo.m_uSNGHuoJiangCiShu =  JsonValueAdapter(snginfo["sngrewardcout"]);
		detailInfo.m_uSNGZhouDaShiFen =  JsonValueAdapter(snginfo["sngweekmasterscore"]);
		detailInfo.m_iSNGDaShiPaiMing =  JsonValueAdapter(snginfo["sngmaterrank"]);
		detailInfo.m_uSNGJiangBei[0] =  JsonValueAdapter(snginfo["sngrank1cout"]);
		detailInfo.m_uSNGJiangBei[1] =  JsonValueAdapter(snginfo["sngrank2cout"]);
		detailInfo.m_uSNGJiangBei[2] =  JsonValueAdapter(snginfo["sngrank3cout"]);
		m_pGameHall->UpdateUserSngInfo(detailInfo) ;

	}else
	
	//更新表情权限
	if (rpcID == RPCCmd::GetExpressPower)
	{
		unsigned useExpressbag = JsonValueAdapter(result["data"]["expressbag"]);
		unsigned useMagciExpressbag = JsonValueAdapter(result["data"]["expressmagicbag"]);

        auto& skinLst = result["data"]["skinlist"];
        //if (skinLst.size() > 0)
			 g_globalMyData.m_uSkinListCount = 0;
		g_globalMyData.m_userSkinList.clear() ;

		char* skinIconPath[] = {
			"defaulticon.png",
			"defaulticon_nv.png"
		} ; 

		char* skinXXPath[] = {
			"hall/xingxiang2.png",
			"hall/xingxiang1.png",
		} ; 


	

        for (unsigned i = 0; i < skinLst.size(); i++)
        {
			UserSkin skin ;
            skin.m_uID = JsonValueAdapter(skinLst[i]["id"]);
            skin.m_strIconUrl = skinLst[i]["icon"].asString();
            skin.m_strAvatorUrl = skinLst[i]["avator"].asString();
            skin.m_strVoiceDir = skinLst[i]["voice"].asString();
            skin.m_strAvatorBgUrl = skinLst[i]["bg"].asString();
			g_globalMyData.m_userSkinList.push_back(skin);
            ++g_globalMyData.m_uSkinListCount;
        }
		g_globalMyData.setUseExpress(useExpressbag) ;
		g_globalMyData.setuseMagicExpress(useMagciExpressbag) ;

	}else
	//获取道具列表
	if (rpcID == RPCCmd::GetUserItemBagPack)
	{

		g_globalUserItemList.m_vecUserItemList.clear();
		auto& items = result["data"];
		for (int i=0; i<items.size(); i++ )
		{
			UserItemInfo item ;

			item.m_iconUrl = items[i]["iconUrl"].asString();
			item.m_name = items[i]["name"].asString();
			item.m_expDate =  items[i]["expDate"].asString();
			item.m_des = items[i]["des"].asString();
			item.m_count = JsonValueAdapter(items[i]["count"]);
			item.m_type = JsonValueAdapter(items[i]["type"]); 
			item.m_state = JsonValueAdapter(items[i]["state"]); 
			item.m_btdes = items[i]["btdes"].asString();
			g_globalUserItemList.m_vecUserItemList.push_back(item);
		}

		m_pGameHall->updateUserItem() ;

	}else
	if (rpcID == RPCCmd::GetGuideTipsInfo)
    {
        GuideInfo->clearOldGuideInfo();
        auto& guideInfo = result["data"];
        for (int i=0; i<guideInfo.size(); i++)
        {
            GuideInfo->getAllGuideInfo(JsonValueAdapter(guideInfo[i]));
        }
    }else
    if (rpcID == RPCCmd::ModifyGuideTipsInfo)
    {
        //bool ret = JsonValueAdapter(result["data"]["s"]);
        unsigned id = JsonValueAdapter(result["data"]);
        GuideInfo->ModifyGuideInfo(id, 1);
    }else
    if (rpcID == RPCCmd::GetNewShopPrice)
    {
		auto& fruits = result["data"]["fruit"];
		for (unsigned i = 0; i < fruits.size(); i++)
		{
			g_globalMallInfo.m_fruitInfo[i].m_uID = JsonValueAdapter(fruits[i]["id"]);
			g_globalMallInfo.m_fruitInfo[i].m_dPrice = JsonValueAdapter(fruits[i]["price"]);
			g_globalMallInfo.m_fruitInfo[i].m_dReturnCash = JsonValueAdapter(fruits[i]["return_beans"]);
			g_globalMallInfo.m_fruitInfo[i].m_strIconUrl = fruits[i]["url"].asString();
			g_globalMallInfo.m_fruitInfo[i].m_strName = fruits[i]["name"].asString();
		}
		//std::string m_iconUrl ; //图片地址
		//std::string m_name ;    //名称
		//std::string m_des;		//描述
		//unsigned m_price;		//价格
		//unsigned m_uID;			//id编号
		//bool isNull;	
		//unsigned m_index ;		//序列
		g_globalMallInfo.m_vecShopItemList.clear() ;
		auto& items = result["data"]["shop"];
		for (unsigned i = 0; i < items.size(); i++)
		{
			ItemInShopInfo item ; 
			item.m_iconUrl = items[i]["url"].asString();
			item.m_name = items[i]["name"].asString();
			item.m_des = items[i]["description"].asString();
			item.m_price =  JsonValueAdapter(items[i]["price"]);
			item.m_uID =  JsonValueAdapter(items[i]["id"]);
			item.isNull = false ;
			item.m_index = i ;
			g_globalMallInfo.m_vecShopItemList.push_back(item) ;
		}
        g_globalMallInfo.m_vecHeadShop.clear();
        auto &headItem = result["data"]["headportraits"];
        for (int i=0; i<headItem.size(); i++)
        {
            ItemInShopInfo headInfo;
            headInfo.m_uID = JsonValueAdapter(headItem[i]["id"]);
            headInfo.m_name = headItem[i]["name"].asString();
            headInfo.m_des = headItem[i]["description"].asString();
            headInfo.m_uLastTime = JsonValueAdapter(headItem[i]["day"]);
            headInfo.m_price = JsonValueAdapter(headItem[i]["price"]);
            headInfo.m_iconUrl = headItem[i]["url"].asString();
            headInfo.m_iconUrl2 = headItem[i]["url2"].asString();
            headInfo.isNull = false;
            headInfo.m_index = i;
            g_globalMallInfo.m_vecHeadShop.push_back(headInfo);
        }
        g_globalMallInfo.m_vecDiamond.clear();
        auto &diamondInfo = result["data"]["diamond"];
        for (int i=0; i<diamondInfo.size(); i++)
        {
            ItemInShopInfo diamondItem;
            diamondItem.m_uID = JsonValueAdapter(diamondInfo[i]["id"]);
            diamondItem.m_uDiamondNum = JsonValueAdapter(diamondInfo[i]["name"]);//名称就是钻石个数
            diamondItem.m_des = diamondInfo[i]["description"].asString();
            diamondItem.m_price = JsonValueAdapter(diamondInfo[i]["price"]);
            diamondItem.m_iconUrl = diamondInfo[i]["url"].asString();
            diamondItem.m_iconUrl2 = diamondInfo[i]["url2"].asString();
            diamondItem.isNull = false;
            diamondItem.m_index = i;
            g_globalMallInfo.m_vecDiamond.push_back(diamondItem);
        }

        if (m_pGameMall != nullptr)
        {
            m_pGameMall->initHeadShopLayer();
        }
		/*RPC rpc(RPCCmd::GetVIPPrice, "texas_m_vip_rchglist", g_globalMyData.m_iUserId, g_globalSession.m_szSecret);
		m_rpcRoutine.AddRequest(rpc);*/
    }else
    // 获取新的OPEN ID和KEY 来上传照片
    if (rpcID == RPCCmd::GetNewOpenIDKey) {
        std::string newOpenId = result["data"]["openid"].asString();
        std::string newOpenKey = result["data"]["openkey"].asString();
        int newTimestamp = JsonValueAdapter(result["data"]["timestamp"]);
        
        doUploadHeader(newOpenId, newOpenKey, newTimestamp);
    } else
	//检查会员等级
	if (rpcID == RPCCmd::CheckVipLv)
	{
		unsigned lv = JsonValueAdapter(result["data"]["vip_lv"]) ;

		//if (lv==5)
		//{
			if(g_globalMyData.IsBindPhone())
			{
				m_pGameHall->showMobileCheck(true,g_globalMyData.m_iUserId) ;
				m_pGameHall->callUpdateMobileCheckPhone(g_globalMyData.getBindPhoneNumber()) ;
			}
			else
			{
				m_pGameHall->showMobileBind(true) ;
			}
		//}
		//else
		//{
		//	//PopTipLayerWithBt1("只能转让给天使玩家","确定",nullptr) ;
		//}
		
	}else
	//检查新消息
	if (rpcID == RPCCmd::CheckNewMessage)
	{
		unsigned time = JsonValueAdapter(result["data"]) ;
		if (time!=0)
		{
			GameData::getInstance()->setNewMessageTime(time) ;
			//m_pGameHall->updateShowNewMessage() ;
		}


	}else

	//更换俱乐部信息
	if (rpcID == RPCCmd::EditFamilyInfo)
	{
		std::string type = rpc.m_rpcParam[3].asString() ;
		if (type == "icon")
		{
			unsigned id = JsonValueAdapter(rpc.m_rpcParam[5]) ;
			g_globalMyFamilyInfo.setFamilyIconID(id);
		}
		if (type == "notice")
		{
			std::string snotice = rpc.m_rpcParam[4].asString() ;
			m_pGameHall->callUpdateFamilyNotice(snotice) ;
		}

	}else



	//邀请好友
	if (rpcID == RPCCmd::InviteFreinds)
	{
		std::string uid = rpc.m_rpcParam[3].asString() ;
		m_pGameHall->removeInviteFriendsView(uid) ;
		PopTipLayerWithBt1(result["data"].asString(),"确定",nullptr) ;
		

	}else

    if (rpcID == RPCCmd::CallExchangeInfo)
    {
		//showAndroidLog("兑换数据1") ;
	/*	Json::Value test ;
		test["duihuang_globalMallInfom_vecToExchange"] = g_globalMallInfo.m_vecToExchange.size() ;
		test["duihuang_m_vecHeadShopm_vecHeadShop"] = g_globalMallInfo.m_vecHeadShop.size() ;
		test["duihuang_globalMallInfom_vecShopItemList"] = g_globalMallInfo.m_vecShopItemList.size() ;
		showAndroidLogByJson(test) ;*/

        g_globalMallInfo.m_vecExchange.clear() ;
        auto& exchangeInfo = result["data"];

		//Json::Value msg2 ;
		//msg2["duihuan3exchangeInfo"] = exchangeInfo.size() ;
		//showAndroidLogByJson(msg2) ;
		//showAndroidLog("兑换数据2") ;
	

        for (int i=0; i<exchangeInfo.size(); i++)
        {
            ItemInShopInfo shopInfo;
			shopInfo.m_uTicketConsume = JsonValueAdapter(exchangeInfo[i]["consume"]);
			shopInfo.m_uTicketNum = JsonValueAdapter(exchangeInfo[i]["count"]);
            shopInfo.m_uID = JsonValueAdapter(exchangeInfo[i]["gift_id"]);
			shopInfo.m_iconUrl = exchangeInfo[i]["iconUrl"].asString();
			shopInfo.m_des = exchangeInfo[i]["name"].asString();//这里名称和描述相同
     

			if (shopInfo.m_uTicketNum ==0)
			{
				shopInfo.m_isPastDue = false;
				shopInfo.m_strIndate = "永久";
			}
			else
			{
				shopInfo.m_isPastDue = JsonValueAdapter(exchangeInfo[i]["exp"]);
				shopInfo.m_strIndate = exchangeInfo[i]["expDate"].asString();
			}
            
            g_globalMallInfo.m_vecExchange.push_back(shopInfo);
        }
		
        if (m_pGameMall != nullptr)
        {
            m_pGameMall->initExChangeLayer();
        }
    }else
    if (rpcID == RPCCmd::ExchangeObject)
    {
        bool ret = JsonValueAdapter(result["s"]);
        unsigned uType = 0;
        if (ret && m_pGameMall != nullptr)
        {
             std::string strExchangeName = result["data"]["name"].asString();
             int iConsume = JsonValueAdapter(result["data"]["num"]);
             unsigned id = JsonValueAdapter(result["data"]["id"]);
             unsigned uType = JsonValueAdapter(result["data"]["type"]);//1:正常兑换 2：过期兑换
             for (auto &item : g_globalMallInfo.m_vecExchange)
             {
                 if (item.m_uID == id)
                 {
                     item.m_uTicketNum += iConsume;
                     if (uType == 1)//兑换成功提示
                         m_pGameMall->updateExchangeTips(2, "恭喜您获得", strExchangeName);
                     if (uType == 2)//过期的兑换券请求返回直接清空
                     {
                         item.m_uTicketNum = 0;
                         m_pGameMall->initExChangeLayer();
                     }
                     break;
                 }
             }
        }
        else
        {
            if (uType == 1 && m_pGameMall != nullptr)//兑换失败提示
                m_pGameMall->updateExchangeTips(2, "兑换出现了问题，请稍后再试", "");
        }
    }else


	//申请处理
	if (rpcID == RPCCmd::DealApplyFamily)
	{

        //处理请求后，再次请求俱乐部消息
        Json::Value param;
        param["type"] = 2;
        SendClientRequest(ClientRequest::kGetMessageInfo, param);

		unsigned mid =JsonValueAdapter(rpc.m_rpcParam[5])  ;
			unsigned deal =JsonValueAdapter(result["s"])  ;
		std::vector<MessageInfo> vecMessages = g_globalMessageInfo.m_vecMessageInfo[2];
		for (int i=0; i<vecMessages.size();i++)
		{
			if (vecMessages.at(i).m_uID == mid)
			{
				vecMessages.at(i).m_hasDeal = deal ;
				m_pGameHall->callUpdateMessageFamily(vecMessages.at(i)) ;
			}
		}

		if (deal == 1)
		{

			Json::Value param;
			param["clubId"] = rpc.m_rpcParam[2] ;
			param["uid"] = rpc.m_rpcParam[4] ;
			SendClientRequest(ClientRequest::kNoticeJoinClub,param) ;

			callSocketTips(Notic_JoinFamily,JsonValueAdapter(rpc.m_rpcParam[4]),g_globalMyData.getFamilyName(), g_globalMyData.getFamilyID()) ;

		}
		

	}
	else

	//邀请处理
	if (rpcID == RPCCmd::DealInvateJoinFamily)
	{
		unsigned mid =JsonValueAdapter(rpc.m_rpcParam[3])  ;
		unsigned mcon =JsonValueAdapter(rpc.m_rpcParam[2])  ;

		unsigned con =JsonValueAdapter(result["s"])  ;

		if (con==2&&mcon!=2)
		{
			SendGameTip(result["data"].asString()) ;
		}

		std::vector<MessageInfo> vecMessages = g_globalMessageInfo.m_vecMessageInfo[2];
		for (int i=0; i<vecMessages.size();i++)
		{
			if (vecMessages.at(i).m_uID == mid)
			{
				vecMessages.at(i).m_hasDeal = JsonValueAdapter(con) ;
				m_pGameHall->callUpdateMessageFamily(vecMessages.at(i)) ;

				//加入俱乐部了

				if (con==1)
				{

					std::string  name = result["data"]["fname"].asString();


					char tips[64] ;
					sprintf(tips,"恭喜你加入俱乐部%s,现在您是其中的一员了",name.c_str()) ;

					PopTipWithBt1(tips,"确定",nullptr) ;

				}
				//PopTipWithBt1()

				return ;
			}
		
		}
	}
	else


	//搜索俱乐部
	if (rpcID == RPCCmd::SearchFamily)
	{
		g_globalFamilyInfo.m_vecFamilyInfo.clear();
		auto& sfamilyData = result["data"];
		g_globalFamilyInfo.familyListSize = sfamilyData.size() ;
		for (int i=0;i<sfamilyData.size();i++)
		{		    
			FamilyInfo quickFollowInfo;
			quickFollowInfo.m_familyName = sfamilyData[i]["name"].asString();
			quickFollowInfo.m_familyID = JsonValueAdapter(sfamilyData[i]["id"]);
			quickFollowInfo.m_familylevel = JsonValueAdapter(sfamilyData[i]["lv"]);
			quickFollowInfo.m_curRoleCount = JsonValueAdapter(sfamilyData[i]["mnum"]);
			quickFollowInfo.m_maxRoleCount = JsonValueAdapter(sfamilyData[i]["max_mnum"]);
			
			quickFollowInfo.m_familyFatherID =  JsonValueAdapter(sfamilyData[i]["header"]);
			quickFollowInfo.m_isOwner = (g_globalMyData.m_iUserId == quickFollowInfo.m_familyFatherID) ;

			std::string fUrl = sfamilyData[i]["icon"].asString() ;
			//quickFollowInfo.m_familyIconID = JsonValueAdapter(sfamilyData[i]["icon"]);

			quickFollowInfo.m_familyIconID = 0 ;
			quickFollowInfo.m_hasJoin =  JsonValueAdapter(sfamilyData[i]["is_member"]);
			for (int j=0; j< g_globalMyData.m_FamilySkinListCount;j++)
			{

				if (fUrl == g_globalMyData.m_familySkinList[j].m_strIconUrl)
				{

					quickFollowInfo.m_familyIconID = j+1 ;
				}

			}



			quickFollowInfo.m_familyRankID=JsonValueAdapter(sfamilyData[i]["rank"]);;
			g_globalFamilyInfo.m_vecFamilyInfo.push_back(quickFollowInfo);
		}

		m_pGameHall->callUpdateFamilyRankList();
			

	}else


	//领取奖励
	if (rpcID == RPCCmd::GetFamiliyReward)
	{
		auto& data = result["data"] ;
		int money = JsonValueAdapter(data["texas_beans"]);
		int add = JsonValueAdapter(data["add"]);
		char info[64];
		sprintf(info,"恭喜你领取%d德州币",add) ;
		PopTipWithBt1(info,"确定",nullptr) ;
		g_globalMyData.SetCash(money);

		unsigned time = JsonValueAdapter(result["next_time"]) ;
		m_pGameHall->callGetReward(time);

	}else

		

	//转让俱乐部
	if ( rpcID == RPCCmd::ChangeFamilyShaikh)
	{
	

		auto& mAdmin = result["data"];
		g_globalMyFamilyInfo.m_AllAdmin.clear() ;
		for (int i = 0; i < mAdmin.size(); i++)
		{
			FamilyRoleBaseInfo admin ;
			admin.m_uid = JsonValueAdapter(mAdmin[i]["id"]) ;
			admin.m_iconUrl = mAdmin[i]["icon"].asString();
			admin.m_power = JsonValueAdapter(mAdmin[i]["role"]) ;
			g_globalMyFamilyInfo.m_AllAdmin.push_back(admin);
			m_pGameHall->callUpdateAdminList() ;
		}


		int uid = JsonValueAdapter( rpc.m_rpcParam[3]) ;


		for (auto&role:g_globalMyFamilyInfo.m_allTopRank)
		{
			if (role.m_uid == uid)
			{
				role.m_isOwnner = true ;
				m_pGameHall->callUpdateRanlistItem(role);
			}
			if (role.m_uid == g_globalMyData.m_iUserId)
			{
				role.m_isOwnner = false ;
				m_pGameHall->callUpdateRanlistItem(role);
			}
		}



		PopTipWithBt1("您已成功转让俱乐部！","确定",nullptr) ;


		g_globalMyFamilyInfo.m_isOwner = false ;

		m_pGameHall->updateSelfClubView() ;

		Json::Value param;
		param["clubId"] = rpc.m_rpcParam[2] ;
		param["ownerId"] = rpc.m_rpcParam[3] ;
		SendClientRequest(ClientRequest::kNoticeSellingClub,param) ;

		callSocketTips(Notic_NewShaikh,JsonValueAdapter(rpc.m_rpcParam[3]),g_globalMyData.getFamilyName(), g_globalMyData.getFamilyID()) ;

		//清除我所创建的家族
		g_globalMyData.setFamilyID(0) ;
		g_globalMyData.setFamilyName("") ;
		g_globalMyData.setFamilyPower(0) ;

	}else



	//好友 成员信息
	if ( rpcID == RPCCmd::GetAllFriends)
	{
		g_globalMyFamilyInfo.m_allFriends.clear();

		auto& sfamilyRole = result["data"];
		for (int i=0;i<sfamilyRole.size();i++)
		{
			FamilyRoleBaseInfo roleInfo;
			roleInfo.m_uname = sfamilyRole[i]["name"].asString() ;
			roleInfo.m_uid = JsonValueAdapter(sfamilyRole[i]["id"]);
			roleInfo.m_iconUrl = sfamilyRole[i]["icon"].asString() ;

			g_globalMyFamilyInfo.m_allFriends.push_back(roleInfo);

		}
		//m_pGameHall->callUpdateMyFamilyRoleRankList();
		m_pGameHall->callUpdateFriendsList();
	}else

	//俱乐部无权限人员 成员信息
	if ( rpcID == RPCCmd::GetFamilyNoSKRole)
	{
		g_globalMyFamilyInfo.m_NoPowerRole.clear();

		auto& sfamilyRole = result["data"];
		for (int i=0;i<sfamilyRole.size();i++)
		{
			FamilyRoleBaseInfo roleInfo;
			roleInfo.m_uname = sfamilyRole[i]["nickname"].asString() ;
			roleInfo.m_uid = JsonValueAdapter(sfamilyRole[i]["id"]);
			roleInfo.m_iconUrl = sfamilyRole[i]["icon"].asString() ;


			g_globalMyFamilyInfo.m_NoPowerRole.push_back(roleInfo);

		}
		//m_pGameHall->callUpdateMyFamilyRoleRankList();
		m_pGameHall->callUpdateNoPowerRoleList() ;
	}else


	//设置副族长
	if ( rpcID == RPCCmd::To_ViceShaikh)
	{
	
		auto& mAdmin = result["data"];
		g_globalMyFamilyInfo.m_AllAdmin.clear() ;
		for (int i = 0; i < mAdmin.size(); i++)
		{
			FamilyRoleBaseInfo admin ;
			admin.m_uid = JsonValueAdapter(mAdmin[i]["id"]) ;
			admin.m_iconUrl = mAdmin[i]["icon"].asString();
			admin.m_power = JsonValueAdapter(mAdmin[i]["role"]) ;
			g_globalMyFamilyInfo.m_AllAdmin.push_back(admin);
		}

		  
		unsigned uid = JsonValueAdapter( rpc.m_rpcParam[3]) ;

		for (int i=0; i< g_globalMyFamilyInfo.m_allRankRole.size(); i++)
		{
			FamilyRoleBaseInfo role = g_globalMyFamilyInfo.m_allRankRole.at(i);

			if (role.m_uid == uid)
			{
				role.m_power = 1 ;
				m_pGameHall->callUpdateRanlistItem(role);
			}

		}

		m_pGameHall->callUpdateAdminList();

		PopTipWithBt1("设置副族长成功","确定",nullptr) ;

		callSocketTips(Notic_ToViceSHaikh,JsonValueAdapter(rpc.m_rpcParam[3]),g_globalMyData.getFamilyName(), g_globalMyData.getFamilyID()) ;
	
	}else
		//取消副族长
	if ( rpcID == RPCCmd::Move_ViceShaikh)
	{
		auto& sfamilyRole = result["data"];
		auto& mAdmin = result["data"];
		g_globalMyFamilyInfo.m_AllAdmin.clear() ;
		for (int i = 0; i < mAdmin.size(); i++)
		{
			FamilyRoleBaseInfo admin ;
			admin.m_uid = JsonValueAdapter(mAdmin[i]["id"]) ;
			admin.m_iconUrl = mAdmin[i]["icon"].asString();
			admin.m_power = JsonValueAdapter(mAdmin[i]["role"]) ;
			g_globalMyFamilyInfo.m_AllAdmin.push_back(admin);
		}


		int uid = JsonValueAdapter( rpc.m_rpcParam[3]) ;

		for (int i=0; i< g_globalMyFamilyInfo.m_allRankRole.size(); i++)
		{
			FamilyRoleBaseInfo role = g_globalMyFamilyInfo.m_allRankRole.at(i);

			if (role.m_uid == uid)
			{
				role.m_power = 0 ;
				m_pGameHall->callUpdateRanlistItem(role);
			}
		
		}
		
		m_pGameHall->callUpdateAdminList();
		PopTipWithBt1("撤销副族长成功","确定",nullptr) ;

		callSocketTips(Notic_NOViceSHaikh,JsonValueAdapter(rpc.m_rpcParam[3]),g_globalMyData.getFamilyName(), g_globalMyData.getFamilyID()) ;
	}else

	//俱乐部成员信息
	if ( rpcID == RPCCmd::GetFamilyAllRole)
	{
		g_globalMyFamilyInfo.m_allRankRole.clear();

		auto& sfamilyRole = result["data"];
		for (int i=0;i<sfamilyRole.size();i++)
		{
			FamilyRoleBaseInfo roleInfo;
			roleInfo.m_uname = sfamilyRole[i]["nickname"].asString() ;
			roleInfo.m_uid = JsonValueAdapter(sfamilyRole[i]["id"]);
			roleInfo.m_iconUrl = sfamilyRole[i]["icon"].asString() ;
			roleInfo.m_rank =i;
			roleInfo.m_power = JsonValueAdapter(sfamilyRole[i]["role"]);
			roleInfo.m_contribution = JsonValueAdapter(sfamilyRole[i]["data"]);
			roleInfo.m_viplv = JsonValueAdapter(sfamilyRole[i]["viplv"]);
			g_globalMyFamilyInfo.m_allRankRole.push_back(roleInfo);
			
		}
		//m_pGameHall->callUpdateMyFamilyRoleRankList();

	}else
		// 踢出俱乐部
	if ( rpcID == RPCCmd::PushOutFamily)
	{


		
		unsigned uid =JsonValueAdapter(result["data"]); 

		m_pGameHall->callPushOutRole(uid);

		unsigned fid = JsonValueAdapter(rpc.m_rpcParam[2]) ;

		std::vector<FamilyRoleBaseInfo>::iterator iter;
		iter = g_globalMyFamilyInfo.m_AllAdmin.begin();

		for (int i=0; i<g_globalMyFamilyInfo.m_AllAdmin.size(); i++)
		{
			if (uid == g_globalMyFamilyInfo.m_AllAdmin.at(i).m_uid)
			{
				g_globalMyFamilyInfo.m_AllAdmin.erase(iter+i);
				m_pGameHall->callUpdateAdminList() ;
			}
		}
		std::vector<FamilyRoleBaseInfo>::iterator iter2;
		iter2 = g_globalMyFamilyInfo.m_allTopRank.begin();
		for (int i=0; i<g_globalMyFamilyInfo.m_allTopRank.size(); i++)
		{
			if (uid == g_globalMyFamilyInfo.m_allTopRank.at(i).m_uid)
			{
				g_globalMyFamilyInfo.m_allTopRank.erase(iter2+i);
				m_pGameHall->callUpdateTop5RankList() ;
			}
		}
		
		
		//如果提的是自己
		if (uid == g_globalMyData.m_iUserId)
		{


			if (fid == g_globalMyData.m_familyId)
			{
				g_globalMyData.setFamilyID(0) ;
				g_globalMyData.setFamilyName("");
				g_globalMyData.setFamilyPower(0);
			}
			m_pGameHall->showSelfClub(false) ;
			PopTipWithBt1("您已退出俱乐部！","确定",nullptr) ;



			std::vector<FamilyInfo>::iterator iter2;
			iter2 = g_globaSelfJoinClub.m_vecSelfJoinClub.begin();
			for (int i=0; i<g_globaSelfJoinClub.m_vecSelfJoinClub.size(); i++)
			{
			if (fid == g_globaSelfJoinClub.m_vecSelfJoinClub.at(i).m_familyID)
				{
					g_globaSelfJoinClub.m_vecSelfJoinClub.erase(iter2+i) ;
					m_pGameHall->callUpdateSelfJoinClubList() ;

				}
			}



			for (auto &club:g_globalFamilyInfo.m_vecFamilyInfo)
			{
				if (club.m_familyID == fid)
				{
					club.m_hasJoin = false ;
					m_pGameHall->callUpdateFamilyRankList() ;
				}
			}

			

		}
		else
		{
			callSocketTips(Notic_PushOutFamily,JsonValueAdapter(rpc.m_rpcParam[3]),"", 0) ;
		}

		Json::Value param;
		param["clubId"] =  rpc.m_rpcParam[2];
		param["ownerId"] = uid ;
		SendClientRequest(ClientRequest::kNoticeBeKickedClub,param) ;
	


	}else
		//加入俱乐部
	if (rpcID == RPCCmd::JoinFamily)
	{

		PopTipWithBt1(result["data"].asString(),"确定",nullptr) ;



	}else
		 //离开俱乐部
		if (rpcID == RPCCmd::LevelFamily)
	{


	}
		//获取俱乐部列表
	else if (rpcID == RPCCmd::GetFamilyList)
	{
		//g_globalTargetFamilyInfo.setFamilyName(JsonValueAdapter(result["auth"]["openid"]));

		//int size = JsonValueAdapter(result["size"]);

		
		
		g_globalFamilyInfo.m_vecFamilyInfo.clear();
		auto& sfamilyData = result["data"];
		g_globalFamilyInfo.familyListSize = sfamilyData.size() ;
		for (int i=0;i<sfamilyData.size();i++)
		{		    
				FamilyInfo quickFollowInfo;
				quickFollowInfo.m_familyName = sfamilyData[i]["name"].asString();
				quickFollowInfo.m_familyID = JsonValueAdapter(sfamilyData[i]["id"]);
				quickFollowInfo.m_familylevel = JsonValueAdapter(sfamilyData[i]["lv"]);
				quickFollowInfo.m_curRoleCount = JsonValueAdapter(sfamilyData[i]["mnum"]);
				quickFollowInfo.m_maxRoleCount = JsonValueAdapter(sfamilyData[i]["max_mnum"]);

				std::string fUrl = sfamilyData[i]["icon"].asString() ;
				//quickFollowInfo.m_familyIconID = JsonValueAdapter(sfamilyData[i]["icon"]);

				quickFollowInfo.m_familyIconID = 0 ;

				quickFollowInfo.m_familyFatherID =  JsonValueAdapter(sfamilyData[i]["header"]);
				quickFollowInfo.m_isOwner = (g_globalMyData.m_iUserId == quickFollowInfo.m_familyFatherID) ;
				quickFollowInfo.m_hasJoin =  JsonValueAdapter(sfamilyData[i]["is_member"]);
				for (int j=0; j< g_globalMyData.m_FamilySkinListCount;j++)
				{

					if (fUrl == g_globalMyData.m_familySkinList[j].m_strIconUrl)
					{
		
						quickFollowInfo.m_familyIconID = j+1 ;
					}

				}


				quickFollowInfo.m_familyRankID=JsonValueAdapter(sfamilyData[i]["rank"]);
				quickFollowInfo.m_familyRankID =i ;
				g_globalFamilyInfo.m_vecFamilyInfo.push_back(quickFollowInfo);

		}

		m_pGameHall->callUpdateFamilyRankList();


	}

	else if (rpcID == RPCCmd::GetMyFamilyInfo||rpcID == RPCCmd::GetTargetFamilyInfo||rpcID == RPCCmd::ShowGetTargetFamilyInfo )
	{
		//获取我的俱乐部数据

		g_globalMyFamilyInfo.cleanMyFamliyInfo() ;
		auto& mfData = result["data"];
		auto& baseInfo = result["data"]["info"];

		g_globalMyFamilyInfo.setFamilyName(baseInfo["name"].asString());
		g_globalMyFamilyInfo.setFamilyID(JsonValueAdapter(baseInfo["id"]));
		g_globalMyFamilyInfo.setCurRank(JsonValueAdapter(baseInfo["rank"]));
		g_globalMyFamilyInfo.setFamilyLevel(JsonValueAdapter(baseInfo["lv"]));
		g_globalMyFamilyInfo.setFamilyCurRole(JsonValueAdapter(baseInfo["mnum"])) ;
		g_globalMyFamilyInfo.setFamilyMaxRole(JsonValueAdapter(baseInfo["max_mnum"]));
		g_globalMyFamilyInfo.setFamilyIconUrl(baseInfo["icon"].asString()) ;
		g_globalMyFamilyInfo.setFamilyNotice(mfData["notice"].asString()) ;

		g_globalMyFamilyInfo.m_fund = JsonValueAdapter(mfData["fund"]) ;
		g_globalMyFamilyInfo.m_ownerUid = JsonValueAdapter(mfData["header"]) ;

		g_globalMyFamilyInfo.m_isOwner = (g_globalMyData.m_iUserId==g_globalMyFamilyInfo.m_ownerUid);

		g_globalMyFamilyInfo.m_AllAdmin.clear() ;
		
		auto& mRanklist = result["data"]["members"];

		g_globalMyFamilyInfo.m_allTopRank.clear();
		for (int i = 0;  i<mRanklist.size(); i++)
		{
			FamilyRoleBaseInfo aRank ;
			aRank.m_uid = JsonValueAdapter(mRanklist[i]["id"]) ;
			aRank.m_uname =  mRanklist[i]["nickname"].asString();
			aRank.m_iconUrl = mRanklist[i]["icon"].asString();
			aRank.m_power = JsonValueAdapter(mRanklist[i]["role"]) ;
			aRank.m_isOwnner = (aRank.m_uid == g_globalMyFamilyInfo.m_ownerUid) ;
			aRank.m_hasCash = JsonValueAdapter(mRanklist[i]["fund"]) ; 
			g_globalMyFamilyInfo.m_allTopRank.push_back(aRank);
		}

		
		/*if (rpcID == RPCCmd::GetMyFamilyInfo)
		{*/
			//g_globalMyData.setFamilyPower(JsonValueAdapter(result["data"]["my_role"])) ;
			m_pGameHall->callUpdateMyFamily();
			/*	}

			if (rpcID == RPCCmd::GetTargetFamilyInfo)
			{
			m_pGameHall->callUpdateTargetFamilyInfo();
			}

			if (rpcID == RPCCmd::ShowGetTargetFamilyInfo)
			{
			m_pGameHall->showTargetFamily(false,true) ;
			m_pGameHall->callUpdateTargetFamilyInfo();
			}*/




	}
	else if (rpcID == RPCCmd::CreatFamily)
	{
		GameData::getInstance()->setFirstCreatFamily(g_globalMyData.m_iUserId,true) ;
		g_globalMyData.setFamilyID(JsonValueAdapter(result["data"]["fid"])) ;
		g_globalMyData.setFamilyName(result["data"]["fname"].asString());
		g_globalMyData.setFamilyPower(JsonValueAdapter(result["role"]));

		Json::Value parm ;
		parm["clubId"] = result["data"]["fid"] ;
		parm["clubname"] = result["data"]["fname"] ;
		parm["ownerId"] = g_globalMyData.m_iUserId;
		SendClientRequest(ClientRequest::kNoticeCreatClub,parm) ;

		//CCLOG("创建俱乐部") ; 
		PopTipWithBt1("恭喜您，成功创建了一个俱乐部", "确定",
			nullptr
		);

		m_pGameHall->showGameClub(true) ;
	}
	else if (rpcID == RPCCmd::GetRegisterCode)
	{
		PopTipWithBt1("获取验证码成功","确定",nullptr) ;
	}
    else if (rpcID == RPCCmd::ResetPassword)
    {
        PopTipWithBt1("修改密码成功","确定",[this](unsigned uBt){
            if (uBt == 0)
            {
                m_pLogonLayerEx->ShowAccountLogon();
                m_pLogonLayerEx->setNewPwd();
            }
        });
    }
	else if (rpcID == RPCCmd::BindPhone)
	{
		g_globalMyData.SetBindPhone(true); 
		g_globalMyData.setBindPhoneNumber(rpc.m_rpcParam[2].asString()) ;
			//g_globalMyData.setBindPhoneCode( msg["phone"].asString()) ;
		PopTipWithBt1("恭喜绑定成功","确定",nullptr) ;

	}
	else if (rpcID == RPCCmd::Regisit)
	{
		//RPC rpc(RPCCmd::Logon, "texas_m_mobile_login", g_globalTempData.m_strLastRegisitPhone, g_globalTempData.m_strLastRegisitPwd, 1);
        RPC rpc(RPCCmd::Logon, "texas_m_user_login", g_globalTempData.m_strLastRegisitPhone, g_globalTempData.m_strLastRegisitPwd, 1);
		m_rpcRoutine.AddRequest(rpc);

		g_globalConfig.SetLastLogonInfo(g_globalTempData.m_strLastRegisitPhone, g_globalTempData.m_strLastRegisitPwd, true, true, false);
	}
	else if (rpcID == RPCCmd::Logon ||
		rpcID == RPCCmd::GuestLogon ||
		rpcID == RPCCmd::AutoLogon ||
		rpcID == RPCCmd::QQLogon ||
		rpcID == RPCCmd::WXLogon||
		rpcID == RPCCmd::SinaLogon||
		rpcID == RPCCmd::The3rdLogon
		
		
		)
	{
		
		GameTipsManager::getInstance()->clean() ;
		g_globalSession.SetOpenID(JsonValueAdapter(result["auth"]["openid"]));
		g_globalSession.SetOpenKey(JsonValueAdapter(result["auth"]["openkey"]));
		g_globalSession.SetSerectKey(JsonValueAdapter(result["secret"]));
		g_globalSession.SetTimeStamp(JsonValueAdapter(result["auth"]["timestamp"]));

		auto& userInfo = result["data"]["userinfo"];
		g_globalConfig.SetLastLogonResult(true, userInfo["username"].asString(), userInfo["password"].asString());
		g_globalMyData.m_iUserId = JsonValueAdapter(userInfo["id"]);
		g_globalMyData.SetNickName(userInfo["nickname"].asString());
        g_globalMyData.SetIconURL(userInfo["icon"].asString());
		g_globalMyData.setBindPhoneNumber(userInfo["phone"].asString()) ;

		g_globalMyData.SetCash(JsonValueAdapter(userInfo["beans"]));
        g_globalMyData.SetDiamond(JsonValueAdapter(userInfo["diamond"]));
		g_globalMyData.setSngMasterScore(JsonValueAdapter(userInfo["maseterscore"]));
		g_globalMyData.setSngMasterRank(JsonValueAdapter(userInfo["maseterrank"])) ;

		g_globalMyData.setMttMasterScore(JsonValueAdapter(userInfo["maseterscore_mtt"]));
		g_globalMyData.setMttMasterRank(JsonValueAdapter(userInfo["mttmaseterrank"])) ;


		g_globalMyData.SetVipLevel(JsonValueAdapter(userInfo["vip_lv"]));

		g_globalMyData.m_strQQ = userInfo["qq"].asString();
		g_globalMyData.m_strEmail = userInfo["email"].asString();
		g_globalMyData.SetGender(JsonValueAdapter(userInfo["gender"]));
        //兑换人信息
        g_globalMyAddr.m_strXingMing = userInfo["realname"].asString();
        g_globalMyAddr.m_strShouJi = userInfo["phone2"].asString();
        g_globalMyAddr.m_strDiZhi = userInfo["address"].asString();
        g_globalMyAddr.m_strQQ = userInfo["qq"].asString();
        g_globalMyAddr.m_strEmail = userInfo["email"].asString();

		//俱乐部数据

		g_globalMyData.setFamilyID(JsonValueAdapter(result["data"]["fid"])) ;
		g_globalMyData.setFamilyName(result["data"]["fname"].asString());
		//g_globalMyData.setFamilyPower(JsonValueAdapter(result["data"]["role"]));


		auto& skinLst = result["data"]["skinlist"];
		g_globalMyData.m_uSkinListCount = 0 ;
		g_globalMyData.m_userSkinList.clear() ;

		char* skinIconPath[] = {
			"defaulticon.png",
			"defaulticon_nv.png"
		} ; 

		char* skinXXPath[] = {
			"hall/xingxiang2.png",
			"hall/xingxiang1.png",
		} ; 
			

		
		
		for (unsigned i = 0; i < skinLst.size(); i++)
		{
			UserSkin skin ;
			skin.m_uID = JsonValueAdapter(skinLst[i]["id"]);
			skin.m_strIconUrl = skinLst[i]["icon"].asString();
			skin.m_strAvatorUrl = skinLst[i]["avator"].asString();
			skin.m_strVoiceDir = skinLst[i]["voice"].asString();
			skin.m_strAvatorBgUrl = skinLst[i]["bg"].asString();
			g_globalMyData.m_userSkinList.push_back(skin);
			++g_globalMyData.m_uSkinListCount;
		
		}
        auto& activityState = result["data"]["actioneffective"];
        for (unsigned i=0; i<activityState.size(); i++)
        {
            ActivityState state ;
            state.m_activityName = activityState[i]["name"].asString();
            state.m_state = JsonValueAdapter(activityState[i]["status"]);
            state.m_strURL = activityState[i]["url"].asString();
            //state.m_uIndex = JsonValueAdapter(activityState[i]["status"]);
            g_globalMyData.m_vecActivityState.push_back(state);
        }





		g_globalMyData.m_FamilySkinListCount = 0 ;
		auto& skinfamilyLst = result["data"]["ficonlist"];
		for (unsigned i = 0; i < skinfamilyLst.size(); i++)
		{
			FamilySkin& skinf = g_globalMyData.m_familySkinList[i];
			skinf.m_fuid = i+1;
			skinf.m_strIconUrl = skinfamilyLst[i].asString();
			++g_globalMyData.m_FamilySkinListCount;
		}


		g_globalMyData.SetSkinID(JsonValueAdapter(result["data"]["skin"]));
		g_globalMyData.SetVoiceID(JsonValueAdapter(result["data"]["skin"]));

		g_globalServerInfo.SetGameServerInfo(JsonValueAdapter(result["data"]["server"]["conn_server_ip"]["ip"]), JsonValueAdapter(result["data"]["server"]["conn_server_ip"]["port"]));
		std::string strAddr = result["data"]["server"]["chat_server_ip"].asString();
		auto iterSep = strAddr.begin() + strAddr.find(':');
		g_globalServerInfo.m_strChatIp = std::string(strAddr.begin(), iterSep);
		g_globalServerInfo.m_uChatPort = atoi(std::string(++iterSep, strAddr.end()).c_str());

		
		auto& ratioConfig = result["data"]["fconf"];
		//载入俱乐部福利数据
		for (int i=0; i<10 ;i++)
		{
			g_globalRatio.m_dataRatio[i] = new DataRatio;
			g_globalRatio.m_dataRatio[i]->m_personRatio =JsonValueAdapter(ratioConfig[i]["user_return_ratio"]);
			g_globalRatio.m_dataRatio[i]->m_fatherRatio =JsonValueAdapter(ratioConfig[i]["header_return_ratio"]);
			g_globalRatio.m_dataRatio[i]->maxRole =JsonValueAdapter(ratioConfig[i]["max_members"]);
			
		}
		g_globalMyData.setuseMagicExpress(false);
		g_globalMyData.setUseExpress(false) ;
		SendClientRequest(ClientRequest::kCallExpressPower) ;
	    //SendClientRequest(ClientRequest::kCallExchangeInfo);
		
		readData() ;

        SendClientRequest(ClientRequest::kCallGuideTipsInfo);//获取功能引导信息

#if TEST_IN
		RPC rpc1(RPCCmd::GetNewTaskInfo, "texas_new_tasklist", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, g_globalServerInfo.m_iGameID);
#else
		RPC rpc1(RPCCmd::GetNewTaskInfo, "texas_new_tasklist", g_globalMyData.m_iUserId, g_globalSession.m_szSecret, g_globalServerInfo.m_iGameID);
#endif
		m_rpcRoutine.AddRequest(rpc1);


		RPC rpc2(RPCCmd::GetNewShopPrice, "texas_m_new_gifts_conf", g_globalMyData.m_iUserId, g_globalSession.m_szSecret);
		m_rpcRoutine.AddRequest(rpc2);


		RPC rpc3(RPCCmd::GetVIPPrice, "texas_m_vip_rchglist", g_globalMyData.m_iUserId, g_globalSession.m_szSecret);
		m_rpcRoutine.AddRequest(rpc3);


		ConnectGameServer();
	}

	//else if (rpcID == RPCCmd::EditMyInfo)
	//{
	//	unsigned uOp = JsonValueAdapter(result["data"]["type"]);
	//	std::string strData = result["data"]["data"].asString();
	//	/*
	//	当type为1时，修改用户的【头像】
	//	当type为2时，修改用户的【昵称】
	//	当type为3时，修改用户的【性别】
	//	当type为4时，修改用户的【QQ号码】
	//	当type为5时，修改用户的【邮箱】
 //       当type为6时，修改用户的【收货人】
 //       当type为7时，修改用户的【手机号码】
 //       当type为8时，修改用户的【所在地区】
	//	*/
	//	if (uOp == 1)
	//	{
	//		g_globalMyData.SetSkinID(atoi(strData.c_str()));
	//		//m_pGameHall->UpdateMyInfo();
	//	}
	//	else if (uOp == 2)
	//	{
	//		g_globalMyData.SetNickName(strData);
	//		//m_pGameHall->UpdateMyInfo();
	//	}
	//	else if (uOp == 3)
	//	{
	//		g_globalMyData.SetGender(atoi(strData.c_str()));
	//	}
	//	else if (uOp == 4)
	//	{
	//		g_globalMyData.m_strQQ = strData;
 //           g_globalMyAddr.m_strQQ = strData;
	//	}
	//	else if (uOp == 5)
	//	{
	//		g_globalMyData.m_strEmail = strData;
 //           g_globalMyAddr.m_strEmail = strData;
	//	}
 //       else if (uOp == 6)
 //       {
 //           g_globalMyAddr.m_strXingMing = strData;
 //       }
 //       else if (uOp == 7)
 //       {
 //           g_globalMyAddr.m_strShouJi = strData;
 //       }
 //       else if (uOp == 8)
 //       {
 //           g_globalMyAddr.m_strDiZhi = strData;
 //       }
	//}
	else if (rpcID == RPCCmd::GetNewTaskInfo)
	{
        //每日任务
		std::vector<MissionInfo> vecMissionInfo;
		auto data = result["data"]["daily"];
		for (int i = 0; i < data.size(); i++)
		{
			auto& taskInfo = data[i];
			MissionInfo missionInfo;
			missionInfo.uMissionID = JsonValueAdapter(taskInfo["task_id"]);
			missionInfo.uMissionType = 0; // 0日常任务 1新手任务 2成就任务 3本周任务
			missionInfo.uMissionDrawID = JsonValueAdapter(taskInfo["utask_id"]); // 领取ID
			missionInfo.strMissionName = taskInfo["name"].asString();
			missionInfo.strMissionDesc = taskInfo["description"].asString();
			missionInfo.strMissionIcon = taskInfo["icon"].asString();
			missionInfo.uCurStep = JsonValueAdapter(taskInfo["schedule"]);
			missionInfo.uAllStep = JsonValueAdapter(taskInfo["total"]);
			missionInfo.uMissonStatus = JsonValueAdapter(taskInfo["status"]); // 0未完成 1已完成 2已领取
			missionInfo.bHasDraw = JsonValueAdapter(taskInfo["is_reward"]); // 是否已经领取
            missionInfo.uScence = JsonValueAdapter(taskInfo["scence"]);//所属场次
			// missionInfo.missionTag = taskInfo["tags"];
			auto&& tagName = taskInfo["tags"].getMemberNames();
			if (!tagName.empty())
			{
				missionInfo.strMissionTagName = tagName.front();
				missionInfo.uMissionTagValue = JsonValueAdapter(taskInfo[missionInfo.strMissionTagName]);
			}
			missionInfo.uMissionTag = JsonValueAdapter(taskInfo["is_reward"]);
			missionInfo.uAcceptLevel = JsonValueAdapter(taskInfo["accept_level"]);
			missionInfo.dAward = JsonValueAdapter(taskInfo["award"]["texas_beans"]);
            missionInfo.strRewardName = taskInfo["award"]["name"].asString();
			missionInfo.bHasChildMission = JsonValueAdapter(taskInfo["pid"]);
			missionInfo.uNextMissionID = JsonValueAdapter(taskInfo["next"]);
			missionInfo.bTipNextMission = JsonValueAdapter(taskInfo["next_cue"]);
			vecMissionInfo.push_back(missionInfo);
		}
		g_globalMissionInfo.GetMissions(0) = vecMissionInfo;

        //本周任务
        vecMissionInfo.clear();
        //std::vector<MissionInfo> vecMissionInfo;
        data = result["data"]["week"];
        for (int i = 0; i < data.size(); i++)
        {
            auto& taskInfo = data[i];
            MissionInfo missionInfo;
            missionInfo.uMissionID = JsonValueAdapter(taskInfo["task_id"]);
            missionInfo.uMissionType = 0; // 0日常任务 1新手任务 2成就任务 3本周任务
            missionInfo.uMissionDrawID = JsonValueAdapter(taskInfo["utask_id"]); // 领取ID
            missionInfo.strMissionName = taskInfo["name"].asString();
            missionInfo.strMissionDesc = taskInfo["description"].asString();
            missionInfo.strMissionIcon = taskInfo["icon"].asString();
            missionInfo.uCurStep = JsonValueAdapter(taskInfo["schedule"]);
            missionInfo.uAllStep = JsonValueAdapter(taskInfo["total"]);
            missionInfo.uMissonStatus = JsonValueAdapter(taskInfo["status"]); // 0未完成 1已完成 2已领取
            missionInfo.bHasDraw = JsonValueAdapter(taskInfo["is_reward"]); // 是否已经领取
            missionInfo.uScence = JsonValueAdapter(taskInfo["scence"]);//所属场次
            // missionInfo.missionTag = taskInfo["tags"];
            auto&& tagName = taskInfo["tags"].getMemberNames();
            if (!tagName.empty())
            {
                missionInfo.strMissionTagName = tagName.front();
                missionInfo.uMissionTagValue = JsonValueAdapter(taskInfo[missionInfo.strMissionTagName]);
            }
            missionInfo.uMissionTag = JsonValueAdapter(taskInfo["is_reward"]);
            missionInfo.uAcceptLevel = JsonValueAdapter(taskInfo["accept_level"]);
            missionInfo.dAward = JsonValueAdapter(taskInfo["award"]["texas_beans"]);
            missionInfo.strRewardName = taskInfo["award"]["name"].asString();
            missionInfo.bHasChildMission = JsonValueAdapter(taskInfo["pid"]);
            missionInfo.uNextMissionID = JsonValueAdapter(taskInfo["next"]);
            missionInfo.bTipNextMission = JsonValueAdapter(taskInfo["next_cue"]);
            vecMissionInfo.push_back(missionInfo);
        }
        g_globalMissionInfo.GetMissions(3) = vecMissionInfo;

		// 新手任务
		vecMissionInfo.clear();
		//std::vector<MissionInfo> vecMissionInfo;
		data = result["data"]["newer"];
		for (int i = 0; i < data.size(); i++)
		{
			auto& taskInfo = data[i];
			MissionInfo missionInfo;
			missionInfo.uMissionID = JsonValueAdapter(taskInfo["task_id"]);
			missionInfo.uMissionType = 1; // 0日常任务 1新手任务 2成就任务
			missionInfo.uMissionDrawID = JsonValueAdapter(taskInfo["utask_id"]); // 领取ID
			missionInfo.strMissionName = taskInfo["name"].asString();
			missionInfo.strMissionDesc = taskInfo["description"].asString();
			missionInfo.strMissionIcon = taskInfo["icon"].asString();
			missionInfo.uCurStep = JsonValueAdapter(taskInfo["schedule"]);
			missionInfo.uAllStep = JsonValueAdapter(taskInfo["total"]);
			missionInfo.uMissonStatus = JsonValueAdapter(taskInfo["status"]); // 0未完成 1已完成 2已领取
			missionInfo.bHasDraw = JsonValueAdapter(taskInfo["is_reward"]); // 是否已经领取
            missionInfo.uScence = JsonValueAdapter(taskInfo["scence"]);//所属场次
			// missionInfo.missionTag = taskInfo["tags"];
			auto&& tagName = taskInfo["tags"].getMemberNames();
			if (!tagName.empty())
			{
				missionInfo.strMissionTagName = tagName.front();
				missionInfo.uMissionTagValue = JsonValueAdapter(taskInfo["tags"][missionInfo.strMissionTagName]);
			}
			missionInfo.uMissionTag = JsonValueAdapter(taskInfo["is_reward"]);
			missionInfo.uAcceptLevel = JsonValueAdapter(taskInfo["accept_level"]);
			missionInfo.dAward = JsonValueAdapter(taskInfo["award"]["texas_beans"]);
            missionInfo.strRewardName = taskInfo["award"]["name"].asString();
			missionInfo.bHasChildMission = JsonValueAdapter(taskInfo["pid"]);
			missionInfo.uNextMissionID = JsonValueAdapter(taskInfo["next"]);
			missionInfo.bTipNextMission = JsonValueAdapter(taskInfo["next_cue"]);

			missionInfo.strNextMissionName = taskInfo["next_name"].asString();
			missionInfo.strNextMissionDescription = taskInfo["next_description"].asString();
			vecMissionInfo.push_back(missionInfo);
		}
		g_globalMissionInfo.GetMissions(1) = vecMissionInfo;

		// 成就任务
		vecMissionInfo.clear();
		//std::vector<MissionInfo> vecMissionInfo;
		data = result["data"]["achievement"];
		for (int i = 0; i < data.size(); i++)
		{
			auto& taskInfo = data[i];
			MissionInfo missionInfo;
			missionInfo.uMissionID = JsonValueAdapter(taskInfo["task_id"]);
			missionInfo.uMissionType = 2; // 0日常任务 1新手任务 2成就任务
			missionInfo.uMissionDrawID = JsonValueAdapter(taskInfo["utask_id"]); // 领取ID
			missionInfo.strMissionName = taskInfo["name"].asString();
			missionInfo.strMissionDesc = taskInfo["description"].asString();
			missionInfo.strMissionIcon = taskInfo["icon"].asString();
			missionInfo.uCurStep = JsonValueAdapter(taskInfo["schedule"]);
			missionInfo.uAllStep = JsonValueAdapter(taskInfo["total"]);
			missionInfo.uMissonStatus = JsonValueAdapter(taskInfo["status"]); // 0未完成 1已完成 2已领取
			missionInfo.bHasDraw = JsonValueAdapter(taskInfo["is_reward"]); // 是否已经领取
            missionInfo.uScence = JsonValueAdapter(taskInfo["scence"]);//所属场次
			// missionInfo.missionTag = taskInfo["tags"];
			auto&& tagName = taskInfo["tags"].getMemberNames();
			if (!tagName.empty())
			{
				missionInfo.strMissionTagName = tagName.front();
				missionInfo.uMissionTagValue = JsonValueAdapter(taskInfo[missionInfo.strMissionTagName]);
			}
			
			missionInfo.uMissionTag = JsonValueAdapter(taskInfo["is_reward"]);
			missionInfo.uAcceptLevel = JsonValueAdapter(taskInfo["accept_level"]);
			missionInfo.dAward = JsonValueAdapter(taskInfo["award"]["texas_beans"]);
            missionInfo.strRewardName = taskInfo["award"]["name"].asString();
			missionInfo.bHasChildMission = JsonValueAdapter(taskInfo["pid"]);
			missionInfo.uNextMissionID = JsonValueAdapter(taskInfo["next"]);
			missionInfo.bTipNextMission = JsonValueAdapter(taskInfo["next_cue"]);

			vecMissionInfo.push_back(missionInfo);
		}
		g_globalMissionInfo.GetMissions(2) = vecMissionInfo;
	}

	else if (rpcID == RPCCmd::GetVIPPrice)
	{
	//		VIPInfo info[6] = {
	//	{100000, 35, 3000, 36, false, false, 200, "关闭", 10},
	//	{350000, 75, 10000, 58, true, true, 300, "关闭", 20},
	//	{700000, 180, 20000, 88, true, true, 500, "关闭", 30},
	//	{1500000, 460, 30000, 128, true, true, 800, "关闭", 40},
	//	{4000000, 1200, 60000, 168, true, true, 1000, "开启", 50},
	//	{11000000, 2000, 150000, 288, true, true, 2000, "开启\n可自定义", 60}
	//};
		static double s_dZengSongLeBi[] = {100000, 350000, 700000, 1500000, 4000000, 11000000};
		static unsigned s_uZengJaiJingYan[] = {35, 75, 180, 460, 1200, 2000};
		static double s_dDengLuFanHuan[] = {3000, 10000, 20000, 30000, 60000, 150000};
		static unsigned s_uAddMeiLi[] = {36, 58, 88, 128, 168, 288};
		static bool s_bOpenGuiZuBiaoQing[] = {false, true, true, true, true, true};
		static bool s_bOpenGuiZuMoFaBiaoQing[] = {false, true, true, true, true, true};
		static unsigned s_uJiLuPaiJu[] = {200, 300, 500, 800, 1000, 2000};
		static bool s_bOpenSiMiKaiFang[] = {true, true, true, true, true, true};
		static bool s_bOpenZiDingYiKaiFang[] = {false, false, false, false, true, true};
		auto& vipPrices = result["data"]["rchglist"];
		auto& vipname = result["data"]["name"];
		for (unsigned i = 0; i < 5; i++)
		{
			g_globalMallInfo.m_vipInfo[i].m_dAddLeBi = s_dZengSongLeBi[i];
			g_globalMallInfo.m_vipInfo[i].m_uAddJingYan = s_uZengJaiJingYan[i];
			g_globalMallInfo.m_vipInfo[i].m_dDengLuFanHuan = s_dDengLuFanHuan[i];
			g_globalMallInfo.m_vipInfo[i].m_uAddMeiLi = s_uAddMeiLi[i];
			g_globalMallInfo.m_vipInfo[i].m_bOpenGuiZuBiaoQing = s_bOpenGuiZuBiaoQing[i];
			g_globalMallInfo.m_vipInfo[i].m_bOpenGuiZuMoFaBiaoQing = s_bOpenGuiZuMoFaBiaoQing[i];
			g_globalMallInfo.m_vipInfo[i].m_uJiLuPaiJu = s_uJiLuPaiJu[i];
			g_globalMallInfo.m_vipInfo[i].m_bOpenSiMiKaiFang = s_bOpenSiMiKaiFang[i];
			g_globalMallInfo.m_vipInfo[i].m_bOpenZiDingYiKaiFang = s_bOpenZiDingYiKaiFang[i];
			g_globalMallInfo.m_vipInfo[i].m_dPrice = JsonValueAdapter(vipPrices[i]);
			g_globalMallInfo.m_vipInfo[i].m_strVipName = vipname[i].asString();
		}
	}
	else if (rpcID == RPCCmd::GetYinLianTradeID)
	{
		PopPaymentLoading(false);
		std::string strYinLianTradeID = result["data"].asString();
		MobilePay(0, strYinLianTradeID,  JsonValueAdapter(rpc.m_rpcParam[5]), "");
	}
	else if (rpcID == RPCCmd::GetAliPayTradeID)
	{
		PopPaymentLoading(false);
		std::string strYinLianTradeID = result["data"].asString();
		std::string strName = rpc.m_rpcParam[9].asString();
		MobilePay(1, strYinLianTradeID,  JsonValueAdapter(rpc.m_rpcParam[3]), strName);
	}
	else if (rpcID == RPCCmd::GetWeixinPayTradeID)
	{
		PopPaymentLoading(false);
		auto data = result["data"];
		std::string strWeixinTradeID = data["prepayid"].asString();
		std::string strName = rpc.m_rpcParam[9].asString();
		MobilePay(2, strWeixinTradeID,  JsonValueAdapter(rpc.m_rpcParam[5]), strName);
	}
	else if (rpcID == RPCCmd::Get3rdTradeID)
	{
		PopPaymentLoading(false);
		auto data = result["data"];
		std::string strTradeID = result["data"].asString();
		std::string strName = rpc.m_rpcParam[9].asString();
		unsigned itemid =  JsonValueAdapter(rpc.m_rpcParam[8]) ;
		double dPrice = JsonValueAdapter(rpc.m_rpcParam[3]) ;
		double index = JsonValueAdapter(rpc.m_rpcParam[6]) ;
		char str[256] = {0};
		sprintf(str, "%.2f", dPrice);

		//RPC rpc(RPCCmd::Get3rdTradeID, "m_get_order_no", g_globalMyData.m_iUserId, g_globalMyData.m_iUserId, "malipay", dPrice, "", 0, 0, GAME_ID, uID,strName);


		Json::Value text;
		text["uid"] = g_globalMyData.m_iUserId;
		text["uname"] = g_globalMyData.GetNickName();
		text["strTradeID"] =  strTradeID;
		text["itemName"] = strName;
		text["itemprice"] = str;
		text["itemid"] = itemid;
		text["index"] = index;
		Json::FastWriter fast_writer2;
		std::string strBody = fast_writer2.write(text);
		callGameBilling(strBody);
	}
    else if (rpcID == RPCCmd::GetAppstoreTradeID)
    {
        PopPaymentLoading(false);
        auto data = result["data"];
        std::string strTradeID = result["data"].asString();
        std::string strName = rpc.m_rpcParam[9].asString();
        unsigned itemid =  JsonValueAdapter(rpc.m_rpcParam[8]) ;
        double dPrice = JsonValueAdapter(rpc.m_rpcParam[3]) ;
        double index = JsonValueAdapter(rpc.m_rpcParam[5]) ;
        char str[256] = {0};
        sprintf(str, "%.2f", dPrice);
        
        //RPC rpc(RPCCmd::Get3rdTradeID, "m_get_order_no", g_globalMyData.m_iUserId, g_globalMyData.m_iUserId, "malipay", dPrice, "", 0, 0, GAME_ID, uID,strName);
        
        
        Json::Value text;
        text["uid"] = g_globalMyData.m_iUserId;
        text["uname"] = g_globalMyData.GetNickName();
        text["strTradeID"] =  strTradeID;
        text["itemName"] = strName;
        text["itemprice"] = str;
        text["itemid"] = itemid;
        text["index"] = index;
        Json::FastWriter fast_writer2;
        std::string strBody = fast_writer2.write(text);
        callGameBilling(strBody);
    }
	else if (rpcID == RPCCmd::UpdateTask)
	{
		unsigned uTaskID = JsonValueAdapter(result["data"]["task_id"]);
		unsigned uTaskStep = JsonValueAdapter(result["data"]["schedule"]);
		unsigned uTaskStatus = JsonValueAdapter(result["data"]["status"]);
        unsigned uCategory = JsonValueAdapter(result["data"]["category"]);
		bool bNeedUpdate = false;
		//auto mission = g_globalMissionInfo.GetMission(uTaskID);
        /*if (mission)
        {
        mission->uCurStep = uTaskStep;
        mission->uMissonStatus = uTaskStatus;
        bNeedUpdate = true;
        }*/
		unsigned uMissionType = GetMissionType(uCategory);
		for (auto& mission : g_globalMissionInfo.GetMissions(uMissionType))
		{
			if (mission.uMissionID == uTaskID)
			{
				mission.uCurStep = uTaskStep;
				mission.uMissonStatus = uTaskStatus;
				bNeedUpdate = true;
				break;
			}
		}
		if (bNeedUpdate)
		{
			// TODO
			if (m_pGameTable)
			{
				m_pGameTable->UpdateMissionList();
				m_pGameTable->UpdateItemBox();
                if (uTaskID>=2036 && uTaskID<=2053)
                {
                    if (m_bJumpOverGuide && uTaskID == 2036)
                    {}
                    else
                    {
                        if (m_pNewPlayerTask == nullptr)
                            PopNewerTask();
                        m_pNewPlayerTask->UpdateTask(true);
                    }
                        
                }
                    //m_pGameTable->UpdateNewPlayerTask();
			}			

			if (m_pGameHall != nullptr)
				m_pGameHall->UpdateMissionInfo();
		}
	}
	else if (rpcID == RPCCmd::DrawMissionAward)
	{
		unsigned uMissionID = JsonValueAdapter(result["task_id"]);
		auto mission = g_globalMissionInfo.GetMission(uMissionID);
		double dDrawAward = -1;
		if (mission)
		{
			mission->bHasDraw = true;
			mission->uMissonStatus = 2;
			dDrawAward = mission->dAward;
		}

		if (dDrawAward > 0)
		{
            m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallUpdateCashAndVip);
		}
        unsigned uCategory = GetMissionType(JsonValueAdapter(result["category"]));
        unsigned uScence = JsonValueAdapter(result["scence"]);
		auto awardInfo = result["award_info"];
		double dAward = JsonValueAdapter(awardInfo["texas_beans"]);
		auto nextMask = result["next_task"];
		if (nextMask.size() > 0)
		{
			auto taskInfo = nextMask[0u];
			MissionInfo missionInfo;
			missionInfo.uMissionID = JsonValueAdapter(taskInfo["task_id"]);
			missionInfo.uMissionDrawID = JsonValueAdapter(taskInfo["utask_id"]); // 领取ID
			missionInfo.strMissionName = taskInfo["name"].asString();
			missionInfo.strMissionDesc = taskInfo["description"].asString();
			missionInfo.strMissionIcon = taskInfo["icon"].asString();
			missionInfo.uCurStep = JsonValueAdapter(taskInfo["schedule"]);
			missionInfo.uAllStep = JsonValueAdapter(taskInfo["total"]);
			missionInfo.uMissonStatus = JsonValueAdapter(taskInfo["status"]); // 0未完成 1已完成 2已领取
			missionInfo.bHasDraw = JsonValueAdapter(taskInfo["is_reward"]); // 是否已经领取
			auto&& tagName = taskInfo["tags"].getMemberNames();
			if (!tagName.empty())
			{
				missionInfo.strMissionTagName = tagName.front();
				missionInfo.uMissionTagValue = JsonValueAdapter(taskInfo["tags"][missionInfo.strMissionTagName]);
			}

			missionInfo.uMissionTag = JsonValueAdapter(taskInfo["is_reward"]);
			missionInfo.uAcceptLevel = JsonValueAdapter(taskInfo["accept_level"]);
			missionInfo.dAward = JsonValueAdapter(taskInfo["award"]["texas_beans"]);
			missionInfo.bHasChildMission = JsonValueAdapter(taskInfo["pid"]);
			missionInfo.uNextMissionID = JsonValueAdapter(taskInfo["next"]);
			missionInfo.bTipNextMission = JsonValueAdapter(taskInfo["next_cue"]);
			//missionInfo.uMissionType = GetMissionType(JsonValueAdapter(taskInfo["category"]));
            missionInfo.strNextMissionName = taskInfo["next_name"].asString();
            missionInfo.strNextMissionDescription = taskInfo["next_description"].asString();
            missionInfo.uScence = uScence;
            if (uCategory == 1)
            {
                g_globalMissionInfo.GetMissions(uCategory).clear();
            }

			g_globalMissionInfo.GetMissions(uCategory).push_back(missionInfo);
		}

		if (m_pGameTable)
		{
			m_pGameTable->UpdateMissionList();
			m_pGameTable->UpdateItemBox();
		}
		m_pGameHall->UpdateMissionInfo();
	}
	else if (rpcID == RPCCmd::GetFollowList)
	{
		unsigned uType = JsonValueAdapter(result["type"]);
		unsigned uPage = JsonValueAdapter(result["pageid"]);
		unsigned uMaxPage = JsonValueAdapter(result["maxpage"]);
		auto list = result["data"];
		auto& vecFollowUserInfo = g_globalFollowUserInfo.m_vecFollowUser[uType-1];
		vecFollowUserInfo.clear();
		for (unsigned i = 0; i < list.size(); i++)
		{
			auto& user = list[i];
			FollowUserInfo followUserInfo;
			followUserInfo.m_uID = JsonValueAdapter(user["id"]);
			followUserInfo.m_strIcon = user["icon"].asString();
			followUserInfo.m_strNickName = user["nickname"].asString();
			followUserInfo.m_dCash = JsonValueAdapter(user["beans"]);
            followUserInfo.m_uAttendStatus = JsonValueAdapter(user["guanzhi"]);
			vecFollowUserInfo.push_back(followUserInfo);
		}
		g_globalFollowUserInfo.m_uPageInfo[uType-1][0] = uPage;
		g_globalFollowUserInfo.m_uPageInfo[uType-1][1] = uMaxPage;
		m_pGameHall->UpdateFollowList(uType-1);
	}
	else if(rpcID == RPCCmd::GetWeekWinPotRank)
	{
		unsigned uWinType = 0;
		unsigned uRankType = JsonValueAdapter(result["type"]);
		unsigned uOff = uWinType * 2 + uRankType - 1;
		unsigned uPage = JsonValueAdapter(result["pageid"]);
		unsigned uMaxPage = JsonValueAdapter(result["maxpage"]);
		
		
		auto& list = result["data"];
		auto& vecRank = g_globalRankUserInfo.m_vecRankList[uOff];
		vecRank.clear();
		for (unsigned i = 0; i < list.size(); i++)
		{
			auto& user = list[i];
			RankUserInfo userInfo;
			userInfo.m_uID = JsonValueAdapter(user["id"]);
			userInfo.m_strIconUrl = user["icon"].asString();
			userInfo.m_strNickName = user["nickname"].asString();
			userInfo.m_dWinValue = JsonValueAdapter(user["score"]);
			userInfo.m_uRank = (uPage - 1) * list.size() + i;
			vecRank.push_back(userInfo);
		}
		g_globalRankUserInfo.m_uPageInfo[uOff][0] = uPage;
		g_globalRankUserInfo.m_uPageInfo[uOff][1] = uMaxPage;
		m_pGameHall->UpdateRankList(uRankType - 1, uWinType);
	}
	else if(rpcID == RPCCmd::GetWeekWinCashRank)
	{
		unsigned uWinType = 1;
		unsigned uRankType = JsonValueAdapter(result["type"]);
		unsigned uOff = uWinType * 2 + uRankType - 1;
		unsigned uPage = JsonValueAdapter(result["pageid"]);
		unsigned uMaxPage = JsonValueAdapter(result["maxpage"]);
		
		
		auto& list = result["data"];
		auto& vecRank = g_globalRankUserInfo.m_vecRankList[uOff];
		vecRank.clear();
		for (unsigned i = 0; i < list.size(); i++)
		{
			auto& user = list[i];
			RankUserInfo userInfo;
			userInfo.m_uID = JsonValueAdapter(user["id"]);
			userInfo.m_strIconUrl = user["icon"].asString();
			userInfo.m_strNickName = user["nickname"].asString();
			userInfo.m_dWinValue = JsonValueAdapter(user["score"]);
			userInfo.m_uRank = (uPage - 1) * list.size() + i;
			vecRank.push_back(userInfo);
		}
		g_globalRankUserInfo.m_uPageInfo[uOff][0] = uPage;
		g_globalRankUserInfo.m_uPageInfo[uOff][1] = uMaxPage;
		m_pGameHall->UpdateRankList(uRankType - 1, uWinType);
	}
	else if(rpcID == RPCCmd::GetDailyWinCashRank)
	{
		unsigned uWinType = 2;
		unsigned uRankType = JsonValueAdapter(result["type"]);
		unsigned uOff = uWinType * 2 + uRankType - 1;
		unsigned uPage = JsonValueAdapter(result["pageid"]);
		unsigned uMaxPage = JsonValueAdapter(result["maxpage"]);
		
		
		auto& list = result["data"];
		auto& vecRank = g_globalRankUserInfo.m_vecRankList[uOff];
		vecRank.clear();
		for (unsigned i = 0; i < list.size(); i++)
		{
			auto& user = list[i];
			RankUserInfo userInfo;
			userInfo.m_uID = JsonValueAdapter(user["id"]);
			userInfo.m_strIconUrl = user["icon"].asString();
			userInfo.m_strNickName = user["nickname"].asString();
			userInfo.m_dWinValue = JsonValueAdapter(user["score"]);
			userInfo.m_uRank = (uPage - 1) * list.size() + i;
			vecRank.push_back(userInfo);
		}
		g_globalRankUserInfo.m_uPageInfo[uOff][0] = uPage;
		g_globalRankUserInfo.m_uPageInfo[uOff][1] = uMaxPage;
		m_pGameHall->UpdateRankList(uRankType - 1, uWinType);
	}
	else if(rpcID == RPCCmd::GetCashRank)
	{
		unsigned uWinType = 3;
		unsigned uRankType = JsonValueAdapter(result["type"]);
		unsigned uOff = uWinType * 2 + uRankType - 1;
		unsigned uPage = JsonValueAdapter(result["pageid"]);
		unsigned uMaxPage = JsonValueAdapter(result["maxpage"]);
		
		
		auto& list = result["data"];
		auto& vecRank = g_globalRankUserInfo.m_vecRankList[uOff];
		vecRank.clear();
		for (unsigned i = 0; i < list.size(); i++)
		{
			auto& user = list[i];
			RankUserInfo userInfo;
			userInfo.m_uID = JsonValueAdapter(user["id"]);
			userInfo.m_strIconUrl = user["icon"].asString();
			userInfo.m_strNickName = user["nickname"].asString();
			userInfo.m_dWinValue = JsonValueAdapter(user["score"]);
			userInfo.m_uRank = (uPage - 1) * list.size() + i;
			vecRank.push_back(userInfo);
		}
		g_globalRankUserInfo.m_uPageInfo[uOff][0] = uPage;
		g_globalRankUserInfo.m_uPageInfo[uOff][1] = uMaxPage;
		m_pGameHall->UpdateRankList(uRankType - 1, uWinType);
	}

    else if (rpcID == RPCCmd::GetSngScoreRank)
    {
        unsigned uWinType = 1;
        unsigned uRankType = JsonValueAdapter(result["type"]);
        unsigned uOff = uWinType * 2 + uRankType - 1;
        unsigned uPage = JsonValueAdapter(result["pageid"]);
        unsigned uMaxPage = JsonValueAdapter(result["maxpage"]);

        auto& list = result["data"];
        auto& vecRank = g_globalSngRankInfo.m_vecSngRankList[uOff];
        vecRank.clear();
        for (unsigned i = 0; i < list.size(); i++)
        {
            auto& user = list[i];
            SngRankInfo userInfo;
            userInfo.m_uID = JsonValueAdapter(user["id"]);
            userInfo.m_strIconUrl = user["icon"].asString();
            userInfo.m_strNickName = user["nickname"].asString();
            userInfo.m_dWinValue = JsonValueAdapter(user["score"]);
            userInfo.m_uRank = (uPage - 1) * list.size() + i;
            vecRank.push_back(userInfo);
        }
        g_globalSngRankInfo.m_uPageInfo[uOff][0] = uPage;
        g_globalSngRankInfo.m_uPageInfo[uOff][1] = uMaxPage;

		UpdateSngRankList(uRankType - 1, uWinType);

    }
	else if (rpcID == RPCCmd::GetMttScoreRank)
	{
		unsigned uWinType = 2;
		unsigned uRankType = JsonValueAdapter(result["type"]);
		unsigned uOff = uWinType * 2 + uRankType - 1;
		unsigned uPage = JsonValueAdapter(result["pageid"]);
		unsigned uMaxPage = JsonValueAdapter(result["maxpage"]);

		auto& list = result["data"];
		auto& vecRank = g_globalSngRankInfo.m_vecSngRankList[uOff];
		vecRank.clear();
		for (unsigned i = 0; i < list.size(); i++)
		{
			auto& user = list[i];
			SngRankInfo userInfo;
			userInfo.m_uID = JsonValueAdapter(user["id"]);
			userInfo.m_strIconUrl = user["icon"].asString();
			userInfo.m_strNickName = user["nickname"].asString();
			userInfo.m_dWinValue = JsonValueAdapter(user["score"]);
			userInfo.m_uRank = (uPage - 1) * list.size() + i;
			vecRank.push_back(userInfo);
		}
		g_globalSngRankInfo.m_uPageInfo[uOff][0] = uPage;
		g_globalSngRankInfo.m_uPageInfo[uOff][1] = uMaxPage;

		UpdateSngRankList(uRankType - 1, uWinType);

	}
	else if (rpcID == RPCCmd::GetMasterScoreRank		)
	{
		unsigned uWinType = 0;
		unsigned uRankType = JsonValueAdapter(result["type"]);
		unsigned uOff = uWinType * 2 + uRankType - 1;
		unsigned uPage = JsonValueAdapter(result["pageid"]);
		unsigned uMaxPage = JsonValueAdapter(result["maxpage"]);

		auto& list = result["data"];
		auto& vecRank = g_globalSngRankInfo.m_vecSngRankList[uOff];
		vecRank.clear();
		for (unsigned i = 0; i < list.size(); i++)
		{
			auto& user = list[i];
			SngRankInfo userInfo;
			userInfo.m_uID = JsonValueAdapter(user["id"]);
			userInfo.m_strIconUrl = user["icon"].asString();
			userInfo.m_strNickName = user["nickname"].asString();
			userInfo.m_dWinValue = JsonValueAdapter(user["score"]);
			userInfo.m_uRank = (uPage - 1) * list.size() + i;
			vecRank.push_back(userInfo);
		}
		g_globalSngRankInfo.m_uPageInfo[uOff][0] = uPage;
		g_globalSngRankInfo.m_uPageInfo[uOff][1] = uMaxPage;

		UpdateSngRankList(uRankType - 1, uWinType);

	}
	else if (rpcID == RPCCmd::FollowUser)
	{
		std::string strBtnType = result["btn_type"].asString();
		unsigned uOpType = JsonValueAdapter(result["act_type"]);

		unsigned uTID = JsonValueAdapter(result["tuid"]);
		//如果取消关注成功 删除好友列表关注，删除关注列表  删除牌桌内关注列表

		//删除好友列表关注
		if (strBtnType == "follow" && uOpType == 0) // 取消关注成功
		{
			
			auto& vecUserInfo = g_globalFollowUserInfo.m_vecFollowUser[0];
			for (auto iter = vecUserInfo.begin(); iter != vecUserInfo.end(); ++iter)
			{
				if (iter->m_uID == uTID)
				{
					vecUserInfo.erase(iter);
					m_pGameHall->UpdateFollowList(0);
					break;
				}
			}
		}
		//删除大厅关注列表及游戏中关注列表中关注的玩家
        std::vector<QuickFollowInfo>& vecQuickFollowInfo = g_globalQuickFollowInfo.m_vecQuickFollowInfo[0];
        for (auto iter = vecQuickFollowInfo.begin(); iter != vecQuickFollowInfo.end(); ++iter)
        {
            if (iter->m_uUserID == uTID)
            {
                vecQuickFollowInfo.erase(iter);
                break;
            }
        }
        QuickFollowInfo quickFollowInfo;
        quickFollowInfo.m_uUserID = uTID;
        if (m_pGameHall != nullptr)
            m_pGameHall->OperateQuickFollowList(0, 0, quickFollowInfo);
        if(m_pGameTable != nullptr)
            m_pGameTable->OperateOnlineFollow(0, 0, quickFollowInfo);

	}
	else if (rpcID == RPCCmd::GetUserDetailInfo)
	{
		auto& info = result["data"];
		UserDetailInfo detailInfo;
		detailInfo.m_uUID = JsonValueAdapter(info["id"]);
		detailInfo.m_bBoy = static_cast<unsigned>(JsonValueAdapter(info["gender"])) == 1;
		detailInfo.m_strIcon = info["icon"].asString();
		detailInfo.m_strName = info["nickname"].asString();
		detailInfo.m_dCash = JsonValueAdapter(info["beans"]);
        detailInfo.m_dDiamond = JsonValueAdapter(info["diamond"]);
		detailInfo.m_iVipDengJi = JsonValueAdapter(info["vip_lv"]);
		detailInfo.m_uJingYanDengJi = JsonValueAdapter(info["lv"]);
		detailInfo.m_uJingYanZongZhi = JsonValueAdapter(info["lv_total_exp"]);
		detailInfo.m_uJingYanJinDu = JsonValueAdapter(info["exp"]);
		detailInfo.m_iMeiLiZhi = JsonValueAdapter(info["charm"]);
		detailInfo.m_fZhouYinLi = JsonValueAdapter(info["week_win"]);
		detailInfo.m_uZongJuShu = JsonValueAdapter(info["play_times"]);
		unsigned uWinTime = JsonValueAdapter(info["win_times"]);

		double dWinRate = JsonValueAdapter(info["total_win_rate"]);

		detailInfo.m_uZongShengLv = dWinRate * 100 > 100 ? int(dWinRate*100)%100 : dWinRate * 100;
		detailInfo.m_iYinLiPaiMing = JsonValueAdapter(info["week_rank"]);
		detailInfo.m_iDanJuPaiMing = JsonValueAdapter(info["day_rank"]);
		detailInfo.m_fRiDanJu = JsonValueAdapter(info["day_win"]);
		auto& maxCard = info["max_card"];
		for (unsigned i = 0; i < 5; i++)
		{
			detailInfo.m_cbMaxCardData[i] = static_cast<unsigned>(JsonValueAdapter(maxCard[i]));
		}
		auto& chengjiu = info["achievement_summary"]["data"];
		for (unsigned i = 0; i < 4; i++)
		{
			detailInfo.m_uChengJiu[i] = static_cast<unsigned>(JsonValueAdapter(chengjiu[i]));
		}
		detailInfo.m_uChengJiuStep[0] = JsonValueAdapter(info["achievement_summary"]["completed"]);
		detailInfo.m_uChengJiuStep[1] = JsonValueAdapter(info["achievement_summary"]["count"]);

		//sng
		//// SNG盈利排名信息
		//unsigned m_uSNGBiSaiCiShu;
		//unsigned m_uSNGHuoJiangCiShu;
		//unsigned m_uSNGZhouDaShiFen;
		//int m_iSNGDaShiPaiMing;
		//unsigned m_uSNGJiangBei[3];
	/*	auto& snginfo = result["data"]["sng"];
		detailInfo.m_uSNGBiSaiCiShu =  JsonValueAdapter(snginfo["sngmatchcout"]);
		detailInfo.m_uSNGHuoJiangCiShu =  JsonValueAdapter(snginfo["sngrewardcout"]);
		detailInfo.m_uSNGZhouDaShiFen =  JsonValueAdapter(snginfo["sngweekmasterscore"]);
		detailInfo.m_iSNGDaShiPaiMing =  JsonValueAdapter(snginfo["sngmaterrank"]);

		detailInfo.m_uSNGJiangBei[0] =  JsonValueAdapter(snginfo["sngrank1cout"]);
		detailInfo.m_uSNGJiangBei[1] =  JsonValueAdapter(snginfo["sngrank2cout"]);
		detailInfo.m_uSNGJiangBei[2] =  JsonValueAdapter(snginfo["sngrank3cout"]);*/


		detailInfo.m_familyName = result["data"]["fname"].asString();
		detailInfo.m_familyID = JsonValueAdapter(result["data"]["fid"]);
		detailInfo.m_familyLV = JsonValueAdapter(result["data"]["flv"]);
		detailInfo.m_hasJoinFamily = detailInfo.m_familyID!=0 ;

		m_pGameHall->UpdateUserInfo(detailInfo);
	}
	else if (rpcID == RPCCmd::GetTableUserDetailInfo) 
	{
		auto& data = result["data"];
		UserDetailInfo usrInfo;
		usrInfo.m_bBoy = true;
		usrInfo.m_bZhuBo = false;
		usrInfo.m_uUID = JsonValueAdapter(data["id"]);
		usrInfo.m_strName = data["nickname"].asString();

		auto player = g_globalTableConfig.GetPlayerByID(usrInfo.m_uUID);
		if (nullptr != player) {
			usrInfo.m_strIcon = player->m_strIconUrl;
		} else {
			usrInfo.m_strIcon = data["icon"].asString();
		}
		
		usrInfo.m_dCash = JsonValueAdapter(data["beans"]);
        usrInfo.m_dDiamond = JsonValueAdapter(data["diamond"]);

		usrInfo.m_iVipDengJi = JsonValueAdapter(data["vip_lv"]);
		usrInfo.m_uJingYanDengJi = JsonValueAdapter(data["lv"]);
		usrInfo.m_uJingYanJinDu = JsonValueAdapter(data["curExp"]);
		usrInfo.m_uJingYanZongZhi = JsonValueAdapter(data["allExp"]);
		usrInfo.m_iMeiLiZhi = JsonValueAdapter(data["charm"]);

		usrInfo.m_uPaiJu = JsonValueAdapter(data["play_times"]);
		usrInfo.m_fRuJuLv = JsonValueAdapter(data["vpip_times"]);
		usrInfo.m_fShengLv = JsonValueAdapter(data["total_win_rate"]);
		usrInfo.m_fTanPaiLv = JsonValueAdapter(data["wtsd"]);

		usrInfo.m_fJiJinDu = JsonValueAdapter(data["af"]);
		usrInfo.m_fZaiJiaZhu = JsonValueAdapter(data["pfr_times"]);
		usrInfo.m_fFanQianXiaZhu = JsonValueAdapter(data["bet3_times"]);
		usrInfo.m_fChiXuXiaZhu = JsonValueAdapter(data["cbet"]);
		usrInfo.m_familyID = JsonValueAdapter(data["fid"]);
		usrInfo.m_familyName = data["fname"].asString();
		usrInfo.m_familyLV = JsonValueAdapter(data["flv"]);
		if (usrInfo.m_uUID != g_globalMyData.m_iUserId)
		{
			auto tip = data["tip"];
			if (tip.isObject())
			{
				usrInfo.m_iBiaoJi = JsonValueAdapter(tip["tipId"]);
				usrInfo.m_strBiaoJi = tip["tipDes"].asString();
				usrInfo.m_strBeiZhu = tip["remarkDes"].asString();
			}
			else
			{
				usrInfo.m_iBiaoJi = -1;
			}
			usrInfo.m_bYiGuanZhu = JsonValueAdapter(data["is_followed"]);
			usrInfo.m_bYiPingBi = JsonValueAdapter(data["is_shielded"]);
		}
		if (usrInfo.m_strIcon == "")
			usrInfo.m_strIcon = "zhongnan_3.jpg";
		m_pGameTable->PopUserInfo(usrInfo);
	}
	//else if (rpcID == RPCCmd::GetMyDetailInfo)
	//{
	//	auto& info = result["data"];
	//	g_globalMyData.SetGender(JsonValueAdapter(info["gender"]));
	//	g_globalMyData.SetNickName(info["nickname"].asString());
	//	g_globalMyData.SetCash(JsonValueAdapter(info["beans"]));
	//	g_globalMyData.SetVipLevel(JsonValueAdapter(info["vip_lv"]));

	//}
	else if (rpcID == RPCCmd::GetActivityInfo)
	{
		g_globalActivityInfo.m_vecActivityInfo.clear();
        g_globalActivityInfo.m_vecPushActivityInfo.clear();
		//auto& activities = result["data"];
		//std::string strDescription = "";
		//for (unsigned i = 0; i < activities.size(); i++)
		//{
		//	auto& activity = activities[i];
		//	ActivityInfo activityInfo;
  //          PushActivityInfo pushActivity;
  //          //大厅操作栏的消息
		//	activityInfo.m_uType = JsonValueAdapter(activity["type"]);
		//	activityInfo.m_strTitle = activity["title"].asString();
		//	strDescription = activity["description"].asString();
		//	activityInfo.m_strDescription = FormatAutoLine(strDescription);
		//	//activityInfo.m_strDescription = activity["description"].asString();
		//	activityInfo.m_strIcon = activity["icon"].asString();
		//	activityInfo.m_strUrl = activity["url2"].asString();
  //          g_globalActivityInfo.m_vecActivityInfo.push_back(activityInfo);
  //          //推送消息或广告
  //          pushActivity.m_bIsShow = JsonValueAdapter(activity["is_show"]);
  //          pushActivity.m_pActivityTitle = activity["title"].asString();
  //          pushActivity.m_pActivityUrl = activity["url1"].asString();
  //          g_globalActivityInfo.m_vecPushActivityInfo.push_back(pushActivity);
		//}
        auto& activities = result["data_activity"];
        std::string strDescription = "";
        for (unsigned i = 0; i < activities.size(); i++)
        {
            //大厅操作栏的消息
            auto& activity = activities[i];
            ActivityInfo activityInfo;
            activityInfo.m_uType = JsonValueAdapter(activity["type"]);
            activityInfo.m_strTitle = activity["title"].asString();
            strDescription = activity["description"].asString();
            //activityInfo.m_strDescription = FormatAutoLine(strDescription);
            activityInfo.m_strDescription = activity["description"].asString();
            activityInfo.m_strIcon = activity["icon"].asString();
            activityInfo.m_strUrl = activity["url"].asString();
            activityInfo.m_bIsShow = JsonValueAdapter(activity["is_show"]);
            activityInfo.m_uRedefine = JsonValueAdapter(activity["is_jump"]);
            activityInfo.m_uSecondRedfine = JsonValueAdapter(activity["second_jump"]);
            activityInfo.m_uSortIndex = JsonValueAdapter(activity["paixu"]);
            activityInfo.m_uID = JsonValueAdapter(activity["no"]);
            g_globalActivityInfo.m_vecActivityInfo.push_back(activityInfo);
        }
        auto& pushAds = result["data_haibao"];
        for (unsigned i = 0; i < pushAds.size(); i++)
        {
            //推送消息或广告
            auto& pushAd = pushAds[i];
            PushActivityInfo pushActivity;
            pushActivity.m_uShowType = JsonValueAdapter(pushAd["fullScreen"]);
            pushActivity.m_bIsShow = JsonValueAdapter(pushAd["is_show"]);
            pushActivity.m_uRedefine = JsonValueAdapter(pushAd["jump_to"]);
            pushActivity.m_pActivityTitle = pushAd["title"].asString();
            pushActivity.m_pActivityUrl = pushAd["img_url"].asString();
            pushActivity.m_strID = pushAd["no"].asString();
            g_globalActivityInfo.m_vecPushActivityInfo.push_back(pushActivity);
        }
		requestHasMatchByInTable(false) ;
		
	}
	else if (rpcID == RPCCmd::GetMessageInfo)
	{
		unsigned uType = JsonValueAdapter(result["type"]);
		auto& vecMessages = g_globalMessageInfo.m_vecMessageInfo[uType-1];
		vecMessages.clear();
		auto& messages = result["data"];



		if (uType==0)
		{

		}else
		if (uType!=3)
		{
			char buffer[64];
			for (unsigned i = 0; i < messages.size(); i++)
			{
				auto& message = messages[i];
				MessageInfo info;
				info.m_uID = JsonValueAdapter(message["id"]);
				info.m_uType = uType;
				info.m_strContent = message["content"].asString();
				if(uType != 4)
				{
					info.m_dReward = JsonValueAdapter(message["reward"]["texas_beans"]);
				}
					

				/*info.m_strDate = message["date"].asString();
				info.m_strTime = message["time"].asString();*/
				time_t created_at = (time_t)(double)JsonValueAdapter(message["created_at"]);

				tm *tDesc = localtime(&created_at);
				memset(buffer, 0, 64);
				sprintf(buffer, "%02d:%02d", tDesc->tm_hour, tDesc->tm_min);
				info.m_strTime = buffer;
				memset(buffer, 0, 64);
				sprintf(buffer, "%d.%d", tDesc->tm_mon + 1, tDesc->tm_mday);
				info.m_strDate = buffer;
				
				vecMessages.push_back(info);
			}
		}
		else
		{
            bool bHasClubMessage = false;
		    for (unsigned i = 0; i < messages.size(); i++)
			{
				auto& message = messages[i];
				MessageInfo info;
				info.m_uID = JsonValueAdapter(message["id"]);
				info.m_uType = uType;
				info.m_strContent = message["content"].asString();
				info.m_dReward = JsonValueAdapter(message["reward"]["texas_beans"]);
			/*	info.m_strDate = message["date"].asString();
				info.m_strTime = message["time"].asString();*/

				
				info.m_familyid =  JsonValueAdapter(message["fid"]);
				info.m_familyName = message["fname"].asString() ;
				info.m_familyIcon = JsonValueAdapter(message["ficon"]) ;
				//info.m_familyIcon =  message["ficon"].asString() ;
				info.m_userid =  JsonValueAdapter(message["uid"]);
				info.m_userName = message["nickname"].asString() ;
				info.m_userIcon =  message["icon"].asString() ;
				info.m_eventType = JsonValueAdapter(message["event"]);

				info.m_hasDeal = JsonValueAdapter(message["status"]) ;
				info.m_strLastTime = JsonValueAdapter(message["time"]) ;
				vecMessages.push_back(info);

                if (info.m_eventType == 1 && info.m_hasDeal ==0)//申请加入并且没有处理
                {
                    bHasClubMessage = true;
                }
			}
            m_pGameHall->updateShowNewMessage(bHasClubMessage);
		}
		
		m_pGameHall->UpdateMessageInfo(uType-1);
	}
	else if (rpcID == RPCCmd::GetAchievements)
	{
		auto& data = result["data"];
		for (unsigned i = 0; i < 5; i++)
		{
			auto& items = data[i];
			auto& vecItems = g_globalAchievementInfo.m_vecAchievementInfo[i];
			vecItems.clear();
			for (unsigned j = 0; j < items.size(); j++)
			{
				auto& item = items[j];
				AchievementInfo info;
				info.m_strTip = item["tips"].asString();
				info.m_strUrl = item["icon"].asString();
				info.m_dNum = JsonValueAdapter(item["total"]);
				info.m_bActivated = JsonValueAdapter(item["flag"]);
				vecItems.push_back(info);
			}
		}
		m_pGameHall->UpdateAchievementInfo();
	}
    else if (rpcID == RPCCmd::BuyGoodsByDiamond)
    {
        //使用钻石购买道具成功返回
        SendClientRequest(ClientRequest::kCallUpdateCashAndVip);
        SendClientRequest(ClientRequest::kCallExpressPower);//是否可以使用魔法表情
        RPC rpc2(RPCCmd::GetNewShopPrice, "texas_m_new_gifts_conf", g_globalMyData.m_iUserId, g_globalSession.m_szSecret);
        m_rpcRoutine.AddRequest(rpc2);

        PopTipWithBt1("恭喜您已购买成功！", "确定", nullptr);
    }
    else if (rpcID == RPCCmd::PopActReward)
    {
        bool bSuccess = JsonValueAdapter(result["s"]);
        if (bSuccess)
        {
            g_globalActRewardInfo.m_vecReward.clear();
            auto& data = result["data"];
            std::string strTitle = data["title"].asString();
            auto& reward = data["award"];
            ActReward actReward;
            unsigned uStatus = -1;
            for (int i=0; i<reward.size(); i++)
            {
                actReward.m_strDes = reward[i]["desc"].asString();
                actReward.m_strURL = reward[i]["url"].asString();
                actReward.m_uActivityID = JsonValueAdapter(reward[i]["id"]);
                actReward.m_uState = JsonValueAdapter(reward[i]["tankuangstatus"]);
                g_globalActRewardInfo.m_vecReward.push_back(actReward);
                uStatus = JsonValueAdapter(reward[i]["tankuangstatus"]);
            }
            g_globalActRewardInfo.m_strTitle = strTitle;
            if (uStatus == 0)
                PopActivityReward();
        }
    }
	else if (rpcID == RPCCmd::GetGamePlayBackList)
	{
        if (m_pGameRecord == nullptr)
        {
            m_pGameRecord = GameRecord::create();
            Size sz = m_pGameRecord->getContentSize();
            m_pGameRecord->setPosition(GameData::getInstance()->getGameWidth()/2 - sz.width/2, GameData::getInstance()->getGameHeight()/2 - sz.height/2);
            this->addChild(m_pGameRecord, GameRecordZOrder);
        }
        else
        {
            m_pGameRecord->setVisible(true);
        }

		std::string strID = result["data"][0u]["gameid"].asString();
		Json::Value param;
		param["id"] = strID;
		//SendClientRequest(ClientRequest::kGetGamePlayBack, param);

		unsigned uCurPage = JsonValueAdapter(result["pageid"]);
		g_globalGameRecord.m_uAllPage = JsonValueAdapter(result["maxpage"]);
		unsigned uSortType = JsonValueAdapter(result["type"]);
		auto& records = result["data"];
        uSortType -= 1;
		auto& vecRecords = g_globalGameRecord.m_vecRecord[uSortType];
		g_globalGameRecord.m_vecPageIdx[uSortType].push_back(vecRecords.size());
		GameRecordInfo gameRecord;
		for (unsigned i = 0; i < records.size(); i++)
		{
			gameRecord.m_strID = records[i]["gameid"].asString();
			gameRecord.m_uIdx = JsonValueAdapter(records[i]["id"]);
			gameRecord.m_strTableName = records[i]["tablename"].asString();
			gameRecord.m_uMaxPlayer = JsonValueAdapter(records[i]["maxplayer"]);
			gameRecord.m_dBigBlind = JsonValueAdapter(records[i]["bigblind"]);
			gameRecord.m_dSmallBlind = gameRecord.m_dBigBlind / 2;
			gameRecord.m_dWinLost = JsonValueAdapter(records[i]["winlosemoney"]);
			gameRecord.m_dTime = JsonValueAdapter(records[i]["time"]);
			gameRecord.m_dTime /= 1000.0f;
            gameRecord.m_cbHandCard[0] = static_cast<unsigned>(JsonValueAdapter(records[i]["handCard"][0u]));
            gameRecord.m_cbHandCard[1] = static_cast<unsigned>(JsonValueAdapter(records[i]["handCard"][1u]));
			vecRecords.push_back(gameRecord);
		}

        m_pGameRecord->UpdataRecordData();//排序方式，页偏移
	}
	else if (rpcID == RPCCmd::GetGamePlayBack)
	{
		PopLoading(false);
		auto&data = result["data"];
		GameData::getInstance()->setSoundOn(false);
		PopTablePlayBack(data);
		auto funEndCollecting = [this](){
			GameData::getInstance()->setSoundOn(true) ;
		}; 
		this->runAction(  Sequence::create(DelayTime::create(0.2f),CallFunc::create(funEndCollecting),nullptr)) ;
	}
    else if (rpcID == RPCCmd::GetMySpread)
    {
        g_globalFriendsSpread.m_vecFriendsSpread.clear();
        auto& data = result["data"];
        for (int i=0; i<data.size(); i++)
        {
            FollowUserInfo followInfo;
            followInfo.m_uID = JsonValueAdapter(data[i]["promoter_id"]);
            followInfo.m_strIcon = data[i]["icon"].asString();
            followInfo.m_strNickName = data[i]["nickname"].asString();
            g_globalFriendsSpread.m_vecFriendsSpread.push_back(followInfo);
        }
        m_pGameHall->updateSpreadList();
    }
    else if (rpcID == RPCCmd::BindSpread)
    {
        bool bRet = JsonValueAdapter(result["s"]);
        if (bRet)
        {
            auto& data = result["data"];
            g_globalFriendsSpread.m_iUserID = JsonValueAdapter(data["uid"]);
            g_globalFriendsSpread.m_strName = data["nickname"].asString();
            g_globalFriendsSpread.m_strIcon = data["icon"].asString();
            g_globalFriendsSpread.m_bIsSpread = true;
        }
        else
        {
            g_globalFriendsSpread.m_bIsSpread = false;
        }
        m_pGameHall->popBindSpreadResult(bRet);
    }

	else if (rpcID == RPCCmd::GetTicketLogin)
	{

		m_globaPrivateRoomInfo.clean();
		int success = JsonValueAdapter(result["s"]);
		auto& data = result["data"];
		g_globalMyData.SetCash(JsonValueAdapter(data["money"]));
		g_globalMyData.SetVipLevel(JsonValueAdapter(data["vip_lv"]));
		g_globalMyData.SetLadyLevel(JsonValueAdapter(data["is_guan"]));
		if (g_globalMyData.GetVipLevel() > 0) // 是vip
		{
			g_globalMyData.m_strVipName = data["vip_name"].asString();
			g_globalMyData.m_uVipValidDay = JsonValueAdapter(data["vip_day"]);
			g_globalMyData.m_bNeedShowVipAward = JsonValueAdapter(data["vip_visible"]);
			g_globalMyData.m_dVipAward = JsonValueAdapter(data["vip_money"]);
		}
		g_globalMyData.SetBindPhone(JsonValueAdapter(data["phone_status"]));

		//m_pLogonLayerEx->setVisible(false);
		EnterGameHall();

		GameData::getInstance()->cleanMoveLable() ;
		m_pGameHall->reSetScorlllTips() ;
		if (m_pGameTable != nullptr)
			m_pGameTable->reSetScorlllTips() ;
		GameData::getInstance()->cleanMoveLable() ;

	

		//每日签到
        g_globalUserSigin.m_vecUserSigin.clear();
		g_globalUserSigin.m_vecUserContinuousSingin.clear() ;
        g_globalMyData.m_continueLoginType = JsonValueAdapter(data["continueType"]);
		auto& continueLogon = data["continueLogon"];
		auto& continueSingin = data["continueLogonTotal"];
		g_globalUserSigin.m_continusdays =   JsonValueAdapter(data["continusdays"]);
			
		g_globalUserSigin.m_hasShowSiginLayer = false ;
		g_globalUserSigin.m_hasGetContinuonsReward = true ;
		g_globalUserSigin.m_hasGetSiginReward = true ;
		//g_globalUserSigin.m_continusdays = 7 ;
		//g_globalUserSigin.m_today = 5 ;
		
		g_globalUserSigin.m_today =   JsonValueAdapter(data["w"]);
		//g_globalUserSigin.m_hasGetContinuonsReward = JsonValueAdapter(data["m_hasGetContinuonsReward"]); ;
		//g_globalUserSigin.m_hasGetSiginReward = JsonValueAdapter(data["m_hasGetSiginReward"]); 
		g_globalMyData.m_WaveGrilAward = JsonValueAdapter(data["guanLoginMoney"]); //德女郎登录奖励
		for (int i=0; i<continueLogon.size(); i++)
		{
            if (g_globalMyData.m_continueLoginType == 0)
            {
                g_globalContinueLogin.dayNum = continueLogon.size();
                g_globalContinueLogin.SignID[i] = JsonValueAdapter(continueLogon[i]["id"]);
                g_globalContinueLogin.SignStatus[i] = JsonValueAdapter(continueLogon[i]["status"]);
                //g_globalMyData.m_WaveGrilAward = JsonValueAdapter(data["guanLoginMoney"]);
                g_globalContinueLogin.SignMoney[i] = JsonValueAdapter(continueLogon[i]["money"]);
            }
            else if (g_globalMyData.m_continueLoginType == 1)
            {
				//签到
                SiginDay signday;
       
                signday.m_state = JsonValueAdapter(continueLogon[i]["r_state"]);
                signday.m_count = JsonValueAdapter(continueLogon[i]["m_count"]);
                signday.m_itemName = continueLogon[i]["m_itemName"].asString();
                signday.m_iconUrl = continueLogon[i]["m_iconUrl"].asString();

				if ( signday.m_state == 2)
				{
					g_globalUserSigin.m_hasGetSiginReward = false ;
				}
                g_globalUserSigin.m_vecUserSigin.push_back(signday);


				//连续登录
				ContinuousSingin continuousSingin ;

				unsigned state =  JsonValueAdapter(continueSingin[i]["r_state"]) ;

				unsigned itemid =  JsonValueAdapter(continueSingin[i]["id"]) ;
				//无物品  已领取  当前领取  未领取  0，1，2，3
				continuousSingin.m_hasItem = state!=0?true:false ;
				if (state==2)
				{
					g_globalUserSigin.m_hasGetContinuonsReward = false ;
				}
				continuousSingin.m_itemName =  continueSingin[i]["m_itemName"].asString();
				continuousSingin.m_count =  JsonValueAdapter(continueSingin[i]["m_count"]);
				continuousSingin.m_iconUrl =  continueSingin[i]["m_iconUrl"].asString();
				g_globalUserSigin.m_vecUserContinuousSingin.push_back(continuousSingin) ;

            }
		}


		// rtmp服务器地址+app应用名
		g_globalServerInfo.m_strVideoServerApp = data["streamUrl"].asString();	

		g_globalConfig.m_bJinZhiPangGuan = JsonValueAdapter(data["ifLook"]);
		g_globalConfig.m_bJinZhiTongZhuo = JsonValueAdapter(data["ifIPLimit"]);
		g_globalConfig.m_bJuJueYaoQing = JsonValueAdapter(data["ifDenyInvite"]); // TODO:

		g_globalConfig.m_dMaxWinPerDay = JsonValueAdapter(data["antiAddiction1"]["money"]);
		g_globalConfig.m_uFreezenMinWhenWin = JsonValueAdapter(data["antiAddiction1"]["time"]);
		g_globalConfig.m_dMaxLosePerDay = JsonValueAdapter(data["antiAddiction2"]["money"]);
		g_globalConfig.m_uFreezenMinWhenLose = JsonValueAdapter(data["antiAddiction2"]["time"]);;
		g_globalConfig.m_MaxOnlineHour = JsonValueAdapter(data["antiAddiction3"]["money"]);
		g_globalConfig.m_uFreezenMinWhenTimeFull = JsonValueAdapter(data["antiAddiction3"]["time"]);
		

		SendClientRequest(ClientRequest::kGetActivityInfo);
		m_gameMsgIO.SendGameMessage(GameMsgCmdC::GetFollowUserInfo);
		/*requestHasMatchByInTable(false) ;*/
	}
}

void GameFrame::OnRpcResultFailed(RPCCmd rpcID, const char *pFaildInfo)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	log("%s", pFaildInfo);
	SendGameTip(pFaildInfo);
#endif
	if (rpcID == RPCCmd::Logon || rpcID == RPCCmd::GuestLogon || rpcID == RPCCmd::AutoLogon || rpcID == RPCCmd::QQLogon || rpcID == RPCCmd::WXLogon||rpcID == RPCCmd::SinaLogon||
		rpcID == RPCCmd::The3rdLogon)
	{
		m_pLogonLayerEx->CancelLoading();
		g_globalConfig.SetLastLogonResult(false, std::string(""), std::string(""));
		m_pLogonLayerEx->ShowAccountLogon();
		SendGameTip("登录失败,请重新登录！");
		//SendGameTip(pFaildInfo);
	}

	else
	//申请处理
	if (rpcID == RPCCmd::DealApplyFamily)
	{

		PopTipWithBt1(pFaildInfo,"确定",nullptr) ;
	}
	else if ( rpcID == RPCCmd::To_ViceShaikh)
	{
		PopTipWithBt1(pFaildInfo,"确定",nullptr) ;
	}
	else if ( rpcID == RPCCmd::Move_ViceShaikh)
	{
		//SendGameTip(pFaildInfo);
		PopTipWithBt1(pFaildInfo,"确定",nullptr) ;
	}
	else if ( rpcID == RPCCmd::GetFamiliyReward)
	{
		PopTipWithBt1(pFaildInfo,"确定",nullptr) ;
	}
	else if ( rpcID == RPCCmd::CreatFamily)
	{
		PopTipWithBt1(pFaildInfo,"确定",nullptr) ;
	}
	else if ( rpcID == RPCCmd::InviteFreinds)
	{
		PopTipWithBt1(pFaildInfo,"确定",nullptr) ;
	}
    else if (rpcID == RPCCmd::BuyGoodsByDiamond)
    {
        PopTipWithBt1(pFaildInfo, "确定", nullptr);
    }
    else if ( rpcID == RPCCmd::CallExchangeInfo)
    {
        g_globalMallInfo.m_vecExchange.clear();
        if (m_pGameMall != nullptr)
        {
            m_pGameMall->initExChangeLayer();
        }
    }
	else if ( rpcID == RPCCmd::ChangeFamilyShaikh)
	{
		PopTipWithBt1(pFaildInfo,"确定",nullptr) ;
	}
	else if ( rpcID == RPCCmd::BindPhone)
	{
		PopTipWithBt1(pFaildInfo,"确定",nullptr) ;
	}
    else if ( rpcID == RPCCmd::ResetPassword)
    {
        PopTipWithBt1(pFaildInfo,"确定",nullptr) ;
    }
	else if ( rpcID == RPCCmd::JoinFamily)
	{
		PopTipWithBt1(pFaildInfo,"确定",nullptr) ;
	}
	else if ( rpcID == RPCCmd::SearchFamily)
	{
		PopTipWithBt1(pFaildInfo,"确定",nullptr) ;
	}
	else if ( rpcID == RPCCmd::DealInvateJoinFamily)
	{
		PopTipWithBt1(pFaildInfo,"确定",nullptr) ;
	}
	else if ( rpcID == RPCCmd::GetChangeShaiKhCode)
	{
		PopTipWithBt1(pFaildInfo,"确定",nullptr) ;
	}
	else if ( rpcID == RPCCmd::GetRegisterCode)
	{
		PopTipWithBt1(pFaildInfo,"确定",nullptr) ;
	}
	else if ( rpcID == RPCCmd::GetBindSafeCode)
	{
		PopTipWithBt1(pFaildInfo,"确定",nullptr) ;
	}	
	else if (rpcID == RPCCmd::EditFamilyInfo)
	{
		//更换俱乐部信息
		PopTipWithBt1(pFaildInfo,"确定",nullptr) ;
	}
	else if (rpcID == RPCCmd::GetMessageInfo)
	{
		m_pGameHall->callHideMessageLoading() ;
	}
    else if (rpcID == RPCCmd::GetGamePlayBack)
    {
        PopLoading(false);
        SendGameTip("获取牌局回放数据失败！");
    }
	else if (rpcID == RPCCmd::GetActivityInfo)
	{
		/*PopLoading(false);
		SendGameTip("获取牌局回放数据失败！");*/
		requestHasMatchByInTable(false) ;
	}
    else  if (rpcID == RPCCmd::CheckAppStoreProduce)
    {
        Json::Value param;
        param["cmd"] = NonUI_MobilePayResult;
        param["res"] = 2;
        SendNonUIMsg(param);
        
        
	}else 	if (rpcID == RPCCmd::GetPrivateFundData)
	{
		

	}else if (rpcID == RPCCmd::CallAllotFund)
	{
		PopTipWithBt1(pFaildInfo,"确定",nullptr) ;
	}
    else if (rpcID == RPCCmd::BindSpread)
    {
        m_pGameHall->popBindSpreadResult(false);
    }
    else if (rpcID == RPCCmd::GetMySpread)
    {
        g_globalFriendsSpread.m_vecFriendsSpread.clear();
        m_pGameHall->updateSpreadList();
    }

	// GetMyFamilyInfo,
	//	GetTargetFamilyInfo,
	//	ShowGetTargetFamilyInfo,
	//	ShowTableGetTargetFamilyInfo,
	//	GetFamilyList,

	//	GetFamilyAllRole,
	//	GetFamilyNoSKRole,//获取没有权限的俱乐部人员列表
	//	GetAllFriends,


	//	To_ViceShaikh,//设置副族长
	//	Move_ViceShaikh,//取消副族长
	//	GetChangeShaiKhCode,
	//	ChangeFamilyShaikh,//转让俱乐部

	//	GetFamiliyReward,//领取俱乐部奖励
	//	InviteFreinds,//邀请好友
	//	SearchFamily,
	//	DealApplyFamily, //处理俱乐部申请
	//	DealInvateJoinFamily,//处理俱乐部邀请
}

void GameFrame::OnGameIOAbord()
{

	//m_loginFlag = false;
	cocos2d::log("%s", "GameIOAbord\n");
    
    
    
    //bool connected = ConnectGameServer();
    //if (connected)
    //    return;
    
    if (!m_pLogonLayerEx->isVisible()) {
        startCheckNetwork();
		popupNetworkLoading(true);
    }

    
	//SendGameTip("与游戏服务器的连接中断了，请检查您的账号是否被重复登陆或者网络是否存在异常。");
    /*
	if (g_globalTableConfig.m_uTableID >= 0) {
		forceLeaveTable();
		stopVideo();
	} else if (g_globalTableConfig.m_uTableID == -1) {
		forceLeaveHall();
	}
    */
}

void GameFrame::OnGameMessage(const GameMsgHead& msgHead, const char*pBody, unsigned uBodySize)
{
	long m_startTime = getCurrentTime();
	// free it after using.
	unsigned char* pBody2 = nullptr;
	unsigned uBodySize2;
	if (ZipUtils::isGZipBuffer((unsigned char*)pBody, uBodySize)) {
		long lastTime1 = getCurrentTime();
		uBodySize2 = (unsigned)ZipUtils::inflateMemory((unsigned char*)(pBody), uBodySize, &pBody2);
	}

	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	cocos2d::log("GameMsg: %d", msgHead.m_uCmd);
	const char* pBody3 = (const char*)pBody2;
	auto l = uBodySize2;
	auto leftSize = uBodySize2;
	unsigned c = 0;
	do
	{
		char szLog[4096];
		memset(szLog, 0, 4096);
		strncpy(szLog, pBody3+c, (leftSize > 4096) ? 4096 : leftSize);
		c += 4096;
		log("%s", szLog);

		if (leftSize > 4096)
			leftSize -= 4096;
	} while (c < l);
#endif
	Json::Reader reader;
	Json::Value msg;
	if (!reader.parse(std::string((const char*)pBody2, uBodySize2), msg))
	{
		cocos2d::log("msg parse failed!");
	}

	if (pBody2 != nullptr) {
		free(pBody2);
		pBody2 = nullptr;
		uBodySize2 = 0;
	}
	//float mpTime = (getCurrentTime()-m_startTime)/1000.00f;

    if(!m_bIsRecvGameScence && checkTableSocket(msgHead.m_uCmd))
    {
        //m_gameMsgIO.SendGameMessage(GameMsgCmdC::LeaveTable);
        return ;
    }

	switch (msgHead.m_uCmd)
	{
	case  GameMsgCmdS::GetUpdatePlayerCash:
		{
			bool result = JsonValueAdapter(msg["result"]);
			if (!result)
			{
				std::string msgError = msg["errorMsg"].asCString() ;
				PopTipLayerWithBt1(msgError.c_str(),"确定",nullptr) ;
				return ;
			}
			unsigned fid = JsonValueAdapter(msg["clubid"]);
			int touid = JsonValueAdapter(msg["uid"]);
			int conuid = JsonValueAdapter(msg["ownerid"]);
			double fund = JsonValueAdapter(msg["fund"]);
			std::string uname = msg["uname"].asCString() ;
			std::string clubname  = msg["clubName"].asCString() ;

			double clubFund = JsonValueAdapter(msg["cludfund"]);
			double ufund = JsonValueAdapter(msg["ufund"]);


			char buff[64] ;
			if (conuid == g_globalMyData.m_iUserId)
			{
				sprintf(buff,"成功分配给成员%s基金%.0f",uname.c_str(),fund) ;
				PopTipWithBt1(buff,"确定",nullptr) ;
			}
			else
			{
				sprintf(buff,"%s俱乐部管理员给你分配了%.0f基金",clubname.c_str(),fund) ;
				PopTipWithBt1(buff, "确定", nullptr);
			}
		
			if (fid == g_globalMyFamilyInfo.m_familyID)
			{
				g_globalMyFamilyInfo.m_fund = clubFund ;
				for (auto& role:g_globalMyFamilyInfo.m_allTopRank)
				{
					if (role.m_uid == touid)
					{
						role.m_hasCash = ufund ;
						m_pGameHall->updateSelfClubRoleFundInfo(clubFund,touid,ufund) ;
					}
				}
			}
			


		}
		break ;
	case GameMsgCmdS::GetPrivateRoomLeavelRoomResult:
		{
			PopTipWithBt1("您已被房主踢出房间，请返回大厅", "确定", [this](unsigned uBt){
				if (uBt == 0)
				{
					SendClientRequest(ClientRequest::kCallLevelTable) ;
				}
			});
			
		}
		break ;
	case GameMsgCmdS::GetPrivateRoomGameOver:
		{
			PopTipWithBt1("牌局已解散，请返回大厅", "确定", [this](unsigned uBt){
				if (uBt == 0)
				{
					SendClientRequest(ClientRequest::kCallLevelTable) ;
				}
			});

		}
		break ;
	case GameMsgCmdS::GetPrivateRoomPushPlayer:
		{
			unsigned result = JsonValueAdapter(msg["result"]);
			if (result)
			{
				PopTipWithBt1("本场牌局结束后将踢出该玩家", "确定", nullptr);
			}

		}
		break ;

	case GameMsgCmdS::GetJoinToPrivateRoomReslut:
		{
			unsigned result = JsonValueAdapter(msg["result"]);
			//unsigned uTag = JsonValueAdapter(msg["tag"]);
			
			if (result)
			{
			}
			else
			{
				unsigned  roomid = JsonValueAdapter(msg["roomid"]);
				PopTipLayerWithBt1(msg["data"].asString(), "确定");
			
				std::vector<DefineRoomInfo>::iterator iter ;
				for( iter = g_globalVipOpenRoom.m_UserDefineRoom.begin(); iter!=g_globalVipOpenRoom.m_UserDefineRoom.end();++iter)
				{
					if (iter->m_uAreaID == roomid)
					{
						g_globalVipOpenRoom.m_UserDefineRoom.erase(iter);
						break ;
					}
				}
				m_pVipPrivateNormal->showRoomView(false) ;

			}
			PopLoading(false);
			//unsigned uTag = JsonValueAdapter(msg["tag"]);
			//if (uTag == 0) // 钱不够
			//{
			//	PopTipLayerWithBt2(msg["data"].asString(), "去充值", "返回", [](unsigned uBt){
			//		if (uBt == 0)
			//		{
			//			Json::Value param;
			//			param["idx"] = 0;
			//			SendClientRequest(ClientRequest::kPopMall, param);
			//		}
			//	});
			//}
			//else
			//{
			//	PopTipLayerWithBt1(msg["data"].asString(), "确定");
			//}
		}
		break ;
	case GameMsgCmdS::GetPrivateRoomName:
		{
			std::string roomName = msg["roomname"].asCString() ;
			m_pVipPrivateNormal->callUpdateRoomName(roomName) ;
		}
		break ;
		//私密房开关
	case GameMsgCmdS::GetPrivateRoomControll:
		{
			unsigned isopen = JsonValueAdapter(msg["con"]);

			unsigned result = JsonValueAdapter(msg["result"]);
			//关闭
			if (isopen ==0)
			{
				if (result == 0)
				{

				}
				else
				{
					m_globaPrivateRoomInfo.m_isGameState = 2;
					m_pGameTable->doControllPrivateTable(m_globaPrivateRoomInfo.m_isOwner,2) ;
					
				}
			}
			else
			{
				if (result == 0)
				{

				}
				else
				{
					m_globaPrivateRoomInfo.m_isGameState = 1;
					m_pGameTable->doControllPrivateTable(m_globaPrivateRoomInfo.m_isOwner,1) ;

				}
			}
			

		}
		break ;


	case GameMsgCmdS::GetCurPrivateRoomInfo:
		{
		
			auto &normalInfos = msg["data"];
			bool result = JsonValueAdapter(normalInfos["type"]);
			if (result)
			{
				m_globaPrivateRoomInfo.m_curCollectFund = JsonValueAdapter(normalInfos["fund"]);;
			}
			else
			{
				
				m_globaPrivateRoomInfo.m_Type =  JsonValueAdapter(normalInfos["kind"]);
				m_globaPrivateRoomInfo.m_playerNum = JsonValueAdapter(normalInfos["numLimit"]);
				m_globaPrivateRoomInfo.m_ownerName =  normalInfos["ownerName"].asString();
				m_globaPrivateRoomInfo.m_roomName = normalInfos["roomName"].asString();
				m_globaPrivateRoomInfo.m_password = normalInfos["password"].asString();
				m_globaPrivateRoomInfo.m_minBlind = JsonValueAdapter(normalInfos["smallBlind"]);
				m_globaPrivateRoomInfo.m_maxBlind = JsonValueAdapter(normalInfos["bigBlind"]);
				m_globaPrivateRoomInfo.m_ante = JsonValueAdapter(normalInfos["mustBet"]);
				m_globaPrivateRoomInfo.m_minCarry = JsonValueAdapter(normalInfos["minCarry"]);;
				m_globaPrivateRoomInfo.m_maxCarry = JsonValueAdapter(normalInfos["maxCarry"]);
				double rate= (JsonValueAdapter(normalInfos["fundRate"])) ;
				m_globaPrivateRoomInfo.m_fundP = rate*100 ;
				m_globaPrivateRoomInfo.m_minCollect = JsonValueAdapter(normalInfos["startFund"]);
				m_globaPrivateRoomInfo.m_maxCollect =  JsonValueAdapter(normalInfos["endFund"]);;
				m_globaPrivateRoomInfo.m_roomid =  JsonValueAdapter(normalInfos["roomId"]);;
				m_globaPrivateRoomInfo.m_curCollectFund = JsonValueAdapter(normalInfos["fund"]);;
				m_globaPrivateRoomInfo.m_isOwner = JsonValueAdapter(normalInfos["isOwner"]);
				m_globaPrivateRoomInfo.m_hasGetData = true ;
			}

			m_pGameTable->updatePrivateFund(m_globaPrivateRoomInfo.m_isOwner) ;

		}
		break ;
	case GameMsgCmdS::GetPrivateRoomPasswordReslut:
		{
			bool result = JsonValueAdapter(msg["result"]);
			if (result)
			{
				auto &normalInfos = msg["data"];
				DefineRoomInfo pDefineOpenRoomInfo ;

				pDefineOpenRoomInfo.m_uType = JsonValueAdapter(normalInfos["kind"]);
				pDefineOpenRoomInfo.m_uType-=1 ;
				pDefineOpenRoomInfo.m_uAreaID = JsonValueAdapter(normalInfos["roomId"]);
				pDefineOpenRoomInfo.m_strRoomName = normalInfos["roomName"].asString();
				pDefineOpenRoomInfo.m_strOwnerName = normalInfos["ownerName"].asCString();
				pDefineOpenRoomInfo.m_strRoomPwd = normalInfos["password"].asString();
				pDefineOpenRoomInfo.m_uPlayers = JsonValueAdapter(normalInfos["playerNum"]);
				pDefineOpenRoomInfo.m_uCurPlayers = JsonValueAdapter(normalInfos["inChairPlayer"]);
				pDefineOpenRoomInfo.m_uMaxPlayers =  JsonValueAdapter(normalInfos["numLimit"]);
				pDefineOpenRoomInfo.m_uLookOnNum = JsonValueAdapter(normalInfos["lookOnPlayer"]);
				pDefineOpenRoomInfo.m_minCollect = JsonValueAdapter(normalInfos["startFund"]);
				pDefineOpenRoomInfo.m_maxCollect = JsonValueAdapter(normalInfos["endFund"]);;
				double rate= (JsonValueAdapter(normalInfos["fundRate"])) ;
				pDefineOpenRoomInfo.m_fundP = rate*100 ;
				pDefineOpenRoomInfo.m_dMinCarry = JsonValueAdapter(normalInfos["minCarry"]);;
				pDefineOpenRoomInfo.m_dMaxCarry = JsonValueAdapter(normalInfos["maxCarry"]);
				pDefineOpenRoomInfo.m_dLittleBlind = JsonValueAdapter(normalInfos["smallBlind"]);
				pDefineOpenRoomInfo.m_dBigBlind = JsonValueAdapter(normalInfos["bigBlind"]);
				pDefineOpenRoomInfo.m_dMustBet = JsonValueAdapter(normalInfos["mustBet"]);
				pDefineOpenRoomInfo.m_dCoverCharge = JsonValueAdapter(normalInfos["coverCharge"]);
				pDefineOpenRoomInfo.m_dEnterMoney = JsonValueAdapter(normalInfos["entermoney"]);
				pDefineOpenRoomInfo.m_state = JsonValueAdapter(normalInfos["roomState"]);
				pDefineOpenRoomInfo.m_isMine = JsonValueAdapter(normalInfos["isOwner"]);
				


				m_pVipPrivateNormal->updateCurPrivateRoomInfo(pDefineOpenRoomInfo);

				bool flag  = false;

				for (int i=0; i<g_globalVipOpenRoom.m_UserDefineRoom.size(); i++)
				{
					if (pDefineOpenRoomInfo.m_uAreaID == g_globalVipOpenRoom.m_UserDefineRoom.at(i).m_uAreaID)
					{
						flag = true ;
					}
				}
				if (!flag)
				{
					g_globalVipOpenRoom.m_UserDefineRoom.push_back(pDefineOpenRoomInfo);
					m_pVipPrivateNormal->showRoomView(true) ;
				
				}
					m_pVipPrivateNormal->updateJoinRommBtShow(true) ;
			}
			else
			{
				m_pVipPrivateNormal->updateJoinRommBtShow(false) ;
				PopTipWithBt1("房间不存在","确定",nullptr) ;
			}

		}
		break ;
	case GameMsgCmdS::GetServerIsAutoTrust:
		{
			bool result = JsonValueAdapter(msg["result"]);
			if (m_pGameTable)
			{
				m_pGameTable->showGameAutoTrust(result) ;
			}
			
		}
		break ;
		//进入托管
	case GameMsgCmdS::GetSetServerAutoTrust:
		{
			m_pGameTable->showGameAutoTrust(true) ;
		}
		break ;


	case GameMsgCmdS::GetServerTipsToSinglePlayer:
		{
			std::string  tips = msg["info"].asString();
			char buff[64] ;

			MoveLabel	*m_messageLabel  = MoveLabel::create() ;
			//m_messageLabel->Appand(22,"fzlt.ttf","在刚刚结束的",Color4B::WHITE);
			m_messageLabel->Appand(22,"fzlt.ttf",tips.c_str(),Color4B(0xff,0xd2,0x00,0xff));
			m_messageLabel->setVisible(false) ;
			GameData::getInstance()->addGameTips(m_messageLabel);
		}
		break ;

	case GameMsgCmdS::GetRefshRebuyAndAddBuyCount:
		{
			unsigned m_rebuyCount = JsonValueAdapter(msg["rebuyNum"]);
			unsigned m_addonCount = JsonValueAdapter(msg["addonNum"]);
			g_globaMttGameInfo.m_rebuyCount = m_rebuyCount ;
			g_globaMttGameInfo.m_addonCount = m_addonCount ;
			m_pGameTable->updateMttButtonShow() ;
		}
		break ;
	case GameMsgCmdS::GetHasMatchNow:
		{
			
			
			unsigned id = JsonValueAdapter(msg["result"]);
			if (id==1)
			{
				unsigned tableType = JsonValueAdapter(msg["tableType"]);
				if (tableType == 1)//普通
				{
					setTableType(NormalTable) ;
				}
				else if (tableType == 2)//sng
				{
					m_pLogonLayerEx->setVisible(false);
					GameData::getInstance()->setHasRequestMatch(true) ;
					m_pGameHall->setVisible(true);
				}
				else if (tableType == 3)//mtt
				{
                    setTableType(MttTable) ;
					GameTipsManager::getInstance()->clean() ;
					GameData::getInstance()->setHasRequestMatch(true) ;
				}
								
			}
			else
			{
				if (m_pGameTable->isVisible())
				{
					BackToPrevView() ;
				}
				else
				{
					m_pLogonLayerEx->setVisible(false);
					GameData::getInstance()->setHasRequestMatch(true) ;
					m_pGameHall->setVisible(true);
				}


			}

		}
		break ;
	case GameMsgCmdS::GetMttFloatMatchPoolAndRoleCount:
		{
			unsigned pool = JsonValueAdapter(msg["floatRewardPool"]);
			unsigned num = JsonValueAdapter(msg["num"]);
			g_globaMttGameInfo.m_curMttTable.m_MttFudongReward.m_maxReward = pool ;
			g_globaMttGameInfo.m_curMttTable.m_MttFudongReward.m_realRegistCount = num ;
		}
		break ;


	case GameMsgCmdS::GetMttFloatMatchInfo:
		{
			m_mttFloatMatchRewardInfo.m_fLoatReward.clear();
			//mtt比赛模板数据
			auto& data = msg["mttFloatReward"] ;

			for (int i=0;i<data.size();i++)
			{
				auto& list = data[i] ;
				SingleFloatReward singleReward;


				std::string abasf = list["rangeNum"].asString();
				int index = abasf.find("-") ;
				std::string pre = abasf.substr(0,index) ;
				std::string end = abasf.substr(index+1,abasf.length()) ;

				singleReward.m_rolePre =  std::atoi( pre.c_str() ); 
				singleReward.m_roleEnd =  std::atoi( end.c_str() ); 

				
				auto& allReward = list["rankRewardList"] ;
				for (int j=0; j<allReward.size(); j++)
				{
					auto& rewardlist = allReward[j] ;
					MttReward reward;
					reward.m_bShowGold = false;
					reward.m_rankid = j+1;

					reward.m_score =  JsonValueAdapter(rewardlist["score"]);
					double rate =JsonValueAdapter(rewardlist["rate"]); 
					//reward.m_rewardInfo 
					char str[64] ;
					sprintf(str, "%.2f%%", rate);

					reward.m_rewardInfo = str ;

					reward.m_rolePre = JsonValueAdapter(rewardlist["minNum"]);
					reward.m_roleEnd = JsonValueAdapter(rewardlist["maxNum"]);

					if (reward.m_rolePre == reward.m_roleEnd)
					{
						sprintf(str, "%d", reward.m_rolePre);
					}
					else
					{
						sprintf(str, "%d-%d", reward.m_rolePre,reward.m_roleEnd);
					}
					
					reward.m_roleIntervals=str;

					singleReward.m_MttReward.push_back(reward);
				}
				m_mttFloatMatchRewardInfo.m_fLoatReward.push_back(singleReward);

			}
			m_mttFloatMatchRewardInfo.m_maxReward = 10000000 ;
			GameData::getInstance()->setGetMttFloatMathcInfo(true) ;
		}
		break ;
	case GameMsgCmdS::GetUpdateMttButtonShow:
		{

			
			g_globaMttGameInfo.m_showMttbutton =  JsonValueAdapter(msg["rebuy_addon"]);
			m_pGameTable->updateMttButtonShow() ;

		}
		break ;
	case GameMsgCmdS::GetUpdateMttRankAndAcore:
		{
			unsigned score = JsonValueAdapter(msg["score"]);
			g_globalMyData.setMttMasterScore(score);
			//g_globalMyData.setMttMasterRank(detailInfo.m_iMTTDaShiPaiMing);

			if (m_pGameMttHall!=nullptr)
			{
				m_pGameMttHall->updateMttHallInfo() ;
			}

		}
		break ;
	case GameMsgCmdS::GetMttMatchTableDissolve:
		{
			g_globaMttGameInfo.m_hasDismassTable = true ;
	
			m_pGameTable->doDismassMttTable() ;

		}
		break ;
	case GameMsgCmdS::GetMttMatchDissolve:
		{
			unsigned id = JsonValueAdapter(msg["id"]);
			std::string name = msg["name"].asString() ;
			std::string reason = msg["msg"].asString() ;
			showMttMatchDisMissTips(true,id,name,reason) ;
            if (m_pMTTMatchState != nullptr){
                m_pMTTMatchState->setVisible(false);
            }
		}
		break ;
	case GameMsgCmdS::GetMttUpdateTicket:
		{
			if (m_pGameMttHall!=nullptr)
			{
				m_MttChangedTicket.m_allTicket.clear() ;
				auto& ticket = msg["tickets"] ;
				for (int i=0; i<ticket.size();i++)
				{
					TableAndTicket item ;
					item.id = JsonValueAdapter(ticket[i]["id"]) ;
					item.num = JsonValueAdapter(ticket[i]["ticketNum"]) ;
					m_MttChangedTicket.m_allTicket.push_back(item) ;
				}


				m_pGameMttHall->updateTicket() ;
			}

		}
		break ;
	case GameMsgCmdS::GetMttRebuyResult:
		{
			

			unsigned result = JsonValueAdapter(msg["result"]);
			if (result ==0)
			{
				
			}
			if (result ==1) //成功
			{
				
				unsigned count = JsonValueAdapter(msg["leftNum"]) ;
				g_globaMttGameInfo.m_rebuyCount = count ;

				m_pGameTable->updateMttButtonShow() ;
			}

			std::string strTips = msg["msg"].asString() ;
			PopTipWithBt1(strTips.c_str(),"确定",nullptr) ;

		}
		break ;
	case GameMsgCmdS::GetMttAddonResult:
		{
			
			unsigned result = JsonValueAdapter(msg["result"]);
			if (result ==0)
			{
				
			}
			if (result ==1) //成功
			{
				unsigned count = JsonValueAdapter(msg["leftNum"]) ;
				g_globaMttGameInfo.m_addonCount = count ;
				m_pGameTable->updateMttButtonShow() ;
			}
			std::string strTips = msg["msg"].asString() ;
			PopTipWithBt1(strTips.c_str(),"确定",nullptr) ;

		}
		break ;
	case GameMsgCmdS::GetMttRebuyTipsByServer:
		{
			int time  = JsonValueAdapter(msg["time"]); 
			m_pGameTable->showMttRebuyTipsByServer(true,time) ;
		}
		break ;
	case GameMsgCmdS::GetMttSingleMatchInfo:
		{

			auto& data = msg["mttList"] ;

				char buff[128] ;
				char buff1[64] ;
				char buff2[64] ;

				GameMttTableInfo table;
				table.m_isFloatMatch = JsonValueAdapter(data["floatMatch"]); 
				table.m_iconurl = data["icon"].asString() ;
				table.m_tableName =data["name"].asString() ;
				table.m_tableID = JsonValueAdapter(data["id"]); 
				table.m_roleCountInGame = JsonValueAdapter(data["playerNum"]); 
				table.m_GameType = JsonValueAdapter(data["type"]);
				table.m_titleDay = data["dayTitle"].asString() ; //今日 日期等
				std::string timeTitle = data["timeTitle"].asString() ; //即将开始
				std::string openTime = data["openTime"].asString() ; //今日 日期等
				int leftTime =  JsonValueAdapter(data["leftSeconds"]); //倒计时时间
				

				table.m_showCountDown = JsonValueAdapter(data["showCountDown"]); 

				if (table.m_showCountDown)
				{
					table.m_TimeDes = timeTitle ; //即将开始
					table.m_strTime =  data["openTime"].asString() ;
				}
				else
				{
					table.m_TimeDes = table.m_titleDay; 
					table.m_strTime =  data["openTime"].asString() ;
				}

				table.m_matchTimeBeforeStart=JsonValueAdapter(data["registHour"]);
				table.m_realTime = JsonValueAdapter(data["leftSeconds"]); //倒计时时间
				table.m_isdelayRegist = JsonValueAdapter(data["delayRegist"]);  //是否延迟比赛
				table.m_freeTicket =  JsonValueAdapter(data["ticketId"]); 
				table.m_myTicket = JsonValueAdapter(data["ticketsNum"]); 
				table.m_entryFee = JsonValueAdapter(data["registFee"]); 
				table.m_fee = JsonValueAdapter(data["serviceFee"]); 


				table.m_canUseMoney=table.m_entryFee>0;
				table.m_canUseTicket=table.m_freeTicket>0;

				table.m_isFreeMathch = table.m_entryFee==0&&table.m_freeTicket==0 ;
				int addonCount =  JsonValueAdapter(data["addonNum"]); //增购次数
				int rebuyCount =  JsonValueAdapter(data["rebuyNum"]); //重购次数
				
				table.m_isRebuy = rebuyCount>0;
				table.m_isAddOn = addonCount>0  ;
				
			

				int siginstate = JsonValueAdapter(data["state"]); // 0未开放  1报名   2进行中  3已结束   4已报名
				table.m_SiginState = siginstate; //报名状态
		
				MttTableDes tableDes;
				
				if (table.m_canUseMoney&&table.m_canUseTicket)
				{
					sprintf(buff2,"%d+%d或1张%s",table.m_fee,table.m_entryFee,table.m_tableName.c_str()) ;//报名费
					tableDes.m_siginDes = buff2;
				}else if (table.m_canUseMoney)
				{
				    sprintf(buff,"%d+%d",table.m_fee,table.m_entryFee) ;//组装德州币
					tableDes.m_siginDes = buff;
				}else if (table.m_canUseTicket)
				{
					sprintf(buff,"1张%s",table.m_tableName.c_str()) ;//报名费
					tableDes.m_siginDes = buff;
				}

				sprintf(buff2,"%s%s",table.m_titleDay.c_str(),table.m_strTime.c_str()) ;//组装比赛时间

				tableDes.m_matchStartTime = buff2;

				tableDes.m_minRole = JsonValueAdapter(data["minPlayerNum"]);
				tableDes.m_maxRole =  JsonValueAdapter(data["maxPlayerNum"]);
				tableDes.m_rebuyCount =  JsonValueAdapter(data["rebuyNum"]);
				tableDes.m_rebuyMin = JsonValueAdapter(data["rebuyLevelMin"]);
				tableDes.m_rebuyMax =  JsonValueAdapter(data["rebuyLevelMax"]);
				tableDes.m_rebuyMoney = JsonValueAdapter(data["rebuyPrice"]);
				tableDes.m_rebuyToCarch = JsonValueAdapter(data["rebuyChips"]);

				tableDes.m_addOnCount =  JsonValueAdapter(data["addonNum"]);
				tableDes.m_addOnLevelMin =  JsonValueAdapter(data["addonLevelMin"]);
				tableDes.m_addOnLevelMax =  JsonValueAdapter(data["addonLevelMax"]);
				tableDes.m_addOnMoney =  JsonValueAdapter(data["addonPrice"]);
				tableDes.m_addOnToCarch =  JsonValueAdapter(data["addonChips"]);

				tableDes.m_firstMoney =  JsonValueAdapter(data["initChips"]);

				table.m_mttTableDes = tableDes;

				table.m_MttRank.clear() ;
				auto& blinddata = data["blinds"] ;
			
				int bdSize = blinddata.size();
				for (int j=0; j < blinddata.size(); j++)
				{
					int size = blinddata.size();
					MttBlind mttBlind;
					mttBlind.m_level = j+1;

					if ( j>=tableDes.m_rebuyMin&&j<=tableDes.m_rebuyMax)
					{
						mttBlind.m_isRebuy =true;
					}
					else
					{
						mttBlind.m_isRebuy =false;
					}
					if ( mttBlind.m_level>=tableDes.m_addOnLevelMin&&mttBlind.m_level<=tableDes.m_addOnLevelMax)
					{
						mttBlind.m_isAddon =true;
					}
					else
					{
						mttBlind.m_isAddon =false;
					}

					int blind	=JsonValueAdapter(blinddata[j]["blind"]); 
					sprintf(buff, "%d/%d", blind, blind*2);
					mttBlind.m_blind =buff ;
					mttBlind.m_preBlind = JsonValueAdapter(blinddata[j]["pre"]); ;
					mttBlind.m_time = JsonValueAdapter(blinddata[j]["time"]);
					table.m_MttBilnd.push_back(mttBlind);
				}
				//----------------------------------------
				auto& rewarddata = data["rewards"] ;
				for (int j=0; j<rewarddata.size(); j++)
				{
					MttReward mttReward ;
					mttReward.m_rankid = j+1;
		
					mttReward.m_rewardInfo = rewarddata[j]["itemDesc"].asString();
					mttReward.m_money = JsonValueAdapter(rewarddata[j]["money"]); 
					mttReward.m_score = JsonValueAdapter(rewarddata[j]["score"]); 
					mttReward.m_bShowItem =  mttReward.m_rewardInfo ==""?false:true ;
					mttReward.m_bShowGold = mttReward.m_money>0;
					mttReward.m_bShowScore = mttReward.m_score>0;
					table.m_MttReward.push_back(mttReward);
				}
				g_globaMttGameInfo.m_getRecord = true ;
				g_globaMttGameInfo.setCurMttMatchInfo(table) ;
				g_globaMttGameInfo.m_rebuyCount = g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_rebuyCount ;
				g_globaMttGameInfo.m_addonCount = g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_addOnCount ;

				
				
				m_pGameTable->updateMttButtonShow() ;
                showMttMatchState(false, 0) ;
				if (m_pMTTMatchState->curTableDataIsNULL())
				{
					showMttMatchState(true, 0) ;
				}

				m_gameMsgIO.SendGameMessage(GameMsgCmdC::CallRefshRebuyAndAddBuyCount);

				//g_globalMttHallInfo.m_mttTableInfo.push_back(table);
				
		}
		break ;
		//mtt 个人钱没有后的排名提示
	case GameMsgCmdS::GetMttRankByGameOver:
		{
			int rank = JsonValueAdapter(msg["rank"]) ;
			int money = JsonValueAdapter(msg["money"]) ;
			int score = JsonValueAdapter(msg["score"]) ;
			std::string name = msg["item"].asString() ;
			std::string iconUrl = msg["icon"].asString() ;
			std::string desc = msg["desc"].asString() ;
			g_globaMttGameInfo.m_isMttlose = true ;
			m_pGameTable->showMttRankView(true,rank,desc,iconUrl,name,money,score) ;

            updateMTTTask();
		}
		break ;

		//mtt排行列表
	case GameMsgCmdS::GetMttMatchRankList:
		{
			g_globaMttGameInfo.m_curMttTable.m_MttRank.clear();
			auto& data = msg["rankList"] ;
			for (int i=0; i<data.size(); i++)
			{
				MttRankInTable role;
				role.m_icon =   data[i]["icon"].asString() ;
				role.m_name =  data[i]["name"].asString() ;
				role.m_rankid =JsonValueAdapter(data[i]["rank"]) ;
				role.m_cash = JsonValueAdapter(data[i]["money"]) ;
				role.m_uid = JsonValueAdapter(data[i]["openid"]) ;
				g_globaMttGameInfo.m_curMttTable.m_MttRank.push_back(role) ;
			}
			m_pMTTMatchState->UpdatePaiMingInfo();
		}

		break ;
		//mtt退赛
	case GameMsgCmdS::GetMttMatchQuit:
		{
			//"rank", ri.rank, "num", num);
			unsigned result = JsonValueAdapter(msg["result"]);
			if (result ==0)
			{
				std::string strTips = msg["reason"].asString() ;
				PopTipWithBt1(strTips.c_str(),"确定",nullptr) ;
			}
			if (result ==1) //成功
			{
				//unsigned tableid = JsonValueAdapter(msg["id"]);
				unsigned state = JsonValueAdapter(msg["state"]);	
			
				if (m_pMTTMatchState!=nullptr &&m_pMTTMatchState->isVisible())
				{
					g_globaMttGameInfo.m_curMttTable.m_SiginState = state ;
					m_pMTTMatchState->UpdateGaiShuInfo(g_globaMttGameInfo.m_curMttTable) ;
				}
				unsigned useType = JsonValueAdapter(msg["useTicket"]);
				showQuitMatch(true,useType) ;
				
				if (m_pGameMttHall!=nullptr)
				{
					m_pGameMttHall->updateCurViewListByQuitMatch() ;
				}
			}
		}

		break ;
		//mtt报名
	case GameMsgCmdS::GetMttMatchSiginUp:
		{
			//"rank", ri.rank, "num", num);
			unsigned result = JsonValueAdapter(msg["result"]);
			if (result ==0)
			{
				std::string strTips = msg["msg"].asString() ;
				PopTipWithBt1(strTips.c_str(),"确定",nullptr) ;
			}
			if (result ==1) //成功
			{
				unsigned state = JsonValueAdapter(msg["state"]);	
				if (m_pMTTMatchState!=nullptr &&m_pMTTMatchState->isVisible())
				{
					g_globaMttGameInfo.m_curMttTable.m_SiginState = state ;
					m_pMTTMatchState->UpdateGaiShuInfo(g_globaMttGameInfo.m_curMttTable) ;
				}
				char buff[64] ;
				sprintf(buff,"%d",(g_globaMttGameInfo.m_curMttTable.m_fee+g_globaMttGameInfo.m_curMttTable.m_entryFee));

				showSignUpSucess(true,
					g_globaMttGameInfo.m_curMttTable.m_tableName.c_str(),
					g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_siginDes.c_str(),
					g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_matchStartTime.c_str()
					) ;
			}
		}

		break ;
		//牌桌上显示mtt排行xx/xx 
	case GameMsgCmdS::GetUpdateMttMatchRank:
		{
			//"rank", ri.rank, "num", num);
			unsigned rank = JsonValueAdapter(msg["rank"]);
			unsigned num = JsonValueAdapter(msg["num"]);
			g_globaMttGameInfo.m_rank = rank ;
			g_globaMttGameInfo.m_playerCount = num ;
            if (m_pGameTable != nullptr)
			    m_pGameTable->updateMttRankInTable(true) ;
		}
		break ;
		//进入mtt比赛后 倒计时
	case GameMsgCmdS::GetEnterMttGame:
		{
			unsigned reslut = JsonValueAdapter(msg["result"]);
			if (reslut ==0)
			{
				std::string  reason = msg["msg"].asString();
				PopTipWithBt1(reason.c_str(),"确定",nullptr) ;
			}
			else{
				unsigned time = JsonValueAdapter(msg["seconds"]);
				std::string  name = msg["tableName"].asString();
				unsigned tableid = JsonValueAdapter(msg["id"]);
				JoinToWaitingRoom(tableid,name,time) ;
			}
			
		}
		break ;
		//获取mtt比赛解散的消息
	case GameMsgCmdS::GetUpdateMttMatchDissmissNotify:
		{
			unsigned tableid = JsonValueAdapter(msg["id"]);
			std::string  name = msg["name"].asString();
			std::string  reason = msg["reason"].asString();

			showMttMatchDisMissTips(true,tableid,name,reason) ;
		}
		break ;
		//获取mtt比赛即将开始的提示
	case GameMsgCmdS::GetUpdateMttMatchCountDownNotify:
		{

			//type 0 每五分钟提示 1倒数60  2倒数20

			unsigned tableid = JsonValueAdapter(msg["id"]);
			std::string  name = msg["name"].asString();
			int type  = JsonValueAdapter(msg["type"]);
			int time = JsonValueAdapter(msg["time"]);


			if (type ==0)
			{
				showMttGameStart(true,name) ;
			}else if (type ==1)
			{
				showMttStartNotice(true,tableid,name,time) ;
			}else if(type ==2)
			{
				
				showJoinMatchTips(true,name,time,tableid) ;
			}

		}
		break ;
		//获取mtt 赛事单个列表人数
	case GameMsgCmdS::GetUpdateMttGameSiginPlayerNum:
		{
			unsigned tableid = JsonValueAdapter(msg["id"]);
			unsigned playerNum = JsonValueAdapter(msg["playerNum"]);	
			if (m_pGameMttHall)
			{
				m_pGameMttHall->updateMatchPlayerNum(tableid,playerNum) ;
			}
		}
		break ;
		//获取mtt大厅消息 更改当前状态
	case GameMsgCmdS::GetUpdateMttGameState:
		{
			////0未开放  1报名   2进行中  3已结束   4已报名 5 可进入 6 已解散
			unsigned tableid = JsonValueAdapter(msg["id"]);
			unsigned state = JsonValueAdapter(msg["state"]);	
			std::string timeTile = msg["timeTile"].asString();
			int delayRegistTime = JsonValueAdapter(msg["leftSeconds"]);
			std::string reason = msg["reason"].asString();
			bool delayRegist = JsonValueAdapter(msg["delayRegist"]);
			bool showCountDown = JsonValueAdapter(msg["showCountDown"]);
			
			if (m_pGameMttHall!=nullptr)
			{
				m_pGameMttHall->updateItemByTableID(tableid,state,timeTile,reason,delayRegist,delayRegistTime,showCountDown);
			}

		}
		break ;
		//更新mtt大厅数据
	case GameMsgCmdS::GetMttHallInfo:
		{
			auto& nameType = msg["types"];
			g_globalMttHallInfo.m_mttTypeName.clear() ;
			MttTypeName mMttTypeName;
			mMttTypeName.type = 0 ;
			mMttTypeName.name = "全部";
			g_globalMttHallInfo.m_mttTypeName.push_back(mMttTypeName) ;

			for (int i=0;i<nameType.size();i++)
			{
				MttTypeName sMttTypeName;
				sMttTypeName.type = JsonValueAdapter(nameType[i]["type"]) ;
				sMttTypeName.name = nameType[i]["name"].asString();
				g_globalMttHallInfo.m_mttTypeName.push_back(sMttTypeName) ;
			}

			g_globalMttHallInfo.m_allTypeCount =g_globalMttHallInfo.m_mttTypeName.size();

			//大厅排行
			unsigned rankid = JsonValueAdapter(msg["masterRank"]) ;
			unsigned mttscore = JsonValueAdapter(msg["score"]) ;

			g_globalMyData.setMttMasterScore(mttscore);
			g_globalMyData.setMttMasterRank(rankid);

			//大厅列表
			g_globalMttHallInfo.m_mttTableInfo.clear();
			auto& data = msg["mttList"];

			g_globalMttHallInfo.m_maxTableCount = data.size() ;
			for (int i=0;i<data.size();i++)
			{
				char buff[128] ;
				char buff1[64] ;
				char buff2[64] ;

				GameMttTableInfo table;
				table.m_isFloatMatch = JsonValueAdapter(data[i]["floatMatch"]); 

				

				table.m_IndexInPage = i%SinglePage ;
				table.m_Index = i ;
				table.m_iconurl = data[i]["icon"].asString() ;
				table.m_tableName =data[i]["name"].asCString() ;
				table.m_tableID = JsonValueAdapter(data[i]["id"]); 
				table.m_roleCountInGame = JsonValueAdapter(data[i]["playerNum"]); 
				table.m_GameType = JsonValueAdapter(data[i]["type"]);
				table.m_titleDay = data[i]["dayTitle"].asString() ; //今日 日期等
				std::string timeTitle = data[i]["timeTitle"].asString() ; //即将开始
				std::string openTime = data[i]["openTime"].asString() ; //今日 日期等
				int leftTime =  JsonValueAdapter(data[i]["leftSeconds"]); //倒计时时间
				

				table.m_showCountDown = JsonValueAdapter(data[i]["showCountDown"]); 

				if (table.m_showCountDown)
				{
					table.m_TimeDes = timeTitle ; //即将开始
					table.m_strTime =  data[i]["openTime"].asString() ;
				}
				else
				{
					table.m_TimeDes = table.m_titleDay; 
					table.m_strTime =  data[i]["openTime"].asString() ;
				}

				table.m_matchTimeBeforeStart=JsonValueAdapter(data[i]["registHour"]);
				table.m_realTime = JsonValueAdapter(data[i]["leftSeconds"]); //倒计时时间
				table.m_isdelayRegist = JsonValueAdapter(data[i]["delayRegist"]);  //是否延迟比赛
				table.m_freeTicket = JsonValueAdapter(data[i]["ticketId"]);  ;
				table.m_myTicket = JsonValueAdapter(data[i]["ticketsNum"]); 
				table.m_entryFee = JsonValueAdapter(data[i]["registFee"]); 
				table.m_fee = JsonValueAdapter(data[i]["serviceFee"]); 

				int addonCount =  JsonValueAdapter(data[i]["addonNum"]); //增购次数
				int rebuyCount =  JsonValueAdapter(data[i]["rebuyNum"]); //重购次数
				
				table.m_isRebuy = rebuyCount>0;
				table.m_isAddOn = addonCount>0  ;
				
				table.m_canUseMoney=table.m_entryFee>0;
				table.m_canUseTicket=table.m_freeTicket>0;
				table.m_isFreeMathch = table.m_entryFee==0&&table.m_freeTicket==0 ;
				int siginstate = JsonValueAdapter(data[i]["state"]); // 0未开放  1报名   2进行中  3已结束   4已报名
				
				table.m_SiginState = siginstate; //报名状态
				
				MttTableDes tableDes;
				
				if (table.m_isFreeMathch)
				{
					tableDes.m_siginDes = "免费";
				}
				else
				{
					if (table.m_canUseMoney&&table.m_canUseTicket)
					{
						sprintf(buff,"%d+%d或1张%s",table.m_fee,table.m_entryFee,data[i]["ticketName"].asCString()) ;//报名费

						tableDes.m_siginDes = buff;
					}else if (table.m_canUseMoney)
					{
						sprintf(buff,"%d+%d",table.m_fee,table.m_entryFee) ;//组装德州币
						tableDes.m_siginDes = buff;
					}else if (table.m_canUseTicket)
					{
						sprintf(buff,"1张%s",table.m_tableName.c_str()) ;//报名费
						tableDes.m_siginDes = buff;
					}
				}
				

				sprintf(buff2,"%s%s",table.m_titleDay.c_str(),table.m_strTime.c_str()) ;//组装比赛时间

				tableDes.m_matchStartTime = buff2;

				tableDes.m_minRole = JsonValueAdapter(data[i]["minPlayerNum"]);
				tableDes.m_maxRole =  JsonValueAdapter(data[i]["maxPlayerNum"]);

				tableDes.m_rebuyCount =  JsonValueAdapter(data[i]["rebuyNum"]);
				tableDes.m_rebuyMin = JsonValueAdapter(data[i]["rebuyLevelMin"]);
				tableDes.m_rebuyMax =  JsonValueAdapter(data[i]["rebuyLevelMax"]);
				tableDes.m_rebuyMoney = JsonValueAdapter(data[i]["rebuyPrice"]);
				tableDes.m_rebuyToCarch = JsonValueAdapter(data[i]["rebuyChips"]);

				tableDes.m_addOnCount =  JsonValueAdapter(data[i]["addonNum"]);
				tableDes.m_addOnLevelMin =  JsonValueAdapter(data[i]["addonLevelMin"]);
				tableDes.m_addOnLevelMax =  JsonValueAdapter(data[i]["addonLevelMax"]);
				tableDes.m_addOnMoney =  JsonValueAdapter(data[i]["addonPrice"]);
				tableDes.m_addOnToCarch =  JsonValueAdapter(data[i]["addonChips"]);

				tableDes.m_firstMoney =  JsonValueAdapter(data[i]["initChips"]);

				table.m_mttTableDes = tableDes;

				table.m_MttRank.clear() ;
		
				auto& blinddata = data[i]["blinds"] ;
			
				int bdSize = blinddata.size();
				for (int j=0; j < blinddata.size(); j++)
				{
					int size = blinddata.size();
					MttBlind mttBlind;
					mttBlind.m_level = j+1;

					if ( mttBlind.m_level>=tableDes.m_rebuyMin&&mttBlind.m_level<=tableDes.m_rebuyMax)
					{
						mttBlind.m_isRebuy =true;
					}
					else
					{
						mttBlind.m_isRebuy =false;
					}
					if ( mttBlind.m_level>=tableDes.m_addOnLevelMin&&mttBlind.m_level<=tableDes.m_addOnLevelMax)
					{
						mttBlind.m_isAddon =true;
					}
					else
					{
						mttBlind.m_isAddon =false;
					}

					int blind	=JsonValueAdapter(blinddata[j]["blind"]); 
					sprintf(buff, "%d/%d", blind/2, blind);
					mttBlind.m_blind =buff ;
					mttBlind.m_preBlind = JsonValueAdapter(blinddata[j]["pre"]); ;
					mttBlind.m_time = JsonValueAdapter(blinddata[j]["time"]);
					table.m_MttBilnd.push_back(mttBlind);
				}
				//----------------------------------------

				if (table.m_isFloatMatch)
				{
					table.m_MttFudongReward.m_fLoatReward = m_mttFloatMatchRewardInfo.m_fLoatReward ;
					table.m_MttFudongReward.m_maxReward = JsonValueAdapter(data[i]["floatRewardPool"]) ;
					g_globaMttGameInfo.m_curMttTable.m_MttFudongReward.m_realRegistCount = 0 ;
				}
				else
				{
					auto& rewarddata = data[i]["rewards"] ;
					for (int j=0; j<rewarddata.size(); j++)
					{
						MttReward mttReward ;
						mttReward.m_rankid = j+1;

						mttReward.m_rewardInfo = rewarddata[j]["itemDesc"].asString();
						mttReward.m_money = JsonValueAdapter(rewarddata[j]["money"]); 
						mttReward.m_score = JsonValueAdapter(rewarddata[j]["score"]); 
						mttReward.m_bShowItem =  mttReward.m_rewardInfo ==""?false:true ;
						mttReward.m_bShowGold = mttReward.m_money>0;
						mttReward.m_bShowScore = mttReward.m_score>0;
						table.m_MttReward.push_back(mttReward);
					}
				}


				/*			table.m_isRebuy = rand()%2 ;
				table.m_isAddOn = rand()%2 ;
				table.m_isFloatMatch = rand()%2 ;
				table.m_isdelayRegist = rand()%2 ;*/

				g_globalMttHallInfo.m_mttTableInfo.push_back(table);
			}
	
			float dealTime = (getCurrentTime()-m_startTime)/1000.00f;
			if (m_pGameMttHall->getEnterFormHall())
			{
				m_deqHallOrTable.back()->setVisible(false);
				m_pGameMttHall->setVisible(true);
				m_deqHallOrTable.push_back(m_pGameMttHall);
				m_pGameMttHall->updateMttHallList(dealTime) ;
			}
			else
			{
				m_pGameMttHall->updateCurViewList(dealTime);
			}
			PopLoading(false) ;
			showBgLayer(false);
			
		}
		break ;

	case GameMsgCmdS::GetContinueSiginReward:
		{
			bool bSuccess = JsonValueAdapter(msg["sendResult"]);
			if (bSuccess)
			{
				double signAward = JsonValueAdapter(msg["money"]);
				double dCash = g_globalMyData.GetCash();

				if (dCash!=signAward)
				{
					g_globalMyData.SetCash(signAward); 
				}

			}
		}
		
			
		break ;

	case GameMsgCmdS::GetSiginReward:
		{
			bool bSuccess = JsonValueAdapter(msg["sendResult"]);
			if (bSuccess)
			{
				double signAward = JsonValueAdapter(msg["money"]);
				double dCash = g_globalMyData.GetCash();
				//dCash += signAward;
				if (dCash!=signAward)
				{
					g_globalMyData.SetCash(signAward); 
					m_pGameHall->PopDailySignReward(true);
				}
				else
				{
					m_pGameHall->PopDailySignReward(false);
				}

				

			}
		}

		break ;


	case GameMsgCmdS::GetRobotRoomMessage:
		{
			auto chat = msg["text"];
			unsigned uChatType = JsonValueAdapter(chat["messageType"]);
			unsigned uChair = JsonValueAdapter(chat["chairID"]);

			if (m_pGameTable)
			{						
				unsigned uSUID = JsonValueAdapter(msg["suid"]);

				bool bLookOn = g_globalTableConfig.GetPlayerByID(uSUID) == nullptr;

				if (bLookOn)
				{
					std::string strName = msg["sname"].asString();
					if (uChatType == 0)
					{					
						std::string strText = chat["chatText"].asString();
						std::string strIcon = msg["icon"].asString();
						if (strText.length()!=0)
						{
							m_pGameTable->LookonUsrChat(uSUID, strName, strIcon, strText);
						}

					}
					else if (uChatType == 1)
					{
						unsigned uType = JsonValueAdapter(chat["type"]);
						unsigned uIdx = JsonValueAdapter(chat["faceID"]);
						m_pGameTable->LookonUsrExpress(strName, uIdx, uType);
					}
				}
				else
				{
					if (uChatType == 0)
					{					
						std::string strText = chat["chatText"].asString();

						if (strText.length()!=0)
						{
							m_pGameTable->UsrChat(g_globalTableConfig.GetViewChairID(uChair), strText);
						}

					}
					else if (uChatType == 1)
					{
						unsigned uType = JsonValueAdapter(chat["type"]);
						unsigned uIdx = JsonValueAdapter(chat["faceID"]);
						m_pGameTable->UsrExpress(g_globalTableConfig.GetViewChairID(uChair), uIdx, uType);
					}
				}				
			}
		}
		break;

	case GameMsgCmdS::GetContinueTicketLogin:
		{
			bool bSuccess = JsonValueAdapter(msg["sendResult"]);
			if (bSuccess)
			{
				if (g_globalMyData.m_continueLoginType == 0)
				{
					double signAward = JsonValueAdapter(msg["money"]);
					double dCash = g_globalMyData.GetCash();
					dCash += signAward;
					g_globalMyData.SetCash(dCash); 
					PopDrawingAwardAni(signAward);
				}
				else if (g_globalMyData.m_continueLoginType == 1)
				{
					m_pGameHall->PopDailySignReward(false);
				}
			}
		}
		break ;

	case GameMsgCmdS::CheckOnline:
		m_onlineFlags = 2;
		break;

	case GameMsgCmdS::GetMasterScore:
		{
			int score = JsonValueAdapter(msg["master"]) ;
			g_globalMyData.setSngMasterScore(score);
		}

		break ;
		//服务器发来的桌面消息
	case GameMsgCmdS::GetServerTableScorllTips:
		{
			std::string sTableName = msg["table"].asString() ;
			std::string sUName = msg["name"].asString() ;
			unsigned sMoney =  JsonValueAdapter(msg["money"]);
			unsigned sScore =  JsonValueAdapter(msg["master"]);

			std::string rewardDes = msg["rewardDes"].asString() ;

			char buff[64] ;

			MoveLabel			*m_messageLabel  = MoveLabel::create() ;
			m_messageLabel->Appand(22,"fzlt.ttf","在刚刚结束的",Color4B::WHITE);
			m_messageLabel->Appand(22,"fzlt.ttf",sTableName.c_str(),Color4B(0xff,0xd2,0x00,0xff));
			m_messageLabel->Appand(22,"fzlt.ttf","比赛中，",Color4B::WHITE);
			m_messageLabel->Appand(22,"fzlt.ttf",sUName.c_str(),Color4B(0xff,0xd2,0x00,0xff));
			m_messageLabel->Appand(22,"fzlt.ttf","获得",Color4B::WHITE);
			m_messageLabel->Appand(22,"fzlt.ttf","第一名，",Color4B(0xff,0xd2,0x00,0xff));
			m_messageLabel->Appand(22,"fzlt.ttf","奖励",Color4B::WHITE);
			m_messageLabel->Appand(22,"fzlt.ttf",rewardDes,Color4B(0xff,0xd2,0x00,0xff));
			m_messageLabel->setVisible(false) ;
			GameData::getInstance()->addGameTips(m_messageLabel);

		}



		break ;

		//服务器发来的桌面消息
	case GameMsgCmdS::GetServerTableTips:



		SendTableTip(msg["content"].asString()) ;

		break ;

	case GameMsgCmdS::GetSngGameOver:
		{
			SendClientRequest(ClientRequest::kCallLookUp);
			g_globalSngTableInfo.m_isGameOver = true ;
			m_pGameTable->showSngGameOver(true) ;

            updateSNGTask();
		}
		break ;
	case GameMsgCmdS::GetMttGameOver:
		{
			//SendClientRequest(ClientRequest::kCallLookUp);
			g_globaMttGameInfo.m_isGameOver = true ;
			if (m_pGameTable->isVisible())
			{
				m_pGameTable->showMttGameOver(true) ;
			}
			
		}
		break ;

	case GameMsgCmdS::GetSngMyRank:
		{
			g_globalSngTableInfo.m_rank = JsonValueAdapter(msg["rank"]) ;
			m_pGameTable->updateSngTableRankTips() ;
		}
		break ;
	case GameMsgCmdS::GetSngPlayerFailedRankInTable:
		{
			//cmd=3011 body={"chair":2,"rank":5}
			int chairid = JsonValueAdapter(msg["chair"]) ;
			int rank = JsonValueAdapter(msg["rank"]) ;



			m_pGameTable->callShowSngRankInFailed(chairid,rank) ;
		}
		break;
	case GameMsgCmdS::GetShowSngRankView:
		{
			//SendClientRequest(ClientRequest::kCallLookUp);

			unsigned tickedNum = JsonValueAdapter(msg["ticketNum"]) ;
			for (int i=0;i<g_globalSngHallInfo.m_sngTableInfo.size();i++)
			{
				GlobalSngTableInfo tableInfo = g_globalSngHallInfo.m_sngTableInfo.at(i) ;

				if (g_globalSngTableInfo.m_tableId == tableInfo.m_tableID)
				{
					tableInfo.m_myTicket = tickedNum ;
				}

			}
			g_globalSngHallInfo.m_curJoinTable.m_myTicket = tickedNum ;
			g_globalSngTableInfo.m_isSnglose = true ;
			m_pGameTable->showSngRankView(true,JsonValueAdapter(msg["rank"]),JsonValueAdapter(msg["bonus"]),JsonValueAdapter(msg["master"]));


		}
		break ;
	case GameMsgCmdS::GetSngMatchRankList:
		{
		
				g_globalSngTableInfo.m_sngRoleInfo.clear() ;
				auto& data = msg["rankList"] ;
				for (int i=0; i<data.size(); i++)
				{
					SngRoleInfo role;
					role.m_icon =   data[i]["icon"].asString() ;
					role.m_name =  data[i]["name"].asString() ;
					role.m_rankid =JsonValueAdapter(data[i]["rank"]) ;
					role.m_cash = JsonValueAdapter(data[i]["money"]) ;
					role.m_uid = JsonValueAdapter(data[i]["openid"]) ;
					g_globalSngTableInfo.m_sngRoleInfo.push_back(role) ;
				}
				m_pGameTable->updateSngRankList() ;
	
			
		}
		break ;

	case GameMsgCmdS::GetSngTableMangzhu:
		{
			g_globalSngTableInfo.m_sngShengmang.clear();
			auto& data = msg["blindList"] ;

			for (int i=0; i<data.size(); i++)
			{
				SngShengmang mangzhu;
				mangzhu.m_level =  JsonValueAdapter(data[i]["level"]) ;
				mangzhu.m_mangzhu =  data[i]["blinds"].asString() ;
				mangzhu.m_qiangzhu =JsonValueAdapter(data[i]["prev"]) ;
				mangzhu.m_time = JsonValueAdapter(data[i]["time"]) ;
				g_globalSngTableInfo.m_sngShengmang.push_back(mangzhu) ;
			}

			m_pGameTable->updateSngUIBlind() ;

			//g_globalSngTableInfo.m_sngShengmang
		}

		break ;

	case GameMsgCmdS::GetSngHallInfo:

		{
			g_globalSngHallInfo.m_sngTableInfo.clear() ;
			/*	g_globalSngHallInfo.m_masterRank = 20 ;
			g_globalSngHallInfo.m_rank = 15 ;*/
			auto& data = msg["SNGRoomInfo"] ;
			int length = msg.size();

			std::string str_tableName[6] = {"菜鸟场","普通场","精英场","卓越场","大师场","宗师场",} ;
			int tanbleImage[6] = {1,4,0,4,3,2} ;
			for (int i=0;i<6;i++)
			{
				GlobalSngTableInfo table0;

				table0.m_index = i ;
				table0.m_tableName = str_tableName[i] ;
				table0.m_ImgID = tanbleImage[i] ;
				table0.m_tableID = JsonValueAdapter(data[i]["id"]) ;

				table0.m_maxPlayRoleCount =   JsonValueAdapter(data[i]["capacity"]) ;
				table0.m_roleCountInGame =  JsonValueAdapter(data[i]["playerNum"]) ;
				table0.m_rewardCount =table0.m_maxPlayRoleCount==6?2:3 ;


				auto& bouns = data[i]["bonus"] ;
				auto& masterScore = data[i]["master"] ;
				for (int j=0; j<table0.m_rewardCount; j++)
				{
					table0.m_reward[j] = JsonValueAdapter(bouns[j]) ;
					table0.m_score[j]= JsonValueAdapter(masterScore[j]) ;
				}
				table0.m_entryFee = JsonValueAdapter(data[i]["entryFee"]) ;
				table0.m_fee =  JsonValueAdapter(data[i]["serviceFee"]) ;
				table0.m_freeTicket = 1 ;
				table0.m_myTicket =  JsonValueAdapter(data[i]["ticketNum"]) ;
				g_globalSngHallInfo.m_sngTableInfo.push_back(table0) ;
			}
			if (m_pGameSngHall!=nullptr&&m_pGameSngHall->isVisible())
			{
				m_pGameSngHall->updateGameSngData() ;
			}


		};




		break ;



		//通知玩家加入俱乐部
	case GameMsgCmdS::NoticeRoleUpdateFamilyInfo:

		{
			unsigned fid =  JsonValueAdapter(msg["fid"]);

			char tips[128] ;

			int type = JsonValueAdapter(msg["type"]);

			std::string fname = msg["data"].asString() ;
			if (type ==  Notic_JoinFamily)
			{
				
				sprintf(tips,"您的入会申请被通过，恭喜您加入 %s 俱乐部",fname.c_str()) ;
				PopTipLayerWithBt1(tips,"确定",nullptr) ;

				if (g_globalMyFamilyInfo.m_familyID == fid)
				{
					m_pGameHall->updateSelfClubView() ;
				}

			}
			if (type ==  Notic_ToViceSHaikh)
			{

				std::string iconUrl = g_globalMyData.GetSkinID() == 0 ? g_globalMyData.GetIconURL() : 
					g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl;

				FamilyRoleBaseInfo admin ;
				admin.m_uid = g_globalMyData.m_iUserId ;
				admin.m_iconUrl = iconUrl; // g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl;
				admin.m_power = 1 ;
				g_globalMyFamilyInfo.m_AllAdmin.push_back(admin);
				m_pGameHall->callUpdateAdminList() ;

				g_globalMyData.setFamilyPower(1) ;
				PopTipLayerWithBt1("恭喜您被提升为副族长","确定",nullptr) ;

			}
			if (type ==  Notic_NOViceSHaikh)
			{

		/*		std::vector<FamilyRoleBaseInfo>::iterator iter;
				iter = g_globalMyFamilyInfo.m_AllAdmin.begin();

				for (int i=0; i<g_globalMyFamilyInfo.m_AllAdmin.size(); i++)
				{
					if (g_globalMyData.m_iUserId == g_globalMyFamilyInfo.m_AllAdmin.at(i).m_uid)
					{
						g_globalMyFamilyInfo.m_AllAdmin.erase(iter+i);
						m_pGameHall->callUpdateAdminList() ;
					}
				}*/



				g_globalMyData.setFamilyPower(0) ;
				PopTipLayerWithBt1("您被撤销了副族长职务","确定",nullptr) ;
			}
			if (type ==  Notic_NewShaikh)
			{



				//std::vector<FamilyRoleBaseInfo>::iterator iter;
				//iter = g_globalMyFamilyInfo.m_AllAdmin.begin();
				//for (int i=0; i<g_globalMyFamilyInfo.m_AllAdmin.size(); i++)
				//{
				//	if (g_globalMyFamilyInfo.m_AllAdmin.at(i).m_power==2)
				//	{
				//		g_globalMyFamilyInfo.m_AllAdmin.erase(iter+i);

				//	}
				//}

				//iter = g_globalMyFamilyInfo.m_AllAdmin.begin();
				//for (int i=0; i<g_globalMyFamilyInfo.m_AllAdmin.size(); i++)
				//{
				//	if (g_globalMyData.m_iUserId==g_globalMyFamilyInfo.m_AllAdmin.at(i).m_uid)
				//	{
				//		g_globalMyFamilyInfo.m_AllAdmin.erase(iter+i);

				//	}

				//}


				//FamilyRoleBaseInfo admin ;
				//admin.m_uid = g_globalMyData.m_iUserId ;
				//admin.m_iconUrl = g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl;
				//admin.m_power = 2 ;
				//g_globalMyFamilyInfo.m_AllAdmin.push_back(admin);
				//m_pGameHall->callUpdateAdminList() ;

				g_globalMyData.setFamilyID(fid);
				g_globalMyData.setFamilyName(fname.c_str()) ;
				g_globalMyData.setFamilyPower(2) ;

				sprintf(tips,"恭喜您被提升为%s新的俱乐部长",fname.c_str()) ;
				PopTipLayerWithBt1(tips,"确定",nullptr) ;

			}
			if (type ==  Notic_PushOutFamily)
			{
				sprintf(tips,"您被%s逐出了俱乐部",fname.c_str()) ;
				PopTipLayerWithBt1(tips,"确定",nullptr) ;
				m_pGameHall->showSelfClub(false) ;

					
				std::vector<FamilyInfo>::iterator iter2;
				iter2 = g_globaSelfJoinClub.m_vecSelfJoinClub.begin();
				for (int i=0; i<g_globaSelfJoinClub.m_vecSelfJoinClub.size(); i++)
				{
					if (fid == g_globaSelfJoinClub.m_vecSelfJoinClub.at(i).m_familyID)
					{
						g_globaSelfJoinClub.m_vecSelfJoinClub.erase(iter2+i) ;
						m_pGameHall->callUpdateFamilyRankList() ;

					}
				}

				for (auto &club:g_globalFamilyInfo.m_vecFamilyInfo)
				{
					if (club.m_familyID == fid)
					{
						club.m_hasJoin = false ;
						m_pGameHall->callUpdateFamilyRankList() ;
					}
				}

			}
		}

		break ;


	case GameMsgCmdS::LogonResult: // 有时候进入游戏后可能还会再次受到该消息
		{
			bool bSuccess = JsonValueAdapter(msg["isSuccess"]);
			if (bSuccess)
			{
				// inTable, 0: 不在桌子上, 1: 在桌子上
				//unsigned inTable = JsonValueAdapter(msg["inTable"]);
				//if (inTable == 0 && g_globalTableConfig.m_uTableID >= 0) {
				//	// 不在桌子上，要离开桌子
				//	m_pGameTable->ShowMeHasActioned();
				//	m_gameMsgIO.SendGameMessage(GameMsgCmdC::LeaveTable);
				//	m_roomMsgIO.StopIO();
				//	stopVideo();
				//	BackToPrevView();
				//	g_globalGameSerialization.AddAtomicAction(AtomicAction::GameEnd);
				//	g_globalTableConfig.Clear();
				//	g_globalTableConfig.ClearPlayer();
				//} else if (inTable == 1 && g_globalTableConfig.m_uTableID >= 0) {
				//	// 在桌子上，什么不做
				//} else if (inTable == 1 && g_globalTableConfig.m_uTableID < 0) {
				//	// 在大厅，离开桌子
				//	m_gameMsgIO.SendGameMessage(GameMsgCmdC::LeaveTable);
				//} else if (inTable == 0 && g_globalTableConfig.m_uTableID < 0) {
				//	if (m_pGameHall2 != nullptr && m_pGameHall2->isVisible()) {
				//		m_pGameHall2->refreshHallInfo();
				//	}
				//}

				m_loginFlag = true;
				if (!m_pLogonLayerEx->isVisible())
				{
					PopLoading(false);
					resetCheckNetwork();
					m_checkDuration = 0.0f;
					m_lastDeltaTime = 0.0f;
					if (g_globalTableConfig.m_uTableID >= 0)
					{
						ConnectChatServer();
					}


					if (m_pGameTable&&m_pGameTable->isVisible())
					{

						if (m_pGameTable->getTableType() == SngTable)
						{
							PopLoading(false);
							m_checkDuration = 0.0f;
							m_lastDeltaTime = 0.0f;
							if (m_pGameTable != nullptr && g_globalTableConfig.m_uTableID >= 0) {
								g_pGameFrame->forceLeaveTable();
							} else if (m_pGameHall != nullptr) {
								g_pGameFrame->forceLeaveHall();
							}
							SendClientRequest(ClientRequest::kRequsetIsAutoTrust);
						}

						else if (m_pGameTable->getTableType() == MttTable)
						{
							SendClientRequest(ClientRequest::kRequsetIsAutoTrust);
						}
						else
						{
							requestHasMatchByInTable(true) ;
						}
					}

					if (m_pGameMttHall!=nullptr && m_pGameMttHall->isVisible())
					{
						m_pGameMttHall->updateFixTime(0) ;
					}



					return;
				}



			


				preloadHallTable();

				Json::Value params;
				OnClientRequest(ClientRequest::kGetDailyLogin, params);
			}
			else
			{
				SendGameTip("游戏服务器返回连接失败！");
				cleanLogin() ;
			}
		}
		break;
	case GameMsgCmdS::ContinueLoginResult:
		{
			bool bSuccess = JsonValueAdapter(msg["sendResult"]);
			if (bSuccess)
			{
				if (g_globalMyData.m_continueLoginType == 0)
				{
					double signAward = JsonValueAdapter(msg["money"]);
					double dCash = g_globalMyData.GetCash();
					dCash += signAward;
					g_globalMyData.SetCash(dCash); 
					PopDrawingAwardAni(signAward);
				}
				else if (g_globalMyData.m_continueLoginType == 1)
				{
					m_pGameHall->PopDailySignReward(false);
				}
			}
		}
		break;



	case GameMsgCmdS::DrawVipLoginAwardResut:
		{
			bool bSuccess = JsonValueAdapter(msg["sendResult"]);
			if (bSuccess)
			{
				double dVipAward = JsonValueAdapter(msg["beans_after"]);
				double dCash = g_globalMyData.GetCash();
				dCash += dVipAward;
				g_globalMyData.SetCash(dCash); 
				//m_pGameHall->UpdateMyInfo();

				PopDrawingAwardAni(dVipAward);
				g_globalMyData.m_bNeedShowVipAward = false;
			}
		}
		break;

	case GameMsgCmdS::DrawWaveGrilLoginAwardResult:
		{
			bool bSuccess = JsonValueAdapter(msg["sendResult"]);
			if (bSuccess)
			{
                g_globalMyData.m_WaveGrilAward = -1;
				double dWaveMoney = JsonValueAdapter(msg["money"]);
				double dCash = g_globalMyData.GetCash();
				dCash += dWaveMoney;
				g_globalMyData.SetCash(dCash); 
				PopDrawingAwardAni(dWaveMoney);
			}
		}
		break;

	case GameMsgCmdS::EnterTableFailed:
		{
			PopLoading(false);
			unsigned uTag = JsonValueAdapter(msg["tag"]);
			if (uTag == 0) // 钱不够
			{
				PopTipLayerWithBt2(msg["data"].asString(), "去充值", "返回", [](unsigned uBt){
					if (uBt == 0)
					{
						Json::Value param;
						param["idx"] = 0;
						SendClientRequest(ClientRequest::kPopMall, param);
					}
				});
			}
			else
			{
				PopTipLayerWithBt1(msg["data"].asString(), "确定");
			}

		}
		break;

	case GameMsgCmdS::SitDownFailed:
		{
			PopTipLayerWithBt1(msg["data"].asString(), "确定");
		}
		break;

	case GameMsgCmdS::GameScence: // 
		{
            m_bIsRecvGameScence = true;
			StopBackGroundSound(false);

			m_pGameTable->CleanTable();
			

			m_pGameTable->CleanPlayer();
			g_globalTableConfig.ClearPlayer();

			m_pGameHall->reSetScorlllTips() ;
			m_pGameTable->callNextScrollText() ;

			unsigned uTableID = JsonValueAdapter(msg["tid"]);
			int uChairID = JsonValueAdapter(msg["cid"]);
			unsigned uRoomID = JsonValueAdapter(msg["rid"]);
			unsigned uAreaID = 0;

			if (uRoomID >= 0 && uRoomID < 6)
			{
				uAreaID = 0;
				TrackMgr->event("GameScene", "初出茅庐");
			}
			else if (uRoomID >= 6 && uRoomID < 12)
			{
				uAreaID = 1;
				TrackMgr->event("GameScene", "小有成就");
			}
			else if (uRoomID >= 12 && uRoomID < 22)
			{
				uAreaID = 2;
				TrackMgr->event("GameScene", "小有成就");
			}
			else if (uRoomID >= 22 && uRoomID <100)
			{
				uAreaID = 3;
				TrackMgr->event("GameScene", "大显身手");
			}
			else
			{
				uAreaID = uRoomID;
			}
			g_globalTableConfig.m_uRoomType = uAreaID;
			bool bIsVipRoom = JsonValueAdapter(msg["isvip"]);
			if (bIsVipRoom)
			{
                uAreaID = 4;
				g_globalTableConfig.m_uRoomType = 4;
				TrackMgr->event("GameScene", "贵族开房");
			}
			bool bLookOn = JsonValueAdapter(msg["ifLookOn"]);
			g_globalTableConfig.SetTableMode(bLookOn ? TableLooking : TablePlaying);
			if (bLookOn) // 打补丁，旁观模式下，服务端可能传一个无效椅子号，比如5人场传一个椅子号5，这回导致椅子号转换到view模式下时，部分玩家显示重叠，这里设置为0
			{
				uChairID = 0;
			}		

			g_globalTableConfig.m_reason=0 ;
			g_globalTableConfig.SetMyChairID(uChairID);
			double dBigBlind = JsonValueAdapter(msg["minBet1_2"]);
			g_globalTableConfig.m_strRoomName = msg["roomDes"].asString();
			g_globalTableConfig.m_uAreaID = uAreaID;
			g_globalTableConfig.m_uRoomID = uRoomID;
            g_globalTableConfig.m_bIsMTTScene = JsonValueAdapter(msg["mtt"]);
			g_globalTableConfig.m_uChairID = uChairID;
			g_globalTableConfig.m_uTableID = uTableID;
			g_globalTableConfig.m_uMaxPlayer = JsonValueAdapter(msg["numlimit"]);
			g_globalTableConfig.m_uPotLimitTime = 1;
			g_globalTableConfig.m_dLittleBlind = dBigBlind / 2;
			g_globalTableConfig.m_dBigBlind = dBigBlind;
			g_globalTableConfig.m_uCardRangeLow = 5;
			g_globalTableConfig.m_uCardRangeHigh = 14;
			g_globalTableConfig.m_dRewardMoney = JsonValueAdapter(msg["rewardMoney"]);
			g_globalTableConfig.m_dMagicExpressPrice = JsonValueAdapter(msg["magicMoney"]);
			g_globalTableConfig.m_dAutoBringMoney = JsonValueAdapter(msg["autoBringMoney"]);
			g_globalTableConfig.m_dMinBringMoney = JsonValueAdapter(msg["minBringMoney"]);
			g_globalTableConfig.m_dMaxBringMoney = JsonValueAdapter(msg["maxBringMoney"]);
			g_globalTableConfig.m_uAutoSupplyToMaxMoney = JsonValueAdapter(msg["ifAutoGetMax"]);
			g_globalTableConfig.m_dAutoSupplyMoney = JsonValueAdapter(msg["AutoGetMaxMoney"]);
			g_globalTableConfig.m_enterMoney = JsonValueAdapter(msg["entermoney"]);

			RewardListInfo* rewardlistinfo = nullptr;
			rewardlistinfo = g_globalTableConfig.m_RewardListInfo;
			rewardlistinfo->m_uUpdateCount = JsonValueAdapter(msg["reward"]);
			rewardlistinfo->m_uInput =	JsonValueAdapter(msg["pushReward"]);
			rewardlistinfo->m_uRoyalCount =	JsonValueAdapter(msg["hjThFlash"]);
			rewardlistinfo->m_uflowersCount =	JsonValueAdapter(msg["thFlash"]);
			auto rankList = msg["rankList"];
			for (int i = 0; i < rankList.size(); i++)
			{
				rewardlistinfo[i].m_uRankID = JsonValueAdapter(rankList[i]["id"]);
				rewardlistinfo[i].m_dRankCount = JsonValueAdapter(rankList[i]["money"]);
				rewardlistinfo[i].m_strNickName = rankList[i]["nickname"].asString();
				rewardlistinfo[i].m_bIfShow = JsonValueAdapter(rankList[i]["type"]);
				rewardlistinfo[i].m_uID = JsonValueAdapter(rankList[i]["uid"]);
			}	
			ConnectChatServer();
			GameData::getInstance()->setSoundOn(false);
			EnterGameTable();

			for(auto& quickFollow : g_globalQuickFollowInfo.m_vecQuickFollowInfo[0])
			{
				/*QuickFollowInfo MyQuickFollowInfo;
				MyQuickFollowInfo.m_uType = quickFollow.m_uType;
				MyQuickFollowInfo.m_uUserID = quickFollow.m_uUserID;
				MyQuickFollowInfo.m_strNickName = quickFollow.m_strNickName;
				MyQuickFollowInfo.m_strIconUrl = quickFollow.m_strIconUrl;
				MyQuickFollowInfo.m_dCash = quickFollow.m_dCash;
				MyQuickFollowInfo.m_bOnLine = quickFollow.m_bOnLine;
				MyQuickFollowInfo.m_uAreaID = quickFollow.m_uAreaID;
				MyQuickFollowInfo.m_uTableID = quickFollow.m_uTableID;
				MyQuickFollowInfo.m_dLittleBlind = quickFollow.m_dLittleBlind;
				MyQuickFollowInfo.m_dBigBlind = quickFollow.m_dBigBlind;
				MyQuickFollowInfo.m_bHasPassword = quickFollow.m_bHasPassword;
				m_pGameTable->OperateOnlineFollow(2, 0, MyQuickFollowInfo);*/
                m_pGameTable->OperateOnlineFollow(2, 0, quickFollow);
			}
            for (auto& quickFollow : g_globalQuickFollowInfo.m_vecQuickFollowInfo[1])
            {
                m_pGameTable->OperateOnlineFollow(2, 1, quickFollow);
            }
			m_bIfYaoQing = false;
			onScheduleIfRemoveShouYaoQing(1);
			
			//SendClientRequest(ClientRequest::kQQLogon);
			m_pGameTable->SetValue(rewardlistinfo->m_uUpdateCount);
			m_pGameTable->UpdateItemBox();
			//bool bLookOn = JsonValueAdapter(msg["ifLookOn"]);
			//g_globalTableConfig.SetTableMode(bLookOn ? TableLooking : TablePlaying);
			//if (!bLookOn)
			//{
			//	g_globalTableConfig.AddPlayer(uChairID, g_globalMyData.m_iUserId, g_globalTableConfig.m_dAutoBringMoney, g_globalMyData.GetNickName(), g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl, g_globalMyData.GetSkinID(), g_globalMyData.GetGender());
			//	m_pGameTable->SetUser(g_globalTableConfig.GetMyViewChairID(), g_globalMyData.GetNickName(), g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl, g_globalTableConfig.m_dAutoBringMoney, g_globalMyData.m_iUserId, g_globalMyData.GetSkinID(), g_globalMyData.GetVipLevel(), 0, 0,g_globalMyData.GetGender());
			//}
			//else
			//{
			// bug1557
			//PopTipLayerWithBt2("很抱歉，该桌子人数已满，您可以旁观牌局等到有空时坐下。", "继续旁观", "返回", 
			//	[](unsigned uBt){
			//	if (uBt == 1)
			//	{
			//		SendClientRequest(ClientRequest::kLeaveTable);
			//	}
			//});
			//}

			bool bHasPlayer = JsonValueAdapter(msg["ifHave"]);
			auto otherPlayer = msg["otherPerson"];
			for (int i = 0; i < otherPlayer.size(); i++)
			{
				auto& player = otherPlayer[i];
				uChairID = JsonValueAdapter(player["roleChair"]);
				if (i == 0 && bLookOn)
				{
					g_globalTableConfig.SetMyChairID(uChairID);
				}				
				unsigned uViewChairID = g_globalTableConfig.GetViewChairID(uChairID);
				unsigned u = g_globalTableConfig.GetChairID(uViewChairID);
				int uBiaoJiId = -1;
				if (player["tip"].isObject() && !player["tip"].isNull())
				{
					uBiaoJiId = JsonValueAdapter(player["tip"]["tipId"]);
					//++uBiaoJiId;
				}
				unsigned uLadyId = JsonValueAdapter(player["is_guan"]);
				std::string strVideoStreamUrl = player["streamUrl"].asString();
				m_pGameTable->SetUser(uViewChairID,
					JsonValueAdapter(player["name"]), 
					player["icon"].asString(),
					JsonValueAdapter(player["getMoney"]), JsonValueAdapter(player["id"]), 
					
					JsonValueAdapter(player["voiceId"]),
					JsonValueAdapter(player["skin_id"]),
					JsonValueAdapter(player["vip_lv"]),
					uBiaoJiId, uLadyId, JsonValueAdapter(player["gender"]), strVideoStreamUrl);
				int iPlayerStatus = JsonValueAdapter(player["roleStatus"]);
				m_pGameTable->SetUserShowName(uViewChairID, iPlayerStatus);
				m_pGameTable->SetHandCard(uViewChairID, iPlayerStatus);
				double dBeted = JsonValueAdapter(player["curBet"]);
				if (dBeted > 1)
				{
					m_pGameTable->SetUsrBetedJetton(uViewChairID, dBeted);
				}

				g_globalTableConfig.AddPlayer(uChairID, JsonValueAdapter(player["id"]), JsonValueAdapter(player["getMoney"]), 
					player["name"].asString(), player["icon"].asString(), JsonValueAdapter(player["voiceId"]),JsonValueAdapter(player["skin_id"]), JsonValueAdapter(player["gender"]));
			}

			if (otherPlayer.size() > 0) {
				PlayEffectSound("sound/act/sit.mp3");
			}

			auto arrPool = msg["poolArr"];
			std::vector<double> vecSidePot;
			double dAllPot = 0;
			for (int i = 0; i < arrPool.size(); i++)
			{
				if (i == 0)
				{
					dAllPot = JsonValueAdapter(arrPool[i]);
				}
				else
				{
					vecSidePot.push_back(JsonValueAdapter(arrPool[i]));
				}
			}
			m_pGameTable->SetAllPot(dAllPot);
			if (!vecSidePot.empty())
			{
				m_pGameTable->SetSidePot(vecSidePot);
			}			

			auto publicCard = msg["matchCard"];
			std::vector<unsigned char> vecPublicCard;
			for (int i = 0; i < publicCard.size(); i++)
			{
				vecPublicCard.push_back((unsigned)JsonValueAdapter(publicCard[i]));
			}
			m_pGameTable->SetPublicCard(vecPublicCard);

			int curOperate = JsonValueAdapter(msg["curOperate"]);
			if (curOperate >= 0) {
				unsigned uViewChairID = g_globalTableConfig.GetViewChairID((curOperate));
				m_pGameTable->SetActiveStatus(uViewChairID, US_THINK);
			}

			unsigned uDUser = JsonValueAdapter(msg["bankerCh"]);
			unsigned uDUserView = g_globalTableConfig.GetViewChairID(uDUser);
			m_pGameTable->SetDPosition(uDUserView);
				

			if (m_pGameTable->getTableType() == SngTable)
			{
				TrackMgr->event("GameScene", "SNG场");
				//sng更新桌子名字 盲注
				g_globalTableConfig.m_strRoomName = msg["tableDesc"].asString();
				m_pGameTable->updateMatchBindTipsInTable() ;
				//更新盲注信息
				g_globalSngTableInfo.m_rank = JsonValueAdapter(msg["rank"]) ;
				m_pGameTable->updateSngTableRankTips() ;
				SendClientRequest(ClientRequest::kCallSngTableMangzhu) ;
				SendClientRequest(ClientRequest::kRequsetIsAutoTrust);
			}

			else if (m_pGameTable->getTableType() == MttTable)
			{
				g_globaMttGameInfo.m_hasJoinMatch = true ;
				g_globaMttGameInfo.m_mangzhuIndex = JsonValueAdapter(msg["blindLevel"]) ;
				g_globaMttGameInfo.m_inWaittingRoom = false ;
				TrackMgr->event("GameScene", "MTT场");
				g_globaMttGameInfo.m_isGameStart = true ;
				g_globalTableConfig.m_strRoomName = msg["tableDesc"].asString();
				m_pGameTable->updateMatchBindTipsInTable() ;
				m_pGameTable->reSetMttTable(true) ;
				SendClientRequest(ClientRequest::kRequsetIsAutoTrust);
				

			}
			else if (m_pGameTable->getTableType() == PrivateTable)
			{
				m_pGameTable->setPrivateGameControllButtonVisiable(true) ;
				m_pGameTable->updatePrivateTipsPosition() ;
			}


			auto funEndCollecting = [this](){
				GameData::getInstance()->setSoundOn(true) ;
			}; 
			this->runAction(Sequence::create(DelayTime::create(0.2f),CallFunc::create(funEndCollecting),nullptr)) ;
		}
		break;
	case GameRewardPool:
		{
			RewardListInfo* rewardlistinfo = nullptr;
			rewardlistinfo = g_globalTableConfig.m_RewardListInfo;
			rewardlistinfo->m_uUpdateCount = JsonValueAdapter(msg["reward"]);
			auto rankList = msg["rankList"];

			//if (rankList.isObject())
			{
				for(int i = 0; i < rankList.size(); i++)
				{
					rewardlistinfo[i].m_uRankID = JsonValueAdapter(rankList[i]["id"]);
					rewardlistinfo[i].m_dRankCount = JsonValueAdapter(rankList[i]["money"]);
					rewardlistinfo[i].m_strNickName = rankList[i]["nickname"].asString();
					rewardlistinfo[i].m_bIfShow = JsonValueAdapter(rankList[i]["type"]);
					rewardlistinfo[i].m_uID = JsonValueAdapter(rankList[i]["uid"]);
				}
			}


			m_pGameTable->SetValue(rewardlistinfo->m_uUpdateCount);
		}
		break;
	case GameMsgCmdS::GameScenceRestore: // 站起到坐下后的场景恢复
		{
			// 略坑，2人单挑场人数已经满了，旁观者坐下时可能返回这个消息，坐下失败或者成功要看lookOn字段。。。所以这里要屏蔽下
			bool bLookOn = JsonValueAdapter(msg["ifLookOn"]);
			if (bLookOn)
			{
				break;
			}
			m_pGameTable->SetLookingOn(true);
			m_pGameTable->CleanTable();
			m_pGameTable->CleanPlayer();
			g_globalTableConfig.ClearPlayer();
			g_globalTableConfig.SetTableMode(TablePlaying);			
			int uChairID = JsonValueAdapter(msg["cid"]);
			g_globalTableConfig.SetMyChairID(uChairID);

			std::string iconUrl = g_globalMyData.GetSkinID() == 0 ? g_globalMyData.GetIconURL() : 
				g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl;

			g_globalTableConfig.AddPlayer(uChairID, g_globalMyData.m_iUserId, g_globalTableConfig.m_dAutoBringMoney, 
				g_globalMyData.GetNickName(), iconUrl, 
				g_globalMyData.GetVoiceID(),g_globalMyData.GetSkinID(), 
				g_globalMyData.GetGender());
			m_pGameTable->SetUser(g_globalTableConfig.GetMyViewChairID(), g_globalMyData.GetNickName(), iconUrl, 
				g_globalTableConfig.m_dAutoBringMoney, g_globalMyData.m_iUserId,g_globalMyData.GetVoiceID(), g_globalMyData.GetSkinID(), g_globalMyData.GetVipLevel(), -1, 0,g_globalMyData.GetGender(), g_globalMyData.m_strVideoStreamUrl);

			//m_pGameTable->SetUser(g_globalTableConfig.GetMyViewChairID(), g_globalMyData.GetNickName(), msg["icon"].asString(), g_globalTableConfig.m_dAutoBringMoney, g_globalMyData.m_iUserId, g_globalMyData.GetSkinID(), g_globalMyData.GetVipLevel(), 0, 0,g_globalMyData.GetGender(), g_globalMyData.m_strVideoStreamUrl);
			bool bHasPlayer = JsonValueAdapter(msg["ifHave"]);
			auto& otherPlayer = msg["otherPerson"];
			for (int i = 0; i < otherPlayer.size(); i++)
			{
				auto& player = otherPlayer[i];
				uChairID = JsonValueAdapter(player["roleChair"]);
				unsigned uViewChairID = g_globalTableConfig.GetViewChairID(uChairID);

				int uBiaoJiId = -1;
				if (player["tip"].isObject() && !player["tip"].isNull())
				{
					uBiaoJiId = JsonValueAdapter(player["tip"]["tipId"]);
					//++uBiaoJiId;
				}


				g_globalTableConfig.AddPlayer(uChairID, JsonValueAdapter(player["id"]), JsonValueAdapter(player["getMoney"]), 
					player["name"].asString(), player["icon"].asString(),
					JsonValueAdapter(player["voiceId"]),JsonValueAdapter(player["skin_id"]),
					JsonValueAdapter(player["gender"]));

				unsigned uLadyId = JsonValueAdapter(player["is_guan"]);
				m_pGameTable->SetUser(uViewChairID,
					JsonValueAdapter(player["name"]), 
					player["icon"].asString(),
					JsonValueAdapter(player["getMoney"]), JsonValueAdapter(player["id"]), 
					JsonValueAdapter(player["voiceId"]),JsonValueAdapter(player["skin_id"]),
					JsonValueAdapter(player["vip_lv"]), uBiaoJiId, uLadyId, JsonValueAdapter(player["gender"]), JsonValueAdapter(player["streamUrl"]));
				int iPlayerStatus = JsonValueAdapter(player["roleStatus"]);
				m_pGameTable->SetUserShowName(uViewChairID, iPlayerStatus);
				m_pGameTable->SetHandCard(uViewChairID, iPlayerStatus);
				double dBeted = JsonValueAdapter(player["curBet"]);
				if (dBeted > 1)
				{
					m_pGameTable->SetUsrBetedJetton(uViewChairID, dBeted);
				}


			}

			auto arrPool = msg["poolArr"];
			std::vector<double> vecSidePot;
			double dAllPot = 0;
			for (int i = 0; i < arrPool.size(); i++)
			{
				if (i == 0)
				{
					dAllPot = JsonValueAdapter(arrPool[i]);
				}
				else
				{
					vecSidePot.push_back(JsonValueAdapter(arrPool[i]));
				}
			}
			m_pGameTable->SetAllPot(dAllPot);
			if (!vecSidePot.empty())
			{
				m_pGameTable->SetSidePot(vecSidePot);
			}			
			auto publicCard = msg["matchCard"];
			std::vector<unsigned char> vecPublicCard;
			for (int i = 0; i < publicCard.size(); i++)
			{
				vecPublicCard.push_back((unsigned)JsonValueAdapter(publicCard[i]));
			}
			m_pGameTable->SetPublicCard(vecPublicCard);

			int curOperate = JsonValueAdapter(msg["curOperate"]);
			if (curOperate >= 0) {
				unsigned uViewChairID = g_globalTableConfig.GetViewChairID(curOperate);
				m_pGameTable->SetActiveStatus(uViewChairID, US_THINK);
			}
		}
		break;

	case GameMsgCmdS::PlayerComeIn:
		{
			int iUserID = JsonValueAdapter(msg["id"]);
			bool bLookOn = JsonValueAdapter(msg["ifLookOn"]);
			int uChairID = JsonValueAdapter(msg["cid"]);
			unsigned uSkin = JsonValueAdapter(msg["skin_id"]);
			unsigned uVoice = JsonValueAdapter(msg["voiceId"]);
			unsigned uGender = JsonValueAdapter(msg["gender"]);



			std::string strVideoStreamUrl = msg["streamUrl"].asString();


			

			if (iUserID == g_globalMyData.m_iUserId)
			{
				
				g_globalMyData.m_strVideoStreamUrl = strVideoStreamUrl;
				g_globalTableConfig.SetTableMode(bLookOn ? TableLooking : TablePlaying);
				if (!bLookOn)
				{
					std::string iconUrl;
					if (g_globalMyData.GetSkinID() == 0) {
						iconUrl = g_globalMyData.GetIconURL();
					} else {
						iconUrl = g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl;
					}
					unsigned uLadyLevel = JsonValueAdapter(msg["is_guan"]);
					g_globalTableConfig.AddPlayer(uChairID, g_globalMyData.m_iUserId, g_globalTableConfig.m_dAutoBringMoney, g_globalMyData.GetNickName(), iconUrl,
						g_globalMyData.GetVoiceID(),g_globalMyData.GetSkinID(), 
						g_globalMyData.GetGender());
					//m_pGameTable->SetUser(g_globalTableConfig.GetMyViewChairID(), g_globalMyData.GetNickName(), iconUrl, g_globalTableConfig.m_dAutoBringMoney, g_globalMyData.m_iUserId, g_globalMyData.GetSkinID(), g_globalMyData.GetVipLevel(), 0, uLadyLevel,g_globalMyData.GetGender(), strVideoStreamUrl);

					m_pGameTable->UsrSitDown(g_globalTableConfig.GetMyViewChairID(), g_globalMyData.GetNickName(), iconUrl, g_globalTableConfig.m_dAutoBringMoney, g_globalMyData.m_iUserId, 
						g_globalMyData.GetVoiceID(),g_globalMyData.GetSkinID(), 
						g_globalMyData.GetVipLevel(), -1, uLadyLevel, g_globalMyData.GetGender(), strVideoStreamUrl,false,true);
				
					//m_pGameTable->SetUser(g_globalTableConfig.GetMyViewChairID(), g_globalMyData.GetNickName(), g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl, g_globalTableConfig.m_dAutoBringMoney, g_globalMyData.m_iUserId, g_globalMyData.GetSkinID(), g_globalMyData.GetVipLevel(), 0, uLadyLevel,g_globalMyData.GetGender(), strVideoStreamUrl);
					if (m_pGameTable->isVisible())
						PlayEffectSound("sound/act/sit.mp3");
				}
				else
				{
					unsigned uLookOnReason = JsonValueAdapter(msg["lookOnReason"]);

					if (uLookOnReason == 1) // 弹出钱不够，继续旁观或者充值的提示
					{
						//g_globalTableConfig.m_reason = 2 ;
						m_tipDialogVisible = false;
						int flags = checkVideoVisible();
						if (flags == 1) {
							m_tipDialogVisible = true;
							setVideoVisible(false);
						}
						PopTipWithBt2("德州币不够；您的德州币小于此房间的最小买入，去商城补充点德州币吧！","商城","继续旁观",[this](unsigned ubt)
						{
							if(ubt == 0)
							{
								Json::Value param;
								param["idx"] = 1;
								SendClientRequest(ClientRequest::kPopMall, param);
							}

							if (m_tipDialogVisible) {
								setVideoVisible(true);
								m_tipDialogVisible = false;
							}
						});
					}
					else if (uLookOnReason == 2) // 人数已满，继续旁观或者充值的提示
					{
						m_tipDialogVisible = false;
						int flags = checkVideoVisible();
						if (flags == 1) {
							m_tipDialogVisible = true;
							setVideoVisible(false);
						}
						PopTipWithBt2("很抱歉，该桌子人数已满，您可以旁观牌局等待到有空位时坐下！","返回","继续旁观",[](unsigned ubt)
						{
							if(ubt == 0)
							{
								SendClientRequest(ClientRequest::kLeaveTable);
							}

							if (m_tipDialogVisible) {
								setVideoVisible(true);
								m_tipDialogVisible = false;
							}
						});
					}
					else if (uLookOnReason == 0) // 不显示面板
					{

					}
					else if (uLookOnReason == 4) // 钱太多了 只能去高级场地
					{

						m_tipDialogVisible = false;
						int flags = checkVideoVisible();
						if (flags == 1) {
							m_tipDialogVisible = true;
							setVideoVisible(false);
						}

						g_globalTableConfig.m_reason = 1 ;

						if (getTableType() == NormalTable )
						{
							PopTipWithBt2("您的德州币超过80万，请前往更高级的场次！","返回","继续旁观",[](unsigned ubt)
							{
								if(ubt == 0)
								{
									SendClientRequest(ClientRequest::kLeaveTable);
								}

								if (m_tipDialogVisible) {
									setVideoVisible(true);
									m_tipDialogVisible = false;
								}
							});
						}

					}
				}
			}
			else // 其他玩家
			{
				std::string strTip;
				if (bLookOn)
				{
					strTip = "玩家 " + msg["nickname"].asString() + " 加入了游戏, 正在旁观";
				}
				else
				{
					strTip = "玩家 " + msg["nickname"].asString() + " 加入了游戏";
				}				 
				SendTableTip(strTip);
			}

			if (iUserID == g_globalMyData.m_iUserId)
			{			
				if (bLookOn)//旁观状态时uChairID可能为-1或5，这样会导致玩家视图错误
				{
					uChairID = 0;
					//uChairID = g_globalTableConfig.GetRandChairID();
				}
				g_globalTableConfig.SetMyChairID(uChairID);
				if (!bLookOn)
				{
					g_globalTableConfig.AddPlayer(uChairID, g_globalMyData.m_iUserId, g_globalTableConfig.m_dAutoBringMoney, g_globalMyData.GetNickName(),msg["icon"].asString(),uVoice, uSkin, g_globalMyData.GetGender());
					m_pGameTable->CaoDanDeSendReady();
				}
				g_globalTableConfig.SetTableMode(bLookOn ? TableLooking : TablePlaying);
			}
			else
			{
				if (!bLookOn)
				{
					g_globalTableConfig.AddPlayer(uChairID, iUserID, g_globalTableConfig.m_dAutoBringMoney, msg["nickname"].asString(), msg["icon"].asString(), 
						uVoice,uSkin, 
						uGender);

					unsigned uViewChairID = g_globalTableConfig.GetViewChairID(uChairID);
					int uBiaoJiId = -1;
					if (msg["tip"].isObject() && !msg["tip"].isNull())
					{
						uBiaoJiId = JsonValueAdapter(msg["tip"]["tipId"]);
						//++uBiaoJiId;
					}
					unsigned uLadyLevel = JsonValueAdapter(msg["is_guan"]);
					m_pGameTable->UsrSitDown(uViewChairID, JsonValueAdapter(msg["nickname"]), msg["icon"].asString(), g_globalTableConfig.m_dAutoBringMoney, iUserID,
						uVoice, uSkin, 
						JsonValueAdapter(msg["vip_lv"]), uBiaoJiId, uLadyLevel, uGender, strVideoStreamUrl,true,true);
					if (m_pGameTable->isVisible())
						PlayEffectSound("sound/act/sit.mp3");
				}
			}

			bool bMySelf = (iUserID == g_globalMyData.m_iUserId);
			if (bMySelf)
			{
				if (bLookOn)
				{
					SendClientRequest(ClientRequest::kCheckSitdown);
				}
			}
			else
			{
				if (g_globalTableConfig.GetTableMode() == TableLooking)
				{
					SendClientRequest(ClientRequest::kCheckSitdown);
				}
			}

			if (getTableType() == MttTable)
			{
				unsigned tableMoney = JsonValueAdapter(msg["tableMoney"]);
				unsigned uChairView = g_globalTableConfig.GetViewChairID(uChairID);
				m_pGameTable->SetUsrCash(uChairView,tableMoney);
			}
			m_pGameTable->updateCurSngPlayerCount() ;
		}
		break;

	case GameMsgCmdS::PlayerLeave:
		{
			int iUserID = JsonValueAdapter(msg["id"]);
			int iChair = JsonValueAdapter(msg["cid"]);
			bool bLookOn = JsonValueAdapter(msg["ifLookOn"]);
			if (iUserID != g_globalMyData.m_iUserId)
			{
				std::string strTip;
				if (bLookOn)
				{
					strTip = "玩家 " + msg["nickname"].asString() + " 正在旁观游戏";
				}
				else
				{
					strTip = "玩家 " + msg["nickname"].asString() + " 悄悄的离开了游戏";
				}

				SendTableTip(strTip);
			}
			if (m_pGameTable->getTableType() == SngTable)
			{
				if (!g_globalSngTableInfo.m_isGameStart)
				{
					g_globalTableConfig.RemovePlayerByID(iUserID);
				}
			}
			else
			{
				g_globalTableConfig.RemovePlayerByID(iUserID);
			}


			if (iUserID == g_globalMyData.m_iUserId)
			{
				if (bLookOn) // TODO: 如果是自己真的离开而非站起会受到消息吗？
				{
					m_pGameTable->UsrStandUp(g_globalTableConfig.GetMyViewChairID());
					//g_globalTableConfig.SetTableMode(TableLooking);
				}
				//sng场不检查德州币
				if(m_pGameTable->getTableType()==NormalTable)
				{
					/*	if (g_globalMyData.GetCash() <= g_globalTableConfig.m_enterMoney/10)
					{
					PopTipLayerWithBt1("您的金额不足,去商城补充筹码吧！", "确定");
					}*/
				}
				if(m_pGameTable->getTableType()==MttTable)
				{
					m_pGameTable->updateMttButtonShow() ;
				}
			
			}
			else
			{ 
				if (iChair >= 0)
				{
					unsigned uViewChairID = g_globalTableConfig.GetViewChairID(iChair);
					m_pGameTable->UsrStandUp(uViewChairID);
				}
			}
			//g_globalTableConfig.RemovePlayerByID(iUserID);

			bool bMySelf = (iUserID == g_globalMyData.m_iUserId);
			if (bMySelf)
			{
				g_globalTableConfig.m_reason=0 ;
			}
			else
			{
				if (g_globalTableConfig.GetTableMode() == TableLooking)
				{
					SendClientRequest(ClientRequest::kCheckSitdown);
				}
			}
			m_pGameTable->updateCurSngPlayerCount() ;
		}
		break;

	case GameMsgCmdS::UpdateWaitPlayerNum:
		{
			unsigned uWaitNum = JsonValueAdapter(msg["playerNum"]);
			m_pGameTable->UpdateWaitPlayerNum(uWaitNum);
		}
		break;

	case GameMsgCmdS::UpdateMoney:
		{
			unsigned uChairID = JsonValueAdapter(msg["cid"]);
			unsigned uViewChairID = g_globalTableConfig.GetViewChairID(uChairID);
			double dSupply = JsonValueAdapter(msg["addMoney"]);
			double dRest = JsonValueAdapter(msg["acountMoney"]);
            int userID = JsonValueAdapter(msg["openid"]);
			if(m_pGameTable != nullptr /*&& g_globalMyData.m_iUserId == userID*/)//MTT场Allin后赢家的金币显示为0(其他玩家观察)
			{
				m_pGameTable->SetUsrCash(uViewChairID, dSupply);
			}
			if (g_globalMyData.m_iUserId == userID)
			{
				g_globalMyData.SetCash(dSupply + dRest);
			}
			// g_globalGameSerialization.AddAtomicAction(AtomicAction::UpdateCash);
		}
		break;
	case GameMsgCmdS::UpdateMoneyFaile:
		{
			SendTableTip("亲~  您的筹码添加失败！");
		}
		break;
	case GameMsgCmdS::AutoSupplyMoneyTip:
		{
			double dSupply = JsonValueAdapter(msg["addMoney"]);
			double dRest = JsonValueAdapter(msg["acountMoney"]);
			char szAutoSupplyTip[128] = {0};
			sprintf(szAutoSupplyTip, "系统帮您自动带入%s筹码，当前德州币余额%s", FormatCash3(dSupply).c_str(), FormatCash3(dRest).c_str());
			Json::Value tip;
			tip["strTip"] = szAutoSupplyTip;
			SendClientRequest(ClientRequest::kSendTableTip, tip);

			g_globalMyData.SetCash(dSupply + dRest);
			// g_globalGameSerialization.AddAtomicAction(AtomicAction::UpdateCash);
		}
		break;


	case GameMsgCmdS::GameStart:
		{
			//m_pGameTable->CleanTableAfterEnd() ;
			m_pGameTable->CleanTable();
			auto players = msg["gameUser"];
			std::deque<unsigned> deqPlayers;
			std::vector<unsigned> vecDispatchPlayers;
			for (int i = 0; i < players.size(); i++)
			{
				unsigned uChair = JsonValueAdapter(players[i]);
				unsigned uViewChair = g_globalTableConfig.GetViewChairID(uChair);
				deqPlayers.push_back(uViewChair); 
			}
			std::reverse(deqPlayers.begin(), deqPlayers.end());// 后端给的椅子序号是递减的，比如第一个玩家椅子为3，下一个说话的玩家椅子为2，导致我们前端发牌顺序为逆时针的。所以这里要倒序一下
			unsigned uDUser = JsonValueAdapter(msg["bankerCh"]);
			unsigned uDUserView = g_globalTableConfig.GetViewChairID(uDUser);
			if (uDUserView == g_globalTableConfig.GetMyViewChairID())
			{
				g_globalGameSerialization.AddAtomicAction(AtomicAction::TaskButton);
			}

			while (deqPlayers[0] != uDUserView)
			{
				unsigned u = deqPlayers.front();
				deqPlayers.push_back(u);
				deqPlayers.pop_front();
			}
			for (int i = 0; i < deqPlayers.size(); i++)
			{
				vecDispatchPlayers.push_back(deqPlayers[i]);
			}
			// CAODANDE发送小盲注
			unsigned uLittleBlindCh = JsonValueAdapter(msg["smallBlindPos"]);
			if (g_globalTableConfig.GetMyChairID() == uLittleBlindCh)
			{
				m_pGameTable->CaoDanDeSendLittleBlind();
			}

			// CAODANDE 一个设计错误导致的补丁 所有的底池数（这里如果）
			auto arrPool = msg["allPool"];
			std::vector<double> vecSidePot;
			double dAllPot = 0;
			for (int i = 0; i < arrPool.size(); i++)
			{
				if (i == 0)
				{
					dAllPot = JsonValueAdapter(arrPool[i]);
				}
				else
				{
					vecSidePot.push_back(JsonValueAdapter(arrPool[i]));
				}
			}
			m_pGameTable->SetAllPot(dAllPot);
			if (!vecSidePot.empty())
			{
				m_pGameTable->SetSidePot(vecSidePot);
			}	
			


			m_pGameTable->SetDPosition(uDUserView);
			unsigned char cbMyHandCard[2];
			auto card = msg["cardData"];
			cbMyHandCard[0] = static_cast<unsigned>(JsonValueAdapter(card[0u]));
			cbMyHandCard[1] = static_cast<unsigned>(JsonValueAdapter(card[1u]));
			m_pGameTable->DispatchHandCard(vecDispatchPlayers, cbMyHandCard);

			if (m_pGameTable->getTableType()== SngTable )
			{

				g_globalSngTableInfo.m_roudIdex++ ;
				g_globalSngTableInfo.m_isGameStart = true ;
				//sng更新桌子名字 盲注
				g_globalTableConfig.m_strRoomName = msg["tableDesc"].asString();
				m_pGameTable->updateMatchBindTipsInTable() ;

				//更新盲注索引

				g_globalSngTableInfo.m_mangzhuIndex = JsonValueAdapter(msg["blindIndex"]) ;

				//每局更新大小盲
				unsigned dBigBlind = JsonValueAdapter(msg["bigBlind"]) ;
				g_globalTableConfig.m_dLittleBlind = dBigBlind / 2;
				g_globalTableConfig.m_dBigBlind = dBigBlind;

			}
			if (m_pGameTable->getTableType()== MttTable)
			{

				g_globaMttGameInfo.m_roudIdex++ ;
				g_globaMttGameInfo.m_isGameStart = true ;
				//sng更新桌子名字 盲注
				g_globalTableConfig.m_strRoomName = msg["tableDesc"].asString();
				m_pGameTable->updateMatchBindTipsInTable() ;
				//更新盲注索引
				g_globaMttGameInfo.m_mangzhuIndex = JsonValueAdapter(msg["blindLevel"]) ;
				//每局更新大小盲
				unsigned dBigBlind = JsonValueAdapter(msg["bigBlind"]) ;
				g_globalTableConfig.m_dLittleBlind = dBigBlind / 2;
				g_globalTableConfig.m_dBigBlind = dBigBlind;

				m_pGameTable->updateMttButtonShow() ;
			}

			if (g_globalTableConfig.GetTableMode() == TablePlaying)
			{
				g_globalGameSerialization.AddAtomicAction(AtomicAction::GameBegin);
			}

			

		}
		break;

	case GameMsgCmdS::FoldCardResult:
		{
			unsigned uChair = JsonValueAdapter(msg["giveUpCh"]);
			if (g_globalTableConfig.GetTableMode() == TablePlaying && uChair == g_globalTableConfig.GetMyChairID())
			{
				g_globalGameSerialization.AddAtomicAction(AtomicAction::GameEnd);
			}
			unsigned uChairView = g_globalTableConfig.GetViewChairID(uChair);
			m_pGameTable->UsrFoldCard(uChairView);

			int iCurThinkingUsr = m_pGameTable->GetCurThinkingUsr();
			if (iCurThinkingUsr == -1)
			{
				int iCurChair = JsonValueAdapter(msg["nextChairId"]);
				if (iCurChair >= 0 && iCurChair < GAME_PLAYER)
				{
					unsigned uCurChairView = g_globalTableConfig.GetViewChairID(iCurChair);
					double dBanlance = JsonValueAdapter(msg["mBalanceScore"]);

					bool bHasDispatchPublicCard = static_cast<unsigned>(JsonValueAdapter(msg["sendCardTurnCount"])) > 0;
					if (uCurChairView == g_globalTableConfig.GetMyViewChairID()) // 轮到我说话
					{
						m_pGameTable->SetSelfActionInfo(0,dBanlance, JsonValueAdapter(msg["curMinAdd"]), JsonValueAdapter(msg["curMaxAdd"]), bHasDispatchPublicCard);
					}
					else
					{
						m_pGameTable->WaitUsrAction(uCurChairView);
						m_pGameTable->UpdateSelfMinCall(dBanlance);
					}
				}

			}
		}
		break;
	case GameMsgCmdS::BrightBrandResult:
		{
			unsigned char carData[2];
			unsigned uChair = JsonValueAdapter(msg["ch"]);
			auto CardData = msg["cardData"];
			carData[0] = static_cast<unsigned>(JsonValueAdapter(CardData[0u]));
			carData[1] = static_cast<unsigned>(JsonValueAdapter(CardData[1u]));
			unsigned uChairView = g_globalTableConfig.GetViewChairID(uChair);
			m_pGameTable->UsrShowCard(uChairView,carData);
		}
		break;
	case GameMsgCmdS::BetResult:
		{
			unsigned uChair = JsonValueAdapter(msg["betCh"]);
			unsigned uChairView = g_globalTableConfig.GetViewChairID(uChair);
			double dBetValue = JsonValueAdapter(msg["betJettonNum"]);
			unsigned uBetType = JsonValueAdapter(msg["betType"]);
			unsigned uBlindType = JsonValueAdapter(msg["mangTag"]);
			m_pGameTable->SetUsrCash(uChairView, JsonValueAdapter(msg["addMoney"]));

			if (uBlindType == 1 || uBlindType == 2)
			{
				m_pGameTable->UsrPlaceJetton(uChairView, dBetValue);
				if (uBlindType == 1)
				{
					int iCurChair = JsonValueAdapter(msg["nextChairId"]);
					if (iCurChair >= 0 && iCurChair < GAME_PLAYER)
					{
						unsigned uCurChairView = g_globalTableConfig.GetViewChairID(iCurChair);
						if (uCurChairView == g_globalTableConfig.GetMyViewChairID())
						{
							m_pGameTable->CaoDanDeSendBigBlind();
						}
					}
				}
			}
			else
			{
				if (uBetType == 0 || uBetType == 2)
				{
					m_pGameTable->UsrRaise(uChairView, dBetValue);
					if (uChairView == g_globalTableConfig.GetMyViewChairID())
					{
						g_globalGameSerialization.AddAtomicAction(AtomicAction::TaskRaise);
					}
				}
				else if (uBetType == 1)
				{
					m_pGameTable->UsrCall(uChairView, dBetValue);
				}
				else if (uBetType == 3)
				{
					g_globalTableConfig.m_uSound = 1;
					m_pGameTable->UsrAllIn(uChairView, dBetValue);
				}
				else if (uBetType == 4)
				{
					m_pGameTable->UsrCheck(uChairView);
				}
				else if (uBetType == 5)
				{
					// m_pGameTable->UsrFoldCard(uChairView);
				}
			}

			auto arrPool = msg["allPool"];
			std::vector<double> vecSidePot;
			double dAllPot = 0;
			for (int i = 0; i < arrPool.size(); i++)
			{
				if (i == 0)
				{
					dAllPot = JsonValueAdapter(arrPool[i]);
				}
				else
				{
					vecSidePot.push_back(JsonValueAdapter(arrPool[i]));
				}
			}

			bool bTurnOver = JsonValueAdapter(msg["ifTurnOver"]);
			if (bTurnOver)
			{
				m_pGameTable->CollectUsrBetToPot(vecSidePot);

				auto publicCards = msg["cardData"];
				std::vector<unsigned char> vecPublicCard;
				for (int i = 0; i < publicCards.size(); i++)
				{
					vecPublicCard.push_back(static_cast<unsigned>(JsonValueAdapter(publicCards[i])));
				}
				m_pGameTable->DispatchPublicCard(vecPublicCard);				
			}
			else // 一局尚未平衡
			{
			}
			if (uBlindType != 1)
			{
				int iCurChair = JsonValueAdapter(msg["nextChairId"]);
				if (iCurChair >= 0 && iCurChair < GAME_PLAYER)
				{
					unsigned uCurChairView = g_globalTableConfig.GetViewChairID(iCurChair);
					double dBanlance = JsonValueAdapter(msg["mBalanceScore"]);

					bool bHasDispatchPublicCard = static_cast<unsigned>(JsonValueAdapter(msg["sendCardTurnCount"])) > 0;
					m_pGameTable->UpdateSelfMinCall(dBanlance);





					if (uCurChairView == g_globalTableConfig.GetMyViewChairID()) // 轮到我说话
					{
						m_pGameTable->SetSelfActionInfo(uBetType,dBanlance, JsonValueAdapter(msg["curMinAdd"]), JsonValueAdapter(msg["curMaxAdd"]), bHasDispatchPublicCard);
					}
					else
					{
						m_pGameTable->WaitUsrAction(uCurChairView);
					}
				}
			}
		}
		break;

	case GameMsgCmdS::GameEnd:
		{
			// CAODANDE 由于协议设计问题，游戏结束前的下注消息可能nextchairid为我自己，这个时候需要强制取消自动弃牌的功能
			m_pGameTable->ShowMeHasActioned();
			// CAODANDE end

			bool bNeedOpenCard = JsonValueAdapter(msg["ifNormalEnd"]);
			if (bNeedOpenCard)
			{
				// CAODANDE 最后一次下注信息以及收集筹码到边池 这个放在游戏结束消息里显然不太好，待协议优化吧
				unsigned uChair = JsonValueAdapter(msg["betCh"]);
				unsigned uChairView = g_globalTableConfig.GetViewChairID(uChair);
				double dBetValue = JsonValueAdapter(msg["betJettonNum"]);
				unsigned uBetType = JsonValueAdapter(msg["betType"]);
				g_globalTableConfig.m_uSound = JsonValueAdapter(msg["sound"]);

				if (uBetType == 0 || uBetType == 2)
				{
					m_pGameTable->UsrRaise(uChairView, dBetValue);
				}
				else if (uBetType == 1)
				{
					m_pGameTable->UsrCall(uChairView, dBetValue);
				}
				else if (uBetType == 3)
				{
					//allin后底池显示不对
					//m_pGameTable->UsrAllIn(uChairView, dBetValue);
				}
				else if (uBetType == 4)
				{
					m_pGameTable->UsrCheck(uChairView);
				}

				auto arrPool = msg["allPool"];
				std::vector<double> vecSidePot;
				double dAllPot = 0;
				for (int i = 0; i < arrPool.size(); i++)
				{
					if (i == 0)
					{
						dAllPot = JsonValueAdapter(arrPool[i]);
					}
					else
					{
						vecSidePot.push_back(JsonValueAdapter(arrPool[i]));
					}
				}
				m_pGameTable->CollectUsrBetToPot(vecSidePot);
				// CAODANDE end

				//m_pGameTable->ActivateUserStatus(US_NORMAL, US_NORMAL | US_WAITNEXTGAME);

				auto opencardChair = msg["showCardUser"];
				auto openCard = msg["cardData"];
				std::vector<unsigned> vecOpenChairs;
				std::vector<unsigned> vecOpenCardData;
				for (unsigned i = 0; i < opencardChair.size(); i++)
				{
					unsigned uChair = JsonValueAdapter(opencardChair[i]);
					unsigned uChairV = g_globalTableConfig.GetViewChairID(uChair);
					vecOpenChairs.push_back(uChairV);
					vecOpenCardData.push_back(JsonValueAdapter(openCard[i*2]));
					vecOpenCardData.push_back(JsonValueAdapter(openCard[i*2+1]));
				}		
				m_pGameTable->OpenUsrCard(vecOpenChairs, vecOpenCardData);
			}
			else
			{
				//m_pGameTable->ActivateUserStatus(US_NORMAL, US_FOLD | US_WAITNEXTGAME);
				// CAODANDE begin
				auto arrPool = msg["allPool"];
				std::vector<double> vecSidePot;
				double dAllPot = 0;
				for (int i = 0; i < arrPool.size(); i++)
				{
					if (i == 0)
					{
						dAllPot = JsonValueAdapter(arrPool[i]);
					}
					else
					{
						vecSidePot.push_back(JsonValueAdapter(arrPool[i]));
					}
				}
				// CAODANDE end
				m_pGameTable->CollectUsrBetToPot(vecSidePot);
			}
			double dPotsMoney[GAME_PLAYER];
			for (auto& item : dPotsMoney)
			{
				item = 0;
			}
			auto allPool = msg["allPool"];
			if (allPool.size() > 0) // 这里有可能发过来一个空的allPool，导致下面的代码会溢出崩溃
			{
				unsigned uPots = allPool.size()-1;
				for (unsigned i = 0; i < uPots; i++)
				{
					dPotsMoney[i] = JsonValueAdapter(allPool[i+1]);
				}
			}


			UsrWinInfo usrWinInfos[GAME_PLAYER];
			memset(usrWinInfos, 0, sizeof(usrWinInfos));
			auto usrWinPools = msg["poolsMoney"];
			for (unsigned i = 0; i < usrWinPools.size(); i++)
			{
				auto potWinInfo = usrWinPools[i];
				unsigned uPot = JsonValueAdapter(potWinInfo["id"]);
				auto potWinners = potWinInfo["gameUserCHairs"];
				unsigned uPotWinners = potWinners.size();
				double dWinValue = dPotsMoney[uPot] / uPotWinners;
				for (int j = 0; j < uPotWinners; j++)
				{
					unsigned uChair = JsonValueAdapter(potWinners[j]);
					usrWinInfos[uChair].uChair = uChair;
					usrWinInfos[uChair].uWinPotsIdx[usrWinInfos[uChair].uWinPots] = uPot;
					usrWinInfos[uChair].dPotsWinValue[usrWinInfos[uChair].uWinPots] = dWinValue;
					++usrWinInfos[uChair].uWinPots;
				}
			}

			auto usrWinLose = msg["showWinLoseMoney"];
			std::vector<UsrWinInfo> vecUsrWinInfo;
			for (int i = 0; i < GAME_PLAYER; i++)
			{
				usrWinInfos[i].dAllWinValue = JsonValueAdapter(usrWinLose[i]);
				if (usrWinInfos[i].uWinPots > 0) // #2192
				{
					usrWinInfos[i].uChair = g_globalTableConfig.GetViewChairID(usrWinInfos[i].uChair);
					usrWinInfos[i].bIsWinner = false;
					vecUsrWinInfo.push_back(usrWinInfos[i]);
				}
			}

			unsigned winType = JsonValueAdapter(msg["winMaxType"]);
			std::vector<unsigned> vecWinMaxChairs;
			auto winChairs = msg["winMaxChairs"];
			for (int i = 0; i < winChairs.size(); i++)
			{
				auto ch = JsonValueAdapter(winChairs[i]);
				vecWinMaxChairs.push_back(g_globalTableConfig.GetViewChairID(ch));
				for (auto& itemUserWinInfo : vecUsrWinInfo)
				{
					if (itemUserWinInfo.uChair == g_globalTableConfig.GetViewChairID(ch))
					{
						itemUserWinInfo.bIsWinner = true;
					}
				}
			}
			std::sort(vecUsrWinInfo.begin(), vecUsrWinInfo.end(), comp);
			m_pGameTable->ShowUsrWinInfo(winType, vecWinMaxChairs, bNeedOpenCard, vecUsrWinInfo);

			if (g_globalTableConfig.GetTableMode() == TablePlaying)
			{
				g_globalGameSerialization.AddAtomicAction(AtomicAction::GameEnd);
			}
			m_pGameTable->ShowGameHasEnd(bNeedOpenCard);
			m_pGameTable->ActivateUserStatus(US_NORMAL, US_NORMAL | US_CHECK | US_CALL | US_RAISE | US_ALLIN | US_FOLD | US_OPENCARD | US_SHOWCARD | US_WINTYPE | US_WAITNEXTGAME);
		}
		break;

	case RewardResult:
		{
			unsigned uSuccess = JsonValueAdapter(msg["sendResult"]);
			if (uSuccess == 1)
			{
				unsigned uChair = JsonValueAdapter(msg["rewardUser"]);
				unsigned uChairV = g_globalTableConfig.GetViewChairID(uChair);
				m_pGameTable->UsrAward(uChairV, g_globalTableConfig.m_dRewardMoney);

			}
			else
			{
				//sjf 2015-6-12 后台金钱不足 打赏失败提示
				SendTableTip("德州币不足，无法打赏！") ;
			}
		}
		break;

	case GetHallInfoResult:
		{
			unsigned uAreaType = JsonValueAdapter(msg["type"]);
			unsigned uTableMaxPlayer = JsonValueAdapter(msg["num"]);

			TableAreaInfo *pAreaInfo = nullptr;
			if (uAreaType == 1)
			{
				pAreaInfo = g_globalTableAreaInfo.m_areaBSQJ;
			}
			else if (uAreaType == 2)
			{
				if (uTableMaxPlayer == 6)
				{
					pAreaInfo = g_globalTableAreaInfo.m_areaZCJJ6;
				}
				else if (uTableMaxPlayer == 5)
				{
					pAreaInfo = g_globalTableAreaInfo.m_areaZCJJ9;
				}
			}
			if (uAreaType >= 3)
			{
				g_globalTableAreaInfo.m_areaDZDH.clear();
				auto &areaInfos = msg["tableConf"];
				for (unsigned i = 0; i < areaInfos.size(); i++)
				{
					DeZhouMagnateInfo magNateInfo;
					magNateInfo.m_uDeskID = JsonValueAdapter(areaInfos[i]["areaId"]);
					magNateInfo.m_uRoomName = JsonValueAdapter(areaInfos[i]["maxCarry"]);
					magNateInfo.m_sRoomVipName = areaInfos[i]["roomName"].asString();
					magNateInfo.m_uMustBet = JsonValueAdapter(areaInfos[i]["must_bet"]);
					magNateInfo.m_uCurPlayer = JsonValueAdapter(areaInfos[i]["playerNum"]);
					magNateInfo.m_uMaxPlayer = JsonValueAdapter(areaInfos[i]["numLimit"]);
					magNateInfo.m_uOnlookNum = JsonValueAdapter(areaInfos[i]["lookCount"]);
					magNateInfo.m_bIsCamera = JsonValueAdapter(areaInfos[i]["ifVd"]);
					magNateInfo.m_bIsVip = JsonValueAdapter(areaInfos[i]["ifVip"]);
					magNateInfo.m_bIsChallenge = JsonValueAdapter(areaInfos[i]["ifDantiao"]);
					magNateInfo.m_bIsMustPour = JsonValueAdapter(areaInfos[i]["ifAnti"]);


					auto &info = areaInfos[i]["userData"];
					for (unsigned j=0; j < info.size(); j++)
					{
						magNateInfo.m_strIconURL[j] = areaInfos[i]["userData"][j]["ico"].asString();
						magNateInfo.m_strPlayerName[j] = areaInfos[i]["userData"][j]["nickName"].asString();
						magNateInfo.m_UserID = JsonValueAdapter(areaInfos[i]["userData"][j]["id"]);
					}
					g_globalTableAreaInfo.m_areaDZDH.push_back(magNateInfo);
				}
			}
			else
			{
				auto &areaInfos = msg["tableConf"];
				for (unsigned i = 0; i < areaInfos.size(); i++)
				{
					pAreaInfo[i].m_uAreaID = JsonValueAdapter(areaInfos[i]["areaId"]);
					pAreaInfo[i].m_dLittleBlind = JsonValueAdapter(areaInfos[i]["smallBlind"]);
					pAreaInfo[i].m_dBigBlind = JsonValueAdapter(areaInfos[i]["bigBlind"]);
					pAreaInfo[i].m_dMaxCarry = JsonValueAdapter(areaInfos[i]["maxCarry"]);
					pAreaInfo[i].m_uPlayers = JsonValueAdapter(areaInfos[i]["playerNum"]);
				}
			}

			if (nullptr != m_pGameHall2)
				m_pGameHall2->UpdateView();
		}
		break;
	case GetMagnateInfoResult:
		{
			g_globalTableAreaInfo.m_areaDZPage.clear();
			unsigned uAreaType = JsonValueAdapter(msg["type"]);
			unsigned uTableMaxPlayer = JsonValueAdapter(msg["num"]);
			auto &areaInfos = msg["tableConf"];
			DeZhouTableInfo deZhouTable;
			for (unsigned i = 0; i < areaInfos.size(); i++)
			{
				auto &info = areaInfos[i];
				deZhouTable.m_uAreaId = JsonValueAdapter(areaInfos[i]["value"]["areaId"]);
				deZhouTable.m_uBlindId = JsonValueAdapter(areaInfos[i]["value"]["blindId"]);
				deZhouTable.m_dBlindMoney = JsonValueAdapter(areaInfos[i]["value"]["blindMoney"]);
				deZhouTable.m_strBlindName = info["value"]["blindName"].asString();
				g_globalTableAreaInfo.m_areaDZPage.push_back(deZhouTable);
			}
			m_pGameHall2->UpdateDeZhouPage();
		}
		break;
	case GetVipOpenRoomResult:
		{

		
			DefineRoomInfo pDefineOpenRoomInfo;
			g_globalVipOpenRoom.m_UserDefineRoom.clear();
			g_globalVipOpenRoom.m_ClubDefineRoom.clear();
			auto &areaDefineInfos = msg["privateroominfo"];
			for (unsigned i = 0; i < areaDefineInfos.size(); i++)
			{
				auto &normalInfos = areaDefineInfos[i];
				pDefineOpenRoomInfo.m_uType = JsonValueAdapter(normalInfos["kind"]);
				pDefineOpenRoomInfo.m_uType-=1 ;
				pDefineOpenRoomInfo.m_uAreaID = JsonValueAdapter(normalInfos["roomId"]);
				pDefineOpenRoomInfo.m_strRoomName = normalInfos["roomName"].asString();
				pDefineOpenRoomInfo.m_strOwnerName = normalInfos["ownerName"].asCString();
				pDefineOpenRoomInfo.m_strRoomPwd = normalInfos["password"].asString();
				pDefineOpenRoomInfo.m_uPlayers = JsonValueAdapter(normalInfos["playerNum"]);
				pDefineOpenRoomInfo.m_uCurPlayers = JsonValueAdapter(normalInfos["inChairPlayer"]);
				pDefineOpenRoomInfo.m_uMaxPlayers =  JsonValueAdapter(normalInfos["numLimit"]);
				pDefineOpenRoomInfo.m_uLookOnNum = JsonValueAdapter(normalInfos["lookOnPlayer"]);
				pDefineOpenRoomInfo.m_minCollect = JsonValueAdapter(normalInfos["startFund"]);
				pDefineOpenRoomInfo.m_maxCollect = JsonValueAdapter(normalInfos["endFund"]);;
				double rate= (JsonValueAdapter(normalInfos["fundRate"])) ;
				pDefineOpenRoomInfo.m_fundP = rate*100 ;
				pDefineOpenRoomInfo.m_dMinCarry = JsonValueAdapter(normalInfos["minCarry"]);;
				pDefineOpenRoomInfo.m_dMaxCarry = JsonValueAdapter(normalInfos["maxCarry"]);
				pDefineOpenRoomInfo.m_dLittleBlind = JsonValueAdapter(normalInfos["smallBlind"]);
				pDefineOpenRoomInfo.m_dBigBlind = JsonValueAdapter(normalInfos["bigBlind"]);
				pDefineOpenRoomInfo.m_dMustBet = JsonValueAdapter(normalInfos["mustBet"]);
				pDefineOpenRoomInfo.m_dCoverCharge = JsonValueAdapter(normalInfos["coverCharge"]);
				pDefineOpenRoomInfo.m_dEnterMoney = JsonValueAdapter(normalInfos["entermoney"]);
				pDefineOpenRoomInfo.m_state = JsonValueAdapter(normalInfos["roomState"]);
				pDefineOpenRoomInfo.m_isMine = JsonValueAdapter(normalInfos["isOwner"]);
				g_globalVipOpenRoom.m_UserDefineRoom.push_back(pDefineOpenRoomInfo);
			}


			auto &areaClubDefineInfos = msg["clubroominfo"];
			for (unsigned i = 0; i < areaClubDefineInfos.size(); i++)
			{
				auto &normalInfos = areaClubDefineInfos[i];
				pDefineOpenRoomInfo.m_uType = JsonValueAdapter(normalInfos["kind"]);
				pDefineOpenRoomInfo.m_uType-=1 ;
				pDefineOpenRoomInfo.m_uAreaID = JsonValueAdapter(normalInfos["roomId"]);
				pDefineOpenRoomInfo.m_strRoomName = normalInfos["roomName"].asString();
				pDefineOpenRoomInfo.m_strOwnerName = normalInfos["ownerName"].asCString();
				pDefineOpenRoomInfo.m_strRoomPwd = normalInfos["password"].asString();
				pDefineOpenRoomInfo.m_uPlayers = JsonValueAdapter(normalInfos["playerNum"]);
				pDefineOpenRoomInfo.m_uCurPlayers = JsonValueAdapter(normalInfos["inChairPlayer"]);
				pDefineOpenRoomInfo.m_uMaxPlayers =  JsonValueAdapter(normalInfos["numLimit"]);
				pDefineOpenRoomInfo.m_uLookOnNum = JsonValueAdapter(normalInfos["lookOnPlayer"]);
				pDefineOpenRoomInfo.m_minCollect = JsonValueAdapter(normalInfos["startFund"]);
				pDefineOpenRoomInfo.m_maxCollect = JsonValueAdapter(normalInfos["endFund"]);;
				double rate= (JsonValueAdapter(normalInfos["fundRate"])) ;
				pDefineOpenRoomInfo.m_fundP = rate*100 ;
				pDefineOpenRoomInfo.m_dMinCarry = JsonValueAdapter(normalInfos["minCarry"]);;
				pDefineOpenRoomInfo.m_dMaxCarry = JsonValueAdapter(normalInfos["maxCarry"]);
				pDefineOpenRoomInfo.m_dLittleBlind = JsonValueAdapter(normalInfos["smallBlind"]);
				pDefineOpenRoomInfo.m_dBigBlind = JsonValueAdapter(normalInfos["bigBlind"]);
				pDefineOpenRoomInfo.m_dMustBet = JsonValueAdapter(normalInfos["mustBet"]);
				pDefineOpenRoomInfo.m_dCoverCharge = JsonValueAdapter(normalInfos["coverCharge"]);
				pDefineOpenRoomInfo.m_dEnterMoney = JsonValueAdapter(normalInfos["entermoney"]);
				pDefineOpenRoomInfo.m_state = JsonValueAdapter(normalInfos["roomState"]);
				pDefineOpenRoomInfo.m_isMine = JsonValueAdapter(normalInfos["isOwner"]);
				g_globalVipOpenRoom.m_ClubDefineRoom.push_back(pDefineOpenRoomInfo);
			}


			if (m_pVipPrivateNormal!=nullptr)
			{
				m_pVipPrivateNormal->showRoomView(true);
			}
			
		}
		break;
	case GetDefineVipOpenResult:
		{
			bool result = JsonValueAdapter(msg["result"]);
			if (result)
			{
				DefineRoomInfo pDefineOpenRoomInfo, pDefineRoomInfo;
				pDefineOpenRoomInfo = g_globalVipOpenRoom.m_areaCreateRoom;
				auto &normalInfos = msg["data"];
				if (!normalInfos.empty())
				{


					pDefineOpenRoomInfo.m_uType = JsonValueAdapter(normalInfos["kind"]);
					pDefineOpenRoomInfo.m_uType-=1 ;
					pDefineOpenRoomInfo.m_uAreaID = JsonValueAdapter(normalInfos["roomId"]);
					pDefineOpenRoomInfo.m_strRoomName = normalInfos["roomName"].asString();
					pDefineOpenRoomInfo.m_strOwnerName = normalInfos["ownerName"].asCString();
					pDefineOpenRoomInfo.m_strRoomPwd = normalInfos["password"].asString();
					pDefineOpenRoomInfo.m_uPlayers = JsonValueAdapter(normalInfos["playerNum"]);
					pDefineOpenRoomInfo.m_uCurPlayers = JsonValueAdapter(normalInfos["inChairPlayer"]);
					pDefineOpenRoomInfo.m_uMaxPlayers =  JsonValueAdapter(normalInfos["numLimit"]);
					pDefineOpenRoomInfo.m_uLookOnNum = JsonValueAdapter(normalInfos["lookOnPlayer"]);
					pDefineOpenRoomInfo.m_minCollect = JsonValueAdapter(normalInfos["startFund"]);
					pDefineOpenRoomInfo.m_maxCollect = JsonValueAdapter(normalInfos["endFund"]);;
					double rate= (JsonValueAdapter(normalInfos["fundRate"])) ;
					pDefineOpenRoomInfo.m_fundP = rate*100 ;
					pDefineOpenRoomInfo.m_dMinCarry = JsonValueAdapter(normalInfos["minCarry"]);;
					pDefineOpenRoomInfo.m_dMaxCarry = JsonValueAdapter(normalInfos["maxCarry"]);
					pDefineOpenRoomInfo.m_dLittleBlind = JsonValueAdapter(normalInfos["smallBlind"]);
					pDefineOpenRoomInfo.m_dBigBlind = JsonValueAdapter(normalInfos["bigBlind"]);
					pDefineOpenRoomInfo.m_dMustBet = JsonValueAdapter(normalInfos["mustBet"]);
					pDefineOpenRoomInfo.m_dCoverCharge = JsonValueAdapter(normalInfos["coverCharge"]);
					pDefineOpenRoomInfo.m_dEnterMoney = JsonValueAdapter(normalInfos["entermoney"]);
					pDefineOpenRoomInfo.m_state = JsonValueAdapter(normalInfos["roomState"]);
					pDefineOpenRoomInfo.m_isMine = JsonValueAdapter(normalInfos["isOwner"]);


					if (pDefineOpenRoomInfo.m_uType ==0)
					{
						g_globalVipOpenRoom.m_UserDefineRoom.push_back(pDefineOpenRoomInfo);
					}
					if (pDefineOpenRoomInfo.m_uType ==1)
					{
						g_globalVipOpenRoom.m_ClubDefineRoom.push_back(pDefineOpenRoomInfo);
					}
					

					if (m_pVipPrivateNormal!=nullptr)
					{
						m_pVipPrivateNormal->showRoomView(true) ;

						PopTipLayerWithBt1("自定义开房成功", "确定");
						//m_pVipPrivateNormal->UpdateView() ;
					}

				}	
				else
				{
					PopTipLayerWithBt1("自定义开房失败", "确定");
				}
			}
			else
			{
				PopTipLayerWithBt1( msg["msg"].asString(), "确定");
			}
	
		}
		break;
	case QueryUserInfoResult:
		{
			UserDetailInfo usrInfo;
			usrInfo.m_bBoy = true;
			usrInfo.m_bZhuBo = false;
			usrInfo.m_uUID = JsonValueAdapter(msg["id"]);
			usrInfo.m_strName = msg["nickname"].asString();
			usrInfo.m_strIcon = msg["icon"].asString();
			usrInfo.m_dCash = JsonValueAdapter(msg["beans"]);
            usrInfo.m_dDiamond = JsonValueAdapter(msg["beans"]);

			usrInfo.m_iVipDengJi = JsonValueAdapter(msg["vip_lv"]);
			usrInfo.m_uJingYanDengJi = JsonValueAdapter(msg["lv"]);
			usrInfo.m_uJingYanJinDu = JsonValueAdapter(msg["curExp"]);
			usrInfo.m_uJingYanZongZhi = JsonValueAdapter(msg["allExp"]);
			usrInfo.m_iMeiLiZhi = JsonValueAdapter(msg["charm"]);

			usrInfo.m_uPaiJu = JsonValueAdapter(msg["play_times"]);
			usrInfo.m_fRuJuLv = JsonValueAdapter(msg["vpip_times"]);
			usrInfo.m_fShengLv = JsonValueAdapter(msg["total_win_rate"]);
			usrInfo.m_fTanPaiLv = JsonValueAdapter(msg["wtsd"]);

			usrInfo.m_fJiJinDu = JsonValueAdapter(msg["af"]);
			usrInfo.m_fZaiJiaZhu = JsonValueAdapter(msg["pfr_times"]);
			usrInfo.m_fFanQianXiaZhu = JsonValueAdapter(msg["bet3_times"]);
			usrInfo.m_fChiXuXiaZhu = JsonValueAdapter(msg["cbet"]);
			usrInfo.m_familyID = JsonValueAdapter(msg["fid"]);
			usrInfo.m_familyName = msg["fname"].asString();
			usrInfo.m_familyLV = JsonValueAdapter(msg["flv"]);
			if (usrInfo.m_uUID != g_globalMyData.m_iUserId)
			{
				auto tip = msg["tip"];
				if (tip.isObject())
				{
					usrInfo.m_iBiaoJi = JsonValueAdapter(tip["tipId"]);
					usrInfo.m_strBiaoJi = tip["tipDes"].asString();
					usrInfo.m_strBeiZhu = tip["remarkDes"].asString();
				}
				else
				{
					usrInfo.m_iBiaoJi = -1;
				}
				usrInfo.m_bYiGuanZhu = JsonValueAdapter(msg["is_followed"]);
				usrInfo.m_bYiPingBi = JsonValueAdapter(msg["is_shielded"]);
			}
			if (usrInfo.m_strIcon == "")
				usrInfo.m_strIcon = "zhongnan_3.jpg";
			m_pGameTable->PopUserInfo(usrInfo);
		}
		break;

	case FollowPlayerResult:
		{
			unsigned uSrcCh = JsonValueAdapter(msg["scid"]);
			unsigned uDstCh = JsonValueAdapter(msg["rscid"]);
			m_pGameTable->UserFollowUser(g_globalTableConfig.GetViewChairID(uSrcCh),g_globalTableConfig.GetViewChairID(uDstCh));
		}
		break;
	case OperateOtherPlayerResult:
		{
			unsigned uOpType = JsonValueAdapter(msg["type"]);
			if (uOpType == 1) // 关注
			{				
				unsigned uOpData = JsonValueAdapter(msg["data"]);
				unsigned uTID = JsonValueAdapter(msg["tuid"]);
				int iRet = JsonValueAdapter(msg["sendResult"]);
				m_pGameTable->SetGuanZhuResult(uTID, uOpData, iRet);
			}
			else if (uOpType == 2) // 屏蔽
			{				
				unsigned uOpData = JsonValueAdapter(msg["data"]);
				unsigned uTID = JsonValueAdapter(msg["tuid"]);
				int iRet = JsonValueAdapter(msg["sendResult"]);
				m_pGameTable->SetPingBiResult(uTID, uOpData, iRet);
			}
			else if (uOpType == 4) // 备注和标记
			{
				int iRet = JsonValueAdapter(msg["sendResult"]);
				if (iRet == 1)
				{					
					auto data = msg["data"];
					unsigned uTID = JsonValueAdapter(msg["tuid"]);
					m_pGameTable->SetBiaoJiOrBeiZhuResult(uTID, data["remarkDes"].asString(), JsonValueAdapter(data["tipId"]), data["tipDes"].asString());
				}
			}
		}
		break;

	case SendToolResult:
		{
			unsigned uRes = JsonValueAdapter(msg["sendResult"]);
			if (uRes == 1)
			{
				unsigned uSrc = JsonValueAdapter(msg["sendUser"]);
				unsigned uDst = JsonValueAdapter(msg["rcvUser"]);
				unsigned uTool = JsonValueAdapter(msg["magicID"]);
				m_pGameTable->UsrTool(g_globalTableConfig.GetViewChairID(uSrc), 
					g_globalTableConfig.GetViewChairID(uDst), uTool);
			}
			else if (uRes == 0)
			{
				SendTableTip("余额不足，表情赠送失败!");
			}
		}
		break;

	case GetFollowUserInfoResult:
		{
			auto& followLst = msg["onLineFriendsInfo"];
			int szFollow = followLst.size() ; //> 10 ? 10 : followLst.size();
			auto& girlLst = msg["onLineGirlsInfo"];
			int szGirl = girlLst.size() ; //> 15 ? 15 : girlLst.size();

			g_globalQuickFollowInfo.m_vecQuickFollowInfo[0].clear();
			for (unsigned i = 0; i < szFollow; i++)
			{
				auto& info = followLst[i];
				QuickFollowInfo quickFollowInfo;
				quickFollowInfo.m_uType = 0;
				quickFollowInfo.m_uUserID = JsonValueAdapter(info["id"]);
				quickFollowInfo.m_strNickName = info["nickname"].asString();
				quickFollowInfo.m_strIconUrl = info["icon"].asString();
				quickFollowInfo.m_dCash = JsonValueAdapter(info["beans"]);

				quickFollowInfo.m_bOnLine = JsonValueAdapter(info["isOnline"]);
				quickFollowInfo.m_uAreaID = JsonValueAdapter(info["areaid"]);
				quickFollowInfo.m_uTableID = JsonValueAdapter(info["tableid"]);
				quickFollowInfo.m_dLittleBlind = JsonValueAdapter(info["smallblind"]);
				quickFollowInfo.m_dBigBlind = JsonValueAdapter(info["bigblind"]);
				quickFollowInfo.m_bHasPassword = JsonValueAdapter(info["ifHavePassWord"]);
				quickFollowInfo.m_uTableType = JsonValueAdapter(info["gameType"]);

				g_globalQuickFollowInfo.m_vecQuickFollowInfo[0].push_back(quickFollowInfo);
			}
			m_pGameHall->UpdateQuickFollowList(0);
			if (m_pGameTable)
			{
				m_pGameTable->UpdateOnlineFollow(0);
			}

			g_globalQuickFollowInfo.m_vecQuickFollowInfo[1].clear();
			for (unsigned i = 0; i < szGirl; i++)
			{
				auto& info = girlLst[i];
				QuickFollowInfo quickFollowInfo;
				quickFollowInfo.m_uType = 0;
				quickFollowInfo.m_uUserID = JsonValueAdapter(info["id"]);
				quickFollowInfo.m_strNickName = info["nickname"].asString();
				quickFollowInfo.m_strIconUrl = info["icon"].asString();
				quickFollowInfo.m_dCash = JsonValueAdapter(info["beans"]);

				quickFollowInfo.m_bOnLine = JsonValueAdapter(info["isOnline"]);
				quickFollowInfo.m_uAreaID = JsonValueAdapter(info["areaid"]);
				quickFollowInfo.m_uTableID = JsonValueAdapter(info["tableid"]);
				quickFollowInfo.m_dLittleBlind = JsonValueAdapter(info["smallblind"]);
				quickFollowInfo.m_dBigBlind = JsonValueAdapter(info["bigblind"]);
				quickFollowInfo.m_bHasPassword = JsonValueAdapter(info["ifHavePassWord"]);
				quickFollowInfo.m_uTableType = JsonValueAdapter(info["gameType"]);
				g_globalQuickFollowInfo.m_vecQuickFollowInfo[1].push_back(quickFollowInfo);
			}
			m_pGameHall->UpdateQuickFollowList(1);
			if (m_pGameTable)
			{
				m_pGameTable->UpdateOnlineFollow(1);
			}

		}
		break;
	case UpdateFollowUserInfo://  大厅关注用户信息刷新
		{
			unsigned uOp = JsonValueAdapter(msg["operation"]);
			unsigned uType = JsonValueAdapter(msg["listtype"]);//0：我关注的  1：德女郎
            if (uOp == 1 || (uOp == 0 && uType ==1))//德女郎列表为所有德女郎列表，不能没有增删
                uType = 0;
			auto& info = msg["info"];
			std::vector<QuickFollowInfo>& vecQuickFollowInfo = g_globalQuickFollowInfo.m_vecQuickFollowInfo[uType];

			QuickFollowInfo quickFollowInfo;
			quickFollowInfo.m_uType = uType;
			quickFollowInfo.m_uUserID = JsonValueAdapter(info["id"]);
			quickFollowInfo.m_strNickName = info["nickname"].asString();
			quickFollowInfo.m_strIconUrl = info["icon"].asString();
			quickFollowInfo.m_dCash = JsonValueAdapter(info["beans"]);
			quickFollowInfo.m_bOnLine = JsonValueAdapter(info["isOnline"]);
			quickFollowInfo.m_uAreaID = JsonValueAdapter(info["areaid"]);
			quickFollowInfo.m_uTableID = JsonValueAdapter(info["tableid"]);
			quickFollowInfo.m_dLittleBlind = JsonValueAdapter(info["smallblind"]);
			quickFollowInfo.m_dBigBlind = JsonValueAdapter(info["bigblind"]);
			quickFollowInfo.m_bHasPassword = JsonValueAdapter(info["ifHavePassWord"]);
			quickFollowInfo.m_uTableType = JsonValueAdapter(info["gameType"]);
            if (uType == 1)
            {   //更新德女郎列表时，如果该德女郎被我关注，则同时更新我关注列表信息，否则两个列表信息不一致
                for (auto& itemInfo : g_globalQuickFollowInfo.m_vecQuickFollowInfo[0])
                {
                    if (quickFollowInfo.m_uUserID == itemInfo.m_uUserID)
                    {
                        itemInfo = quickFollowInfo;
                        //m_pGameTable->OperateOnlineFollow(2, 0, quickFollowInfo);
                        if (m_pGameHall != nullptr)
                        {
                            m_pGameHall->OperateQuickFollowList(uOp, 0, quickFollowInfo);
                        }

                        if (m_pGameTable != nullptr)
                        {
                            m_pGameTable->OperateOnlineFollow(uOp, 0, quickFollowInfo);
                        }
                        break;
                    }
                }
            }
			if (uOp == 0)
			{
				for (auto iter = vecQuickFollowInfo.begin(); iter != vecQuickFollowInfo.end(); ++iter)
				{
					if (iter->m_uUserID == quickFollowInfo.m_uUserID)
					{
						vecQuickFollowInfo.erase(iter);
						break;
					}
				}
			}
			else if (uOp == 1)
			{
				bool IsHave = false;
				for (auto iter = vecQuickFollowInfo.begin(); iter != vecQuickFollowInfo.end(); ++iter)
				{
					if (iter->m_uUserID == quickFollowInfo.m_uUserID)
					{
						uOp = 2;
						*iter = quickFollowInfo;
						IsHave = true;
						break;
					}
				}
				if(!IsHave)
				{
					vecQuickFollowInfo.insert(vecQuickFollowInfo.begin(), quickFollowInfo); 
				}
			}
			else if (uOp == 2)
			{
				for (auto iter = vecQuickFollowInfo.begin(); iter != vecQuickFollowInfo.end(); ++iter)
				{
					if (iter->m_uUserID == quickFollowInfo.m_uUserID)
					{
						*iter = quickFollowInfo;
						break;
					}
				}
			}
			if (m_pGameHall!=nullptr)
			{
				m_pGameHall->OperateQuickFollowList(uOp, uType, quickFollowInfo);
			}
			
			if (m_pGameTable)
			{
				m_pGameTable->OperateOnlineFollow(uOp, uType, quickFollowInfo);
			}
		}
		break;

	case GameMsgCmdS::GameSetInfo:
		{
			//g_globalConfig.m_bJinZhiPangGuan = JsonValueAdapter(msg["ifLook"]);
			//g_globalConfig.m_bJinZhiTongZhuo = JsonValueAdapter(msg["ifIPLimit"]);
			//g_globalConfig.m_bJuJueYaoQing = JsonValueAdapter(msg["ifDenyInvite"]); // TODO:

			//g_globalConfig.m_dMaxWinPerDay = JsonValueAdapter(msg["antiAddiction1"]["money"]);
			//g_globalConfig.m_uFreezenMinWhenWin = JsonValueAdapter(msg["antiAddiction1"]["time"]);
			//g_globalConfig.m_dMaxLosePerDay = JsonValueAdapter(msg["antiAddiction2"]["money"]);
			//g_globalConfig.m_uFreezenMinWhenLose = JsonValueAdapter(msg["antiAddiction2"]["time"]);;
			//g_globalConfig.m_MaxOnlineHour = JsonValueAdapter(msg["antiAddiction3"]["money"]);
			//g_globalConfig.m_uFreezenMinWhenTimeFull = JsonValueAdapter(msg["antiAddiction3"]["time"]);
		}
		break;

	case GameMsgCmdS::UserInviteRequest:
		{

			if (g_globalConfig.m_bJuJueYaoQing)
				break;

			g_globalQuickFollowInfo.m_bIsReceiveInvite = true;
			//std::vector<QuickFollowInfo>& vecQuickFollowInfo = g_globalQuickFollowInfo.m_vecQuickFollowInfo[2];
			QuickFollowInfo quickFollowInfo;
			quickFollowInfo.m_uType = 2;
			quickFollowInfo.m_uUserID = JsonValueAdapter(msg["id"]);
			quickFollowInfo.m_strNickName = msg["nickname"].asString();
			quickFollowInfo.m_strIconUrl = msg["icon"].asString();
			quickFollowInfo.m_dCash = 0;
			quickFollowInfo.m_bOnLine = true;
			quickFollowInfo.m_uAreaID = JsonValueAdapter(msg["areaid"]);
			quickFollowInfo.m_uTableID = JsonValueAdapter(msg["tableid"]);
			quickFollowInfo.m_dLittleBlind = JsonValueAdapter(msg["smallblind"]);
			quickFollowInfo.m_dBigBlind = JsonValueAdapter(msg["bigblind"]);
			quickFollowInfo.m_uTableType = JsonValueAdapter(msg["gametype"]);
			auto bNeedUpdate = false;
			for (auto iter = g_globalQuickFollowInfo.m_vecQuickFollowInfo[2].begin(); iter != g_globalQuickFollowInfo.m_vecQuickFollowInfo[2].end(); ++iter)
			{
				if (iter->m_uUserID == quickFollowInfo.m_uUserID)
				{
					g_globalQuickFollowInfo.m_vecQuickFollowInfo[2].erase(iter);
					bNeedUpdate = true;
					break;
				}
			}
			g_globalQuickFollowInfo.m_vecQuickFollowInfo[2].insert(g_globalQuickFollowInfo.m_vecQuickFollowInfo[2].begin(), quickFollowInfo);
			if (m_pGameTable)
			{
				//m_pGameTable->UpdateOnlineFollow(2);
				m_bIfYaoQing = true;
				m_pGameTable->OperateOnlineFollow(3, 2, quickFollowInfo);
				scheduleOnce(schedule_selector(GameFrame::onScheduleIfRemoveShouYaoQing), 30.0f);
			}
		}
		break;

	case GameMsgCmdS::EditMyInfoResult:
		{
			unsigned uRet = JsonValueAdapter(msg["sendResult"]);
			unsigned uOp = JsonValueAdapter(msg["type"]);
			std::string strData = msg["data"].asString();
			if (uRet == 0)
			{
				PopTipWithBt1(strData,"确定",nullptr) ;
				return;
			}
			/*
			当type为1时，修改用户的【头像】
			当type为2时，修改用户的【昵称】
			当type为3时，修改用户的【性别】
			当type为4时，修改用户的【QQ号码】
			当type为5时，修改用户的【邮箱】
            当type为6时，修改用户的【收货人】
            当type为7时，修改用户的【手机号码】
            当type为8时，修改用户的【所在地区】
			*/

			if (uOp == 1)
			{
				g_globalMyData.SetSkinID(atoi(strData.c_str()));
				g_globalMyData.SetVoiceID(atoi(strData.c_str()));
				//m_pGameHall->UpdateMyInfo();
			}
			else if (uOp == 2)
			{
				g_globalMyData.SetNickName(strData);
				//m_pGameHall->UpdateMyInfo();
			}
			else if (uOp == 3)
			{
				g_globalMyData.SetGender(atoi(strData.c_str()));
			}
			else if (uOp == 4)
			{
				g_globalMyData.m_strQQ = strData;
                g_globalMyAddr.m_strQQ = strData;
			}
			else if (uOp == 5)
			{
				g_globalMyData.m_strEmail = strData;
                g_globalMyAddr.m_strEmail = strData;
			}
            else if (uOp == 6)
            {
                g_globalMyAddr.m_strXingMing = strData;
            }
            else if (uOp == 7)
            {
                g_globalMyAddr.m_strShouJi = strData;
            }
            else if (uOp == 8)
            {
                g_globalMyAddr.m_strDiZhi = strData;
            }
		}
		break;

	case GameMsgCmdS::KickedByLogonTwice:
		{
			//m_pLogonLayerEx->ShowAccountLogon();

			if (g_globalTableConfig.m_uTableID >= 0) {
				forceLeaveTable();
				stopVideo();
			} else if (g_globalTableConfig.m_uTableID == -1) {
				forceLeaveHall();
			}

			PopTipWithBt1("您的账号被其他设备登录，请检查设备或者联系客服。","确定");
		}
		break;
	case ServerStopNotify: //  通知客户端，停服公告  
		{
			auto& content = msg["content"];
			PopTipWithBt1(content.asString(), "确定");
		}
		break;

	case GameMsgCmdS::RechargeUpdateMoney:
		{
			int newmoney = JsonValueAdapter(msg["money"]);
			int uVipLevel =  JsonValueAdapter(msg["viplv"]);
			int addmoney  = JsonValueAdapter(msg["addmoney"]);
            double newdiamond = JsonValueAdapter(msg["diamond"]);
			int AppendVipLevel = uVipLevel - g_globalMyData.GetVipLevel();//计算出新增加的VIP等级	
			if(AppendVipLevel > 0)
			{
				g_globalMyData.SetVipLevel(uVipLevel);
				//PopTipWithBt1("恭喜您开通VIP贵族权限！","确定");

				//Json::Value param;
				//param["idx"] = 1;
				//SendClientRequest(ClientRequest::kPopMall, param);

				if(g_globalMyData.GetVipLevel() >= 1)
				{
					if(m_pOpenUpVip == nullptr)
					{
						m_pOpenUpVip = OpenUpVip::create();
						Size sz = m_pOpenUpVip->getContentSize();
						m_pOpenUpVip->setPosition(GameData::getInstance()->getGameWidth()/2 - sz.width/2, GameData::getInstance()->getGameHeight()/2 - sz.height/2);
						addChild(m_pOpenUpVip,OpenUpVipZOrder);
					}
					m_pOpenUpVip->Update();
					m_pOpenUpVip->setVisible(true);
				}
			}
			if(addmoney >= 1)
			{
				PopDrawingAwardAni(addmoney);
			}	
			g_globalMyData.SetCash(newmoney);
            g_globalMyData.SetDiamond(newdiamond);
		}
		break;

	case ServerMessage://  后台消息通知
		{
			auto& content = msg["content"];
			PopTipWithBt1(content.asString(), "确定");
		}
		break;
	case BroadcastTips:
		{
			std::string nickName = msg["nickname"].asString();
			unsigned type = JsonValueAdapter(msg["type"]);
			double money = JsonValueAdapter(msg["money"]);
			if (type == 0 || type == 1) {
				char buffer[256];
				sprintf(buffer, "玩家%s在游戏中获取%s同花顺，获得%d德州币奖励", nickName.c_str(), type == 0 ? "皇家" : "", (int)money);
				SendTableTip(buffer);
			} 
		}
		break;
	default:
		break;
	}
}

void GameFrame::onScheduleIfRemoveShouYaoQing(float fDelat)
{
	QuickFollowInfo RemoveQuickFollowInfo;
	for(auto& quickFollow : g_globalQuickFollowInfo.m_vecQuickFollowInfo[2])
	{
		RemoveQuickFollowInfo.m_uType = quickFollow.m_uType;
		RemoveQuickFollowInfo.m_uUserID = quickFollow.m_uUserID;
		RemoveQuickFollowInfo.m_strNickName = quickFollow.m_strNickName;
		RemoveQuickFollowInfo.m_strIconUrl = quickFollow.m_strIconUrl;
		RemoveQuickFollowInfo.m_dCash = quickFollow.m_dCash;
		RemoveQuickFollowInfo.m_bOnLine = quickFollow.m_bOnLine;
		RemoveQuickFollowInfo.m_uAreaID = quickFollow.m_uAreaID;
		RemoveQuickFollowInfo.m_uTableID = quickFollow.m_uTableID;
		RemoveQuickFollowInfo.m_dLittleBlind = quickFollow.m_dLittleBlind;
		RemoveQuickFollowInfo.m_dBigBlind = quickFollow.m_dBigBlind;
		RemoveQuickFollowInfo.m_bHasPassword = quickFollow.m_bHasPassword;
	}
	g_globalQuickFollowInfo.m_bIsReceiveInvite = false;
	for (auto iter = g_globalQuickFollowInfo.m_vecQuickFollowInfo[2].begin(); iter != g_globalQuickFollowInfo.m_vecQuickFollowInfo[2].end(); ++iter)
	{
		if (m_bIfYaoQing == true && iter->m_uUserID == RemoveQuickFollowInfo.m_uUserID)
		{
			g_globalQuickFollowInfo.m_vecQuickFollowInfo[2].erase(iter);
			m_pGameTable->OperateOnlineFollow(3, 2,  RemoveQuickFollowInfo);
			m_bIfYaoQing = false;
			break;
		}
		if (m_bIfYaoQing == false && iter->m_uUserID == RemoveQuickFollowInfo.m_uUserID && 
			RemoveQuickFollowInfo.m_uTableID == g_globalTableConfig.m_uTableID && RemoveQuickFollowInfo.m_uAreaID == g_globalTableConfig.m_uRoomID )
		{
			g_globalQuickFollowInfo.m_vecQuickFollowInfo[2].erase(iter);
			m_pGameTable->OperateOnlineFollow(3, 2,  RemoveQuickFollowInfo);	
			break;
		}
		m_bIfYaoQing = true;
	}
}

void GameFrame::OnRoomIOAbord()
{

	cocos2d::log("%s", "RoomIOAbord\n");
	//PopTipWithBt1("与聊天服务器的连接中断了，请检查您的账号是否被重复登陆或者网络是否存在异常。", "确定", nullptr);
	/*
	bool connected = ConnectChatServer();
	if (connected)
	return;

	SendTableTip("与聊天服务器的连接中断了，请检查您的账号是否被重复登陆或者网络是否存在异常。");
	*/
}
void GameFrame::OnRoomMessage(RoomMsgCmdS cmdS, const char *pMsgBody, unsigned uMsgBodySize)
{

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	cocos2d::log("RoomMsg: %d %s\n", as_integer(cmdS), pMsgBody);
#endif

	std::string strBody;
	Replain2UTF8(pMsgBody, uMsgBodySize, strBody);

	Json::Value param;
	Json::Reader reader;
	//reader.parse(pMsgBody, pMsgBody + uMsgBodySize, param);
	reader.parse(strBody, param);
	switch (cmdS)
	{
	case RoomMsgCmdS::Chat:
		{
			//std::string strSName;
			//const char *pUnicode = param["sname"].asCString();
			//Replain2UTF8(pUnicode, strlen(pUnicode), strSName);
			//std::string strTName;
			//pUnicode = param["tname"].asCString();
			//Replain2UTF8(pUnicode, strlen(pUnicode), strTName);

			auto chat = param["text"];
			unsigned uChatType = JsonValueAdapter(chat["messageType"]);
			unsigned uChair = JsonValueAdapter(chat["chairID"]);

			if (m_pGameTable)
			{						
				unsigned uSUID = JsonValueAdapter(param["suid"]);

				bool bLookOn = g_globalTableConfig.GetPlayerByID(uSUID) == nullptr;

				if (bLookOn)
				{
					std::string strName = param["sname"].asString();
					if (uChatType == 0)
					{					
						std::string strText = chat["chatText"].asString();
						std::string strIcon = param["icon"].asString();
						m_pGameTable->LookonUsrChat(uSUID, strName, strIcon, strText);
					}
					else if (uChatType == 1)
					{
						unsigned uType = JsonValueAdapter(chat["type"]);
						unsigned uIdx = JsonValueAdapter(chat["faceID"]);
						m_pGameTable->LookonUsrExpress(strName, uIdx, uType);
					}
				}
				else
				{
					if (uChatType == 0)
					{					
						std::string strText = chat["chatText"].asString();
						m_pGameTable->UsrChat(g_globalTableConfig.GetViewChairID(uChair), strText);
					}
					else if (uChatType == 1)
					{
						unsigned uType = JsonValueAdapter(chat["type"]);
						unsigned uIdx = JsonValueAdapter(chat["faceID"]);
						m_pGameTable->UsrExpress(g_globalTableConfig.GetViewChairID(uChair), uIdx, uType);
					}
				}				
			}
		}
		break;
	case RoomMsgCmdS::RepeatLogin:
		{
			m_roomMsgIO.StopIO();
		}
		break;
	default:
		break;
	}
}



GameFrame::GameFrame() :
m_networkListener(nullptr)
{
}

GameFrame::~GameFrame()
{
	if (m_networkListener != nullptr) {
		Director::getInstance()->getEventDispatcher()->removeEventListener(m_networkListener);
	}
}

bool GameFrame::ConnectGameServer()
{

	m_gameMsgIO.StopIO();

#if TEST_IN
	//std::string m_ip = "192.168.1.201"  ;//
	//std::string m_ip = "192.168.1.107"  ;//
	std::string m_ip = "192.168.1.184"  ;//吕伟男
	//std::string m_ip = "192.168.1.201"  ;//内网
	//std::string m_ip = "192.168.1.187"  ;//波波
	//std::string m_ip = "60.174.249.98"  ;//ip
	//std::string m_ip = "127.0.0.1"  ;
	GameData::getInstance()->readIpAddress(m_ip) ; //写入一个默认的ip
	m_ip = GameData::getInstance()->getIpAddress() ; //打给服务器测试的包 时打开 服务器自己写ip
	bool bConnected = m_gameMsgIO.StartIO(m_ip, 9990);
	//bool bConnected = m_gameMsgIO.StartIO(m_ip, 9990);
#else
	 //bool bConnected = m_gameMsgIO.StartIO("121.40.190.33", 9990);
	bool bConnected = m_gameMsgIO.StartIO(g_globalServerInfo.m_strGameIp, g_globalServerInfo.m_uGamePort);
#endif
	 
	if (bConnected)
	{
		Json::Value body;
		body["openkey"] = g_globalSession.m_szOpenKey;
		body["gameid"] = GAME_ID;
		body["connectTag"] = 0;
		body["timestamp"] = g_globalSession.m_iTimeStamp;
		body["openid"] = g_globalSession.m_szOpenId;
        unsigned inTable = g_globalTableConfig.m_uTableID >= 0 ? 1 : 0;
        body["inTable"] = inTable;
        unsigned client = 1;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        client = 1;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        client = 2;
#endif
        body["client"] = client;
		m_gameMsgIO.SendGameMessage(GameMsgCmdC::Logon, body);
	}
	return bConnected;
}







bool GameFrame::ConnectChatServer()
{
	m_roomMsgIO.StopIO();
#if TEST_IN
	bool bConnected = m_roomMsgIO.StartIO("192.168.1.201", 8855);
#else
	// bool bConnected = m_roomMsgIO.StartIO("121.41.47.175", 8855);
	bool bConnected = m_roomMsgIO.StartIO(g_globalServerInfo.m_strChatIp, g_globalServerInfo.m_uChatPort);
#endif
	// 
	
	if (bConnected)
	{
		char buffer[32];
		sprintf(buffer, "%d%d", g_globalTableConfig.m_uRoomID, g_globalTableConfig.m_uTableID);
		Json::Value msg;
		msg["roomid"] = buffer; //g_globalTableConfig.m_uTableID;// g_globalServerInfo.GetCurGameRoomID();
		msg["userid"] = g_globalMyData.m_iUserId;
		msg["timestamp"] = g_globalSession.m_iTimeStamp;
		msg["openkey"] = g_globalSession.m_szOpenKey;

		m_roomMsgIO.SendRoomMessage(RoomMsgCmdC::Logon, msg);
	}
	return bConnected;
}





void GameFrame::PopLoading(bool bVisible, const char* msg)
{
	if (bVisible)
	{
		if (m_pGameLoading == nullptr)
		{
			m_pGameLoading = GameLoading::create(cocos2d::Color4B(0, 0, 0, 128));
			m_pGameLoading->setVisible(false);
			addChild(m_pGameLoading, GameLoadingZOrder);
		}
		m_pGameLoading->setVisible(true);

        if (msg != ""&&msg[0]!='\0')
            m_pGameLoading->setTips(msg);
	}
	else
	{
		if (m_pGameLoading)
		{
			m_pGameLoading->setVisible(false);
		}
	}
}

void GameFrame::PopPaymentLoading(bool bVisible)
{
    if (bVisible)
    {
        if (m_pPaymentLoading == nullptr)
        {
            m_pPaymentLoading = PaymentLoading::create();
            m_pPaymentLoading->setVisible(false);
            m_pPaymentLoading->setPosition(0, GameData::getInstance()->getGameHeight()/2 - 51);
            addChild(m_pPaymentLoading, GameLoadingZOrder);
        }
        m_pPaymentLoading->setVisible(true);
    }
    else
    {
        if (m_pPaymentLoading)
        {
            m_pPaymentLoading->setVisible(false);
        }
    }
}

void GameFrame::PopDrawingAwardAni(double dAward)
{
	if (m_pRewardDrawingAni == nullptr)
	{
		m_pRewardDrawingAni = RewardDrawingAni::create();
		addChild(m_pRewardDrawingAni, RewardDrawingAniZOrder);
	}
	m_pRewardDrawingAni->playGoldAnimation(dAward);
	PlayEffectSound("sound/drawreward.mp3");
}
void GameFrame::PopTablePlayBack(const Json::Value& gameRecord)
{
	if (m_pTablePlayer == nullptr)
	{
		m_pTablePlayer = GameTablePlayer::create();
		m_pTablePlayer->setVisible(false);
		addChild(m_pTablePlayer, TablePlayerZOrder);
	}
	m_pTablePlayer->StartPlay(gameRecord);
}

void GameFrame::DestroyNewbieGuide()
{
	if (m_pNewbieGuide != nullptr) {
		m_pNewbieGuide->setVisible(false);
		m_pNewbieGuide->removeFromParentAndCleanup(true);
		m_pNewbieGuide = nullptr;
	}
}

void GameFrame::PopPassWordInputBox(std::function<void(const std::string&)> cbBt)
{
	if (m_pPwdInput == nullptr)
	{
		m_pPwdInput = EnterVipRoomEx::create();
		m_pPwdInput->setVisible(false);
		m_pPwdInput->setAnchorPoint(Vec2(0.5, 0.5));
		auto pWdSize = m_pPwdInput->getContentSize();
        m_pPwdInput->setPosition((GAME_WIDTH - pWdSize.width) / 2, (GAME_HEIGHT - pWdSize.height) / 2);
		addChild(m_pPwdInput, PassWordInputZOrder);
	}
	m_pPwdInput->setVisible(true);
	m_pPwdInput->SetHitCB(cbBt);
}

void GameFrame::EnterGameHall()
{
	if (nullptr == m_pGameHall)
	{
		m_pGameHall = GameHall::create();
		m_pGameHall->setVisible(false);
		addChild(m_pGameHall, GameHallZOrder);
//		m_pGameHall->UpdateMyInfo();

	}
	
	//m_pGameHall->setVisible(true);
	m_deqHallOrTable.push_back(m_pGameHall);
	m_pGameHall->checkNewMessage() ;
}

void GameFrame::EnterGameHall2()
{
	m_deqHallOrTable.back()->setVisible(false);
	if (nullptr == m_pGameHall2)
	{
		m_pGameHall2 = GameHall2::create();
		m_pGameHall2 -> ignoreAnchorPointForPosition(false);
		m_pGameHall2 -> setAnchorPoint(Point(0.5,0.5));
		m_pGameHall2->setPosition(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height/2);
		m_pGameHall2->setVisible(false);
		addChild(m_pGameHall2, GameHall2ZOrder);
	}
	m_pGameHall2->reSetActivateInsex();
	m_pGameHall2->setVisible(true);
	m_deqHallOrTable.push_back(m_pGameHall2);
}
																					

void GameFrame::EnterGameSngHall()
{
	m_deqHallOrTable.back()->setVisible(false);
	if (nullptr == m_pGameSngHall)
	{
		m_pGameSngHall = GameSngHall::create();
		m_pGameSngHall -> ignoreAnchorPointForPosition(false);
		m_pGameSngHall -> setAnchorPoint(Point(0.5,0.5));
		m_pGameSngHall->setPosition(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height/2);
		m_pGameSngHall->setVisible(false);
		addChild(m_pGameSngHall, GameHall2ZOrder);
	}
	m_pGameSngHall->setVisible(true);
	m_deqHallOrTable.push_back(m_pGameSngHall);
}


void GameFrame::EnterGameMttHall()
{
	if (!GameData::getInstance()->getHasLoadMttFloatMatchInfo())
	{
		SendClientRequest(ClientRequest::kCallGetMttFloatMatchInfo) ;
	}

	if (nullptr == m_pGameMttHall)
	{
		m_pGameMttHall = GameMttHall::create();
		m_pGameMttHall -> ignoreAnchorPointForPosition(false);
		m_pGameMttHall -> setAnchorPoint(Point(0.5,0.5));
		m_pGameMttHall->setPosition(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height/2);
		m_pGameMttHall->setVisible(false);
		addChild(m_pGameMttHall, GameHall2ZOrder);
	}
	m_pGameMttHall->setEnterFormHall(true) ;
	PopLoading(true) ;
	SendClientRequest(ClientRequest::kMttHallInfo);




}


void GameFrame::EnterVipOpenRoom()
{
	m_deqHallOrTable.back()->setVisible(false);
	if (nullptr == m_pVipPrivateNormal)
	{
		m_pVipPrivateNormal = VipPrivateNormal::create() ;
        m_pVipPrivateNormal -> ignoreAnchorPointForPosition(false);
        m_pVipPrivateNormal -> setAnchorPoint(Point(0.5,0.5));
        m_pVipPrivateNormal->setPosition(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height/2);
		m_pVipPrivateNormal->setVisible(false);
		addChild(m_pVipPrivateNormal, VipPrivateNormalOrder);
	}
	m_pVipPrivateNormal->setVisible(true);
	m_deqHallOrTable.push_back(m_pVipPrivateNormal);
}

void GameFrame::showMttMatchState(bool flag, unsigned index)
{
    if (nullptr == m_pMTTMatchState)
    { 
        m_pMTTMatchState = MTTMatchState::create();
        m_pMTTMatchState -> ignoreAnchorPointForPosition(false);
        m_pMTTMatchState -> setAnchorPoint(Point(0.5,0.5));
      
        m_pMTTMatchState->setVisible(false);
        addChild(m_pMTTMatchState, MttMatchStateZOrder);
    }
	 m_pMTTMatchState->setPosition(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height/2);
	if (flag)
	{
		GameMttTableInfo mttTableInfo = g_globaMttGameInfo.m_curMttTable;
		m_pMTTMatchState->UpdateMTTMatchStateInfo(index);
	}
	showBgLayer(flag) ;
    m_pMTTMatchState->setVisible(flag);
    if (m_pGameTable != nullptr)
        m_pMTTMatchState->getGameTableType(m_pGameTable->getTableType());
}

unsigned GameFrame::getMttMatchStateIndex()
{
    if (m_pMTTMatchState != nullptr)
    {
        return m_pMTTMatchState->getCurSelectIndex();
    }
}

bool GameFrame::isMttMatchStateVisible()
{
    if (m_pMTTMatchState != nullptr)
    {
        return m_pMTTMatchState->getVisibleState();
    }
}

void GameFrame::updateMttMatchState()
{
    if (m_pMTTMatchState != nullptr)
    {
        return m_pMTTMatchState->UpdateMTTMatchStateInfo(0);
    }
}

void GameFrame::EnterGameTable()
{

	if (nullptr == m_pGameTable) {
		m_pGameTable = GameTable::create();
		//m_pGameTable->setPosition(GAME_WIDTH/2,GAME_HEIGHT/2);
		addChild(m_pGameTable, GameTableZOrder);

	} else {
		m_pGameTable->ResetTable();
	}

	//if (!m_pGameTable->isVisible())
	//{
		auto preNode = m_deqHallOrTable.back();
		preNode->setVisible(false);
		PopLoading(false);
		m_pGameTable->setVisible(true);
		m_pGameHall->setVisible(false);
		if (m_pGameTable != preNode)
			m_deqHallOrTable.push_back(m_pGameTable);
	//}

	

	GameDialog::cleanAllDialogs();


	if (m_pLogonLayerEx && m_pLogonLayerEx->isVisible())
	{
		m_pLogonLayerEx->setVisible(false);
	}



	//if (m_pGameTable && m_pGameTable->isVisible())
	//{
	//	m_deqHallOrTable.pop_back();
	//	PopLoading(false);
	//	if (m_pGameTable)
	//	{
	//		removeChild(m_pGameTable);
	//		m_pGameTable = nullptr;
	//	}
	//	m_pGameTable = GameTable::create();
	//	//m_pGameTable->setPosition(GAME_WIDTH/2,GAME_HEIGHT/2);
	//	addChild(m_pGameTable, GameTableZOrder);
	//	m_deqHallOrTable.push_back(m_pGameTable);
	//}
	//else
	//{
	//	m_deqHallOrTable.back()->setVisible(false);
	//	PopLoading(false);
	//	if (m_pGameTable)
	//	{
	//		removeChild(m_pGameTable);
	//		m_pGameTable = nullptr;
	//	}
	//	m_pGameTable = GameTable::create();
	//	//m_pGameTable->setPosition(GAME_WIDTH/2,GAME_HEIGHT/2);
	//	addChild(m_pGameTable, GameTableZOrder);
	//	m_deqHallOrTable.push_back(m_pGameTable);
	//	//m_pGameTable->setVisible(true);
	//}
}

void GameFrame::preloadHallTable()
{
	//if (nullptr == m_pGameHall) {
	//	m_pGameHall = GameHall::create();
	//	m_pGameHall->setVisible(false);
	//	addChild(m_pGameHall, GameHallZOrder);
	//}

	/*if (nullptr == m_pGameHall2)
	{
		m_pGameHall2 = GameHall2::create();
		m_pGameHall2 -> ignoreAnchorPointForPosition(false);
		m_pGameHall2 -> setAnchorPoint(Point(0.5,0.5));
		m_pGameHall2->setPosition(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height/2);
		m_pGameHall2->setVisible(false);
		addChild(m_pGameHall2, GameHall2ZOrder);
	}*/

	if (nullptr == m_pGameTable) {
		m_pGameTable = GameTable::create();
		addChild(m_pGameTable, GameTableZOrder);
		m_pGameTable->setVisible(false);
	}
}


void GameFrame::BackToPrevView()
{
	auto preScene = m_deqHallOrTable.back();
	if (nullptr != preScene) {
		preScene->setVisible(false);
	}
	m_deqHallOrTable.pop_back();
	m_deqHallOrTable.back()->setVisible(true);
}

void GameFrame::ShowGameSet(bool bNeedConfirm)
{
	if (nullptr == m_pGameSet)
	{
		m_pGameSet = GameSet::create();
		m_pGameSet -> ignoreAnchorPointForPosition(false);

        m_pGameSet -> setAnchorPoint(Point(0.5, 0.5));
        auto itemSz = m_pGameSet->getContentSize();
        m_pGameSet->setPosition(GameData::getInstance()->getGameWidth()/2, GameData::getInstance()->getGameHeight()/2);
		
		addChild(m_pGameSet, GameSetZOrder);
		//m_pGameSet->setVisible(false);
		m_pGameSet->UpdateSet();
	}
    if (bNeedConfirm)
    {
        //float ratioDif = (GameData::getInstance()->getGameWidth() / GameData::getInstance()->getGameHeight() - 1.5)/2.75;
        //if (ratioDif < 0) ratioDif = 0;
        //m_pGameSet -> setAnchorPoint(Point(0.7, ratioDif));
        //auto itemSz = m_pGameSet->getContentSize();
        //m_pGameSet->setPosition(GameData::getInstance()->getGameWidth()/2 + itemSz.width*2/10, GameData::getInstance()->getGameHeight()/2 - itemSz.height*(5.0-ratioDif*10)/10);
    }
    else
    {
        m_pGameSet -> setAnchorPoint(Point(0.5, 0.5));
        auto itemSz = m_pGameSet->getContentSize();
        m_pGameSet->setPosition(GameData::getInstance()->getGameWidth()/2, GameData::getInstance()->getGameHeight()/2);
    }

    m_pGameHall->ShowLayerBg(true);
	m_pGameSet->setVisible(true);
	m_pGameSet->ZhuXiaoShowSetVisible(bNeedConfirm);
    //m_deqHallOrTable.push_back(m_pGameSet);
}

void GameFrame::forceLeaveTable()
{
	m_pGameTable->ShowMeHasActioned();
    for (auto it : m_deqHallOrTable) {
        if (it != nullptr)
            it->setVisible(false);
    }
	m_gameMsgIO.SendGameMessage(GameMsgCmdC::LeaveTable);
	m_roomMsgIO.StopIO();
    m_gameMsgIO.StopIO();
	stopVideo();
	m_deqHallOrTable.clear();
    m_pLogonLayerEx->setVisible(true);
    g_globalGameSerialization.AddAtomicAction(AtomicAction::GameEnd);
	g_globalTableConfig.Clear();
	g_globalTableConfig.ClearPlayer();
	m_loginFlag = false;
	GameTipsManager::getInstance()->clean() ;
	StopBackGroundSound(false);
}

void GameFrame::forceLeaveHall()
{
    for(auto deq : m_deqHallOrTable)
    {
        if(deq != nullptr)
        {
            deq->setVisible(false);
        }
    }
    if(m_pGameHall != nullptr)
        m_pGameHall->HideShowAll();
    m_deqHallOrTable.clear();
    m_gameMsgIO.StopIO();
	m_pLogonLayerEx->setVisible(true);
    //玩家账号重复登录时，强制退出到快速登陆界面且没有提示
    //if(m_pTipLayerWithBt1 != nullptr)
    //    m_pTipLayerWithBt1->setVisible(false);
    if(m_pTipLayerWithBt2 != nullptr)
        m_pTipLayerWithBt2->setVisible(false);
	m_loginFlag = false;

	GameTipsManager::getInstance()->clean() ;
	StopBackGroundSound(false);
}

void GameFrame::jumpToLogin()
{
    if (m_pGameTable != nullptr && g_globalTableConfig.m_uTableID >= 0) {
        g_pGameFrame->forceLeaveTable();
    } else if (m_pGameHall != nullptr) {
        g_pGameFrame->forceLeaveHall();
    }

	if(m_pGameMall != nullptr && m_pGameMall->isVisible())
	{
		m_pGameMall->setVisible(false) ;
	}
	if (m_sngSignUp!=nullptr &&m_sngSignUp->isVisible())
	{
		m_sngSignUp->setVisible(false) ;
	}
	if (m_pMTTMatchState!=nullptr && m_pMTTMatchState->isVisible())
	{
		m_pMTTMatchState->setVisible(false) ;
	}
	StopBackGroundSound(false);
}

void GameFrame::forceLeave()
{
	GameData::getInstance()->setSoundOn(false);
	auto dt = DelayTime::create(0.5f);
	auto cf = CallFunc::create([this]() {
		if (m_pGameTable != nullptr && g_globalTableConfig.m_uTableID >= 0) {
			g_pGameFrame->forceLeaveTable();
			showAutoLogin();
		} else if (m_pGameHall != nullptr) {
			g_pGameFrame->forceLeaveHall();
			showAutoLogin();
		}

		GameData::getInstance()->setSoundOn(true);
	});
	this->runAction(Sequence::createWithTwoActions(dt, cf));
	

}

void GameFrame::showAutoLogin()
{
	if (checkNetworkState())
	{
		std::string strAccount, strPwd;
		bool bLastLogonSuccess = false;
		bool bAutoLogin = false;
		g_globalConfig.GetLastLogonResult(strAccount, strPwd, bAutoLogin, bLastLogonSuccess);

#if Use_3rd_Login
		bAutoLogin = false;
#endif 


		if (bAutoLogin && bLastLogonSuccess)
		{
			Json::Value param;
			param["account"] = strAccount;
			param["password"] = strPwd;

			if (param["account"]==""||param["password"]==""||param["password"]==" ")
			{
				SendGameTip("用户名或密码不能为空");
			}
			else
			{
				SendClientRequest(ClientRequest::kAutoLogon, param);
				m_pLogonLayerEx->ShowQuickLogon();
				m_pLogonLayerEx->ShowLoading();
                scheduleOnce(schedule_selector(GameFrame::CheckLogin), 20.f);
			}


		}
	}
	else
	{
		PopTipLayerWithBt1(NETWORK_DISCONNECT_STRING, "确定");
	}
}

void GameFrame::CheckLogin(float fDelt)
{
    m_pLogonLayerEx->onScheduleIfLogin(1.0);
}

void GameFrame::updateDialogVisible(float delta)
{
	m_checkDialog += delta;
	if (m_checkDialog > 0.5) {
		m_checkDialog = 0;

		if (m_pGameTable == nullptr || false == m_pGameTable->isVisible() || checkVideoVisible() == 0)
			return;

		if (GameDialog::getDialogEmpty()) {

			if (m_dlgOriginalStatus == 1) {
				int flags = checkVideoVisible();
				if (flags == 1)
					innerSetVideoVisible(true);
				else if (flags == 2) {
					innerSetVideoVisible(false);
				}

				m_dlgOriginalStatus = 0;
				cocos2d::log("test 11111111111");
			}

			return;
		}

		if (GameDialog::getDialogVisible() && checkVideoVisible() == 1 && m_dlgOriginalStatus == 0) {
			m_dlgOriginalStatus = 1;
			innerSetVideoVisible(false);
			cocos2d::log("test 22222222222222");
		}
	}
}

void GameFrame::callSocketTips(SocketNoticeType type,unsigned toUid,std::string fanme,unsigned fid)
{
	Json::Value param;
	param["uid"] = toUid;
	param["data"] = fanme ;
	param["fid"] = fid ;
	param["type"] = type ;
	SendClientRequest(ClientRequest::kCallRoleUpdateFamilyInfo,param) ;
}

void GameFrame::resetCheckNetwork()
{
    m_checkNetwork = false;
    m_checkNetworkDelta = 1.0f;
    //m_checkDuration = 0.0f;
}
void GameFrame::setTableType(TableType type)
{
	//m_pGameTable->setTableType(PrivateTable);

	m_pGameTable->setTableType(type);
}

TableType GameFrame::getTableType()
{
	return m_pGameTable->getTableType() ;
}

void GameFrame::reconnectGameServer(float delta)
{
    if (!m_checkNetwork || !m_loginFlag ||m_showReconnect || isLoginVisible())
        return;
    
    m_checkNetworkDelta += delta;
    long curTime = getSystemTime();
    if (m_lastDeltaTime == 0) {
        m_lastDeltaTime = curTime;
    }
    float realDelta = curTime - m_lastDeltaTime;
	m_checkDuration += realDelta;
    m_lastDeltaTime = curTime;
    //log("wwwww %d", (int)realDelta);
    if (m_checkNetworkDelta < 1)
        return;
    
    m_checkNetworkDelta -= 1;
    if (false == m_gameMsgIO.check()) {
        bool connected = ConnectGameServer();
		resetOnlineVars();
        if (connected && m_gameMsgIO.check())
        {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            PauseHeartBeatCheck(0);
#endif
            //SendGameTip("亲，已经重新连接到服务器...");

			/*       PopLoading(false);
			resetCheckNetwork();
			m_checkDuration = 0.0f;
			m_lastDeltaTime = 0.0f;
			if (g_globalTableConfig.m_uTableID >= 0)
			{
			ConnectChatServer();
			}*/

		
             
			

        } else {
			m_gameMsgIO.StopIO();


				if (m_checkDuration >= 30.0f) {
					m_showReconnect = true ;
					PopTipLayerWithBt2("是否继续连接服务器？", "继续", "关闭", [this](unsigned uBt){
						if (uBt == 0)
						{
							m_checkDuration = 0 ;
							m_showReconnect = false ;
						}
						if (uBt == 1)
						{
							m_showReconnect = false ;
							PopLoading(false);
							resetCheckNetwork();
							m_checkDuration = 0.0f;
							m_lastDeltaTime = 0.0f;
							if (m_pGameTable != nullptr && g_globalTableConfig.m_uTableID >= 0) {
								g_pGameFrame->forceLeaveTable();
							} else if (m_pGameHall != nullptr) {
								g_pGameFrame->forceLeaveHall();
							}

							PopTipLayerWithBt1("未能连接服务器，请检查网络是否正常！", "确定");

							cleanLogin() ;

						}
					});
				}



		


			//if (m_pGameTable!=nullptr && m_pGameTable->isVisible() && (m_pGameTable->getTableType() == MttTable ||m_pGameTable->getTableType() == SngTable))
			//{

			//}
			//else
			//{
			//

			//	/*if (m_checkDuration >= 30.0f) {
			//		PopLoading(false);
			//		resetCheckNetwork();
			//		m_checkDuration = 0.0f;
			//		m_lastDeltaTime = 0.0f;
			//		if (m_pGameTable != nullptr && g_globalTableConfig.m_uTableID >= 0) {
			//			g_pGameFrame->forceLeaveTable();
			//		} else if (m_pGameHall != nullptr) {
			//			g_pGameFrame->forceLeaveHall();
			//		}

			//		PopTipLayerWithBt1("未能连接服务器，请检查网络是否正常！", "确定");

			//		cleanLogin() ;
			//	}*/
			//}
		
		}
    } else {
        resetCheckNetwork();
		//checkOnline();
    }
}

bool GameFrame::isLoginVisible()
{
    if (nullptr != m_pLogonLayerEx && m_pLogonLayerEx->isVisible()) {
        return true;
    }
    
    return false;
}

void GameFrame::startCheckNetwork()
{
	if (m_checkNetwork)
		return;

	resetCheckNetwork();
	m_checkNetwork = true;
}

void GameFrame::checkOnline(float dt)
{
	if (m_onlineDelta < 12.0f) {
		m_onlineDelta += dt;
		return;
	}
	
	m_onlineDelta = 0.0f;

	if (isLoginVisible())
		return;

	//log("GameFrame::checkOnline()");
	m_onlineFlags = 1;
	Json::Value param;
	m_gameMsgIO.SendGameMessage(GameMsgCmdC::QueryCheckOnline, param);
	scheduleOnce(schedule_selector(GameFrame::updateCheckOnline), 8.0f);
}

void GameFrame::updateCheckOnline(float dt)
{
	if (isLoginVisible())
	{
		return ;
	}
	if (m_onlineFlags == 2) {
		// normal
		m_onlineFlags = 0;
		resetCheckNetwork();
		//cocos2d::log("updateCheckOnline...");
		return;
	}
	
	if (m_checkNetwork)
		return;

	//log("GameFrame::updateCheckOnline 2");
	popupNetworkLoading(true);
	m_gameMsgIO.StopIO();
	startCheckNetwork();
}

void GameFrame::popupNetworkLoading(bool visible)
{
	PopLoading(visible);
	if (visible && nullptr != m_pGameLoading) {
		m_pGameLoading->setTips(NETWORK_RECONNECTING_STRING);
	}
}

void GameFrame::resetOnlineVars()
{
	m_onlineDelta = 0.0f;
}

void GameFrame::showGameRun(bool flag)
{
	if (flag)
	{
		m_pLogonLayerEx = LogonLayerEx::create();
		//m_pLogonLayerEx->setPosition(0, 0);
		m_pLogonLayerEx -> ignoreAnchorPointForPosition(false);
		m_pLogonLayerEx -> setAnchorPoint(Point(0.5,0.5));
		m_pLogonLayerEx->setPosition(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height/2);

		addChild(m_pLogonLayerEx, LogonZOrder);
		m_pLogonLayerEx->ShowQuickLogon();

		m_pTextTip = TextTip2::create();
		m_pTextTip -> ignoreAnchorPointForPosition(false);
		m_pTextTip -> setAnchorPoint(Point(0.5,0.5));

		auto textTipSz = m_pTextTip->getContentSize();
		m_pTextTip->setPosition(GameData::getInstance()->getGameWidth()/2, GameData::getInstance()->getGameHeight() - textTipSz.height/2);
		addChild(m_pTextTip, TextTipZOrder);

		srand(time(nullptr));

		//preloadHallTable();
		//showAutoLogin();
        

        TrackMgr->event("EnterGame", "进入登陆界面");
	}
}

void GameFrame::showLoginLayer(bool flag)
{
	m_pLogonLayerEx->setVisible(flag) ;
}

void GameFrame::showLoadingLayer(bool flag)
{
	//if (m_loadingLayer == nullptr)
	//{
	//	m_loadingLayer = LogoLayer::create() ;
	//	m_loadingLayer -> ignoreAnchorPointForPosition(false);
	//	m_loadingLayer -> setAnchorPoint(Point(0.5,0.5));
	//	m_loadingLayer->setPosition(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height/2);

	//	addChild(m_loadingLayer, LogonZOrder);
	//}
	//m_loadingLayer->setVisible(flag) ;
}

void GameFrame::PopNewerTask()
{
    m_pNewPlayerTask = NewPlayerTask::create();
    m_pNewPlayerTask -> ignoreAnchorPointForPosition(false);
    m_pNewPlayerTask -> setAnchorPoint(Point(0.5,0.5));
    m_pNewPlayerTask->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
    addChild(m_pNewPlayerTask, NewPlayerTaskZOrder);
    //m_pNewPlayerTask->setVisible(false);
}

void GameFrame::callShowSngSignUp(bool flag,const GlobalSngTableInfo& followInfo)
{
	showBgLayer(flag) ;

	if (m_sngSignUp == nullptr)
	{
		float ratioDif = (GameData::getInstance()->getGameWidth() / GameData::getInstance()->getGameHeight() - 1.5)/2.75;
		if (ratioDif < 0) ratioDif = 0;
		m_sngSignUp = SngSignUp::create() ;
		m_sngSignUp -> ignoreAnchorPointForPosition(false);
		auto itemSz = m_sngSignUp->getContentSize();;
		m_sngSignUp -> setAnchorPoint(Point(0.5,0.5));
		m_sngSignUp->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);

		addChild(m_sngSignUp,SngSignUpZOrder);	
	}

	m_sngSignUp->setVisible(flag) ;
	if (flag)
	{
		m_sngSignUp->UpdateView(followInfo);
	}
	//showBlack(flag) ;
}

void GameFrame::hideUserItemLayer()
{
	 m_pGameHall->doHideUserItemLayer() ;
}

void GameFrame::showGameMall(int index)
{

	if (m_pGameMall == nullptr)
	{
		m_pGameMall = GameMall::create();
		m_pGameMall -> ignoreAnchorPointForPosition(false);
		m_pGameMall -> setAnchorPoint(Point(0.5,0.5));
		m_pGameMall->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		m_pGameMall->SwitchToArea(index);
		this->addChild(m_pGameMall, GameMallZOrder);
	}
	else
	{
		m_pGameMall->SwitchToArea(index);
		m_pGameMall->setVisible(true);
	}
	m_pGameMall->onCancelMenuEnable(g_globalMyData.GetVipLevel());

}


void GameFrame::callShowUserItem()
{
	m_pGameHall->showUserItem(true) ;
}

/************************************************************************/
/* 读取本地相关存档                                                                     */
/************************************************************************/
void GameFrame::readData()
{
	GameData::getInstance()->readSngOnline(g_globalMyData.m_iUserId) ;//sng更新
}

void GameFrame::cleanLogin()
{
	m_rpcRoutine.clear();
}

void GameFrame::callGameTipsMove()
{
	if (m_pGameTable!=nullptr&&m_pGameTable->isVisible())
	{
		m_pGameTable->callNextScrollText() ;
	}
	else
	{
		if (m_pGameHall!=nullptr)
		{
			m_pGameHall->callScrollTextMove() ;
		}
	}
}


void GameFrame::showBgLayer(bool flag)
{

	if (m_bgLayer == nullptr)
	{
		m_bgLayer = LayerColor::create(Color4B(0x00, 0x00, 0x00, 0xaf), GameData::getInstance()->getGameWidth(), GameData::getInstance()->getGameHeight()) ;
		m_bgLayer->setAnchorPoint(Point(0.5,0.5)) ;
		m_bgLayer->ignoreAnchorPointForPosition(false) ;
		m_bgLayer->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2) ;
		this->addChild(m_bgLayer,SngSignUpZOrder-1);
	}
	m_bgLayer->setVisible(flag);
}


void GameFrame::showMasterRankList(bool flag)
{
	if (m_masterRankList == nullptr)
	{
		m_masterRankList = SngRankList::create() ;
		m_masterRankList -> ignoreAnchorPointForPosition(false);
		m_masterRankList -> setAnchorPoint(Point(0.5,0.5));
		m_masterRankList->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		addChild(m_masterRankList,ZOrder_Rule);	
	}
	m_masterRankList->setVisible(flag) ;
	showBgLayer(flag) ;
}
void GameFrame::UpdateSngRankList(unsigned uRankType, unsigned uWinType)
{
	if (m_masterRankList && m_masterRankList->isVisible())
	{
		m_masterRankList->UpdateRankList(uRankType, uWinType);
	}
}

/************************************************************************/
/* mtt比赛倒计时60秒的提示                                                                     */
/************************************************************************/
void GameFrame::showMttStartNotice(bool flag ,unsigned tableid ,std::string name,unsigned time)
{
	if (m_mttStartNotice == nullptr)
	{
		m_mttStartNotice = MttStartNotice::create() ;
		m_mttStartNotice->ignoreAnchorPointForPosition(false) ;
		m_mttStartNotice->setAnchorPoint(Point(0.5,1)) ;
		m_mttStartNotice->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()) ;
		this->addChild(m_mttStartNotice,MttStart_ZORDER) ;
	}
	if (flag)
	{
		bool flag = false ;
		unsigned tableTime ;
		for (auto& table:g_globalMttHallInfo.m_mttTableInfo)
		{
			if (table.m_tableID == tableid)
			{
				flag = true ;
				tableTime = table.m_realTime ;
			}
		}
		if (flag)
		{
			if (tableTime<=time)
			{
				m_mttStartNotice->addTips(tableid,name,tableTime,false) ;
			}
			else
			{
				m_mttStartNotice->addTips(tableid,name,time,false) ;
			}
		}
		else
		{
			m_mttStartNotice->addTips(tableid,name,time,false) ;
		}




		
	}
	m_mttStartNotice->setVisible(flag);

}

/************************************************************************/
/* mtt比赛 报名后的 每五分一次的提示                                                                     */
/************************************************************************/
void GameFrame::showMttGameStart(bool flag,std::string name/* ="" */)
{
	if (m_mttStartNotice == nullptr)
	{
		m_mttStartNotice = MttStartNotice::create() ;
		m_mttStartNotice->ignoreAnchorPointForPosition(false) ;
		m_mttStartNotice->setAnchorPoint(Point(0.5,1)) ;
		m_mttStartNotice->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()) ;
		this->addChild(m_mttStartNotice,MttStart_ZORDER) ;
	}
	if (flag)
	{
		m_mttStartNotice->callMatchTips(name) ;
	}
	m_mttStartNotice->setVisible(flag);
}

void GameFrame::showMttMatchDisMissTips(bool flag ,unsigned tableid ,std::string name,std::string reason)
{
	
	if (flag)
	{
		PopTipWithBt1("比赛由于人数不足解散，报名费将返还到您的账户", "确定", [this,tableid](unsigned uBt){
			if (uBt == 0)
			{

				if (g_pGameFrame->isInTable()&&g_pGameFrame->getTableType() == MttTable 
					&& tableid ==g_globaMttGameInfo.m_curMttTable.m_tableID) //在等候室中
				{
					SendClientRequest(ClientRequest::kCallLevelTable) ;
				}
			}
		});
	}

}


void GameFrame::doJoinMttMatch(unsigned tableid,bool delayRegist)
{
 
	m_pGameTable->ShowMeHasActioned();
	//m_gameMsgIO.SendGameMessage(GameMsgCmdC::LeaveTable);
	m_roomMsgIO.StopIO();
	stopVideo();
	g_globalGameSerialization.AddAtomicAction(AtomicAction::GameEnd);
	StopBackGroundSound(false);

	Json::Value param;
	param["id"] = tableid;
	SendClientRequest(ClientRequest::kEnterMttTable, param);

}


void GameFrame::JoinToWaitingRoom(unsigned tableid,std::string tablename,int time)
{
	if (m_pMTTMatchState != nullptr){
		m_pMTTMatchState->setVisible(false);
	}
	if(m_pGameMall != nullptr && m_pGameMall->isVisible())
	{
		m_pGameMall->setVisible(false) ;
	}

	if(m_pGameHall != nullptr)
		m_pGameHall->HideShowAll();

	g_globaMttGameInfo.clear() ;
	
	bool flag = false ;
	for (auto &table:g_globalMttHallInfo.m_mttTableInfo)
	{
		if (table.m_tableID ==tableid )
		{
			g_globaMttGameInfo.m_getRecord = true ;
			g_globaMttGameInfo.setCurMttMatchInfo(table);
			flag = true ;
		}
	}

	if (!flag)
	{
		if (!GameData::getInstance()->getHasLoadMttFloatMatchInfo())
		{
			SendClientRequest(ClientRequest::kCallGetMttFloatMatchInfo) ;
		}

		g_globaMttGameInfo.m_getRecord = false ;
		SendClientRequest(ClientRequest::kCallMttSingleMatchInfo) ;
		
	}
	else
	{
		g_globaMttGameInfo.m_rebuyCount = g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_rebuyCount ;
		g_globaMttGameInfo.m_addonCount = g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_addOnCount ;
	}
	

	g_pGameFrame->hideUserItemLayer() ;
	showMttGameStart(false); //关闭五分钟 和60秒的提示
	showJoinMatchTips(false) ;//关闭20秒的提示
	showMttMatchDisMissTips(false) ;//关闭解散提示


	setTableType(MttTable) ;
    if (m_pMTTMatchState != nullptr)
        m_pMTTMatchState->getGameTableType(m_pGameTable->getTableType());
	PopLoading(false);
	m_pGameTable->CleanTable();
	m_pGameTable->CleanPlayer();
	g_globalTableConfig.ClearPlayer();
	m_pGameHall->reSetScorlllTips() ;
	//m_pGameTable->callNextScrollText() ;
	EnterGameTable();
	m_pGameTable->UpdateItemBox();
	
	GameData::getInstance()->setSoundOn(true) ;
	m_pGameTable->SetUser(g_globalTableConfig.GetMyViewChairID(), g_globalMyData.GetNickName(), 
		g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl,
		g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_firstMoney, g_globalMyData.m_iUserId,
		g_globalMyData.GetVoiceID(),g_globalMyData.GetSkinID(),
		g_globalMyData.GetVipLevel(), -1, 0,g_globalMyData.GetGender(), "");

	//sng更新桌子名字 盲注

	g_globalTableConfig.m_strRoomName = tablename;

	m_pGameTable->updateMatchBindTipsInTable() ;
	g_globaMttGameInfo.m_rank = 1 ;
	//m_pGameTable->updateMttRankInTable(false) ;
	m_pGameTable->updateMttTime(time) ;
	m_pGameTable->reSetMttTable(false) ;

	g_globaMttGameInfo.m_inWaittingRoom = true ;
}

/************************************************************************/
/* 退赛界面     0金币  1 门票  2免费                                                                */
/************************************************************************/
void GameFrame::showQuitMatch(bool flag ,int type)
{
	if (m_mttQuitMatch == nullptr)
	{
		m_mttQuitMatch = MttQuitMatch::create() ;
		m_mttQuitMatch -> ignoreAnchorPointForPosition(false);
		m_mttQuitMatch -> setAnchorPoint(Point(0.5,0.5));
		m_mttQuitMatch->setPosition(GameData::getInstance()->getGameWidth()/2 ,GameData::getInstance()->getGameHeight()/2 );
		addChild(m_mttQuitMatch,MttStart_ZORDER);	
	}

	m_mttQuitMatch->setVisible(flag) ;
	m_mttQuitMatch->updateView(type) ;
	//showBgLayer(flag) ;
}


/************************************************************************/
/* 显示报名成功                                                                     */
/************************************************************************/
void GameFrame::showSignUpSucess(bool flag,std::string name/* ="" */,std::string money/* ="" */,std::string time/* ="" */)
{
	if (m_mttSignUpSucess == nullptr)
	{
		m_mttSignUpSucess = MttSignUpSuccess::create() ;
		m_mttSignUpSucess -> ignoreAnchorPointForPosition(false);
		m_mttSignUpSucess -> setAnchorPoint(Point(0.5,0.5));
		m_mttSignUpSucess->setPosition(GameData::getInstance()->getGameWidth()/2 ,GameData::getInstance()->getGameHeight()/2 );
		addChild(m_mttSignUpSucess,MttSiginUpSuccess);	
	}
	if (flag)
	{
		m_mttSignUpSucess->updateView(name,money,time) ;
	}
	m_mttSignUpSucess->setVisible(flag) ;
}

bool GameFrame::isInTable()
{
	return m_pGameTable->isVisible() ;
}

void GameFrame::updateMttHallFixTime(float time)
{
	if (m_pGameMttHall!=nullptr)
	{
		m_pGameMttHall->updateFixTime(time) ;
	}
	if (m_mttJoinMatchTips!=nullptr && m_mttJoinMatchTips->isVisible())
	{
		m_mttJoinMatchTips->doAddMissTime(time);
	}

	if (m_mttStartNotice!=nullptr && m_mttStartNotice->isVisible())
	{
		m_mttStartNotice->addMissTime(time) ;
	}

}



/************************************************************************/
/* 倒数20秒入场提示                                                                     */
/************************************************************************/
void GameFrame::showJoinMatchTips(bool flag,std::string name/* ="" */,unsigned time/* =0 */,unsigned tableid)
{
	if (m_pGameMttHall==nullptr)
	{
		return ;
	}
	if (!m_pGameMttHall->isVisible())
	{
		return ;
	}

	if (m_mttJoinMatchTips == nullptr)
	{
		m_mttJoinMatchTips = MttJoinMatchTips::create() ;
		m_mttJoinMatchTips -> ignoreAnchorPointForPosition(false);
		m_mttJoinMatchTips -> setAnchorPoint(Point(0.5,0.5));
		m_mttJoinMatchTips->setPosition(GameData::getInstance()->getGameWidth()/2 ,GameData::getInstance()->getGameHeight()/2 );
		addChild(m_mttJoinMatchTips,MttJoin_ZORDER);	
	}

	if (flag)
	{
		m_mttJoinMatchTips->updateView(name,time,tableid,false) ;
	}
	m_mttJoinMatchTips->setVisible(flag) ;
	showBgLayer(flag) ;
}


bool GameFrame::isMttStateVisiable()
{
	return m_pMTTMatchState->isVisible()  ;
}
bool GameFrame::isNewPlayerTaskVisiable()
{
	return m_pNewPlayerTask->isVisible()  ;
}

void GameFrame::popHallBottomDialog(unsigned uType)
{
    m_pGameHall->showBottomView(uType);
}


void GameFrame::requestHasMatchByInTable(bool flag)
{

	GameData::getInstance()->setHasRequestMatch(false) ;

	Json::Value msg;
	msg["inTable"] = flag?1:2;


	msg["openid"] = g_globalSession.m_szOpenId;


	SendClientRequest(ClientRequest::kCallRequestHasMatch,msg);
}

void GameFrame::doShankeTable(float px,float py,float time, int count)
{
	
	if (count ==0 )
	{
		m_pGameTable->setPositionX(0);
		m_pGameTable->setPositionY(0);
		return ;
	}
	count-=1 ;
	auto action  = [this,px,py,time,count](){
		if (count%2==0)
		{
			m_pGameTable->setPositionX(this->getPositionX()+px);
			m_pGameTable->setPositionY(this->getPositionX()+py);
		}
		else
		{
			m_pGameTable->setPositionX(this->getPositionX()-px);
			m_pGameTable->setPositionY(this->getPositionX()-py);
		}
		
	} ;

	auto callNext = [this,px,py,time,count](){
		doShankeTable(px,py,time,count) ;
	} ;

	this->runAction(Sequence::create(CallFunc::create(action),DelayTime::create(time),CallFunc::create(callNext),NULL)) ;
}

void GameFrame::PopActivityReward()
{
    if (m_pActivityRewards == nullptr)
    {
        m_pActivityRewards = ActivityRewards::create();
        m_pActivityRewards -> ignoreAnchorPointForPosition(false);
        m_pActivityRewards -> setAnchorPoint(Point(0.5,0.5));
        m_pActivityRewards->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
        this->addChild(m_pActivityRewards, ActRewardZORDER);
    }
    m_pActivityRewards->setVisible(true);
    m_pActivityRewards->UpdateActivityRewards();
}

void GameFrame::updateSNGTask()
{
    unsigned uAreaID = g_globalTableConfig.m_uAreaID + 1959;
    for (auto itemInfo : g_globalMissionInfo.GetMissions(0))//在每日任务中遍历更新SNG任务
    {
        if (itemInfo.uMissonStatus == 0 && itemInfo.uMissionID == uAreaID)
        {
            Json::Value param;
            param["task_id"] = itemInfo.uMissionID;
            SendClientRequest(ClientRequest::kUpdateMission, param);
        }
    }
    for (auto itemInfo : g_globalMissionInfo.GetMissions(3))//在每周任务中遍历更新SNG任务
    {
        if (itemInfo.uMissonStatus == 0 && (itemInfo.uMissionID == 2068 || itemInfo.uMissionID == 2069))
        {
            Json::Value param;
            param["task_id"] = itemInfo.uMissionID;
            SendClientRequest(ClientRequest::kUpdateMission, param);
        }
    }
    //更新竞技之神
    for (auto itemInfo : g_globalMissionInfo.GetMissions(2))
    {
        if (itemInfo.uMissonStatus == 0 && (itemInfo.uMissionID >= 2072 && itemInfo.uMissionID <= 2080))
        {
            Json::Value param;
            param["task_id"] = itemInfo.uMissionID;
            SendClientRequest(ClientRequest::kUpdateMission, param);
        }
    }
}

void GameFrame::updateMTTTask()
{
    //结算后更新所有MTT相关的任务成就
    for (auto itemInfo : g_globalMissionInfo.GetMissions(0))//在每日任务中遍历更新MTT任务
    {
        if (itemInfo.uMissonStatus == 0 && (itemInfo.uMissionID == 2065 || itemInfo.uMissionID == 2066))
        {
            Json::Value param;
            param["task_id"] = itemInfo.uMissionID;
            SendClientRequest(ClientRequest::kUpdateMission, param);
        }
    }
    for (auto itemInfo : g_globalMissionInfo.GetMissions(3))//在每周任务中遍历更新MTT任务
    {
        if (itemInfo.uMissonStatus == 0 && (itemInfo.uMissionID == 2070 || itemInfo.uMissionID == 2071))
        {
            Json::Value param;
            param["task_id"] = itemInfo.uMissionID;
            SendClientRequest(ClientRequest::kUpdateMission, param);
        }
    }
    //MTT牌局结束后更新德州之王
    for (auto itemInfo : g_globalMissionInfo.GetMissions(2))
    {
        if (itemInfo.uMissonStatus == 0 && (itemInfo.uMissionID >= 2081 && itemInfo.uMissionID <= 2089))
        {
            Json::Value param;
            param["task_id"] = itemInfo.uMissionID;
            SendClientRequest(ClientRequest::kUpdateMission, param);
        }
    }
}

void GameFrame::updateNormalTask()
{
    if (g_globalTableConfig.m_uAreaID >= 0 && g_globalTableConfig.m_uAreaID < 4)
    {
        // 初出茅庐，小有成就，大显身手
        static char *s_szTagName[] ={
            "texas_ccml",
            "texas_xycj",
            "texas_dxss",
        };
        unsigned uAreaID = g_globalTableConfig.m_uAreaID;
        if (uAreaID == 1 || uAreaID == 2)
            uAreaID = 1;
        else if (uAreaID == 3)
            uAreaID = 2;
        for (int i = 0; i < g_globalMissionInfo.GetMissions(0).size(); i++)
        {
            auto& mission = g_globalMissionInfo.GetMissions(0)[i];
            if (mission.uMissonStatus == 0 && mission.strMissionTagName == s_szTagName[uAreaID])
            {
                Json::Value param;
                param["task_id"] = mission.uMissionID;
                SendClientRequest(ClientRequest::kUpdateMission, param);
            }
        }
        //更新富可敌国、勤劳致富
        for (auto itemInfo : g_globalMissionInfo.GetMissions(2))
        {
            if (itemInfo.uMissonStatus == 0 && (itemInfo.uMissionID >= 2009 && itemInfo.uMissionID <= 2026))
            {
                Json::Value param;
                param["task_id"] = itemInfo.uMissionID;
                SendClientRequest(ClientRequest::kUpdateMission, param);
            }
        }
    }
}



void GameFrame::showSelfFundLayer(bool flag,int index)
{
	if (m_selfFund == nullptr)
	{
		m_selfFund = SelfFund::create() ;
		m_selfFund -> ignoreAnchorPointForPosition(false);
		m_selfFund -> setAnchorPoint(Point(0.5,0.5));
		m_selfFund->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		this->addChild(m_selfFund,SelfFund_ZORDER) ;
	}
	if (flag)
	{
		m_selfFund->setCurFundType(index) ;
	}
	m_selfFund->setVisible(flag);
}


bool GameFrame::checkTableSocket(unsigned tid)
{
    bool flag = false ;
    if(tid == GameMsgCmdS::PlayerComeIn
        ||tid == GameMsgCmdS::PlayerLeave
        ||tid == GameMsgCmdS::GameStart
        ||tid == GameMsgCmdS::BetResult
        ||tid == GameMsgCmdS::FoldCardResult
        ||tid == GameMsgCmdS::BrightBrandResult
        ||tid == GameMsgCmdS::GameEnd
        ||tid == GameMsgCmdS::RewardResult
        )
    {
        flag = true ;
    }

    return flag;
}
