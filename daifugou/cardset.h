//
// CardSet.h - トランプカードの集合型(C++版)
//	作者: (あなたの名前); 日付: (完成した日付)
//
#ifndef CARDSET_H
#define CARDSET_H

#include <iostream>
#include <cstdlib>

#include "card.h"

//
// CardSet - トランプカードの集合型
//
class CardSet {
// 定義・宣言
public:
	const static int maxnumcard = 53;	// カード総数

	// メンバ変数
private:
	int count;		// 現在の集合内のカード数
	Card cards[maxnumcard];	// カードのデータ


	// target に等しいカードの cards 中の格納位置を返す( == count: ない)
	int find(const Card & target) const;
	// rank が num に等しいカードの格納位置を返す(== count: ない)
	int find(const int num) const;

public:
	// デフォルトコンストラクタ(初期値空集合)
	CardSet();

	CardSet(const CardSet & cset);

	// 自身をカラにする
	void clear() { count = 0; }

	// 空集合か否かの判定 (true: 空; false: 非空)
	bool isEmpty() const { return count == 0; }
	int size() const { return count; }
	bool equals(const CardSet &) const;

	// 添字でアクセスする
	const Card & at(const int i) const { return cards[i]; }
	const Card & operator[](const int i) const { return at(i); }
	Card & at(const int i) { return cards[i]; }
	Card & operator[](const int i) { return at(i); }

	// １デッキすべての(maxnumcard 枚の)カードを入れる
	void setupDeck();

	// target に等しいカードがあるかどうかを返す( true: ある; false: ない)
	bool includes(const Card & target) const { return find(target) != count; }

	// rank が num に等しいカードがあるかどうかを返す( true: ある; fals: ない)
	bool includes(const int num) const { return find(num) != count; }

	// newcard を追加する ( false: 要素数に変化なし; true: 追加成功)
	bool insert(const Card & newcard);
	// cardset のカードすべてを追加する ( false: 要素数に変化なし; true: 一枚以上追加成功)
	bool insert(const CardSet & cardset);
	
	// target と同一のカードを取り除く (false: 要素数に変化なし; true: みつけて削除が成功)
	bool remove(const Card & target);
	// rank が num であるカードいずれか一枚を除く(false: 要素数に変化なし; true: 成功)
	bool remove(int num);

	// cardset に含まれるカードをすべて取り除く (false: 要素数に変化なし; true: 一枚以上削除)
	bool remove(const CardSet & cardset);

	// （0枚目はじまりで）targetpos 枚目のカードを除き，そのカードを card で返す．
	// targetpos == -1 の場合，ランダムに抜いて返す．
	// targetpos がカード数以上の場合，失敗する．
	// (false: 失敗，要素数に変化なし，card の値は変更されない; true: 成功)
	// デフォルトで（引数を省略した場合）ランダムに抜いて返す．
	bool draw(Card & card, int targetpos = -1);

	// シャフルする
	void shuffle();

	// rank と suit で昇順にソートする
	void sort(Card::CardComparator func = Card::ascending);

	// 自身の状態を出力する
	std::ostream&  printOn(std::ostream & out, const bool faceup = true) const;
	friend std::ostream& operator<<(std::ostream& out, const CardSet & cset) {
		return cset.printOn(out);
	}

};

#endif
