#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#include<easyx.h>

enum wh
{
	bgwidth = 598,
	bgheight = 1065,
	bullet_num = 15,
	enemy_num = 10,
	BIG,
	SMALL
};

struct plane
{
	int x;
	int y;
	int width;
	int height;
	int hp;
	int type;
	bool live; 
}player, bullet[bullet_num], enemy[enemy_num];

void enemyHP(int i)
{
	int flag = rand() % 10;
	if (flag >= 0 && flag <= 1)
	{
		enemy[i].type = BIG;
		enemy[i].hp = 3;
		enemy[i].width = 100;
		enemy[i].height = 100;
	}
	else
	{
		enemy[i].type = SMALL;
		enemy[i].hp = 1;
		enemy[i].width = 80;
		enemy[i].height = 80;
	}
}

void game_csh() 
{
	player.x = bgwidth / 2 - 40;
	player.y = bgheight - 120;
	player.live = true;
	player.width = 80;
	player.height = 80;
	for (int i = 0; i < bullet_num; i++)
	{
		bullet[i].x = 0;
		bullet[i].y = 0;
		bullet[i].live = false;
	}
	for (int i = 0; i < enemy_num; i++)
	{
		enemy[i].live = false;
		enemyHP(i);
	}
}
IMAGE bg;
IMAGE t_player;
IMAGE t_bullet;
IMAGE t_enemy1;
IMAGE t_enemy2;

void Load(){
	loadimage(&bg,"bg.jpg" );
	loadimage(&t_player,"p.jpg",80,80);
	loadimage(&t_bullet,"b.jpg",50,10);
	loadimage(&t_enemy1,"e1.jpg",80,80);
	loadimage(&t_enemy2,"e2.jpg",100,100);
}				  
				   
void gamedraw()
{
	Load();

	putimage(0, 0, &bg);
	putimage(player.x, player.y, &t_player);
	for (int i = 0; i < bullet_num; i++)
	{
		if (bullet[i].live)
		{
			putimage(bullet[i].x, bullet[i].y, &t_bullet);
		}
	}
	for (int i = 0; i < enemy_num; i++)
	{
		if (enemy[i].type == BIG)
		{
			putimage(enemy[i].x, enemy[i].y, & t_enemy2);
		}
		else putimage(enemy[i].x, enemy[i].y, & t_enemy1);
	}
}

bool timer(int ms, int id)
{
	static DWORD t[20];
	if (clock() - t[id] > ms)
	{
		t[id] = clock();
		return true;
	}
	return false;
}

void bullet_creat()
{
	for (int i = 0; i < bullet_num; i++)
	{
		if (bullet[i].live == false)
		{
			bullet[i].x = player.x + 35;
			bullet[i].y = player.y;
			bullet[i].live = true;
			break;
		}
	}
}

void enemy_creat()
{
	for (int i = 0; i < enemy_num; i++)
	{
		if (!enemy[i].live)
		{
			enemy[i].live = true;
			enemy[i].x = rand() % (bgwidth - 100);
			enemy[i].y = 0;
			enemyHP(i);
			break;
		}
	}
}

void move_b(int speed)
{
	if (GetAsyncKeyState(VK_SPACE) && timer(300, 1))
	{
		bullet_creat();
	}
	for (int i = 0; i < bullet_num; i++)
	{
		if (bullet[i].live)
		{
			bullet[i].y -= speed;
			if (bullet[i].y < 0)
			{
				bullet[i].live = false;
			}
		}
	}
}

void move_e(int speed)
{
	for (int i = 0; i < enemy_num; i++)
	{
		if (enemy[i].live)
		{
			enemy[i].y += speed;
			if (enemy[i].y > bgheight)
			{
				enemy[i].live = false;
			}
		}
	}
}

void move_p(int speed)
{
#if  0  if(_kbhit())
	{
		char key = _getch();
		switch (key)
		{
		case'w':
		case'W':
			player.y -= speed;
			break;
		case's':
		case'S':
			player.y += speed;
			break;
		case'a':
		case'A':
			player.x -= speed;
			break;
		case'd':
		case'D':
			player.x += speed;
			break;
		}
	}
#elif 1
	if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W'))
	{
		if (player.y > 0)player.y -= speed;
	}
	if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S'))
	{
		if (player.y < bgheight - 100)player.y += speed;
	}
	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A'))
	{
		if (player.x > -50)player.x -= speed;
	}
	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D'))
	{
		if (player.x < bgwidth)player.x += speed;
	}
#endif
}

void hit()
{
	for (int i = 0; i < enemy_num; i++)
	{
		if (!enemy[i].live)
			continue;
		for (int j = 0; j < bullet_num; j++)
		{
			if (!bullet[j].live)
				continue;
			if (bullet[j].x > enemy[i].x &&
				bullet[j].x < enemy[i].x + enemy[i].width &&
				bullet[j].y> enemy[i].y &&
				bullet[j].y < enemy[i].y + enemy[i].height)
			{
				bullet[j].live = false;
				enemy[i].hp--;
			}
		}
		if (enemy[i].hp <= 0)enemy[i].live = false;
	}
	for (int i = 0; i < enemy_num; i++)
	{
		if (player.x + player.width >= enemy[i].x &&
			player.x <= enemy[i].x + enemy[i].width &&
			player.y <= enemy[i].y + enemy[i].height &&
			player.y + player.height >= enemy[i].y)
		{
			player.live = false;
		}
	}
	if (player.live == false)
	{
		exit(1);
	}
}

int main()
{
	initgraph(bgwidth, bgheight);
	gamedraw();
	game_csh();
	while (1)
	{
		
		move_p(2);
		move_b(2);
		if (timer(500, 0))  enemy_creat();
		if (timer(5, 2))   move_e(1);

		hit();
	}
	return 0;
}