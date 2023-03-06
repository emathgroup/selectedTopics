#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>
#include <vector>
#include <set>
#include <string>
#include <assert.h>

#ifndef NDEBUG
#define ASSERT(x) assert(x)
#else
#define ASSERT(x)
#endif

#ifndef FOLDER
#define FOLDER "/home/zdu"
#endif

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>
#include <ctype.h>
#include <algorithm>
using namespace std;
#define SEARCH 
template <int TNODES,int NODES_PER_EDGE=4>
class node_edge_set{
    public:
        static const int max_edges_per_node = (TNODES-1)/(NODES_PER_EDGE-1);
        static const int max_total_edges = (((TNODES-1)/(NODES_PER_EDGE-1))*TNODES)/NODES_PER_EDGE;
        struct final_edge_set{
            static int num_edges;
            char edges[max_total_edges][NODES_PER_EDGE];
            static void set_curr_num_edges(int n){num_edges=n;}
			void reverse(){
				int min_node_id=TNODES,max_node_id=-1;
				char tmp[NODES_PER_EDGE];
				int i,j;
				for(i=0;i<num_edges;i++){
					for(j=0;j<NODES_PER_EDGE;j++){
						if(edges[i][j]<min_node_id)min_node_id=edges[i][j];
						if(edges[i][j]>max_node_id)max_node_id=edges[i][j];
					}
				}
				for(i=0;i<num_edges/2;i++){
					for(j=0;j<NODES_PER_EDGE;j++){
						tmp[j]=edges[i][j];
					}
					for(j=0;j<NODES_PER_EDGE;j++){
						edges[i][j]=max_node_id-edges[num_edges-1-i][NODES_PER_EDGE-1-j];
					}
					for(j=0;j<NODES_PER_EDGE;j++){
						edges[num_edges-1-i][j]=max_node_id-tmp[NODES_PER_EDGE-1-j];
					}
				}
				if((num_edges&1)!=0){
					for(j=0;j<NODES_PER_EDGE;j++){
						tmp[j]=edges[i][j];
					}
					for(j=0;j<NODES_PER_EDGE;j++){
						edges[i][j]=max_node_id-tmp[NODES_PER_EDGE-1-j];
					}
				}
			}
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
//                    printf(" ");
                }
                printf("\n");
            }
	    void showmap(const char *nmap)const{
                int i,j;
                for(i=0;i<num_edges;i++){
                    for(j=0;j<NODES_PER_EDGE;j++){
                        printf("%c",'A'+nmap[edges[i][j]]);
                    }
//                    printf(" ");
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
            bool ssave()const{
                return save_item(edges,num_edges);
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
		char node_map[TNODES];//final node map
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
                    int j;
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
                                smaller=node_groups[i].group_counter;
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
				exit(-1);
            }
            int i;
            final_edge_set e;
            char rev[max_total_edges];
            for(i=0;i<edges;i++){
                rev[g.get_edge_group_id(i)]=i;
            }
			for(i=0;i<TNODES;i++)node_map[i]=-1;
            for(i=0;i<edges;i++){
                int j;
                for(j=0;j<NODES_PER_EDGE;j++){
                    e.edges[i][j]=g.get_node_group_id(edges_info[rev[i]][j]);
					node_map[e.edges[i][j]]=edges_info[rev[i]][j];
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

		const char *get_node_map()const{return node_map;}


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
			final_edges.reverse();
            return final_edges;
        }
};

template <int TNODES,int NODES_PER_EDGE>
int node_edge_set<TNODES,NODES_PER_EDGE>::final_edge_set::num_edges;
#define INIT_NODES 16
///change configure here to build different benchmark
const int MAX_NODE_NUM=20;
#define TARGET_NODES 20
const int MAX_EDGE_NUM=30;
const int NODES_PER_EDGE=4;
const int MAX_NUM_VAR = MAX_NODE_NUM*2;
const int MAX_QUAD_VAR = ((MAX_NUM_VAR+1)*MAX_NUM_VAR)/2;
const int MAX_EQUATS = MAX_EDGE_NUM*2;
const int MAX_NON_ZEROS = MAX_EDGE_NUM*MAX_NODE_NUM;
#define NUM_NODES MAX_NODE_NUM
#define MAX_TOTAL_EDGES MAX_EDGE_NUM
#define MAXLINE_COUNT MAX_EDGE_NUM
#define MAX_DEPTH 6

int lb[]={11,15,19,22,24};
int maxd[]={16,18,19,21,26};
int showd[]={14,14,16,18,22};

int lower_bound(int n){
    return lb[n-INIT_NODES];
}
int max_edges(int n){
    return maxd[n-INIT_NODES];
}

typedef node_edge_set<MAX_NODE_NUM> NESET;

const int P=2147483629;
signed extGCD(signed a, signed b, signed & ar, signed & br)
{
	signed x = 0, y = 1;
	ar = 1, br = 0;
	unsigned t, q;
	while (b != 0)
	{
		t = b; q = a / b; b = a % b; a = t;
		t = x; x = ar - q * x; ar = t;
		t = y; y = br - q * y; br = t;
	}
	return (a);
}

class fpn{
	unsigned v;
public:
	fpn(){v=0;}
	fpn(const fpn& n){v=n.v;}
	fpn(unsigned x){v=x%P;}
	fpn& operator+=(const fpn& n);
	fpn& operator-=(const fpn& n);
	fpn& operator*=(const fpn& n);
	fpn& operator/=(const fpn& n);
	fpn operator+(const fpn& n)const{fpn x(*this);return x+=n;}
	fpn operator-(const fpn& n)const{fpn x(*this);return x-=n;}
	fpn operator*(const fpn& n)const{fpn x(*this);return x*=n;}
	fpn operator/(const fpn& n)const{fpn x(*this);return x/=n;}
	bool isZero()const{return v==0;}
	bool isOne()const{return v==1;}
	bool operator==(const fpn& n)const;
	bool operator!=(const fpn& n)const;
	fpn operator++(int){fpn f(*this);*this+=fpn(1);return f;}
	fpn& operator++(){return *this+=fpn(1);}
	fpn operator--(int){fpn f(*this);*this-=fpn(1);return f;}
	fpn& operator--(){return *this-=fpn(1);}
	fpn inv()const;
	fpn neg()const{fpn n;if(v!=0)n.v=P-v;else n.v=0;return n;}
	fpn& operator=(const fpn& n){v=n.v;return *this;}
	int  getsv()const{int x=v;if(x<P/2)return x;else return -(P-x);}
};

fpn& fpn::operator+=(const fpn& n)
{
	if(v>=P-n.v){
		v=v+n.v-P;
	}else{
		v+=n.v;
	}
	return *this;
}

fpn& fpn::operator-=(const fpn& n)
{
	if(v>=n.v)
		v-=n.v;
	else
		v=v+P-n.v;
	return *this;
}

fpn& fpn::operator*=(const fpn& n)
{
	unsigned long long t = (unsigned long long)v * (unsigned long long) n.v;
	t%=P;
	v=(unsigned)t;
	return *this;
}

fpn& fpn::operator/=(const fpn& n)
{
	return (*this) *= n.inv();
}

fpn fpn::inv()const
{
	signed ar,br;
	extGCD(v, P, ar, br);
	if(ar<0)ar+=P;
	return ar;
}

bool fpn::operator==(const fpn& n)const
{
	return v==n.v;
}

bool fpn::operator!=(const fpn& n)const
{
	return v!=n.v;
}


class msolver{
	short _m; //number of equations;
    short _el;
	short _nodes;//number of nodes used
	char _n_infx,_n_infy,_n_infp1,_n_infp2;
	char _v;//current variable count
	bool _valid;///set to false when failed
	bool _changed;
	char sname[MAX_QUAD_VAR][2];
	fpn eq_rows[MAX_EQUATS][MAX_QUAD_VAR];
	fpn nzl_rows[MAX_EQUATS][MAX_NUM_VAR];
    short nzl_evar[MAX_EQUATS];
	//variable 0 reserved for const
	fpn coord_rows[MAX_NODE_NUM*2+2][MAX_NUM_VAR];
	short col_index[MAX_QUAD_VAR];
	short _lcol,_ecol,_tcol;
	short _lastr;
protected:
	void clear(){
		int i,j;
		_nodes=0;
		_m=_el=0;
		_valid=true;
		for(i=0;i<MAX_EQUATS;i++)for(j=0;j<MAX_QUAD_VAR;j++)eq_rows[i][j]=0;
		for(i=0;i<MAX_EQUATS;i++)for(j=0;j<MAX_NUM_VAR;j++)nzl_rows[i][j]=0;
		for(i=0;i<MAX_NODE_NUM*2+2;i++)for(j=0;j<MAX_NUM_VAR;j++)coord_rows[i][j]=0;
	}
	void init(char line_buf[MAX_EDGE_NUM][NODES_PER_EDGE], int line_count, char node_degrees[MAX_NODE_NUM],int infty_edge);
	void show_linear_row(FILE *f, const fpn *row)const{
		int i;
		bool nz=false;
		for(i=0;i<_v;i++){
			if(row[i]!=0){
				int v=row[i].getsv();
				if(i==0){
					fprintf(f,"%+d",v);
				}else{
					fprintf(f,"%+d*V%d",v,i);
				}
				nz=true;
			}
		}
		if(!nz){
			fprintf(f,"0");
		}
	}
	void show_quad_row(FILE *f,const fpn *row)const{
		int i;
		bool nz=false;
		for(i=0;i<_v*(_v+1)/2;i++){
			if(row[i]!=0){
				int v=row[i].getsv();
				if(i==0){
					fprintf(f,"%+d",v);
				}else if(sname[i][0]==0){
					fprintf(f,"%+d*V%d",v,sname[i][1]);
				}else if(sname[i][0]==sname[i][1]){
					fprintf(f,"%+d*V%d^2",v,sname[i][1]);
				}else{
					fprintf(f,"%+d*V%d*V%d",v,sname[i][0],sname[i][1]);
				}
				nz=true;
			}
		}
		if(!nz){
			fprintf(f,"0");
		}
	}
	static int get_const_var_id(){return 0;}
	static int get_quad_col(int v1,int v2){
		if(v2<v1){
			int t=v1;v1=v2;v2=t;
		}
		return (v2*(v2+1))/2+v1;
	}

	bool is_infty_node(int n)const{return n==_n_infx||n==_n_infy||n==_n_infp1||n==_n_infp2;}

	const fpn *getSlope1Coord()const{return coord_rows[0];}
	const fpn *getSlope2Coord()const{return coord_rows[1];}
	const fpn *getXCoord(int node)const{return coord_rows[2+node*2];}
	const fpn *getYCoord(int node)const{return coord_rows[3+node*2];}
	fpn *getSlope1Coord(){return coord_rows[0];}
	fpn *getSlope2Coord(){return coord_rows[1];}
	fpn *getXCoord(int node){return coord_rows[2+node*2];}
	fpn *getYCoord(int node){return coord_rows[3+node*2];}

	void start_one_line(){
		int i;
		for(i=0;i<_v*(_v+1)/2;i++){
			assert(eq_rows[_m][i]==0);
		}
	}
	void end_one_line(){_m++;}
	void add_x_line(char n1,char n2);
	void add_y_line(char n1,char n2);
	void add_p1_line(char n1,char n2);
	void add_p2_line(char n1,char n2);
	void add_limit_line(char n1, char n2, char n3);
	void add_p1x(char n);
	void sub_p1x(char n);
	void add_p2x(char n);
	void sub_p2x(char n);
	void add_x(char n);
	void sub_x(char n);
	void add_y(char n);
	void sub_y(char n);
	void add_xy(char nx,char ny);
	void sub_xy(char nx,char ny);

	void remove_empty(){
		int i,j;
		for(i=0;i<_m;i++){
			int zc=0;
			for(j=0;j<_v*(_v+1)/2;j++){
				if(eq_rows[i][j]!=0){
					zc++;
					if(zc>=2)break;
				}
			}
			if(zc==1){
				_valid=false;
				return;
			}
			if(zc==0){
				swap_row(i,_m-1);
				_m--;
				i--;
			}
		}
	}

	void replace_linear_row(fpn *rows, int nzl_rid){
		int j;
		fpn t = rows[_v-1];
		rows[_v-1] = 0;
		if(t!=0)
			for(j=0;j<_v;j++){
				rows[j]+=nzl_rows[nzl_rid][j]*t;
			}
	}

	void replace_quad_row(fpn *rows,int nzl_rid){
		int j,k;
		for(j=0;j<_v-1;j++){
			int c = get_quad_col(j,_v-1);
			fpn t = rows[c];
			if(t==0)continue;
			rows[c]=0;
			for(k=0;k<_v;k++){
				int tc = get_quad_col(j,k);
				rows[tc]+=t*nzl_rows[nzl_rid][k];
			}
		}
		int c = get_quad_col(_v-1,_v-1);
		fpn t = rows[c];
		rows[c]=0;
		if(t!=0)
			for(j=0;j<_v;j++){
				if(nzl_rows[nzl_rid][j]==0)continue;
				for(k=0;k<_v;k++){
					int tc = get_quad_col(j,k);
					rows[tc]+=t*nzl_rows[nzl_rid][j]*nzl_rows[nzl_rid][k];
				}
			}
	}
	void replace_one_var(int nzl_rid,int vid){
        int i;
		swap_var(vid,_v-1);///now eliminate _v-1 by nzl_rid
        if(vid!=_v-1){
            for(i=0;i<_el;i++){
                if(nzl_evar[i]==_v-1)nzl_evar[i]=vid;
            }
        }
		for(i=0;i<_m;i++){
			replace_quad_row(eq_rows[i],nzl_rid);
		}
		for(i=0;i<_nodes*2+2;i++){
			replace_linear_row(coord_rows[i],nzl_rid);
		}
		for(i=0;i<_el;i++){
			replace_linear_row(nzl_rows[i],nzl_rid);
		}
		_v--;
	}

	void swap_var(int v1,int v2){
		if(v1==v2)return;
		int i,j;
		for(i=0;i<_v;i++){
			if(i!=v1&&i!=v2){
				int c1=get_quad_col(i,v1);
				int c2=get_quad_col(i,v2);
				swap_col(c1,c2);
				for(j=0;j<_m;j++){
					fpn tmp = eq_rows[j][c1];
					eq_rows[j][c1]=eq_rows[j][c2];
					eq_rows[j][c2]=tmp;
				}
			}
		}
		int c1=get_quad_col(v1,v1);
		int c2=get_quad_col(v2,v2);
		swap_col(c1,c2);
		for(j=0;j<_m;j++){
			fpn tmp=eq_rows[j][c1];
			eq_rows[j][c1]=eq_rows[j][c2];
			eq_rows[j][c2]=tmp;
		}
		for(j=0;j<_el;j++){
			fpn tmp = nzl_rows[j][v1];
			nzl_rows[j][v1] = nzl_rows[j][v2];
			nzl_rows[j][v2] = tmp;
		}
		for(j=0;j<_nodes*2+2;j++){
			fpn tmp = coord_rows[j][v1];
			coord_rows[j][v1] = coord_rows[j][v2];
			coord_rows[j][v2] = tmp;
		}
	}
	void init_indexes(){
		int i;
		for(i=0;i<(_v*(_v+1))/2;i++){
			col_index[i]=i;
		}
	}

	void swap_col(int c1,int c2){
		short t=col_index[c1];col_index[c1]=col_index[c2];col_index[c2]=t;
	}
	void swap_row(int r1,int r2){
		if(r1==r2)return;
		int i;
		for(i=0;i<(_v*(_v+1))/2;i++){
			fpn tmp=eq_rows[r1][i];
			eq_rows[r1][i]=eq_rows[r2][i];
			eq_rows[r2][i]=tmp;
		}
	}
	void swap_nzl_row(int r1, int r2){
		if(r1==r2)return;
		int i;
		for(i=0;i<_v;i++){
			fpn tmp=nzl_rows[r1][i];
			nzl_rows[r1][i]=nzl_rows[r2][i];
			nzl_rows[r2][i]=tmp;
		}
	}

	void linear_simplify(int vid){
		int i,j,k;
		int s=_lastr;
		int cc=get_quad_col(0,vid);
        int ecol=_tcol;
		for(j=_lcol;j<ecol;j++){
			int c=col_index[j];
			if(c==cc)continue;
			for(i=s;i<_m;i++){
				if(eq_rows[i][c]!=0)
					break;
			}
			if(i==_m){
				swap_col(j,ecol-1);
				ecol--;
                j--;
				continue;
			}
			if(i!=s)swap_row(i,s);
			fpn n=eq_rows[s][c].inv();
			for(i=_lcol;i<_tcol;i++){
				int c2=col_index[i];
				eq_rows[s][c2]*=n;
			}
			for(i=_lastr;i<_m;i++){
				if(i==s)continue;
				if(eq_rows[i][c]==0)continue;
				fpn n=eq_rows[i][c];
				for(k=_lcol;k<_tcol;k++){
					int c2=col_index[k];
					eq_rows[i][c2]-=eq_rows[s][c2]*n;
				}
			}
			s++;
		}
		_m=s;
        s=_lastr;
        _el = 0;
        for(j=_lcol;j<ecol;j++){
            int c=col_index[j];
            if(c==cc)continue;
            short evarid=sname[c][0];
            if(evarid==vid)evarid=sname[c][1];
            nzl_evar[_el]=evarid;
            assert(eq_rows[s][c]==1);
            for(i=0;i<_v;i++){
                assert(nzl_rows[_el][i]==0);
            }
			eq_rows[s][c]=0;
            for(i=_lcol;i<_tcol;i++){
                int c2=col_index[i];
                if(c2!=c){
                    int lvid=sname[c2][0];
                    if(lvid==vid)lvid=sname[c2][1];
                    nzl_rows[_el][lvid]=eq_rows[s][c2].neg();
                }
                eq_rows[s][c2]=0;
            }
            _el++;s++;
        }
        _m=_lastr;
	}

	void linear_replace(int vid){
		int i,j;
		linear_simplify(vid);
        for(i=0;i<_el;i++){
            replace_one_var(i,nzl_evar[i]);
		}
		for(i=0;i<_el;i++){
			for(j=0;j<_v;j++){
				nzl_rows[i][j]=0;
			}
		}
		_el=0;
	}

	void init_col_by_var(int vid){
		short vlist[MAX_QUAD_VAR];
		int i,j;
		init_indexes();
		for(i=0;i<(_v*(_v+1))/2;i++)vlist[i]=1;
		for(i=0;i<_v;i++){
			short c=get_quad_col(vid,i);
			vlist[c]=0;
		}
		for(i=0,j=(_v*(_v+1))/2-1;i<j;i++){
			if(vlist[i]==0){
				while(j>i&&vlist[j]==0)j--;
				if(j>i){
					swap_col(i,j);
					j--;
				}
			}
		}
		_lcol = _ecol = (_v*(_v-1))/2;
		_tcol = (_v*(_v+1))/2;
	}

	bool reduce_eq(){
		int i,j,t;
		for(i=0;i<_ecol;i++){
			if(i>=_m)break;
			int c=col_index[i];
			for(t=i;t<_m;t++){
				if(eq_rows[t][c]!=0)
					break;
			}
			if(t>=_m){
				swap_col(i,_ecol-1);
				_ecol--;
				i--;
				continue;
			}

			if(t!=i)swap_row(t,i);
			fpn tmp=eq_rows[i][c].inv();
			for(j=i;j<_tcol;j++){
				eq_rows[i][col_index[j]]*=tmp;
			}
			for(t=i+1;t<_m;t++){
				tmp=eq_rows[t][c];
				int nzcount=0;
				for(j=i;j<_tcol;j++){
					eq_rows[t][col_index[j]]-=tmp*eq_rows[i][col_index[j]];
					if(eq_rows[t][col_index[j]]!=0){
						nzcount++;
					}
				}
				if(nzcount==0){///empty row found
					swap_row(t,_m-1);
					--_m;
					t--;
					continue;
				}else if(nzcount==1){
					_valid=false;
					return false;///no solution
				}
			}
		}

		if(i<_m){
			_lastr = i;
			for(;i<_m;i++){
				int nzcount=0;
				for(j=_lcol;j<_tcol;j++){
					if(eq_rows[i][col_index[j]]!=0){
						nzcount++;
						if(nzcount>=2)break;
					}
				}
				if(nzcount==1){
					_valid=false;
					return false;
				}else if(nzcount==0){
					swap_row(i,_m-1);
					--_m;
					--i;
				}
			}
			if(_lastr<_m)return true;
		}

		return false;
	}

	void transform_one_var(int vid){
		init_col_by_var(vid);
		if(reduce_eq()){
			linear_replace(vid);
		}

		if(_valid){
			remove_empty();
		}
	}

public:
    int  get_score()const{return _v*10-_m;}
	void add_line(char n1, char n2, char n3);
	void add_line(char n1, char n2, char n3, char n4);
	void add_node();
	int  nodeCount()const{return _nodes;}

	void stddump()const{dump(stdout);}
	void errdump()const{dump(stderr);}
	void dump(FILE *f)const{
		fprintf(f,"Start DUMP:\nNodes:%d\n",_nodes);
		fprintf(f,"Infty Nodes:<X:%d,Y:%d, S1:%d,S2:%d>\n",_n_infx,_n_infy,_n_infp1,_n_infp2);
		fprintf(f,"Coordinate list:\n");
		fprintf(f,"slope 1: ");
		show_linear_row(f,getSlope1Coord());
		fprintf(f,"\nslope 2: ");
		show_linear_row(f,getSlope2Coord());
		int i;
		for(i=0;i<_nodes;i++){
			if(!is_infty_node(i)){
				fprintf(f,"\nX_%d: ",i);
				show_linear_row(f,getXCoord(i));
				fprintf(f,"\nY_%d: ",i);
				show_linear_row(f,getYCoord(i));
			}
		}
		fprintf(f,"\nEquations:");
		for(i=0;i<_m;i++){
			fprintf(f,"\n");
			show_quad_row(f,eq_rows[i]);
		}
		fprintf(f,"\nEnd DUMP\n");
	}

	void simple_reduce(){
		int i;
		_changed=false;
		bool changed;
		do{
			changed=false;
			for(i=0;i<_v;i++){
				int oldv=_v;
				transform_one_var(i);
#ifdef DUMP_OUTPUT
				fprintf(stderr,"simple dump v=%d\n",i);
				errdump();
#endif
				if(!_valid)
					break;
				if(_v!=oldv){
					changed=true;
				}
			}
		}while(changed);
	}

	void reduce(){simple_reduce();}

	msolver(){_nodes=0;}
	msolver(char line_buf[MAX_EDGE_NUM][NODES_PER_EDGE], int edges);
	void reset(char line_buf[MAX_EDGE_NUM][NODES_PER_EDGE], int edges, char degrees[MAX_NODE_NUM], int infty_edge){
		clear();
		init(line_buf,edges,degrees,infty_edge);
	}

        void reset(){
            clear();
        }

        bool isEmpty()const{return _nodes==0;}

	void addLastEdge(char line_buf[MAX_EDGE_NUM][NODES_PER_EDGE], int edges);

	msolver& operator=(const msolver& ms);

	bool isValid()const{return _valid;}
};

msolver& msolver::operator=(const msolver& ms)
{
	int i,j;
	clear();
	_m=ms._m;
	_el=ms._el;
	_nodes=ms._nodes;
	_n_infx=ms._n_infx;
	_n_infy=ms._n_infy;
	_n_infp1=ms._n_infp1;
	_n_infp2=ms._n_infp2;
	_v=ms._v;
	_valid=ms._valid;
	for(i=0;i<_v*(_v+1)/2;i++){
		sname[i][0]=ms.sname[i][0];
		sname[i][1]=ms.sname[i][1];
	}
	for(i=0;i<_m;i++)for(j=0;j<_v*(_v+1)/2;j++){
		eq_rows[i][j]=ms.eq_rows[i][j];
	}
	for(i=0;i<_el;i++)for(j=0;j<_v;j++){
		nzl_rows[i][j]=ms.nzl_rows[i][j];
	}
	for(i=0;i<_nodes*2+2;i++)for(j=0;j<_v;j++){
		coord_rows[i][j]=ms.coord_rows[i][j];
	}
	return *this;
}

void msolver::addLastEdge(char line_buf[][NODES_PER_EDGE], int edges)
{
	int i;
	this->add_line(line_buf[edges-1][0],line_buf[edges-1][1],line_buf[edges-1][2],line_buf[edges-1][3]);
}

void msolver::init(char line_buf[MAX_EDGE_NUM][NODES_PER_EDGE], int edges,char degrees[MAX_NODE_NUM], int infty_edge)
{
	int best_edge=infty_edge;
	int max_l_degree=-1;
	int max_node_id=-1;
	int i,j;
	for(i=0;i<edges;i++){
		for(j=0;j<NODES_PER_EDGE;j++){
			if(line_buf[i][j]>max_node_id)
				max_node_id=line_buf[i][j];
		}
	}

	int inft_nodes[NODES_PER_EDGE];
	for(i=0;i<NODES_PER_EDGE;i++)
		inft_nodes[i]=line_buf[best_edge][i];
	for(i=0;i<NODES_PER_EDGE;i++){
		for(j=i+1;j<NODES_PER_EDGE;j++){
			if(degrees[inft_nodes[i]]<degrees[inft_nodes[j]]){
				int t=inft_nodes[i];
				inft_nodes[i]=inft_nodes[j];
				inft_nodes[j]=t;
			}
		}
	}

	assert(max_node_id<MAX_NODE_NUM);
	_nodes = max_node_id + 1;
	_n_infx = inft_nodes[0];
	_n_infy = inft_nodes[1];
	_n_infp1 = inft_nodes[2];
	_n_infp2 = inft_nodes[3];

    int fx=-1,sx=-1;
    int fy=-1;
    for(i=0;i<edges;i++){
        if(i==best_edge)continue;
        for(j=0;j<NODES_PER_EDGE;j++){
            if(line_buf[i][j]==_n_infx){
                if(fx==-1)
                    fx=i;
                else if(sx==-1)
                    sx=i;
            }else if(line_buf[i][j]==_n_infy){
                if(fy==-1)
                    fy=i;
            }
        }
    }

	_m = 0;
	_el = 0;
	_valid = true;
	_changed = false;

	getSlope1Coord()[0]=1;///slope 1 is constant
	getSlope2Coord()[1]=1;///slope 2 use the first variable
	_v=2;
	for(i=0;i<_nodes;i++){
		if(!is_infty_node(i)){
            int matchx=-1,matchy=-1;
            if(fx!=-1){
                for(j=0;j<NODES_PER_EDGE;j++){
                    if(line_buf[fx][j]==i){
                        matchx=0;
                        break;
                    }
                }
            }
            if(matchx==-1&&sx!=-1){
                for(j=0;j<NODES_PER_EDGE;j++){
                    if(line_buf[sx][j]==i){
                        matchx=1;
                        break;
                    }
                }
            }
            if(fy!=-1){
                for(j=0;j<NODES_PER_EDGE;j++){
                    if(line_buf[fy][j]==i){
                        matchy=0;
                        break;
                    }
                }
            }
            if(matchy==-1){
			    getXCoord(i)[_v++]=1;
            }else{
                getXCoord(i)[0]=matchy;
            }
            if(matchx==-1){
			    getYCoord(i)[_v++]=1;
            }else{
                getYCoord(i)[0]=matchx;
            }
		}
	}
	for(i=0;i<_v;i++){
		for(j=i;j<_v;j++){
			int c=get_quad_col(i,j);
			sname[c][0]=i;
			sname[c][1]=j;
		}
	}
	for(i=0;i<edges;i++){
		if(i==best_edge)continue;
		add_line(line_buf[i][0],line_buf[i][1],line_buf[i][2],line_buf[i][3]);
	}
}

void msolver::add_node()
{
	int i,j;
	for(i=0;i<_v;i++){
		for(j=_v;j<_v+2;j++){
			int c=get_quad_col(i,j);
			sname[c][0]=i;
			sname[c][1]=j;
		}
	}
	int c=get_quad_col(_v,_v+1);
	sname[c][0]=_v;
	sname[c][1]=_v+1;
	getXCoord(_nodes)[_v++]=1;
	getYCoord(_nodes)[_v++]=1;
	i=_nodes++;
}

msolver::msolver(char line_buf[MAX_EDGE_NUM][NODES_PER_EDGE], int edges)
{
	char degrees[MAX_NODE_NUM];
	char ldegrees[MAX_EDGE_NUM];
	int best_edge=-1;
	int max_l_degree=-1;
	int max_node_id=-1;
	int i,j;
	for(i=0;i<MAX_NODE_NUM;i++)degrees[i]=0;
	for(i=0;i<MAX_EDGE_NUM;i++)ldegrees[i]=0;
	for(i=0;i<edges;i++){
		for(j=0;j<NODES_PER_EDGE;j++){
			if(line_buf[i][j]>max_node_id)
				max_node_id=line_buf[i][j];
			degrees[line_buf[i][j]]++;
		}
	}
	for(i=0;i<edges;i++){
		int s=0;
		for(j=0;j<NODES_PER_EDGE;j++){
			s+=degrees[line_buf[i][j]];
		}
		ldegrees[i]=s;
		if(s>=max_l_degree){
			max_l_degree=s;
			best_edge=i;
		}
	}

	int inft_nodes[NODES_PER_EDGE];
	for(i=0;i<NODES_PER_EDGE;i++)
		inft_nodes[i]=line_buf[best_edge][i];
	for(i=0;i<NODES_PER_EDGE;i++){
		for(j=i+1;j<NODES_PER_EDGE;j++){
			if(degrees[inft_nodes[i]]<degrees[inft_nodes[j]]){
				int t=inft_nodes[i];
				inft_nodes[i]=inft_nodes[j];
				inft_nodes[j]=t;
			}
		}
	}

	assert(max_node_id<MAX_NODE_NUM);
	_nodes = max_node_id + 1;
	_n_infx = inft_nodes[0];
	_n_infy = inft_nodes[1];
	_n_infp1 = inft_nodes[2];
	_n_infp2 = inft_nodes[3];

    int fx=-1,sx=-1;
    int fy=-1;
    for(i=0;i<edges;i++){
        if(i==best_edge)continue;
        for(j=0;j<NODES_PER_EDGE;j++){
            if(line_buf[i][j]==_n_infx){
                if(fx==-1)
                    fx=i;
                else if(sx==-1)
                    sx=i;
            }else if(line_buf[i][j]==_n_infy){
                if(fy==-1)
                    fy=i;
            }
        }
    }

	_m = 0;
	_el = 0;
	_valid = true;
	_changed = false;

	getSlope1Coord()[0]=1;///slope 1 is constant
	getSlope2Coord()[1]=1;///slope 2 use the first variable
	_v=2;
	for(i=0;i<_nodes;i++){
		if(!is_infty_node(i)){
            int matchx=-1,matchy=-1;
            if(fx!=-1){
                for(j=0;j<NODES_PER_EDGE;j++){
                    if(line_buf[fx][j]==i){
                        matchx=0;
                        break;
                    }
                }
            }
            if(matchx==-1&&sx!=-1){
                for(j=0;j<NODES_PER_EDGE;j++){
                    if(line_buf[sx][j]==i){
                        matchx=1;
                        break;
                    }
                }
            }
            if(fy!=-1){
                for(j=0;j<NODES_PER_EDGE;j++){
                    if(line_buf[fy][j]==i){
                        matchy=0;
                        break;
                    }
                }
            }
            if(matchy==-1){
			    getXCoord(i)[_v++]=1;
            }else{
                getXCoord(i)[0]=matchy;
            }
            if(matchx==-1){
			    getYCoord(i)[_v++]=1;
            }else{
                getYCoord(i)[0]=matchx;
            }
		}
	}
	for(i=0;i<_v;i++){
		for(j=i;j<_v;j++){
			int c=get_quad_col(i,j);
			sname[c][0]=i;
			sname[c][1]=j;
		}
	}
	for(i=0;i<edges;i++){
		if(i==best_edge)continue;
		add_line(line_buf[i][0],line_buf[i][1],line_buf[i][2],line_buf[i][3]);
	}
}

void msolver::add_x(char n)
{
	const fpn *c = getXCoord(n);
	int i;
	for(i=0;i<_v;i++){
		if(c[i]!=0){
			int col = get_quad_col(0,i);
			eq_rows[_m][col]+=c[i];
		}
	}
}

void msolver::sub_x(char n)
{
	const fpn *c = getXCoord(n);
	int i;
	for(i=0;i<_v;i++){
		if(c[i]!=0){
			int col = get_quad_col(0,i);
			eq_rows[_m][col]-=c[i];
		}
	}
}

void msolver::add_y(char n)
{
	const fpn *c = getYCoord(n);
	int i;
	for(i=0;i<_v;i++){
		if(c[i]!=0){
			int col = get_quad_col(0,i);
			eq_rows[_m][col]+=c[i];
		}
	}
}

void msolver::sub_y(char n)
{
	const fpn *c = getYCoord(n);
	int i;
	for(i=0;i<_v;i++){
		if(c[i]!=0){
			int col = get_quad_col(0,i);
			eq_rows[_m][col]-=c[i];
		}
	}
}

void msolver::add_xy(char nx,char ny)
{
	const fpn *cx = getXCoord(nx);
	const fpn *cy = getYCoord(ny);
	int i,j;
	for(i=0;i<_v;i++)if(cx[i]!=0){
		for(j=0;j<_v;j++)if(cy[j]!=0){
			int col = get_quad_col(i,j);
			eq_rows[_m][col]+=cx[i]*cy[j];
		}
	}
}

void msolver::sub_xy(char nx,char ny)
{
	const fpn *cx = getXCoord(nx);
	const fpn *cy = getYCoord(ny);
	int i,j;
	for(i=0;i<_v;i++)if(cx[i]!=0){
		for(j=0;j<_v;j++)if(cy[j]!=0){
			int col = get_quad_col(i,j);
			eq_rows[_m][col]-=cx[i]*cy[j];
		}
	}
}

void msolver::add_p1x(char n)
{
	const fpn *c = getXCoord(n);
	const fpn *cp = getSlope1Coord();
	int i,j;
	for(i=0;i<_v;i++)if(c[i]!=0){
		for(j=0;j<_v;j++)if(cp[j]!=0){
			int col = get_quad_col(j,i);
			eq_rows[_m][col]+=c[i]*cp[j];
		}
	}
}

void msolver::sub_p1x(char n)
{
	const fpn *c = getXCoord(n);
	const fpn *cp = getSlope1Coord();
	int i,j;
	for(i=0;i<_v;i++)if(c[i]!=0){
		for(j=0;j<_v;j++)if(cp[j]!=0){
			int col = get_quad_col(j,i);
			eq_rows[_m][col]-=c[i]*cp[j];
		}
	}
}

void msolver::add_p2x(char n)
{
	const fpn *c = getXCoord(n);
	const fpn *cp = getSlope2Coord();
	int i,j;
	for(i=0;i<_v;i++)if(c[i]!=0){
		for(j=0;j<_v;j++)if(cp[j]!=0){
			int col = get_quad_col(j,i);
			eq_rows[_m][col]+=c[i]*cp[j];
		}
	}
}

void msolver::sub_p2x(char n)
{
	const fpn *c = getXCoord(n);
	const fpn *cp = getSlope2Coord();
	int i,j;
	for(i=0;i<_v;i++)if(c[i]!=0){
		for(j=0;j<_v;j++)if(cp[j]!=0){
			int col = get_quad_col(j,i);
			eq_rows[_m][col]-=c[i]*cp[j];
		}
	}
}


void msolver::add_x_line(char n1, char n2)
{
	add_y(n1);
	sub_y(n2);
}

void msolver::add_y_line(char n1, char n2)
{
	add_x(n1);
	sub_x(n2);
}

void msolver::add_p1_line(char n1, char n2)
{
	add_p1x(n1);
	sub_p1x(n2);
	sub_y(n1);
	add_y(n2);
}

void msolver::add_p2_line(char n1, char n2)
{
	add_p2x(n1);
	sub_p2x(n2);
	sub_y(n1);
	add_y(n2);
}

void msolver::add_limit_line(char n1, char n2,char n3)
{
	add_xy(n2,n1);
	add_xy(n3,n2);
	add_xy(n1,n3);
	sub_xy(n1,n2);
	sub_xy(n2,n3);
	sub_xy(n3,n1);
}


void msolver::add_line(char n1, char n2, char n3)
{
	start_one_line();
	if(n3==_n_infx)add_x_line(n1,n2);
	else if(n3==_n_infy)add_y_line(n1,n2);
	else if(n3==_n_infp1)add_p1_line(n1,n2);
	else if(n3==_n_infp2)add_p2_line(n1,n2);
	else add_limit_line(n1,n2,n3);
	end_one_line();
}

void msolver::add_line(char n1, char n2, char n3, char n4)
{
	char ninf=-1;
	char nodes[4];
	int t=0;
	if(is_infty_node(n1)){
		ninf=n1;
	}else{
		nodes[t++]=n1;
	}
	if(is_infty_node(n2)){
		ninf=n2;
	}else{
		nodes[t++]=n2;
	}
	if(is_infty_node(n3)){
		ninf=n3;
	}else{
		nodes[t++]=n3;
	}
	if(is_infty_node(n4)){
		ninf=n4;
	}else{
		nodes[t++]=n4;
	}
	assert(t==0||t>=3);
	if(t==0)return;//skip infty line
	if(t==3){
		add_line(nodes[0],nodes[1],ninf);
		add_line(nodes[0],nodes[2],ninf);
	}else{//t==4
		add_line(nodes[0],nodes[1],nodes[2]);
		add_line(nodes[0],nodes[1],nodes[3]);
	}
}

#define MAX_SOLVERS 16
msolver solvers[MAX_DEPTH][MAX_SOLVERS];
NESET::final_edge_set last_e[MAX_DEPTH];
char chessboard[NUM_NODES][NUM_NODES];
char used[NUM_NODES];
char line_buf[MAX_TOTAL_EDGES][NODES_PER_EDGE];
char line_buf2[MAX_TOTAL_EDGES][NODES_PER_EDGE];
int line_count2;
int line_count;
int cur_free_edge;
int cur_max_degree;
int curr_comb_id;
int curr_line_num;
int last_comb_id;
int last_line_num;
int dep;

vector<NESET::final_edge_set> vecs[MAX_DEPTH];
vector<int>                   esize[MAX_DEPTH];
void tsearch(int num_p_nodes);
std::string stdformat[MAX_DEPTH];
FILE *tmpfile2[MAX_DEPTH];
void output_cur(int n)
{
    int i,j,k;
    NESET nes(line_count,line_buf);
	int d=used[n-1];
	NESET::final_edge_set r = nes.normalize(TARGET_NODES);
	char outputbuf2[4*MAXLINE_COUNT];
#ifdef TRACE
	r.show(outputbuf2);
	fprintf(stderr, "output %s\n",outputbuf2);
#endif
#if 1
	NESET nes2(line_count-d,r.edges);
	NESET::final_edge_set r2= nes2.normalize(TARGET_NODES);
	r2.show(outputbuf2);
#ifdef TRACE
	fprintf(stderr,"prev %s : std %s\n",outputbuf2, stdformat[dep].c_str());
#endif
	if(stdformat[dep]!=outputbuf2)return;
#endif
        //last_e[n-INIT_NODES]=r;
        {
            NESET::final_edge_set::num_edges = line_count;
	    r.show(tmpfile2[dep]);
        }
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
    chessboard[x][y]=chessboard[x][z]=chessboard[x][w]=
        chessboard[y][x]=chessboard[y][z]=chessboard[y][w]=
        chessboard[z][x]=chessboard[z][y]=chessboard[z][w]=
        chessboard[w][x]=chessboard[w][y]=chessboard[w][z]=value;
}

void setv(int x,int y,int z,int w)
{
    set_to(x,y,z,w,line_count);
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
    set_to(x,y,z,w,-1);
    used[x]--;
    used[y]--;
    used[z]--;
    used[w]--;
    line_count--;
}

int depth;
int deb[MAX_DEPTH];
void search(int num_nodes, int sv);

void tsearch(int num_p_nodes)
{
    int i;
//    if(2*used[num_p_nodes-1]+2+line_count<lower_bound(num_p_nodes+1))
  //      return;
    int d=num_p_nodes;
    for(i=0;i<num_p_nodes;i++){
        if(d>used[i])d=used[i];
    }
    if(2*d+2+line_count<lower_bound(num_p_nodes+1))return;
    //deb[num_p_nodes-INIT_NODES]=d+1;
    //vecs[num_p_nodes-INIT_NODES].clear();
    //esize[num_p_nodes-INIT_NODES].clear();
    search(num_p_nodes+1,0);
}

void search(int num_nodes,int sv)
{
    int v,w,z;
    int i;
    int u=num_nodes-1;
    int minused=num_nodes;
    for(i=0;i<u;i++)if(used[i]<minused)minused=used[i];
    if(minused<used[u])return;
    if(line_count+minused>=lower_bound(num_nodes)){
        output_cur(num_nodes);
    }
//    if(used[u]>=deb[num_nodes-1-INIT_NODES])return;
    if(line_count>=max_edges(num_nodes))return;
    for(v=sv;v<num_nodes-1;v++){
        if(chessboard[u][v]>=0)continue;
        for(w=v+1;w<num_nodes-1;w++){
            if(chessboard[u][w]>=0||chessboard[v][w]>=0)continue;
            for(z=w+1;z<num_nodes-1;z++){
                if(chessboard[u][z]>=0||chessboard[v][z]>=0||chessboard[w][z]>=0)
                    continue;
                setv(u,v,w,z);
                depth++;
                search(num_nodes,v+1);
                depth--;
                unsetv(u,v,w,z);
            }
        }
    }
}

char line[1024];
int lid;
void process_one_line(char *input, int curnodes)
{
    int i;
    init();
    i=strlen(input);
    while(i>0&&(input[i-1]<'A'||input[i-1]>'Z'))i--;
    if(i<=0)return;
    input[i]='\0';
    if(i%4!=0){
        fprintf(stderr,"Invalid input %s\n",input);
        return;
    }
    int bc=i/4;
    for(i=0;i<bc;i++){
        setv(input[i*4]-'A',input[i*4+1]-'A',input[i*4+2]-'A',input[i*4+3]-'A');
    }
    depth=0;
    char outputbuf[4*MAXLINE_COUNT];
    NESET neset(line_count,line_buf);
    NESET::final_edge_set r = neset.normalize(TARGET_NODES);
    r.show(outputbuf);
#ifdef TRACE
    fprintf(stderr,"process %s\n",outputbuf);
#endif
    stdformat[dep]=outputbuf;
    char fname[200];
    sprintf(fname,"tmpfilef%d",dep);
    tmpfile2[dep] = fopen(fname,"w");
    if(tmpfile2[dep]==NULL){
	    fprintf(stderr,"Fail to open tmpfilef\n");
	    exit(-1);
    }
#if 0
    NESET ne(line_count, line_buf);
    last_e[0]=ne.normalize(INIT_NODES);
    for(i=0;i<line_count&&i<MAX_SOLVERS;i++){
        solvers[0][i].reset(line_buf,line_count,used,i);
        solvers[0][i].reduce();
        if(!solvers[0][i].isValid())
            return;
        while(solvers[0][i].nodeCount()<INIT_NODES)
            solvers[0][i].add_node();
    }
    if(i<MAX_SOLVERS){
        solvers[0][i].reset();
    }
#endif
    tsearch(curnodes);
    fclose(tmpfile2[dep]);
    tmpfile2[dep]=NULL;
    sprintf(fname,"sort -u tmpfilef%d | ./csolve | ./solve8s > tmpfileg%d",dep, dep);
    // fprintf(stderr,"%s\n",fname);
    system(fname);
    sprintf(fname,"sed -r \'/^.{,%d}$/d\' tmpfileg%d >> out%d", 4*showd[curnodes+1-INIT_NODES]-2, dep, curnodes+1);
    // fprintf(stderr,"%s\n",fname);
    system(fname);
    if(curnodes>=TARGET_NODES-1)return;
    dep++;
    sprintf(fname,"tmpfileg%d",dep-1);
    tmpfile2[dep-1] = fopen(fname,"r");
    if(tmpfile2[dep-1] == NULL){
	    fprintf(stderr, "Fail to open file %s\n",fname);
	    exit(-1);
    }
    lid++;
    while(fgets(line,1024,tmpfile2[dep-1])){
	    process_one_line(line,curnodes+1);
    }
    fclose(tmpfile2[dep-1]);
    tmpfile2[dep-1]=NULL;
    dep--;
}


int main()
{
    int lc=0;
#ifdef USE_INIT
    FILE *init_f=fopen("tag","r");
    int stid=0;
    if(init_f!=NULL){
        fscanf(init_f,"%d",&stid);
        fclose(init_f);
        if(stid<0)stid=0;
    }
    int i;
    for(i=0;i<stid;i++){
        if(fgets(line,1024,stdin)==NULL)
            return -1;
    }
    lc=stid;
#endif
    int start=time(NULL);
    while(fgets(line,1024,stdin)){
        if(line[0]<'A'||line[0]>'Z')
            continue;
        process_one_line(line, INIT_NODES);
        lc++;
        fprintf(stderr,"%d(%ds)\n",lc,(int)(time(NULL)-start));
#ifdef USE_INIT
        init_f=fopen("tag","w");
        fprintf(init_f,"%d\n",lc);
        fclose(init_f);
#endif
    }
    return 0;
}
