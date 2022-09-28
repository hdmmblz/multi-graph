#include<bits/stdc++.h>
using namespace std;
int a[1005];
int lab[1005],tmp[1005],tmp2[1005];
vector<int>s[1005][5];
vector<int>ty[1005][5];
set<int>st;
set<int>::iterator it;
void add_edge(int x,int y,int t,int typ)
{
    s[x][t].push_back(y);
    ty[x][t].push_back(typ);
    s[y][t].push_back(x);
    ty[y][t].push_back(typ);
}
void generate_graph(int layer,int n,int m,int ty_range,int rel_num=1)
{
    st.clear();
    int x,y;
    for(int i=0;i<n;i++)for(int j=0;j<layer;j++)s[i][j].clear(),ty[i][j].clear();
    for(int i=0;i<m;i++)
    {
        while(1)
        {
            x=rand()%n;
            y=rand()%n;
            if(x==y)continue;
            if(x>y)swap(x,y);
            it=st.find(n*x+y);
            if(it!=st.end())continue;
            st.insert(n*x+y);
            break;
        }
        for(int j=0;j<layer;j++)
        {
            for(int k=0;k<rel_num;k++)
            {
                int rd=rand()%2;
                if(rd==0)continue;
                //add_edge(x,y,j,0);
                add_edge(x,y,j,k);
            }
        }
    }
    for(int i=0;i<n;i++)a[i]=rand()%ty_range;
}
void generate_1(int n_train,int n_test,int n_node_l,int n_node_r)
{
    int cnt[1005][2];
    int sum_lab=0,edg=0;
    FILE* ftrain=fopen("tp1/train-random-erdos-5000-40-50.txt","w");
    FILE* ftest=fopen("tp1/test-random-erdos-500-40-50.txt","w");
    FILE* out;
    fprintf(ftrain,"%d %d\n",n_train*2,2);
    fprintf(ftest,"%d %d\n",n_test*2,2);
    for(int i=1;i<=n_train+n_test;i++)
    {

        int n=rand()%(n_node_r-n_node_l+1)+n_node_l;
        generate_graph(2,n,4*n,2);
        for(int j=0;j<n;j++)cnt[j][0]=cnt[j][1]=0;
        for(int j=0;j<n;j++)
        {
            edg+=s[j][0].size();
            for(int k=0;k<s[j][0].size();k++)
            {
                int v=s[j][0][k];
                int typ=ty[j][0][k];
                if(a[v]==0)cnt[j][0]++;
            }
            for(int k=0;k<s[j][1].size();k++)
            {
                int v=s[j][1][k];
                int typ=ty[j][1][k];
                if(a[v]==1)cnt[j][1]++;
            }
            //printf("%d %d\n",cnt[j][0],cnt[j][1]);
            int pre_sum=sum_lab;
            if(cnt[j][0]>=2&&cnt[j][1]>=1)lab[j]=1,sum_lab++;
            else lab[j]=0;
            //printf("%d\n",sum_lab-pre_sum);
        }
        if(i<=n_train)out=ftrain;
        else out=ftest;
        for(int j=0;j<2;j++)
        {
            fprintf(out,"%d %d\n",n,1);
            for(int k=0;k<n;k++)
            {
                fprintf(out,"1 %d %d %d",a[k],lab[k],s[k][j].size());
                for(int u=0;u<s[k][j].size();u++)
                {
                    int v=s[k][j][u];
                    int typ=ty[k][j][u];
                    fprintf(out," %d %d",v,typ);
                }
                fprintf(out,"\n");
            }
        }
    }
    printf("%d %d\n",sum_lab/(n_train+n_test),edg/(n_train+n_test));
}
void generate_2(int n_train,int n_test,int n_node_l,int n_node_r)
{
    int cnt[1005][2];
    int sum_lab=0;
    FILE* ftrain=fopen("tp2/train-random-erdos-5000-40-50.txt","w");
    FILE* ftest=fopen("tp2/test-random-erdos-500-40-50.txt","w");
    FILE* out;
    fprintf(ftrain,"%d %d\n",n_train*2,2);
    fprintf(ftest,"%d %d\n",n_test*2,2);
    for(int i=1;i<=n_train+n_test;i++)
    {

        int n=rand()%(n_node_r-n_node_l+1)+n_node_l;
        generate_graph(2,n,4*n,2);
        for(int j=0;j<n;j++)cnt[j][0]=cnt[j][1]=0;
        int pre_sum=sum_lab;
        for(int j=0;j<n;j++)
        {
            for(int k=0;k<s[j][0].size();k++)
            {
                int v=s[j][0][k];
                int typ=ty[j][0][k];
                if(a[v]==0)cnt[j][0]++;
            }
            for(int k=0;k<s[j][1].size();k++)
            {
                int v=s[j][1][k];
                int typ=ty[j][1][k];
                if(a[v]==1)cnt[j][1]++;
            }
            //printf("%d %d\n",cnt[j][0],cnt[j][1]);
            if(cnt[j][0]>=2&&cnt[j][1]>=1)lab[j]=1,sum_lab++;
            else lab[j]=0;
        }
        for(int j=0;j<n;j++)
        {
            int all=sum_lab-pre_sum;
            int my=0;
            for(int k=0;k<s[j][1].size();k++)
            {
                int v=s[j][1][k];
                if(lab[v])my++;
            }
            //printf("%d %d %d\n",all-my,all,my);
            if(all-my>=37&&all-my<=55)tmp[j]=1;
            else tmp[j]=0;
        }
        sum_lab=pre_sum;
        for(int j=0;j<n;j++)
        {
            lab[j]=tmp[j];
            if(lab[j]==1)sum_lab++;
        }
        if(i<=n_train)out=ftrain;
        else out=ftest;
        for(int j=0;j<2;j++)
        {
            fprintf(out,"%d %d\n",n,1);
            for(int k=0;k<n;k++)
            {
                fprintf(out,"1 %d %d %d",a[k],lab[k],s[k][j].size());
                for(int u=0;u<s[k][j].size();u++)
                {
                    int v=s[k][j][u];
                    int typ=ty[k][j][u];
                    fprintf(out," %d %d",v,typ);
                }
                fprintf(out,"\n");
            }
        }
    }
    printf("%d\n",sum_lab/(n_train+n_test));
}
void generate_3(int n_train,int n_test,int n_node_l,int n_node_r)
{
    int cnt[1005][2];
    int sum_lab=0;
    FILE* ftrain=fopen("tp3/train-random-erdos-5000-40-50.txt","w");
    FILE* ftest=fopen("tp3/test-random-erdos-500-40-50.txt","w");
    FILE* out;
    fprintf(ftrain,"%d %d\n",n_train*2,2);
    fprintf(ftest,"%d %d\n",n_test*2,2);
    for(int i=1;i<=n_train+n_test;i++)
    {

        int n=rand()%(n_node_r-n_node_l+1)+n_node_l;
        generate_graph(2,n,4*n,2);
        for(int j=0;j<n;j++)cnt[j][0]=cnt[j][1]=0;
        for(int j=0;j<n;j++)
        {
            int up=0;
            for(int k=0;k<s[j][0].size();k++)
            {
                int v=s[j][0][k];
                int typ=ty[j][0][k];
                if(a[v]==0)
                {
                    for(int kk=0;kk<s[j][1].size();kk++)
                    {
                        int vv=s[j][1][kk];
                        if(vv==v)
                        {
                            up++;
                            if(up>=2)
                            {
                                lab[j]=1,sum_lab++;
                                goto tag;
                            }
                        }
                    }
                }
            }
            lab[j]=0;
            tag:;
            //printf("%d\n",sum_lab-pre_sum);
        }
        if(i<=n_train)out=ftrain;
        else out=ftest;
        for(int j=0;j<2;j++)
        {
            fprintf(out,"%d %d\n",n,1);
            for(int k=0;k<n;k++)
            {
                fprintf(out,"1 %d %d %d",a[k],lab[k],s[k][j].size());
                for(int u=0;u<s[k][j].size();u++)
                {
                    int v=s[k][j][u];
                    int typ=ty[k][j][u];
                    fprintf(out," %d %d",v,typ);
                }
                fprintf(out,"\n");
            }
        }
    }
    printf("%d\n",sum_lab/(n_train+n_test));
}
void generate_4(int n_train,int n_test,int n_node_l,int n_node_r)
{
    int cnt[1005][2];
    int sum_lab=0;
    FILE* ftrain=fopen("tp7/train-random-erdos-5000-40-50.txt","w");
    FILE* ftest=fopen("tp7/test-random-erdos-500-40-50.txt","w");
    FILE* out;
    fprintf(ftrain,"%d %d\n",n_train*2,2);
    fprintf(ftest,"%d %d\n",n_test*2,2);
    for(int i=1;i<=n_train+n_test;i++)
    {

        int n=rand()%(n_node_r-n_node_l+1)+n_node_l;
        generate_graph(2,n,4*n,2,3);
        for(int j=0;j<n;j++)cnt[j][0]=cnt[j][1]=0;
        for(int j=0;j<n;j++)
        {
            int up=0;
            tmp2[j]=0;
            for(int k=0;k<s[j][0].size();k++)
            {
                int v=s[j][0][k];
                int typ=ty[j][0][k];
                if(typ!=0)continue;
                if(a[v]==0)
                {
                    for(int kk=0;kk<s[j][1].size();kk++)
                    {
                        int vv=s[j][1][kk];
                        int typp=ty[j][1][kk];
                        if(typp!=0)continue;
                        if(vv==v)
                        {
                            up++;
                            if(up>=2)
                            {
                                lab[j]=1;
                                goto tag;
                            }
                        }
                    }
                }
            }
            lab[j]=0;
            tag:;
            //printf("%d\n",sum_lab-pre_sum);
        }
        for(int j=0;j<n;j++)
        {
            for(int k=0;k<s[j][0].size();k++)
            {
                int v=s[j][0][k];
                int typ=ty[j][0][k];
                if(typ!=0&&lab[v]==1)tmp2[j]+=typ;
            }
        }
        for(int j=0;j<n;j++)
        {
            if(tmp2[j]>=2)lab[j]=1,sum_lab++;
            else lab[j]=0;
        }
        if(i<=n_train)out=ftrain;
        else out=ftest;
        for(int j=0;j<2;j++)
        {
            fprintf(out,"%d %d\n",n,1);
            for(int k=0;k<n;k++)
            {
                fprintf(out,"1 %d %d %d",a[k],lab[k],s[k][j].size());
                for(int u=0;u<s[k][j].size();u++)
                {
                    int v=s[k][j][u];
                    int typ=ty[k][j][u];
                    fprintf(out," %d %d",v,typ);
                }
                fprintf(out,"\n");
            }
        }
    }
    printf("%d\n",sum_lab/(n_train+n_test));
}
int main()
{
    srand(233);
    int n_node_l=50;
    int n_node_r=100;
    int n_train=7000;
    int n_test=500;
    generate_1(n_train,n_test,n_node_l,n_node_r);
    generate_2(n_train,n_test,n_node_l,n_node_r);
    generate_3(n_train,n_test,n_node_l,n_node_r);
    generate_4(n_train,n_test,n_node_l,n_node_r);
    return 0;
}