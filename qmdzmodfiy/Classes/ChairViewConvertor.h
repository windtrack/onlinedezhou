#ifndef __CHAIRVIEWCONVERTOR_H__
#define __CHAIRVIEWCONVERTOR_H__

class ChairViewConvertor
{
public:
	void Init(unsigned uMaxPlayer, unsigned uMyChairID, unsigned uMyViewChairID)
	{
		m_uMaxPlayer = uMaxPlayer;
		m_uMyChairID = uMyChairID;
		m_uMyViewChairID = uMyViewChairID;
	}

	unsigned GetMyChairID()
	{
		return m_uMyChairID;
	}
	unsigned GetMyViewChairID()
	{
		return m_uMyViewChairID;
	}
	unsigned GetViewChairID(unsigned uChairID)
	{
        unsigned uView = 8 - (((uChairID + 9) - m_uMyChairID + 4) % 9);

        if (m_uMaxPlayer == 5)
        {
            if (uView > GetMyViewChairID())
            {
                unsigned uOff = uView - GetMyViewChairID();
                static unsigned s_TargetV[] = {4, 5, 8, 0, 3};
                uView = s_TargetV[uOff];
            }
            else if (uView < GetMyViewChairID())
            {
                unsigned uOff = GetMyViewChairID() - uView;
                static unsigned s_TargetV[] = {4, 3, 0, 8, 5};
                uView = s_TargetV[uOff];
            }
        }
        return uView;
		//return m_uMaxPlayer - 1 - (((uChairID + m_uMaxPlayer) - m_uMyChairID + m_uMyViewChairID) % m_uMaxPlayer);
	}
	unsigned GetChairID(unsigned uViewChairID)
	{
        if (m_uMaxPlayer == 5)
        {
	        if (uViewChairID == 0)
	        {
		        uViewChairID = GetMyViewChairID() - 2;
	        }
	        else if (uViewChairID == 8)
	        {
		        uViewChairID = GetMyViewChairID() + 2;
	        }
        }
	    return (m_uMyChairID + 9 - (uViewChairID + 9 - GetMyViewChairID()) % 9) % 9;
		//return (m_uMyChairID + m_uMaxPlayer - (uViewChairID + m_uMaxPlayer - m_uMyViewChairID) % m_uMaxPlayer) % m_uMaxPlayer;
	}

private:
	unsigned m_uMaxPlayer;
	unsigned m_uMyChairID;
	unsigned m_uMyViewChairID;
};


#endif