#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
using namespace std;
class String {
private:
    char *a;
    int N = 1;
    int Size = 0;
    vector<int>F;
    int lastcalc=0;
    vector<int>Suffix;
    vector<int>Prefix;
    vector<vector<int>>Hash,RevHash;
    static vector<int>primes_forbase;
    static vector<int>primes_formod;
    vector<vector<int>>power;
    vector<vector<int>>inv;
    vector<int>MOD;
    vector<int>base;
    vector<int>inv_base;
    int strlen(char *)const;
    void trick() ;
    void Failure();
    int add(int  , int  , int )const;
    int mul(int  , int  , int )const;
    int fastpower(int ,int ,int ) const;
    void init_Hash(); // Hash String O(N)
    void  hash(vector<vector<int>>&,const String &);
    pair<int,int>Get_Hash(int ,int ,vector<vector<int>>&);

public:
    String() ;
    String(int ) ;
    String(char *) ;
    String(int , char ) ;
    String(const String &); // copy constructor
    int size() const ; // get length of string O(1)
    bool empty() const; // check if string is empty O(1)
    void push_back(char x) ; // push char in end of string O(1)
    void pop_back() ;  // delete last char in end of string O(1)
    char operator[](int ) const; // access char by index O(1)
    char & operator[](int ) ;
    void operator+=(char ) ;  // push char in end of string O(1)
    void operator+=(String ) ; // push string in end of string O(N)
    String & operator=(const String &) ; // assignment O(N)
    bool operator==(const String &) const ; // compare two string O(N)
    bool  operator!=(const String &)const;
    bool operator>(const String &) const;
    bool operator <(const String &) const;
    bool operator <=(const String &) const;
    bool operator >=(const String &) const;
    char *begin() const; // first pointer in array
    char *end() const ;// end pointer in array  = next of last pointer in array
    char back() const; // last char in array
    char &back() ;
    int find(const String &)const; //  return first position for match  O(N*M)
    int find(char *)const;
    String substr(int ,int )const; // get string from left to right O(N)
    String substr(int )const;
    String substr(char *,char *)const; // get string from first pointer to prev of last pointer O(N)
    int longest_prefix_equal_suffix();
    int longest_prefix_equal_suffix(const String & ); // O(N+M) // prefix from first string and suffix in second string
    int longest_prefixPalindrome(); // O(N)
    int longest_suffixPalindrome(); // O(N)
    vector<int>all_prefix_equal_suffix();  // O(N)
    vector<int>lps();  // failure function O(N)
    int findbyKMP( String );// return first position for match  O(N+M)
    int count_as_substring_byKMP_with_overlap( String ); // Number substring in S equal string T O(N+M)
    bool check_if_prefix_Palindrome(int ); // O(N)  not implemented yet you can use is palindrome in range in O(1) by hash
    bool check_if_suffix_Palindrome(int ); //O(N) O(N)  not implemented yet you can use is palindrome in range in O(1) by hash
    vector<int>all_strart_indceis_formatch(String ) ; // O(N+M)
    int Nth_root(); // Minimum prefix in S such that S is a duplicate of the prefix. ex: abxabxabx ans =abx =3 O(N)
    vector<int>all_Nth_root();
    pair<int,int>Get_FullHash(); // get hash for string O(1)
    void rehash();
    pair<int,int>Get_Hash(int ,int ); // get hash in range O(1)
    bool ispalindrome(int ,int); // check if substring is palindrome O(1)
    ~String() ;
};
int String:: strlen(char *str)const{
    int len=0,i=0;
    while (str[i]!='\0')
        len++,i++;
    return len;

}
void  String::trick() {
    this->N *= 2;
    char *cpy = new char[this->N];
    for (int i = 0; i < this->Size; i++)
        cpy[i] = a[i];
    delete [] a;
    a = cpy;
    cpy= nullptr;
}
String ::String () {
    a = new char[this->N];
    F=vector<int>(this->N,-1);
    Prefix=vector<int>(this->N,-1);
    Suffix=vector<int>(this->N,-1);
}
String::String(int N) {
    assert(N >= 0);
    this->Size = N;
    this->N = N+1;
    a = new char[this->N];
    F=vector<int>(this->N,-1);
    Prefix=vector<int>(this->N,-1);
    Suffix=vector<int>(this->N,-1);
}
String::String(char *str) : String(strlen(str)) {
    int SZ= strlen(str);
    for (int i = 0; i < SZ; ++i)
        a[i] = str[i];

}
String::String(int N, char x) : String(N) {
    for (int i = 0; i < this->Size; ++i)
        a[i] = x;

}
String:: String(const String &A):String()
{
    *this=A;
}
int  String::size() const {
    return this->Size;
}
bool String::empty() const {
    return this->Size == 0;
}
void String::push_back(char x) {
    if (this->Size < this->N)
        a[Size++] = x,F.push_back(-1),Prefix.push_back(-1),Suffix.push_back(-1);
    else {
        this->trick();
        this->push_back(x);
    }

}
void String:: pop_back() {
    assert(Size > 0);
    a[Size--] = '\0';
    F.pop_back(),Suffix.pop_back(),Prefix.pop_back();
    lastcalc=min(lastcalc,Size-1);
}
char String::operator[](int index) const{
    assert(index < this->Size);
    return a[index];
}
char & String:: operator[](int index) {
    assert(index < this->Size);
    return a[index];
}
void String:: operator+=(char x) {
    return push_back(x);
}
void String:: operator+=(String A) {
    for (int i = 0; i <A.size(); ++i) {
        this->push_back(A[i]);
    }
}
String & String:: operator=(const String &A) {
    if(&A==this)
        return *this;
    this->Size=A.Size;
    this->N= this->Size+1;
    delete [] a;
    a=new char [this->N];
    for (int i = 0; i < A.Size; ++i) {
        this->a[i] = A[i];
    }
    this->F=A.F;
    this->Prefix=A.Prefix;
    this->Suffix=A.Suffix;
    this->lastcalc=A.lastcalc;
    this->Hash=A.Hash;
    this->inv=A.inv;
    this->MOD=A.MOD;
    this->RevHash=A.RevHash;
    this->base=A.base;
    this->power=A.power;
    return *this;
}
bool String:: operator==(const String &A) const {
    if (A.size() != this->size())
        return 0;
    for (int i = 0; i < this->Size; ++i) {
        if (A[i] != this->a[i])
            return 0;
    }
    return 1;
}
bool String:: operator!=(const String &A)const{
    return !(*this==A);
}
bool String::operator>(const String &A) const {
    int SZ = this->size();
    if (A.size() < SZ)
        SZ = A.size();
    for (int i = 0; i < SZ; ++i) {
        if (this->a[i] > A[i])
            return 1;
        else if (this->a[i] < A[i])
            return 0;
    }
    if (this->size() > A.size())
        return 1;
    return 0;
}
bool String::operator <(const String &A) const
{
    if(A > *this)
        return 1;
    return 0;
}
bool  String::operator <=(const String &A) const
{

    return !(*this>A);
}
bool String:: operator >=(const String &A) const
{
    return !(*this<A);
}
char * String::begin() const{
    return a;
}
char * String::end() const {
    return a + this->Size;
}
char  String:: back() const {
    assert(this->Size > 0);
    return a[this->size()-1];
}
char & String:: back()
{
    assert(this->Size>0);
    lastcalc=0;
    return (*this)[this->Size-1];
}
int String::find(const String &A) const {
    if(A.size()> this->size())
        return -1;
    for (int i = 0; i <this->size()-A.size()+1; ++i) {
        if(A== this->substr(i,i+A.size()-1))
            return i;
    }
    return -1;
}
int String::find(char *str)const {
    return find(String(str));
}
String String::substr(int l, int r) const {
    assert(l<=r&&l>=0&&r< this->size());
    String Tmp(r-l+1);
    for (int i = l; i <=r ; ++i) {
        Tmp[i-l]= this->a[i];
    }
    return Tmp;
}
String String::substr(int l) const {
    return substr(l, this->size()-1);
}
String String::substr(char *start,char *end) const {
    return substr(start- this->a,end-start-1);
}
void String::Failure() {
    if(this->empty())
        return;
    int n= this->size();
    this->F[0]=0;
    int len=0;
    for (int i = this->lastcalc+1; i <n ; ++i) {
        while(len>0&& (*this)[len]!= (*this)[i])
            len=F[len-1];
        if((*this)[i]== (*this)[len])
            len++;
        F[i]=len;
    }
    this-> lastcalc=n-1;
}
vector<int>String::lps()
{
    this->Failure();
    return F;
}
int String::longest_prefix_equal_suffix() {
    if(this->empty())
        return 0;
    this->Failure();
    return F[this->size()-1];
}
vector<int>String::all_prefix_equal_suffix(){
    if(this->empty())
        return {-1};
    this->Failure();
    vector<int>ans;
    int len= this->size();
    while (len>0)
    {
        len=F[len-1];
        ans.push_back(len);
    }
    reverse(ans.begin(),ans.end());
    return ans;
}
int  String::longest_prefix_equal_suffix(const String & A) {
    String N=*this;
    N+='#';
    N+=A;
    return N.longest_prefix_equal_suffix();
}
int String::longest_suffixPalindrome() {
    String X= *this;
    reverse(X.begin(),X.end());
    X+="#";
    X+=*this;
    return X.longest_prefix_equal_suffix();

}
int String::longest_prefixPalindrome()
{
    String X=*this;
    reverse(X.begin(),X.end());
    return X.longest_suffixPalindrome();
}
int String::findbyKMP( String A)
{
    A.Failure();
    int len=0;
    for (int i = 0; i < this->size() ; ++i) {
        while (len>0&&(*this)[i]!=A[len])
            len=A.F[len-1];
        if(A[len]==(*this)[i])
            ++len;
        if(len==A.size())
            return i-A.size()+1;
    }
    return -1;
}
int String::Nth_root() {
    this->Failure();
    int len= this->size();
    if(len==0)
        return len;
    len=F[len-1];
    if(this->size()%(this->size()-len)==0)
        return this->size()-len;
    return this->size();

}
vector<int>String::all_strart_indceis_formatch(String A){
    A.Failure();
    int len=0;
    vector<int>ans;
    for (int i = 0; i < this->size(); ++i) {
        while (len>0&& (*this)[i]!=A[len])
            len=A.F[len-1];
        if(A[len]==(*this)[i])
            ++len;
        if(len==A.size())
        {

            ans.push_back(i-A.size()+1);
            len=A.F[len-1];
        }
    }
    return ans;
}
int String:: count_as_substring_byKMP_with_overlap (String A)
{
    return this->all_strart_indceis_formatch(A).size();
}
vector<int>String::primes_forbase={31,37,47,53,59,67,73,79,97,313};
vector<int>String::primes_formod={1000000007,1000000009,1000000021,1000000097,1000000103,1000000433,1000001329};
int String::add(int a , int b , int m)const{
    return (a % m + b % m) % m ;
}
int String::mul(int a , int b , int m)const{
    return 1LL * a * b % m ;
}
int String:: fastpower(int a ,int b ,int m ) const{
    if(b==0)
        return 1;
    int res= fastpower(a,b/2,m)%m;
    res= mul(res,res,m);
    if(b&1)
        res= mul(res,a,m);
    return res;
}
void String:: init_Hash(){
    this->Hash= this->power= this->inv=vector<vector<int>>(2,vector<int>(this->size()));
    int n= this->size();
    this->power[0][0]= this->power[1][0]= this->inv[0][0]= this->inv[1][0]=1;
    base=MOD=vector<int>(2);
    int p1=rand()%primes_forbase.size();
    int p2=rand()%primes_forbase.size();
    while (p2==p1)
        p2=rand()%primes_forbase.size();
    int mod1=::rand()%primes_formod.size();
    int mod2=::rand()%primes_formod.size();
    while (mod2==mod1)
        mod2=::rand()%primes_formod.size();
    base[0]=primes_forbase[p1];
    base[1]=primes_forbase[p2];
    MOD[0]=primes_formod[mod1];
    MOD[1]=primes_formod[mod2];
    inv_base = {fastpower(base[0] , MOD[0]-2, MOD[0]),fastpower(base[1] , MOD[1]-2, MOD[1]) };
    for(int i = 1 ; i < n ; i++){
        for (int j = 0; j <2 ; ++j) {
            this->power[j][i] = mul(this->power[j][i - 1] , base[j] , MOD[j]);
            this->inv[j][i] = mul(this->inv[j][i - 1] , inv_base[j] , MOD[j]) ;
        }
    }
    hash(Hash,*this);
}
void String::hash(vector<vector<int>>&H,const String &A) {
    int n= this->size();
    H[0][0]=A[0] - 'a' + 1;
    H[1][0]=A[0] - 'a' + 1;
    for(int i = 1 ; i < n ; i++){
        for (int j = 0; j <2 ; ++j) {
            H[j][i] = add(H[j][i-1] , mul(power[j][i] , (A[i] - 'a' + 1) ,MOD[j]) , MOD[j]) ;
        }

    }
}
pair<int,int>String::Get_FullHash() {
    if (Hash.empty())
        this->init_Hash();
    return {this->Hash[0].back(), this->Hash[1].back()};
}

pair<int,int>String::Get_Hash(int l,int r,vector<vector<int>>&H){
    assert(l>=0&&r<H[0].size()&&r>=l);
    int H1=H[0][r];
    int H2=H[1][r];
    if(l-1>=0) {
        H1 = mul(add(H[0][r], MOD[0] - H[0][l - 1], MOD[0]), inv[0][l], MOD[0]);
        H2 = mul(add(H[1][r], MOD[1] - H[1][l - 1], MOD[1]), inv[1][l], MOD[1]);
    }
    return {H1 , H2} ;
}
pair<int,int>String::Get_Hash(int l,int r)
{
    if(Hash.empty())
        this->init_Hash();
    return this->Get_Hash(l,r,Hash);
}
bool String::ispalindrome(int l, int r) {
    if(Hash.empty())
        this->init_Hash();
    if(RevHash.empty())
    {
        RevHash=vector<vector<int>>(2, vector<int>(this->size()));
        String A=*this;
        reverse(A.begin(),A.end());
        hash(RevHash,A);
    }
    //abcde
    //edcba
    if(Get_Hash(l,r)== Get_Hash(this->size()-r-1,this->size()-l-1,RevHash))
        return 1;
    return 0;
}
void String::rehash() {
    this->init_Hash();
    String A= *this;
    reverse(A.begin(),A.end());
    this->hash(RevHash,A);
}
String::~String() {
    delete [] a;
}
String  operator + ( String A,String  B)
{
    A+=B;
    return A;
}
String  operator + ( String A,char *str)
{
    A+=str;
    return A;
}

String  operator + ( char * str ,String  A)
{
    String B=String(str);
    A=B+A;
    return A;
}
std::istream & operator >> (std::istream &in, String &A)
{

    A="";
    while (in.peek()==' '||(int)(in.peek())==10)
        in.get();
    char x;
    while (in.good()&&!in.eof()&&in.peek()!=' '&&(int)(in.peek())!=10&&in>>x)
    {
        A.push_back(x);
    }

    return in;
}
std::ostream & operator << (std::ostream &out, const String &A)
{

    for (int i = 0; i <A.size() ; ++i) {
        out<<A[i];
    }
    return out;
}

void test() {

    String s;
    String t;
    cin>>s>>t;
    //ex: s=abccab
    //ex: t=cab
    cout<<s.findbyKMP(t)<<endl;
    cout<<s.longest_prefix_equal_suffix()<<endl;
    cout<<s.ispalindrome(2,3)<<endl;
    cout<<s.ispalindrome(0,5)<<endl;
    cout<<s.longest_prefix_equal_suffix(t)<<endl;
    cout<<t.longest_prefix_equal_suffix(s)<<endl;


}
int main()
{
    test();
    return 0;
}
