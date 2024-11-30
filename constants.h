#ifndef CONSTANTS_H
#define CONSTANTS_H

// actual constants
const int DECKSIZE = 52;

// options
const double ROUNDS_PER_HOUR = 80;
const long NUM_HOURS = 100;
const long NUM_ROUNDS = NUM_HOURS * ROUNDS_PER_HOUR;
const long NUM_SIMULS = 10000;
const int BETSPREAD = 8;
const int WONG_IN = 2;
const int WONG_OUT = 1;

const double BANKROLL = 7000;

// game rules
const double MINBET = 25;
const int NUM_DECKS = 8;
const double PENETRATION = 0.75;
const double BJ_PAYOUT = 1.5;
const bool H17 = false;
const bool DA2 = true;
const bool DAS = true;
const bool RSA = false;
const bool SUR = true;

#endif
