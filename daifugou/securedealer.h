/*
 * securedealer.h
 *
 *  Created on: 2017/05/12
 *      Author: sin
 */

#ifndef SECUREDEALER_H_
#define SECUREDEALER_H_

#include <cinttypes>
#include <map>
#include <set>
#include <vector>

#include "card.h"
#include "cardset.h"
#include "dealer.h"

class SecureDealer : public Dealer {

public:
	typedef std::set<Card,Card::CardComparator> MyCardSet;

private:
	CardSet  shadow[NUM_OF_MAX_PLAYERS];

public:
	SecureDealer() : Dealer() {}
	virtual ~SecureDealer();

	virtual bool deal(void);
	virtual void accept( CardSet & opened);

	bool verifyHand(const int id) const;
	bool verifyPlayers() const;
};

#endif /* SECUREDEALER_H_ */
