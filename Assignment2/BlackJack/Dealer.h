#pragma once
#include "Hand.h"

class Dealer
{
public:

    void resetHand()
    {
        hand = Hand();
    }

    bool shouldHit()
    {
        return hand.getValue() < 17;
    }

    Hand hand;
};