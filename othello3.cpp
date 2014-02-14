#include <stdio.h>

#define SENTE 1
#define GOTE -1
#define NONE 0
#define HINT 2
#define MASUmax 8
#define WHITE GOTE
#define BLACK SENTE
#define TAISENSUU 100

//自由に使っていい変数。ループなど、その場で終わるのに使うのを推奨。
int i,j,k,l,m,n,o,p;
int x,y,z;
int BAN[MASUmax+2][MASUmax+2];//ここにその時の盤面が入る。
int hint[65];



//ここからメインプログラム。
int J_BAN[MASUmax+2][MASUmax+2];
int J_flag[10];
int J_HINT[65];
int J_teban;
int J_TE;
int J_x,J_y;
int J_wincount[3]={0,0,0};

int henkan(int t){                               //変換する関数
	if(t/100==3){
		J_x=J_HINT[t%100]%10;
		J_y=J_HINT[(t%100)]/10;
	}else if(t/100==2){
		J_x=((t%100)-1)%8+1;
		J_y=((t%100)-1)/8+1;
	}else if(t/100==1){
		J_x=t%10;
		J_y=(t%100)/10;
	}
}

int AI(){
	return 301;                                   //AIの一例。
}

int senshu(int shiaisuu){
	if(J_flag[4]==1){
		printf("条件にあった三桁の整数を入れてください");
		scanf("%d",&J_TE);
		henkan(J_TE);
	}else if(J_flag[4]==2){
		henkan(301);
	}else if(J_flag[4]==3){
		henkan(AI());
	}/*else if(J_flag[4]==4){
		このあたりにAIの呼び出しをする
		henkan(??);
	}*/
	
	if(J_BAN[J_x][J_y]!=HINT){
		J_turn(shiaisuu);
	}
}

int J_turn(int shiaisuu){
	for(i=0;i<=10;i++){
		for(j=0;j<=10;j++){
			BAN[i][j]=J_BAN[i][j];
		}
	}
	for(i=0;i<65;i++){                                     //ヒントと盤を与えた上で手番
		hint[i]=J_HINT[i];
	}
	if((J_teban==SENTE&&shiaisuu%2==1)||(J_teban==GOTE&&shiaisuu%2==0)){ //試合数が奇数なら1Pが先手、偶数なら1Pは後手
		J_flag[4]=J_flag[2];
	}else{
		J_flag[4]=J_flag[3];
	}
	senshu(shiaisuu);
}


int J_hyouzi(){                                //表示プログラム
	int count[3];
	count[2]=count[1]=count[0]=0;
	printf("\n");
	for(j=0;j<=MASUmax;j++){
		if(j==0){
			printf(" ");
			for(k=1;k<=MASUmax;k++){
				printf("   %d",k);
			}
			printf("\n ┏━");
			for(o=1;o<=MASUmax-1;o++){
				printf("┳━");
			}
			printf("┓\n");
		}else{
			for(i=0;i<=MASUmax;i++){
				if(i==0){
					printf("%d",j);
				}else if(J_BAN[i][j]==WHITE){
					printf("●");
					count[1]++;
				}else if(J_BAN[i][j]==BLACK){
					printf("○");
					count[2]++;
				}else if(J_BAN[i][j]==HINT){
					printf("＠");
					count[0]++;
				}else if(J_BAN[i][j]==NONE){
					printf("  ");
				}
				printf("┃");
			}
			if(j!=MASUmax){
				printf("\n ┣━");
				for(o=1;o<=MASUmax-1;o++){
					printf("╋━");
				}
				printf("┫\n");
			}
		}
	}
	printf("\n ┗━");
	for(o=1;o<=MASUmax-1;o++){
		printf("┻━");
	}
	printf("┛\n");
	printf("●%d枚 ○%d枚\n",count[1],count[2]);
}

int J_zyunbi(){                                  //初期化＆初期位置に駒を置く
	for(y=0;y<MASUmax+2;y++){
		for(x=0;x<MASUmax+2;x++){
			J_BAN[x][y]=NONE;
		}
	}
	J_BAN[4][5]=J_BAN[5][4]=SENTE;
	J_BAN[4][4]=J_BAN[5][5]=GOTE;
	J_teban=SENTE;
}

int J_passJud(int shiaisuu){                      //パス判定をする
	for(i=0;i<=64;i++){
		J_HINT[i]=0;
	}
	o=0;
	for(y=0;y<MASUmax+2;y++){
		for(x=0;x<MASUmax+2;x++){
			if(J_BAN[x][y]==HINT){                 //ヒントを消す
				J_BAN[x][y]=NONE;
			}
			J_KANAME(1);                            //その上で、その位置が置けるかどうかを判定する
		}
	}
	if(J_HINT[1]==0){                               //ヒントがひとつもなければ、それは置けるところがないということ
		J_flag[0]++;
		J_teban*=-1;
		if(J_flag[0]==2){
			J_endflag(shiaisuu);                     //二回同じ現象が続けば、両者打つところがないので、終了判定。
		}else{
			J_passJud(shiaisuu);                     //一回目なら、手番が相手に移り、パス判定。
		}
	}else{
		J_flag[0]=0;
	}

}

int J_hikkurikaeshi(){
	x=J_x;
	y=J_y;
	J_KANAME(2);                                     //KANAMEにxとyを使ったので、ここで変換
}


int J_KANAME(int PorH){
	if((J_BAN[x][y]==NONE||J_BAN[x][y]==HINT)&&x>=1&&x<=8&&y>=1&&y<=8){//駒がない&盤上でのみ判定。
		for(i=-1;i<=1;i++){
			for(j=-1;j<=1;j++){                                        //そのマスの周り八マスを判定
				if(!(i==0&&j==0)){
					p=1;
					while(J_BAN[x+p*i][y+p*j]==-J_teban){
						p++;                                           //そのマスから相手の駒が続く限りその方向に進む
					}
					if(J_teban==J_BAN[x+p*i][y+p*j]&&p!=1){            //相手の駒ではなくなったとき、自分の駒なら、
						if(PorH==1){
							J_BAN[x][y]=HINT;                          //その場所がおけるという判定をし、
							J_HINT[++o]=(x)+(y)*10;                    //その場所を記憶。
						}else if(PorH==2){
							for(m=0;m<=p;m++){
								J_BAN[x+m*i][y+m*j]=J_teban;           //置いたマスから自分のマスまでの敵の駒をひっくり返す。
							}
						}
					}
				}
			}
		}
	}
}

int J_shiaimae(){
	printf("盤を使うなら1、使わないなら0を入力：");
	scanf("%d",&J_flag[1]);
	printf("1P番");
	scanf("%d",&J_flag[2]);
	printf("2P番");
	scanf("%d",&J_flag[3]);
}

int J_endflag(int shiaisuu){
	int count[2]={0,0};
	for(i=1;i<=MASUmax;i++){                                            //両者のコマの数を数えて終了するかどうか判定する
		for(j=1;j<=MASUmax;j++){
			if(J_BAN[i][j]==SENTE){
				count[0]++;
			}else if(J_BAN[i][j]==GOTE){
				count[1]++;
			}
		}
	}
	if(count[0]==0||count[1]==0||count[0]+count[1]==MASUmax*MASUmax){   //片方のコマ数が0か、両方のコマ数の合計がマスの数、つまり全部埋まっていれば終了
		J_flag[0]=2;                                                    //終了フラグ
		if(count[0]!=count[1]){
			if(shiaisuu%2==1){                                          //試合数が奇数なら、黒が1P
				count[0]>count[1]?J_wincount[0]++:J_wincount[1]++;      //コマ数が多い方が勝ち
				J_wincount[2]+=(count[0]-count[1]);                     //コマ数の差を取り、最終のコマ数が多い方が勝ちの計算を使う
			}else{                                                      //偶数なら白が1P
				count[0]<count[1]?J_wincount[0]++:J_wincount[1]++;
				J_wincount[2]+=(count[1]-count[0]);
			}
		}
	}
}

int J_kekkahyouzi(){
	printf("1P:%d勝 2P:%d勝\n",J_wincount[0],J_wincount[1]);
	if(J_wincount[0]!=J_wincount[1]){
		J_wincount[0]>J_wincount[1]?printf("1Pの勝利"):printf("2Pの勝利\n"); //勝ち数が多いほうが勝ち
	}else if(J_wincount[2]!=0){
		J_wincount[2]>0?printf("1Pの勝利"):printf("2Pの勝利\n");             //勝ち数が同じなら、取った駒の合計が多いほうが勝ち
	}else{
		printf("引き分け");                                                  //駒の数の合計が同じなら、引き分け
	}
}

int main(){
	int shiaisuu=0;
	int star[3]={0,0,0};
	J_shiaimae();
	while(shiaisuu++<TAISENSUU){                                              //TAISENSUUの数だけ繰り返す
		J_flag[0]=0;
		J_zyunbi();
		J_passJud(shiaisuu);
		do{
			if(J_flag[1]==1){
				J_hyouzi();
			}
			J_turn(shiaisuu);
			J_hikkurikaeshi();
			J_teban*=-1;
			J_endflag(shiaisuu);
			if(J_flag[0]!=2){
				J_passJud(shiaisuu);
			}
		}while(J_flag[0]!=2);
		if(J_flag[1]==1){
			J_hyouzi();
		}
	}
	J_kekkahyouzi();
	return 0;
}