/*
 * CheatingPlayer.h
 *
 *  Created on: 2017/05/22
 *      Author: sin
 */

#ifndef CHEATINGPLAYER_H_
#define CHEATINGPLAYER_H_

#include "player.h"
#include "gamestatus.h"

class CheatingPlayer : public Player {
//definition
public:
	enum CheatType {
		DROP_CARD = 1,
		SWAP_CARD = 1<<1,
		INSERT_JOKER = 1<<2,
		NONE = 0,
	};
//member
private:
	CheatType cheat;

public:
	CheatingPlayer(const char * myname = "SimplePlayer", const CheatType ch = NONE) : Player(myname), cheat(ch) {}

	virtual void ready(void);
	virtual bool follow(const GameStatus & gstat, CardSet & cards);
	virtual bool approve(const GameStatus & gstat);
};


#endif /* CHEATINGPLAYER_H_ */
