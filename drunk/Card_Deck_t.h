#pragma once
#include <deque>
#include <vector>
#include <string>
#include <iostream>


struct Card_Deck_t {
	Card_Deck_t();

private:
	struct Card_t {
		Card_t(std::vector<std::wstring>::iterator pRank, std::vector<std::wstring>::iterator pSuit, int value);

		struct Character_Cards_t {
			std::vector<std::wstring>::iterator GetSuit();
			std::vector<std::wstring>::iterator GetRank();
			int GetSuitSIZE();
			int GetRankSIZE();

			std::wstring SPADES		= L"\x2660";
			std::wstring CLUBS		= L"\x2663";
			std::wstring HEARTS		= L"\x1B[31m\x2665\033[0m";
			std::wstring DIAMONDS	= L"\x1B[31m\x2666\033[0m";

			std::vector<std::wstring> Suit{ SPADES, CLUBS, HEARTS, DIAMONDS };

			std::wstring Six	= L"6";
			std::wstring Seven	= L"7";
			std::wstring Eight	= L"8";
			std::wstring Nine	= L"9";
			std::wstring Ten	= L"10";
			std::wstring Jack	= L"J";
			std::wstring Queen	= L"Q";
			std::wstring King	= L"K";
			std::wstring Ace	= L"A";

			std::vector<std::wstring> Rank{ Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };
		};
		std::pair<std::wstring, std::wstring> _Card;
		int value;
	};
	std::deque<Card_t> Deck;

public:
	using type_card = Card_t;

	std::deque<type_card>& GetDeck();
	void DelCard();
	void Show_Card(type_card card);
	void Show_Shirt();
};