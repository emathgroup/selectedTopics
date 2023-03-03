#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <algorithm>
#include <vector>
#include <set>
using namespace std;
template <int TNODES,int NODES_PER_EDGE=4>
class node_edge_set{
    public:
        static const int max_edges_per_node = (TNODES-1)/(NODES_PER_EDGE-1);
        static const int max_total_edges = (((TNODES-1)/(NODES_PER_EDGE-1))*TNODES)/NODES_PER_EDGE;
        struct final_edge_set{
            static int num_edges;
            char edges[max_total_edges][NODES_PER_EDGE];
            static void set_curr_num_edges(int n){num_edges=n;}
            bool operator<(const final_edge_set& s)const{
                int i,j;
                for(i=0;i<num_edges;i++)for(j=0;j<NODES_PER_EDGE;j++){
                    if(edges[i][j]<s.edges[i][j])return true;
                    if(edges[i][j]>s.edges[i][j])return false;
                }
                return false;
            }
            bool operator==(const final_edge_set& s)const{
                int i,j;
                for(i=0;i<num_edges;i++)for(j=0;j<NODES_PER_EDGE;j++){
                    if(edges[i][j]!=s.edges[i][j])return false;
                }
                return true;
            }
            void show()const{
                int i,j;
                for(i=0;i<num_edges;i++){
                    for(j=0;j<NODES_PER_EDGE;j++){
                        printf("%c",'A'+edges[i][j]);
                    }
                    printf(" ");
                }
                printf("\n");
            }
            void show(char *outbuf)const{
                int i,j;
                for(i=0;i<num_edges;i++){
                    for(j=0;j<NODES_PER_EDGE;j++){
                        outbuf[i*NODES_PER_EDGE+j]='A'+edges[i][j];
                    }
                }
                outbuf[i*NODES_PER_EDGE]='\0';
            }
            void show(int step_id)const{
                char tname[100];
                char obuf[1000];
                char tbuf[1000];
                int i,j;
                int checksum=0;
                for(i=0;i<num_edges;i++)for(j=0;j<NODES_PER_EDGE;j++){
                    checksum+=edges[i][j]*(i+1);
                }
                sprintf(tname,"data/t%d/%d",step_id,checksum);
                FILE *pOutput=fopen(tname,"a+");
                if(pOutput==NULL){
                    fprintf(stderr,"Cannot write to file %s\n",tname);
                    exit(-1);
                }
                for(i=0;i<num_edges;i++){
                    for(j=0;j<NODES_PER_EDGE;j++){
                        tbuf[i*NODES_PER_EDGE+j]='A'+edges[i][j];
                    }
                }
                tbuf[(num_edges)*NODES_PER_EDGE]='\n';
                tbuf[(num_edges)*NODES_PER_EDGE+1]='\0';
                if(fseek(pOutput,-(num_edges*NODES_PER_EDGE+2),SEEK_END)==0){
                    fread(obuf,num_edges*NODES_PER_EDGE+2,1,pOutput);
                    if(obuf[0]=='\n'&&strncmp(obuf+1,tbuf,(num_edges-2)*NODES_PER_EDGE)==0){
                        fclose(pOutput);
                        return;
                    }
                }else if(fseek(pOutput,0,SEEK_SET)==0){
                    fread(obuf,num_edges*NODES_PER_EDGE+1,1,pOutput);
                    if(strncmp(obuf,tbuf,num_edges*NODES_PER_EDGE+1)==0){
                        fclose(pOutput);
                        return;
                    }
                }
                fseek(pOutput,0,SEEK_END);
                fwrite(tbuf,num_edges*NODES_PER_EDGE+1,1,pOutput);
                fclose(pOutput);
            }
            void show(FILE *pFile)const{
                int i,j;
                for(i=0;i<num_edges;i++){
                    for(j=0;j<NODES_PER_EDGE;j++){
                        fprintf(pFile,"%c",'A'+edges[i][j]);
                    }
                }
                fprintf(pFile,"\n");
            }
        };
        struct node_group_type{
            char groupid;
            char group_counter;
            char tedges;
            char eids[max_edges_per_node];
            bool operator<(const node_group_type& n)const{
                int i;
                if(tedges<n.tedges)return true;
                if(tedges>n.tedges)return false;
                for(i=0;i<tedges;i++){
                    if(eids[i]<n.eids[i])return true;
                    if(eids[i]>n.eids[i])return false;
                }
                return false;
            }
            bool operator==(const node_group_type& n)const{
                int i;
                if(tedges<n.tedges)return false;
                if(tedges>n.tedges)return false;
                for(i=0;i<tedges;i++){
                    if(eids[i]<n.eids[i])return false;
                    if(eids[i]>n.eids[i])return false;
                }
                return true;
            }
            void show()const{
                int i;
                printf("%d*{ ",group_counter);
                if(eids[0]==-1){
                    printf("D=%d ",tedges);
                }else{
                    for(i=0;i<tedges;i++){
                        printf("%d ",eids[i]);
                    }
                }
                printf("}");
            }
        };
        struct edge_group_type{
            char groupid;
            char group_counter;
            char nids[NODES_PER_EDGE];
            bool operator<(const edge_group_type& e)const{
                int i;
                for(i=0;i<NODES_PER_EDGE;i++){
                    if(nids[i]<e.nids[i])return true;
                    if(nids[i]>e.nids[i])return false;
                }
                return false;
            }
            bool operator==(const edge_group_type& e)const{
                int i;
                for(i=0;i<NODES_PER_EDGE;i++){
                    if(nids[i]<e.nids[i])return false;
                    if(nids[i]>e.nids[i])return false;
                }
                return true;
            }
            void show()const{
                int i;
                printf("%d*{ ",group_counter);
                for(i=0;i<NODES_PER_EDGE;i++)printf("%d ",nids[i]);
                printf("}");
            }
        };
    private:
        int  num_of_edges;///Number of total edges. The number of total nodes is TNODES
        char edges_info[max_total_edges][NODES_PER_EDGE];///nodes in each edge.
        char nodes_info[TNODES][max_edges_per_node];///edges in each node
        char nodes_degree[TNODES];///number of edges in each node
        class group_infos{
            private:
                int  edge_groups_count;///number of edge groups
                int  node_groups_count;///number of node groups
                node_group_type node_groups[TNODES];///the info of each group of node
                edge_group_type edge_groups[max_total_edges];///the info of each group of edge
                char node_group_id[TNODES];///The node group id of a node
                char edge_group_id[max_total_edges];///The edge group id of an edge
                node_edge_set *node_edges;
            public:
                void show_edges()const{
                    int i;
                    for(i=0;i<edge_groups_count;i++){
                        edge_groups[i].show();
                    }
                    printf("\n");
                    printf("id:{ ");
                    for(i=0;i<node_edges->num_of_edges;i++){
                        printf("%d ",edge_group_id[i]);
                    }
                    printf("}\n");
                }
                bool simple_case(int node_id)const{
                     int i,j;
                    if(node_edges->nodes_degree[node_id]==0)
                        return true;
                    if(node_edges->nodes_degree[node_id]>1)
                        return false;
                    int ngid=node_group_id[node_id];
                    int e=node_edges->nodes_info[node_id][0];
                    for(j=0;j<NODES_PER_EDGE;j++){
                        int n=node_edges->edges_info[e][j];
                        if(node_group_id[n]==ngid)continue;
                        if(node_groups[node_group_id[n]].group_counter>1)
                            break;
                    }
                    return j==NODES_PER_EDGE;
                }
                void show_nodes()const{
                    int i;
                    for(i=0;i<node_groups_count;i++){
                        node_groups[i].show();
                    }
                    printf("\n");
                    printf("id:{ ");
                    for(i=0;i<TNODES;i++){
                        printf("%d ",node_group_id[i]);
                    }
                    printf("}\n");
                }
                group_infos(node_edge_set *ne):node_edges(ne){}
                group_infos(const group_infos& g){
                    int i;
                    node_edges=g.node_edges;
                    edge_groups_count=g.edge_groups_count;
                    node_groups_count=g.node_groups_count;
                    for(i=0;i<node_groups_count;i++)node_groups[i]=g.node_groups[i];
                    for(i=0;i<edge_groups_count;i++)edge_groups[i]=g.edge_groups[i];
                    for(i=0;i<TNODES;i++)node_group_id[i]=g.node_group_id[i];
                    for(i=0;i<node_edges->num_of_edges;i++)edge_group_id[i]=g.edge_group_id[i];
                }
                int get_edge_groups_count()const{return edge_groups_count;}
                int get_node_groups_count()const{return node_groups_count;}
                int get_node_group_id(int nid)const{
                    return node_group_id[nid];
                }
                int get_edge_group_id(int eid)const{
                    return edge_group_id[eid];
                }
                int get_edge_group_counter(int gid)const{
                    return edge_groups[gid].group_counter;
                }
                void assign_node_edge_id(char new_node_id[TNODES], int& next_node_id){
                    int i;
                    for(i=0;i<TNODES;i++){
                        if(new_node_id[i]==-1){
                            if(node_groups[node_group_id[i]].group_counter==1){
                                new_node_id[i]=next_node_id++;
                            }
                        }
                    }
                }
                int undetermined_node_group()const{
                    int i;
                    int smaller=TNODES+1;
                    int si=-1;
                    for(i=0;i<node_groups_count;i++){
                        if(node_groups[i].tedges>0&&node_groups[i].group_counter>1){
                            if(node_groups[i].group_counter<smaller){
                                smaller-node_groups[i].group_counter;
                                si=i;
                            }
                        }
                    }
                    return si;
                }
            private:
                void sort_node_group()///sort node_groups
                {
                    int i;
                    char tmp2[TNODES];
                    for(i=0;i<node_groups_count;i++)node_groups[i].groupid=i;
                    sort(&node_groups[0],&node_groups[node_groups_count]);
                    for(i=0;i<node_groups_count;i++)tmp2[node_groups[i].groupid]=i;
                    for(i=0;i<TNODES;i++)node_group_id[i]=tmp2[node_group_id[i]];
                }
                void sort_edge_group()///sort edge_groups
                {
                    int i;
                    char tmp2[max_total_edges];
                    for(i=0;i<edge_groups_count;i++)edge_groups[i].groupid=i;
                    sort(&edge_groups[0],&edge_groups[edge_groups_count]);
                    for(i=0;i<edge_groups_count;i++)tmp2[edge_groups[i].groupid]=i;
                    for(i=0;i<node_edges->num_of_edges;i++)edge_group_id[i]=tmp2[edge_group_id[i]];
                }
                void set_edge_group()///reset edge group according to node group
                {
                    int i,j;
                    edge_groups_count=0;
                    for(i=0;i<node_edges->num_of_edges;i++){
                        edge_group_type eg;
                        for(j=0;j<NODES_PER_EDGE;j++){
                            eg.nids[j]=node_group_id[node_edges->edges_info[i][j]];
                        }
                        sort(&eg.nids[0],&eg.nids[NODES_PER_EDGE]);
                        for(j=0;j<edge_groups_count;j++){
                            if(eg==edge_groups[j])
                                break;
                        }
                        if(j<edge_groups_count){///Matchs
                            edge_group_id[i]=j;
                            edge_groups[j].group_counter++;
                        }else{
                            edge_groups[edge_groups_count]=eg;
                            edge_groups[edge_groups_count].group_counter=1;
                            edge_group_id[i]=edge_groups_count++;
                        }
                    }
                    sort_edge_group();
                }
                void set_node_group()
                {
                    int i,j;
                    char old_id[TNODES];
                    for(i=0;i<TNODES;i++)old_id[i]=node_group_id[i];
                    node_groups_count=0;
                    for(i=0;i<TNODES;i++){
                        node_group_type ng;
                        ng.tedges=node_edges->nodes_degree[i];
                        for(j=0;j<ng.tedges;j++){
                            ng.eids[j]=edge_group_id[node_edges->nodes_info[i][j]];
                        }
                        sort(&ng.eids[0],&ng.eids[ng.tedges]);
                        for(j=0;j<node_groups_count;j++){
                            if(ng==node_groups[j]&&
                                    old_id[i]==node_groups[j].groupid)break;
                        }
                        if(j<node_groups_count){
                            node_group_id[i]=j;
                            node_groups[j].group_counter++;
                        }else{
                            node_groups[node_groups_count]=ng;
                            node_groups[node_groups_count].group_counter=1;
                            node_groups[node_groups_count].groupid = old_id[i];///save old id
                            node_group_id[i]=node_groups_count++;
                        }
                    }
                    sort_node_group();
                }
            public:
                void promote_searched(int searched){
                    int old_node_id[TNODES];
                    int node_id_map[TNODES];
                    int i;
                    if(searched<=0)return;
                    int gcount=0;
                    for(i=0;i<TNODES;i++){
                        old_node_id[i]=node_group_id[i];
                    }
                    sort(&old_node_id[0],&old_node_id[searched]);
                    sort(&old_node_id[searched],&old_node_id[TNODES]);
                    for(i=1;i<TNODES;i++){
                        if(old_node_id[i]!=old_node_id[i-1]){
                            old_node_id[++gcount]=old_node_id[i];
                        }
                    }
                    gcount++;
                    for(i=0;i<gcount;i++){
                        node_id_map[old_node_id[i]]=i;
                    }
                    for(i=0;i<gcount;i++)old_node_id[i]=-1;
                    for(i=0;i<gcount;i++){
                        if(old_node_id[i]==-1){///Order not changed.
                            node_group_type backupn=node_groups[i];
                            int start=i;
                            if(node_id_map[start]!=start){
                                do{
                                    int next=node_id_map[start];
                                    node_group_type t=node_groups[next];
                                    node_groups[next]=backupn;
                                    old_node_id[start]=1;
                                    backupn=t;
                                    start=next;
                                }while(start!=i);
                            }
                        }
                    }
                    for(i=0;i<TNODES;i++){
                        node_group_id[i]=node_id_map[node_group_id[i]];
                    }
                    set_edge_group();
                }
                void init_group_info(int searched){
                    int i,j;
                    node_groups_count=0;
                    ///first initialize node groups via degrees;
#ifdef OUTPUT
                    printf("start init group info\n");
#endif
                    for(i=0;i<searched;i++){
                        for(j=0;j<node_groups_count;j++){
                            if(node_groups[j].tedges==node_edges->nodes_degree[i])
                                break;
                        }
                        if(j<node_groups_count){
                            node_group_id[i]=j;
                            node_groups[j].group_counter++;
                        }else{
                            node_groups[node_groups_count].group_counter=1;
                            node_groups[node_groups_count].tedges=node_edges->nodes_degree[i];
                            node_groups[node_groups_count].eids[0]=-1;
                            node_group_id[i]=node_groups_count++;
                        }
                    }
                    int searched_groups=node_groups_count;
                    for(;i<TNODES;i++){
                        for(j=searched_groups;j<node_groups_count;j++){
                            if(node_groups[j].tedges==node_edges->nodes_degree[i])
                                break;
                        }
                        if(j<node_groups_count){
                            node_group_id[i]=j;
                            node_groups[j].group_counter++;
                        }else{
                            node_groups[node_groups_count].group_counter=1;
                            node_groups[node_groups_count].tedges=node_edges->nodes_degree[i];
                            node_groups[node_groups_count].eids[0]=-1;
                            node_group_id[i]=node_groups_count++;
                        }
                    }
                    sort_node_group();
#ifdef OUTPUT
                    printf("Init node info:\n");
                    show_nodes();
#endif

                    int old_node_group;

                    do{
                        old_node_group=node_groups_count;
                        set_edge_group();
#ifdef OUTPUT
                        printf("edge info:\n");
                        show_edges();
#endif
                        set_node_group();
#ifdef OUTPUT
                        printf("node info:\n");
                        show_nodes();
#endif
                    }while(node_groups_count!=old_node_group);
#ifdef OUTPUT
                    printf("end init\n");
#endif
                }
                bool unique_in_group(int nid){
                    int group=node_group_id[nid];
                    return (node_groups[group].group_counter==1);
                }
                void split_node(int nid){
                    int old_group=node_group_id[nid];
                    int i;
                    node_groups[old_group].group_counter--;
                    node_groups[node_groups_count].group_counter=1;
                    node_groups[node_groups_count].tedges=node_groups[old_group].tedges;
                    for(i=0;i<node_groups[old_group].tedges;i++){
                        node_groups[node_groups_count].eids[i]=node_groups[old_group].eids[i];
                    }
                    node_group_id[nid]=node_groups_count++;
                    int old_node_group;
                    do{
                        old_node_group=node_groups_count;
                        set_edge_group();
                        set_node_group();
                    }while(node_groups_count!=old_node_group);
#ifdef OUTPUT
                    printf("split node %d\n",nid);
                    printf("nodes:");
                    show_nodes();
                    set_edge_group();
                    printf("edges:");
                    show_edges();
#endif
                }
        };

        void add_result(final_edge_set& final_edges, const group_infos& g)
        {
            int edges=g.get_edge_groups_count();
            if(edges!=final_edges.num_edges){
                int i;
                for(i=0;i<final_edges.num_edges;i++){
                    printf("e[%d]=%d ",i,g.get_edge_group_id(i));
                }
                printf("\n");
                for(i=0;i<edges;i++){
                    printf("c[%d]=%d ",i,g.get_edge_group_counter(i));
                }
                printf("\n");
                for(i=0;i<TNODES;i++){
                    printf("n[%d]=%d ",i,g.get_node_group_id(i));
                }
                printf("\n");
                g.show_edges();
                throw exception();
            }
            int i;
            final_edge_set e;
            char rev[max_total_edges];
            for(i=0;i<edges;i++){
                rev[g.get_edge_group_id(i)]=i;
            }
            for(i=0;i<edges;i++){
                int j;
                for(j=0;j<NODES_PER_EDGE;j++){
                    e.edges[i][j]=g.get_node_group_id(edges_info[rev[i]][j]);
                }
                sort(&e.edges[i][0],&e.edges[i][NODES_PER_EDGE]);
            }
#ifdef OUTPUT
            printf("Find: ");
            e.show();
#endif
            if(e<final_edges)final_edges=e;
        }
        
        void find_all_edges(final_edge_set& final_edges,const group_infos& g, int searched)
        {
            int next_id = g.undetermined_node_group();
            if(next_id<0){///All identify is found
                group_infos ng(g);
                if(searched>0){
                    ng.promote_searched(searched);
                }
                add_result(final_edges,ng);
            }else{
                int i;
                for(i=0;i<TNODES;i++){
                    if(g.get_node_group_id(i)==next_id){
                        bool simple = g.simple_case(i);
                        group_infos ng(g);
                        ng.split_node(i);
                        find_all_edges(final_edges,ng,searched);
                        if(simple)
                            break;
                    }
                }
            }
        }
    public:
        node_edge_set(int nedges, char info[][NODES_PER_EDGE]){
            int i,j;
            num_of_edges=nedges;
            for(i=0;i<nedges;i++){
                for(j=0;j<NODES_PER_EDGE;j++){
                    edges_info[i][j]=info[i][j];
                }
            }
            for(i=0;i<TNODES;i++)nodes_degree[i]=0;
            for(i=0;i<num_of_edges;i++){
                for(j=0;j<NODES_PER_EDGE;j++){
                    char the_nid=info[i][j];
                    nodes_info[the_nid][nodes_degree[the_nid]++]=i;
                }
            }
        }


        void find_all_node_types( int searched, char group[TNODES]){
            int i,j;
            final_edge_set finals[TNODES];
            for(i=0;i<TNODES;i++){
                group[i]=-1;
                for(j=0;j<num_of_edges;j++){
                    int k;
                    for(k=0;k<NODES_PER_EDGE;k++){
                        finals[i].edges[j][k]=TNODES;
                    }
                }
            }
            group_infos g(this);
            g.init_group_info(searched);
            final_edge_set::set_curr_num_edges(num_of_edges);
            for(i=searched;i<TNODES;i++){
                if(g.unique_in_group(i))
                    continue;
                group_infos ng(g);
                ng.split_node(i);
                find_all_edges(finals[i],ng,searched);
            }
            for(i=searched;i<TNODES;i++){
                if(g.unique_in_group(i)){
                    group[i]=1;
                    continue;
                }
                for(j=searched;j<i;j++){
                    if(finals[j]==finals[i])break;
                }
                if(j==i){
                    group[i]=1;
                }
            }
        }

        final_edge_set normalize(int searched){
            group_infos g(this);
            g.init_group_info(searched);
            int i;
            final_edge_set::set_curr_num_edges(num_of_edges);
            final_edge_set final_edges;
            for(i=0;i<num_of_edges;i++){
                int j;
                for(j=0;j<NODES_PER_EDGE;j++)
                    final_edges.edges[i][j]=TNODES;//set it to maxiaml node
            }
            find_all_edges(final_edges, g,searched);
            return final_edges;
        }
};

template <int TNODES,int NODES_PER_EDGE>
int node_edge_set<TNODES,NODES_PER_EDGE>::final_edge_set::num_edges;
#if 0
#define NUM_EDGES 23
#define NUM_NODES 25
char init_value[NUM_EDGES][5]={
    "BIFS","BOLG","BMPH","BQNJ","EBCD",
    "AIET","EMOF","EQPS","EKNH","CKMA",
    "CQOT","CNPG","CJHS","DQMI","DNOA",
    "DPLF","DJGT","MRGS","NRFT","IORH",
    "JPRA","AFGH","KQRL"
};
char init_v2[NUM_EDGES][4];
typedef node_edge_set<NUM_NODES> NESET;

int main()
{
    int i,j;
    for(i=0;i<NUM_EDGES;i++)for(j=0;j<4;j++){
        init_v2[i][j]=init_value[i][j]-'A';
    }
    NESET nes(NUM_EDGES,init_v2);
    NESET::final_edge_set e=nes.normalize(4);
    printf("final result:\n");
    e.show();
    return 0;
}
#endif

#define NUM_NODES 15
#define NODES_PER_EDGE   4
#define MAX_FREEDOM      10

#define MAX_EDGES_PER_NODE  ((NUM_NODES-1)/(NODES_PER_EDGE-1))
#define MAX_TOTAL_EDGES     ((MAX_EDGES_PER_NODE*NUM_NODES)/NODES_PER_EDGE)
#define MAX_FREE_EDGES   (MAX_FREEDOM*NODES_PER_EDGE)

char chessboard[NUM_NODES][NUM_NODES];
char used[NUM_NODES];
char line_buf[MAX_TOTAL_EDGES][NODES_PER_EDGE];
int line_count;
int cur_free_edge;
int cur_max_degree;
typedef node_edge_set<NUM_NODES,NODES_PER_EDGE> NESET;

void reorder_node(int orig_node, int new_node)
{
    int i,j;
    char tmp;
    if(orig_node==new_node)return;
    for(i=0;i<NUM_NODES;i++){
        tmp=chessboard[orig_node][i];
        chessboard[orig_node][i]=chessboard[new_node][i];
        chessboard[new_node][i]=tmp;
    }
    for(i=0;i<NUM_NODES;i++){
        tmp=chessboard[i][orig_node];
        chessboard[i][orig_node]=chessboard[i][new_node];
        chessboard[i][new_node]=tmp;
    }
    tmp=used[orig_node];
    used[orig_node]=used[new_node];
    used[new_node]=tmp;
    for(i=0;i<line_count;i++){
        for(j=0;j<NODES_PER_EDGE;j++){
            if(line_buf[i][j]==orig_node){
                line_buf[i][j]=new_node;
            }else if(line_buf[i][j]==new_node){
                line_buf[i][j]=orig_node;
            }
        }
    }
}
int last_line_count;
NESET::final_edge_set last_e;
void output_cur(int step)
{
    int i;
    int changed=0;
    for(i=step;i<NUM_NODES;i++){
        if(used[i]>used[step-1])
            return;
    }
    if(last_line_count!=line_count){
        changed=1;
    }
    last_line_count=line_count;
    {
        NESET nes(line_count, line_buf);
        NESET::final_edge_set e=nes.normalize(step);
        if(!changed&&e==last_e)
            return;
        last_e=e;
        e.show(step);
    }
}

void init()
{
    int i,j;
    line_count=0;
    for(i=0;i<NUM_NODES;i++)for(j=0;j<NUM_NODES;j++){
        chessboard[i][j]=0;
    }
    for(i=0;i<NUM_NODES;i++)used[i]=0;
}

char test(int x,int y,int z, int w)
{
    if(chessboard[x][y]||chessboard[x][z]||
            chessboard[x][w]||chessboard[y][z]||
            chessboard[y][w]||chessboard[z][w])
        return 0;
    return 1;
}

void set_to(int x,int y, int z,int w, int value)
{
    chessboard[x][y]=chessboard[x][z]=chessboard[x][w]=
        chessboard[y][x]=chessboard[y][z]=chessboard[y][w]=
        chessboard[z][x]=chessboard[z][y]=chessboard[z][w]=
        chessboard[w][x]=chessboard[w][y]=chessboard[w][z]=value;
}

void setv(int x,int y,int z,int w)
{
    set_to(x,y,z,w,1);
    used[x]++;
    used[y]++;
    used[z]++;
    used[w]++;
    line_buf[line_count][0]=x;
    line_buf[line_count][1]=y;
    line_buf[line_count][2]=z;
    line_buf[line_count][3]=w;
    line_count++;
}

void unsetv(int x,int y,int z,int w)
{
    set_to(x,y,z,w,0);
    used[x]--;
    used[y]--;
    used[z]--;
    used[w]--;
    line_count--;
}

void create_step_one()
{
    char filename[100];
    mkdir("data/s1",-1);
    FILE *pFile=fopen("data/s1/0","w");
    int i,j;
    for(i=MAX_EDGES_PER_NODE;i>=1;i--){
        if((MAX_EDGES_PER_NODE-i)*NUM_NODES>MAX_FREE_EDGES)
            break;
        for(j=0;j<i;j++){
            fprintf(pFile,"A%c%c%c",j*3+'B',j*3+'C',j*3+'D');
        }
        fprintf(pFile,"\n");
    }
    fclose(pFile);
}

void search(int u)
{
    int v,w,z;
    if((MAX_EDGES_PER_NODE-used[u])*(NUM_NODES-u)<=cur_free_edge){
        output_cur(u+1);
    }
    if(used[u]>=cur_max_degree)return;
    for(v=u+1;v<NUM_NODES;v++){
        if(used[v]>=cur_max_degree)continue;
        if(chessboard[u][v])continue;
        for(w=v+1;w<NUM_NODES;w++){
            if(used[w]>=cur_max_degree)continue;
            if(chessboard[u][w]||chessboard[v][w])continue;
            for(z=w+1;z<NUM_NODES;z++){
                if(used[z]>=cur_max_degree)continue;
                if(chessboard[u][z]||chessboard[v][z]||chessboard[w][z])
                    continue;
                setv(u,v,w,z);
                search(u);
                unsetv(u,v,w,z);
            }
        }
    }
}

void process_one_line(int step, char *input)
{
    int i;
    init();
    for(i=0;;i++){
        char c=input[i*4];
        if(c>='A'&&c<='Z'){
            setv(input[i*4]-'A',input[i*4+1]-'A',input[i*4+2]-'A',input[i*4+3]-'A');
        }else{
            break;
        }
    }
    cur_free_edge=0;
    cur_max_degree=0;
    for(i=0;i<step-1;i++){
        cur_free_edge+=MAX_EDGES_PER_NODE - used[i];
        if(cur_max_degree<used[i]){
            cur_max_degree=used[i];
        }
    }
    for(;i<NUM_NODES;i++){
        if(used[i]>cur_max_degree)
            return;
    }
    cur_free_edge = MAX_FREE_EDGES - cur_free_edge;
    if(cur_free_edge<0){
        fprintf(stderr,"Warning, invalid input\n");
        return;
    }
    char group[NUM_NODES];
    for(i=0;i<NUM_NODES;i++)group[i]=1;
/*    if(step<(NUM_NODES-1)/3){
       NESET nes(line_count, line_buf);
       NESET::final_edge_set e=nes.normalize(step);
       nes.find_all_node_types(step-1,group);
    }*/

    for(i=step-1;i<NUM_NODES;i++){
       if(group[i]>0){
            reorder_node(i,step-1);
            search(step-1);
            reorder_node(i,step-1);
        }
    }
}

void sort_step(int step)
{
    char tname[100];
    char cmdline[200];
    sprintf(tname,"data/t%d",step);
    DIR *dirs=opendir(tname);
    if(dirs==NULL){
        fprintf(stderr,"Failed to open dir %s\n",tname);
        return;
    }
    struct dirent *flist;
    while(flist=readdir(dirs)){
        if(flist->d_name[0]<'0'||flist->d_name[0]>'9')
            continue;
        sprintf(cmdline,"sort -u data/t%d/%s -o data/s%d/%s",step,flist->d_name, step, flist->d_name);
        system(cmdline);
    }
    closedir(dirs);
    sprintf(cmdline,"rm -rf data/t%d",step);
    system(cmdline);
}

#define LINE_BUF_SIZE (MAX_TOTAL_EDGES*(NODES_PER_EDGE+1)+20)
void create_step(int step)
{
    char sname[100];
    char line[LINE_BUF_SIZE];
    FILE *pRead, *pWrite;
    sprintf(sname,"data/t%d",step);
    mkdir(sname,-1);
    sprintf(sname,"data/s%d",step);
    mkdir(sname,-1);
    sprintf(sname,"data/s%d",step-1);
    DIR *dirs=opendir(sname);
    if(dirs==NULL){
        fprintf(stderr,"Failed to open dir %s\n",sname);
        return;
    }
    struct dirent *flist;
    while(flist=readdir(dirs)){
        if(flist->d_name[0]<'0'||flist->d_name[0]>'9')
            continue;
        sprintf(sname,"data/s%d/%s",step-1,flist->d_name);
        pRead=fopen(sname,"r");
        if(pRead==NULL){
            fprintf(stderr,"Cannot read input file %s\n",sname);
            exit(-1);
        }
        while(fgets(line,LINE_BUF_SIZE,pRead)){
            process_one_line(step,line);
        }
        fclose(pRead);
    }
    closedir(dirs);
    sort_step(step);
}

int main(char argc, char *argv[])
{
    int step=atoi(argv[1]);
    if(step<1){
        fprintf(stderr,"step should be no less than 1\n");
        return -1;
    }
    if(step==1){
        mkdir("data",-1);
        create_step_one();
    }else{
        create_step(step);
    }
    return 0;
}
