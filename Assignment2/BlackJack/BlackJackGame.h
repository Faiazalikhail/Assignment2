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

        player.getActiveHand().addCard(deck.deal());
        dealer.hand.addCard(deck.deal());

        player.getActiveHand().addCard(deck.deal());
        dealer.hand.addCard(deck.deal());
    }

    void playerHit()
    {
        player.hand.addCard(deck.deal());
    }

    void playerDouble()
    {
        if (state != GameState::PlayerTurn) return;

        if (player.getActiveHand().getCards().size() == 2 && player.credits >= player.bet)
        {
            // bet is doubled for this hand
            player.credits -= player.bet; // subtract the same amount again
            player.bet *= 2;

            player.getActiveHand().addCard(deck.deal());
            playerStand();
        }
    }

    void playerSplit()
    {
        if (state != GameState::PlayerTurn) return;

        if (player.getActiveHand().getCards().size() == 2 &&
            player.getActiveHand().isSplittable() &&
            player.credits >= player.bet &&
            !player.isSplit)
        {
            player.credits -= player.bet; // matching bet for the second hand

            // we split the hand
            Card splitCard = player.getActiveHand().getCards()[1];
            player.getActiveHand().getCards().pop_back();

            Hand secondHand;
            secondHand.addCard(splitCard);
            player.hands.push_back(secondHand);

            player.isSplit = true;
            player.activeHand = 0;

            // Give a card to the first hand
            player.getActiveHand().addCard(deck.deal());
            if (player.getActiveHand().getValue() == 21) {
                playerStand();
            }
        }
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