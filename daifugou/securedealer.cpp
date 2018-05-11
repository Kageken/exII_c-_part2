/*
 * securedealer.cpp
 *
 *  Created on: 2017/05/12
 *      Author: sin
 */

#include "securedealer.h"

SecureDealer::~SecureDealer(void) {
	for(unsigned int i = 0; i < numParticipants; i++) {
		delete(participant[i]);
	}
}


bool SecureDealer::deal(void) {
	Card top;

	for (unsigned int i = 0; i < numParticipants ; i++) {
		player(i).clearHand();
		//
		shadow[i].clear();
	}
	theDeck.setupDeck();
	theDeck.shuffle();
	int index = 0;
	while ( !theDeck.isEmpty() ) {
		theDeck.draw(top, 0);
		player(index).takeCards(top);
		//
		index = (index + 1) % numParticipants;
	}

	for(unsigned int i = 0; i < numParticipants; i++) {
		for(int ci = 0; ci < player(i).inHand().size(); ++ci) {
			shadow[i].insert(player(i).inHand()[ci]);
		}
	}

	for(unsigned int i = 0; i < numParticipants; i++)
		player(i).ready();

	turnIndex = 0;
	leaderIndex = 0;
	turnPassed = false;

	return true;
}

void SecureDealer::accept(CardSet & opened) {
	// passed all the checks.
	discarded.clear();
	for(int i = 0; i < opened.size(); i++)
		discarded.insert(opened.at(i));
	turnPassed = false;
	//
	for(int i = 0; i < opened.size(); i++)
		shadow[turnIndex].remove(opened[i]);

}


bool SecureDealer::verifyHand(const int id) const {
	return shadow[id].equals( participant[id]->inHand() );
}

bool SecureDealer::verifyPlayers() const {
	bool result = true;
	for(unsigned int i = 0; i < numParticipants; ++i) {
		if ( !verifyHand(i) ) {
			std::cout << "Found ikasama by " << participant[i]->playerName() << std::endl;
			result = false;
		}
	}
	return result;
}
