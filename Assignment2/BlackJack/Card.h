#pragma once

struct Card
{
    int suit; // 0-3
    int rank; // 0-12 (0 is Ace, 10 is Jack, 11 is Queen, 12 is King)

    Card(int s, int r)
    {
        suit = s;
        rank = r;
    }

    int value() const
    {
        if (rank >= 9) return 10; // 10, J(10), Q(11), K(12) all index >= 9
        if (rank == 0) return 11; // Ace
        return rank + 1; // 2 through 9
    }
};