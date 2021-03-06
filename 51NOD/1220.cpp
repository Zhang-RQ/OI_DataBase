#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cctype>
#include<cmath>
#include<iostream>
#include<algorithm>
#include<vector>
#include<set>
#include<map>
#include<queue>
#include<stack>
#include<cassert>
typedef long long ll;
typedef unsigned long long ull;
using namespace std;

const int P=1E9+7;
const int MAXN=1E6+10;
const int inv2=500000004;

bool vis[MAXN];
int pri[MAXN],cnt;
ll mu[MAXN];
map<ll,ll> MU;

void sieve()
{
    mu[1]=1;
    for(int i=2;i<MAXN;i++)
    {
        if(!vis[i]) pri[++cnt]=i,mu[i]=-1;
        for(int j=1;j<=cnt&&i*pri[j]<MAXN;j++)
        {
            vis[i*pri[j]]=1;
            if(i%pri[j]==0) {mu[i*pri[j]]=0;break;}
            else mu[i*pri[j]]=-mu[i];
        }
    }
    for(int i=1;i<MAXN;i++)
        mu[i]=(mu[i-1]+mu[i]*i%P+P)%P;
}

ll ask(ll n)
{
    if(n<MAXN) return mu[n];
    if(MU.find(n)!=MU.end()) return MU[n];
    ll ret=1;
    for(ll i=2,pos;i<=n;i=pos+1)
    {
        pos=n/(n/i);
        ret=(ret-((pos-i+1)%P)*((pos+i)%P)%P*inv2%P*ask(n/i)%P+P)%P;
    }
    return MU[n]=ret;
}

ll f(ll n)
{
    ll ret=0;
    for(ll i=1,pos;i<=n;i=pos+1)
        pos=n/(n/i),(ret+=((pos-i+1)%P)*((pos+i)%P)%P*inv2%P*(n/i)%P)%=P;
    return ret;
}

int main()
{
    ll n,ans=0;sieve();
    scanf("%lld",&n);
    for(ll i=1,pos;i<=n;i=pos+1)
    {
        pos=n/(n/i);
        (ans+=((ask(pos)-ask(i-1))%P+P)%P*f(n/i)%P*f(n/i)%P)%=P;
    }
    printf("%lld\n",ans);
    #ifdef LOCAL
        system("pause");
    #endif
}