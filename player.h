#ifndef PLAYER_H
#define PLAYER_H
#include "card.h"
#include "constants.h"

enum Action {Stand, Hit, Double, Split, Surrender, Bust};

struct ruined{};


// will move like the dealer moves
inline Action dlrAct(Hand &h) {
	int tot = h.total();
	if(tot > 21) return Bust;
	else if(tot > 17) return Stand;
	else if(tot == 17) return !h.hard() && H17 ? Hit : Stand;
	else return Hit;
}

class Actor{
	public:
		// will do the proper move according to basic strategy + some deviations
		Action act(Card dlr, Hand &h, int count);
		inline bool Insurance(Card dlr, int count) {return dlr == 11 && count >= 3;}
};

class Bettor{
	int betSpread;
	public:
	Bettor(int betSpread): betSpread{betSpread} {}
	inline double bet(int count, double minBet){
		if(count <= 1) return minBet;
		else if(count >= 6) return betSpread * minBet;
		return count * minBet;
	}
};

class Player{
	const double startingBR;
	double bankroll;
	int wongIn;
	int wongOut;
	bool in;
	Actor a;
	Bettor b;
	public:
	Player(double bankroll, int wongIn, int wongOut, int betSpread): startingBR{bankroll}, bankroll{bankroll},
		wongIn{wongIn}, wongOut{wongOut}, in{false}, a{}, b{betSpread} {}
	inline double bet(int count) {
		if(bankroll < 6 * MINBET) throw ruined{};
		if(count >= wongIn) in = true;
		else if(count <= wongOut) in = false;
		if(in){
			double curBet = b.bet(count, MINBET);
			bankroll -= curBet;
			return curBet;
		}
		return 0;
	}
	inline bool Insurance(Card dlr, int count){ return a.Insurance(dlr, count);}
	inline void win(double money) {bankroll += money;}
	inline void lose(double money) {bankroll -= money;}
	inline double profit() {return bankroll - startingBR;}
	inline Action play(Card dlr, Hand &h, int count) {return a.act(dlr, h, count);}
};

#endif
