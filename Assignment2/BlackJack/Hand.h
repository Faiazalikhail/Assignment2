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

    bool isSplittable()
    {
        if (cards.size() == 2)
        {
            // Usually split requires same rank
            return cards[0].rank == cards[1].rank;
        }
        return false;
    }

    bool bust()
    {
        return getValue() > 21;
    }

    std::vector<Card>& getCards()
    {
        return cards;
    }

    void popCard()
    {
        if (!cards.empty()) {
            cards.pop_back();
        }
    }

private:

    std::vector<Card> cards;
};