/*
 * Author: Dake Xiao
 * Email : dakex@uci.edu
 */
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cstring>
#include <cstdlib>

using namespace std;
const int MAX_CHARS_PER_LINE = 100;
const int MAX_TOKEN = 100;

int my_stoi(string s) {
    int ans = 0;
    int weight = 1;
    for (int i = (int)s.size() - 1; i >= 0; i--) {
        ans += (int(s[i]) - int('0')) * weight;
        weight *= 10;
    }
    return ans;
}

int parserArg(int argc, char *argv[], string &filename, int &cycles) {
    if (argc < 3) {
        cerr <<
        "Usage: " << argv[0] << " filename.txt" << " cycles" << endl;
        return -1;
    } else {
        filename = string(argv[1]);
        cycles = my_stoi(argv[2]);
        return 0;
    }
}

class PN {
private:
    vector<string> places;
    vector<string> trans;
    map<string, int> token;
    map<string, vector<string> > edges;
    map<string, vector<string> > edgesTo;

    void insertPlace(string name, int num) {
        if (token.find(name) == token.end()) {
            places.push_back(name);
            token[name] = num;
        } else {
            return;
        }
    }

    void insertTrans(string name) {
        trans.push_back(name);
    }

    void insertEdge(string v1, string v2) {
        edges[v1].push_back(v2);
        edgesTo[v2].push_back(v1);
    }

    void parseLine(string *t, char *buf) {
        char *p = strtok(buf, " ");
        int i = 0;
        while (p != NULL) {
            t[i++] = string(p);
            p = strtok(NULL, " ");
        }
    }

public:
    PN(string filename) {
        ifstream fin;
        fin.open(filename.c_str());
        if (!fin.good()) {
            cerr << "No such file" << endl;
            exit(-1);
        }
        while (!fin.eof()) {
            char buf[MAX_CHARS_PER_LINE];
            fin.getline(buf, MAX_CHARS_PER_LINE);
            string t[3];
            parseLine(t, buf);
            switch (t[0][0]) {
                case 'p':
                    insertPlace(t[1], my_stoi(t[2]));
                    break;
                case 't':
                    insertTrans(t[1]);
                    break;
                case 'e':
                    insertEdge(t[1], t[2]);
                    break;
                default:
                    cerr << "Wrong Type" << endl;
                    break;
            }
        }
    }

    bool tryFire(map<string, int> &temp, string T) {
        for (int i = 0; i < (int)edgesTo[T].size(); i++) {
            if (--(temp[edgesTo[T][i]]) < 0)
                return false;
        }
        for (int i = 0; i < (int)edges[T].size(); i++) {
            if (++(temp[edges[T][i]]) > MAX_TOKEN)
                return false;
        }
        return true;
    }

    void fire(map<string, int> temp) {
        token = temp;
    }

    int run(int n) {
        string T = trans[n];
        map<string, int> temp = token;
        if (tryFire(temp, T)) {
            fire(temp);
            return 1;
        } else {
            return 0;
        }
    }

    int getTransNum() {
        return (int) trans.size();
    }

    void printTokens() {
        for (int i = 0; i < (int)places.size(); i++)
            cout << places[i] << ": " << token[places[i]] << " ";
        cout << endl;
    }
};

class my_vector {
private:
    vector<int> v;
public:
    my_vector(int n) {
        for (int i = 0; i < n; i++)
            v.push_back(1);
    }

    void reset() {
        for (int i = 0; i < (int)v.size(); i++)
            v[i] = 1;
    }

    void setZero(int n) {
        v[n] = 0;
    }

    bool allZero() {
        for (int i = 0; i < (int)v.size(); i++)
            if (v[i] == 1)
                return false;
        return true;
    }
};

int main(int argc, char *argv[]) {
    string filename;
    int cycles;

    if (parserArg(argc, argv, filename, cycles) != 0)
        return 1;

    PN pn(filename);
    pn.printTokens();

    int Ts = pn.getTransNum();
    my_vector flag(Ts);

    for (int i = 0; i < cycles;) {
        int t = rand() % Ts;
        if (pn.run(t)) {
            i++;
            pn.printTokens();
            flag.reset();
        } else {
            flag.setZero(t);
            if (flag.allZero()) {
                cout << "Converge" << endl;
                break;
            }
        }
    }
    return 0;
}
