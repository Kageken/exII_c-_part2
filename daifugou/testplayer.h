/*
 *  testplayer.h
 *  PlayingCard
 *
 *  Created by 下薗 真一 on 09/04/12.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TESTPLAYER_H_
#define _TESTPLAYER_H_

#include <random>

#include "player.h"
#include "gamestatus.h"

/*
 * すこしルールを覚えたプレイヤーのクラス．
 * Player クラスを基底クラスとする継承クラスの例である
 */
class TestPlayer : public Player{
	/*
	 * 変更不可の部分は，変更しないので定義せずそのまま引き継ぐ．
	 *
	 * その他の部分も引き継がれるが，follow などプレーヤーの挙動を変える
	 * のに必要な関数は，上書きして再定義する．
	 * 自分で新しく作成するデータ構造や関数も，定義する．
	 */

public:
	// 拡張した派生クラスでオーバーライドする／できる関数
	TestPlayer(const char * myname = "TestPlayer") : Player(myname) {}
    virtual ~TestPlayer() { }

	virtual void ready(void);
	virtual bool follow(const GameStatus & gstat, CardSet & cards);
	virtual bool approve(const GameStatus & gstat);
	bool checkPair(int h[Card::RANK_JOKER+1]);

	void makeHistogram(int histo[Card::RANK_JOKER+1]);
};

#endif
