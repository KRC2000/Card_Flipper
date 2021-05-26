#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

enum FruitTypes
{
	TOMATO,
	STRAWBERRY,
	APPLE,
	RASPBERRY,
	PUMPKIN,
	PEAS,
	PEANUT,
	PEACH,
	LEMON,
	COCONUTS,
	CHILLI,
	CARROT,
	CABBAGE,
	PEPER,
	BEET,
	BANANA,
	AVOCADO,
	AUBERGINE,
	AMOUNT,
	NONE
};

struct Card
{
	RectangleShape rect_back;
	RectangleShape rect_foreground;
	FruitTypes type;
	Clock animTimer;

	bool flipped = false;
	bool deleted = false;

	Card(FruitTypes type, Vector2f pos, Vector2f size, vector<Texture>& texturesVec): type(type)
	{
		rect_foreground.setPosition(pos);
		rect_foreground.setSize(size);
		rect_foreground.setTexture(&texturesVec[type]);

		rect_back.setPosition(pos);
		rect_back.setSize(size);
		rect_back.setFillColor(Color::Green);
		rect_back.setOutlineThickness(1);
	};

	void draw(RenderWindow& window)
	{
		if (!deleted)
		{
			window.draw(rect_back);
			if (flipped) window.draw(rect_foreground);
		}
	}

	void setFlipped(bool flip) { flipped = flip; };
};

void loadTextures(vector<Texture>& texturesVec);
void createCardsAccordingToLayout(vector<Card>& cardsVec, vector<Texture>& texturesVec);
void generateRandomCardsLayout();
void cardClick(Card& clickedCard);
void twoCardsFlippedProcessing(vector<Card>& cardsVec);


const int cardsWidthAmount = 6;
const int cardsHeightAmount = 6;
const int cardAmount = 36;
const int cardPairsAmount = 18;
Vector2f cardSize{80, 80};

int cardsStartLayout_arr[cardsWidthAmount][cardsHeightAmount]{};

bool gameOver = false;
int cardsFlipped = 0;
Clock unflipTimer;
Card* firstFlippedCard = nullptr;
Card* secondFlippedCard = nullptr;

Font font;
Text endGameMessage;

int main()
{
	srand(time(NULL));
	RenderWindow window(VideoMode(500, 500), "SFML works!");
	
	generateRandomCardsLayout();

	vector<Texture> texturesVec;
	loadTextures(texturesVec);

	vector<Card> cardsVec;
	createCardsAccordingToLayout(cardsVec, texturesVec);

	font.loadFromFile("PlayfairDisplay-Regular.ttf");
	endGameMessage.setFont(font);
	endGameMessage.setString("You won!\n Press any key to restart");
	endGameMessage.setFillColor({0, 0, 0, 0});


	while (window.isOpen())
	{
		Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::MouseButtonPressed)
			{
				if (event.key.code == Mouse::Left)
				{
					if (cardsFlipped < 2)
					{
						for (auto& card : cardsVec)
						{
							if (card.rect_back.getGlobalBounds().contains(mousePos))
								cardClick(card);
						}
					}
				}
			}
			if (event.type == Event::KeyPressed)
			{
				if (gameOver)
				{
					generateRandomCardsLayout();
					createCardsAccordingToLayout(cardsVec, texturesVec);
					endGameMessage.setFillColor({ 0, 0, 0, 0 });
				}
			}
		}

		twoCardsFlippedProcessing(cardsVec);
		

		window.clear(Color::Cyan);

		for (int i = 0; i < cardsVec.size(); i++)
			cardsVec[i].draw(window);

		window.draw(endGameMessage);

		window.display();
	}

	return 0;
}


void loadTextures(vector<Texture>& texturesVec)
{
	for (int i = 0; i < AMOUNT; i++)
	{
		Texture cardTexture;
		texturesVec.push_back(cardTexture);
	}
	texturesVec[TOMATO].loadFromFile("tomato.png");
	texturesVec[STRAWBERRY].loadFromFile("strawberry.png");
	texturesVec[APPLE].loadFromFile("shiny-apple.png");
	texturesVec[RASPBERRY].loadFromFile("raspberry.png");
	texturesVec[PUMPKIN].loadFromFile("pumpkin.png");
	texturesVec[PEAS].loadFromFile("peas.png");
	texturesVec[PEANUT].loadFromFile("peanut.png");
	texturesVec[PEACH].loadFromFile("peach.png");
	texturesVec[LEMON].loadFromFile("cut-lemon.png");
	texturesVec[COCONUTS].loadFromFile("coconuts.png");
	texturesVec[CHILLI].loadFromFile("chili-pepper.png");
	texturesVec[CARROT].loadFromFile("carrot.png");
	texturesVec[CABBAGE].loadFromFile("cabbage.png");
	texturesVec[PEPER].loadFromFile("bell-pepper.png");
	texturesVec[BEET].loadFromFile("beet.png");
	texturesVec[BANANA].loadFromFile("banana-bunch.png");
	texturesVec[AVOCADO].loadFromFile("avocado.png");
	texturesVec[AUBERGINE].loadFromFile("aubergine.png");
};

void createCardsAccordingToLayout(vector<Card>& cardsVec, vector<Texture>& texturesVec)
{
	cardsVec.clear();

	for (int i = 0; i < cardsWidthAmount; i++)
	{
		for (int k = 0; k < cardsHeightAmount; k++)
		{
			Card card((FruitTypes)cardsStartLayout_arr[i][k], { i * (cardSize.x + 5), k * (cardSize.y + 5) }, cardSize, texturesVec);
			cardsVec.push_back(card);
		}
	}
};


void generateRandomCardsLayout()
{
	int cardsLeft[AMOUNT];
	for (int i = 0; i < AMOUNT; i++)
		cardsLeft[i] = 2;

	for (int i = 0; i < cardsWidthAmount; i++)
	{
		for (int k = 0; k < cardsHeightAmount; k++)
		{
			bool correctCardGenerated = false;
			while (!correctCardGenerated)
			{
				int card = rand() % AMOUNT;
				if (cardsLeft[card] > 0)
				{
					cardsLeft[card]--;
					cardsStartLayout_arr[i][k] = card;
					correctCardGenerated = true;
				}
			}
		}
	}
};


void cardClick(Card& clickedCard)
{
	if (!clickedCard.flipped && !clickedCard.deleted)
	{
		clickedCard.setFlipped(true);
		if (cardsFlipped == 0) firstFlippedCard = &clickedCard;
		if (cardsFlipped == 1) secondFlippedCard = &clickedCard;
		cardsFlipped++;
		if (cardsFlipped == 2) unflipTimer.restart();
	}
};


void twoCardsFlippedProcessing(vector<Card>& cardsVec)
{
	if (cardsFlipped == 2)
	{
		if (unflipTimer.getElapsedTime().asSeconds() > 1)
		{
			if (firstFlippedCard->type == secondFlippedCard->type)
			{
				firstFlippedCard->deleted = true;
				secondFlippedCard->deleted = true;
				firstFlippedCard = secondFlippedCard = nullptr;

				bool allCardsDeleted = true;
				for (auto& card : cardsVec)
				{
					if (!card.deleted) allCardsDeleted = false;
				}
				if (allCardsDeleted)
				{
					endGameMessage.setFillColor({ 0, 0, 0, 255 });
					gameOver = true;
				}
			}
			
			cardsFlipped = 0;
			for (auto& card : cardsVec)
				card.flipped = false;
		}
	}
};
