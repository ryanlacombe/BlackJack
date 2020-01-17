// BlackJack 2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <windows.h>
#include <iostream>
#include <random>
#include <time.h>
#include <io.h>			// needed for the _setmode function
#include <fcntl.h>		// needed for the _O_U16TEXT output mode

// unicode values for the suit glyphs
const wchar_t SPADE[] = L"\u2660";
const wchar_t CLUB[] = L"\u2663";
const wchar_t HEART[] = L"\u2665";
const wchar_t DIAMOND[] = L"\u2666";

const wchar_t* SUIT[] = {
	SPADE, CLUB, HEART, DIAMOND
};

struct Card {
	int suit;
	int value;
};

// Function to deal a new random card. 
// A random value from 1 to 52 is generated, then we use math to 
// calculate the suit and value of the card
Card dealCard()
{
	int cardIndex = rand() % 52;		// card index, a value from 0 to 51 inclusive
	int suit = cardIndex / 13;
	int value = cardIndex % 13;			// the value of the card, from 1 to 13

	Card card = { suit, value };
	return card;
}

// Calculates the sum of the value of the cards in the array
// The first ace is read as 11, and any following ace has a value of 1
// Other face cards have a value of 10
// If there is an ace in the cards and the total exceeds 21, then we recount
// the whole array again but this time all ace cards get a value of 1
int calculateTotal(Card* cardArray, int size)
{
	bool hasAce = false;
	bool isFirstTime = true;
	int total = 0;

	for (int i = 0; i < size; i++)
	{
		if (cardArray[i].value == 1)
		{
			if (hasAce == true)
				total += 1;
			else
				total += 11;
			hasAce = true;
		}
		else if (cardArray[i].value < 10)
		{
			total += cardArray[i].value;
		}
		else
		{
			total += 10;
		}

		if (total > 21 && hasAce && isFirstTime)
		{
			// if its the first time we've counted, we have an ace in the deck, and
			// the total value is over 21, then count the whole array again (this time
			// all aces will have a score of 1 so the total may not go over 21)
			i = -1;
			total = 0;
			isFirstTime = false;
		}
	}
	return total;
}

// Print a card to the console
// Use the unicode glyphs to output the card suit, 
// along with the value of the card.
// Face cards appear as A,J,K or Q
void printCard(const Card& card)
{
	//output the card and suite
	switch (card.value)
	{
	default:
		std::wcout << card.value << SUIT[card.suit];
		break;
	case 1:
		std::wcout << L"A" << SUIT[card.suit];
		break;
	case 11:
		std::wcout << L"J" << SUIT[card.suit];
		break;
	case 12:
		std::wcout << L"K" << SUIT[card.suit];
		break;
	case 13:
		std::wcout << L"Q" << SUIT[card.suit];
		break;
	}
}

// This function will step through the cards in the input card array
// and draw each card to the console.
// We call the 'calculateTotal' function to get the sum of all cards in the array
void printCards(Card* cardArray, int size)
{
	for (int i = 0; i < size; i++)
	{
		printCard(cardArray[i]);
		std::wcout << "  ";
	}
	std::wcout << std::endl;

	int total = calculateTotal(cardArray, size);

	std::wcout << L"Current total: " << total << std::endl;
}

void main()
{
	// Setup the console to display unicode characters. 
	// Without this, the console will only display ASCII, 
	// which means our glyphs won't print correctly
	_setmode(_fileno(stdout), _O_U16TEXT);

	// seed the random function
	srand(time(nullptr));

	int money = 100;			// amount of money the player starts with
	Card cards[10];				// array to store the cards the player is dealt. Reused for each hand
	int cardCount = 0;			// number of cards the player has been dealt

	// game loop. To exit, the player can choose to quit at the end of each hand
	while (true)
	{
		// reset the number of cards in the player's hand
		cardCount = 0;
		int bet = 0;
		std::wcout << L"How much would you like to bet this hand?" << std::endl;

		std::cin >> bet;
		if (std::cin.fail())			// verify player input
		{
			std::wcout << L"Illegal input." << std::endl;
			continue;
		}

		// deal the first card and increment the card count
		cards[cardCount++] = dealCard();
		// print all cards in the player's hand to the console
		printCards(cards, cardCount);
		// calculate the sum of all cards in the player's hand
		int total = calculateTotal(cards, cardCount);

		// player can keep drawing cards until total is over 21
		while (total < 21)
		{
			std::wcout << "(H)it or (S)tand? ";
			char command;
			std::cin >> command;
			if (std::cin.fail())
			{
				std::wcout << L"Illegal input." << std::endl;
				continue;
			}

			// if player wants a new card, deal card
			if (command == 'h' || command == 'H')
			{
				// draw a new card, output to console, calculate total
				cards[cardCount++] = dealCard();
				printCards(cards, cardCount);
				total = calculateTotal(cards, cardCount);
			}
			else {
				// any input except 'hit' will be interpreted as 'stand'
				break;
			}
		}

		// this is a super simple blackjack program, so just simulate the dealer's hand
		// this will just generate a believable random number for the dealer
		int dealer = 10 + (rand() % 15);

		// check the dealer's and player's totals against each other to see who won
		if (total > 21)
		{
			std::wcout << L"You bust, dealer wins." << std::endl;
			money -= bet;
		}
		else if (dealer > 21)
		{
			std::wcout << L"Dealer busts, you win." << std::endl;
			money += bet;
		}
		else if (dealer > total)
		{
			std::wcout << L"Dealer wins." << std::endl;
			money -= bet;
		}
		else
		{
			std::wcout << L"You win." << std::endl;
			money += bet;
		}

		// display result, and ask player if they want to play again
		std::wcout << L"You have " << money << " in the bank." << std::endl;
		std::wcout << L"Play again? (Y/N)" << std::endl;
		char command;
		std::cin >> command;
		if (!(command == 'y' || command == 'Y'))
			break;
	}
}

