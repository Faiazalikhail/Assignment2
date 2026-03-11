#pragma once
#include "Deck.h"
#include "Player.h"
#include "Dealer.h"

class BlackJackGame
{
public:

    void startRound()
    {
        player.resetHand();
        dealer.resetHand();

        player.hand.addCard(deck.deal());
        dealer.hand.addCard(deck.deal());

        player.hand.addCard(deck.deal());
        dealer.hand.addCard(deck.deal());
    }

    void playerHit()
    {
        player.hand.addCard(deck.deal());
    }

    void dealerTurn()
    {
        while (dealer.shouldHit())
        {
            dealer.hand.addCard(deck.deal());
        }
    }

    void settle()
    {
        int p = player.hand.getValue();
        int d = dealer.hand.getValue();

        if (player.hand.bust())
            return;

        if (dealer.hand.bust() || p > d)
            player.win();
        else if (p == d)
            player.push();
    }

    Deck deck;
    Player player;
    Dealer dealer;
};