#pragma once

struct Card
{
    int suit;
    int rank;

    Card(int s, int r)
    {
        suit = s;
        rank = r;
    }

    int value() const
    {
        if (rank >= 10) return 10;
        if (rank == 1) return 11;
        return rank;
    }
};