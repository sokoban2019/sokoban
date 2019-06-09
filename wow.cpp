#include<stdio.h>
#include<termio.h>
#include<stdlib.h>


void loadMap();
void prtMap();
void command();
void map_Check();
void checkclear();
void Save();
void up();
void down();
void right();
void left();
void New();
void undo();
void file();
void display();
void undoupdate();
void undoupdate_first();
void re();
void saverank();
void top();
void TOP();

int stage = 0, move, STAGE = 0;
char map[5][30][30];
char undomap[5][30][30];
char Original_map[5][30][30];
int player_x[5], player_y[5];
int max_x[5], max_y[5];
int undocount = 5;
char USER[11];
char Copy_map[5][30][30];
int Copy_player_x[5];
int Copy_player_y[5];
int player_num=0;
int s[100];
char n[100][11];
int m[100];

int getch(void) {
   int ch;
   struct termios buf, save;
   tcgetattr(0, &save);
   buf = save;
   buf.c_lflag &= ~(ICANON | ECHO);
   buf.c_cc[VMIN] = 1;
   buf.c_cc[VTIME] = 0;
   tcsetattr(0, TCSAFLUSH, &buf);
   ch = getchar();
   tcsetattr(0, TCSAFLUSH, &save);
   return ch;
}//getch 이용하기 위한 함수


int main()//메인 함수
{
   loadMap();
   map_Check();

   printf("Start..\nInput name :");
   scanf("%s", USER);

   undoupdate_first();
   command();

}

void command()//키보드로 명령 입력 받는 함수
{
   char usr;
   


   while (1)
   {
      prtMap();

      usr = getch();

      switch (usr)
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

      case 'e': //e 누르면 저장하고 나가기
         Save();
         printf("SEE YOU...%s", USER);
         exit(1);
         break;

      case 's':
         Save();
         break;

      case 'n':
         New();
         undoupdate_first();
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
         top();
         break;

      }








      checkclear();
      if (stage == 5)//스테이지 끝까지 클리어 하면 축하 메시지 출력하고 종료
      {
         printf("\nGAME CLEAR!");
         char a = ' ';

         while (a != '\n')
         {

            a = getch();
         }

         exit(1);

      }


   }
}



void display()//도움말 출력 함수
{
   system("clear");

   printf("Hello %s\n", USER);
   printf("h(왼쪽), j(아래), k(위), l(오른쪽)\n");
   printf("u(undo)\n");
   printf("r(replay)\n");
   printf("n(new)\n");
   printf("e(exit)\n");
   printf("s(save)\n");
   printf("f(file load)\n");
   printf("d(display help)\n");
   printf("t(top)\n");

   char a = ' ';
   while (a != '\n')
   {
      a = getch();

   }//엔터 누르면 종료

   return;

}

void loadMap() {
   FILE *fp = fopen("map.txt", "r");
   int x = 0, y = 0, STAGE = 0;
   char temp;

   for (int c = 0; c < 5; c++)
   {
      max_x[c] = 0;
      max_y[c] = 0;
   }

   while (1) {
      fscanf(fp, "%c", &temp);
      if (temp == 'e') break;

      if (temp >= '1' && temp <= '5') { //스테이지 구별
         if (temp != '1') {
            x = y = 0;//스테이지 넘어갈 때 창고 위치 초기화
            STAGE++;
         }

         fscanf(fp, "\n", &temp);
      }
      else {
         if (temp == '@') {
            player_x[STAGE] = x;//창고 위치 저장
            player_y[STAGE] = y;
         }

         map[STAGE][x][y] = temp;//@, 스테이지, e를 제외한 나머지
         Original_map[STAGE][x][y] = (temp == '@' || temp == '$') ? '.' : temp;//커서 이동했을 때 원래 맵 배열 유지시키기 위해
         x++;
         if (y == 0) max_x[STAGE]++;//맵 열의 수 세준다

         if (temp == '\n') {
            x = 0;
            y++;
            max_y[STAGE]++;//맵 행의 수 세준다
         }
      }
   }

   fclose(fp);
   return;
}

void prtMap()
{
   system("clear");
   printf("Hello %s\n", USER);

   for (int y = 0; y < max_y[stage]; y++) {
      for (int x = 0; x < max_x[stage]; x++) {
         if (map[stage][x][y] == '.')
            printf(" ");//map 텍스트 파일에서 .으로 입력된 부분을 공백으로 출력
         else
            printf("%c", map[stage][x][y]);
      }
   }
}

void map_Check()//보물 개수와 상자 개수 같은지 확인하는 함수
{

   int box_cnt[5]={0}, depository[5]={0};

   for (int i = 0; i<5; i++)
   {

      for (int j = 0; j<30; j++)
      {

         for (int k = 0; k<30; k++)
         {

            if (map[i][j][k] == '$')
               box_cnt[i]++;//보물 개수 세기
            if (map[i][j][k] == 'O')
               depository[i]++;//상자 개수 세기

         }
      }

   }
   for (int k = 0; k<5; k++)
   {
      if(box_cnt[k]!=depository[k]){
         printf("error\n");
         exit(1);//보물 개수와 상자 개수 같지 않으면 에러 메시지 출력하고 프로그램 종료
      }

   }



}

void checkclear()//스테이지 클리어 조건 만족하면 다음 스테이지로 넘어가는 함수
{
   int flag = 1;
   for (int i = 0; i<max_y[stage]; i++) {
      for (int j = 0; j<max_x[stage]; j++) {
         if (Original_map[stage][j][i] == 'O') {
            if (map[stage][j][i] != '$')
               flag = 0;//상자 안에 보물이 다 들어갔는지 확인
         }
      }
   }
   if (flag) {
      saverank();//랭크 저장
      move = 0;//움직임 횟수 초기화
      undocount = 5;//언두 횟수 초기화
      stage++;//다음 스테이지로 넘어감
   }

}

void re()//리플레이 함수 
{
   system("clear");
   loadMap();//화면 지우고 로드맵

}

void New()//뉴플레이 함수
{
   system("clear");
   stage = 0;
   move = 0;
   undocount = 5;//스테이지, 움직임 횟수, 언두 횟수 초기화
   loadMap();//로드맵


}


void left()//왼쪽으로 이동하는 함수
{
   int dx = 1;

   if (map[stage][player_x[stage] - dx][player_y[stage]] == '#') {
      move++;//벽에 부딪쳤을 때 움직임 횟수만 증가
   }
   else {

      if (map[stage][player_x[stage] - dx][player_y[stage]] == '$') {
         if (map[stage][player_x[stage] - dx - dx][player_y[stage]] == '#' || map[stage][player_x[stage] - dx - dx][player_y[stage]] == '$') {
            move++;//플레이어 왼쪽이 보물이고, 그 왼쪽이 벽이나 보물이면 움직임 횟수만 증가
         }
         else {

            if (map[stage][player_x[stage] - dx - dx][player_y[stage]] == 'O') {
               move++;
               map[stage][player_x[stage]][player_y[stage]] = Original_map[stage][player_x[stage]][player_y[stage]];
               map[stage][player_x[stage] - dx][player_y[stage]] = '@';
               map[stage][player_x[stage] - dx - dx][player_y[stage]] = '$';
               player_x[stage]--;
            }//플레이어 왼쪽이 보물이고, 그 왼쪽이 창고면 움직임 횟수 증가, 창고지기 왼쪽으로 이동, 그 왼쪽으로 보물도 이동
            else {
               move++;
               map[stage][player_x[stage]][player_y[stage]] = Original_map[stage][player_x[stage]][player_y[stage]];
               map[stage][player_x[stage] - dx][player_y[stage]] = '@';
               map[stage][player_x[stage] - dx - dx][player_y[stage]] = '$';
               player_x[stage]--;
            }//플레이어 왼쪽이 보물이고, 그 왼쪽이 공백이면 움직임 횟수 증가, 창고지기 왼쪽으로 이동, 그 왼쪽으로 보물도 이동
         }
      }
      else {
         move++;
         map[stage][player_x[stage]][player_y[stage]] = Original_map[stage][player_x[stage]][player_y[stage]];
         map[stage][player_x[stage] - dx][player_y[stage]] = '@';
         player_x[stage]--;
      }//플레이어 왼쪽이 창고나 공백이면, 창고지기 왼쪽으로 이동, 창고지기 x좌표 1 감소
   }
}

void up()//위로 이동하는 함수
{
   int dy = 1;

   if (map[stage][player_x[stage]][player_y[stage] - dy] == '#') {
      move++;
   }
   else {

      if (map[stage][player_x[stage]][player_y[stage] - dy] == '$') {
         if (map[stage][player_x[stage]][player_y[stage] - dy - dy] == '#' || map[stage][player_x[stage]][player_y[stage] - dy - dy] == '$') {
            move++;
         }
         else {

            if (map[stage][player_x[stage]][player_y[stage] - dy - dy] == 'O') {
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

void down()//아래로 이동하는 함수
{
   int dy = 1;

   if (map[stage][player_x[stage]][player_y[stage] + dy] == '#') {
      move++;
   }
   else {

      if (map[stage][player_x[stage]][player_y[stage] + dy] == '$') {
         if (map[stage][player_x[stage]][player_y[stage] + dy + dy] == '#' || map[stage][player_x[stage]][player_y[stage] + dy + dy] == '$') {
            move++;
         }
         else {

            if (map[stage][player_x[stage]][player_y[stage] + dy + dy] == 'O') {
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

void right()//오른쪽으로 이동하는 함수
{
   int dx = 1;

   if (map[stage][player_x[stage] + dx][player_y[stage]] == '#') {
      move++;
   }
   else {

      if (map[stage][player_x[stage] + dx][player_y[stage]] == '$') {
         if (map[stage][player_x[stage] + dx + dx][player_y[stage]] == '#' || map[stage][player_x[stage] + dx + dx][player_y[stage]] == '$') {
            move++;
         }
         else {

            if (map[stage][player_x[stage] + dx + dx][player_y[stage]] == 'O') {
               move++;
               map[stage][player_x[stage]][player_y[stage]] = Original_map[stage][player_x[stage]][player_y[stage]];
               map[stage][player_x[stage] + dx][player_y[stage]] = '@';
               map[stage][player_x[stage] + dx + dx][player_y[stage]] = '$';
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

void undoupdate_first()//맨 처음 언두맵 초기화하는 함수
{


   for (int c = 0; c<5; c++)//맨처음에 언두 맵을 처음 맵으로 다 초기화 시켜주기
   {
      for (int i = 0; i<max_x[stage]; i++)
      {
         for (int j = 0; j<max_y[stage]; j++)
         {
            undomap[c][i][j] = map[stage][i][j];
         }
      }
   }

}

void undoupdate()//움직일 때마다 바뀌는 맵 상태 저장하는 함수
{

   for (int c = 5; c>1; c--)//언두맵 맨 마지막 값은 버리고 하나씩 밀어서 넣기
   {
      for (int i = 0; i<max_x[stage]; i++)
      {
         for (int j = 0; j<max_y[stage]; j++)
         {

            undomap[c - 1][i][j] = undomap[c - 2][i][j];
         }
      }
   }

   for (int k = 0; k<max_x[stage]; k++)//현재 맵 상태를 undompa에 넣기
   {
      for (int l = 0; l<max_y[stage]; l++)
      {
         undomap[0][k][l] = map[stage][k][l];
      }
   }

   for (int p = 4; p >= 1; p--)//저장된 그전 플레이어 위치도 하나씩 밀기
   {
      Copy_player_x[p] = Copy_player_x[p - 1];
      Copy_player_y[p] = Copy_player_y[p - 1];

   }

   Copy_player_x[0] = player_x[stage];//현재 위치를 저장
   Copy_player_y[0] = player_y[stage];

}

void undo()//언두 함수 
{
   if (undocount == 0)//만약 언두 횟수 다 사용했다면 언두 실행하지 않도록 검사
   {
      printf("undo finished");
      return;
   }
   player_x[stage] = Copy_player_x[0];//undoupdate에서 저장해놓은 그전 플레이어 위치를 현재 플레이어 위치를 바꿈
   player_y[stage] = Copy_player_y[0];

   for (int p = 0; p < 4; p++)//그전 플레이어 위치를 하나씩 끌어오기
   {
      Copy_player_x[p] = Copy_player_x[p + 1];
      Copy_player_y[p] = Copy_player_y[p + 1];

   }

   for (int i = 0; i<max_x[stage]; i++)
   {
      for (int j = 0; j<max_y[stage]; j++)
      {
         map[stage][i][j] = undomap[0][i][j];//현재 맵 상태를 그전 상태로 바꾸기
      }
   }

   for (int c = 0; c<4; c++)
   {
      for (int i = 0; i<max_x[stage]; i++)
      {
         for (int j = 0; j<max_y[stage]; j++)
         {
            undomap[c][i][j] = undomap[c + 1][i][j];//그전 상태를 하나씩 끌어오기
         }
      }
   }


   undocount--;//언두 횟수 하나 줄이기
}

void Save()//맵을 저장하는 함수
{
   FILE *save;

   save = fopen("sokoban.txt", "w");//sokoban 텍스트 파일을 쓰기 모드로 열기

   fprintf(save, "%s\n", USER);
   fprintf(save, "%d\n", undocount);
   fprintf(save, "%d\n", move);
   fprintf(save, "%d\n", stage);//이름, 언두 횟수, 움직임 횟수, 스테이지 저장

   for (int i = 0; i < max_y[stage]; i++)
   {
      for (int j = 0; j < max_x[stage]; j++)
      {
         fprintf(save, "%c", map[stage][j][i]);//맵 상태 저장
      }
   }

   fclose(save);//파일 닫기
}

void file()//저장한 파일 불러오는 함수
{
   FILE *fileload;
   char ch;
   int x = 0, y = 0;

   fileload = fopen("sokoban.txt", "r");//sokoban 텍스트 파일을 읽기 모드로 열기

   if (fileload == NULL) exit(1);//빈 파일이면 나가기


   fscanf(fileload, "%s\n%d\n%d\n%d\n", &USER, &undocount, &move, &stage);//이름, 언두 횟수, 움직임 횟수, 스테이지 불러오기

   while (fscanf(fileload, "%c", &ch) != EOF) {
      map[stage][x][y] = ch;
      printf("%c", ch);//저장한 맵 출력하기

      if (ch == '@') {
         player_x[stage] = x;
         player_y[stage] = y;//@만나면 창고지기 위치 저장
      }

      if (ch == '\n') {
         y++;
         x = 0;//'\n'을 만나면 한 줄 띄기
      }
      else {
         x++;//아니면 옆으로 한칸 넘어가기
      }
   }

   fclose(fileload);//파일 닫기
   prtMap();//맵 배열 출력
}

void saverank()
{
   FILE *rank;



         rank = fopen("ranking.txt", "a");

         fprintf(rank, "%d %s %d\n", stage, USER, move);//ranking 텍스트 파일에 스테이지, 이름, 움직임 횟수 저장
      

   fclose(rank);

}

void top()
{
	FILE *top;
	int temp;
	
	top = fopen("ranking.txt", "r");
	
	while(1)
	{
		
		temp=fscanf(top, "%d %s %d\n", &s[player_num],&n[player_num],&m[player_num]);
	
		if (temp == EOF)
			break;
			

		player_num++;
		
	}
	TOP();
	 

	fclose(top);

	char a = ' ';
	while (a != '\n')
	{
		a = getch();

	}
	
	
	

}

void TOP(){
	char c;
	int num;

	putchar('t');
	while (c=getchar() != '\n'){
		scanf("%d",&num);
	}
	
	system("clear");
	
		if (num==1){
			printf("map 1\n");
			printf("\n");
			for (int j=0; j<player_num; j++){
					if (s[j]==0){
							printf("%s %d\n", n[j], m[j]);
					}
				}


		}
		else if (num==2){
			printf("map 2\n");
			printf("\n");
			for (int j=0; j<player_num; j++){
					if (s[j]==1){
							printf("%s %d\n", n[j], m[j]);
					}
				}

				
		}
		else if (num==3){
			printf("map 3\n");
			printf("\n");
			for (int j=0; j<player_num; j++){
					if (s[j]==2){
					printf("%s %d\n", n[j], m[j]);
					}
				}

		}
		else if (num==4){
			printf("map 4\n");
			printf("\n");
			for (int j=0; j<player_num; j++){
					if (s[j]==3){
							printf("%s %d\n", n[j], m[j]);
					}
				}

		}
		else if (num==5){
			printf("map 5\n");
			printf("\n");
			for (int j=0; j<player_num; j++){
					if (s[j]==4){
						printf("%s %d\n", n[j], m[j]);
					}
				}

		}
		else{
			printf("map 1\n");
			printf("\n");
			for (int j=0; j<player_num; j++){
					if (s[j]==0){
						printf("%s %d\n", n[j], m[j]);
					}
				}
			printf("\n");	
			printf("map 2\n");
			printf("\n");
			for (int j=0; j<player_num; j++){
					if (s[j]==1){
						printf("%s %d\n", n[j], m[j]);
					}
				}
			printf("map 3\n");
			printf("\n");
			for (int j=0; j<player_num; j++){
					if (s[j]==2){
						printf("%s %d\n", n[j], m[j]);
					}
				}
			printf("\n");	
			printf("map 4\n");
			printf("\n");		
			for (int j=0; j<player_num; j++){
					if (s[j]==3){
						printf("%s %d\n", n[j], m[j]);
					}
				}
			printf("\n");
			printf("map 5\n");		
			for (int j=0; j<player_num; j++){
					if (s[j]==4){
						printf("%s %d\n", n[j], m[j]);
					}
				}
			
		}
}
