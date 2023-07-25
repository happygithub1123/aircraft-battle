# include <stdio.h>
# include <graphics.h>
# include <conio.h>
# include <time.h>
# include <stdlib.h>
# include <mmsystem.h> //������������ͷ�ļ�
# pragma comment(lib,"winmm.lib") //��������
/*
* 2023��7��22��22:33:40
* easyx��һ��ͼ�ο���ڿ�����Ϸ����
* ������Ա��wkx
* ��Ŀ���ƣ��ɻ���ս
*/

//ȫ�ָı䴰��,�ӵ����ɻ�����
enum My
{
	WIDTH = 500, //���ڿ��
	HEIGHT = 888, //���ڸ߶�
	BULLET_NUM = 15, //����ӵ�����
	ENEMY_NUM = 10, // �л�������
	BIG, //��ɻ�
	SMALL //С�ɻ�
};

//�ɻ��ṹ���� �ӵ��ṹ���� �л��ɻ��ṹ����
struct Plance 
{
	int x;  //x����
	int y; //y����
	bool live; //�Ƿ���
	int width; //�ɻ����
	int height; //�ɻ��߶�
	int hp; //�ɻ�Ѫ��
	int type; //�л����� big, small
}player, bull[BULLET_NUM], enemy[ENEMY_NUM]; //player������� bull[BULLET_NUM]�����ӵ���enemy[ENEMY_NUM]����л�����

DWORD PlayMusic(LPVOID lp); //���̲߳����ӵ�
IMAGE bk; //���汳��ͼƬ ����ͼƬ��С520 * 800
IMAGE img_role[2]; //�������ͼƬ
IMAGE img_bull[2]; //�����ӵ�ͼƬ
IMAGE img_enemy[2][2]; //����л�ͼƬ

//����ͼƬ����
void loadImg() 
{
	
	loadimage(&bk, "./images/R-C.jpg");//���ر���ͼƬ

	loadimage(&img_role[0], "./images/planeNormal_1.jpg"); //�������1����ͼƬ
	loadimage(&img_role[1], "./images/planeNormal_2.jpg"); //�������1��ʵ��ͼƬ

	loadimage(&img_bull[0], "./images/bullet1.jpg"); //�����ӵ�1����ͼƬ
	loadimage(&img_bull[1], "./images/bullet2.jpg"); //�����ӵ�1��ʵ��ͼƬ

	loadimage(&img_enemy[0][0], "./images/enemyPlane1.jpg"); //����big�л�����ͼƬ
	loadimage(&img_enemy[0][1], "./images/enemyPlane2.jpg"); //����big�л�ʵ��ͼƬ
	loadimage(&img_enemy[1][0], "./images/enemy_1.jpg"); //����small�л�����ͼƬ
	loadimage(&img_enemy[1][1], "./images/enemy_2.jpg"); //����small�л�ʵ��ͼƬ
} 

//ȷ���ɻ������ͺ��� ��big����small
void enemyHp(int i)
{
	int flag = rand() % 10;
	if (flag >= 1 && flag <=2) //0~9
	{
		enemy[i].type = BIG;
		enemy[i].hp = 3;
		enemy[i].width = 104;
		enemy[i].height = 148;
	
	}
	else
	{
		enemy[i].type = SMALL;
		enemy[i].hp = 1;
		enemy[i].width = 52;
		enemy[i].height = 39;
	}
}

//��ʼ����Ϸ���Ժ���
void gameInit()
{
	mciSendString("open ./images/wkx.mp3 alias bgBGM",NULL,NULL,NULL); //����Ϸ alias ȡ����
	mciSendString("play bgBGM", NULL, NULL, NULL); //���ű�������
	loadImg(); //���ر���ͼ
	player.x = WIDTH / 2 - 58; //��ʼ�����1��x����
	player.y = HEIGHT - 200; //��ʼ�����1��y����
	player.live = true;  //��ʼ����ҵ�ǰ״̬�Ǵ��״̬

	for (int i = 0; i < BULLET_NUM; i++) //��ʼ���ӵ�
	{
		bull[i].x = 0;
		bull[i].y = 0;
		bull[i].live = false;
	}

	for (int i = 0; i < ENEMY_NUM; i++)//��ʼ���л�
	{
		enemy[i].live = false; //�л���ʼ״̬
		enemyHp(i);
	}
}

//��Ϸ�Ļ��ƺ���
void gameDraw()
{
	
	putimage(0, 0, &bk);  //�ѱ���ͼƬ����������
	putimage(player.x, player.y, &img_role[0],NOTSRCERASE); //ճ�����1����ͼ
	putimage(player.x, player.y, &img_role[1],SRCINVERT); //ճ�����1ʵ��ͼ

	for (int i = 0; i < BULLET_NUM; i++) //�����ӵ�
	{
		if (bull[i].live) //�ж��Ƿ�Ϊ�棬Ϊ������ӵ���Ϊ�ٲ������ӵ�
		{
			putimage(bull[i].x, bull[i].y, &img_bull[0], NOTSRCERASE);
			putimage(bull[i].x, bull[i].y, &img_bull[1], SRCINVERT);
		}
	}

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].live)
		{
			if (enemy[i].type == BIG) //���ƴ�ɻ�
			{
				putimage(enemy[i].x,enemy[i].y,&img_enemy[0][0], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &img_enemy[0][1], SRCINVERT);
			}
			else  //����С�ɻ�
			{
				putimage(enemy[i].x, enemy[i].y, &img_enemy[1][0], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &img_enemy[1][1], SRCINVERT);
			}
		}
	}
}

void createBullet() //�����ӵ�����
{
	for (int i = 0; i < BULLET_NUM; i++) //��ʼ���ӵ�
	{
		if (!bull[i].live)
		{
			bull[i].x = player.x+48;
			bull[i].y = player.y;
			bull[i].live = true;
			break;
		}
	}
}

//�ӵ��ƶ�����
void BulletMove(int speed)
{
	for (int i = 0; i < BULLET_NUM; i++) //��ʼ���ӵ�
	{
		if (bull[i].live)
		{
			bull[i].y -= speed; //�ӵ��ƶ���������1
			if (bull[i].y < 0)  //����ѭ������ӵ�
			{
				bull[i].live = false;
			}
		}
	}
}

//��ʱ������
BOOL Timer(int ms, int id)
{
	static DWORD t[10];
	if (clock() - t[id] > ms)
	{
		t[id] = clock();
		return true;
	}
	return false;
}


//��ɫ�ƶ�����
void playerMove(int speed)
{
#if 0
	if (_kbhit())
	{
		// ���ַ���
			//��һ�� _getch() ������������scanfһ�������û�����룬�ͻῨס������һֱ�ȴ����룬���������������C���Ա�׼��������Ҫ����ͷ�ļ�conio.h 
		char key = _getch();
		switch (key)
		{
		case 'w':   //�����ƶ�
		case 'W':
			player.y -= speed;
			break;
		case 's': // �����ƶ�
		case 'S':
			player.y += speed;
			break;
		case 'a': // �����ƶ�
		case 'A':
			player.x -= speed;
			break;
		case 'd': //�����ƶ�
		case 'D':
			player.x += speed;
			break;
		}
	}


#elif 1
	//������������ǳ�������
    //�ڶ��ַ�ʽ ʹ��windows�е�GetAsyncKeyState()������ȡ��������
    //��������ĸ�����������ô��֣������Ĵ�дСд�����Լ�⵽�������Сд����һ������ⲻ��
	if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W'))
	{
		if (player.y >= 0) //����ϱ߽紦��
		{
			player.y -= speed;
		}	
	}
	if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S'))
	{
		if(player.y <= HEIGHT-118) // ����±߽紦��
		player.y += speed;
	}
	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A'))
	{
		if (player.x+55 >= 0) //�����߽紦��
		{
			player.x -= speed;
		}
	}
	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D'))
	{
		if (player.x <= WIDTH-55) //����ұ߽紦��
		{
			player.x += speed;
		}	
	}
#endif // 0

	if (GetAsyncKeyState(VK_SPACE)&& Timer(10,1))//����Ƿ��м��̰��£�����м��̰����򷵻��棬���򷵻ؼ�
	{ //GetAsyncKeyState(VK_SPACE)&& Timer(10,1) �������ӵ������Ƶ�� ��spaceһ��10msһ���ӵ�
	/*
	        _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
			_In_ SIZE_T dwStackSize,
			_In_ LPTHREAD_START_ROUTINE lpStartAddress,
			_In_opt_ __drv_aliasesMem LPVOID lpParameter,
			_In_ DWORD dwCreationFlags,
			_Out_opt_ LPDWORD lpThreadId
	*/
		HANDLE threadID = CreateThread(NULL, 0, PlayMusic,NULL,0, 0);; //���̲߳������� ����һ�����HANDLE threadID
		CloseHandle(threadID); //������֮��رվ��
		createBullet();//����һ���ӵ�
	}
	
}

//�л��ƶ�����
void enemyMove(int speed)
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].live)
		{
			enemy[i].y += speed;
			if (enemy[i].y > HEIGHT)
			{
				enemy[i].live = false;
			}
		}
	}
}

//�����л�����
void createEnemy()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].live)
		{
			enemy[i].live = true;
			
			enemy[i].x = rand() % (WIDTH - 60);
			enemy[i].y = 0;
			enemyHp(i); //Ѫ������
			printf("�й������꣡pos(%d,%d) %d %d \n", enemy[i].x, enemy[i].y, enemy[i].live, enemy[i].hp);
			break;
		}
	}
}

//��ɻ�
void playPlance()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].live) //�ж��Ƿ����
			continue;
		for (int k = 0; k < BULLET_NUM; k++)
		{
			if (!bull[k].live)
				continue;
			if (bull[k].x > enemy[i].x && bull[k].x<enemy[i].x + enemy[i].width
				&& bull[k].y>enemy[i].y && bull[k].y < enemy[i].y + enemy[i].height)
			{
				bull[k].live = false;
				enemy[i].hp--;
			}
		}
		if (enemy[i].hp <= 0) //�л�����
		{
			enemy[i].live = false;
		}
	}
}

void showenemy()
{
	for (int i = 0; i < ENEMY_NUM; i++)
		printf("pos(%d,%d) %d %d \n",enemy[i].x,enemy[i].y,enemy[i].live,enemy[i].hp);
}


//������
int main() 
{
	//����һ������ ����̨�������Զ������ģ�ͼ�δ����Լ��ֶ�����
	initgraph(WIDTH, HEIGHT, SHOWCONSOLE);//��ʼ�����ڴ�С��ͼƬ��С����520 * 800�����һ���������Խ�����̨���ݴ�ӡ
	gameInit(); //��ʼ����ϷһЩ����

	BeginBatchDraw();//��ʼ˫�����ͼ
	while(1) //ʹ���ɵĴ�����ͣ��windows
	{
		gameDraw(); //����ͼƬ����
		FlushBatchDraw(); //ˢ��һ�¼��ص�ͼƬ
		playerMove(1); //��ɫ�ƶ�
		BulletMove(5); //�ӵ��ƶ�
		//printf("�٣����ǿ������~\n");
		//createEnemy(); 
		if ( Timer(500,0))
		{
			createEnemy();
		}
		

		if (Timer(10, 2)) // �л��ƶ�ʱ����
		{
			enemyMove(1); //�л��ƶ��ٶ�			
		}

		playPlance(); //����ɻ�
	}
	EndBatchDraw();//����˫�����ͼ
	return 0;
}

//���̲߳�������
DWORD PlayMusic(LPVOID lp)
{
	mciSendString("close shoot", NULL, NULL, NULL);
	mciSendString("open ./images/explode.mp3 alias shoot", NULL, NULL, NULL); //����Ϸ alias ȡ����
	mciSendString("play shoot", NULL, NULL, NULL); //�����ӵ�����
	return NULL;
}

