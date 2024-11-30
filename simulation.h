#ifndef SIMULATION_H
#define SIMULATION_H
#include "constants.h"
#include "card.h"
#include "player.h"
#include <vector>
// a struct to hold all the necessary information related to a simulation
struct simulResult{
	int rounds;
	double profit;
	bool ruined;
};


// acts out a hand that has been initially dealt
static inline std::vector<Hand> playHand(Shoe &sh, Player &p, Hand &h, Card dlr){
	std::vector<Hand> hands{};
	while(hands.empty()){
		Action a = p.play(dlr, h, sh.count());
		switch(a){
			case Stand:
				hands.push_back(h);
				break;
			case Hit:
				h.addCard(sh.draw());
				break;
			case Double:
				p.lose(h.bet);
				h.bet *= 2;
				h.addCard(sh.draw());
				hands.push_back(h);
				break;
			case Surrender:
				p.win(h.bet / 2);
				return hands;
			case Bust:
				return hands;
			case Split:
				Card c = h.doSplit();
				Hand h2{true};
				h2.bet = h.bet;
				p.lose(h.bet);
				h2.addCard(c);
				h.addCard(sh.draw());
				h2.addCard(sh.draw());
				hands = playHand(sh, p, h, dlr);
				auto hands2 = playHand(sh, p, h2, dlr);
				hands.insert(hands.end(), hands2.begin(), hands2.end());
				break;
		}
	}
	return hands;
}

// deals a full hand, complete with betting and making actions
static void inline dealHand(Shoe &sh, Player &p){
	Hand h;
	Hand dh;

	// the player decides whether or not to be in the hand
	h.bet = p.bet(sh.count());
	// if not we just play out 5 cards, which is about average for a hand between a dealer and a player
	if(!h.bet){
		for(int i = 0; i < 5; ++i) sh.draw();
		return;
	}

	// deals the initial hand
	dh.addCard(sh.draw());
	dh.addCard(sh.draw());
	h.addCard(sh.draw());
	h.addCard(sh.draw());

	// checks for BJ
	bool insurance = p.Insurance(dh.upCard(), sh.count());
	if(dh.total() == 21) {
		if(insurance) p.win(h.bet);
		if(h.total() == 21) p.win(h.bet);
	} else if(h.total() == 21)
		p.win(h.bet * (1 + BJ_PAYOUT));
	// otherwise we play as normal
	else {
		auto hands = playHand(sh, p, h, dh.upCard());
		if(hands.empty()) return;

		// now we do the dealer's turn
		Action da;
		do{
			da = dlrAct(dh);
			if(da == Hit) dh.addCard(sh.draw());
		} while(da == Hit);

		// now we compare
		for(auto &hand : hands){
			if(da == Bust) p.win(2 * hand.bet);
			if(dh.total() < hand.total()) 		 p.win(2 * hand.bet);
			else if(dh.total() == hand.total()) p.win(hand.bet);
		}
	}

	return;
}

// will run one simulation and return the average dollars per round for the run
simulResult runSimulation(int wongIn, int wongOut, int betSpread){
	Shoe sh{NUM_DECKS};
	Player p{BANKROLL, wongIn, wongOut, betSpread};
	int i = 0;
	try{
		for(; i < NUM_ROUNDS; ++i) dealHand(sh, p);
	} catch(ruined){
		return simulResult{i + 1, p.profit(), true};
	}
	return simulResult{NUM_ROUNDS, p.profit(), false};
}

#endif
