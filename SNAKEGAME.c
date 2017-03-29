#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

/** Bu fonksiyon C compiler icin gotoxy'i tanimlar
	@param x,y   	koordinat
	Kaynakca	  	https://instilledbee.wordpress.com/2011/01/30/using-gotoxy-in-dev-c/comment-page-1/
*/
void gotoxy(short x, short y){ 
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE) ; 
    COORD position = { x, y } ; 
    SetConsoleCursorPosition( hStdout, position ) ;
}

/** Bu fonksiyon matrisi ve score'u ekrana yazdirir.
	@param board	ekrana yazdirilan matris
	@param size		ekrana yazdirilacak matrisin boyutu
	@param score	puan
*/
void yazdir(char board[][50],int size,int score){
	system("cls");
	int i,j;
	for(i=0;i<size;i++){
		for(j=0;j<size;j++){
			printf("%c ",board[i][j]);
		}
		printf("\n");
	}
	gotoxy(15,size+3);
	printf("SCORE=%d\n",score);	
}

/** Bu fonksiyon board matrisine yem icin random yer atar
	@param board	ekrana yazdirilan matris
	@param size		ekrana yazdirilacak matrisin boyutu
*/
void food(char board[][50],int size){
	int x,y;
	do{
		x=rand()%size;
		y=rand()%size;
	}while(board[x][y]=='o' || board[x][y]=='0');	
	board[x][y]='*';
}

/** Bu fonksiyon yilanin baslangic konumunu ve yonunu belirler,yilanin baslangic konumunun koordinatlarini snake matrisine atar
	@param board 			ekrana yazdirilan matris
	@param snake			yilanin yer koordinatlarinin saklandigi matris
	@param size				ekrana yazdirilacak matrisin boyutu
	@return direction		yilanin yonu
*/
int starting(char board[][50],int snake[50][2],int size){
	int x,y,a,b,hata,direction;
	//yilanin ilk parcasinin yemin uzerine gelmeme kontrolu yapilir, gelirse tekrar random degerler atanir
	do{
		x=rand()%size;
		y=rand()%size;
	}while(board[x][y]=='*');
	//random belirlenen yerler snake matrisine kaydedilir ve board matrisine sembolu kaydedilir
	board[x][y]='o';
	snake[0][0]=x;
	snake[0][1]=y;
	
	//yilanin 2. parcasinin yemin ustune gelmeme kontrolu yapilir, gelirse tekrar random degerler atanir 
	do{
	direction=1+rand()%4;
	hata=0;	
	switch(direction){
		case 1: {
			a=x;
			b=((y-1)+size)%size;
			if(board[a][b]!='*'){
				board[a][b]='o';
			}
			else
				hata=1;
			break;
		}
		case 2: {
			a=(x+1)%size;
			b=y;
			if(board[a][b]!='*'){
				board[a][b]='o';
			}
			else 
				hata=1;
			break;
		}
		case 3: {
			a=x;
			b=(y+1)%size;
			if(board[a][b]!='*'){
				board[a][b]='o';
			}
			else
				hata=1;
			break;
		}
		case 4: {
			a=((x-1)+size)%15;
			b=y;
			if(board[a][b]!='*'){
				board[a][b]='o';
			}
			else
				hata=1;
			break;
		}
	}
	}while(hata==1);
	//random belirlenen yerler snake matrisine kaydedilir
	snake[1][0]=a;
	snake[1][1]=b;
	return direction;
}

/** Bu fonksiyon yilanin belirlenen yonde kaymasini, yemle karsilasirsa uzamasini, kendi parcasiyla karsilasirsa gameover olmasini saglar
	Yem bulup bulmadigini cikti verir
	@param board 			ekrana yazdirilan matris
	@param snake			yilanin yer koordinatlarinin saklandigi matris
	@param po				yilanin parcalarini birbirine yonlendiren matris (linked list mantigi)
	@param *front			yilanin hangi parcada kaldigini gosteren degisken (linked list icin kullanilan degisken)
	@param direction		yilanin yonu
	@param *length			yilanin uzunlugu
	@param size				ekrana yazdirilacak matrisin boyutu
	@param *gameover		yilan kendi parcasiyla karsilasirsa 1 donusecek olan degisken
	@return foodfound		yilan yemle karsilastiginda 1'e donusen degisken 
*/
int slip(char board[50][50],int snake[50][2],int po[],int *front,int direction,int *length,int size,int *gameover){
	int x,y,foodfound=0,i,j,yedek;
	//yilanin ilk parcasinin koordinatlarini alir
	x=snake[*front][0];
	y=snake[*front][1];
	//yilan hangi yonde gidiyorsa koordinati o yonde artirir
	switch(direction){
		case 1:{
			y++;
			y=y%size;
			break;
		}
		case 2:{
			x--;
			if(x<0)
				x+=size;
			break;
		}
		case 3:{
			y--;
			if(y<0)
				y+=size;
			break;
		}
		case 4:{
			x++;
			x=x%size;
			break;
		}
	}
	
	//eger yilanin kendisiyle karsilasirsa gameover olur
	if(board[x][y]=='o'){
		*gameover=1;
	}
	else{
		yedek=*front;
		//yilanin en arkadaki parcasina ulasilir ve koordinatlari alinir
		*front=po[*front];
		i=snake[*front][0];
		j=snake[*front][1];
		
		//yilanin en arkadaki parcasi 0'a esitse yilan yemi yemistir ve uzunlugu 1 artirilir, po dizisinin parca yonlendirmeleri eklenir
		if(board[i][j]=='0'){
			board[i][j]='o';
			(*length)++;
			po[(*length)-1]=*front;
			po[yedek]=(*length)-1;
			snake[(*length)-1][0]=x;
			snake[(*length)-1][1]=y;
			*front=(*length)-1;
		}
		//yilanin en arkadaki parcasi 0'a esit degilse en arkadaki parca bosluga cevrilir
		else{
			board[i][j]=' ';
			snake[*front][0]=x;
			snake[*front][1]=y;
		}
		//yilanin bas parcasinin bir sonraki gidecegi yer yeme esitse 0' cevrilir
		if(board[x][y]=='*'){
			board[x][y]='0';
			foodfound=1;
		}
		//degilse arka parcasi bir sonraki yere tasinir
		else{
			board[x][y]='o';
		}
	}
	return foodfound;
}

/** Bu fonksiyon kullanicidan alinan tuslara gore yilanin yonunu belirler.Bu fonksiyonla yilana durma ozelligi de saglar.ters yone basilirsa yilan durur
	@param direction			yilanin yonu
	@param key					kullanicidan alinan tus
	@return buttondirection		kullanicidan alinan tuslara gore yilanin yonu
*/
int button(int direction,int key){
	int hata=0,wrongdirection,buttondirection;
	wrongdirection=(direction+2)%4;
	do{
		hata=0;
		if(kbhit())
			key=getch();
		switch(key){
			case 77: buttondirection=1; 			break;	//saga
			case 72: buttondirection=2; 			break;	//yukari
			case 75: buttondirection=3;				break;	//sola
			case 80: buttondirection=4; 			break;	//asagi
			default : buttondirection=direction; 	break;	
		}

		if(buttondirection==wrongdirection){	//kullanicinin bastigi tus ters yonse hata 1 olur ve tekrar tus giriline kadar yilan durur.
			hata=1;
		}
	}while(hata==1); 
	return buttondirection;
}

/** Main fonksiyon kullanicidan oyunun boyutunu ve tuslari alir ,YILAN oyununu organize eder
*/
int main(){
	srand(time(NULL));
	char board[50][50];
	int snake[50][2],po[50],size,i,j,direction,found,front=0,length=2,gameover=0,time=500,key,score=0;
	po[0]=1; 
	po[1]=0;
	
	printf("You are playing SNAKE GAME\nYou can stop the snake if you press cross direction\nAND if the snake catch itself,the game finishes\n\nplease enter the board's size=");
	scanf("%d",&size);
	//board matrisinin tum elemanlari bosluga esitlenir
	for(i=0;i<size;i++){
		for(j=0; j<size; j++){
			board[i][j]=' ';
		}
	}
	direction=starting(board,snake,size);	//yilanin baslangic konumunu ve yonunu belirleyen fonksiyon cagrilir
	food(board,size);						//board'a yem koyan fonksiyon cagrilir
	//gameover 1 olmadigi surece oyun devam eder
	while(gameover!=1){		
		if(kbhit())
			key=getch();				
		direction=button(direction,key);	//kullanicidan alinan tuslara gore yon belirleyen fonksiyon cagrilir
		found=slip(board,snake,po,&front,direction,&length,size,&gameover);		//yilanin kaymasini saglayan fonksiyon cagrilir
		yazdir(board,size,score);	//board matrisini ekrana yazdiran fonksiyon cagrilir
		Sleep(time);		
		
		// Eger yem bulunmussa score artirilir,sure azaltilir ve yeni yem konur 
		if(found==1){		
			score+=10;
			food(board,size);
			time-=20;
		}	
	}
	printf("         ***GAME OVER***"); 
}

