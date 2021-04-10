#include<iostream>
#include <stdlib.h>
#include <vector>
#include<queue>
#include <set> 
#include<limits>
#define loop(i,a,b) for(int i=a;i<=b;i++)
#define pb push_back
#define travel(i,a) for(auto i=a.begin();i!=a.end();i++)//very imp macro for traversal of cointainers
#include <utility> 
#include <algorithm> 
#include <math.h>
#include <bits/stdc++.h> 

// #include <ctime>
// #include <ratio>
// #include <chrono>

using namespace std;
typedef vector< int > vi;
typedef pair< int,int > pi;
#define no_of_datapoints 30000
int alpha;
class datapoint;
class region;
class internalnode;
class leafnode;

class datapoint{
    public:  
    int id;
    int x;
    int y;
    datapoint()
    {
        id=0;
        x=0;
        y=0;
    }
    
};

class region{

    public:
    int xmin; 
    int xmax;
    int ymin;
    int ymax;
    region()
    {
        xmin=0;
        xmax=400;
        ymin=0;
        ymax=400;    
    }

};
region root_region;

class internalnode{
    public:  
    internalnode* leftptr;
    internalnode* rightptr;
    leafnode* leftleafptr;
    leafnode* rightleafptr;
    internalnode* parentptr;
    int axis_split;//-1 for leaf , 0 for x , 1 for y
    int axis_coordinate;//value of coordinate of the splitting axis,-1 for leaf
    string lr_child; //"notset" for root and at the time of node creation ,"left" for left child ,"right" for right child
    string left_node;//leafnode or internalnode
    string right_node;//leafnode or internalnode
    internalnode()
    {
        leftleafptr=NULL;
        rightleafptr=NULL;
        leftptr=NULL;
        rightptr=NULL;
        parentptr=NULL;
        axis_split=-1;
        axis_coordinate=-1;
        lr_child="notset";
        left_node="notset";
        right_node="notset";
    }
    
};
internalnode* root=NULL;

class leafnode{
    public:  
    internalnode* parentptr;
    string lr_child; //"notset" for root and at the time of node creation ,"left" for left child ,"right" for right child
    vector<datapoint> data;
    leafnode()
    {
        parentptr=NULL;
        lr_child=-1;       
        data.clear();
    }
};


void generate_dataset(vector<datapoint> &v)
{
    v.clear();
    loop(i,1,no_of_datapoints)
    {
        datapoint temp;
        temp.id=i;
        temp.x=rand()%399 +1;
        temp.y=rand()%399 +1;
        v.pb(temp);
    }   
}

void generate_datapoints_exec_time(vector<datapoint> &v)
{
    v.clear();
    loop(i,1,10)
    {
        datapoint temp;
        temp.id=i;
        temp.x=rand()%399 +1;
        temp.y=rand()%399 +1;
        v.pb(temp);
    }   
}

void generate_dataset_test(vector<datapoint> &v)
{
    v.clear();

    datapoint temp;
    temp.id=1;
    temp.x=42;
    temp.y=114;
    v.pb(temp);
    

    temp.id=2;
    temp.x=140;
    temp.y=70;
    v.pb(temp); 
    

    temp.id=3;
    temp.x=18;
    temp.y=164;
    v.pb(temp); 
    

    temp.id=4;
    temp.x=120;
    temp.y=216;
    v.pb(temp); 
    

    temp.id=5;
    temp.x=230;
    temp.y=126;
    v.pb(temp);
    

    temp.id=6;
    temp.x=307;
    temp.y=232;
    v.pb(temp); 
    

    temp.id=7;
    temp.x=350;
    temp.y=167;
    v.pb(temp);  
}

void view_dataset(vector<datapoint> &v)
{  
   loop(i,0,v.size()-1)
   {
       cout<<"ID : "<<v[i].id<<", point : ( "<<v[i].x<<" , "<<v[i].y<<" )"<<endl;
   }
}
void display_region_of_node(region r);
region region_of_node(internalnode* n);
void display_internal_node(internalnode* n)
{     
    
    cout<<endl<<"-----------------------------------------------BEGIN";
    cout<<endl<<"NODE  TYPE : ";
    if(n->lr_child=="notset")
    cout<<"ROOT";
    else
    cout<<"INTERNAL NODE";
    cout<<endl;
    cout<<"LINE SPLIT : ";
    if(n->axis_split==0)
    cout<< "x="<<n->axis_coordinate;
    else if (n->axis_split==1)
    cout<< "y="<<n->axis_coordinate;
    cout<<endl;
    cout<<"-----------------------------------------------END"<<endl;

}


void display_leaf_node(leafnode* n)
{     
    cout<<endl<<"-----------------------------------------------BEGIN";
    cout<<endl<<"NODE  TYPE : LEAF"<<endl;
    cout<<"datapoints in leaf:"<<endl;
    view_dataset(n->data);
    cout<<"-----------------------------------------------END"<<endl;

}

bool sortx(datapoint d1, datapoint d2) 
{ 
    return (d1.x < d2.x); 
}

bool sorty(datapoint d1, datapoint d2) 
{ 
    return (d1.y < d2.y); 
}

bool find_median_and_axis(vector<datapoint> &v,int l,int r,int& axis_split,int& axis_coordinate)
{
    if(r-l<alpha)return 1;
    int m=l+(r-l)/2;
    sort(v.begin()+l,v.begin()+r+1, sortx);
    // view_dataset(v);
    int xdiff=v[r].x-v[l].x;
    sort(v.begin()+l,v.begin()+r+1, sorty); 
    // view_dataset(v); 
    int ydiff=v[r].y-v[l].y;
    // cout<<"xdiff:"<<xdiff<<"  "<<"ydiff:"<<ydiff<<endl<<endl;
    if(ydiff>=xdiff)
    {
        axis_split=1;
        axis_coordinate= v[m].y;
        // view_dataset(v); 
    }
    else
    { 
        sort(v.begin()+l,v.begin()+r+1, sortx); 
        axis_split=0;
        axis_coordinate= v[m].x;
        // view_dataset(v); 
    }
    // cout<<endl<<"axis_split:"<<axis_split<<"  "<<"axis_coordinate:"<<axis_coordinate<<endl;
    return 0;

}


void generate_tree(vector<datapoint> &v,int l, int r,internalnode* ptr,string dir)
{
    int axis_split,axis_coordinate;
    if(find_median_and_axis(v,l,r,axis_split,axis_coordinate))
    {   
        if(dir=="left")ptr->left_node="leafnode";
        else if(dir=="right")ptr->right_node="leafnode";
        leafnode* temp=new leafnode();
        temp->parentptr=ptr;
        temp->lr_child=dir;
        for(int i=l;i<=r;i++)
        {
            (temp->data).push_back(v[i]);
        }
        if(dir=="left")ptr->leftleafptr=temp;
        else if(dir=="right")ptr->rightleafptr=temp;
        //insert leaf and terminate recurtion
        return;
    }
    else
    { 
        if(root==NULL)
        {
            int m=l+(r-l)/2;
            if (axis_split==0)
            {
                while(axis_coordinate==v[m].x)
                {
                    m++;
                    if(m==r+1)break;
                }
                m--;
            }
            else if(axis_split==1)
            {
                while(axis_coordinate==v[m].y)
                {
                    m++;
                    if(m==r+1)break;
                }
                m--; 
            }
            if(m==r)
            {
                cout<<"no use forming a tree.";
                return;
            }
            internalnode * temp=new internalnode();
            temp->axis_split=axis_split;
            temp->axis_coordinate=axis_coordinate;
            root=temp;
            generate_tree(v,l,m,root,"left");
            generate_tree(v,m+1,r,root,"right");
        }
        else
        {
            int m=l+(r-l)/2;
            if (axis_split==0)
            {
                while(axis_coordinate==v[m].x)
                {
                    m++;
                    if(m==r+1)break;
                }
                m--;
            }
            else if(axis_split==1)
            {
                while(axis_coordinate==v[m].y)
                {
                    m++;
                    if(m==r+1)break;
                }
                m--; 
            }
            if(m==r)
            {
                if(dir=="left")ptr->left_node="leafnode";
                else if(dir=="right")ptr->right_node="leafnode";
                leafnode* templ=new leafnode();
                templ->parentptr=ptr;
                templ->lr_child=dir;
                for(int i=l;i<=r;i++)
                {
                    (templ->data).push_back(v[i]);
                }
                if(dir=="left")ptr->leftleafptr=templ;
                else if(dir=="right")ptr->rightleafptr=templ;
                //insert leaf and terminate recurtion
            }
            else
            {
            internalnode * temp=new internalnode();
            temp->axis_split=axis_split;
            temp->axis_coordinate=axis_coordinate;
            temp->parentptr=ptr;
            temp->lr_child=dir; 
            if(dir=="left")ptr->left_node="internalnode";
            else if(dir=="right")ptr->right_node="internalnode";
            if(dir=="left")ptr->leftptr=temp;
            else if(dir=="right")ptr->rightptr=temp;
            generate_tree(v,l,m,temp,"left");
            generate_tree(v,m+1,r,temp,"right");
            }
        }
    }
}

void display_region_of_node(region r)
{   
    cout<<"xmin:"<<r.xmin<<", xmax:"<<r.xmax<<", ymin:"<<r.ymin<<", ymax:"<<r.ymax;
} 


region region_of_node(internalnode* n)
{
    region r1;
    internalnode* temp=n;
    if(temp->parentptr==NULL)
    {
    //   display_region_of_node(root_region);
      return root_region;
    }
    while(temp->parentptr!=NULL)
    {   
        int axis=temp->parentptr->axis_split;
        int coordinate=temp->parentptr->axis_coordinate;    
        if(temp->lr_child=="left")
        {
            if(axis==0 && r1.xmax==400) r1.xmax=coordinate;
            if(axis==1 && r1.ymax==400) r1.ymax=coordinate;
        }
        else if(temp->lr_child=="right")
        {
            if(axis==0 && r1.xmin==0) r1.xmin=coordinate;
            if(axis==1 && r1.ymin==0) r1.ymin=coordinate;
        }
        temp=temp->parentptr;
    }
    // display_region_of_node(r1);
    return r1;
}


void enqueue(queue<internalnode*> *q, internalnode*p){
    if(!p) return;
    q->push(p);
}

void LevelOrder(internalnode* root)
{
    if (!root) return;
    queue<internalnode*> q;
    enqueue(&q, root);
    internalnode *curr;
    while(!q.empty()){
        curr = q.front();
        q.pop();
        display_internal_node(curr);
        region r = region_of_node(curr);
        enqueue(&q, curr->leftptr);
        enqueue(&q, curr->rightptr);
    }
}

void enqueue_with_leaf(queue<pair< internalnode*,leafnode*> > *q, pair< internalnode*,leafnode*>p){
    
    q->push(p);
}

void LevelOrder_with_leaf(internalnode* root)
{
    if (!root) return;
    queue<pair< internalnode*,leafnode*> > q;
    enqueue_with_leaf(&q, pair< internalnode*,leafnode*>(root,NULL));
    pair< internalnode*,leafnode*> curr;
    // cout<<"check1"<<endl;
    while(!q.empty()){
        curr = q.front();
        q.pop();
        if(curr.first!=NULL)
        {
            display_internal_node(curr.first);
            if((curr.first)->leftptr!=NULL)enqueue_with_leaf(&q,pair< internalnode*,leafnode*>((curr.first)->leftptr,NULL));
            else if ((curr.first)->leftleafptr!=NULL)enqueue_with_leaf(&q,pair< internalnode*,leafnode*>(NULL,(curr.first)->leftleafptr));

            if((curr.first)->rightptr!=NULL)enqueue_with_leaf(&q,pair< internalnode*,leafnode*>((curr.first)->rightptr,NULL));
            else if ((curr.first)->rightleafptr!=NULL)enqueue_with_leaf(&q,pair< internalnode*,leafnode*>(NULL,(curr.first)->rightleafptr));

        }
        else if (curr.second!=NULL) display_leaf_node(curr.second);
        // region r = region_of_node(curr);
        // cout<<"check2"<<endl;
    }
    // cout<<"check3"<<endl;

}

float distance_between_two_points(int x1, int y1, int x2, int y2) 
{ 
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0); 
} 

float minimum_distance_from_a_region(datapoint d,internalnode* n)
{
    region r=region_of_node(n);
    if((d.x>=r.xmin && d.x<=r.xmax) && (d.y>=r.ymin && d.y<=r.ymax))
    return 0;
    else if((d.x>=r.xmin && d.x>=r.xmax) && (d.y>=r.ymin && d.y<=r.ymax))
    return d.x-r.xmax;
    else if((d.x<=r.xmin && d.x<=r.xmax) && (d.y>=r.ymin && d.y<=r.ymax))
    return r.xmin-d.x;
    else if((d.x>=r.xmin && d.x<=r.xmax) && (d.y>=r.ymin && d.y>=r.ymax))
    return d.y-r.ymax;
    else if((d.x>=r.xmin && d.x<=r.xmax) && (d.y<=r.ymin && d.y<=r.ymax))
    return r.ymin-d.y;
    else
    {
        float min= numeric_limits<float>::max();
        float d1=distance_between_two_points(d.x,d.y,r.xmin,r.ymin);
        if(d1<=min)min=d1;
        float d2=distance_between_two_points(d.x,d.y,r.xmin,r.ymax);
        if(d2<=min)min=d2;
        float d3=distance_between_two_points(d.x,d.y,r.xmax,r.ymin);
        if(d3<=min)min=d3;
        float d4=distance_between_two_points(d.x,d.y,r.xmax,r.ymax);
        if(d4<=min)min=d4;
        return min;
    }
}

struct CustomCompare
{
    bool operator()(const pair< leafnode*,float >& lhs, const pair< leafnode*,float >& rhs)
    {
        if(lhs.second >= rhs.second)
        return true;
        else
        return false;
    }
    
};
struct CustomCompare1
{
    bool operator()(const pair< datapoint,float >& lhs, const pair< datapoint,float >& rhs)
    {
        if(lhs.second >= rhs.second)
        return true;
        else
        return false;
    }
    
};

set< pair< datapoint,float > , CustomCompare1 > knn_set;
set< pair< datapoint,float > , CustomCompare1 > naive_knn_set;

void insert_in_set(leafnode* l,datapoint q)
{   

    // display_leaf_node(l);
    vector<datapoint> v=l->data;
    for(int i=0;i<v.size();i++)
    {
     pair<datapoint,float >temp;
     temp.first=v[i];
     temp.second=distance_between_two_points(v[i].x,v[i].y,q.x,q.y);
     pair<datapoint,float > cmp=*(knn_set.begin());
     if(temp.second<=cmp.second)
     {
     knn_set.erase(knn_set.begin()); 
     knn_set.insert(temp);
     }
    }
}

void naive_insert_in_set(datapoint d,datapoint q)
{
     pair< datapoint,float >temp;
     temp.first=d;
     temp.second=distance_between_two_points(d.x,d.y,q.x,q.y);
     pair< datapoint,float > cmp=*(naive_knn_set.begin());
     if(temp.second<=cmp.second)
     {
        naive_knn_set.erase(naive_knn_set.begin()); 
        naive_knn_set.insert(temp);
     }
}

void display_knn_set(set< pair<datapoint,float > , CustomCompare1 > &s)
{
    set< pair< datapoint,float > , CustomCompare1 > :: iterator itr; 
    for (itr = s.begin(); itr != s.end(); ++itr) 
    { 
        cout<<"( "<<(itr->first).x<<","<<(itr->first).y<<" )"<<", "<<"distance="<<itr->second<<endl; 
    } 

}

void display_naive_knn_set(set< pair< datapoint,float > , CustomCompare1 > &s)
{
    set< pair< datapoint,float > , CustomCompare1 > :: iterator itr; 
    for (itr = s.begin(); itr != s.end(); ++itr) 
    { 
        cout<<"( "<<(itr->first).x<<","<<(itr->first).y<<" )"<<", "<<"distance="<<itr->second<<endl; 
    } 

}

void reverse_search(internalnode* ptr,string dir,datapoint q)//dir can be "left" , "right" or "up"
{   
    // display_internal_node(ptr);
    // cout<<"direction:"<<dir<<endl;
    pair< datapoint,float > cmp=*(knn_set.begin());
    if(minimum_distance_from_a_region(q,ptr)>=cmp.second)return;//prune out the unnecessary region
    if(ptr==NULL)
    return;
    if (dir=="left")
    {
        if(ptr->leftleafptr!=NULL)
        {
            insert_in_set(ptr->leftleafptr,q);
            return;
        }
        else
        {
            reverse_search(ptr->leftptr,"left",q);
            reverse_search(ptr->leftptr,"right",q);
            return;
        }
    }
    else if(dir=="right")
    {
        if(ptr->rightleafptr!=NULL)
        {    
            insert_in_set(ptr->rightleafptr,q);
            return;
        }
        else
        {
            reverse_search(ptr->rightptr,"left",q);
            reverse_search(ptr->rightptr,"right",q);
            return;
        }
    }
    else if(dir=="up")
    {
       
        if(ptr->lr_child=="left") reverse_search(ptr->parentptr,"right",q);
        else if(ptr->lr_child=="right") reverse_search(ptr->parentptr,"left",q);
        if(ptr->parentptr!=NULL)reverse_search(ptr->parentptr,"up",q);
        return;
    }
}

leafnode* search(internalnode* ptr,datapoint q)
{
    // display_internal_node(ptr);
    int axis=ptr->axis_split;
    int coordinate=ptr->axis_coordinate;
    if((axis==0 && q.x>coordinate) || (axis==1 && q.y>coordinate))
    {   
        if(ptr->rightleafptr!=NULL)
        {
            return ptr->rightleafptr;
        }
        return search(ptr->rightptr,q);
    }
    else
    {   
        if(ptr->leftleafptr!=NULL)
        {
            return ptr->leftleafptr;
        }
        return search(ptr->leftptr,q);
    }
}

void KNN(int k,datapoint q,vector<datapoint > &v)
{   
    knn_set.clear();
    for(int i=0;i<k;i++)
    {
        pair< datapoint,float > temp;
        temp.first=v[0];
        temp.second= numeric_limits<float>::max();
        knn_set.insert(temp);
    }
    // display_knn_set(knn_set);
    leafnode* l=search(root,q);
    insert_in_set(l,q);
    // display_knn_set(knn_set);
    if(l->lr_child=="left") reverse_search(l->parentptr,"right",q);
    else if(l->lr_child=="right") reverse_search(l->parentptr,"left",q);
    reverse_search(l->parentptr,"up",q);

}

void naive_KNN(int k,datapoint q,vector<datapoint > &v)
{   
    naive_knn_set.clear();
    for(int i=0;i<k;i++)
    {
        pair< datapoint,float > temp;
        temp.first=v[0];
        temp.second= numeric_limits<float>::max();
        naive_knn_set.insert(temp);
    }
    // display_naive_knn_set(naive_knn_set);
    for (int i = 0; i < v.size(); i++)
    {
        naive_insert_in_set(v[i],q);
    }

}

double execution_time_for_a_k(int k,vector<datapoint > &d)
{
    double avg=0;
    vector<datapoint > dataset_exec_time;
    generate_datapoints_exec_time(dataset_exec_time);
    loop(i,0,dataset_exec_time.size()-1)
    {
        clock_t start, end; 
        start = clock(); 
        KNN(k,dataset_exec_time[i],d);
        end = clock();  
        double time_taken = (double(end - start)*1000) / double(CLOCKS_PER_SEC); 
        // cout << "Time taken by program is : " << fixed  
        //     << time_taken << setprecision(15); 
        // cout << " milisecsec " << endl; 
        avg+=time_taken;
    }
    return avg/dataset_exec_time.size();

}

double naive_execution_time_for_a_k(int k,vector<datapoint > &d)
{
    double avg=0;
    vector<datapoint > dataset_exec_time;
    generate_datapoints_exec_time(dataset_exec_time);
    loop(i,0,dataset_exec_time.size()-1)
    {
        clock_t start, end; 
        start = clock(); 
        naive_KNN(k,dataset_exec_time[i],d);
        end = clock();  
        double time_taken = (double(end - start)*1000) / double(CLOCKS_PER_SEC); 
        // cout << "Time taken by program is : " << fixed  
        //     << time_taken << setprecision(15); 
        // cout << " milisecsec " << endl; 
        avg+=time_taken;
    }
    return avg/dataset_exec_time.size();

}

int main()
{       
  
    alpha=1;
    vector<datapoint > dataset;
    generate_dataset_test(dataset);
    root=NULL;
    generate_tree(dataset,0,dataset.size()-1,root,"notset");
    cout<<endl<<endl<<"Enter corresponding number"<<endl<<endl;
    while (1)
    {   int flag=0;
        cout<<"1 : USE MANUAL DATASET FOR TESTING"<<endl;
        cout<<"2 : USE RANDOM DATASET WITH 30000 DATAPOINITS"<<endl;
        cout<<"3 : LEVEL ORDER TRAVERSAL"<<endl;
        cout<<"4 : KNN QUERY"<<endl;
        cout<<"5 : KNN QUERY USING NAIVE ALGORITHM"<<endl;
        cout<<"6 : EXPIRIMENT ON EXECUTION TIME"<<endl;
        cout<<"7 : EXPIRIMENT ON EXECUTION TIME USING NAIVE KNN ALGORITHM"<<endl;
        cin>>flag;
        cout<<endl;
        if(flag==3)
        {
            cout<<endl<<"START**************************************LEVEL ORDER TRAVERSAL*********************************************START"<<endl;
            LevelOrder_with_leaf(root);
            cout<<endl<<"END***************************************LEVEL ORDER TRAVERSAL*********************************************END"<<endl<<endl<<endl;
        }
        else if(flag==4)
        {
            int xc=0,yc=0,k=1;
            cout<<"enter value of K:";
            cin>>k;
            cout<<"enter x-cordinate of query point:";
            cin>>xc;
            cout<<"enter y-cordinate of query point:";
            cin>>yc; 
            datapoint query;
            query.x=xc;
            query.y=yc;     
            KNN(k,query,dataset);
            cout<<"-------------------------------------"<<endl;
            cout<<"The "<<k<<" nearest neighbours are:"<<endl;
            display_knn_set(knn_set);
            cout<<"-------------------------------------"<<endl;
        }
        else if(flag==5)
        {
            int xc=0,yc=0,k=1;
            cout<<"enter value of K:";
            cin>>k;
            cout<<"enter x-cordinate of query point:";
            cin>>xc;
            cout<<"enter y-cordinate of query point:";
            cin>>yc; 
            datapoint query;
            query.x=xc;
            query.y=yc;     
            naive_KNN(k,query,dataset);
            cout<<"-------------------------------------"<<endl;
            cout<<"The "<<k<<" nearest neighbours are:"<<endl;
            display_naive_knn_set(naive_knn_set);
            cout<<"-------------------------------------"<<endl;
        }
        else if(flag==1)
        {
            dataset.clear();
            int num=0;
            cout<<"ENTER NUMBER OF DATAPOINTS:";
            cin>>num;
            cout<<"ENTER x and y coordinates of all datapoints:"<<endl;
            loop(i,1,num)
            {
                int in_x,in_y;
                cout<<"x=";
                cin>>in_x;
                cout<<"y=";
                cin>>in_y;
                datapoint tempd;
                tempd.id=i;
                tempd.x=in_x;
                tempd.y=in_y;
                dataset.pb(tempd);    
            }
            cout<<"enter alpha:";
            cin>>alpha;
            if(alpha>=dataset.size())
            {
                cout<<"alpha should be less than the size of dataset"<<endl;
                return 0;
            }
            root=NULL;
            generate_tree(dataset,0,dataset.size()-1,root,"notset");
        }
        else if(flag==2)
        {
            dataset.clear();
            generate_dataset(dataset);
            cout<<"enter alpha:";
            cin>>alpha;
            if(alpha>=dataset.size())
            {
                cout<<"alpha should be less than the size of dataset"<<endl;
                return 0;
            }
            root=NULL;
            generate_tree(dataset,0,dataset.size()-1,root,"notset");
        }
        else if(flag==6)
        {
            dataset.clear();
            generate_dataset(dataset);
            cout<<"USING RANDON DATASET WITH 30000 datapoints-"<<endl;
            cout<<"enter alpha:";
            cin>>alpha;
            if(alpha>=dataset.size())
            {
                cout<<"alpha should be less than the size of dataset"<<endl;
                return 0;
            }
            root=NULL;
            generate_tree(dataset,0,dataset.size()-1,root,"notset");
            cout<<"--------------------------------START"<<endl;
            cout<<"k=5  Execution time= "<<execution_time_for_a_k(5,dataset)<<endl;
            cout<<"k=20  Execution time= "<<execution_time_for_a_k(20,dataset)<<endl;
            cout<<"k=50  Execution time= "<<execution_time_for_a_k(50,dataset)<<endl;
            cout<<"k=100 Execution time= "<<execution_time_for_a_k(100,dataset)<<endl;
            cout<<"k=500 Execution time= "<<execution_time_for_a_k(500,dataset)<<endl;
            cout<<"k=1000 Execution time= "<<execution_time_for_a_k(1000,dataset)<<endl;
            // generate_tree(dataset,0,dataset.size()-1,root,"notset");
        }
        else if(flag==7)
        {

            cout<<"--------------------------------START"<<endl;
            cout<<"k=5  Execution time= "<<naive_execution_time_for_a_k(5,dataset)<<endl;
            cout<<"k=20  Execution time= "<<naive_execution_time_for_a_k(20,dataset)<<endl;
            cout<<"k=50  Execution time= "<<naive_execution_time_for_a_k(50,dataset)<<endl;
            cout<<"k=100 Execution time= "<<naive_execution_time_for_a_k(100,dataset)<<endl;
            cout<<"k=500 Execution time= "<<naive_execution_time_for_a_k(500,dataset)<<endl;
            cout<<"k=1000 Execution time= "<<naive_execution_time_for_a_k(1000,dataset)<<endl;
            // generate_tree(dataset,0,dataset.size()-1,root,"notset");
        }
        
    }
    return 0;
}