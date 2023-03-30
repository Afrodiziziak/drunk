#include "Card_Deck_t.h"

Card_Deck_t::Card_Deck_t() {
	Card_Deck_t::Card_t::Character_Cards_t Parametrs;
	std::vector<std::wstring>::iterator itRank = Parametrs.GetRank();
	std::vector<std::wstring>::iterator itSuit = Parametrs.GetSuit();
	int value = 6;

	for (int i = 0; i < Parametrs.GetRankSIZE(); i++) {
		for (int j = 0; j < Parametrs.GetSuitSIZE(); j++) {
			Deck.push_back(Card_t(itRank, itSuit, value));
			++itSuit;
		}
		itSuit = Parametrs.GetSuit();
		++itRank;
		value++;
	}
}

std::deque<Card_Deck_t::type_card>& Card_Deck_t::GetDeck() {
	return this->Deck;
}

void Card_Deck_t::DelCard() {
	Deck.pop_front();
}

void Card_Deck_t::Show_Card(type_card card) {
	std::wcout << card._Card.first << card._Card.second;
}

void Card_Deck_t::Show_Shirt() {
	std::wcout << L"\x2593";
}

Card_Deck_t::Card_t::Card_t(std::vector<std::wstring>::iterator pRank, std::vector<std::wstring>::iterator pSuit, int value) {
	this->_Card.first	= *pRank;
	this->_Card.second	= *pSuit;
	this->value			= value;
}

std::vector<std::wstring>::iterator Card_Deck_t::Card_t::Character_Cards_t::GetSuit() { return Suit.begin(); }

std::vector<std::wstring>::iterator Card_Deck_t::Card_t::Character_Cards_t::GetRank() { return Rank.begin(); }

int Card_Deck_t::Card_t::Character_Cards_t::GetSuitSIZE() { return Suit.size(); }

int Card_Deck_t::Card_t::Character_Cards_t::GetRankSIZE() { return Rank.size(); }