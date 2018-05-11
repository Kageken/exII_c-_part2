/*
 * cardset_remove.cpp
 *
 *  Created on: 2015/04/15
 *      Author: sin
 */

#include "cardset.h"
#include "card.h"

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
