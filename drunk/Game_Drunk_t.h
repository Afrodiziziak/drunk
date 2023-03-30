#pragma once
#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <conio.h>
#include "Card_Deck_t.h"
#include "Player_t.h"
#include <ctime>
#include <algorithm>
#include <thread>
#include <chrono>
#include <mutex>

struct Game_Drunk_t {
	enum STATUS		{ RETIRE = -1, LOSE, WIN, DRAW, AUTO_WIN = 15 };
	enum COUNTING	{ NONE = -1, EMPTY };

	Game_Drunk_t();

	void SetPeople(bool people);
	bool GetPeople();

	void SetStop_Thread(bool stop_thread);
	bool GetStop_Thread();

	void SetQuantity_Players(int value);
	int GetQuantity_Players();
	int CheckEmpty();

	void Create_Players();
	void Remove_Player();

	std::vector<Player_t> GetPlayers();

	void Mix_Cards(std::deque<Card_Deck_t::type_card>& deck);
	void Distribute_Cards();

	void ResetReplenish_Players();
	void ResetNum_card();

	void Circle(int& walking_player, const bool draw = false, const int index_loser = NONE);

	bool Six_Ace(std::vector<int> score);

	std::vector<int> Comparison(std::vector<int>& winner, const int index_loser = NONE, const int index_win = NONE);

	void Win_Lose(int& num_of_win, int& num_of_loser,
		int& index_win, int& index_loser, int& index_retired,
		std::vector<int>& winner);
	void Auto_Win(int& index_win, const int index_loser, const int index_retired);

	void SetDraw(bool draw);
	bool GetDraw();

	int Dispute(int& walking_player, const int index_loser, const int index_retired);
	int Dispute_Cards(const int index_loser, const int index_retired);

	void Discard_Cards(const int index_loser);
	void Winning_Cards(const int index_win, const int index_loser, const int index_retired);

	int Game();

	void Show_Winner(int walking_player = NULL);
	void Show_Drunk(int index);
	void Show_Draw(int walking_player = NULL);
	void Show_Info(int walking_player, int index_win = NONE);
	void Show_Control();

	void Animation_Mix(int x, int y);
	void Animation_Win();
	int Animation_Wait(bool stop_thread);

private:
	std::vector<Player_t> Players;
	std::vector<bool> Flags;
	std::vector<wchar_t> ForCardAnimation;
	std::vector<wchar_t> ForWaitAnimation;
	Card_Deck_t main_deck;
	bool draw;
	bool people;
	bool stop_thread;
	int quantity_players;
};