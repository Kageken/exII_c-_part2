/*
 *  Player.h
 *  PlayingCard
 *
 *  Created by 下薗 真一 on 09/04/12.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "gamestatus.h"

/*
 * プレイヤークラスの基底クラス．
 * Player を public の基底クラスとし，グループのプレイヤーを拡張クラスとして
 * 実装する．
 * 他のグループのプレイヤークラス，大会プログラムと互換性がなくなりコンパイルできなく
 * なる可能性があるので，Player クラスの定義と変更不可の関数等のソースコードは，変更しないこと．
 */
class Player {
	/* 変更不可の部分ここから */
private:
	std::string name;

protected:
	CardSet hand;

public:
	bool takeCards(CardSet &);
	bool takeCards(Card &);

	void clearHand();
	bool isEmptyHanded() const;
	//bool insert(const Card &);
	CardSet & inHand() { return hand; }
	int size() const { return hand.size(); }
	const std::string & playerName() const { return name; }

	std::ostream & printOn(std::ostream & out) const;

	/* 変更不可の部分ここまで */

public:
	/*
	 * ユーティリティ関数
	 *
	 * 大富豪ゲームでの rank を強さ int に読み替える関数．
	 * 大きいほど強い．
	 */
	static int strength(const int rank);
	static int strength(const Card & card) { return strength(card.rank()); }
	/*
	 * 大富豪ゲームでの手札の強さにもとづく順序関係
	 * 論理値 one < another を返す
	 * Card::cardComparator 関数ポインタと同じ入出力型なので
	 * CardSet::sort で使用できる
	 */
	static bool daifugouAscending(const Card & one, const Card & another) {
		return strength(one.rank()) < strength(another.rank());
	}

public:
	// 拡張した派生クラスでオーバーライドする／できる関数
	Player(const char * myname);
    virtual ~Player() { }

	virtual void ready(void);
	virtual bool follow(const GameStatus & gstat, CardSet & cards);
	virtual bool approve(const GameStatus & gstat);
};

#endif
