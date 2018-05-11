#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>

#include "card.h"
#include "cardset.h"
#include "dealer.h"

#include "player.h"
#include "simpleplayer.h"
#include "testplayer.h"

#include "think_ta1.h"
/*
#include "Group1.h"
#include "Group2.h"
#include "Group3.h"
#include "group4.h"
#include "Group5.h"
#include "kaiji.h"
#include "Group7.h" // AkiPlayer.h
#include "Team8.h"
*/

bool rankComp(std::pair<std::string,float> a, std::pair<std::string,float> b) {
	return (a.second < b.second);
}

std::vector<Player *> players;

void registerPlayers(Dealer & d) {
	players.clear();
	players.push_back(new SimplePlayer("Somy"));
	players.push_back(new TestPlayer("Sharpo"));
  	players.push_back(new ThinkTA1("Appollo"));
  	players.push_back(new ThinkTA1("Alphabeta"));
	players.push_back(new ThinkTA1("ex-TA"));

	d.regist(players[0]);
	d.regist(players[1]);
	d.regist(players[2]);
	d.regist(players[3]);
	d.regist(players[4]);
}

void play1Game(Dealer & dealer, const bool prompt = true) {
//	bool passed = true;
	CardSet opened;
	std::string tmpstr;

//	dealer.newGame();
	dealer.deal();

	std::cout << std::endl << "start. " << std::endl;
	std::cout << "  -------- " << std::endl;
	dealer.show();
	std::cout << "  -------- " << std::endl;

	while (true) {
		if ( dealer.playerPassed() && dealer.playerInTurnIsLeader() ) {
			dealer.clearDiscarded();

			std::cout << std::endl << "flushed. " << std::endl;
			std::cout << "  -------- " << std::endl;
			dealer.show();
			std::cout << "  -------- " << std::endl;
		}
		if ( prompt )
			std::getline(std::cin, tmpstr);

		std::cout << dealer.playerInTurn().playerName() << "'s turn: ";
		GameStatus state = dealer.gameStatus();
		opened.clear();
		dealer.playerInTurn().follow(state, opened);
		std::cout << opened << std::flush;

		if ( dealer.discardPile().size() == 0 && opened.size() == 0 && dealer.playerInTurnIsLeader() ) {
			// リーダーがパスした．．．
			std::cout << "leader passed." << std::endl;
			dealer.reject(opened);
			dealer.resignPlayerInTurn();
			std::cout << dealer.playerInTurn().playerName() << " resigned. " << std::endl << std::endl;
		} else {
			if ( dealer.acceptCheck(opened) ) {
				dealer.accept(opened);
				std::cout << "accepted." << std::endl;
			} else {
				if ( !opened.isEmpty() )
					std::cout << "rejected, ";
				std::cout << "pass." << std::endl;
				dealer.reject(opened);
			}
			if ( !dealer.playerPassed() ) {
				dealer.setAsLeader();
				std::cout << "--- Lead by " << dealer.currentLeader().playerName() << ", "
						<< dealer.discardPile() << std::endl;
			}
			dealer.showDiscardedToPlayers();

			if ( dealer.playerInTurn().isEmptyHanded() ) {
				// あがり
				std::cout << dealer.playerInTurn().playerName() << " finished. " << std::endl << std::endl;
				dealer.finishPlayerInTurn();
				if ( dealer.howManyPlayingPlayers() <= 1 ) {
					// どべ確定
					dealer.finishPlayerInTurn();
					std::cout << "  -------- " << std::endl;
					dealer.show();
					std::cout << "  -------- " << std::endl;
					break;
				}
			} else {
				dealer.nextPlayer();
			}
		}
	}
	return;
}

int main (int argc, char * const argv[]) {
	
	Dealer dealer;
	int game = 0, times = 0;
	std::map<std::string,float> pointTable;
	std::string resp, tmpstr;
	bool prompt = true;

	if ( argc >= 2 )
		times = atoi(argv[1]);
	if ( argc >= 3 ) {
		resp = argv[2];
	}
	if ( times <= 0 )
		times = 5;

	std::cout << "Registering players." << std::endl;

	registerPlayers(dealer);

	game = 0;
	while (game < times) {
		std::cout << std::endl << "Game " << (game+1) << "." << std::endl;

		if ( (game % 100) == 0 ) // shuffle order per 100 time
			dealer.shuffleOrder();

		play1Game(dealer);

		std::cout << std::endl << "This game's result: " << std::endl;
		for (unsigned int i = 0; i < dealer.howManyParticipants() ; i++) {
			std::cout << i+1 << ": "  << dealer.finishedAt(i) << "\t" << " " << std::endl;
			//
			if ( i == 0 ) {
				pointTable[dealer.finishedAt(i)] += 0.9;
			} else {
				pointTable[dealer.finishedAt(i)] += i + 1;
			}
		}
		if ( prompt )
			std::getline(std::cin, tmpstr);

		dealer.reOrder();

		game++;
	}

	std::vector<std::pair<std::string,float>> list(pointTable.begin(),pointTable.end());
	std::sort(list.begin(), list.end(), rankComp);
	std::cout << std::endl << "Final result: " << std::endl;
	for(std::vector<std::pair<std::string,float>>::iterator itr = list.begin();
			itr != list.end(); ++itr) {
		std::cout << itr->first << ": " << itr->second << std::endl;
	}
	std::cout << std::endl;

    return 0;
}
