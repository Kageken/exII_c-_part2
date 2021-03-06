/*
 *  testPlayer.cpp
 *  PlayingCard
 *
 *  Created by 下薗 真一 on 09/04/12.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>
#include <random>
#include <cstdlib>

#include "card.h"
#include "cardset.h"
#include "player.h"
#include "gamestatus.h"

#include "testplayer.h"

void TestPlayer::ready(void) {
	// 参考：強さでソートする
	hand.sort(daifugouAscending);
}

bool TestPlayer::follow(const GameStatus & gstat, CardSet & cards) {
	/*
	 *  cards はカラの状態で渡される．
	 */
	CardSet pile(gstat.pile); // リード（場札）を pile に取り出す
	int leadSize = pile.size(); // リードの組枚数
	int leadRank = 0; // リードのランク，pile がカラの場合 0
	int histo[Card::RANK_JOKER+1];
	std::random_device rdev;

	// リードの rank を得る
	if ( leadSize > 0 ) {
		pile.sort(daifugouAscending); // strength の昇順にする
		leadRank = pile.at(0).rank(); // リードの（あれば Joker 以外のカードの）rank
	}
	/*
	 *  リードと同じ枚数の組を出す．初心者なので Joker を組み合わせに使わない．
	 */

	std::srand(rdev());
	makeHistogram(histo); // strength ごとに数える
	int myStrength, temp;
	temp = 0;
	for(myStrength = strength(leadRank) + 1 ; myStrength < Card::RANK_JOKER + 1 ; ++myStrength) {
		if ( histo[myStrength] == 0 )
			continue;
		if ( leadSize == 0 ) {
			break;
		}
		if ( histo[myStrength] < leadSize ) {
			continue;
		}
		if ( histo[myStrength] == leadSize ) {
		        break;
		}
		if ( histo[myStrength] >= leadSize ) {
			break;
		}
	}
	// myStrength == 16 はパス
	if ( myStrength < Card::RANK_JOKER + 1 ) {
		// 有効なカード
		/* カードを draw で取り出す．*/
		Card tmp;
		for(int i = 0; i < hand.size(); ) {
			if ( strength(hand.at(i).rank()) == myStrength ) {
				hand.draw(tmp, i);
				cards.insert(tmp);
				// draw した場合はのこりが前詰めになるので i はそのまま
			} else {
				++i;
			}
			if ( (leadSize > 0) && cards.size() == leadSize )
				break;
		}
	}
	/*
	 * 出したカードは手札からなくなる．
	 * reject された場合は，Dealer により手札にもどされる．
	 */
	return true;
}

// strength 値でヒストグラムを作る
void TestPlayer::makeHistogram(int h[Card::RANK_JOKER+1]) {
	for(int i = 0; i <= Card::RANK_JOKER; ++i)
		h[i] = 0;
	for(int i = 0; i < hand.size(); ++i) {
		h[strength(hand.at(i))]++;
	}
}


bool TestPlayer::approve(const GameStatus & gstat) {
	// やはり自分の手札しかみえていない
  return true;
}


bool TestPlayer::checkPair(int h[Card::RANK_JOKER+1]) {
  for(int i = 0; i <= Card::RANK_JOKER; ++i) {
    if(h[i] >= 2){
      return true;
    }
  }
  return false;
}
