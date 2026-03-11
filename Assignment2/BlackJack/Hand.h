#pragma once
#include <vector>
#include "Card.h"

class Hand
{
public:

    void addCard(Card c)
    {
        cards.push_back(c);
    }

    int getValue()
    {
        int total = 0;
        int aces = 0;

        for (auto& c : cards)
        {
            int v = c.value();
            if (v == 11) aces++;

            total += v;
        }

        while (total > 21 && aces > 0)
        {
            total -= 10;
            aces--;
        }

        return total;
    }

    bool bust()
    {
        return getValue() > 21;
    }

    std::vector<Card>& getCards()
    {
        return cards;
    }

private:

    std::vector<Card> cards;
};