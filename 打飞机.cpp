# include <stdio.h>
# include <graphics.h>
# include <conio.h>
# include <time.h>
# include <stdlib.h>
# include <mmsystem.h> //加载音乐所需头文件
# pragma comment(lib,"winmm.lib") //加载音乐
/*
* 2023年7月22日22:33:40
* easyx是一种图形库便于开发游戏界面
* 开发人员：wkx
* 项目名称：飞机大战
*/

//全局改变窗口,子弹，飞机属性
enum My
{
	WIDTH = 500, //窗口宽度
	HEIGHT = 888, //窗口高度
	BULLET_NUM = 15, //玩家子弹数量
	ENEMY_NUM = 10, // 敌机的数量
	BIG, //大飞机
	SMALL //小飞机
};

//飞机结构属性 子弹结构属性 敌机飞机结构属性
struct Plance 
{
	int x;  //x坐标
	int y; //y坐标
	bool live; //是否存活
	int width; //飞机宽度
	int height; //飞机高度
	int hp; //飞机血量
	int type; //敌机类型 big, small
}player, bull[BULLET_NUM], enemy[ENEMY_NUM]; //player代表玩家 bull[BULLET_NUM]代表子弹，enemy[ENEMY_NUM]代表敌机数量

DWORD PlayMusic(LPVOID lp); //多线程播放子弹
IMAGE bk; //保存背景图片 背景图片大小520 * 800
IMAGE img_role[2]; //保存玩家图片
IMAGE img_bull[2]; //保存子弹图片
IMAGE img_enemy[2][2]; //保存敌机图片

//加载图片函数
void loadImg() 
{
	
	loadimage(&bk, "./images/R-C.jpg");//加载背景图片

	loadimage(&img_role[0], "./images/planeNormal_1.jpg"); //加载玩家1背景图片
	loadimage(&img_role[1], "./images/planeNormal_2.jpg"); //加载玩家1的实体图片

	loadimage(&img_bull[0], "./images/bullet1.jpg"); //加载子弹1背景图片
	loadimage(&img_bull[1], "./images/bullet2.jpg"); //加载子弹1的实体图片

	loadimage(&img_enemy[0][0], "./images/enemyPlane1.jpg"); //加载big敌机背景图片
	loadimage(&img_enemy[0][1], "./images/enemyPlane2.jpg"); //加载big敌机实体图片
	loadimage(&img_enemy[1][0], "./images/enemy_1.jpg"); //加载small敌机背景图片
	loadimage(&img_enemy[1][1], "./images/enemy_2.jpg"); //加载small敌机实体图片
} 

//确定飞机的类型函数 是big还是small
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

//初始化游戏属性函数
void gameInit()
{
	mciSendString("open ./images/wkx.mp3 alias bgBGM",NULL,NULL,NULL); //打开游戏 alias 取别名
	mciSendString("play bgBGM", NULL, NULL, NULL); //播放背景音乐
	loadImg(); //加载背景图
	player.x = WIDTH / 2 - 58; //初始化玩家1的x坐标
	player.y = HEIGHT - 200; //初始化玩家1的y坐标
	player.live = true;  //初始化玩家当前状态是存活状态

	for (int i = 0; i < BULLET_NUM; i++) //初始化子弹
	{
		bull[i].x = 0;
		bull[i].y = 0;
		bull[i].live = false;
	}

	for (int i = 0; i < ENEMY_NUM; i++)//初始化敌机
	{
		enemy[i].live = false; //敌机初始状态
		enemyHp(i);
	}
}

//游戏的绘制函数
void gameDraw()
{
	
	putimage(0, 0, &bk);  //把背景图片贴到窗口上
	putimage(player.x, player.y, &img_role[0],NOTSRCERASE); //粘贴玩家1背景图
	putimage(player.x, player.y, &img_role[1],SRCINVERT); //粘贴玩家1实体图

	for (int i = 0; i < BULLET_NUM; i++) //绘制子弹
	{
		if (bull[i].live) //判断是否为真，为真绘制子弹，为假不绘制子弹
		{
			putimage(bull[i].x, bull[i].y, &img_bull[0], NOTSRCERASE);
			putimage(bull[i].x, bull[i].y, &img_bull[1], SRCINVERT);
		}
	}

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].live)
		{
			if (enemy[i].type == BIG) //绘制大飞机
			{
				putimage(enemy[i].x,enemy[i].y,&img_enemy[0][0], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &img_enemy[0][1], SRCINVERT);
			}
			else  //绘制小飞机
			{
				putimage(enemy[i].x, enemy[i].y, &img_enemy[1][0], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &img_enemy[1][1], SRCINVERT);
			}
		}
	}
}

void createBullet() //创建子弹函数
{
	for (int i = 0; i < BULLET_NUM; i++) //初始化子弹
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

//子弹移动函数
void BulletMove(int speed)
{
	for (int i = 0; i < BULLET_NUM; i++) //初始化子弹
	{
		if (bull[i].live)
		{
			bull[i].y -= speed; //子弹移动步数设置1
			if (bull[i].y < 0)  //可以循环输出子弹
			{
				bull[i].live = false;
			}
		}
	}
}

//定时器函数
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


//角色移动函数
void playerMove(int speed)
{
#if 0
	if (_kbhit())
	{
		// 两种方法
			//第一种 _getch() 阻塞函数，和scanf一样，如果没有输入，就会卡住主程序，一直等待输入，并且这个函数不是C语言标准函数，需要包含头文件conio.h 
		char key = _getch();
		switch (key)
		{
		case 'w':   //向上移动
		case 'W':
			player.y -= speed;
			break;
		case 's': // 向下移动
		case 'S':
			player.y += speed;
			break;
		case 'a': // 向左移动
		case 'A':
			player.x -= speed;
			break;
		case 'd': //向右移动
		case 'D':
			player.x += speed;
			break;
		}
	}


#elif 1
	//非阻塞函数会非常的流畅
    //第二种方式 使用windows中的GetAsyncKeyState()函数获取键盘输入
    //如果检测字母按键，必须用大字，这样的大写小写都可以检测到，如果用小写，，一个都检测不到
	if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W'))
	{
		if (player.y >= 0) //玩家上边界处理
		{
			player.y -= speed;
		}	
	}
	if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S'))
	{
		if(player.y <= HEIGHT-118) // 玩家下边界处理
		player.y += speed;
	}
	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A'))
	{
		if (player.x+55 >= 0) //玩家左边界处理
		{
			player.x -= speed;
		}
	}
	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D'))
	{
		if (player.x <= WIDTH-55) //玩家右边界处理
		{
			player.x += speed;
		}	
	}
#endif // 0

	if (GetAsyncKeyState(VK_SPACE)&& Timer(10,1))//检测是否有键盘按下，如果有键盘按下则返回真，否则返回假
	{ //GetAsyncKeyState(VK_SPACE)&& Timer(10,1) 按键与子弹发射的频率 按space一下10ms一个子弹
	/*
	        _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
			_In_ SIZE_T dwStackSize,
			_In_ LPTHREAD_START_ROUTINE lpStartAddress,
			_In_opt_ __drv_aliasesMem LPVOID lpParameter,
			_In_ DWORD dwCreationFlags,
			_Out_opt_ LPDWORD lpThreadId
	*/
		HANDLE threadID = CreateThread(NULL, 0, PlayMusic,NULL,0, 0);; //多线程播放音乐 返回一个句柄HANDLE threadID
		CloseHandle(threadID); //播放完之后关闭句柄
		createBullet();//创建一个子弹
	}
	
}

//敌机移动函数
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

//产生敌机函数
void createEnemy()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].live)
		{
			enemy[i].live = true;
			
			enemy[i].x = rand() % (WIDTH - 60);
			enemy[i].y = 0;
			enemyHp(i); //血量重置
			printf("中国人万岁！pos(%d,%d) %d %d \n", enemy[i].x, enemy[i].y, enemy[i].live, enemy[i].hp);
			break;
		}
	}
}

//打飞机
void playPlance()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].live) //判断是否存在
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
		if (enemy[i].hp <= 0) //敌机生命
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


//主函数
int main() 
{
	//创建一个窗口 控制台窗口是自动创建的，图形窗口自己手动创建
	initgraph(WIDTH, HEIGHT, SHOWCONSOLE);//初始化窗口大小和图片大小都是520 * 800，最后一个参数可以将控制台数据打印
	gameInit(); //初始化游戏一些数据

	BeginBatchDraw();//开始双缓冲绘图
	while(1) //使生成的窗口暂停在windows
	{
		gameDraw(); //背景图片加载
		FlushBatchDraw(); //刷新一下加载的图片
		playerMove(1); //角色移动
		BulletMove(5); //子弹移动
		//printf("嘿！我是开心玩家~\n");
		//createEnemy(); 
		if ( Timer(500,0))
		{
			createEnemy();
		}
		

		if (Timer(10, 2)) // 敌机移动时间间隔
		{
			enemyMove(1); //敌机移动速度			
		}

		playPlance(); //消灭飞机
	}
	EndBatchDraw();//结束双缓冲绘图
	return 0;
}

//多线程播放音乐
DWORD PlayMusic(LPVOID lp)
{
	mciSendString("close shoot", NULL, NULL, NULL);
	mciSendString("open ./images/explode.mp3 alias shoot", NULL, NULL, NULL); //打开游戏 alias 取别名
	mciSendString("play shoot", NULL, NULL, NULL); //播放子弹音乐
	return NULL;
}

