/*
 *  think_ta1.h
 *  PlayingCard
 *
 *  Created by TA (Ryosuke Tadano) @ 2009
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _THINK_TA1_H_
#define _THINK_TA1_H_

#include "player.h"

class ThinkTA1 : public Player {

public:
	ThinkTA1(const char * name = "TA") : Player(name) {}

	virtual bool follow(const GameStatus & gstat, CardSet & cards);

	bool sort(CardSet&);
};

#endif
