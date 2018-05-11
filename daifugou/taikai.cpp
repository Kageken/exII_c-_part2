#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>

#include "card.h"
#include "cardset.h"
#include "securedealer.h"

#include "player.h"
#include "cheatingplayer.h"
#include "simpleplayer.h"
#include "think_ta1.h"


#include "Group1.h"
#include "Group2.h"
#include "Group3.h"
#include "group4.h"
#include "Group5.h"
#include "kaiji.h"
#include "Group7.h" // AkiPlayer.h


bool rankComp(std::pair<std::string,float> a, std::pair<std::string,float> b) {
	return (a.second < b.second);
}

void registerPlayers(SecureDealer & d) {
	d.regist(new Group1("Group 1"));
	d.regist(new Group2("Group 2"));
	d.regist(new Group3("Group 3"));
	d.regist(new Group4("Group 4"));
	d.regist(new Group5("Group 5"));
	d.regist(new kaiji("Group 6"));
	d.regist(new AkiPlayer("Group 7"));
	d.regist(new ThinkTA1("TA One"));
}

int main (int argc, char * const argv[]) {
	
	SecureDealer dealer;
	int game = 0, times = 0;
	std::map<std::string,float> pointTable;
	std::string resp1, resp2;

	if ( argc >= 2 )
		times = atoi(argv[1]);
	if ( argc >= 3 ) {
		resp1 = argv[2];
	}
	if ( argc >= 4 ) {
		resp2 = argv[3];
	}
	if ( times <= 0 )
		times = 5;

	std::cout << "Registering players." << std::endl;

	registerPlayers(dealer);

	game = 0;
	while (game < times) {
		std::cout << std::endl << "Starting Game " << (game+1) << "." << std::endl;

		if ( (game % 100) == 0 ) // shuffle order per 100 time
			dealer.shuffleOrder();

//		dealer.newGame();
		dealer.deal();

		bool passed = true;
		CardSet opened;
		while (true) {
			while (true) {
				if (passed && dealer.playerInTurnIsLeader() ) {
					dealer.clearDiscarded();
					std::cout << std::endl << "flushed. " << std::endl;
					std::cout << "  -------- " << std::endl;
					dealer.show();
					std::cout << "  -------- " << std::endl;
					if ( resp1.length() == 0 )
						std::getline(std::cin, resp1);
				}
				std::cout << std::endl << dealer.playerInTurn().playerName() << "'s turn: ";
				GameStatus state = dealer.gameStatus();
				opened.clear();
				dealer.playerInTurn().follow(state, opened);
				std::cout << opened << std::flush;
				if ( dealer.acceptCheck(opened) ) {
					std::cout << "accepted." << std::endl;
					dealer.accept(opened);
					passed = false;
				} else {
					if ( !opened.isEmpty() )
						std::cout << "rejected, ";
					std::cout << "pass." << std::endl;
					dealer.reject(opened);
					passed = true;
				}
				if ( !passed ) {
					dealer.setAsLeader();
					std::cout << "--- Lead " << dealer.discardPile() << " by "
							<< dealer.currentLeader().playerName() << ". " << std::endl;
					// std::cin.getline(dummy,31);
				}
				std::cout << std::endl;
				dealer.showDiscardedToPlayers();
				dealer.verifyPlayers();
				if ( dealer.playerInTurn().isEmptyHanded() )
					break; // finished
				dealer.nextPlayer();
			}
			std::cout << dealer.playerInTurn().playerName() << " finished. " << std::endl << std::endl;
			dealer.finishPlayerInTurn();
			if ( dealer.howManyPlayingPlayers() == 1 ) {
				// どべ確定
				dealer.finishPlayerInTurn();
				break;
			}
		}

		std::cout << std::endl 
			<< "This game's result: " << std::endl;
		for (unsigned int i = 0; i < dealer.howManyParticipants() ; i++) {
			std::cout << i+1 << ": "  << dealer.finishedAt(i) << "\t" << " " << std::endl;
			//
			if ( i == 0 ) {
				pointTable[dealer.finishedAt(i)] += 0.9;
			} else {
				pointTable[dealer.finishedAt(i)] += i + 1;
			}
		}

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
