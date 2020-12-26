/* 2019/06/18 農夫のジレンマ 改良版Ver3 */
#include <stdio.h>
#include <stdbool.h> // for bool型
#include <string.h>  // for strcpy()
#pragma warning(disable:4996) // VC++対策 for fopen(),scanf()
#define CHAR_NUM 5

// 終了条件の列挙体
enum staEnd_e{GM_START,GM_END1,GM_END2,GM_END3,GM_CLEAR};

// ゲーム状態の構造体
struct _gameState{
    int lpCnt;// 試行回数
    enum staEnd_e gmSta_e;// 終了判定 
}gmSta_t;

// 登場人物の構造体 : 農夫、狼、山羊、キャベツ、船
struct _charData{
    char chImg[50]; // 登場人物の描画情報
    char scPrt[200];// 画面の描画情報
    bool is_mvPos;  // 初期位置から移動したか
}chDat_t[CHAR_NUM];

// プロトタイプ宣言
void gmSetup(struct _gameState gmSta_t[],struct _charData chDat_t[],FILE** fp,FILE** fp2);// ゲーム初期化の関数
void gmMain(struct _gameState gmSta_t[],struct _charData chDat_t[],FILE* fp,FILE* fp2);// メイン処理の関数
void gmDraw(struct _gameState gmSta_t[],struct _charData chDat_t[],FILE* fp,FILE* fp2);// ゲーム描画の関数
void gmEndPrint(struct _gameState gmSta_t[],struct _charData chDat_t[],FILE* fp,FILE* fp2);// 終了結果の関数

// Entry Point
int main(void){
    /* ゲームの準備 */  
    FILE *fp; FILE *fp2;// 記録用ファイルの生成
    gmSetup(&gmSta_t,chDat_t,&fp,&fp2);// ゲームの初期化
    /* メイン処理 */
    gmMain(&gmSta_t,chDat_t,fp,fp2);
    /* 終了結果 */
    gmEndPrint(&gmSta_t,chDat_t,fp,fp2);
    /* 書込処理 */
    fclose(fp); fclose(fp2);
    return(0);
}
/* ゲーム初期化の関数 */
void gmSetup(struct _gameState gmSta_t[],struct _charData chDat_t[],FILE** fp,FILE** fp2){
    // 記録用ファイルの生成
    *fp = fopen("imageLog.txt","w");// 遷移図を記録
    *fp2 = fopen("txtLog.txt","w");// 実行結果を記録
    // ゲーム状態の初期化
    gmSta_t->gmSta_e = GM_START;
    // 登場人物の読込
    strcpy(chDat_t[0].chImg, "農夫");
    strcpy(chDat_t[1].chImg, "  狼");
    strcpy(chDat_t[2].chImg, "  山羊");
    strcpy(chDat_t[3].chImg, "  キャベツ");
    strcpy(chDat_t[4].chImg, "\n船------------------------");
    for(int i=0;i<=CHAR_NUM;i++)chDat_t[i].is_mvPos = false;// 位置初期化
    // 画面の読込
    strcpy(chDat_t[0].scPrt, "\n\n\n\n農夫のジレンマゲーム  ");
    strcpy(chDat_t[1].scPrt, "回目\n--------------------------\n\n\n");
    strcpy(chDat_t[2].scPrt, "\n--------------------------");
    strcpy(chDat_t[3].scPrt, "\n--------------------------\n"
                               "----------川--------------\n"
                               "--------------------------");
}
/* メイン処理の関数 */
void gmMain(struct _gameState gmSta_t[],struct _charData chDat_t[],FILE* fp,FILE* fp2){
    for(gmSta_t->lpCnt = 0;gmSta_t->gmSta_e==GM_START;gmSta_t->lpCnt++){
    // 開始前の描画処理
    gmDraw(gmSta_t,chDat_t,fp,fp2);
        // 入力処理 
        for(int i=0;;){
            printf("\n\n\n\n農夫が次に運ぶアイテムを番号で選ぶ。\n");fprintf(fp2,"農夫が次に運ぶアイテムを番号で選ぶ。\n");
            printf("(0:農夫のみ 1:狼 2:山羊 3:キャベツ)");fprintf(fp2,"(0:農夫のみ 1:狼 2:山羊 3:キャベツ)");
            scanf("%d",&i);fprintf(fp2,"%d\n",i);
            if((!(chDat_t[0].is_mvPos))&&(!(chDat_t[i].is_mvPos))){//農夫が初期値の場合で、運ぶアイテムも初期値の場合
                switch(i){
                    case 0:chDat_t[0].is_mvPos=true;break;
                    case 1:chDat_t[0].is_mvPos=true;chDat_t[1].is_mvPos=true;break;
                    case 2:chDat_t[0].is_mvPos=true;chDat_t[2].is_mvPos=true;break;
                    case 3:chDat_t[0].is_mvPos=true;chDat_t[3].is_mvPos=true;break;
                    default:printf("もう一度入力してください。\n");fprintf(fp2,"もう一度入力してください。\n");continue;
                }break;
            }
            else if(chDat_t[0].is_mvPos&&chDat_t[i].is_mvPos){//農夫が川を渡っていて、運ぶアイテムも渡っているの場合
                switch(i){
                    case 0:chDat_t[0].is_mvPos=false;break;
                    case 1:chDat_t[0].is_mvPos=false;chDat_t[1].is_mvPos=false;break;
                    case 2:chDat_t[0].is_mvPos=false;chDat_t[2].is_mvPos=false;break;
                    case 3:chDat_t[0].is_mvPos=false;chDat_t[3].is_mvPos=false;break;
                    default:;printf("もう一度入力してください。\n");fprintf(fp2,"もう一度入力してください。\n");continue;
                }break;
            }
            else{// エラー処理
                switch(i){
                    case 1: printf("狼がいないよ\nもう一度入力してください。\n\n");
                            fprintf(fp2,"狼がいないよ\nもう一度入力してください。\n\n");break;
                    case 2: printf("山羊がいないよ\nもう一度入力してください。\n\n");
                            fprintf(fp2,"山羊がいないよ\nもう一度入力してください。\n\n");break;
                    case 3: printf("キャベツがいないよ\nもう一度入力してください。\n\n");
                            fprintf(fp2,"キャベツがいないよ\nもう一度入力してください。\n\n");break;
                    default:printf("もう一度入力してください。\n");
                            fprintf(fp2,"もう一度入力してください。\n");continue;
                }
            }
        }
        /* 終了判定 */ //結果を2進数→10進数に変換 左から農夫、狼、山羊、キャベツ 0010→2
        switch((chDat_t[0].is_mvPos*8)+(chDat_t[1].is_mvPos*4)+(chDat_t[2].is_mvPos*2)+(chDat_t[3].is_mvPos)){
            case 6:  //0110
            case 9:  gmSta_t->gmSta_e = GM_END1;break;//1001
            case 7:  //0111
            case 8:  gmSta_t->gmSta_e = GM_END2;break;//1000
            case 3:  //0011 
            case 12: gmSta_t->gmSta_e = GM_END3;break;//1100
            case 15: gmSta_t->gmSta_e = GM_CLEAR;break;//1111
            default:break;// 何もしない
        }
    }
}
/* メイン処理の関数 */
void gmDraw(struct _gameState gmSta_t[],struct _charData chDat_t[],FILE* fp,FILE* fp2){
    // タイトル描画
    printf("%s%d%s",chDat_t[0].scPrt,gmSta_t->lpCnt,chDat_t[1].scPrt);
    fprintf(fp,"%s%d%s",chDat_t[0].scPrt,gmSta_t->lpCnt,chDat_t[1].scPrt);
    // 登場人物描画(川を渡った場合)
    for(int i=0;i<CHAR_NUM-1;i++){
        if(chDat_t[i].is_mvPos){
            printf("%s",chDat_t[i].chImg);fprintf(fp,"%s",chDat_t[i].chImg);
        }
    }
    // 船の描画準備(農夫の位置をコピー)
    chDat_t[CHAR_NUM-1].is_mvPos = chDat_t[0].is_mvPos;        
    // 船の描画(川を渡ったか->true:渡った false:渡ってない)
    if(chDat_t[CHAR_NUM-1].is_mvPos){
        printf("%s",chDat_t[CHAR_NUM-1].chImg);fprintf(fp,"%s",chDat_t[CHAR_NUM-1].chImg);
    }else{
        printf("%s",chDat_t[2].scPrt);fprintf(fp,"%s",chDat_t[2].scPrt);
    }
    // 川の描画(普通に描画)
    printf("%s",chDat_t[3].scPrt);fprintf(fp,"%s",chDat_t[3].scPrt);
    // 船の描画(初期位置か->true:渡ってない false:渡った)
    if(!chDat_t[CHAR_NUM-1].is_mvPos){
        printf("%s\n",chDat_t[CHAR_NUM-1].chImg);fprintf(fp,"%s\n",chDat_t[CHAR_NUM-1].chImg);
    }else{
        printf("%s\n",chDat_t[2].scPrt);fprintf(fp,"%s\n",chDat_t[2].scPrt);
    }
    // 登場人物の描画(初期位置か)
    for(int i=0;i<CHAR_NUM-1;i++){
        if(!chDat_t[i].is_mvPos){
            printf("%s",chDat_t[i].chImg);fprintf(fp,"%s",chDat_t[i].chImg);
        }
    }
}
/* 終了結果の関数 */
void gmEndPrint(struct _gameState gmSta_t[],struct _charData chDat_t[],FILE* fp,FILE* fp2){
    // 終了後の再描画処理 
    gmDraw(gmSta_t,chDat_t,fp,fp2);
    // 終了結果の表示
    switch(gmSta_t->gmSta_e){
        case GM_CLEAR:printf("ゲームクリアです。\nループは%d回目",gmSta_t->lpCnt);
                      fprintf(fp,"ゲームクリアです。\nループは%d回目",gmSta_t->lpCnt);break;
        case GM_END1:printf("\n\n残念。狼が山羊を食べちゃった。\nループは%d回目",gmSta_t->lpCnt);
                     fprintf(fp,"\n\n残念。狼が山羊食べちゃった。\nループは%d回目",gmSta_t->lpCnt);break;
        case GM_END2:printf("\n\n残念。食物連鎖が起きてしまいました。\nループは%d回目",gmSta_t->lpCnt);
                     fprintf(fp,"\n\n残念。食物連鎖が起きてしまいました。\nループは%d回目",gmSta_t->lpCnt);break;
        case GM_END3:printf("\n\n残念。山羊がキャベツを食べちゃった。\nループは%d回目",gmSta_t->lpCnt);
                     fprintf(fp,"\n\n残念。山羊がキャベツを食べちゃった。\nループは%d回目",gmSta_t->lpCnt);break;
        default:break;// 何もしない
    }
}