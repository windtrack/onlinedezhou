#ifndef __GAMERECORDLIST_H__
#define __GAMERECORDLIST_H__

#include "GameDialog.h"

struct GameRecordInfo
{
	unsigned m_uRecordID;
	unsigned m_uHandCard[2];
	std::string m_strRoomName;
	unsigned m_uMaxPlayer;
	double m_dLittleBlind;
	double m_dBigBlind;
	double m_dWin;
	time_t m_gameTime;
};

class GameRecordList : public GameDialog
{
public:
	CREATE_FUNC(GameRecordList);
	void AddRecordInfo(const GameRecordInfo& gameRecordInfo);

protected:
	GameRecordList();
	virtual ~GameRecordList();

	bool init();

private:

};

#endif