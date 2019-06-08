#include<stdio.h>
#include<termio.h>
#include<stdlib.h>



void loadMap();
void prtMap();
void command();
int map_Check();
void checkclear();
void Save();
void up();
void down();
void right();
void left();
void New();
void undo();
void file();
void display();//명령어 설명*
//void top();
void undoupdate();//*움직일때마다 undomap밀어서 넣어주기
void undoupdate_first();//* 스테이지 시작할때 undomap초기화해주는 과정
void re();



int stage = 0, move, STAGE=0;
char map[5][30][30];
char undomap[5][30][30];
char Original_map[5][30][30];
int player_x[5],player_y[5];
int max_x[5]={0},max_y[5]={0};
int undocount=5;
char USER[11];
char Copy_map[5][30][30];
int Copy_player_x[5];
int Copy_player_y[5];


int getch(void){
    int ch;
    struct termios buf, save;
    tcgetattr(0,&save);
    buf = save;
    buf.c_lflag &= ~(ICANON|ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;
    tcsetattr(0, TCSAFLUSH, &buf);
    ch = getchar();
    tcsetattr(0, TCSAFLUSH, &save);
    return ch;
}


int main()
{
	int i,j,k;
   int mapcheck;
   loadMap();
   mapcheck=map_Check();//맵 출력 + 보물 갯수 확인해서 맞으면 return 1 틀리면 return 0
        if(!mapcheck)
         {
                 printf("error");
                return 0;
          }

   printf("Start..\nInput name :");
   scanf("%s",USER);
   
   undoupdate_first();
   command();// 명령 받기 창고지기 조정,u,r,n,e,s,f,d,t
   
}

void command()
{
   char usr;
   int i,j,k;

   while(1)
{
	printf("%d\n",stage);
   prtMap();
    
   usr=getch();//자동 반환
   
   switch(usr)
{
   case 'h':
	undoupdate();
    left();
   	break;
         
   case 'j':
	undoupdate();
	down();
   	break;

   case 'k':
   	undoupdate();
    up();
   	break;

   case 'l':
   	undoupdate();
    right();
   	break;

   case 'e':
    Save();
    printf("SEE YOU...%s",USER);
    exit(1);
    break;

   case 's':
	Save();
	break;

   case 'n':
   	New();
   	break;

   case 'u':
   	undo();
   	break;

   case 'r':
	re();
   	break;

   case 'f':
   	file();
   	break;

   case 'd':
   	display();
   	break;

   case 't':
   //top();
	break;

}
   

   //stage=stage+checkclear();// 보물을 모두 정리했는지 확인, 만약 다 정리했다면 return 1
   Save();//현재 상태 저장
   checkclear();
   if(stage==6)
{
   printf("GAME CLEAR!");
   exit(1);

}


} 
}



void display()
{
        system("clear");
        
		printf("h(왼쪽), j(아래), k(위), l(오른쪽)\n");
        printf("u(undo)\n");
        printf("r(replay)\n");
        printf("n(new)\n");
        printf("e(exit)\n");
        printf("s(save)\n");
        printf("f(file load)\n");
        printf("d(display help)\n");
        printf("t(top)\n");
		
		char a=' ';
		while(a !='\n')
		{
			a= getch();
			
		}

	return ;
        
}

void loadMap(){   
   FILE *fp = fopen("map1.txt","r");
    int i = 0, j=0,n_time=0,stop=0,STAGE=0, k=0;
    char temp;

	for (k = 0; k < 5; k++)
	{
		max_x[k] = 0 ;
		max_y[k] = 0 ;
	}
    while(stop==0) {
        fscanf(fp,"%c",&temp);
        if(temp=='e'){
            max_x[STAGE]=max_x[STAGE]/max_y[STAGE];
        	stop++;
        }
        else if(temp=='1'){
                 
      	}
        else if(temp>='2' && temp<='5') {
           j=0;
           i=0;
           n_time=0;
           max_x[STAGE]=max_x[STAGE]/max_y[STAGE];
            STAGE++;
        }
        else {
			if(temp=='@'){
          		player_x[STAGE]=i;
          		player_y[STAGE]=j;
         	}
         
			 
         	if(temp=='\n'){
            	if(n_time==0){
               
            	}
            	else if(n_time>=1){
               		map[STAGE][i][j]='\n';
               		Original_map[STAGE][i][j] = '\n';
               		j++;
               		i=0;
               		max_y[STAGE]++;
            	}
            	
            	n_time++;
            }
            
            map[STAGE][i][j] = temp;
            Original_map[STAGE][i][j] = (temp == '@' || temp == '$') ? '.' : temp;
            max_x[STAGE]++;
            i++;
        }
    }
	
	fclose(fp);
	return;
}

void prtMap()
{
	system("clear");
	printf("Hello %s\n",USER);
    int i=0, j=0;
       for(i=0;i<max_y[stage];i++){
          for(j=0;j<max_x[stage];j++){
          if(map[stage][j][i]=='.'){
             printf(" ");
         }
         else{
          printf("%c",map[stage][j][i]);
          }
      }
   }

}

int map_Check()
{

	   int box_cnt[5],depository[5];
   
   for(int i=0;i<5;i++)
   {
      
      for(int j=0;j<30;j++)
      {   

      for(int k=0;k<30;k++)
      {   
   
         if(map[i][j][k]=='$')
            box_cnt[i]++;
         if(map[i][j][k]=='O')
            depository[i]++;

      }      
      }   
   
   }
   for(int k=0;k<5;k++)
   {
      if(box_cnt[k]!=depository[k])
         return 1;
         
   }



   return 0;   


}

void checkclear()
{
   int flag=1;
   for(int i=0;i<max_y[stage];i++){
   	for(int j=0;j<max_x[stage];j++){
   		if( Original_map[stage][j][i]=='O'){
   			if(map[stage][j][i] !='$')
   			 flag = 0;
		   }
	   }
   }
   if(flag){
   	stage++;
   }
   	
}
       



   
void re(){
	system("clear");
	loadMap();
	prtMap();
}

void New()
{
   system("clear");
   stage=0;
   move=0;
   loadMap();
   prtMap();
   
}
//map[stage][i][j] x축 i, y축 j

void left()
{
   int dx = 1;
   // 벽인 경우 
   if(map[stage][player_x[stage] - dx][player_y[stage]] == '#') {
		move++;
   }
   else {
   		// 보물을 미는 경 우 
		if(map[stage][player_x[stage] - dx][player_y[stage]] == '$') {
			if(map[stage][player_x[stage] - dx - dx][player_y[stage]] == '#' || map[stage][player_x[stage] - dx - dx][player_y[stage]] == '$') {
				move++;
			}
			else {
				// 공간에 보물을 넣는 경우 
				if(map[stage][player_x[stage] - dx - dx][player_y[stage]] == 'O') {
					move++;
					map[stage][player_x[stage]][player_y[stage]] = Original_map[stage][player_x[stage]][player_y[stage]];
					map[stage][player_x[stage] - dx][player_y[stage]] = '@';
					map[stage][player_x[stage] - dx - dx][player_y[stage]] ='$';
					player_x[stage]--;
				}
				else {
					move++;
					map[stage][player_x[stage]][player_y[stage]] = Original_map[stage][player_x[stage]][player_y[stage]];
					map[stage][player_x[stage] - dx][player_y[stage]] = '@';
					map[stage][player_x[stage] - dx - dx][player_y[stage]] = '$';
					player_x[stage]--;
				}
			}
		}
		else {
			move++;
			map[stage][player_x[stage]][player_y[stage]] = Original_map[stage][player_x[stage]][player_y[stage]];
			map[stage][player_x[stage] - dx][player_y[stage]] = '@';
			player_x[stage]--;
		}
   }
}

void up()
{
   int dy = 1;
   // 벽인 경우 
   if(map[stage][player_x[stage]][player_y[stage] - dy] == '#') {
		move++;
   }
   else {
   		// 보물을 미는 경 우 
		if(map[stage][player_x[stage]][player_y[stage] - dy] == '$') {
			if(map[stage][player_x[stage]][player_y[stage] - dy - dy] == '#' || map[stage][player_x[stage]][player_y[stage] - dy - dy] == '$') {
				move++;
			}
			else {
				// 공간에 보물을 넣는 경우 
				if(map[stage][player_x[stage]][player_y[stage] - dy - dy] == 'O') {
					move++;
					map[stage][player_x[stage]][player_y[stage]] = Original_map[stage][player_x[stage]][player_y[stage]];
					map[stage][player_x[stage]][player_y[stage] - dy] = '@';
					map[stage][player_x[stage]][player_y[stage] - dy - dy] = '$';
					player_y[stage]--;
				}
				else {
					move++;
					map[stage][player_x[stage]][player_y[stage]] = Original_map[stage][player_x[stage]][player_y[stage]];
					map[stage][player_x[stage]][player_y[stage] - dy] = '@';
					map[stage][player_x[stage]][player_y[stage] - dy - dy] = '$';
					player_y[stage]--;
				}
			}
		}
		else {
			move++;
			map[stage][player_x[stage]][player_y[stage]] = Original_map[stage][player_x[stage]][player_y[stage]];
			map[stage][player_x[stage]][player_y[stage] - dy] = '@';
			player_y[stage]--;
		}
   }
}

void down()
{
   int dy = 1;
   // 벽인 경우 
   if(map[stage][player_x[stage]][player_y[stage] + dy] == '#') {
		move++;
   }
   else {
   		// 보물을 미는 경 우 
		if(map[stage][player_x[stage]][player_y[stage] + dy] == '$') {
			if(map[stage][player_x[stage]][player_y[stage] + dy + dy] == '#' || map[stage][player_x[stage]][player_y[stage] + dy + dy] == '$') {
				move++;
			}
			else {
				// 공간에 보물을 넣는 경우 
				if(map[stage][player_x[stage]][player_y[stage] + dy + dy] == 'O') {
					move++;
					map[stage][player_x[stage]][player_y[stage]] = Original_map[stage][player_x[stage]][player_y[stage]];
					map[stage][player_x[stage]][player_y[stage] + dy] = '@';
					map[stage][player_x[stage]][player_y[stage] + dy + dy] = '$';
					player_y[stage]++;
				}
				else {
					move++;
					map[stage][player_x[stage]][player_y[stage]] = Original_map[stage][player_x[stage]][player_y[stage]];
					map[stage][player_x[stage]][player_y[stage] + dy] = '@';
					map[stage][player_x[stage]][player_y[stage] + dy + dy] = '$';
					player_y[stage]++;
				}
			}
		}
		else {
			move++;
			map[stage][player_x[stage]][player_y[stage]] = Original_map[stage][player_x[stage]][player_y[stage]];
			map[stage][player_x[stage]][player_y[stage] + dy] = '@';
			player_y[stage]++;
		}
   }
}

void right()
{
   int dx = 1;
   // 벽인 경우 
   if(map[stage][player_x[stage] + dx][player_y[stage]] == '#') {
		move++;
   }
   else {
   		// 보물을 미는 경 우 
		if(map[stage][player_x[stage] + dx][player_y[stage]] == '$') {
			if(map[stage][player_x[stage] + dx + dx][player_y[stage]] == '#' || map[stage][player_x[stage] + dx + dx][player_y[stage]] == '$') {
				move++;
			}
			else {
				// 공간에 보물을 넣는 경우 
				if(map[stage][player_x[stage] + dx + dx][player_y[stage]] == 'O') {
					move++;
					map[stage][player_x[stage]][player_y[stage]] = Original_map[stage][player_x[stage]][player_y[stage]];
					map[stage][player_x[stage] + dx][player_y[stage]] = '@';
					map[stage][player_x[stage] + dx + dx][player_y[stage]] ='$';
					player_x[stage]++;
				}
				else {
					move++;
					map[stage][player_x[stage]][player_y[stage]] = Original_map[stage][player_x[stage]][player_y[stage]];
					map[stage][player_x[stage] + dx][player_y[stage]] = '@';
					map[stage][player_x[stage] + dx + dx][player_y[stage]] = '$';
					player_x[stage]++;
				}
			}
		}
		else {
			move++;
			map[stage][player_x[stage]][player_y[stage]] = Original_map[stage][player_x[stage]][player_y[stage]];
			map[stage][player_x[stage] + dx][player_y[stage]] = '@';
			player_x[stage]++;
		}
   }
}

void undoupdate_first()//처음 undomap 초기화해주기
{
   
   
   for(int c=0;c<5;c++)
   {
   for(int i=0;i<max_x[stage];i++)
   {
   for(int j=0;j<max_y[stage];j++)
      {
         undomap[c][i][j]=map[stage][i][j];
      }
   }
   }

}

void undoupdate()//undomap 움직일때마다 밀어서 넣어주기
{

   for(int c=5;c>1;c--)
   {
   for(int i=0;i<max_x[stage];i++)
   {
   for(int j=0;j<max_y[stage];j++)
   {
   
      undomap[c-1][i][j]=undomap[c-2][i][j];
   }
   }
   }
   
   for(int k=0;k<max_x[stage];k++)
   {
   for(int l=0;l<max_y[stage];l++)
   {
      undomap[0][k][l]=map[stage][k][l];
   }
   }
}

void undo()
{
	if (undocount == 0)
	{
		printf("undo finished");
		return;
	}
	player_x[stage] = Copy_player_x[0];
	player_y[stage] = Copy_player_y[0];
	
	for (int p = 0; p < 4; p++)
	{
		Copy_player_x[p] = Copy_player_x[p + 1];
		Copy_player_y[p] = Copy_player_y[p + 1];

	}

	for (int i = 0; i<max_x[stage]; i++)
	{
		for (int j = 0; j<max_y[stage]; j++)
		{
			map[stage][i][j] = undomap[0][i][j];
		}
	}

	for (int c = 0; c<4; c++)
	{
		for (int i = 0; i<max_x[stage]; i++)
		{
			for (int j = 0; j<max_y[stage]; j++)
			{
				undomap[c][i][j] = undomap[c + 1][i][j];
			}
		}
	}
		

	undocount--;
}

void Save()
{
   FILE *save;

   save = fopen("sokoban.txt", "w"); //쓰기 형식
   
   fprintf(save, "%s\n", USER);
   fprintf(save, "%d\n", undocount);
   fprintf(save, "%d\n", move);
   fprintf(save, "%d\n", stage);
   
   for(int i = 0; i < max_y[stage]; i++)
   {
      for(int j = 0; j < max_x[stage]; j++)
      {
         fprintf(save, "%c", map[stage][j][i]);
      }
   }

   fclose(save);
}

void file()
{
   FILE *fileload;
   char ch;
   int x = 0, y = -1;

   fileload = fopen("sokoban.txt", "r");
   
   if (fileload == NULL)
      exit(1); //로드한 파일이 빈 파일이면 프로그램 종료
   else
   {
      // sokoban.txt에서 유저명, 언두횟수, 움직임횟수, 스테이지 받아오기
      fscanf(fileload, "%s\n%d\n%d\n%d\n", &USER, &undocount, &move, &stage);
//      fscanf(fileload, "\n", &ch);
      // 맵 받아오기
      while(fscanf(fileload, "\n%c", &ch) != EOF){
         map[stage][x][y] = ch;
         
         if (ch == '@'){
            player_x[stage]= x;
            player_y[stage]= y;
         }
         else if (ch == '\n'){
            y++;
            x=0;
         }
         else{
         	
            x++;
        }
      }
      printf("moya");
   	fclose(fileload);
	}	
	printf("%c",map);
	printf("so sad");
}



//void top(void){
//
//	putchar();
//	int w;
//	scanf("%d",&w);
//	if (w=='1')
//		t1();
//	else if (w==2)
//		t2();
//	else if (w==3)
//		t3();
//	else if (w==4)
//		t4();
//	else if (w==5)
//		t5();
//	else{
//		t1();
//		t2();
//		t3();
//		t4();
//		t5();
//	}
//}

