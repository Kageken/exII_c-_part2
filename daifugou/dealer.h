/*
 *  Dealer.h
 *  PlayingCard
 *
 *  Created by 下薗 真一 on 09/04/12.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _DEALER_H_
#define _DEALER_H_

#include <vector>
#include <map>

#include "player.h"
#include "gamestatus.h"

class Dealer {
public:
	const static int NUM_OF_ALL_CARDS = 53;
	const static int NUM_OF_MAX_PLAYERS = 10;

protected:
	CardSet theDeck;
	CardSet discarded;
	int turnIndex;
	int leaderIndex;
	bool turnPassed;

	unsigned int numParticipants;

	// 全プレーヤー（参加者）．手札がない場合，あがり済み，ギブアップ済み，またはゲームに不参加
	Player * participant[NUM_OF_MAX_PLAYERS];

	// 複数ゲームを通してのプレーヤーの得点
	std::map<Player *, float> stats;
	// あがったプレーヤーの添字，ギブアップしたプレーヤーの participant での添字
	std::vector<int> finishedOrder, resignedOrder;

	// Rule flags
	bool noMillionaire;
	bool noMiyakoOchi;

	unsigned long hashval;

public:
	Dealer(void);
	virtual ~Dealer();

	static bool checkRankUniqueness(const CardSet &);
	static int getCardStrength(const CardSet &);

//	void newGame();
	bool regist(Player *);
	bool deal();
	void show();
	
	unsigned int howManyPlayingPlayers() const;
	unsigned int howManyFinishedPlayers() const;
	unsigned int howManyParticipants() const { return numParticipants; }

	void showDiscardedToPlayers();
	
	const bool acceptCheck(const CardSet & opened) const;
	void accept( CardSet & opened);
	void reject( CardSet & opened);
	const bool lastPlayerPassed() const;
	const int turn() const { return turnIndex; }

	const CardSet & discardPile() const;
	void clearDiscarded();
	bool playerInTurnIsLeader() const;
	void setAsLeader(void);
	bool playerPassed() const { return turnPassed; }

	Player & player(const int ith) const;   // 現ゲームのならびで ith の player
	Player & player(const Player * p) const;   // ポインタをIDとしてみた player
	Player & player(const std::string & name);
	
	Player & playerInTurn();
	Player & nextPlayer();
	const Player & currentLeader() const;
	Player & finishPlayerInTurn();
	Player & resignPlayerInTurn();
	
	GameStatus gameStatus(void) const;

	const std::string & finishedAt(int i) const;

	void shuffleOrder(void);
	void reOrder(void);

};

#endif
