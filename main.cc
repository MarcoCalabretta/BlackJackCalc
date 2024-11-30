#include <iostream>
#include <fstream>
#include "constants.h"
#include "simulation.h"


int main(){
	/*std::ofstream f{"results.csv"};
	f << "Wong In,Wong Out,Bet Spread,EV/hr,ROR,ROD,ROL\n";
	for(int betSpread = 1; betSpread <= 15; ++betSpread){
		for(int wongIn = 1; wongIn <= 4; ++wongIn){
			for(int wongOut = 1; wongOut < wongIn; ++wongOut){
				long totalRounds = 0;
				double totalReturn = 0;
				int countRuins = 0;
				int countDoubles = 0;
				int countLosses = 0;
				simulResult curResult;
				for(int i = 0; i < NUM_SIMULS; ++i){
					curResult = runSimulation(wongIn, wongOut, betSpread);
					totalRounds += curResult.rounds;
					totalReturn += curResult.profit;
					countRuins += curResult.ruined ? 1 : 0;
					countDoubles += curResult.profit >= BANKROLL ? 1 : 0;
					countLosses += curResult.profit <= 0 ? 1 : 0;
				}
				double EV = (totalReturn / totalRounds) * ROUNDS_PER_HOUR;
				double ROR = static_cast<double>(countRuins) / static_cast<double>(NUM_SIMULS);
				double ROD = static_cast<double>(countDoubles) / static_cast<double>(NUM_SIMULS);
				double ROL = static_cast<double>(countLosses) / static_cast<double>(NUM_SIMULS);
				f << wongIn << ',' << wongOut << ',' << betSpread << ',' << EV << "$," << ROR << "," << ROD << ',' << ROL << '\n';
			}
		}
		std::cout << betSpread << std::endl;
	}
	*/

	long totalRounds = 0;
	double totalReturn = 0;
	int countRuins = 0;
	int countDoubles = 0;
	int countLosses = 0;
	simulResult curResult;
	for(int i = 0; i < NUM_SIMULS; ++i){
		curResult = runSimulation(WONG_IN, WONG_OUT, BETSPREAD);
		totalRounds += curResult.rounds;
		totalReturn += curResult.profit;
		countRuins += curResult.ruined ? 1 : 0;
		countDoubles += curResult.profit >= BANKROLL ? 1 : 0;
		countLosses += curResult.profit <= 0 ? 1 : 0;
	}

	double EV = (totalReturn / totalRounds) * ROUNDS_PER_HOUR;
	double ROR = static_cast<double>(countRuins) / static_cast<double>(NUM_SIMULS);
	double ROD = static_cast<double>(countDoubles) / static_cast<double>(NUM_SIMULS);
	double ROL = static_cast<double>(countLosses) / static_cast<double>(NUM_SIMULS);
	std::cout.precision(4);
	printf("Play from %d to %d with bet spread of %d\n", WONG_IN, WONG_OUT, BETSPREAD);
	std::cout << "Average EV: " << EV << "$/hr\n";
	std::cout << "Risk of Ruin (losing " << BANKROLL << "$): " << ROR * 100 << "%\n";
	std::cout << "Risk of Winning " << BANKROLL << "$: " << ROD * 100 << "%\n";
	std::cout << "Risk of being down after " << NUM_HOURS << " hours: " << ROL * 100 << "%\n";

}
