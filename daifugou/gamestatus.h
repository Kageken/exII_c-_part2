/*
 * GameStatus.h
 *
 *  Created on: 2015/05/07
 *      Author: sin
 */

#ifndef GAMESTATUS_H_
#define GAMESTATUS_H_

#include "card.h"
#include "cardset.h"

#include <string>

struct GameStatus {
public:
	static const int MAXIMUM_NUM_OF_PLAYERS = 8;
	static const int NO_MORE_PLAYERS = 99;

	CardSet pile;
	int numPlayers;
	std::string playerName[MAXIMUM_NUM_OF_PLAYERS];
	int numCards[MAXIMUM_NUM_OF_PLAYERS];
	bool playingOn[MAXIMUM_NUM_OF_PLAYERS];
	int numParticipants;
	int turnIndex;
	int leaderIndex;
	//
	bool revolution; // reserved.

	GameStatus(void) : pile(), numPlayers(0), numParticipants(0), turnIndex(0), leaderIndex(0),
			revolution(false) { }  // Empty instance

	std::ostream & printOn(std::ostream & out) const {
		out << "GameStatus(";
		out << "pile = " << pile;
		out << ", numPlayers = "<< numPlayers;
		out << ", turnIndex = " << turnIndex;
		out << ", leaderIndex = " << leaderIndex;
		out << ", playerName[] = {";
		for(int i = 0; i < numPlayers; i++) {
			out << playerName[i] << ", ";
		}
		out << "]";
		out << ", numCards[] = {";
		for(int i = 0; i < numPlayers; i++) {
			out << numCards[i] << ", ";
		}
		out << "]";
		out << ", numParticipants = ";
		out << numParticipants;

		out << std::endl;
		return out;
	}

	// おまけ
	friend std::ostream & operator<<(std::ostream& ostr, const GameStatus & stat) {
		return stat.printOn(ostr);
	}

};

#endif /* GAMESTATUS_H_ */
