#include "Player_t.h"
#include <ctime>

Player_t::Player_t(bool computer) {
	std::vector<std::wstring> NamesBots{ L"Голлум",
										 L"Дамбудзо Мнангагва",
										 L"Остап Бендер",
										 L"Никола Тесла",
										 L"Зигмунд Фрейд",
										 L"Евпатий",
										 L"Принц Полукровка" };
	if (!computer) {
		srand(time(NULL));
		std::wcout << L"Введите имя игрока: ";
		std::getline(std::wcin, name);
	}
	else {
		this->name = NamesBots[rand() % NamesBots.size()];
	}
	quantity_сards	= NULL;
	num_card		= NULL;
	replenish		= false;
	this->computer	= computer;
}

void Player_t::SetPlayer_Deck(type_card Card) { this->Player_Deck.push_back(Card); }

std::deque<Card_Deck_t::type_card>& Player_t::GetPlayer_Deck() { return this->Player_Deck; }

void Player_t::SetCard_Pile(type_card Card) { this->Card_Pile.push_back(Card); }

std::deque<Card_Deck_t::type_card>& Player_t::GetCard_Pile() { return this->Card_Pile; }

int& Player_t::GetNum_card() { return this->num_card; }

void Player_t::AugmentQuantity_Cards() { this->quantity_сards++; }

void Player_t::DecreaseQuantity_Cards() { this->quantity_сards--; }

int Player_t::GetQuantity_Cards() { return this->quantity_сards; }

void Player_t::SetReplenish(const bool replenish) {	this->replenish = replenish; }

bool Player_t::GetReplenish() { return this->replenish; }

void Player_t::SetComputer(bool comp) { this->computer = comp; }

bool Player_t::GetComputer() { return this->computer; }

std::wstring Player_t::GetName() { return this->name; }

void Player_t::Take_Cards(const bool dispute) {
	int SIZE = Card_Pile.size();

	if (!Card_Pile.empty()) {
		for (int i = 0; i < SIZE; i++) {
			Player_Deck.push_back(Card_Pile.front());
			Card_Pile.erase(Card_Pile.begin());
		}
		SetReplenish(true);

		if (!dispute) GetNum_card() = NULL;
	}
}

void Player_t::Wait() { system("PAUSE > null"); }

void Player_t::Step(const bool draw, const bool dispute) {
	if (GetPlayer_Deck().empty() || GetNum_card() >= GetPlayer_Deck().size())
		Take_Cards(dispute);

	if (computer) Sleep(700);

	if (draw == false) {
		Show_Card(GetPlayer_Deck()[GetNum_card()]);
		std::wcout << L'\t' << GetName();
	}
	else {
		Show_Shirt();
		std::wcout << '\t' << GetName();
	}
}

void Player_t::GoToXY(short x, short y) {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hStdOut, { x, y });
}