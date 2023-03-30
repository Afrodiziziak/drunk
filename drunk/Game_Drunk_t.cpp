#include "Game_Drunk_t.h"

Game_Drunk_t::Game_Drunk_t() {
	this->quantity_players	= NULL;
	this->draw				= false;
	this->people			= false;
	this->stop_thread		= false;

	ForCardAnimation = { L'\x2593', L'\x2592', L'\x2591'};
	ForWaitAnimation = { L'\x25CF', L'\x263C', L'\x25CB', L'\x25CC', L' '};
}

void Game_Drunk_t::SetPeople(bool people) { this->people = people; }

bool Game_Drunk_t::GetPeople() { return this->people; }

void Game_Drunk_t::SetStop_Thread(bool stop_thread) { this->stop_thread = stop_thread; }

bool Game_Drunk_t::GetStop_Thread() { return this->stop_thread; }

void Game_Drunk_t::SetQuantity_Players(int value) { quantity_players = value; }

int Game_Drunk_t::GetQuantity_Players() { return quantity_players; }

int Game_Drunk_t::CheckEmpty() {
	int index = NONE;

	for (int i = 0; i < quantity_players; i++) {
		if (Players.at(i).GetQuantity_Cards() == 0) index = i;
	}
	return index;
}

void Game_Drunk_t::Create_Players() {
	for (int i = quantity_players; i > 0; i--) {
		if (i == quantity_players && !people)
			Players.push_back(Player_t(people));
		else
			Players.push_back(Player_t(!people));
		Flags.push_back(false);
	}
}

void Game_Drunk_t::Remove_Player() {
	int index = CheckEmpty();
	if (index != NONE) {
		Show_Drunk(index);
		quantity_players--;
	}

	auto result = std::remove_if(Players.begin(), Players.end(), [](Player_t& player) {
		return player.GetQuantity_Cards() == 0;
		});

	Players.erase(result, Players.end());
}

std::vector<Player_t> Game_Drunk_t::GetPlayers() { return this->Players; }

void Game_Drunk_t::Mix_Cards(std::deque<Card_Deck_t::type_card>& deck) {
	srand((unsigned int)time(NULL));
	std::random_shuffle(deck.begin(), deck.end());
}

void Game_Drunk_t::Distribute_Cards() {
	Mix_Cards(main_deck.GetDeck());

	for (int i = main_deck.GetDeck().size(); i > 0; ) {
		for (int j = 0; j < quantity_players; j++) {
			Players.at(j).SetPlayer_Deck(main_deck.GetDeck().front());
			Players.at(j).AugmentQuantity_Cards();

			main_deck.DelCard();
			i--;
		}
	}
}

void Game_Drunk_t::ResetReplenish_Players() {
	for (int i = 0; i < quantity_players; i++)
		Players.at(i).SetReplenish(false);
}

void Game_Drunk_t::ResetNum_card() {
	for (int i = 0; i < quantity_players; i++)
		Players.at(i).GetNum_card() = 0;
}

void Game_Drunk_t::Circle(int& walking_player, const bool draw, const int index_loser) {
	if (walking_player >= quantity_players) walking_player = 0;
	int x = 20, y = 11;

	if (GetDraw()) y += quantity_players;

	int i	= walking_player;
	char ch = ' ';

	static const char LEFT	= 75;
	static const char RIGHT = 77;

	for (int j = quantity_players; j != 0; j--, i++) {
		if (i >= quantity_players) i = 0;
		if (i != index_loser) {
			if (!Players.at(i).GetComputer()) {
				bool walk = false;
				while (!walk) {
					Players.at(i).GoToXY(x, y);
					SetStop_Thread(false);
					Sleep(10);

					Players.at(i).GoToXY(--x, y);
					auto th = std::thread(&Game_Drunk_t::Animation_Wait, this, GetStop_Thread());

					ch = _getch();
					SetStop_Thread(true);
					th.join();
					Players.at(i).GoToXY(++x, y);
					if (ch == -32) ch = _getch();

					switch (ch) {
					case LEFT:
						if (!GetDraw()) {
							Animation_Mix(x, y);
							Mix_Cards(Players.at(i).GetPlayer_Deck());
						}
						break;
					case RIGHT:
						Players.at(i).GoToXY(x, y++);
						Players.at(i).Step(draw, GetDraw());
						Players.at(i).GetNum_card()++;

						walk = true;

						break;
					}
				}
			}
			else {
				Players.at(i).GoToXY(x, y++);
				Players.at(i).Step(draw, GetDraw());
				Players.at(i).GetNum_card()++;
			}
		}
	}
	walking_player++;
}

bool Game_Drunk_t::Six_Ace(std::vector<int> score) {
	bool six = false, ace = false, check = false;

	for (int i = 0; i < quantity_players; i++) {
		if (score.at(i) == 6) six	= true;
		if (score.at(i) == 14) ace	= true;
	}
	return six && ace ? true : false;
}

std::vector<int> Game_Drunk_t::Comparison(std::vector<int>& winner, const int index_loser, const int index_win) {
	std::vector<int> Score;

	for (int i = 0; i < quantity_players; i++) {
		i != index_loser ? Score.push_back(Players.at(i).GetPlayer_Deck().at(Players.at(i).GetNum_card() - 1).value) : Score.push_back(RETIRE);

		if (i == index_win) Score.push_back(AUTO_WIN);
	}

	int maximum = 0;

	if (index_win == NONE) {
		if (!Six_Ace(Score)) {
			for (int i = 0; i < Score.size(); i++)
				if (maximum < Score.at(i)) maximum = Score.at(i);
		}
		else {
			maximum = 6;
		}
	}

	for (int i = 0; i < quantity_players; i++) {
		Score.at(i) != maximum && Score.at(i) != RETIRE ? winner.push_back(LOSE) : winner.push_back(WIN);
		if (Score.at(i) == RETIRE) winner.at(i) = RETIRE;
	}

	return winner;
}

void Game_Drunk_t::Win_Lose(int& num_of_win, int& num_of_loser,
	int& index_win, int& index_loser, int& index_retired,
	std::vector<int>& winner) {
	num_of_win = EMPTY;
	num_of_loser = EMPTY;
	index_win = NONE;

	for (int i = 0; i < quantity_players; i++) {
		winner.at(i) == WIN ? num_of_win++ : num_of_loser++;
		if (winner.at(i) == RETIRE) {
			num_of_loser--;
			index_retired = i;
		}
	}

	if (num_of_win == 1)
		for (int i = 0; i < quantity_players; i++) if (winner.at(i) == WIN) index_win = i;
	if (num_of_loser == 1)
		for (int i = 0; i < quantity_players; i++) if (winner.at(i) == LOSE) index_loser = i;
}

void Game_Drunk_t::Auto_Win(int& index_win, const int index_loser, const int index_retired) {
	for (int i = 0; i < quantity_players; i++) {
		if (i != index_loser && i != index_retired) {
			if (Players.at(i).GetQuantity_Cards() - Players.at(i).GetNum_card() == 0)
				index_win = i;
		}
	}
}

void Game_Drunk_t::SetDraw(bool draw) { this->draw = draw; }

bool Game_Drunk_t::GetDraw() { return this->draw; }

int Game_Drunk_t::Dispute(int& walking_player, const int index_loser, const int index_retired) {
	int resultDisput_cards = Dispute_Cards(index_loser, index_retired);

	if (resultDisput_cards == NONE) {
		Circle(walking_player, false, index_loser);

		return NONE;
	}

	Circle(walking_player, true, index_loser);

	walking_player == 0 ? walking_player = quantity_players - 1 : walking_player--;

	Circle(walking_player, false, index_loser);

	return 0;
}

int Game_Drunk_t::Dispute_Cards(const int index_loser, const int index_retired) {
	int quantity = 0;
	for (int i = 0; i < quantity_players; i++) {
		if (i != index_loser && i != index_retired) {
			if (Players.at(i).GetQuantity_Cards() - Players.at(i).GetNum_card() == 0)
				quantity++;
		}
	}
	if (quantity == 1) {
		return WIN;
	}
	if (quantity == 2) {
		return DRAW;
	}
	for (int i = 0; i < quantity_players; i++) {
		if (i != index_loser && i != index_retired) {
			if (Players.at(i).GetQuantity_Cards() - Players.at(i).GetNum_card() == 1)
				return NONE;
		}
	}
	return 0;
}

void Game_Drunk_t::Discard_Cards(const int index_loser) {
	for (int i = 0; i < Players.at(index_loser).GetNum_card(); i++) {
		main_deck.GetDeck().push_back(Players.at(index_loser).GetPlayer_Deck().front());
		Players.at(index_loser).GetPlayer_Deck().pop_front();
		Players.at(index_loser).DecreaseQuantity_Cards();
	}
}

void Game_Drunk_t::Winning_Cards(const int index_win, const int index_loser, const int index_retired) {
	for (int i = 0; i < quantity_players; i++) {
		for (int j = 0; j < Players.at(index_win).GetNum_card(); j++) {
			if (i != index_loser && i != index_retired) {
				Players.at(index_win).SetCard_Pile(Players.at(i).GetPlayer_Deck().front());

				if (i != index_win) {
					Players.at(index_win).AugmentQuantity_Cards();
					Players.at(i).DecreaseQuantity_Cards();
				}
				Players.at(i).GetPlayer_Deck().pop_front();
			}
		}
	}
	if (!main_deck.GetDeck().empty()) {
		int SIZE = main_deck.GetDeck().size();
		for (int i = 0; i < SIZE; i++) {
			Players.at(index_win).SetCard_Pile(main_deck.GetDeck().front());
			Players.at(index_win).AugmentQuantity_Cards();

			main_deck.DelCard();
		}
	}
}

int Game_Drunk_t::Game() {
	int walking_player = 0;

	while (quantity_players > 1) {
		ResetReplenish_Players();
		ResetNum_card();

		int num_of_win		= EMPTY;
		int num_of_loser	= EMPTY;
		int index_loser		= NONE;
		int index_win		= NONE;
		int index_retired	= NONE;
		int index_player	= NONE;

		Show_Info(walking_player);
		Players.at(0).GoToXY(0, 27);
		Show_Control();

		Circle(walking_player);
		if (walking_player >= quantity_players) walking_player = 0;

		std::vector<int> winner;
		Comparison(winner);
		Win_Lose(num_of_win, num_of_loser, index_win, index_loser, index_retired, winner);

		while (num_of_win > 1) {
			SetDraw(true);
			if (num_of_loser == 1) Discard_Cards(index_loser);

			for (int i = 0; i < quantity_players; i++)
				winner.pop_back();

			walking_player == 0 ? walking_player = quantity_players - 1 : walking_player--;

			int resultDisput_cards = Dispute_Cards(index_loser, index_retired);

			if (resultDisput_cards == WIN) {
				Auto_Win(index_win, index_loser, index_retired);
				break;
			}

			if (resultDisput_cards == DRAW)
				return DRAW;

			Dispute(walking_player, index_loser, index_retired);

			Comparison(winner, index_loser, index_win);
			Win_Lose(num_of_win, num_of_loser, index_win, index_loser, index_retired, winner);

			if (num_of_loser == 1) Discard_Cards(index_loser);
		}
		if (!GetDraw() && index_loser != -1) Discard_Cards(index_loser);
		Winning_Cards(index_win, index_loser, index_retired);
		SetDraw(false);

		Sleep(2050);
		Show_Info(walking_player, index_win);

		Remove_Player();
	}
	return WIN;
}

void Game_Drunk_t::Show_Winner(int walking_player) {
	Show_Info(walking_player);
	Players.front().GoToXY(20, 11);
	std::wcout << L"ÏÎÁÅÄÈË " << Players.front().GetName();
	Players.front().GoToXY(0, 27);
	Show_Control();
	Sleep(2050);
}

void Game_Drunk_t::Show_Drunk(int index) {
	Show_Info(NONE);
	Players.front().GoToXY(20, 11);
	std::wcout << Players.at(index).GetName() << " " << L"ÏÜßÍÈÖÀ";
	Players.front().GoToXY(0, 27);
	Show_Control();
	Sleep(2050);
}

void Game_Drunk_t::Show_Draw(int walking_player) {
	Show_Info(walking_player);
	Players.front().GoToXY(20, 11);
	std::wcout << L"ÍÈ×Üß" << std::endl;
	Players.front().GoToXY(0, 27);
	Show_Control();
	Sleep(2050);
}

void Game_Drunk_t::Show_Info(int walking_player, int index_win) {
	system("cls");

	for (int i = 0; i < quantity_players; i++) {
		if (i == walking_player)
			std::wcout << L"\x1B[33m" << Players.at(i).GetName() << "\033[0m"
			<< L" " << L"\x25C4" << L"\n";
		else
			std::wcout << Players.at(i).GetName() << "\n";
		std::wcout << L"ÐÓÊÀ:\t" << Players.at(i).GetPlayer_Deck().size() << "\t"
			<< L"ÑÁÐÎÑ:\t" << Players.at(i).GetCard_Pile().size() << " ";

		if (i == index_win)
			Animation_Win();

		std::wcout << L"\n__________________________" << "\n";
	}
}

void Game_Drunk_t::Show_Control() {
	std::wcout << L"\t" << L"\x2190" << L" " << L"ïåðåìåøàòü êàðòû"
		<< L"\t" << L"\x2192" << L" " << L"ñäåëàòü õîä";
}

void Game_Drunk_t::Animation_Mix(int x, int y) {
	Players.front().GoToXY(x, y);

	for (int i = 0; i < ForCardAnimation.size(); i++) {
		Sleep(100);
		std::wcout << ForCardAnimation.at(i);
	}
	for (int i = 0; i < ForCardAnimation.size(); i++) {
		Sleep(100);
		std::wcout << L"\b \b";
	}
}

void Game_Drunk_t::Animation_Win() {
	int i = ForCardAnimation.size() - 1;

	for (; i > NONE; i--) {
		Sleep(150);
		std::wcout << ForCardAnimation.at(i);
	}
	for (int i = 0; i < ForCardAnimation.size(); i++) {
		Sleep(150);
		std::wcout << L"\b \b";
	}
}

int Game_Drunk_t::Animation_Wait(bool stop_thread) {
	bool stop = stop_thread;
	while (!stop) {
		for (int i = 0; i < ForWaitAnimation.size(); i++) {
			std::wcout << ForWaitAnimation.at(i);
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			std::wcout << L"\b \b";
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		stop = GetStop_Thread();
	}
	return 0;
}