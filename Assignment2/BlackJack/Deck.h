#pragma once
#include <vector>
#include <algorithm>
#include <random>
#include "Card.h"

class Deck
{
public:

    Deck()
    {
        for (int s = 0; s < 4; s++)
        {
            for (int r = 1; r <= 13; r++)
            {
                cards.push_back(Card(s, r));
            }
        }

        shuffle();
    }

    void shuffle()
    {
        std::shuffle(cards.begin(), cards.end(), std::mt19937(std::random_device()()));
        index = 0;
    }

    Card deal()
    {
        if (index >= cards.size())
            shuffle();

        return cards[index++];
    }

private:

    std::vector<Card> cards;
    size_t index = 0;
};