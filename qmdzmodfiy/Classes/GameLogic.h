#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#include "GameCommon.h"

#define FULL_COUNT					    52									//全牌数目
#define MAX_COUNT						2									//最大数目
#define MAX_CENTERCOUNT					5									//最大数目

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//扑克类型
#define CT_SINGLE					1									//单牌类型
#define CT_ONE_LONG					2									//对子类型
#define CT_TWO_LONG					3									//两对类型
#define CT_THREE_TIAO				4									//三条类型
#define	CT_SHUN_ZI					5									//顺子类型
#define CT_TONG_HUA					6									//同花类型
#define CT_HU_LU					7									//葫芦类型
#define CT_TIE_ZHI					8									//铁支类型
#define CT_TONG_HUA_SHUN			9									//同花顺型
#define CT_KING_TONG_HUA_SHUN		10									//皇家同花顺

//胜利信息结构
struct UserWinList
{
	unsigned char bSameCount;
	unsigned short wWinerList[GAME_PLAYER];
};

//分析结构
struct tagAnalyseResult
{
	unsigned char cbFourCount;						//四张数目
	unsigned char cbThreeCount;						//三张数目
	unsigned char cbLONGCount;						//两张数目
	unsigned char cbSignedCount;					//单张数目
	unsigned char cbFourLogicVolue[1];				//四张列表
	unsigned char cbThreeLogicVolue[1];				//三张列表
	unsigned char cbLONGLogicVolue[2];				//两张列表
	unsigned char cbSignedLogicVolue[5];			//单张列表
	unsigned char cbFourCardData[MAX_CENTERCOUNT];	//四张列表
	unsigned char cbThreeCardData[MAX_CENTERCOUNT];	//三张列表
	unsigned char cbLONGCardData[MAX_CENTERCOUNT];	//两张列表
	unsigned char cbSignedCardData[MAX_CENTERCOUNT];//单张数目
};

class CGameLogic
{
public:
	CGameLogic();
	virtual ~CGameLogic();

private:
	static unsigned char m_cbCardData[FULL_COUNT];				//扑克定义

public:
	void SortCardList(unsigned char cbCardData[], unsigned char cbCardCount);
	void RandCardList(unsigned char cbCardBuffer[], unsigned char cbBufferCount);

public:
	unsigned char GetCardType(unsigned char cbCardData[], unsigned char cbCardCount);
	unsigned char GetCardValue(unsigned char cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	unsigned char GetCardColor(unsigned char cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }

public:
	unsigned char GetCardLogicValue(unsigned char cbCardData);
	unsigned char CompareCard(unsigned char cbFirstData[], unsigned char cbNextData[], unsigned char cbCardCount);
	void AnalysebCardData(const unsigned char cbCardData[], unsigned char cbCardCount, tagAnalyseResult & AnalyseResult);
	unsigned char FiveFromSeven(const unsigned char cbHandCardData[],unsigned char cbHandCardCount,const unsigned char cbCenterCardData[],unsigned char cbCenterCardCount,unsigned char cbLastCardData[],unsigned char cbLastCardCount);
	unsigned char FiveFromSix(const unsigned char cbHandCardData[],unsigned char cbHandCardCount,const unsigned char cbCenterCardData[],unsigned char cbCenterCardCount,unsigned char cbLastCardData[],unsigned char cbLastCardCount);
	//bool SelectMaxUser(unsigned char bCardData[GAME_PLAYER][MAX_CENTERCOUNT], UserWinList &EndResult, const __int64 lAddScore[]);

	enum TwoCardType
	{
		Single,
		DuiZi,
		TongHua,
	};

	enum CardValueScope
	{
		CardValue2To5, 
		CardValue5To10,
		CardValue10ToA
	};

	struct TwoCardResult
	{
		unsigned char cbCardType;
		CardValueScope smallCardValueScope;
		CardValueScope bigCardValueScope;
	};
	CardValueScope GetCardValueScope(unsigned char cbCardValue);
	TwoCardResult AnalyseTwoCard(const unsigned char cbCard[2]);
};

#endif