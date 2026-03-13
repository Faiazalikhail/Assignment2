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

        player.getActiveHand().addCard(deck.deal());
        dealer.hand.addCard(deck.deal());

        player.getActiveHand().addCard(deck.deal());
        dealer.hand.addCard(deck.deal());

        state = GameState::PlayerTurn;
        resultMessage = "";

        if (player.getActiveHand().getValue() == 21) {
            settle();
        }
    }

    void playerHit()
    {
        if (state != GameState::PlayerTurn) return;

        player.getActiveHand().addCard(deck.deal());

        if (player.getActiveHand().bust()) {
            playerStand(); // if bust, go to next hand or settle
        } else if (player.getActiveHand().getValue() == 21) {
            playerStand();
        }
    }

    void playerStand()
    {
        if (state != GameState::PlayerTurn) return;

        if (player.isSplit && player.activeHand == 0)
        {
            player.activeHand = 1;
            // auto-deal one card if they only have 1 (from split)
            if (player.getActiveHand().getCards().size() == 1)
            {
                player.getActiveHand().addCard(deck.deal());
                if (player.getActiveHand().getValue() == 21) {
                    playerStand();
                }
            }
        }
        else
        {
            dealerTurn();
            settle();
        }
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
        resultMessage = "";

        int totalWinnings = 0;
        int d = dealer.hand.getValue();
        bool dealerBlackJack = (dealer.hand.getCards().size() == 2 && d == 21);

        for (int i = 0; i < player.hands.size(); i++)
        {
            Hand& h = player.hands[i];
            int p = h.getValue();
            int currentHandBet = player.isSplit ? (player.bet / 2) : player.bet;

            if (h.bust()) {
                resultMessage += "Hand " + std::to_string(i + 1) + " Busts! ";
                continue;
            }

            bool playerBlackJack = (h.getCards().size() == 2 && p == 21 && !player.isSplit);

            if (playerBlackJack && !dealerBlackJack) {
                totalWinnings += (int)(currentHandBet * 2.5);
                resultMessage += "Blackjack! ";
            } else if (dealer.hand.bust() || p > d) {
                totalWinnings += currentHandBet * 2;
                resultMessage += "Player Wins! ";
            } else if (p == d) {
                totalWinnings += currentHandBet;
                resultMessage += "Push. ";
            } else {
                resultMessage += "Dealer Wins. ";
            }
        }

        player.credits += totalWinnings;
        player.bet = 0;
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
    std::string resultMessage;
};