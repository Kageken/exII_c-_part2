/*
 * cheatingplayer.cpp
 *
 *  Created on: 2017/05/22
 *      Author: sin
 */

#include "cheatingplayer.h"

void CheatingPlayer::ready(void) {
	Card t;
	hand.sort(Player::daifugouAscending);
	if ( (cheat & CheatType::INSERT_JOKER) != 0) {
		Card t;
		if ( ! hand.includes(Card::RANK_JOKER)) {
			hand.draw(t,0);
			t.set(Card::SUIT_JOKER, Card::RANK_JOKER);
			hand.insert(t);
		}
	}
	if ( (cheat & CheatType::DROP_CARD )!= 0 ) {
		hand.draw(t,0);
	}
}

bool CheatingPlayer::follow(const GameStatus & gstat, CardSet & cards) {
	return Player::follow(gstat, cards);
}

bool CheatingPlayer::approve(const GameStatus & gstat) {

	return true;
}

