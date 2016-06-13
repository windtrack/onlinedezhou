#include "GameLoading.h"
#include "GameCommon.h"
#include "Util.h"
#include "FrameSprite.h"
#include "AnimateViewer.h"
#include "GameData.h"
USING_NS_CC;

bool GameLoading::init()
{
	if (!GameLayer::init(GameData::getInstance()->getGameWidth(),GameData::getInstance()->getGameHeight()))
	{
		return false;
	}
	auto bg = Sprite::create("loading/bglding.png");
	bg->setPosition(GameData::getInstance()->getGameWidth() / 2, GameData::getInstance()->getGameHeight() / 2);
	addChild(bg);

	m_pAniViewer = AnimateViewer::create("login/waitbegin.png", true, 0.1, 3, 3, 8);
	m_pAniViewer->setPosition(GameData::getInstance()->getGameWidth()/2, 83);
	bg->addChild(m_pAniViewer);

	m_pLabelLoading = CreateLabelMSYH(26, "");
	m_pLabelLoading->setPosition(GameData::getInstance()->getGameWidth()/2, 27);
	bg->addChild(m_pLabelLoading);

	return true;
}

bool GameLoading::init(const cocos2d::Color4B& clr)
{
	if (!GameLayer::init(GameData::getInstance()->getGameWidth(), GameData::getInstance()->getGameHeight(), clr))
	{
		return false;
	}
	auto bg = Sprite::create("loading/bglding.png");
	bg->setPosition(GameData::getInstance()->getGameWidth() / 2, GameData::getInstance()->getGameHeight() / 2);
	addChild(bg);

	m_pAniViewer = AnimateViewer::create("login/waitbegin.png", true, 0.1, 3, 3, 8);
	m_pAniViewer->setPosition(GameData::getInstance()->getGameWidth() / 2, 83);
	bg->addChild(m_pAniViewer);

	m_pLabelLoading = CreateLabelMSYH(26, "");
	m_pLabelLoading->setPosition(GameData::getInstance()->getGameWidth() / 2, 27);
	m_pLabelLoading->setMaxLineWidth(GameData::getInstance()->getGameWidth()-200);
	bg->addChild(m_pLabelLoading);

	SetEscKeyReleasedCB([this](){setVisible(false);});
	return true;
}

GameLoading * GameLoading::create(const cocos2d::Color4B& clr)
{
	auto ret = new GameLoading;
	if (ret->init(clr))
	{
		ret->autorelease();
	}
	else
	{
		delete ret;
		ret = nullptr;
	}
	return ret;
}

void GameLoading::setVisible(bool bVisible)
{
	bool isVisibleNow = isVisible();
	if (isVisibleNow != bVisible)
	{
		GameLayer::setVisible(bVisible);
		m_pAniViewer->setVisible(bVisible);
		if (bVisible)
		{
			ResetTip();
		}
		else
		{
		}		
	}
}

void GameLoading::ResetTip()
{
	const char *szTips[] = {
"拿到皇家同花顺的概率为1/649739",
"拿到两对的概率为1/20",
"等待也是一种幸福，因为运气不知何时就会降临，只要你还活着",
"大盲注的下家叫枪口位，因为翻牌前第一个行动，你需要很好的牌才能玩",
"游戏有它的规则，遵守规则是一个牌手的基本要求",
"尊重你对手的下注，尤其是反加",
"德州扑克制胜格言：冷静的头脑和出众的判断",
"你的水平，与你扔掉AA的次数成正比",
"德州扑克常犯的错误：过于注意手中的牌而忽略对手拿了什么牌",
"人生不能赌，但一定要博",
"德州扑克比牌不分花色，从大小比牌",
"德州扑克里位置很重要，后面的玩家可以获得更多的信息来分析前面的玩家拿了什么牌",
"希望你能通过玩德州学会懂得应该在什么时候放弃",
"有空去“活动”看看，也许会有意外的惊喜",
"任何时候都要保持清醒的头脑和平和的心态",
"打德州不偷鸡不行，老偷鸡更不行（你迟早会被人抓住）",
"需要靠抽牌才能获胜的时候你通常都抽不到",
"别说60%，即使有95%的机会赢，我也不会在公告牌还没发出来就ALL in",
"弃牌是德州扑克里常见的策略，牌不好就放弃掉等待下一次机会",
"当对手给你一个合适的底池比例时，通常你已经输了",
"百人以上的德州扑克竞赛，进入决赛桌和进入钱圈都需要有马拉松长跑的精神",
"人生就像打牌，不要在意抓到多好的牌，关键是怎样打好一手烂牌",
"每个人要走的路都是他自己选的",
"拿到对A并不是你必胜的保证，有可能成为你出局的陷阱",
"永远不要像照片里的人一成不变的挂在那里，你必须综合你的行为，让对手一直在猜测你的目的",
"当你的身家不是最高买入的10倍以上时，最好不要坐进去玩（你在小场一个月赢的钱可能在大场一小时就输光）",
"做一个绅士，即使输掉所有筹码但仍然可以向对手致敬",
"保持着娱乐的心情来游戏吧",
"交手中不断变换自己的打牌作风，可迷惑你的敌手，激起他们的好奇心，分散他们的注意力",
"博弈中必要的三因素是：有六成的胜算；金钱管理术；了解你自己",
"多总结下你输大钱的那些牌，比一直不停的打下去更重要",
"德州扑克最大的悬念在于河牌？不是，是你的心态",
"技术、胆识、运气、心态是赢钱的关键",
"小输十把没关系，大赢一把就OK了",
"想赢钱不要怕有牌就是怕无牌",
"德州扑克游戏的规律就是没有任何一点规律可寻！",
"如果你有一手大牌，别忘了对手可能有一手更大的牌",
"当你把对手套进去的时候，通常你自己也被套进去了",
"当你情绪低落时请不要玩德州扑克，那会让你输光所有的筹码",
"牌不分大小，赢了就叫好牌！赌的不是运气，赢的是你的智慧！",
"系统会自动在您参与的牌局中收取一定数量的台子费",
"冲动是魔鬼，犹豫也是",
"断臂也比失去生命强。牌局如此，股市如此，人生也如此",
"尊重自己，尊重别人是长久胜利的口粮",
"最笨的方法往往是最有效的方法",
"天堂与地狱就在一线之间！",
"沉闷的赢钱总比畅快的输钱要好（这就是为什么高手场总是不停弃牌的原因）",
"宁可仍让一时，切莫冲动一次",
"不要过于依赖自己的底牌，真正的底牌其实是河牌",
"尊重对手就是尊重自己的钱包、",
"有识尚需有胆方可成赢家",
"拼命去争取成功，但不要期望一定会成功——法拉第",
"只要你不是最好的牌，就要认真思索对方下重注的理由",
"底牌拿到AA或KK的几率是1/221",
"大牌要你命，小牌能救命",
"【德州扑克和人生一样】1理解不确定性；2专注有可能的事情；3理智评估概率；4能舍才能得；5避免意气用事",
"那个一直弃牌的傻瓜其实才是能一口吞掉你的巨鲨",
"不要看人家有多少钱，要看自己有什么牌",
"怯者无功，贪者丧身",
"德州扑克最大的特点就是—这不仅仅是个只看底牌大小的游戏！",
"一万赢到一千万并不难，难的是你一直坚持正确的打法",
"就算AA对27，也不是100%赢的",
"神奇的河牌的故事每天都在上演",
"玩牌要注意素质，别输了哇哇的说人运气，赢了哇哇的说人面",
"装成保守的牌手，因为这样别人会把你的底牌想象的无限大",
"你不能控制输赢，但能控制输赢的多寡",
"真正的绅士，不会谈论离别的女人和错过的底牌",
"只要你活的比对手时间长你就赢了",
"人生就像一场牌局，开局都是一样，过程却大不相同，把握机遇，让生活更精彩",
"虽然保守不一定是最好的战略，但冲动只会让你结束得更快",
"做一个绅士，赢掉对方所有的筹码后再礼貌的对他说其实是自己运气好而已",
"每个牌局都好似人生，该出手是就要出手，不要错过机会而感到后悔",
"新手常犯的错误：玩太多的牌，喜欢跟注",
"冲的猛不一定走的远",
"德州扑克可以锻炼你对财富的控制能力，它不是一个简单的游戏",
"输赢不是问题，要的是瞬间的感觉，即使要离开那也是上帝的安排",
"没有最强的对手，只有最弱的自己！",
"打牌要打出拳手的风范，进可攻退可守",
"无数次的弃牌是为了一次ALL IN",
"在战役实施中，只有一个时机是最合适的，能抓住这个时机的，既是天才—拿破仑",
"游戏有它的规则，遵守规则是一个牌手的基本要求",
"中国的老话（在扑克的世界里也合用）：软的怕硬的，硬的怕不要命的",
"人生就像打牌，不要在意抓到多好的牌，关键是怎样打好一手烂牌",
"如果你在三十分钟还找不到桌子上的那条鱼，那你就是鱼",
"没必要花所有的筹码去验证对手是否偷鸡（万一不是你就完了）",
"没有一种胜利比战胜自己及自己的冲动情绪更伟大",
"底牌是好牌还是坏牌，是由发的五张公共牌决定的（AA在翻牌后不一定大）",
"不要启动精巧的机关来杀鱼，这不管用，精巧的机关是对付高手的",
"“进一步万丈深渊，退一步海阔天空。”这句话谁都知道，单丝没多少人能读懂…",
"不要坐在两个比你更有攻击性的对手中间（这就是所谓的夹心饼干）",
"识时务，明事理，知进退-----德州扑克何尝不是如此",
"作为攻击型玩家最大的好处就是，别人不知道你的底细，那些手紧的玩家就象一本打开的书",
"去赢该赢的，别输不该输的",
"不要只看到偷鸡成功的，还要看到有些为了偷鸡输光的",
"公共牌发的第四张牌叫转牌",
"输了后不要情绪化，带着情绪打牌只能让你输的更多",
"把德州币当做自己的私房钱，你还会随便ALL in吗？",
"你要创造一个手松、爱赌且很会玩牌的形象，而不是一个可以让人赢钱、不停盖牌的傻瓜形象",
"不要为了诈唬而诈唬",
"定力和眼力是德州扑克的两大法宝",
"保持住最适合自己的风格，一次失败就患得患失，将是自己最大的敌人",
"对于新手来说，不要随便ALL IN，请记住一句俗话“不见兔子不撒鹰”",
"不会从失败中找教训的人，他们的成功之路是遥远的——拿破仑",
"自己所下的注要对的起自己的底牌，即使底牌是对A，你的下注错误也会输掉一切",
"无限注德州扑克是一门非常刺激的游戏，千万不要用来赌博，不要将所有的财产压上去",
"唯一比凶猛的对手更可怕的，是你错误的打牌方法",
"做人要厚道，玩牌切勿贪，无妄无想才是王道",
"针对不同的对手要有不同的打法",
"冲动是魔鬼，心态好，运气自然来",
"当对手都ALL IN的时候，大多数人是失去理智赌运气，99%的人会输掉，但你要相信，那1%的可能性不会是你！",
"小盲右边的位置叫庄家（dealer)，庄家是翻牌后最后一个行动的人",
"不是无路可走的时候，最后不要起手就ALL IN",
"玩德州扑克不要只看自己的牌，还要想想对手有什么牌",
"如果发现手风不顺，不妨离开下喝杯茶再来",
"打牌要打出气势，没有胆量就没有产量",
"论成败人生豪迈，大不了从头再来",
"不要让无名的欲望控制你！尤其在你抽中想要的转牌或河牌",
"攻击，攻击，不断的攻击，一名优秀的德州扑克选手必然是善于玩弄别人心理的",
"当运气的大门向你关闭的时候，用你的技术从窗户爬进去",
"勇敢地加注吧，你会发现对手的牌并不像想象中的那么可怕",
"守不败之地，胜可赢之敌",
"谁若游戏人生，他就一事无成；谁不能主宰自己，便永远是一个奴隶",
"当你玩牌的时候，千万不要数钱；胜负决出后，你有足够的时间计算自己的输赢",
"竞标赛经常会有些免费报名的比赛，前几名可以获得德州币",
"公共牌发的第五张牌叫河牌",
"中高级场对手看牌停顿的时间能反映出一定的信息",
"冷静，冷静——还是冷静",
"运气是留给勇敢者的",
"手风不顺的时候，可以休息一下",
"扑克的世界只有唯一的目的，把别人钱赢到自己的口袋",
"当对方加注时一般表明他有一手好牌，这个时候你要小心了",
	};
	SetLabelString(m_pLabelLoading, szTips[rand()%(sizeof(szTips)/sizeof(szTips[0]))]);
}

void GameLoading::setTips(const std::string& szTips)
{
	//if (szTips.length()==0 ||szTips.compare(""))
	//{
	//	return ;
	//}
    SetLabelString(m_pLabelLoading, szTips);
}

bool PaymentLoading::init()
{
    if (!GameLayer::init(GameData::getInstance()->getGameWidth(), 103, Color4B(0,0,0,0), true, true))
    {
        return false;
    }

    m_pAniViewer = AnimateViewer::create("loading/flower.webp", true, 1/ 24.0f, 7, 5, 35);
    m_pAniViewer->setPosition(GameData::getInstance()->getGameWidth()/2, 68);
    this->addChild(m_pAniViewer);

    return true;
}

void PaymentLoading::setVisible(bool bVisible)
{
    bool isVisibleNow = isVisible();
    if (isVisibleNow != bVisible)
    {
        GameLayer::setVisible(bVisible);
        m_pAniViewer->setVisible(bVisible);		
    }
}

void PaymentLoading::onKeyEscPressed()
{
    GameLayer::setVisible(false);
    m_pAniViewer->setVisible(false);	
}