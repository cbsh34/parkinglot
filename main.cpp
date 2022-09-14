#include <bit/stdc++.h>
#define INF 1e9
using namespace std;

typedef long long ll;

int mp[MAX_N][MAX_N];
ll G[MAX_N][MAX_N];

struct Point
{
	int x,y;
	ll cost;
	bool operator < (const Point &a) const{
		return cost<a.cost;
	}
}
