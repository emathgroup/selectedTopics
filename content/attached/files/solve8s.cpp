#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "newdef.cpp"
#include <vector>
using namespace std;
#define ASSERT(x)
typedef char cinfo[2];
class ssolver{
    short _n;//number of variables
    short _m;//number of equations
    short _t;//original number of equations
    int   m;///number of columns
    int   lc;//left column count
    bool  _valid;
    bool  _good_choice;
    short **columnid;
    short *left_column;
    cinfo *column_info;
    mpq_t tmp1,tmp2;
    mpq_t **_matrix;
    char  *row_var;
    char  *row_evar;
    short  *ctmp;
    short  *ctmp2;

    void init_memory(){
        int i,j;
        m=_n*(_n+1)/2;
        columnid=new short *[_n];
        columnid[0]=new short[_n*_n];
        for(i=1;i<_n;i++)columnid[i]=columnid[i-1]+_n;
        memset(columnid[0],0,sizeof(short)*m);
        left_column=new short[m];
        column_info=new cinfo[m];
        _matrix = new mpq_t*[_m];///_m equations
        row_var =new char[_m];
        row_evar = new char[_m];
        ctmp =new short[m];
        ctmp2=new short[m];
        _matrix[0]=new mpq_t[m*_m];
        for(i=0;i<_m;i++){
            if(i>0)
            _matrix[i]=_matrix[i-1]+m;
            for(j=0;j<m;j++){
                mpq_init(_matrix[i][j]);
            }
        }
        for(i=0;i<_m;i++){row_var[i]=-1;row_evar[i]=-1;}
        mpq_init(tmp1);mpq_init(tmp2);
        set_column_index();
        _t=_m;
    }

    void set_column_index(){
        int i,j,s=0;
        for(i=0;i<_n;i++){
            for(j=0;j<=i;j++){
                column_info[s][0]=j;
                column_info[s][1]=i;
                columnid[i][j]=columnid[j][i]=s;
                s++;
            }
        }
    }

    void init_left_columns(){
        int i,j;
        for(i=0,lc=0;i<m;i++){
            for(j=0;j<_m;j++){
                if(mpq_sgn(_matrix[j][i])!=0)
                    break;
            }
            if(j<_m){
                left_column[lc++]=i;
            }
        }
        _valid=true;
    }

    void remove_empty_rows(){
        int i,j;
        for(i=0;i<_m;i++){
            for(j=0;j<lc;j++){
                int cid=left_column[j];
                if(mpq_sgn(_matrix[i][cid])!=0){
                    break;
                }
            }
            if(j==lc){
                swap_row(i,_m-1);
                i--;
                _m--;
            }
        }
    }

    int try_reduce_var(int varid);
    int reduce_iterator();
    int reduce_inorder();

    void simplify_result(){
        int i,j;
        for(i=_m;i<_t;i++){
            if(row_evar[i]==-1)continue;///empty line
            int mvar=row_var[i];
            int evar=row_evar[i];
            int c=0;
            for(j=0;j<_n;j++){
                if(j==evar)continue;
                int cid=columnid[mvar][j];
                if(mpq_sgn(_matrix[i][cid])==0)continue;
                ctmp2[c]=j;
                ctmp[c++]=cid;
            }
            if(c==0){
                _valid=false;
                return;
            }
            for(j=i+1;j<_t;j++){
                int mmvar=row_var[j];
                if(mmvar==-1)continue;
                int ncid=columnid[mmvar][evar];
                int k;
                if(mpq_sgn(_matrix[j][ncid])==0)continue;
                mpq_set(tmp1,_matrix[j][ncid]);
                mpq_set_ui(_matrix[j][ncid],0,1);///clear it to zero
                for(k=0;k<c;k++){
                    int ecid=columnid[mmvar][ctmp2[k]];
                    mpq_mul(tmp2,tmp1,_matrix[i][ctmp[k]]);
                    mpq_sub(_matrix[j][ecid],_matrix[j][ecid],tmp2);
                }
            }
        }
        if(row_evar[_t-1]!=-1){
            for(i=_m;i<_t-1;i++){
                if(row_evar[i]==-1){
                    swap_row(i, _t-1);///Make sure the last line is empty
                    break;
                }
            }
        }
    }

    int eliminate_row(int row, int cid){///return v to be eliminated, return -1 if invalid row find
        int i;
        char mvar = row_var[row];
        int evar;
        int ecid;
        if(column_info[cid][0]==mvar){
            evar=column_info[cid][1];
        }else{
            ASSERT(column_info[cid][1]==mvar);
            evar=column_info[cid][0];
        }
        row_evar[row]=evar;
        mpq_set(tmp1,_matrix[row][cid]);
        mpq_set_ui(_matrix[row][cid],1,1);
        ecid=cid;
        int c=0;
        for(i=0;i<lc;i++){
            cid=left_column[i];
            if(cid==ecid)continue;
            if(mpq_sgn(_matrix[row][cid])!=0){
                ctmp[c++]=cid;
                if(column_info[cid][0]==mvar){
                    ctmp2[c-1]=column_info[cid][1];
                }else{
                    ASSERT(column_info[cid][1]==mvar);
                    ctmp2[c-1]=column_info[cid][0];
                }
                mpq_div(_matrix[row][cid],_matrix[row][cid],tmp1);///normalize the line
            }
        }
        if(c==0){
            _valid=false;
#ifdef OUTPUT
            printf("Invalid row %d\n",row);
            output();
#endif
            return -1;///no solution 
        }
        if(row!=_m-1){
            swap_row(row,_m-1);
        }
        row=--_m;

        for(i=0;i<lc;i++){
            int cid=left_column[i];
            int ovar=-1;
            if(column_info[cid][0]==evar&&column_info[cid][1]==evar){
                int u,v,j;
                for(j=0;j<_m;j++){
                    mpq_set(tmp1, _matrix[j][cid]);
                    mpq_set_ui(_matrix[j][cid],0,1);//clear to zero
                    if(mpq_sgn(tmp1)!=0){
                        for(u=0;u<c;u++)for(v=u;v<c;v++){
                            int ncid=columnid[ctmp2[u]][ctmp2[v]];
                            mpq_mul(tmp2,_matrix[row][ctmp[u]],_matrix[row][ctmp[v]]);
                            if(u!=v){
                                mpq_add(tmp2,tmp2,tmp2);
                            }
                            mpq_mul(tmp2,tmp2,tmp1);
                            mpq_add(_matrix[j][ncid],_matrix[j][ncid],tmp2);
                        }
                    }
                }
                continue;
            }else if(column_info[cid][0]==evar){
                ovar=column_info[cid][1];
            }else if(column_info[cid][1]==evar){
                ovar=column_info[cid][0];
            }
            if(ovar>=0){
                int u,j;
                for(j=0;j<_m;j++){
                    mpq_set(tmp1,_matrix[j][cid]);
                    mpq_set_ui(_matrix[j][cid],0,1);
                    if(mpq_sgn(tmp1)!=0){
                        for(u=0;u<c;u++){
                            int ncid=columnid[ctmp2[u]][ovar];
                            mpq_mul(tmp2, _matrix[row][ctmp[u]],tmp1);
                            mpq_sub(_matrix[j][ncid],_matrix[j][ncid],tmp2);
                        }
                    }
                }
            }
        }
        init_left_columns();
        return 0;
    }

    void swap_row(int x,int y){
        if(x==y)return;
        int i;
        for(i=0;i<lc;i++){
            int cid=left_column[i];
            mpq_set(tmp1, _matrix[x][cid]);
            mpq_set(_matrix[x][cid],_matrix[y][cid]);
            mpq_set(_matrix[y][cid],tmp1);
        }
        char c=row_var[x];
        row_var[x]=row_var[y];
        row_var[y]=c;
        c=row_evar[x];
        row_evar[x]=row_evar[y];
        row_evar[y]=c;
    }

public:
    ssolver(short n,short m){_n=n,_m=m;init_memory();_good_choice=false;}

    ~ssolver(){
        int i,j;
        mpq_clear(tmp1);mpq_clear(tmp2);
        for(i=0;i<_t;i++){
            for(j=0;j<m;j++){
                mpq_clear(_matrix[i][j]);
            }
        }
        delete []_matrix[0];
        delete []_matrix;
        delete []columnid[0];
        delete []columnid;
        delete []left_column;
        delete []column_info;
        delete []row_var;
        delete []row_evar;
        delete []ctmp;
        delete []ctmp2;
    }

    void output(){
        int i,j;
        int start=1;
        printf("solve([");
        if(_m>0){
            for(i=0;i<_n;i++){
                ctmp[i]=0;
            }
            for(i=0;i<lc;i++){
                int cid=left_column[i];
                ctmp[column_info[cid][0]]=1;
                ctmp[column_info[cid][1]]=1;
            }
            for(i=0;i<_m;i++){
                int s;
#ifdef OUTPUT
                if(i!=0)printf(",\n");
#else
                if(i!=0)printf(",");
#endif
                start=0;
                for(j=0;j<lc;j++){
                    int cid=left_column[j];
                    if((s=mpq_sgn(_matrix[i][cid]))!=0){
                        if(s>0){
                            printf("+");
                        }
                        mpq_out_str(stdout,10,_matrix[i][cid]);
                        if(column_info[cid][0]>0){
                            if(column_info[cid][0]&1){//X
                                printf("*%c_X",column_info[cid][0]/2+'A');
                            }else{
                                printf("*%c_Y",(column_info[cid][0]-2)/2+'A');
                            }
                        }
                        if(column_info[cid][1]>0){
                            if(column_info[cid][1]&1){//X
                                printf("*%c_X",column_info[cid][1]/2+'A');
                            }else{
                                printf("*%c_Y",(column_info[cid][1]-2)/2+'A');
                            }
                        }
                    }
                }
            }
        }
        for(i=_m;i<_t;i++){
            int varid=row_var[i];
            if(varid==-1)continue;
            if(start)start=0;
            else printf(",");
            for(j=0;j<_n;j++){
                int cid=columnid[varid][j];
                int s;
                if((s=mpq_sgn(_matrix[i][cid]))!=0){
                    if(s>0)printf("+");
                    mpq_out_str(stdout,10,_matrix[i][cid]);
                    if(j>0){
                        if(j&1){//X
                            printf("*%c_X",j/2+'A');
                        }else{
                            printf("*%c_Y",(j-2)/2+'A');
                        }
                    }
                }
            }
        }
        printf("],[");
        start=1;
        if(_m>0){
            int tt=0;
            for(i=1;i<_n;i++){
                if(ctmp[i]){
                    tt++;
                    if(start)start=0;
                    else printf(",");
                    if(i&1){//X
                        printf("%c_X",i/2+'A');
                    }else{
                        printf("%c_Y",(i-2)/2+'A');
                    }
                }
            }
            if(tt>=_m){
                _good_choice=true;
            }
        }else{
            _good_choice=true;
        }
        for(i=_m;i<_t;i++){
            int varid=row_var[i];
            if(varid==-1)continue;
            if(start)start=0;
            else printf(",");
            int var=row_evar[i];
            if(var&1){
                printf("%c_X",var/2+'A');
            }else{
                printf("%c_Y",(var-2)/2+'A');
            }
        }
        printf("]);\n");
    }
    bool isValid()const{return _valid;}
    bool isGoodChoice()const{return _good_choice;}

    void insert_item(int row, int v1, int v2, mpq_t value){
        int cid=columnid[v1][v2];
        mpq_add(_matrix[row][cid],_matrix[row][cid],value);
    }

    int left_equ()const{return _m;}

    void simplify();
};

#define MAKE_ITEM(item_1, item_2)   ((((item_1)+1)<<16)|((item_2)+1))
#define GET_ITEM_1(x)               (((x)>>16)-1)
#define GET_ITEM_2(x)               (((x)&0xFFFF)-1)
#define MAKE_SAFE_ITEM(item_1, item_2)  ((item_1)>(item_2)?MAKE_ITEM(item_1,item_2):MAKE_ITEM(item_2,item_1))

#define NUM_NODES 80
#define NODES_PER_EDGE   4

#define MAX_EDGES_PER_NODE  ((NUM_NODES-1)/(2))
#define MAX_TOTAL_EDGES     ((MAX_EDGES_PER_NODE*NUM_NODES)/3)

char chessboard[NUM_NODES][NUM_NODES];
char used[NUM_NODES];
char line_buf[MAX_TOTAL_EDGES][NODES_PER_EDGE];
int line_count;
int cur_free_edge;
int cur_max_degree;
int curr_comb_id;
int curr_line_num;
int last_comb_id;
int last_line_num;


void output_cur()
{
    int i,j;
    for(i=0;i<line_count;i++)for(j=0;j<NODES_PER_EDGE;j++){
      if(line_buf[i][j]>=0)
        printf("%c",'A'+line_buf[i][j]);
      else printf(" ");
    }
    printf("\n");
}

void init()
{
    int i,j;
    line_count=0;
    for(i=0;i<NUM_NODES;i++)for(j=0;j<NUM_NODES;j++){
        chessboard[i][j]=-1;
    }
    for(i=0;i<NUM_NODES;i++)used[i]=0;
    curr_comb_id=0;
    curr_line_num=0;
}
void init_mem(){
}

char test(int x,int y,int z, int w)
{
    if(chessboard[x][y]>=0||chessboard[x][z]>=0||
            chessboard[x][w]>=0||chessboard[y][z]>=0||
            chessboard[y][w]>=0||chessboard[z][w]>=0)
        return 0;
    return 1;
}

void set_to(int x,int y, int z,int w, int value)
{
  if(w>=0){
    chessboard[x][y]=chessboard[x][z]=chessboard[x][w]=
        chessboard[y][x]=chessboard[y][z]=chessboard[y][w]=
        chessboard[z][x]=chessboard[z][y]=chessboard[z][w]=
        chessboard[w][x]=chessboard[w][y]=chessboard[w][z]=value;
   }else{
      chessboard[x][y]=chessboard[x][z]=
         chessboard[y][x]=chessboard[y][z]=
         chessboard[z][x]=chessboard[z][y]=value;
   }
}

void setv(int x,int y,int z,int w)
{
    if(w<0)w=-1;
    set_to(x,y,z,w,line_count);
    used[x]++;
    used[y]++;
    used[z]++;
    if(w>=0)used[w]++;
    line_buf[line_count][0]=x;
    line_buf[line_count][1]=y;
    line_buf[line_count][2]=z;
    line_buf[line_count][3]=w;
    line_count++;
}

void unsetv(int x,int y,int z,int w)
{
    set_to(x,y,z,w,-1);
    used[x]--;
    used[y]--;
    used[z]--;
    if(w>=0)used[w]--;
    line_count--;
}

enum value_type{ SET_CONST,UNSET,SET_INDEX,SET_INFTY};

void sort_by_feature(char nodes[NODES_PER_EDGE], int fx[NUM_NODES],int fy[NUM_NODES]){
    int i,j;
    for(i=0;i<NODES_PER_EDGE;i++){
        if(nodes[i]<0)continue;
        for(j=i+1;j<NODES_PER_EDGE;j++){
          if(nodes[j]<0)continue;
            if(fx[nodes[i]]>fx[nodes[j]]||
                    fx[nodes[i]]==fx[nodes[j]]&&fy[nodes[i]]>fy[nodes[j]]){
                char t=nodes[i];
                nodes[i]=nodes[j];
                nodes[j]=t;
            }

        }
    }
}


void output_node_x(char a, int fx[],int x[])
{
    if(fx[a]==UNSET){
        printf("%c_x",a+'A');
    }else if(fx[a]==SET_CONST){
        printf("%d",x[a]);
    }else{
        fprintf(stderr,"Invalid \n");
        exit(-1);
    }
}

void output_node_y(char a, int fy[],int y[])
{
    if(fy[a]==UNSET){
        printf("%c_y",a+'A');
    }else if(fy[a]==SET_CONST){
        printf("%d",y[a]);
    }else{
        fprintf(stderr,"Invaid\n");
        exit(-1);
    }
}

int ssolver::try_reduce_var(int varid)
{
    int cur_row=0;
    int i,j,k;
#ifdef OUTPUT
    if(varid==0){
        printf("try reduce constant\n");
    }else if(varid&1){
        printf("try reduce %c_X\n",'A'+varid/2);
    }else{
        printf("try reduce %c_Y\n",'A'+(varid-2)/2);
    }
#endif
    for(i=0;i<lc;i++){
        int cid=left_column[i];
        if(column_info[cid][0]==varid||column_info[cid][1]==varid)continue;
        for(j=cur_row;j<_m;j++){
            if(mpq_sgn(_matrix[j][cid])!=0)
                break;
        }
        if(j==_m)continue;
        if(j!=cur_row){
            swap_row(j,cur_row);
        }
        mpq_set(tmp1, _matrix[cur_row][cid]);
        for(j=0;j<lc;j++){///normalize the line
            int cid2=left_column[j];
            if(mpq_sgn(_matrix[cur_row][cid2])!=0){
                mpq_div(_matrix[cur_row][cid2],_matrix[cur_row][cid2],tmp1);
            }
        }
        for(k=0;k<_m;k++){///searching all rows except this
            if(k==cur_row)continue;
            if(mpq_sgn(_matrix[k][cid])!=0){
                mpq_set(tmp1,_matrix[k][cid]);
                for(j=0;j<lc;j++){
                    int cid2=left_column[j];
                    if(mpq_sgn(_matrix[cur_row][cid2])!=0){
                        mpq_mul(tmp2,tmp1,_matrix[cur_row][cid2]);
                        mpq_sub(_matrix[k][cid2],_matrix[k][cid2],tmp2);
                    }
                }
            }
        }
        cur_row++;
        if(cur_row==_m)break;
    }
#ifdef OUTPUT
        printf("After eliminate row %d\n", cur_row);
        output();
#endif
    if(cur_row==_m)return 0;
    for(i=cur_row;i<_m;i++){
        row_var[i]=varid;
    }
    int fr=0;
    int end_row=cur_row;
    for(i=_m-1;i>=end_row;i--){
        int j;
        int have_const=0,have_varid=0;
#ifdef OUTPUT
        printf("detect row %d\n",i);
#endif
        for(j=0;j<lc;j++){
            int cid=left_column[j];
            if(varid!=0&&(column_info[cid][0]==0||column_info[cid][1]==0)){
                if(mpq_sgn(_matrix[i][cid])!=0){
#ifdef OUTPUT
           printf("detect const at column %d\n",j);
#endif
                    have_const=1;
                }
            }else if(column_info[cid][0]==varid&&column_info[cid][1]==varid){
                if(mpq_sgn(_matrix[i][cid])!=0){
#ifdef OUTPUT
           printf("detect var at column %d\n",j);
#endif
                    have_varid=1;
                }
            }else if(mpq_sgn(_matrix[i][cid])!=0)break;
        }
        if(j==lc){
            if(!have_const&&!have_varid){///empty row
                if(i!=_m-1){
                    swap_row(i,_m-1);
                }
                row_var[_m-1]=-1;
                --_m;
            }else if(!have_const||!have_varid){
                return -1;
            }else{
                if(i!=end_row){
                    swap_row(i,end_row);
                }
                end_row++;
            }
            continue;
        }
        int r=eliminate_row(i,left_column[j]);
#ifdef OUTPUT
        printf("after eliminate row, r=%d\n",r);
        output();
#endif
        if(r==-1)return -1;
        fr=1;
    }
#ifdef OUTPUT
    printf("Before last eliminate\n");
    output();
#endif
    if(cur_row<end_row){///Need to eliminate varid
        int cid_const=columnid[0][varid];
        int cid_var=columnid[varid][varid];
        mpq_div(tmp1, _matrix[cur_row][cid_const], _matrix[cur_row][cid_var]);
        for(i=cur_row+1;i<end_row;i++){
            mpq_div(tmp2,_matrix[i][cid_const],_matrix[i][cid_var]);
            if(mpq_cmp(tmp1,tmp2)!=0)
                return -1;
        }
        int r=eliminate_row(end_row-1,cid_var);
        if(r==-1)return -1;
        fr=1;
        if(end_row>cur_row+1){
            remove_empty_rows();
        }
    }
#ifdef OUTPUT
    printf("After reduce var\n");
    output();
#endif
    return fr;
}

int ssolver::reduce_inorder()
{
    int i;
    int max_c=-1,max_index=-1;
    for(i=0;i<_n;i++){
        ctmp[i]=0;
    }
    for(i=0;i<lc;i++){
        int cid=left_column[i];
        if(column_info[cid][0]==column_info[cid][1]){
            ctmp[column_info[cid][0]]++;
        }else{
            ctmp[column_info[cid][0]]++;
            ctmp[column_info[cid][1]]++;
        }
    }
    for(i=0;i<_n;i++){
        if(ctmp[i]>0){
            int r=try_reduce_var(i);
            if(r!=0)return r;
        }
    }
    return 0;
}

int ssolver::reduce_iterator()
{
    int i;
    int max_c=-1,max_index=-1;
    for(i=0;i<_n;i++){
        ctmp[i]=0;
    }
    for(i=0;i<lc;i++){
        int cid=left_column[i];
        if(column_info[cid][0]==column_info[cid][1]){
            ctmp[column_info[cid][0]]++;
        }else{
            ctmp[column_info[cid][0]]++;
            ctmp[column_info[cid][1]]++;
        }
    }
    for(i=0;i<_n;i++){
        if(ctmp[i]>max_c){
            max_c=ctmp[i];
            max_index=i;
        }
    }
    return try_reduce_var(max_index);
}

ssolver *solver;
void add_item_x(int x_n, int fx[], int x[], bool sub)
{
    if(fx[x_n]==UNSET){
        mpq_t q;mpq_init(q);mpq_set_ui(q,1,1);
        if(sub)mpq_neg(q,q);
        solver->insert_item(curr_line_num, 2*x_n+1,0,q);
        mpq_clear(q);
    }else if(fx[x_n]==SET_CONST){
        mpq_t q;mpq_init(q);mpq_set_ui(q,x[x_n],1);
        if(sub)mpq_neg(q,q);
        solver->insert_item(curr_line_num, 0,0, q);
        mpq_clear(q);
    }else{
        fprintf(stderr,"invaid\n");
    }
}

void add_item_y(int y_n, int fy[], int y[], bool sub)
{
    if(fy[y_n]==UNSET){
        mpq_t q;mpq_init(q);mpq_set_ui(q,1,1);
        if(sub)mpq_neg(q,q);
        solver->insert_item(curr_line_num, 2*y_n+2, 0, q);
        mpq_clear(q);
    }else if(fy[y_n]==SET_CONST){
        mpq_t q;mpq_init(q);mpq_set_ui(q,y[y_n],1);
        if(sub)mpq_neg(q,q);
        solver->insert_item(curr_line_num, 0, 0, q);
        mpq_clear(q);
    }else{
        fprintf(stderr,"invaid\n");
    }
}

void add_an_item(int x_n, int y_n, int fx[], int x[], int fy[], int y[] , bool sub)
{
    if(fx[x_n]==UNSET&&fy[y_n]==UNSET){
        mpq_t q;
        mpq_init(q);mpq_set_ui(q,1,1);
        if(sub)mpq_neg(q,q);
        solver->insert_item(curr_line_num, 2*x_n+1, 2*y_n+2,q);
        mpq_clear(q);
    }else if(fx[x_n]==UNSET&&fy[y_n]==SET_CONST){
        mpq_t q;mpq_init(q);mpq_set_ui(q,y[y_n],1);
        if(sub)mpq_neg(q,q);
        solver->insert_item(curr_line_num, 2*x_n+1,0,q);
        mpq_clear(q);
    }else if(fx[x_n]==SET_CONST&&fy[y_n]==UNSET){
        mpq_t q;mpq_init(q);mpq_set_ui(q,x[x_n],1);
        if(sub)mpq_neg(q,q);
        solver->insert_item(curr_line_num, 2*y_n+2,0,q);
        mpq_clear(q);
    }else if(fx[x_n]==SET_CONST&&fy[y_n]==SET_CONST){
        mpq_t q;mpq_init(q);mpq_set_ui(q,x[x_n]*y[y_n],1);
        if(sub)mpq_neg(q,q);
        solver->insert_item(curr_line_num, 0,0, q);
        mpq_clear(q);
    }else{
        fprintf(stderr,"Invalid \n");
        exit(-1);
    }
}

#define ADD_AN_ITEM(xn,yn) add_an_item(xn,yn,fx,x,fy,y,false)
#define SUB_AN_ITEM(xn,yn) add_an_item(xn,yn,fx,x,fy,y,true)
#define ADD_X(xn)          add_item_x(xn,fx,x,false)
#define SUB_X(xn)          add_item_x(xn,fx,x,true)
#define ADD_Y(yn)          add_item_y(yn,fy,y,false)
#define SUB_Y(yn)          add_item_y(yn,fy,y,true)

void output_line(char a,char b,char c, int fx[],int fy[],int x[],int y[]){
    if(fx[c]!=SET_INFTY){///Limit case
        ADD_AN_ITEM(b,a);
        ADD_AN_ITEM(c,b);
        ADD_AN_ITEM(a,c);
        SUB_AN_ITEM(b,c);
        SUB_AN_ITEM(c,a);
        SUB_AN_ITEM(a,b);
    }else{
        if(fy[c]==SET_INFTY){///parallel to x or y coord
            if(x[c]==1&&y[c]==0){///y=const
                ADD_Y(a);
                SUB_Y(b);
            }else if(x[c]==0&&y[c]==1){
                ADD_X(a);
                SUB_X(b);
            }else{
                fprintf(stderr,"invalid data");
                exit(-1);
            }
        }else{///The ratio
            ADD_Y(b);
            SUB_Y(a);
            SUB_AN_ITEM(b,c);
            ADD_AN_ITEM(a,c);
        }
    }
    curr_line_num++;
}

void ssolver::simplify()
{
    init_left_columns();
#ifdef OUTPUT
    printf("Before remove empty rows\n");
    output();
#endif
    remove_empty_rows();
#ifdef OUTPUT
    printf("After init\n");
    output();
#endif
    do{
        int r=reduce_iterator();
        if(r==-1){
            _valid=false;
            return;
        }
        if(r==1)continue;
#ifdef OUTPUT
        printf("before reduce inorder\n");
        output();
#endif
        r=reduce_inorder();
#ifdef OUTPUT
        printf("After reduce inorder\n");
        output();
#endif
        if(r==-1){
            _valid=false;
            return;
        }
        if(r==0)break;
    }while(1);
    if(_valid){
        simplify_result();
    }
}

int solve_problem(char *input,
        char e_to_n[MAX_TOTAL_EDGES][MAX_TOTAL_EDGES],
        char n_to_e[NUM_NODES][NUM_NODES],
        int best_i,int best_j,int best_k,
        int cur_score,
        bool force_dump=false)
{
    curr_line_num=0;
    curr_comb_id=0;
    solver =new ssolver(2*NUM_NODES+1, 2*line_count);
    int a=e_to_n[best_j][best_k];///(0,0,1)
    int b=e_to_n[best_i][best_k];///(1,0,0)
    int c=e_to_n[best_i][best_j];///(0,1,0)
    int x[NUM_NODES],y[NUM_NODES];
    int fx[NUM_NODES],fy[NUM_NODES];
    int i;
    for(i=0;i<NUM_NODES;i++){
        fx[i]=fy[i]=UNSET;///value unset
    }
    fx[a]=fy[a]=SET_CONST;x[a]=y[a]=0;
    fx[b]=fy[b]=SET_INFTY;x[b]=1;y[b]=0;
    fx[c]=fy[c]=SET_INFTY;x[c]=0;y[c]=1;
    for(i=0;i<NODES_PER_EDGE;i++){///in best_k, y=0
        if(line_buf[best_k][i]>=0&&line_buf[best_k][i]!=a&&line_buf[best_k][i]!=b){
            int n=line_buf[best_k][i];
            fx[n]=fy[n]=SET_CONST;
            x[n]=1;y[n]=0;
            break;
        }
    }
    for(i++;i<NODES_PER_EDGE;i++){
        if(line_buf[best_k][i]>=0&&line_buf[best_k][i]!=a&&line_buf[best_k][i]!=b){
            int n=line_buf[best_k][i];
            fy[n]=SET_CONST;
            y[n]=0;
            break;
        }
    }
    for(i=0;i<NODES_PER_EDGE;i++){///in best_j, x=0
        if(line_buf[best_j][i]>=0&&line_buf[best_j][i]!=a&&line_buf[best_j][i]!=c){
            int n=line_buf[best_j][i];
            fx[n]=fy[n]=SET_CONST;
            x[n]=0;y[n]=1;
            break;
        }
    }
    for(i++;i<NODES_PER_EDGE;i++){
        if(line_buf[best_j][i]>=0&&line_buf[best_j][i]!=a&&line_buf[best_j][i]!=c){
            int n=line_buf[best_j][i];
            fx[n]=SET_CONST;
            x[n]=0;
            break;
        }
    }
    for(i=0;i<NODES_PER_EDGE;i++){///in best_i, INFTY LINE
        if(line_buf[best_i][i]>=0&&line_buf[best_i][i]!=b&&line_buf[best_i][i]!=c){
            int n=line_buf[best_i][i];
            fx[n]=SET_INFTY;
            fy[n]=UNSET;
            x[n]=1;
        }
    }

    for(i=0;i<line_count;i++){
        if(i==best_i||i==best_j||i==best_k)
            continue;
        int s,t,z,w;
        sort_by_feature(line_buf[i],fx,fy);
        s=line_buf[i][0],t=line_buf[i][1],z=line_buf[i][2],w=line_buf[i][3];
        if(fx[t]==SET_INFTY||w>=0&&fx[z]==SET_INFTY||fx[s]==SET_INFTY){
            printf("%s\nInvalid lines data %d, %d, %d, %d{fx[t]=%d,fx[z]=%d,fx[s]=%d,w=%d}\n",input, best_i, best_j, best_k, i,fx[t],fx[z],fx[s],w);
            delete solver;
            return -2;
        }
        if(w>=0&&fx[w]==SET_INFTY){
            output_line(s,t,w,fx,fy,x,y);
            output_line(s,z,w,fx,fy,x,y);
        }else{
            output_line(s,t,z,fx,fy,x,y);
            if(w>=0)output_line(s,t,w,fx,fy,x,y);
        }
    }
    last_comb_id = curr_comb_id;
    last_line_num = curr_line_num;

    solver->simplify();
    int score=-1;
    if(solver->isValid()){
        if(solver->left_equ()<=1)
            score=0;
        else if(solver->isGoodChoice()){
            score=solver->left_equ();
        }else{
            score=100+solver->left_equ();
        }
        if(force_dump){
            printf("print(%s);\n",input);
            solver->output();
            printf("print(\"");
            for(i=0;i<NUM_NODES;i++){
                if(fx[i]==SET_INFTY&&fy[i]==SET_INFTY){
                    printf("%c=(%d,%d,0) ",i+'A',x[i],y[i]);
                }else if(fx[i]==SET_INFTY){
                    printf("%c=(1,%c_y,0) ",i+'A',i+'A');
                }else{
                    if(fx[i]==SET_CONST){
                        printf("%c_x=%d ",i+'A',x[i]);
                    }
                    if(fy[i]==SET_CONST){
                        printf("%c_y=%d ",i+'A',y[i]);
                    }
                }
            }
            printf("\");\n");
            fflush(stdout);
            fflush(stderr);
        }
    }

    delete solver;
    return score;
}

int chtoi(char c)
{
	if(c>='A'&&c<='Z')return c-'A';
	else if(c>='a')return c-'a'+26;
}

void process_one_line(char *input)
{
    int i,j,k;
    init();
    char *sinput=input;
    if(*input=='{'){
        while(*input!='}')input++;input++;
    }
    for(i=0;;i++){
        char c=input[i*4];
        if(c>='A'&&c<='Z'||c>='a'&&c<='z'){
            setv(chtoi(input[i*4]),chtoi(input[i*4+1]),chtoi(input[i*4+2]),chtoi(input[i*4+3]));
        }else{
            break;
        }
    }

    int max_degree=0;
    char e_to_n[MAX_TOTAL_EDGES][MAX_TOTAL_EDGES];
    char n_to_e[NUM_NODES][NUM_NODES];
    for(i=0;i<MAX_TOTAL_EDGES;i++)for(j=0;j<MAX_TOTAL_EDGES;j++){
        e_to_n[i][j]=-1;
    }
    for(i=0;i<NUM_NODES;i++)for(j=0;j<NUM_NODES;j++){
        n_to_e[i][j]=-1;
    }
    for(i=0;i<line_count;i++){
        for(j=0;j<NODES_PER_EDGE;j++){
            for(k=j+1;k<NODES_PER_EDGE;k++){
              if(line_buf[i][k]>=0)
                n_to_e[line_buf[i][j]][line_buf[i][k]]=
                    n_to_e[line_buf[i][k]][line_buf[i][j]]=i;
            }
        }
    }
    for(i=0;i<NUM_NODES;i++){
        for(j=0;j<NUM_NODES;j++){
            if(n_to_e[i][j]==-1)continue;
            for(k=j+1;k<NUM_NODES;k++){
                if(n_to_e[i][k]==-1)continue;
                if(n_to_e[i][j]==n_to_e[i][k])continue;
                e_to_n[n_to_e[i][j]][n_to_e[i][k]]=
                    e_to_n[n_to_e[i][k]][n_to_e[i][j]]=i;
            }
        }
    }

    int best_score=10000;
    int best_i=-1,best_j=-1,best_k=-1;
    int a,b,c;

    for(i=0;i<line_count;i++){
        for(j=i+1;j<line_count;j++){
            if(e_to_n[i][j]==-1)continue;
            for(k=j+1;k<line_count;k++){
                if(e_to_n[i][k]==-1||e_to_n[j][k]==-1)continue;
                if(e_to_n[i][j]==e_to_n[i][k])continue;

                int score;
                score = solve_problem(input,e_to_n,n_to_e,i,j,k,best_score);
                if(score<best_score&&score>=0){
                    best_score=score;
                    best_i=i,best_j=j,best_k=k;
                }
                if(score==-1){
                    return;
                }
                if(score<=1)break;
                score = solve_problem(input,e_to_n,n_to_e,j,k,i,best_score);
                if(score<best_score&&score>=0){
                    best_score=score;
                    best_i=i,best_j=j,best_k=k;
                }
                if(score==-1){
                    return;
                }
                if(score<=1)break;
                score = solve_problem(input,e_to_n,n_to_e,k,i,j,best_score);
                if(score<best_score&&score>=0){
                    best_score=score;
                    best_i=i,best_j=j,best_k=k;
                }
                if(score==-1){
                    return;
                }
                if(score>=0&&score<=1)break;
           }
        }
    }
    printf("%s\n",sinput);fflush(stdout);
}

#define LINE_BUF_SIZE (MAX_TOTAL_EDGES*(NODES_PER_EDGE+1)+20)
void process_file()
{
    char line[LINE_BUF_SIZE];
    while(fgets(line,LINE_BUF_SIZE,stdin)){
        int len=strlen(line);
        while(len>0&&line[len-1]!=' '&&isspace(line[len-1]))len--;
        line[len]='\0';
        process_one_line(line);
    }
}

int main(int argc, char *argv[])
{
    init_mem();
    process_file();
    return 0;
}

