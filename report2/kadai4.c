#include <stdio.h>
#include <math.h>

#define PICTX 320
#define PICTY 240
#define WSYM 33
#define PI 3.141592



int main(int argc, char *argv[]){

unsigned char picture[PICTX][PICTY][3]; //画像全体
unsigned char qrcode[WSYM][WSYM][3]; //qrコード
int readpict[PICTX][PICTY][3]; //画像読み込み用
char s[128];
int runx[PICTX][PICTY][2]; //ランレングス符号化x方向
int runy[PICTX][PICTY][2]; //ランレングス符号化y方向
int midx[128][2]; //位置検出パターン中心座標候補
int midy[128][2]; //位置検出パターン中心座標候補
int midnumx = 0;
int midnumy = 0;
int pattern[3][2]; //位置検出パターン中心座標
int pul[2]; //位置検出パターン中心座標左上
int pur[2]; //位置検出パターン中心座標右上
int pdl[2]; //位置検出パターン中心座標左下
int search[5];
FILE *rfp, *wfp;


rfp = fopen("kadai3.ppm", "r"); //ファイルオープン
wfp = fopen("kadai4.ppm", "w");

if(rfp == NULL || wfp == NULL){
  printf("ファイルが開けません\n");
  return -1;
}



for(int i = 0; i < 3; i++){ //ヘッダー読み込み、書き込み
  fgets(s, 128, rfp);
  if(i != 1){ //画像サイズ以外のヘッダーはコピー
    fprintf(wfp, "%s", s);
  }else{ //画像サイズ
    fprintf(wfp, "%d %d\n", WSYM, WSYM);
  }
}

for(int j = 0; j < PICTY; j++){ //画像読み込み
  for(int i = 0; i < PICTX; i++){
    fscanf(rfp, "%d %d %d", &readpict[i][j][0], &readpict[i][j][1], &readpict[i][j][2]);
  }
}

fclose(rfp);


for(int j = 0; j < PICTY; j++){
  for(int i = 0; i < PICTX; i++){
    for(int a = 0; a < 3; a++){
      picture[i][j][a] = readpict[i][j][a];
    }
  }
}


/*--------------------位置検出パターン中心座標検出--------------------*/
//x方向ランレングス符号化
int rn;

for(int j = 0; j < PICTY; j++){
  for(int i = 0; i < PICTX; i++){
    if(i == 0){
      rn = 0;
      runx[rn][j][0] = readpict[i][j][0]; //色をコピー
      runx[rn][j][1] = 1;
    }


    if(i != 0 && (readpict[i-1][j][0] == readpict[i][j][0])){ //色が同じなら
      runx[rn][j][1] = runx[rn][j][1] + 1;
    }else if(i != 0){
      rn++;
      runx[rn][j][0] = readpict[i][j][0]; //次の色設定
      runx[rn][j][1] = 1;
    }
  }
}
//x方向ランレングス符号化終わり

int si = 5;
int sum = 0; //比較する5つの合計
double d = 0; //sum / 7
int end = 0; //行の終わりを判定
for(int j = 0; j < PICTY; j++){ //x方向1:1:3:1:1検出
  si = 5;
  end = 0;
  while(1){
    if(si == 5){ //行の始め
      for(int ii = 0; ii < 5; ii++){
        search[ii] = runx[ii][j][1];
      }
    }
//printf("%d %d %d %d %d\n", search[0], search[1], search[2], search[3], search[4]);

    for(int ii = 0; ii < 5; ii++){
      sum  = sum + search[ii];
    }


    if(si == 5){
      end = sum;
    }else{
      end = end + runx[si-1][j][1];
    }

    d = (double)sum / 7; //1:1:3:1:1の1に相当


    if((search[0] > d*0.8) && (search[0] < d*1.2)){ //1:1:3:1:1か調べる
      if((search[1] > d*0.8) && (search[1] < d*1.2)){
        if((search[2] > d*2.4) && (search[2] < d*3.6)){
          if((search[3] > d*0.8) && (search[3] < d*1.2)){
            if((search[4] > d*0.8) && (search[4] < d*1.2)){
              midx[midnumx][0] = end - (sum / 2);
              midx[midnumx][1] = j;
              midnumx++;
              //printf("%d %d\n", end - (sum / 2), j);
            }
          }
        }
      }
    }
    sum = 0;

    if(end == PICTX){
      //printf("\n");
      break;
    }


    for(int ii = 0; ii < 4; ii++){ //次の検出用
      search[ii] = search[ii+1];
    }
    search[4] = runx[si][j][1];
    si  = si + 1;
  }
} //x方向1:1:3:1:1検出終わり



//y方向ランレングス符号化
for(int i = 0; i < PICTX; i++){
  for(int j = 0; j < PICTY; j++){
    if(j == 0){
      rn = 0;
      runy[i][rn][0] = readpict[i][j][0]; //色をコピー
      runy[i][rn][1] = 1;
    }


    if(j != 0 && (readpict[i][j-1][0] == readpict[i][j][0])){ //色が同じなら
      runy[i][rn][1] = runy[i][rn][1] + 1;
    }else if(j != 0){
      rn++;
      runy[i][rn][0] = readpict[i][j][0]; //次の色設定
      runy[i][rn][1] = 1;
    }
  }
}
//y方向ランレングス符号化終わり


si = 5;
sum = 0; //比較する5つの合計
d = 0; //sum / 7
end = 0; //行の終わりを判定
for(int i = 0; i < PICTX; i++){ //y方向1:1:3:1:1検出
  si = 5;
  end = 0;
  while(1){
    if(si == 5){ //行の始め
      for(int ii = 0; ii < 5; ii++){
        search[ii] = runy[i][ii][1];
      }
    }
//printf("%d %d %d %d %d\n", search[0], search[1], search[2], search[3], search[4]);

    for(int ii = 0; ii < 5; ii++){
      sum  = sum + search[ii];
    }


    if(si == 5){
      end = sum;
    }else{
      end = end + runy[i][si-1][1];
    }

    d = (double)sum / 7; //1:1:3:1:1の1に相当


    if((search[0] > d*0.8) && (search[0] < d*1.2)){ //1:1:3:1:1か調べる
      if((search[1] > d*0.8) && (search[1] < d*1.2)){
        if((search[2] > d*2.4) && (search[2] < d*3.6)){
          if((search[3] > d*0.8) && (search[3] < d*1.2)){
            if((search[4] > d*0.8) && (search[4] < d*1.2)){
              midy[midnumy][0] = i;
              midy[midnumy][1] = end - (sum / 2);
              midnumy++;
              //printf("%d %d\n", i, end - (sum / 2));
            }
          }
        }
      }
    }
    sum = 0;

    if(end == PICTY){
      //printf("\n");
      break;
    }


    for(int ii = 0; ii < 4; ii++){ //次の検出用
      search[ii] = search[ii+1];
    }
    search[4] = runy[i][si][1];
    si  = si + 1;
  }
} //y方向1:1:3:1:1検出終わり


int px = 0;
for(int i = 0; i < midnumx; i++){ //位置検出パターン中心座標1つ目
  for(int j = 0; j < midnumy; j++){

    if(midx[i][0] == midy[j][0]){ //x座標が等しい
      if(((midx[i][1] - 1) <= midy[j][1]) && ((midx[i][1] + 1) >= midy[j][1])){ //y座標が+-1以内
        pattern[0][0] = midx[i][0];
        pattern[0][1] = midx[i][1];
        i = midnumx; //ループ脱出
        j = midnumy;
      }
    }

  }
}

for(int i = 0; i < midnumx; i++){ //位置検出パターン中心座標2つ目
  for(int j = 0; j < midnumy; j++){

    if(((midx[i][0] >= (pattern[0][0] - 10)) && (midx[i][0] <= (pattern[0][0] + 10))) || ((midx[i][1] >= (pattern[0][1] - 10)) && (midx[i][1] <= (pattern[0][1] + 10)))){
      //調べる座標が1つ目に検出された座標の近くであれば、検出しない
      break;
    }

    if(midx[i][0] == midy[j][0]){ //x座標が等しい
      if(((midx[i][1] - 1) <= midy[j][1]) && ((midx[i][1] + 1) >= midy[j][1])){ //y座標が+-1以内
        pattern[1][0] = midx[i][0];
        pattern[1][1] = midx[i][1];
        i = midnumx; //ループ脱出
        j = midnumy;
      }
    }

  }
}

for(int i = 0; i < midnumx; i++){ //位置検出パターン中心座標3つ目
  for(int j = 0; j < midnumy; j++){

    if(((midx[i][0] >= (pattern[0][0] - 10)) && (midx[i][0] <= (pattern[0][0] + 10))) || ((midx[i][1] >= (pattern[0][1] - 10)) && (midx[i][1] <= (pattern[0][1] + 10)))){
      //調べる座標が1つ目に検出された座標の近くであれば、検出しない
      break;
    }

    if(((midx[i][0] >= (pattern[1][0] - 10)) && (midx[i][0] <= (pattern[1][0] + 10))) || ((midx[i][1] >= (pattern[1][1] - 10)) && (midx[i][1] <= (pattern[1][1] + 10)))){
      //調べる座標が2つ目に検出された座標の近くであれば、検出しない
      break;
    }

    if(midx[i][0] == midy[j][0]){ //x座標が等しい
      if(((midx[i][1] - 1) <= midy[j][1]) && ((midx[i][1] + 1) >= midy[j][1])){ //y座標が+-1以内
        pattern[2][0] = midx[i][0];
        pattern[2][1] = midx[i][1];
        i = midnumx; //ループ脱出
        j = midnumy;
      }
    }

  }
}

/*
for(int i = 0; i < 3; i++){
  printf("%d %d\n", pattern[i][0], pattern[i][1]);
}
*/

int small = 1000000; //内積最小値
int smallp[2];
int pnum; //どのパターンか
int inner; //内積
int outer; //外積
int multx;
int multy;

//pattern0について
multx = (pattern[0][0] - pattern[1][0]) * (pattern[0][0] - pattern[2][0]);
multy = (pattern[0][1] - pattern[1][1]) * (pattern[0][1] - pattern[2][1]);
inner = multx + multy;
if(small > inner){
  small = inner;
  pnum = 0;
  smallp[0] = pattern[0][0];
  smallp[1] = pattern[0][1];
}

//pattern1について
multx = (pattern[1][0] - pattern[0][0]) * (pattern[1][0] - pattern[2][0]);
multy = (pattern[1][1] - pattern[0][1]) * (pattern[1][1] - pattern[2][1]);
inner = multx + multy;
if(small > inner){
  small = inner;
  pnum = 1;
  smallp[0] = pattern[1][0];
  smallp[1] = pattern[1][1];
}

//pattern2について
multx = (pattern[2][0] - pattern[0][0]) * (pattern[2][0] - pattern[1][0]);
multy = (pattern[2][1] - pattern[0][1]) * (pattern[2][1] - pattern[1][1]);
inner = multx + multy;
if(small > inner){
  small = inner;
  pnum = 2;
  smallp[0] = pattern[2][0];
  smallp[1] = pattern[2][1];
}

pul[0] = smallp[0]; //左上検出
pul[1] = smallp[1];


//purとpdlの判別
if(pnum == 0){ //pattern0がpul
  multx = (pattern[1][0] - pattern[0][0]) * (pattern[2][1] - pattern[0][1]);
  multy = (pattern[1][1] - pattern[0][1]) * (pattern[2][0] - pattern[0][0]);
  outer = multx - multy;
  if(outer > 0){
    pur[0] = pattern[1][0];
    pur[1] = pattern[1][1];
    pdl[0] = pattern[2][0];
    pdl[1] = pattern[2][1];
  }else{
    pur[0] = pattern[2][0];
    pur[1] = pattern[2][1];
    pdl[0] = pattern[1][0];
    pdl[1] = pattern[1][1];
  }
}

if(pnum == 1){ //pattern1がpul
  multx = (pattern[2][0] - pattern[1][0]) * (pattern[0][1] - pattern[1][1]);
  multy = (pattern[2][1] - pattern[1][1]) * (pattern[0][0] - pattern[1][0]);
  outer = multx - multy;
  if(outer > 0){
    pur[0] = pattern[2][0];
    pur[1] = pattern[2][1];
    pdl[0] = pattern[0][0];
    pdl[1] = pattern[0][1];
  }else{
    pur[0] = pattern[0][0];
    pur[1] = pattern[0][1];
    pdl[0] = pattern[2][0];
    pdl[1] = pattern[2][1];
  }
}

if(pnum == 2){ //pattern2がpul
  multx = (pattern[0][0] - pattern[2][0]) * (pattern[1][1] - pattern[2][1]);
  multy = (pattern[0][1] - pattern[2][1]) * (pattern[1][0] - pattern[2][0]);
  outer = multx - multy;
  if(outer > 0){
    pur[0] = pattern[0][0];
    pur[1] = pattern[0][1];
    pdl[0] = pattern[1][0];
    pdl[1] = pattern[1][1];
  }else{
    pur[0] = pattern[1][0];
    pur[1] = pattern[1][1];
    pdl[0] = pattern[0][0];
    pdl[1] = pattern[0][1];
  }
}


printf("pul : %d %d\n", pul[0], pul[1]);
printf("pdl : %d %d\n", pdl[0], pdl[1]);
printf("pur : %d %d\n", pur[0], pur[1]);


//purとpdlの判別終わり
/*--------------------位置検出パターン中心座標検出終わり--------------------*/


/*--------------------シンボル復元--------------------*/

double sx; //x方向倍率
double sy; //y方向倍率
double tx; //x方向平行移動距離
double ty; //y方向平行移動距離
double theta; ////反時計回り回転角度
double pictx; //画像上のx座標
double picty; //画像上のy座標



sx = (pow(pul[0] - pur[0], 2) + pow(pul[1] - pur[1], 2)) / pow(WSYM - 15, 2);
sx = sqrt(sx);
sy = (pow(pul[0] - pdl[0], 2) + pow(pul[1] - pdl[1], 2)) / pow(WSYM - 15, 2);
sy = sqrt(sy);
//sx = round(sx);
//sy = round(sy);
double magnify[3][3] = {{sx,0,0}, {0,sy,0}, {0,0,1}}; //拡大


multx = (pdl[0] - pul[0]) * 0;
multy = (pdl[1] - pul[1]) * (pdl[1] - pul[1]);
inner = multx + multy;
theta = inner / (sqrt((pow(pul[0] - pdl[0], 2) + pow(pul[1] - pdl[1], 2))) * sqrt((pow(pul[0] - pul[0], 2) + pow(pul[1] - pdl[1], 2))));
theta = acos(theta);
theta = theta * 180 / PI;
//theta = round(theta);
double rad = -theta * PI / 180; //反時計回り回転ラジアン
double rot[3][3] = {{cos(rad),-sin(rad),0}, {sin(rad),cos(rad),0}, {0,0,1}}; //回転


tx = (double)pul[0] - ((double)pdl[0] + (double)pur[0] - 2*(double)pul[0])/(WSYM-15)*7;
ty = (double)pul[1] - ((double)pdl[1] + (double)pur[1] - 2*(double)pul[1])/(WSYM-15)*7;
//tx = round(tx);
//ty = round(ty);
double trans[3][3] = {{1,0,tx}, {0,1,ty}, {0,0,1}}; //並進
printf("倍率 x : %f\ty : %f\n平行移動 x : %f\ty : %f\n回転角 : %f\n", sx, sy, tx, ty, theta);



double affine[3][3] = {{0,0,0}, {0,0,0}, {0,0,0}}; //幾何変換のための行列
double inter[3][3] = {{0,0,0}, {0,0,0}, {0,0,0}}; //途中計算用

for(int i = 0; i < 3; i++){ //拡大、並進、回転行列を掛け合わせる
  for(int j = 0; j < 3; j++){
    for(int k = 0; k < 3; k++){
      inter[i][j] += trans[i][k] * rot[k][j];
    }
  }
}

for(int i = 0; i < 3; i++){
  for(int j = 0; j < 3; j++){
    for(int k = 0; k < 3; k++){
      affine[i][j] += inter[i][k] * magnify[k][j];
    }
  }
}


for(int qrx = 0; qrx < WSYM; qrx++){
  for(int qry = 0; qry < WSYM; qry++){
    pictx = affine[0][0] * qrx + affine[0][1] * qry + affine[0][2] * 1;
    picty = affine[1][0] * qrx + affine[1][1] * qry + affine[1][2] * 1;

    for(int a = 0; a < 3; a++){
      qrcode[qrx][qry][a] = picture[(int)pictx][(int)picty][a];
    }
  }
}
/*--------------------シンボル復元終わり--------------------*/


/*--------------------文字列抽出--------------------*/
int chari = 4;
int charj = 4; //文字列抽出に使う配列番号
int flag = 0; //抽出を行ったか判定
int csum = 0; //文字
int count = 0; //黒をカウント

while(1){ //符号化領域への埋め込み

  for(int num = 1; num < 256; num = num << 1){ //文字列抽出
    flag = 0;

    while(flag != 1){ //抽出を行うまでループ
      if(chari >= 12 && chari <= 20 && charj >= 4 && charj <= 11){
        if(qrcode[chari][charj][0] == 255){
          csum += num;
          flag = 1;
        }else{
          flag = 1;
          count++;
        }

      }else if(chari >= 4 && chari <= WSYM-5 && charj >= 12 && charj <= WSYM-13){
        if(qrcode[chari][charj][0] == 255){
          csum += num;
          flag = 1;
        }else{
          flag = 1;
          count++;
        }

      }else if(chari >= 12 && chari <= WSYM-5 && charj >= WSYM-12 && charj <= WSYM-5){
        if(qrcode[chari][charj][0] == 255){
          csum += num;
          flag = 1;
        }else{
          flag = 1;
          count++;
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

  if(count == 8) //コードが00000000なら終了
  break;


  printf("%c", csum);
  csum = 0;
  count = 0;
}
printf("\n");
/*--------------------文字列抽出終わり--------------------*/




/*
int ssum = 0;
for(int rix = 0; rix < PICTX; rix++){
  ssum = 0;
  for(int riy = 0; riy < PICTY; riy++){
    fprintf(wfp, "%d %d\n", runy[rix][riy][0], runy[rix][riy][1]);


    ssum += runy[rix][riy][1];
    if(ssum == PICTY) //長さの合計がPICTY
    break;
  }
}
*/



for(int j = 0; j < WSYM; j++){
  for(int i = 0; i < WSYM; i++){
    fprintf(wfp, "%d %d %d\n", qrcode[i][j][0], qrcode[i][j][1], qrcode[i][j][2]);
  }
}



fclose(wfp);
return 1;
}
