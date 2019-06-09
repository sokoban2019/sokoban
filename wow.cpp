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
}//getch �̿��ϱ� ���� �Լ�


int main()//���� �Լ�
{
   loadMap();
   map_Check();

   printf("Start..\nInput name :");
   scanf("%s", USER);

   undoupdate_first();
   command();

}

void command()//Ű����� ��� �Է� �޴� �Լ�
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

      case 'e': //e ������ �����ϰ� ������
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
      if (stage == 5)//�������� ������ Ŭ���� �ϸ� ���� �޽��� ����ϰ� ����
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



void display()//���� ��� �Լ�
{
   system("clear");

   printf("Hello %s\n", USER);
   printf("h(����), j(�Ʒ�), k(��), l(������)\n");
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

   }//���� ������ ����

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

      if (temp >= '1' && temp <= '5') { //�������� ����
         if (temp != '1') {
            x = y = 0;//�������� �Ѿ �� â�� ��ġ �ʱ�ȭ
            STAGE++;
         }

         fscanf(fp, "\n", &temp);
      }
      else {
         if (temp == '@') {
            player_x[STAGE] = x;//â�� ��ġ ����
            player_y[STAGE] = y;
         }

         map[STAGE][x][y] = temp;//@, ��������, e�� ������ ������
         Original_map[STAGE][x][y] = (temp == '@' || temp == '$') ? '.' : temp;//Ŀ�� �̵����� �� ���� �� �迭 ������Ű�� ����
         x++;
         if (y == 0) max_x[STAGE]++;//�� ���� �� ���ش�

         if (temp == '\n') {
            x = 0;
            y++;
            max_y[STAGE]++;//�� ���� �� ���ش�
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
            printf(" ");//map �ؽ�Ʈ ���Ͽ��� .���� �Էµ� �κ��� �������� ���
         else
            printf("%c", map[stage][x][y]);
      }
   }
}

void map_Check()//���� ������ ���� ���� ������ Ȯ���ϴ� �Լ�
{

   int box_cnt[5]={0}, depository[5]={0};

   for (int i = 0; i<5; i++)
   {

      for (int j = 0; j<30; j++)
      {

         for (int k = 0; k<30; k++)
         {

            if (map[i][j][k] == '$')
               box_cnt[i]++;//���� ���� ����
            if (map[i][j][k] == 'O')
               depository[i]++;//���� ���� ����

         }
      }

   }
   for (int k = 0; k<5; k++)
   {
      if(box_cnt[k]!=depository[k]){
         printf("error\n");
         exit(1);//���� ������ ���� ���� ���� ������ ���� �޽��� ����ϰ� ���α׷� ����
      }

   }



}

void checkclear()//�������� Ŭ���� ���� �����ϸ� ���� ���������� �Ѿ�� �Լ�
{
   int flag = 1;
   for (int i = 0; i<max_y[stage]; i++) {
      for (int j = 0; j<max_x[stage]; j++) {
         if (Original_map[stage][j][i] == 'O') {
            if (map[stage][j][i] != '$')
               flag = 0;//���� �ȿ� ������ �� ������ Ȯ��
         }
      }
   }
   if (flag) {
      saverank();//��ũ ����
      move = 0;//������ Ƚ�� �ʱ�ȭ
      undocount = 5;//��� Ƚ�� �ʱ�ȭ
      stage++;//���� ���������� �Ѿ
   }

}

void re()//���÷��� �Լ� 
{
   system("clear");
   loadMap();//ȭ�� ����� �ε��

}

void New()//���÷��� �Լ�
{
   system("clear");
   stage = 0;
   move = 0;
   undocount = 5;//��������, ������ Ƚ��, ��� Ƚ�� �ʱ�ȭ
   loadMap();//�ε��


}


void left()//�������� �̵��ϴ� �Լ�
{
   int dx = 1;

   if (map[stage][player_x[stage] - dx][player_y[stage]] == '#') {
      move++;//���� �ε����� �� ������ Ƚ���� ����
   }
   else {

      if (map[stage][player_x[stage] - dx][player_y[stage]] == '$') {
         if (map[stage][player_x[stage] - dx - dx][player_y[stage]] == '#' || map[stage][player_x[stage] - dx - dx][player_y[stage]] == '$') {
            move++;//�÷��̾� ������ �����̰�, �� ������ ���̳� �����̸� ������ Ƚ���� ����
         }
         else {

            if (map[stage][player_x[stage] - dx - dx][player_y[stage]] == 'O') {
               move++;
               map[stage][player_x[stage]][player_y[stage]] = Original_map[stage][player_x[stage]][player_y[stage]];
               map[stage][player_x[stage] - dx][player_y[stage]] = '@';
               map[stage][player_x[stage] - dx - dx][player_y[stage]] = '$';
               player_x[stage]--;
            }//�÷��̾� ������ �����̰�, �� ������ â��� ������ Ƚ�� ����, â������ �������� �̵�, �� �������� ������ �̵�
            else {
               move++;
               map[stage][player_x[stage]][player_y[stage]] = Original_map[stage][player_x[stage]][player_y[stage]];
               map[stage][player_x[stage] - dx][player_y[stage]] = '@';
               map[stage][player_x[stage] - dx - dx][player_y[stage]] = '$';
               player_x[stage]--;
            }//�÷��̾� ������ �����̰�, �� ������ �����̸� ������ Ƚ�� ����, â������ �������� �̵�, �� �������� ������ �̵�
         }
      }
      else {
         move++;
         map[stage][player_x[stage]][player_y[stage]] = Original_map[stage][player_x[stage]][player_y[stage]];
         map[stage][player_x[stage] - dx][player_y[stage]] = '@';
         player_x[stage]--;
      }//�÷��̾� ������ â�� �����̸�, â������ �������� �̵�, â������ x��ǥ 1 ����
   }
}

void up()//���� �̵��ϴ� �Լ�
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

void down()//�Ʒ��� �̵��ϴ� �Լ�
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

void right()//���������� �̵��ϴ� �Լ�
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

void undoupdate_first()//�� ó�� ��θ� �ʱ�ȭ�ϴ� �Լ�
{


   for (int c = 0; c<5; c++)//��ó���� ��� ���� ó�� ������ �� �ʱ�ȭ �����ֱ�
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

void undoupdate()//������ ������ �ٲ�� �� ���� �����ϴ� �Լ�
{

   for (int c = 5; c>1; c--)//��θ� �� ������ ���� ������ �ϳ��� �о �ֱ�
   {
      for (int i = 0; i<max_x[stage]; i++)
      {
         for (int j = 0; j<max_y[stage]; j++)
         {

            undomap[c - 1][i][j] = undomap[c - 2][i][j];
         }
      }
   }

   for (int k = 0; k<max_x[stage]; k++)//���� �� ���¸� undompa�� �ֱ�
   {
      for (int l = 0; l<max_y[stage]; l++)
      {
         undomap[0][k][l] = map[stage][k][l];
      }
   }

   for (int p = 4; p >= 1; p--)//����� ���� �÷��̾� ��ġ�� �ϳ��� �б�
   {
      Copy_player_x[p] = Copy_player_x[p - 1];
      Copy_player_y[p] = Copy_player_y[p - 1];

   }

   Copy_player_x[0] = player_x[stage];//���� ��ġ�� ����
   Copy_player_y[0] = player_y[stage];

}

void undo()//��� �Լ� 
{
   if (undocount == 0)//���� ��� Ƚ�� �� ����ߴٸ� ��� �������� �ʵ��� �˻�
   {
      printf("undo finished");
      return;
   }
   player_x[stage] = Copy_player_x[0];//undoupdate���� �����س��� ���� �÷��̾� ��ġ�� ���� �÷��̾� ��ġ�� �ٲ�
   player_y[stage] = Copy_player_y[0];

   for (int p = 0; p < 4; p++)//���� �÷��̾� ��ġ�� �ϳ��� �������
   {
      Copy_player_x[p] = Copy_player_x[p + 1];
      Copy_player_y[p] = Copy_player_y[p + 1];

   }

   for (int i = 0; i<max_x[stage]; i++)
   {
      for (int j = 0; j<max_y[stage]; j++)
      {
         map[stage][i][j] = undomap[0][i][j];//���� �� ���¸� ���� ���·� �ٲٱ�
      }
   }

   for (int c = 0; c<4; c++)
   {
      for (int i = 0; i<max_x[stage]; i++)
      {
         for (int j = 0; j<max_y[stage]; j++)
         {
            undomap[c][i][j] = undomap[c + 1][i][j];//���� ���¸� �ϳ��� �������
         }
      }
   }


   undocount--;//��� Ƚ�� �ϳ� ���̱�
}

void Save()//���� �����ϴ� �Լ�
{
   FILE *save;

   save = fopen("sokoban.txt", "w");//sokoban �ؽ�Ʈ ������ ���� ���� ����

   fprintf(save, "%s\n", USER);
   fprintf(save, "%d\n", undocount);
   fprintf(save, "%d\n", move);
   fprintf(save, "%d\n", stage);//�̸�, ��� Ƚ��, ������ Ƚ��, �������� ����

   for (int i = 0; i < max_y[stage]; i++)
   {
      for (int j = 0; j < max_x[stage]; j++)
      {
         fprintf(save, "%c", map[stage][j][i]);//�� ���� ����
      }
   }

   fclose(save);//���� �ݱ�
}

void file()//������ ���� �ҷ����� �Լ�
{
   FILE *fileload;
   char ch;
   int x = 0, y = 0;

   fileload = fopen("sokoban.txt", "r");//sokoban �ؽ�Ʈ ������ �б� ���� ����

   if (fileload == NULL) exit(1);//�� �����̸� ������


   fscanf(fileload, "%s\n%d\n%d\n%d\n", &USER, &undocount, &move, &stage);//�̸�, ��� Ƚ��, ������ Ƚ��, �������� �ҷ�����

   while (fscanf(fileload, "%c", &ch) != EOF) {
      map[stage][x][y] = ch;
      printf("%c", ch);//������ �� ����ϱ�

      if (ch == '@') {
         player_x[stage] = x;
         player_y[stage] = y;//@������ â������ ��ġ ����
      }

      if (ch == '\n') {
         y++;
         x = 0;//'\n'�� ������ �� �� ���
      }
      else {
         x++;//�ƴϸ� ������ ��ĭ �Ѿ��
      }
   }

   fclose(fileload);//���� �ݱ�
   prtMap();//�� �迭 ���
}

void saverank()
{
   FILE *rank;



         rank = fopen("ranking.txt", "a");

         fprintf(rank, "%d %s %d\n", stage, USER, move);//ranking �ؽ�Ʈ ���Ͽ� ��������, �̸�, ������ Ƚ�� ����
      

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
