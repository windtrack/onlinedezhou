#include "GameAbout.h"
#include "Util.h"
#include "GameCommon.h"

USING_NS_CC;

bool GameAbout::init()
{
	if (!GameDialog::init("dialog/bg.png", true))
	{
		return false;
	}
	auto title = CreateNode1AndPosAddTo<Sprite>("dialog/gameabouttitle.png", 470, 568, this, 0);
	auto m_labelProtocol = CreateLabelMSYHAndAnchorPosClrAddTo(20, "", Vec2(0, 0), 68, 40, Color4B(0x79,0xd5,0x96,0xff), this, 0);
	m_labelProtocol->setHorizontalAlignment(TextHAlignment::LEFT);
	m_labelProtocol->setLineHeight(26);
	m_labelProtocol->setWidth(810);
	m_labelProtocol->setLineBreakWithoutSpace(true);

	std::string strtext = "        本软件由上海谢天网络科技有限公司开发，游戏中使用到的德州币为游戏道具，不具有任何财产性功能，只限用户本人在游戏中使用，本公司对用户所拥有的德州币不提供任何相互叫卖、转让德州币等行为，如一经查出则永久封号，账户清零。我们将会在用户的每次对局中收取参与对局的一定量的门票费用，这个费用根据房间级别设置您不得进行任何侵犯'德州扑克'智慧财产权的行为，或者进行其他的有损于德州其他玩家合法权益的行为，这些行为包括但不限于:\n         1、进行任何破坏游戏公平性或其他影响游戏正常秩序的行为，如主动或被动刷分、刷币、导币、合作作弊、使用游戏外挂或者其他的作弊软件、利用BUG来获得不正当的非法利益，或者利用互联网或其他方式将游戏外挂、作弊软件、BUG公之于众;\n       2、进行任何诸如发布公告、销售商品的商业行为，或者进行任何非法的侵害德州扑克利益的行为，如倒卖外挂、德州币、游戏道具等行为；\n       3、发表、转发、传播含有谩骂、诅咒、诋毁、攻击、诽谤德州扑克的，或者含有封建迷信、淫秽、色情、下流、恐怖、暴力、凶杀、赌博、反动、煽动民族仇恨、危害国家统一、颠覆国家政权等让人反感、厌恶的内容等非法言论，或者设置含有上述内容的头像、网名、游戏角色名；\n        上海谢天网络科技有限公司有权采取技术和法律措施来防止您从事以上的行为，如果您违反约定从事以上三条列明的行为，上海谢天网络科技有限公司有权根据情节严重程度对您给予相应的惩处措施";
	SetLabelString(m_labelProtocol, strtext);
	return true;
}