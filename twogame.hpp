#include <stdlib.h>
#include <stdint.h>
#include <vector>

using namespace std;

class Player {
public:
    string name;
    const char *team;
    double score;
    int cost;

    Player(string n, const char *t, double s, int c); 
};

class Team {
public:
    Team(int size);

    int size;
    Player **players;
    void addPlayer(Player *p);
    double score();
    int cost();
    bool hasPlayer(Player *p);
    bool isValid(int budget, int maxperteam);
    bool isEqual(Team *team);
    Team *dup();
};

class TeamMaker {
public:
    int budget;
    int maxperteam;
    int *positions;
    int numpos;
    int tc;

    vector<vector<Player *> > *players;
    vector<string> *playingTeams;

    double max;
    vector<Team *> *maxTeams;

    TeamMaker(int *positions, int numpos, int budget, int maxperteam, vector<string> *playingTeams);
    pair<double, vector<Team *>* > findTeams();
    void dedup();
    void filterPosition(vector<Player *> *players, int position);
    void printConsiderations();

private:
    void findMaxTeams(int posIndex, Team *team);

};

vector<Player *> qb = {
    new Player("Kaepernick", "SF", 17.4, 7800),
    new Player("Manning", "NYG", 18.43, 7200),
    new Player("Foles", "PHI", 16.20, 8100),
    new Player("Davis", "STL", 16.26, 7800),
    new Player("Brady", "NE", 12.04, 7900),
    new Player("Vick", "NYJ", .89, 6900),
    new Player("Smith", "NYJ", 11.81, 6900),
};

vector<Player *> rb = {
        new Player("Gore", "SF", 11.46, 7200),
        new Player("Hyde", "SF", 5.4, 5800),

        new Player("Jennings", "NYG", 13.20, 7000),
        new Player("Williams", "NYG", 6.7, 6300),

        new Player("McCoy", "PHI", 9.04, 8000),
        new Player("Sproles", "PHI", 12.20, 6100),
        new Player("Polk", "PHI", 10.48, 4800),

        new Player("Stacy", "STL", 9.78, 7200),
        new Player("Cunningham", "STL", 7, 5600),

        new Player("Johnson", "NYJ", 7.48, 6800),
        new Player("Ivory", "NYJ", 10.88, 5600),

        new Player("Ridley", "NE", 9.32, 6300),
        new Player("Vereen", "NE", 9.36, 6200),

};

vector<Player *> wr = {
        new Player("Cruz", "NYG", 9.72, 6800),
        new Player("Beckham", "NYG", 12.4, 6000),
        new Player("Randle", "NYG", 8.48, 5900),
        new Player("Parker", "NYG", 3.64, 4800),

        new Player("Crabtree", "SF", 9.9, 6700),
        new Player("Boldin", "SF", 8.56, 6300),
        new Player("Johnson", "SF", 7.56, 5700),
        new Player("Lloyd", "SF", 3.48, 5300),

        new Player("Maclin", "PHI", 15.88, 8000),
        new Player("Matthews", "PHI", 7.82, 6000),
        new Player("Cooper", "PHI", 5.86, 5400),

        new Player("Quick", "STL", 15.18, 6300),
        new Player("Britt", "STL", 6.83, 5500),
        new Player("Austin", "STL", 3.93, 5200),
        new Player("Pettis", "STL", 5.2, 5000),
        new Player("Bailey", "STL", 6.15, 4700),

        new Player("Edelman", "NE", 11.36, 7200),
        new Player("Amendola", "NE", .86, 4900),

        new Player("Decker", "NYJ", 9.85, 6400),
        new Player("Owusu", "NYJ", 3, 5100),
        new Player("Kerley", "NYJ", 7.2, 4900),
};


vector<Player *> te = {
        new Player("Donnel", "NYG", 11.62, 6000),
        new Player("Fells", "NYG", 4.96, 5000),

        new Player("Davis", "SF", 8.53, 5800),
        new Player("Carrier", "SF", 2.5, 4600),

        new Player("Ertz", "PHI", 7.58, 5200),
        new Player("Celek", "PHI", 1.2, 4500),

        new Player("Cook", "STL", 7.9, 5200),
        new Player("Kendricks", "STL", 5.2, 4900),

        new Player("Gronkowski", "NE", 11.64, 7600),
        new Player("Wright", "NE", 4.5, 5200),

        new Player("Amaro", "NYJ", 4.28, 4700),
        new Player("Cumberland", "NYJ", 2.88, 4500),
};

vector<Player *> k = {
        new Player("Dawson", "SF", 10.8, 5300),
        new Player("Parkey", "PHI", 10.40, 5300),
        new Player("Brown", "NYG", 8, 5200),
        new Player("Zuerlein", "STL", 8.5, 5000),
        new Player("Gostkowski", "NE", 11.4, 5400),
        new Player("Folk", "NYJ", 7.4, 4500),
};

    //  http://www.cheatsheetwarroom.com/fantasy-football/nfl/free/rankings/offense/defenses.aspx
    // http://fftoday.com/stats/playerstats.php?Season=2014&GameWeek=&PosID=99
vector<Player *> d = {
        new Player("Eagles", "PHI", 11.2, 5300),
        new Player("49ers", "SF", 6.2, 5300),
        new Player("Giants", "NYG", 5.8, 4800),
        new Player("Rams", "STL", 5.0, 4700),
        new Player("Patriots", "NE", 8.2, 5400),
        new Player("Jets", "NYJ", 4.2, 4700),
};

