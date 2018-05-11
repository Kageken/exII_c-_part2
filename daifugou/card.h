//
// Card.h - トランプカードの型
//	作者: (あなたの名前); 日付: (完成した日付)
//
#ifndef CARD_H
#define CARD_H

#include <cstdio>
#include <iostream>
//
// Card - トランプカード型
//
class Card {
	// クラス定数
	// トランプの組(suit)のコード
	public:
	enum {
		SUIT_BLANK = 0,
		SUIT_SPADE = 1,
		SUIT_DIAMOND,
		SUIT_HEART,
		SUIT_CLUB,
		SUIT_JOKER,
	};
	
	// カードの比較関数型とデフォルト比較関数
	typedef bool (*(CardComparator))(const Card &, const Card &);

	static bool ascending(const Card & a, const Card & b) {
		return (a.rank() < b.rank()) || (a.rank() == b.rank() && a.suit() <= b.suit());
	}

// メンバ変数
private:
//	インスタンスメンバ変数．Card 型データ（オブジェクト）がそれぞれ持っているデータ．
	char suitval;	// 組
	int rankval;	// 番号

//	static は，クラスメンバのこと．Card クラスの中で共通の定数として Card::suitnames で参照できる．
//  クラス変数（定数）．値の初期化は .cpp で main の外に書いて行う
	static const char * suitnames[];
	static const char * ranknames[];

public:
	static const int RANK_JOKER = 15; // Joker の rank 用ダミー値

// メンバ関数
public:
	// デフォルトコンストラクタ（初期値はブランクカード）
	Card(void) : suitval(SUIT_BLANK), rankval(0) { }

	Card(const char st, const int num) : suitval(st), rankval(num) {}

	Card(const Card & org) : suitval(org.suitval), rankval(org.rankval) { }

	// 組と番号を設定する
	void set(char st, int num) {
		suitval = st;
		rankval = num; 
	}

	void setJoker() {
		set(SUIT_JOKER, RANK_JOKER);
	}

	// アクセサ
	int rank(void) const {
		return rankval;
	}

	int suit(void) const {
		return suitval;
	}

	bool isJoker(void) const {
		return suitval == SUIT_JOKER;
	}

	bool isValid() const;

	// 自身と tgt のカードの組，番号が等しいか判定 (true: 同; false: 異)
	// 同じアドレスにあるデータかどうか，ではない．
	bool equals(const Card & tgt) const {
		return (suitval == tgt.suitval) && (rankval == tgt.rankval);
	}
	
	// 文字列から自身の値をセット
	const char * readFrom(const char *);
	
	/* ストリームか値をセット */
	std::istream & readFrom(std::istream &);

	/* ファイルストリームに出力する */
	std::ostream & printOn(std::ostream &, const bool faceup = true) const;

	/* ストリーム出力演算子 */
	friend std::ostream & operator<<(std::ostream & os, const Card & c) {
		return c.printOn(os);
	}
};
#endif
