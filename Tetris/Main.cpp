#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
using namespace sf;

#define IS_TRUE(x) { if (!(x)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; }

const int mapVertical = 20; 
const int mapHorizontal = 10;

bool gameOver = false;

int gameMap[mapVertical][mapHorizontal] = { 0 };

struct Point 
{int x, y;} a[4], b[4];

int figures[6][4] =
{
	1,3,5,7,
	3,5,7,6,
	3,5,4,6,
	2,4,5,7,
	2,3,5,7,
	3,5,4,7,
};

bool borderCheck()
{
	for (int i = 0; i < 4; i++)
	{
		if (a[i].x < 0 || a[i].x >= mapHorizontal || a[i].y >= mapVertical)
		{
			return 0;
		}
		else if (gameMap[a[i].y][a[i].x])
		{
			return 0;
		}
	}
}

void test_borderCheck()
{
	IS_TRUE(!borderCheck());
}

int main()
{
	srand(time(0));

	RenderWindow window(VideoMode(360, 720), "Tetris");
	Texture tetrisTexture;
	tetrisTexture.loadFromFile("C:/Users/Slimas/Desktop/tetris.png");
	Sprite colorFragment(tetrisTexture);
	colorFragment.setTextureRect(IntRect(0, 0, 36, 36));

	int move = 0;
	bool rotate = 0;
	int colorNum = 1;
	float timer = 0, delay = 0.3;
	Clock clock;

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				window.close();
			}
	
			if (e.type == Event::KeyPressed)
			{
				if (e.key.code == Keyboard::Up)
				{
					rotate = true;
				}
				else if (e.key.code == Keyboard::Left)
				{
					move = -1;
				}
				else if (e.key.code == Keyboard::Right)
				{
					move = 1;
				}
				else if (Keyboard::isKeyPressed(Keyboard::Down))
				{
					delay = 0.05;
				}
			}

			for (int i = 0; i < 4; i++)
			{
				b[i] = a[i];
				a[i].x += move;
			}

			if (!borderCheck())
			{
				for (int i = 0; i < 4; i++)
				{
					a[i] = b[i];
				}
			}
		
			if (rotate)
			{
				Point point = a[1]; 
				for (int i = 0; i < 4; i++)
				{
					int x = a[i].y - point.y;
					int y = a[i].x - point.x;
					a[i].x = point.x - x;
					a[i].y = point.y + y;
				}
				if (!borderCheck())
				{
					for (int i = 0; i < 4; i++)
					{
						a[i] = b[i];
					}
				}
			}
		}

		if (timer > delay)
		{
			for (int i = 0; i < 4; i++)
			{
				b[i] = a[i];
				a[i].y += 1;
			}

			if (!borderCheck())
			{
				int count = 0;
				for (int i = 0; i < 4; i++)
				{
					gameMap[b[i].y][b[i].x] = colorNum;
				}
				colorNum = 1 + rand() % 6;
				int n = rand() % 7;
				for (int i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
					if (gameMap[i][i] != 0)
					{
						gameOver = true;
					}
				}
			}

			timer = 0;
		}

		int temp = mapVertical - 1;
		for (int i = mapVertical-1; i > 0; i--)
		{
			int count = 0;
			for (int j = 0; j < mapHorizontal; j++)
			{
				if (gameMap[i][j])
				{
					count++;
				}
				gameMap[temp][j] = gameMap[i][j];
			}
			if (count < mapHorizontal)
			{
				temp--;
			}
		}

		move = 0; rotate = 0; delay = 0.3;
		window.clear(Color::White);

		for (int i = 0; i < mapVertical; i++)
		{
			for (int j = 0; j < mapHorizontal; j++)
			{
				if (gameMap[i][j] == 0)
				{
					continue;
				}
				colorFragment.setTextureRect(IntRect(gameMap[i][j] * 36, 0, 36, 36));
				colorFragment.setPosition(j * 36, i * 36);
				window.draw(colorFragment);
			}
		}
		for (int i = 0; i < 4; i++)
		{
			colorFragment.setTextureRect(IntRect(colorNum * 36, 0, 36, 36));
			colorFragment.setPosition(a[i].x * 36, a[i].y * 36);
			window.draw(colorFragment);
		}
		
		for (int i = 1; i < mapHorizontal-1; i++)
		{
			if(gameMap[0][i] !=0 )
			{
				gameOver = true;
			}
		}

		if (gameOver == true)
		{
			break;
		}

		window.display();
	}
}