//
// CardSet.cpp - トランプカードの集合型(C++版)
//	作者: (あなたの名前); 日付: (完成した日付)
//
#include <iostream>
#include <random>
#include <cstdlib>
#include <ctime>

#include "card.h"
#include "cardset.h"

CardSet::CardSet(void) {
	count = 0;
}

CardSet::CardSet(const CardSet & cset) {
	count = 0;
	for(int i = 0; i < cset.size(); ++i)
		insert(cset[i]);
}

// target に等しいカードの cards 中の格納位置を返す( == count: ない)
int CardSet::find(const Card & target) const {
	for (int i = 0; i < count; i++) {
		if (target.equals(cards[i]))
			return i;
	}
	return count;	// 見つからなかった
}

// rank が num に等しいカードの格納位置を返す(== count: ない)
int CardSet::find(const int number) const {
	for (int i = 0; i < count; i++) {
		if (number == cards[i].rank())
			return i;
	}
	return count;	// 見つからなかった
}


bool CardSet::equals(const CardSet & cset) const {
	if (size() != cset.size()) return false;
	for(int i = 0; i < size(); i++) {
		if ( !cset.includes(at(i)) )
			return false;
	}
	return true;
}

// １デッキすべての(maxnumcard 枚の)カードを入れる
void CardSet::setupDeck(void) {
	Card c;
	int suit, num;

	for (suit = Card::SUIT_SPADE; suit <= Card::SUIT_CLUB; suit++)
		for (num = 1; num <= 13; num++) {
			c.set(suit, num);
			insert(c);
		}
	c.setJoker();
	insert(c);
}

// targetpos 枚目のカードを除き，そのカードを card で返す
// (false: 要素数に変化なし，card の値は変更されない; true: 成功)
//
// remove() を実装した後でコメントを外して，draw を使えるようにせよ
/*
bool CardSet::draw(Card & card, int targetpos) {
	std::random_device rnd;

	if( targetpos == -1 )
		targetpos = rnd() % count;
	else if( count == 0 || !(targetpos < count) )
		return false;

	card = cards[targetpos];
	return remove(card);
}
 */

// cardset に含まれるカードをすべて取り除く (false: 要素数に変化なし; true: 一枚以上削除)
bool CardSet::remove(const CardSet & cardset) {
  bool result = false;
  for(int i = 0; i < cardset.size(); ++i) {
    result = result || remove(cardset.at(i));
  }
  return result;
}

//
// CardSet::insert() - newcard を入れる (false: 要素数に変化なし; true: 追加成功)
//
bool CardSet::insert(const Card & newcard) {
	int position = find(newcard);
	if (position != count)
		return false;	// 既にある
	// 最後に追加
	if (count >= maxnumcard)
		return false; // もうはいらないし，入れられるカードはないはず
	position = count;
	count++;
	cards[position] = newcard;

	return true;
}

// cardset のカードすべてを追加する ( false: 要素数に変化なし; true: 一枚以上追加成功)
bool CardSet::insert(const CardSet & cardset) {
  bool result = false;
  for(int i = 0; i < cardset.size(); ++i) {
    result = result || insert(cardset.at(i));
  }
  return result;
}

/*
// シャフルする
void CardSet::shuffle(void) {
    int t, i, j, k;
    Card swap;
    std::srand(std::time(0));

    for (t = 0; t < 100; t++) {
      i = std::rand() % count;
      j = i + (std::rand() % (count-i));
      for (k = 0; i + k < j; k++) {
    	  swap = cards[k];
    	  cards[k] = cards[j - k - 1];
    	  cards[j-k - 1] = swap;
       }
    }
}
*/

// シャフルする ver. 2018.05.7
void CardSet::shuffle(void) {
	std::random_device rnd;
	std::mt19937 mt(rnd());
	int i, j, k;
	Card c;

	for (int t = 0; t < 100; t++) {
		i = 1 + (mt() % (count - 1)); 	// ２枚目以降から
		j = mt() % (count - i); // j 枚抜いて
		for (k = 0; k < j ; ++k) {
			draw(c, i+j);		// （１枚ずつ後から抜く）
			insert(c); 			// 上にのせる
		}

		// カット
		i = 1 + (mt() % (count - 1)); 	// ２枚目以降から後すべてを
		for (k = 0; k < count; ++k) {
			draw(c, count - 1);		// （１枚ずつ最後から抜く）
			insert(c); 			// 上にのせる
		}

	}
}


// rank と suit で昇順にソートする
void CardSet::sort(Card::CardComparator func) {
	Card t;
	for(int i = 0; i < count - 1; ++i) {
		for(int j = i+1; j < count; ++j) {
			if ( !func(cards[i], cards[j]) ) {
				t = cards[j];
				cards[j] = cards[i];
				cards[i] = t;
			}
		}
	}
}

//
// CardSet::print() - 自身を出力する
//
std::ostream& CardSet::printOn(std::ostream & out, const bool faceup) const {
	out << "CardSet";
	out << "{";
	if (count == 0) {
		out << " } ";
		return out;
	}
	for (int i = 0; i < count; i++) {
		cards[i].printOn(out, faceup);
		out << ", ";
	}
	out << "} ";
	return out;
}

// CardSet::remove

// target と同一のカードを取り除く (false: 要素数に変化なし; true: みつけて削除が成功)
bool CardSet::remove(const Card & target) {
	int pos = find(target);
	if( pos == count ) // ない
		return false;
	for( ; pos < count; pos++) {
		cards[pos] = cards[pos+1];
	}
	count--;
	return true;
}


// rank が num であるカードいずれか一枚を除く(false: 要素数に変化なし; true: 成功)
bool CardSet::remove(const int num) {
	int pos = find(num);
	if( pos == count ) // ない
		return false;
	for( ; pos < count; pos++) {
		cards[pos] = cards[pos+1];
	}
	count--;
	return true;

}

// CardSet::draw

bool CardSet::draw(Card & card, int pos) {
	if( pos == -1 )
		pos = std::rand() % count;
	else if( count == 0 || !(pos < count) )
		return false;

	card = cards[pos];
	return remove(card);
}
