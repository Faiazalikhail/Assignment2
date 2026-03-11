#pragma once
#include "Deck.h"
#include "Player.h"
#include "Dealer.h"

enum class GameState { Betting, PlayerTurn, DealerTurn, GameOver };

class BlackJackGame
{
public:
    BlackJackGame() : state(GameState::Betting) {}

    void startRound()
    {
        if (player.bet == 0) return;

        player.resetHand();
        dealer.resetHand();

        player.hand.addCard(deck.deal());
        dealer.hand.addCard(deck.deal());

        player.hand.addCard(deck.deal());
        dealer.hand.addCard(deck.deal());

        state = GameState::PlayerTurn;

        if (player.hand.getValue() == 21) {
            settle();
        }
    }

    void playerHit()
    {
        if (state != GameState::PlayerTurn) return;

        player.hand.addCard(deck.deal());

        if (player.hand.bust()) {
            settle();
        } else if (player.hand.getValue() == 21) {
            dealerTurn();
            settle();
        }
    }

    void dealerTurn()
    {
        if (state != GameState::PlayerTurn && state != GameState::DealerTurn) return;
        state = GameState::DealerTurn;

        while (dealer.shouldHit())
        {
            dealer.hand.addCard(deck.deal());
        }
    }

    void settle()
    {
        state = GameState::GameOver;

        int p = player.hand.getValue();
        int d = dealer.hand.getValue();

        if (player.hand.bust()) {
            player.bet = 0;
            return;
        }

        if (dealer.hand.bust() || p > d) {
            player.win();
        }
        else if (p == d) {
            player.push();
        } else {
            player.bet = 0; // Dealer wins
        }
    }

    void resetForNewRound()
    {
        if (state == GameState::GameOver) {
            player.bet = 0;
            state = GameState::Betting;
        }
    }

    Deck deck;
    Player player;
    Dealer dealer;
    GameState state;
};