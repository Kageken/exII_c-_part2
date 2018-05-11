/*
 * cardset_pickup.cpp
 *
 *  Created on: 2015/04/16
 *      Author: sin
 */

#include <cstdlib>

#include "cardset.h"
#include "card.h"

bool CardSet::draw(Card & card, int pos) {
	if( pos == -1 )
		pos = std::rand() % count;
	else if( count == 0 || !(pos < count) )
		return false;

	card = cards[pos];
	return remove(card);
}
