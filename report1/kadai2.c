#include <stdio.h>

#define WSYM 33


void setwhite(int i, int j, unsigned char qrcode[WSYM][WSYM][3]);
void setblack(int i, int j, unsigned char qrcode[WSYM][WSYM][3]);



int main(int argc, char *argv[])
{

unsigned char qrcode[WSYM][WSYM][3]; //QRコード
char ch[] = "1029260135 Shimura Takumi";
char *p;
FILE *fp;


fp = fopen("kadai2.ppm", "w"); //ファイルオープン

if(fp == NULL){
  printf("ファイルが開けません\n");
  return -1;
}


fprintf(fp, "P3\n"); //アスキー形式
fprintf(fp, "%d %d\n", WSYM, WSYM); //画像の幅、高さ
fprintf(fp, "255\n"); //RGB最大値


/*--------------------クワイエットゾーンセット--------------------*/
for(int i = 0; i < 4; i++){ //x < 4
  for(int j = 0; j < WSYM; j++){
    setwhite(i, j, qrcode);
  }
}

for(int i = WSYM-4; i < WSYM; i++){ //x >= WSYM-4
  for(int j = 0; j < WSYM; j++){
    setwhite(i, j, qrcode);
  }
}

for(int j = 0; j < 4; j++){ //y < 4
  for(int i = 0; i < WSYM; i++){
    setwhite(i, j, qrcode);
  }
}

for(int j = WSYM-4; j < WSYM; j++){ //y >= WSYM-4
  for(int i = 0; i < WSYM; i++){
    setwhite(i, j, qrcode);
  }
}
/*--------------------クワイエットゾーンセット終わり--------------------*/


/*--------------------位置検出パターンセット--------------------*/
for(int i = 3; i < 12; i++){ //左上明9*9
  for(int j = 3; j < 12; j++){
    setwhite(i, j, qrcode);
  }
}

for(int i = 4; i < 11; i++){ //左上暗7*7
  for(int j = 4; j < 11; j++){
    setblack(i, j, qrcode);
  }
}

for(int i = 5; i < 10; i++){ //左上明5*5
  for(int j = 5; j < 10; j++){
    setwhite(i, j, qrcode);
  }
}

for(int i = 6; i < 9; i++){ //左上暗3*3
  for(int j = 6; j < 9; j++){
    setblack(i, j, qrcode);
  }
}


for(int i = WSYM-12; i < WSYM-3; i++){ //右上明9*9
  for(int j = 3; j < 12; j++){
    setwhite(i, j, qrcode);
  }
}

for(int i = WSYM-11; i < WSYM-4; i++){ //右上暗7*7
  for(int j = 4; j < 11; j++){
    setblack(i, j, qrcode);
  }
}

for(int i = WSYM-10; i < WSYM-5; i++){ //右上明5*5
  for(int j = 5; j < 10; j++){
    setwhite(i, j, qrcode);
  }
}

for(int i = WSYM-9; i < WSYM-6; i++){ //右上暗3*3
  for(int j = 6; j < 9; j++){
    setblack(i, j, qrcode);
  }
}


for(int i = 3; i < 12; i++){ //左下明9*9
  for(int j = WSYM-12; j < WSYM-3; j++){
    setwhite(i, j, qrcode);
  }
}

for(int i = 4; i < 11; i++){ //左下暗7*7
  for(int j = WSYM-11; j < WSYM-4; j++){
    setblack(i, j, qrcode);
  }
}

for(int i = 5; i < 10; i++){ //左下明5*5
  for(int j = WSYM-10; j < WSYM-5; j++){
    setwhite(i, j, qrcode);
  }
}

for(int i = 6; i < 9; i++){ //左下暗3*3
  for(int j = WSYM-9; j < WSYM-6; j++){
    setblack(i, j, qrcode);
  }
}
/*--------------------位置検出パターンセット終わり--------------------*/


/*--------------------符号化領域への埋め込み--------------------*/
p = ch; //文字列のポインタ
int chari = 4;
int charj = 4; //符号化領域への埋め込みに使う配列番号
int flag = 0; //埋め込みを行ったか判定
int count = 1;

while(*p != '\0'){ //符号化領域への埋め込み

  for(int num = 1; num < 256; num = num << 1){ //画像配列へ書き込み
    flag = 0;

    while(flag != 1){ //埋め込みを行うまでループ
      if(chari >= 12 && chari <= 20 && charj >= 4 && charj <= 11){
        if((*p&num)/num == 1){
          setwhite(chari, charj, qrcode);
          flag = 1;
        }else{
          setblack(chari, charj, qrcode);
          flag = 1;
        }

      }else if(chari >= 4 && chari <= WSYM-5 && charj >= 12 && charj <= WSYM-13){
        if((*p&num)/num == 1){
          setwhite(chari, charj, qrcode);
          flag = 1;
        }else{
          setblack(chari, charj, qrcode);
          flag = 1;
        }

      }else if(chari >= 12 && chari <= WSYM-5 && charj >= WSYM-12 && charj <= WSYM-5){
        if((*p&num)/num == 1){
          setwhite(chari, charj, qrcode);
          flag = 1;
        }else{
          setblack(chari, charj, qrcode);
          flag = 1;
        }

      }

      if(chari == WSYM-5){
        chari = 4;
        charj++;
      }else{
        chari++;
      }

    }


  }

  p++;
}
/*--------------------符号化領域への埋め込み終わり--------------------*/





for(int j = 0; j < WSYM; j++){
  for(int i = 0; i < WSYM; i++){
    fprintf(fp, "%d %d %d\n", qrcode[i][j][0], qrcode[i][j][1], qrcode[i][j][2]);
  }
}



fclose(fp);
return 1;
}



void setwhite(int i, int j, unsigned char qrcode[WSYM][WSYM][3]){

  for(int a = 0; a < 3; a++){
    qrcode[i][j][a] = 255;
  }

}



void setblack(int i, int j, unsigned char qrcode[WSYM][WSYM][3]){

  for(int a = 0; a < 3; a++){
    qrcode[i][j][a] = 0;
  }

}
