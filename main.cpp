#include <bits/stdc++.h>

#define MAX_N 100
#define MAX_M 100

using namespace std;

typedef long long ll;

//맵에 대한 기본 정보
int N, M;
int arr[MAX_N+1][MAX_M+1];
ll G[MAX_N+1][MAX_M+1];

//dijkstra를 위한 변수
ll dist[MAX_N+1][MAX_M+1];
int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1};

//좌표 정보
struct info{
    int x, y;
    ll cost;
    info(int a, int b, ll c){
        x=a, y=b;
        cost = c;
    }
    bool operator < (const info &a) const
    {
        return cost>a.cost;
    }
};

//탈출구, 주차공간 위치
vector< pair<int, int> > exits, park;

struct Data_Log{
    int t;
    int x, y;
    ll val;
    bool operator < (const Data_Log &a) const
    {
        return t > a.t;
    }
};
priority_queue<Data_Log> lst;

//경로 찾기
vector< pair<int, int> > path;
bool get_path(int x, int y, int end_x, int end_y)
{
    if(x==end_x && y==end_y) return 1;
    for(int i=0; i<4; i++){
        int nx = dx[i] + x, ny = dy[i] + y;
        if(nx<1 || ny<1 || nx>N || ny>M) continue;
        if(dist[nx][ny] == dist[x][y] + G[nx][ny]){
            path.push_back({nx, ny});
            if(get_path(nx, ny, end_x, end_y)){
                return 1;
            }
            path.pop_back();
        }
    }
    return 0;
}

void dijkstra(char typ, int start_x, int start_y, int t)
{
    //init
    priority_queue<info> pq;
    pq.push({start_x, start_y, 0LL});
    for(int i=1; i<=MAX_N; i++){ for(int j=1; j<=MAX_M; j++){ dist[i][j]=0x7fffffff; } }
    dist[start_x][start_y] = G[start_x][start_y];

    //dijkstra
    while(!pq.empty()){
        int x = pq.top().x, y = pq.top().y;
        ll cost = pq.top().cost;
        pq.pop();
        if(dist[x][y]<cost) continue;
        for(int i=0; i<4; i++){
            int nx = dx[i] + x, ny = dy[i] + y;
            if(nx<1 || ny<1 || nx>MAX_N || ny>MAX_M || arr[nx][ny]==1 || (arr[x][y]==arr[nx][ny] && arr[x][y]!=0) || arr[nx][ny]==9) continue;
            if(arr[nx][ny]==4) continue;
            if(dist[nx][ny]>dist[x][y] + G[nx][ny]){
                dist[nx][ny] = dist[x][y] + G[nx][ny];
                pq.push({nx, ny, dist[nx][ny]});
            }
        }
    }
    //get_path
    ll mn = 0x7fffffff, end_x, end_y;
    path.clear();
    if(typ=='i'){

        for(pair<int, int> &i : park){
            if(mn>dist[i.first][i.second]){
                mn = dist[i.first][i.second];
                end_x = i.first;
                end_y = i.second;
            }
        }
        path.push_back({start_x, start_y});
        get_path(start_x, start_y, end_x, end_y);
        arr[end_x][end_y] = 4;
    }

    if(typ=='o'){
        for(pair<int, int> &i : exits){
            if(mn>dist[i.first][i.second]){
                mn = dist[i.first][i.second];
                end_x = i.first;
                end_y = i.second;
            }
        }
        path.push_back({start_x, start_y});
        get_path(start_x, start_y, end_x, end_y);
        arr[start_x][start_y] = 2;
    }
    for(pair<int, int> &i : path){
        int k = rand()%10 + 5;
        lst.push({t, i.first, i.second, k});
        lst.push({t+3, i.first, i.second, -k});
    }
    printf("cost : %lld\n", dist[end_x][end_y]);
}

void update(int T)
{
    while(!lst.empty() && lst.top().t==T){
        G[lst.top().x][lst.top().y] += lst.top().val;
        lst.pop();
    }
}

void print_map()
{
    printf("\n\n-------------------------------------------------\nmap\n");
    printf("arr[i][j] = 0 : 빈 칸\n");
    printf("arr[i][j] = 1 : 벽\n");
    printf("arr[i][j] = 2 : 주차 공간\n");
    printf("arr[i][j] = 3 : 입 / 출구\n");
    printf("arr[i][j] = 4 : 이미 차랑 한 몸이 되어버린 주차공간\n");
    for(int i=1; i<=N; i++){
        for(int j=1; j<=M; j++){
            if(arr[i][j]==0) printf(". ");
            else if(arr[i][j]==1) printf("# ");
            else printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
    printf("-------------------------------------------------\n\n");
}

void print_G()
{
    printf("\n\n-------------------------------------------------\nG\n");
    for(int i=1; i<=N; i++){
        for(int j=1; j<=M; j++){
            if(arr[i][j]==1) printf("# ");
            else printf("%d ", G[i][j]);
        }
        printf("\n");
    }
    printf("-------------------------------------------------\n\n");
}

void input()
{
    printf("input of map information : ");
    //arr[i][j] = 0 : 빈 칸
    //arr[i][j] = 1 : 벽
    //arr[i][j] = 2 : 주차 공간
    //arr[i][j] = 3 : 입/출구
    //arr[i][j] = 4 : 이미 차랑 한 몸이 되어버린 주차공간
    scanf("%d %d", &N, &M);
    for(int i=1; i<=N; i++){
        for(int j=1; j<=M; j++){
            scanf("%d", &arr[i][j]);
            if(arr[i][j]==2) park.push_back({i, j});
            if(arr[i][j]==3) exits.push_back({i, j});
        }
    }
    printf("\n");
    int t = 1;
    getchar();
    printf("--- order list ---\n");
    printf("  car  : 입 / 출차 데이터 입력\n");
    printf("  exit : 입 / 출구 위치 출력\n");
    printf("  park : 주차 공간 출력\n");
    printf("  map  : 주차장 구조 출력\n");
    printf("  G    : 가중치 행렬 출력\n");
    printf("  pass : 해당 시간대 건너뛰기\n\n\n");
    while(true){
        string order;
        printf("time %d, 명령을 입력하세요(car, exit, park, map, G, pass) : ", t);
        cin >> order;
        if(order=="car"){
            char ord;
            int x, y;
            getchar();
            printf("  time %d, input data(order, x, y) : ", t);
            scanf("%c %d %d", &ord, &x, &y);
            if((ord=='i' && arr[x][y]!=3) || (ord=='o' && arr[x][y]!=4) || (ord!='i' && ord!='o')){
                printf("  wrong input!\n");
                continue;
            }
            dijkstra(ord, x, y, t);
            update(t);
            getchar();
            t++;
        }
        else if(order=="exit"){
            printf("exit area : ");
            for(auto &i : exits) printf("(%d, %d)   ", i.first, i.second);
            printf("\n");
            continue;
        }
        else if(order=="park"){
            printf("park area : ");
            for(auto &i : park) printf("(%d, %d)   ", i.first, i.second);
            printf("\n");
            continue;
        }
        else if(order=="map"){
            print_map();
            continue;
        }
        else if(order=="G"){
            print_G();
            continue;
        }
        else if(order=="pass"){
            update(t);
            t++;
            continue;
        }
        else{
            printf("wrong input!!!\n");
            continue;
        }

    }
}

int main()
{
    srand(time(NULL));
    for(int i=1; i<=MAX_N; i++){
        for(int j=1; j<=MAX_M; j++){
            G[i][j] = 1;
        }
    }
    input();
    return 0;
}
