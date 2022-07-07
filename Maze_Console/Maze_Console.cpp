#include <iostream>
#include <Windows.h>
#include <conio.h>

int main()
{
	srand((unsigned)time(0));

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	int random_int = 0;
	int start = 0;
	int finish = 0;
	int coins = 0;
	int hp = 5;

	const int width = 50;
	const int height = 15;

	int maze[height][width]{ 0 };

	// генерация
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (y == 0 || y == height - 1 || x == 0 || x == width - 1)
			{
				maze[y][x] = 1;
			}
			else
			{
				random_int = rand() % 101;

				if (random_int <= 80)
				{
					maze[y][x] = 0;
				}
				else if (random_int > 80 && random_int <= 90)
				{
					maze[y][x] = 1;
				}
				else if (random_int > 90 && random_int <= 95)
				{
					maze[y][x] = 2;
				}
				else if (random_int > 95)
				{
					maze[y][x] = 3;
				}
			}
			do
			{
				start = rand() % height;
			} while (maze[start][1] == 1);
			do
			{
				finish = rand() % height;
			} while (maze[finish][width - 2] == 1);
		}
	}

	/*
	0 - коридор
	1 - стена
	2 - монетка
	3 - враг
	*/

	// показ элементов массива
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			switch (maze[y][x])
			{
			case 0:
				SetConsoleTextAttribute(h, 15);
				std::cout << " ";
				SetConsoleTextAttribute(h, 15);
				break;
			case 1:
				SetConsoleTextAttribute(h, 15);
				if (y == start && x == 0)
				{
					SetConsoleTextAttribute(h, 11);
				}
				else if (y == finish && x == width - 1)
				{
					SetConsoleTextAttribute(h, 11);
				}
				else if (y == 0 || y == height - 1 || x == 0 || x == width - 1)
				{
					SetConsoleTextAttribute(h, 9);
				}
				std::cout << (char)178;
				SetConsoleTextAttribute(h, 15);
				break;
			case 2:
				SetConsoleTextAttribute(h, 10);
				std::cout << "$";
				SetConsoleTextAttribute(h, 15);
				break;
			case 3:
				SetConsoleTextAttribute(h, 12);
				std::cout << (char)1;
				SetConsoleTextAttribute(h, 15);
				break;
			case 4:
				SetConsoleTextAttribute(h, 9);
				std::cout << (char)1;
				SetConsoleTextAttribute(h, 15);
				break;
			}
		}
		std::cout << "\n";
	}

	COORD smile;
	smile.X = 0; // поля структуры
	smile.Y = start;
	SetConsoleCursorPosition(h, smile);
	SetConsoleTextAttribute(h, 9);
	std::cout << (char)1;
	maze[smile.Y][smile.X] = 4;

	COORD info;
	info.X = 0;
	info.Y = height + 1;
	SetConsoleCursorPosition(h, info);

	COORD coins_info;
	coins_info.X = 0;
	coins_info.Y = height + 2;
	SetConsoleCursorPosition(h, coins_info);

	COORD hp_info;
	hp_info.X = 0;
	hp_info.Y = height + 3;
	SetConsoleCursorPosition(h, hp_info);

	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = false;
	cursor.dwSize = 100;
	SetConsoleCursorInfo(h, &cursor);

	bool player_won = false;
	bool has_health = true;

	while (!player_won && has_health)
	{
		if (hp > 0)
		{
			SetConsoleTextAttribute(h, 15);
			std::string str = "                                    ";

			int code = _getch();
			if (code == 224) code = _getch();

			SetConsoleCursorPosition(h, smile);
			std::cout << " ";

			// enemy
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					if (maze[y][x] == 3)
					{
						int move = rand() % 4;

						COORD enemy;
						enemy.X = x;
						enemy.Y = y;

						COORD enemy_past;
						enemy_past.X = enemy.X;
						enemy_past.Y = enemy.Y;

						bool moved = false;

						switch (move)
						{
							//Left
						case 0:
							if (maze[enemy.Y][enemy.X - 1] != 1)
							{
								enemy.X--;
								moved = true;
							}
							break;
							//Right
						case 1:
							if (maze[enemy.Y][enemy.X + 1] != 1)
							{
								enemy.X++;
								moved = true;
							}
							break;
							//Up
						case 2:
							if(maze[enemy.Y - 1][enemy.X] != 1)
							{
								enemy.Y--;
								moved = true;
							}
							break;
							//Down
						case 3:
							if (maze[enemy.Y + 1][enemy.X] != 1)
							{
								enemy.Y++;
								moved = true;
							}
							break;
						}
						if (moved)
						{
							maze[enemy.Y][enemy.X] = 3;
							maze[enemy_past.Y][enemy_past.X] = 0;

							SetConsoleCursorPosition(h, enemy);

							SetConsoleTextAttribute(h, 12);
							std::cout << (char)1;
							SetConsoleTextAttribute(h, 15);

							SetConsoleCursorPosition(h, enemy_past);

							SetConsoleTextAttribute(h, 15);
							std::cout << " ";
							SetConsoleTextAttribute(h, 15);
						}
					}
				}
			}

			//player
			switch (code)
			{
				// Left
			case 75:
				if (maze[smile.Y][smile.X - 1] == 1)
				{
					str = "I cant go throught walls :(";
				}
				else if (maze[smile.Y][smile.X - 1] == 2)
				{
					coins++;
					smile.X--;
				}
				else if (maze[smile.Y][smile.X - 1] == 3)
				{
					hp--;
					smile.X--;
				}
				else
				{
					smile.X--;
				}
				break;
				// Right
			case 77:
				if (maze[smile.Y][smile.X + 1] == 1)
				{
					if (smile.Y == finish && smile.X + 1 == width - 1)
					{
						str = "Yeeeehooo, you won the game!";
						player_won = true;
						break;
					}
					else
					{
						str = "I cant go throught walls :(";
					}
				}
				else if (maze[smile.Y][smile.X + 1] == 2)
				{
					coins++;
					smile.X++;
				}
				else if (maze[smile.Y][smile.X + 1] == 3)
				{
					hp--;
					smile.X++;
				}
				else
				{
					smile.X++;
				}
				break;
				// Up
			case 72:
				if (maze[smile.Y - 1][smile.X] == 1)
				{
					str = "I cant go throught walls :(";
				}
				else if (maze[smile.Y - 1][smile.X] == 2)
				{
					coins++;
					smile.Y--;
				}
				else if (maze[smile.Y - 1][smile.X] == 3)
				{
					hp--;
					smile.Y--;
				}
				else
				{
					smile.Y--;
				}
				break;
				// Down
			case 80:
				if (maze[smile.Y + 1][smile.X] == 1)
				{
					str = "I cant go throught walls :(";
				}
				else if (maze[smile.Y + 1][smile.X] == 2)
				{
					coins++;
					smile.Y++;
				}
				else if (maze[smile.Y + 1][smile.X] == 3)
				{
					hp--;
					smile.Y++;
				}
				else
				{
					smile.Y++;
				}
				break;
			}

			SetConsoleTextAttribute(h, 9);
			SetConsoleCursorPosition(h, smile);
			std::cout << (char)1;
			maze[smile.Y][smile.X] = 4;

			SetConsoleCursorPosition(h, info);
			std::cout << str << "\n";

			SetConsoleTextAttribute(h, 10);
			SetConsoleCursorPosition(h, coins_info);
			std::cout << "Coins: " << coins;

			SetConsoleTextAttribute(h, 12);
			SetConsoleCursorPosition(h, hp_info);
			std::cout << "Hp: " << hp;

			SetConsoleTextAttribute(h, 15);
		}
		else
		{
			std::string str = "                                    ";
			str = "You lose :(";

			SetConsoleTextAttribute(h, 9);
			SetConsoleCursorPosition(h, info);
			std::cout << str << "\n";

			has_health = false;
		}
	}

	Sleep(INFINITE);
}