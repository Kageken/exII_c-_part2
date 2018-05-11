//
// cardset.cpp - トランプカードの集合型(C++版)
//	作者: (あなたの名前); 日付: (完成した日付)
//
#include "card.h"

#include <iostream>

// クラス変数（定数）の初期化
const char * Card::suitnames[] = {
		"_", "S", "D", "H", "C", "Jkr",
//		"Blank", "Spade", "Diamond", "Heart", "Club", "Joker",
};

const char * Card::ranknames[] = {
  		"?", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "Jkr", "_",
};

//
// Card::scan() - 標準出力などから値を得る (true: 取得成功; false: 取得失敗)
//
const char * Card::readFrom(const char * str)
{
	char * ptr;
	int s;

  suitval = SUIT_BLANK;
  rankval = 0;

  // 4組のいずれ？
  for(s = SUIT_SPADE; s <= SUIT_CLUB; s++) {
	  if( tolower(*str) == tolower(Card::suitnames[s][0]) ) { // 一文字目だけで判定
		  suitval = s;
		  do { ++str; } while ( isalpha(*str) );
		  break;
	  }
  }
  if ( SUIT_SPADE <= s && s <= SUIT_CLUB ) {
	  // なら番号も得る
	  while ( isspace(*str) ) { ++str; }
	  rankval = strtod(str, &ptr);
	  if ( 0 < rankval && rankval <= 13 ) {
		  return ptr;
	  }  else {
		  if ( rankval == 0 ) {
			  switch( tolower(*str) ) {
			  case 'a':
				  rankval = 1;
				  break;
			  case 'j':
				  rankval = 11;
				  break;
			  case 'q':
				  rankval = 12;
				  break;
			  case 'k':
				  rankval = 13;
				  break;
			  }
		  }
		  return str;
	  }
  } else if( tolower(*str) == 'j' ) {
    suitval = SUIT_JOKER;
    rankval = RANK_JOKER; // joker は number を 15 としておく．
    do { ++str; } while ( isalpha(*str) );
    return str;
  }	std::istream & readFrom(std::istream &);
  return str;
}

std::istream & Card::readFrom(std::istream & in) {
	std::string buf;
	std::getline(in, buf);
	readFrom(buf.c_str());
	return in;
}

std::ostream & Card::printOn(std::ostream& ostr, const bool faceup) const {
	ostr << '[';
	if ( faceup ) {
		ostr << suitnames[(int)suitval];
		if ( suitval != SUIT_JOKER )
			ostr << " " << ranknames[rankval];
	} else {
		ostr << "#";
	}
	ostr << ']';
	return ostr;
}

