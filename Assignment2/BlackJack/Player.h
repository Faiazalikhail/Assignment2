#pragma once
#include "Hand.h"

class Player
{
public:

    Player()
    {
        credits = 1000;
        bet = 0;
    }

    void placeBet(int amount)
    {
        if (amount <= credits)
        {
            bet = amount;
            credits -= amount;
        }
    }

    void win()
    {
        credits += bet * 2;
    }

    void push()
    {
        credits += bet;
    }

    void resetHand()
    {
        hand = Hand();
    }

    Hand hand;

    int credits;
    int bet;
};