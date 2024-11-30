#include "player.h"
#include "constants.h"

Action Actor::act(Card dlr, Hand &h, int count){
	int tot = h.total();
	bool hard = h.hard();
	bool canSpecial = h.size() == 2 && (!h.split || (DAS && hard) || (DAS && RSA));
	if(tot > 21)
		return Bust;
	if(canSpecial){
		if(SUR && hard){
			if(tot == 16 && dlr >= 9) return Surrender;
			if(tot == 15 && dlr == 10) return Surrender;
		}
		if(h.pair()){
			if(!hard) return Split;
			int c = tot/2;
			if(c == 9 && (dlr <= 9 && dlr != 7)) return Split;
			if(c == 8) return Split;
			if(c == 7 && dlr <= 7) return Split;
			if(c == 6){
				if(DAS && dlr <= 6) return Split;
				else if(dlr >= 3 && dlr <= 6) return Split;
			}
			if(c == 4 && DAS && dlr >= 5 && dlr <= 6) return Split;
			if(c == 3 || c == 2){
				if(DAS && dlr <= 7) return Split;
				else if(dlr >= 4 && dlr <= 7) return Split;
			}
		}
	}
	if(hard){
		if(tot >= 17) return Stand;
		else if(tot == 16){
			if(dlr <= 6) return Stand;
			if(dlr == 10 && count > 0) return Stand;
			return Hit;
		} else if(tot == 15){
			if(dlr <= 6) return Stand;
			return Hit;
		} else if(tot >= 13){
			if(dlr <= 6) return Stand;
			return Hit;
		} else if(tot == 12){
			if(dlr >= 4 && dlr <= 6) return Stand;
			if(dlr == 3 && count >= 2) return Stand;
			return Hit;
		} else if(tot == 11) return canSpecial ? Double : Hit;
		else if(tot == 10){
			if(dlr <= 9) return canSpecial ? Double : Hit;
			return Hit;
		} else if(tot == 9){
			if(dlr >= 3 && dlr <= 6) return canSpecial ? Double : Hit;
			if(dlr == 2 && count >= 1) return canSpecial ? Double : Hit;
			return Hit;
		} else return Hit;
	} else{
		// if we can't double then soft hands are simple
		if(!DA2 || !canSpecial){
			if(tot >= 19) return Stand;
			else if(tot == 18){
				if(dlr <= 8) return Stand;
				return Hit;
			} else return Hit;
		}
		if(tot >= 20) return Stand;
		else if(tot == 19){
			if(dlr == 6) return Double;
			return Stand;
		} else if(tot == 18){
			if(dlr <= 6) return Double;
			if(dlr <= 8) return Stand;
			return Hit;
		} else if(tot == 17){
			if(dlr >= 3 && dlr <= 6) return Double;
			return Hit;
		} else if(tot >= 15){
			if(dlr >= 4 && dlr <= 6) return Double;
			return Hit;
		} else {
			if(dlr >= 5 && dlr <= 6) return Double;
			return Hit;
		}
	}
}
