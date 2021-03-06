#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

const int MAXL=1048576;
const int P=998244353;
const int g[]={3,(P+1)/3};

template<typename T1,typename T2>
    void Inc(T1 &a,T2 b){a+=b;if(a>=P) a-=P;}

template<typename T1,typename T2>
    void Dec(T1 &a,T2 b){a-=b;if(a<0) a+=P;}

template<typename T1,typename T2>
    T1 Add(T1 a,T2 b){a+=b;return a>=P?a-P:a;}

template<typename T1,typename T2>
    T1 Sub(T1 a,T2 b){a-=b;return a<0?a+P:a;}

ll ksm(ll a,ll b)
{
    ll ret=1;
    for(;b;b>>=1,(a*=a)%=P) if(b&1) (ret*=a)%=P;
    return ret;
}

namespace NTT_namespace{
    int rev[MAXL];
    ll inv[MAXL],G[2][MAXL];

    void preworks()
    {
        inv[1]=1;
        for(int i=2;i<MAXL;i++) inv[i]=Sub(P,P/i)*inv[P%i]%P;
        G[0][0]=G[1][0]=1;
        G[0][1]=ksm(g[0],(P-1)/MAXL);
        G[1][1]=ksm(g[1],(P-1)/MAXL);
        for(int i=2;i<MAXL;i++)
            G[0][i]=G[0][i-1]*G[0][1]%P,
            G[1][i]=G[1][i-1]*G[1][1]%P;
    }

    int init(int n)
    {
        int tot=1,lg2=0;
        while(tot<n) tot<<=1,lg2++;
        for(int i=0;i<tot;i++) rev[i]=(rev[i>>1]>>1)|((i&1)<<(lg2-1));
        return tot;
    }

    void NTT(ll *a,int n,int f)
    {
        for(int i=0;i<n;i++) if(i<rev[i]) swap(a[i],a[rev[i]]);
        ll* ed=a+n;
        for(int i=2;i<=n;i<<=1)
            for(ll *x=a,*y=a+(i>>1);x!=ed;x+=i,y+=i)
                for(int k=0;k<(i>>1);k++)
                {
                    ll u=x[k],t=y[k]*G[f][MAXL/i*k]%P;
                    x[k]=Add(u,t);y[k]=Sub(u,t);
                }
        if(f) for(int i=0;i<n;i++) (a[i]*=inv[n])%=P;
    }
}

struct Poly{
    
    vector<ll> v;

    Poly(){}
    Poly(int n){v.resize(n);}
    Poly(const initializer_list<ll> &T){v=T;}
    
    ll& operator [] (int x) {return v[x];}
    const ll& operator [] (int x) const {return v[x];}

    int size() const {return v.size();}
    void resize(int x){v.resize(x);}
    void reduce(){while(v.size()&&v.back()==0) v.pop_back();}
    void reverse(){std::reverse(v.begin(),v.end());}

    void print()const{for(size_t i=0;i<v.size();i++) printf("%lld%c",v[i],"\n "[i!=v.size()-1]);}
    
    friend Poly operator + (const Poly &lhs,const Poly &rhs)
    {
        if(lhs.size()>rhs.size())
        {
            Poly Ret(lhs.size());
            for(int i=0;i<rhs.size();i++)
                Ret[i]=Add(lhs[i],rhs[i]);
            for(int i=rhs.size();i<lhs.size();i++)
                Ret[i]=lhs[i];
            return Ret;
        }
        else
        {
            Poly Ret(rhs.size());
            for(int i=0;i<lhs.size();i++)
                Ret[i]=Add(lhs[i],rhs[i]);
            for(int i=lhs.size();i<rhs.size();i++)
                Ret[i]=rhs[i];
            return Ret;
        }
    }

    friend Poly operator - (const Poly &lhs,const Poly &rhs)
    {
        if(lhs.size()>rhs.size())
        {
            Poly Ret(lhs.size());
            for(int i=0;i<rhs.size();i++)
                Ret[i]=Sub(lhs[i],rhs[i]);
            for(int i=rhs.size();i<lhs.size();i++)
                Ret[i]=lhs[i];
            return Ret;
        }
        else
        {
            Poly Ret(rhs.size());
            for(int i=0;i<lhs.size();i++)
                Ret[i]=Sub(lhs[i],rhs[i]);
            for(int i=lhs.size();i<rhs.size();i++)
                Ret[i]=Sub(0,rhs[i]);
            return Ret;
        }
    }

    friend Poly operator * (const Poly &lhs,const Poly &rhs)
    {
        using namespace NTT_namespace;
        static ll t1[MAXL],t2[MAXL];
        if(lhs.size()<20||rhs.size()<20)
        {
            Poly Ret(lhs.size()+rhs.size()-1);
            for(int i=0;i<lhs.size();i++)
                for(int j=0;j<rhs.size();j++)
                    Ret[i+j]+=lhs[i]*rhs[j]%P;
            for(ll& v:Ret.v)
                v%=P;
            return Ret;
        }
        int tot=init(lhs.size()+rhs.size()),siz=lhs.size()+rhs.size()-1;
        Poly Ret(siz);
        memset(t1,0,sizeof(ll)*tot);
        memset(t2,0,sizeof(ll)*tot);
        for(int i=0;i<lhs.size();i++) t1[i]=lhs[i];
        for(int i=0;i<rhs.size();i++) t2[i]=rhs[i];
        NTT(t1,tot,0);NTT(t2,tot,0);
        for(int i=0;i<tot;i++) (t1[i]*=t2[i])%=P;
        NTT(t1,tot,1);
        for(int i=0;i<siz;i++) Ret[i]=t1[i];
        return Ret;
    }

    friend Poly operator * (const Poly &lhs,const ll &rhs)
    {
        Poly Ret(lhs.size());
        for(int i=0;i<lhs.size();i++)
            Ret[i]=lhs[i]*rhs%P;
        return Ret;
    }
};

const int MAXN=2E5+10;

int n,k,siz[MAXN];
ll fac[MAXN],ifac[MAXN],f[MAXN],sf[MAXN],Ans;
vector<int> T[MAXN];

ll C(int n,int m)
{
    if(n<0||m<0||n<m)
        return 0;
    return fac[n]*ifac[n-m]%P*ifac[m]%P;
}

//first f,second g
pair<Poly,Poly> solve(int l,int r,vector<ll> &a,vector<ll> &b)
{
    if(l==r)
    {
        Poly f(2),g(1);
        f[1]=a[l-1],f[0]=1;
        g[0]=b[l-1];
        return make_pair(f,g);
    }
    int mid=(l+r)>>1;
    auto Lhs=solve(l,mid,a,b),Rhs=solve(mid+1,r,a,b);
    return make_pair(Lhs.first*Rhs.first,Lhs.first*Rhs.second+Lhs.second*Rhs.first);
}

void dfs(int x,int fa)
{
    ll tmp=0;
    siz[x]=1;
    vector<ll> a,b;
    for(int v:T[x])
        if(v!=fa)
        {
            dfs(v,x);
            siz[x]+=siz[v];
            Inc(sf[x],sf[v]);
            a.push_back(siz[v]);
            b.push_back(sf[v]);
            Inc(Ans,sf[v]*tmp%P);
            Inc(tmp,sf[v]);
        }
    if(a.size())
    {
        auto t=solve(1,a.size(),a,b);
        t.second=t.second*Poly({1,n-siz[x]});
        for(int i=0;i<t.first.size();i++)
            Inc(f[x],C(k,k-i)*t.first[i]%P*fac[i]%P);
        for(int i=0;i<t.second.size();i++)
            Inc(Ans,C(k,k-i)*t.second[i]%P*fac[i]%P);
    }
    else
        Inc(f[x],C(k,k));
    Inc(sf[x],f[x]);
}

int main()
{
    NTT_namespace::preworks();
    scanf("%d%d",&n,&k);
    if(k==1)
        return printf("%lld\n",1ll*n*(n-1)%P*ksm(2,P-2)%P),0;
    fac[0]=ifac[0]=1;
    for(int i=1;i<=k;i++)
        fac[i]=fac[i-1]*i%P;
    ifac[k]=ksm(fac[k],P-2);
    for(int i=k-1;i;i--)
        ifac[i]=ifac[i+1]*(i+1)%P;
    for(int i=1,u,v;i<n;i++)
        scanf("%d%d",&u,&v),
        T[u].push_back(v),
        T[v].push_back(u);
    dfs(1,0);
    printf("%lld\n",Ans);
}
