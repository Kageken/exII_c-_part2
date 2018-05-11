/*
 *  dealer.cpp
 *  PlayingCard
 *
 *  Created by 下薗 真一 on 09/04/12.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#include <iostream> 
#include <iomanip>
#include <string>
#include <random>

#include <cstdlib>
#include <ctime>

#include "card.h"
#include "cardset.h"
#include "player.h"
#include "dealer.h"
#include "gamestatus.h"

// class methods.

Dealer::Dealer() {
	finishedOrder.clear();
	resignedOrder.clear();
	turnIndex = 0;
	turnPassed = false;
	numParticipants = 0;
	leaderIndex = 0;
	// set rules
	noMillionaire = true;
	noMiyakoOchi = true;
	//
	stats.clear();

	std::random_device randev;
	hashval = (unsigned long) randev();

	return;
}

Dealer::~Dealer(void) {
	for(unsigned int i = 0; i < numParticipants; i++) {
		delete(participant[i]);
	}
}


Player & Dealer::player(const int ith) const {
	return * participant[ith];
}


Player & Dealer::player(const std::string & name) {
	for(unsigned int i = 0; i < numParticipants; ++i) {
		if ( participant[i]->playerName() == name )
			return *participant[i];
	}
	return *participant[numParticipants];
}
/*
Player * Dealer::player(const int id) {
	for(unsigned int i = 0; i < numParticipants; ++i) {
		if ( participant[i].entryID == id )
			return participant[i].player;
	}
	return NULL;
}
*/
// the number of playing, not finished, players
unsigned int Dealer::howManyPlayingPlayers() const {
	unsigned int count = 0;
	for(unsigned int i = 0; i < numParticipants; ++i) {
		if ( !participant[i]->isEmptyHanded() )
			count++;
	}
	return count;
}


bool Dealer::checkRankUniqueness(const CardSet & cs) {
	int rank = 0;

	if (cs.size() == 0)
		return false;

	if ( cs.size() == 1 && cs.at(0).isJoker() )
		return true;

	for (int i = 0; i < cs.size(); i++) {
	  if (cs.at(i).isJoker() )
		  continue;  // Jkrをスキップ
	  if ( rank == 0 ) {
		  rank = cs.at(i).rank();
	  } else if ( rank != cs.at(i).rank() ) {
	    return false;
	  }
	}
	return true;
}


// instance methods.
bool Dealer::regist(Player * pl) {
	if ( !(numParticipants < NUM_OF_MAX_PLAYERS) ) {
		std::cerr << "Error: tried player registration exceeding the limit." << std::endl;
		return false;
	}
	participant[numParticipants] = pl;

	stats[pl] = 0.0;
	numParticipants++;
	return true;
}

const Player & Dealer::currentLeader() const {
	return * participant[leaderIndex];
}

bool Dealer::playerInTurnIsLeader() const {
	return leaderIndex == turnIndex;
}

/*
void Dealer::newGame() {
	for (unsigned int i = 0; i < numParticipants; i++) {
		participant[i]->clearHand();
		participant[i]->ready();
	}
}
*/

void Dealer::shuffleOrder(void) {
	int dst;
	Player * t;
	std::random_device randev;
	std::mt19937 dice(randev());

	for(int i = 0; i < 7; i++) {
		for (int src = 0; src < numParticipants; ++src) {
			dst = dice() % numParticipants;
			if ( src == dst ) continue;
			t = participant[src];
			participant[src] = participant[dst];
			participant[dst] = t;
		}
	}
	return;
}

void Dealer::reOrder(void) {
	Player * newplayorder[NUM_OF_MAX_PLAYERS];
	if ( finishedOrder.size() + resignedOrder.size() != numParticipants ) {
		shuffleOrder();
		return;
	}
	for(unsigned int i = 0; i < finishedOrder.size(); ++i) {
		newplayorder[i] = participant[finishedOrder[i]];
	}
	for(unsigned int i = 0; i < resignedOrder.size(); ++i) {
		newplayorder[numParticipants - i - 1] = participant[resignedOrder[i]];
	}
	for(unsigned int i = 0; i < numParticipants; ++i)
		participant[i] = newplayorder[numParticipants - 1 - i]; // どべから順に
	return;
}

void Dealer::setAsLeader(void) {
	leaderIndex = turnIndex;
}

bool Dealer::deal(void) {
	Card top;

	for (unsigned int i = 0; i < numParticipants ; i++) {
		player(i).clearHand();
	}
	theDeck.setupDeck();
	theDeck.shuffle();
	int index = 0;
	while ( !theDeck.isEmpty() ) {
		theDeck.draw(top, 0);
		player(index).takeCards(top);
		index = (index + 1) % numParticipants;
	}
	for(unsigned int i = 0; i < numParticipants; i++) {
		player(i).ready();
	}

	clearDiscarded();
	turnIndex = 0;
	leaderIndex = 0;
	turnPassed = false;
	finishedOrder.clear();
	resignedOrder.clear();

	return true;
}

void Dealer::clearDiscarded() {
	discarded.clear();
}

const CardSet & Dealer::discardPile() const {
	return discarded;
}


const bool Dealer::acceptCheck(const CardSet & opened) const {
	int openedRank, discardedRank;

	if (opened.isEmpty() )
		return false;  // regard the empty open set as "pass"

	if (!checkRankUniqueness(opened))
		return false;
	if ( opened.size() >= 5 )
		return false;

	if ( discarded.isEmpty() )
		return true;

	if ( discarded.size() != opened.size() )  // the number of cards must be match. no five cards w/ Jkr allowed.
	  return false;
	
	openedRank = getCardStrength(opened);
	discardedRank = getCardStrength(discarded);

	if ( openedRank > discardedRank )
	    return true;
	return false;
}

void Dealer::accept(CardSet & opened) {
	// passed all the checks.
	discarded.clear();
	for(int i = 0; i < opened.size(); i++)
		discarded.insert(opened.at(i));
	turnPassed = false;
}

void Dealer::reject(CardSet & opened) {
	if ( !opened.isEmpty() )
		playerInTurn().takeCards(opened);
	turnPassed = true;
	// leave as untouched the shadow set
}

int Dealer::getCardStrength(const CardSet & cs) {
	int i;
	if ( cs.isEmpty() )
		return 0;

	if ( cs.size() == 1 && cs.at(0).isJoker() ) {
		return 53;
	}
  	for (i = 0; i < cs.size(); i++) {
	  if (!cs.at(i).isJoker()) {
		  break;
	  }
	}
  	if ( cs.at(i).rank() < 3 )
  		return cs.at(i).rank() + 13;
	return cs.at(i).rank();
}

void Dealer::showDiscardedToPlayers() {
	GameStatus gstat = gameStatus();
	if ( turnPassed )
		gstat.pile.clear();
	for (unsigned int i = 1; i < numParticipants; i++) {
		if ( participant[ (turnIndex + i) % numParticipants ]->size() > 0 )
			participant[ (turnIndex + i) % numParticipants ]->approve(gstat);
	}
	return;
}

Player & Dealer::finishPlayerInTurn() {
	Player & p = playerInTurn();
	const unsigned int finishedIndex = turnIndex;
	// finished leader leaves the game,
	// thus the next player is considered as temporarily the leader

	participant[finishedIndex]->clearHand(); // ensure that leaving player holding no cards
	finishedOrder.push_back(finishedIndex);
	if ( howManyPlayingPlayers() > 0 ) {
		nextPlayer();
		setAsLeader();
	}
	return p;
}

Player & Dealer::resignPlayerInTurn() {
	Player & p = playerInTurn();
	const unsigned int finishedIndex = turnIndex;
	// finished leader leaves the game,
	// thus the next player is considered as temporarily the leader

	participant[finishedIndex]->clearHand(); // ensure that leaving player holding no cards
	resignedOrder.push_back(finishedIndex);
	if ( howManyPlayingPlayers() > 0 ) {
		nextPlayer();
		setAsLeader();
	}
	return p;
}

unsigned int Dealer::howManyFinishedPlayers() const {
	return finishedOrder.size() + resignedOrder.size();
}

Player & Dealer::playerInTurn(void) {
	return *participant[turnIndex];
}

Player & Dealer::nextPlayer() {
//	std::cout << "searching for the next player..." << std::endl;
	do {
		turnIndex = (turnIndex+1) % numParticipants;
	} while ( playerInTurn().size() == 0);
	return playerInTurn();
}

const std::string & Dealer::finishedAt(int place) const {
	return participant[finishedOrder[place]]->playerName();
}

void Dealer::show() {
	for (unsigned int i = 0; i < numParticipants ; i++) {
		if ( i == (unsigned int) leaderIndex )
			std::cout << "* ";
		else std::cout << "  ";
		std::cout << std::left << std::setw(8) << participant[i]->playerName()
				<< " " << participant[i]->inHand() << std::endl;
	}
}


GameStatus Dealer::gameStatus(void) const {
	GameStatus gstat;
	gstat.pile = discarded;
	gstat.turnIndex = turnIndex;
	gstat.leaderIndex = leaderIndex;
	gstat.numPlayers = howManyPlayingPlayers();
	for (int i = 0; i < gstat.numPlayers; i++)
		gstat.playerName[i] = participant[i]->playerName();
	for (int i = 0; i < gstat.numPlayers; i++)
		gstat.numCards[i] = player(i).size();
	gstat.numCards[gstat.numPlayers] = GameStatus::NO_MORE_PLAYERS; // for backward compatibility
	return gstat;
}
