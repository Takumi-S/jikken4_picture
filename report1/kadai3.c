#include <stdio.h>
#include <math.h>

#define PICTX 320
#define PICTY 240
#define WSYM 33
#define PI 3.141592


void setgray(int i, int j, unsigned char picture[PICTX][PICTY][3]);
void invmatrix(double a[3][3]);


int main(int argc, char *argv[]){

unsigned char picture[PICTX][PICTY][3]; //画像全体
unsigned char qrcode[WSYM][WSYM][3]; //qrコード
int readqr[WSYM][WSYM][3]; //qrコード読み込み用
double sx = 6; //x方向倍率
double sy = 6; //y方向倍率
double tx = 20; //x方向平行移動距離
double ty = 130; //y方向平行移動距離
double theta = 60; ////反時計回り回転角度
double rad = -theta * PI / 180; //反時計回り回転ラジアン
double qrx; //qrコード上のx座標
double qry; //qrコード上のy座標

double magnify[3][3] = {{sx,0,0}, {0,sy,0}, {0,0,1}}; //拡大
double trans[3][3] = {{1,0,tx}, {0,1,ty}, {0,0,1}}; //並進
double rot[3][3] = {{cos(rad),-sin(rad),0}, {sin(rad),cos(rad),0}, {0,0,1}}; //回転
double affine[3][3]; //幾何変換のための行列
double inter[3][3]; //途中計算用
char s[128];
FILE *rfp, *wfp;


rfp = fopen("kadai2.ppm", "r"); //ファイルオープン
wfp = fopen("kadai3.ppm", "w");

if(rfp == NULL || wfp == NULL){
  printf("ファイルが開けません\n");
  return -1;
}



for(int i = 0; i < 3; i++){ //ヘッダー読み込み、書き込み
  fgets(s, 128, rfp);
  if(i != 1){ //画像サイズ以外のヘッダーはコピー
    fprintf(wfp, "%s", s);
  }else{ //画像サイズ
    fprintf(wfp, "%d %d\n", PICTX, PICTY);
  }
}

for(int j = 0; j < WSYM; j++){ //qrコード読み込み
  for(int i = 0; i < WSYM; i++){
    fscanf(rfp, "%d %d %d", &readqr[i][j][0], &readqr[i][j][1], &readqr[i][j][2]);
  }
}

fclose(rfp);


for(int j = 0; j < WSYM; j++){
  for(int i = 0; i < WSYM; i++){
    for(int a = 0; a < 3; a++){
      qrcode[i][j][a] = readqr[i][j][a];
    }
  }
}


/*--------------------拡大、並進、回転処理--------------------*/
invmatrix(magnify); //それぞれの逆行列を作成
invmatrix(trans);
invmatrix(rot);


for(int i = 0; i < 3; i++){ //拡大、並進、回転行列を掛け合わせる
  for(int j = 0; j < 3; j++){
    for(int k = 0; k < 3; k++){
      inter[i][j] += magnify[i][k] * rot[k][j];
    }
  }
}

for(int i = 0; i < 3; i++){
  for(int j = 0; j < 3; j++){
    for(int k = 0; k < 3; k++){
      affine[i][j] += inter[i][k] * trans[k][j];
    }
  }
}


for(int pictx = 0; pictx < PICTX; pictx++){
  for(int picty = 0; picty < PICTY; picty++){
    qrx = affine[0][0] * pictx + affine[0][1] * picty + affine[0][2] * 1; //画像座標をqrコード上の座標へ変換する
    qry = affine[1][0] * pictx + affine[1][1] * picty + affine[1][2] * 1;


    if(0 <= qrx && qrx <= (WSYM-1) && 0 <= qry && qry <= (WSYM-1)){ //qrコード上に座標があった場合
      for(int a = 0; a < 3; a++){ //qrコードのRGBをコピー
        picture[pictx][picty][a] = qrcode[(int)qrx][(int)qry][a];
      }

    }else{
      setgray(pictx, picty, picture); //それ以外はグレーをセット
    }


    qrx = 0;
    qry = 0;
  }
}
/*--------------------拡大、並進、回転処理終わり--------------------*/



for(int j = 0; j < PICTY; j++){
  for(int i = 0; i < PICTX; i++){
    fprintf(wfp, "%d %d %d\n", picture[i][j][0], picture[i][j][1], picture[i][j][2]);
  }
}


fclose(wfp);
return 1;
}



void setgray(int i, int j, unsigned char picture[PICTX][PICTY][3]){

  for(int a = 0; a < 3; a++){
    picture[i][j][a] = 50;
  }

}



void invmatrix(double a[3][3]){ //逆行列を求める関数
  double b[3][3];
  double buf;

  for(int i = 0; i < 3; i++){ //aをbにコピー
    for(int j = 0; j < 3; j++){
      b[i][j] = a[i][j];
    }
  }


  for(int i = 0; i < 3; i++){ //単位行列を作成
    for(int j = 0; j < 3; j++){
      a[i][j] = (i == j)? 1.0 : 0.0;
    }
  }


  for(int i = 0; i < 3; i++){ //掃き出し法
    buf = 1 / b[i][i];

    for(int j = 0; j < 3; j++){
      b[i][j] *= buf;
      a[i][j] *= buf;
    }

    for(int j = 0; j < 3; j++){
      if(i != j){
        buf = b[j][i];
        for(int k = 0; k < 3; k++){
          b[j][k] -= b[i][k] * buf;
          a[j][k] -= a[i][k] * buf;
        }
      }
    }
  }
}
