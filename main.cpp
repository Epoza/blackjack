#include <iostream>
#include <array>
#include <algorithm> // for std::shuffle
#include "Random.h"  // for Random::mt
#include <cassert>

struct Card
{
  enum Rank
  {
    rank_ace,
    rank_2,
    rank_3,
    rank_4,
    rank_5,
    rank_6,
    rank_7,
    rank_8,
    rank_9,
    rank_10,
    rank_jack,
    rank_queen,
    rank_king,

    max_ranks
  };

  enum Suit
  {
    suit_club,
    suit_diamond,
    suit_heart,
    suit_spade,

    max_suits
  };

  static constexpr std::array allRanks{rank_ace, rank_2, rank_3, rank_4, rank_5, rank_6, rank_7, rank_8, rank_9, rank_10, rank_jack, rank_queen, rank_king};
  static constexpr std::array allSuits{suit_club, suit_diamond, suit_heart, suit_spade};

  Rank rank{};
  Suit suit{};

  friend std::ostream &operator<<(std::ostream &out, const Card &card)
  {
    static constexpr std::array rank{'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};
    static constexpr std::array suit{'C', 'D', 'H', 'S'};

    out << rank[card.rank] << suit[card.suit]; // print your card rank and suit here
    return out;
  }

  int value()
  {
    static constexpr std::array rankValues{11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};
    return rankValues[rank];
  }
};

class Deck
{
private:
  std::array<Card, 52> m_cards{};
  std::size_t m_currentCardIndex{0};

public:
  Deck()
  {
    std::size_t count{0};
    for (auto suit : Card::allSuits)
      for (auto rank : Card::allRanks)
        m_cards[count++] = (Card{rank, suit});
  };

  void shuffle()
  {
    std::shuffle(m_cards.begin(), m_cards.end(), Random::mt);
    m_currentCardIndex = 0;
  }

  Card dealCard() // deal the next card
  {
    assert(m_currentCardIndex != 52 && "Deck::dealCard ran out of cards");
    return m_cards[m_currentCardIndex++];
  }
};

struct Player
{
  int score{0};
};

namespace Settings
{
  constexpr int bust{21};
  constexpr int dealerStop{17};
};

bool dealerTurn(Deck &deck, Player &dealer)
{
  while (dealer.score < Settings::dealerStop)
  {
    Card dealerCard{deck.dealCard()};
    dealer.score += dealerCard.value();
    std::cout << "The dealer flips a " << dealerCard << ". They now have: " << dealer.score << '\n';
  }

  if (dealer.score > Settings::bust)
  {
    std::cout << "The dealer went bust!\n";
    return true;
  }
  return false;
}

char playerWantsHit()
{
  while (true)
  {
    std::cout << "(h) to hit, or (s) to stand: ";
    char input{};
    std::cin >> input;

    // If there is extraneous input, treat as failure case
    if (!std::cin.eof() && std::cin.peek() != '\n')
    {
      std::cout << "Not a valid input. Try again: ";
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore any extraneous input
      continue;
    }
    switch (input)
    {
    case 'h':
      return true;
    case 's':
      return false;
    }
  }
}

bool playerTurn(Deck &deck, Player &player)
{
  while (player.score < Settings::bust && playerWantsHit())
  {
    Card playerCard{deck.dealCard()};
    player.score += playerCard.value();
    std::cout << "You were dealt a " << playerCard << ". You now have: " << player.score << '\n';
  }

  if (player.score > Settings::bust)
  {
    std::cout << "You went bust!\n";
    return true;
  }

  return false;
}

bool playBlackjack()
{
  Deck deck{};
  deck.shuffle();

  Player dealer{deck.dealCard().value()};
  std::cout << "The dealer is showing: " << dealer.score << '\n';

  Player player{deck.dealCard().value() + deck.dealCard().value()};
  std::cout << "You have: " << player.score << '\n';

  if (playerTurn(deck, player))
    return false;

  if (dealerTurn(deck, dealer))
    return true;

  return player.score > dealer.score;
}

int main()
{
  playBlackjack() ? std::cout << "You win!" : std::cout << "You lose!";
  std::cout << '\n';

  return 0;
}