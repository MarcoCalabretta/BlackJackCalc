#ifndef CARD_H
#define CARD_H
// 11 will refer to ace, and should we go over 21 the 11 will be changed to a 1
#include "constants.h"
#include <cstdlib>
#include <vector>


static inline unsigned long long rdtsc(void){
    unsigned long long int x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    // broken for 64-bit builds; don't copy this code
    return x;
}


using Card = char;

const Card NULLCARD = 0;

// I feel relatively certain that it is impossible to have more than 31 cards in a blackjack hand, and I hope 32 bytes is an efficient size
class Hand{
	std::vector<Card> cards;
	public:
	double bet = 0;
	bool split = false;
	inline Hand(bool split = false): split{split} {}
	inline int size() const {return cards.size();}
	inline int total(){
		int ret = 0;
		int ace = -1;
		for(size_t i = 0; i < cards.size(); ++i){
			ret += cards[i];
			if(cards[i] == 11) ace = i;
			if(ret > 21){
				if(ace >= 0){
					cards[ace] = 1;
					ret -= 10;
				} else break;
			}
		}
		return ret;
	}
	inline bool hard() const{
		for(size_t i = 0; i < cards.size(); ++i) if(cards[i] == 11) return false;
		return true;
	}
	inline bool pair() const{
		return size() == 2 && (cards[0] == cards[1] || (!hard() && cards[0] + cards[1]  == 12));
	}
	inline void addCard(Card c){cards.push_back(c);}
	inline Card upCard() const { return cards.front();}
	inline Card doSplit() {
		split = true;
		if(cards[0] == 1) cards[0] = 11;
		if(cards[1] == 1) cards[1] = 11;
		Card c = cards.back();
		cards.pop_back();
		return c;
	}
};


class Shoe{
	int runningCount;
	int numCards;
	int cutCard;
	int curCard;
	Card *cards;
	inline void shuffle(){
		curCard = 0;
		runningCount = 0;
		char tmp;
		int pos;
		for (int i = numCards - 1; i > 0; i--) {
			pos = std::rand() % i;
			tmp = cards[i];
			cards[i] = cards[pos];
			cards[pos] = tmp;
		}
	}
	inline void addDeck(Card *cp){
		char curVal = 2;
		for(int i = 0; i < DECKSIZE / 4; ++i){
			// we want to make the last 3 cards 10s
			if(i >= DECKSIZE / 4 - 3) curVal = 10;
			for(int i = 0; i < 4; ++i){
				cp[i] = curVal;
			}
			cp += 4;
			++curVal;
		}
	}
	public:
	Shoe(int decks): numCards{decks * DECKSIZE}, cutCard{static_cast<int>(PENETRATION * numCards)}, curCard{0}, cards{new Card[numCards]}{
		for(int i = 0; i < decks; ++i) addDeck(cards + i * DECKSIZE);
		std::srand(rdtsc());
		shuffle();
	}
	~Shoe(){delete[] cards;}
	// will draw the top card of the shoe, and will update the count
	inline Card draw(){
		if(curCard < cutCard){
			Card c = cards[curCard++];
			if(c <= 6) ++runningCount;
			else if(c >= 10) --runningCount;
			return c;
		}
		shuffle();
		return draw();
	}
	inline int count() const {return runningCount / (1 + (numCards - curCard) / DECKSIZE);} 
};
#endif
