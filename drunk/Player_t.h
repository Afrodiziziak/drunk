#pragma once
#include <Windows.h>
#include "Card_Deck_t.h"

struct Player_t : public Card_Deck_t {
	Player_t(bool computer);

	void SetPlayer_Deck(type_card Card);
	std::deque<Card_Deck_t::type_card>& GetPlayer_Deck();

	void SetCard_Pile(type_card Card);
	std::deque<Card_Deck_t::type_card>& GetCard_Pile();

	int& GetNum_card();

	void AugmentQuantity_Cards();
	void DecreaseQuantity_Cards();

	int GetQuantity_Cards();

	void SetReplenish(const bool replenish);
	bool GetReplenish();

	void SetComputer(bool comp);
	bool GetComputer();

	std::wstring GetName();

	void Take_Cards(const bool dispute);

	void Wait();

	void Step(const bool draw, const bool dispute = false);

	void GoToXY(short x, short y);

private:
	std::wstring name;
	std::deque<Card_Deck_t::type_card> Player_Deck;
	std::deque<Card_Deck_t::type_card> Card_Pile;
	bool replenish;
	bool computer;
	int quantity_ñards;
	int num_card;
};