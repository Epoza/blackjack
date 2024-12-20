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

int main()
{
  // Print one card
  Card card{Card::rank_5, Card::suit_heart};
  std::cout << card << '\n';

  Deck deck{};
  std::cout << deck.dealCard() << ' ' << deck.dealCard() << ' ' << deck.dealCard() << '\n';

  deck.shuffle();
  std::cout << deck.dealCard() << ' ' << deck.dealCard() << ' ' << deck.dealCard() << '\n';

  return 0;
}