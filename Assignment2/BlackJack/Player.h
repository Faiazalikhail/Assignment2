#pragma once
#include "Hand.h"

class Player
{
public:

    Player()
    {
        credits = 1000;
        bet = 0;
        isSplit = false;
        activeHand = 0;
    }

    void placeBet(int amount)
    {
        if (amount <= credits)
        {
            bet += amount;
            credits -= amount;
        }
    }

    void win(int multiplier = 2)
    {
        credits += bet * multiplier;
    }

    void push()
    {
        credits += bet;
    }

    void resetHand()
    {
        hands.clear();
        hands.push_back(Hand());
        isSplit = false;
        activeHand = 0;
        // Do NOT reset bet here, as the round is just starting with the placed bet.
    }

    Hand& getActiveHand()
    {
        return hands[activeHand];
    }

    std::vector<Hand> hands;
    bool isSplit;
    int activeHand;

    int credits;
    int bet;
};