#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <fstream>
#include <set>
#include <algorithm>
using namespace std;
int n = 5;/////////////////////////////
int maxl = 8; ///////////
typedef long long ll;
vector <vector<tuple< char, int, int>>> word;
ifstream file("word_rus.txt");

struct myhash
{
    vector<myhash*> v;
    bool inf = false;
};

myhash realword;



void hinit(myhash& hah)
{
    hah.v.resize(34, NULL);
}

auto ahash(char h)
{
    if (h == '-') return (ll)33;
    if (h == 'ё') return (ll)32;
    return ((long long)h + 32);
}

bool htake(myhash& hah, string s)
{
    if (s == "")
    {
        return hah.inf;
    }
    else
    {
        auto ah = ahash(s[0]);
        if (hah.v[ah] == NULL)
        {
            return false;
        }
        else
        {
            return htake(*hah.v[ah], s.substr(1, s.size() - 1));
        }
    }
}


void hinse(myhash& hah, string s)
{
    if (s == "")
    {
        hah.inf = true;
    }
    else
    {
        auto ah = ahash(s[0]);
        if (hah.v[ah] != NULL)
        {
            hinse(*hah.v[ah], s.substr(1, s.size() - 1));
        }
        else
        {
            myhash* r = new myhash;
            hah.v[ah] = r;
            hinit(*r);
            hinse(*hah.v[ah], s.substr(1, s.size() - 1));
        }
    }
}


bool chek(vector <vector<char>>& tabl, int ku, int da, vector<tuple< char, int, int>>& wrd)
{
    for (auto u : wrd)
    {
        if (ku == get<1>(u) && da == get<2>(u))
        {
            return false;
        }
    }
    return true;
}

bool isword(vector<tuple< char, int, int>>& wrd)
{
    string s;
    for (auto u : wrd)
    {
        string g = { get<0>(u) };
        s = s + g;
    }
    return htake(realword, s);
}


void smartlengfound(vector <vector<char>>& tabl, int ku, int da, vector<tuple< char, int, int>> slov, int last)
{
    slov.push_back(make_tuple(tabl[ku][da], ku, da));

    if (isword(slov)) { word.push_back(slov);  }
    if (last <= 1) return;

    if (ku > 0)
    {
        if (chek(tabl, ku - 1, da, slov) && tabl[ku - 1][da] != ' ')
            smartlengfound(tabl, ku - 1, da, slov, last - 1);
    }
    if (ku < n - 1)
    {
        if (chek(tabl, ku + 1, da, slov) && tabl[ku + 1][da] != ' ')
            smartlengfound(tabl, ku + 1, da, slov, last - 1);
    }
    if (da > 0)
    {
        if (chek(tabl, ku, da - 1, slov) && tabl[ku][da - 1] != ' ')
            smartlengfound(tabl, ku, da - 1, slov, last - 1);
    }
    if (da < n - 1)
    {
        if (chek(tabl, ku, da + 1, slov) && tabl[ku][da + 1] != ' ')
            smartlengfound(tabl, ku, da + 1, slov, last - 1);
    }
}

void lengfound(vector <vector<char>>& tabl, int ku, int da, vector<tuple< char, int, int>> slov, int last, vector<vector<tuple< char, int, int>>> &redywrd)
{
    slov.push_back(make_tuple(tabl[ku][da], ku, da));

    if (last == 1) { redywrd.push_back(slov); return; }

    if (ku > 0)
    {
        if (chek(tabl, ku - 1, da, slov) && tabl[ku - 1][da] != ' ')
            lengfound(tabl, ku - 1, da, slov, last-1, redywrd);
    }
    if (ku < n - 1)
    {
        if (chek(tabl, ku + 1, da, slov) && tabl[ku + 1][da] != ' ')
            lengfound(tabl, ku + 1, da, slov, last-1, redywrd);
    }
    if (da > 0)
    {
        if (chek(tabl, ku, da-1, slov) && tabl[ku][da - 1] != ' ')
            lengfound(tabl, ku, da - 1, slov, last - 1, redywrd);
    }
    if (da < n - 1)
    {
        if (chek(tabl, ku, da+1, slov) && tabl[ku][da + 1] != ' ')
            lengfound(tabl, ku, da + 1, slov, last - 1, redywrd);
    }
}


void smartperebor(vector <vector<char>> tabl, char l, int ku, int da)
{
    tabl[ku][da] = l;
    vector<vector<tuple< char, int, int>>> redywrd;
    for (int i = 1; i <= maxl; i++)
    {
        vector<tuple< char, int, int>>  slov;
        lengfound(tabl, ku, da, slov, i, redywrd);
        for (auto u = redywrd.begin(); u != redywrd.end(); u++)
        {
            vector<tuple< char, int, int>> uu;
            for (int j = (*u).size()-1; j >0 ; j--)
            {
                uu.push_back((*u)[j]);
            }
            
            smartlengfound(tabl, ku, da, uu, maxl-i);
        }
    }
}



int main()
{
    hinit(realword);
    while (!file.eof())
    {
        string s;
        getline(file, s);
        hinse(realword, s);
    }
    file.close();

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");
    vector <vector<char>> tabl;
    tabl.resize(n);
    for (int i = 0; i < n; i++)
    {
        tabl[i].resize(n, ' ');
    }
    while (true)
    {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
            {
                char x;
                cin >> x;
                if (x == '-') x = ' ';
                tabl[i][j] = x;
            }
        vector <char> letter;
        letter = { 'а','б','в','г','д','е','ё','ж', 'з','и','й','к','л','м','н','о','р','р','с','т','у','ф','ч','ц', 'ш','щ','ъ','ы','ь','э','ю','я','х' };
        for (int l = 0; l < letter.size(); l++)
        {
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                {
                    bool f = false;
                    if (i > 0)
                    {
                        if (tabl[i - 1][j] != ' ') f = true;
                    }
                    if (i < n - 1)
                    {
                        if (tabl[i + 1][j] != ' ') f = true;
                    }
                    if (j > 0)
                    {
                        if (tabl[i][j - 1] != ' ') f = true;
                    }
                    if (j < n - 1)
                    {
                        if (tabl[i][j + 1] != ' ') f = true;
                    }

                    if (tabl[i][j] == ' ' && f)
                    {
                        smartperebor(tabl, letter[l], i, j);
                    }
                }
        }
        // make_tuple(s1, s2, s3);
        set<string> ansver;
        string s = "";
        for (auto i = word.begin(); i != word.end(); i++)
        {
            s = "";
            for (int j = 0; j < (*i).size(); j++)
            {
                s += get<0>((*i)[j]);
            }
            ansver.insert(s);
        }


        for (int i = 1; i <= maxl; i++)
        {
            for (auto j = ansver.begin(); j!= ansver.end(); j++)
            {
                if ((*j).size() == i)
                {
                    cout << *j << endl;
                }
            }
        }
        word.clear();
        ansver.clear();
    }
}



void allfound(vector <vector<char>>& tabl, int ku, int da, vector<tuple< char, int, int>> wrd)
{
    wrd.push_back(make_tuple(tabl[ku][da], ku, da));

    if (isword(wrd)) word.push_back(wrd);

    if (ku > 0)
    {

        if (chek(tabl, ku - 1, da, wrd) && tabl[ku - 1][da] != ' ')
            allfound(tabl, ku - 1, da, wrd);
    }
    if (ku < n - 1)
    {
        if (chek(tabl, ku + 1, da, wrd) && tabl[ku + 1][da] != ' ')
            allfound(tabl, ku + 1, da, wrd);
    }
    if (da > 0)
    {
        if (chek(tabl, ku, da - 1, wrd) && tabl[ku][da - 1] != ' ')
            allfound(tabl, ku, da - 1, wrd);
    }
    if (da < n - 1)
    {
        if (chek(tabl, ku, da + 1, wrd) && tabl[ku][da + 1] != ' ')
            allfound(tabl, ku, da + 1, wrd);
    }
}

void usualperebor(vector <vector<char>> tabl, char l, int ku, int da)
{
    tabl[ku][da] = l;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            if (tabl[i][j] != ' ')
            {
                vector<tuple< char, int, int>> wrd;
                allfound(tabl, i, j, wrd);
            }
        }
}