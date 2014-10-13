#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include "twogame.hpp"
#include <map>
#include <math.h>

using namespace std;

long fac(long n) {
    long result = n;
    for (n--; n > 1; n--) {
        result *= n;
    }
    return result;
}
 
long bico(long n, long k) {
    long result = k == 1 ? n : fac(n)/(fac(k)*(fac(n-k)));
    return result;
}

Player::Player(string n, const char *t, double s, int c) {
    this->name = n;
    this->team = t;
    this->score = s;
    this->cost = c;
}

const int POS_QB = 0;
const int POS_RB = 1;
const int POS_WR = 2;
const int POS_TE = 3;
const int POS_K = 4;
const int POS_D = 5;


Team::Team(int size) {
    this->size = size;
    this->players = (Player **)malloc(sizeof(Player *) * size);
}

double Team::score() {
    double score = 0;
    for (int ii = 0; ii < this->size; ii++) {
        score += this->players[ii]->score;
    }
    return score;
}

int Team::cost() {
    int cost = 0;
    for (int ii = 0; ii < this->size; ii++) {
        cost += this->players[ii]->cost;
    }
    return cost;
}

bool Team::hasPlayer(Player *p) {
    for (int ii = 0; ii < this->size; ii++) {
        if (p == this->players[ii]) {
            return true;
        }
    }
    return false;
}

bool Team::isValid(int budget, int maxperteam) {
    if (this->cost() > budget) {
        return false;
    }

    // this is a really slow bottle neck, but it happens rare enough
    map<const char *, int> teamCounts;
    for (int ii = 0; ii < this->size; ii++) {
        Player *p = this->players[ii];
        if (teamCounts.find(p->team) == teamCounts.end()) {
            teamCounts[p->team] = 1;
        } else {
            teamCounts[p->team]++;
        }
    }

    return true;
}

Team *Team::dup() {
    Team *t = new Team(this->size);
    for (int ii = 0; ii < this->size; ii++) {
        t->players[ii] = this->players[ii];
    }
    return t;
}

bool Team::isEqual(Team *team) {
    if (team->size != this->size) {
        return false;
    }

    // so lazy... should probably sort or something first
    for (int ii = 0; ii < team->size; ii++) {
        bool tf = false;
        for (int jj = 0; jj < this->size; jj++) {
            if (team->players[ii] == this->players[jj]) {
                tf = true;
                break;
            }
        }
        if (!tf) {
            return false;
        }
    }
    return true;
}

TeamMaker::TeamMaker(int *positions, int numpos, int budget, int maxperteam, vector<string> *playingTeams) {
    this->positions = positions;
    this->numpos = numpos;
    this->budget = budget;
    this->maxperteam = maxperteam;
    this->maxTeams = new vector<Team *>();
    this->playingTeams = playingTeams;

    this->players = new vector<vector<Player *> >(6);
    this->filterPosition(&qb, POS_QB);
    this->filterPosition(&rb, POS_RB);
    this->filterPosition(&wr, POS_WR);
    this->filterPosition(&te, POS_TE);
    this->filterPosition(&k, POS_K);
    this->filterPosition(&d, POS_D);

    this->printConsiderations();
}

void TeamMaker::filterPosition(vector<Player *> *posplayers, int position) {
    for (int pp = 0; pp < posplayers->size(); pp++) {
        Player *p = posplayers->at(pp);
        if (find(this->playingTeams->begin(), this->playingTeams->end(), p->team) != this->playingTeams->end()) {
            vector<Player *> *curplayers = &this->players->at(position);
            curplayers->push_back(posplayers->at(pp));
        }
    }
}

void TeamMaker::printConsiderations() {
    int posCount[6] = {0,0,0,0,0,0};
    long total = 1;
    for (int pp = 0; pp < numpos; pp++) {
        int pos = this->positions[pp];
        posCount[pos]++;
    }

    for (int pos = 0; pos < 6; pos++) {
        long count = posCount[pos];
        long size = (&this->players->at(pos))->size();
        //total *= bico(size, count);
        //total *= fac(size)/fac(size-count);
        if (count) {
            printf("multi: %ld, %ld, %f\n", size, count, pow(size,count));
        //    total *= pow(size, count);
        total *= fac(size)/fac(size-count);
        }
        printf("total *= fac(%ld)/fac(%ld) which is %ld\n", size, size-count, fac(size)/fac(size-count));
    }

    printf("TeamMaker Considerations\n");
    printf(" -- Teams: %lu\n", this->playingTeams->size());
    printf(" -- Positions: %d\n", this->numpos);
    printf(" -- Combinations: %lu\n", total);
    printf(" -- Quarterbacks: %lu\n", (&this->players->at(POS_QB))->size());
    printf(" -- Running Backs: %lu\n", (&this->players->at(POS_RB))->size());
    printf(" -- Wide Receivers: %lu\n", (&this->players->at(POS_WR))->size());
    printf(" -- Tight Ends: %lu\n", (&this->players->at(POS_TE))->size());
    printf(" -- Kickers: %lu\n", (&this->players->at(POS_K))->size());
    printf(" -- Defenses: %lu\n", (&this->players->at(POS_D))->size());
    printf("\n\n");
}
    
    
pair<double, vector<Team *>* > TeamMaker::findTeams() {
    Team *t = new Team(this->numpos);
    this->findMaxTeams(0, t);
    printf("Counted %d teams\n", this->tc);
    this->dedup();
    return make_pair(this->max, this->maxTeams);
}

void TeamMaker::dedup() {
    // again, really lazy here
    Team *t1, *t2;
    for (int ii = 0; ii < this->maxTeams->size()-1; ii++) {
        t1 = this->maxTeams->at(ii);
        for (int jj = 1; jj < this->maxTeams->size(); jj++) {
            t2 = this->maxTeams->at(jj);
            if (t1->isEqual(t2)) {
                this->maxTeams->erase(this->maxTeams->begin()+jj);
                jj--;
            }
        }
    }
}

void TeamMaker::findMaxTeams(int posIndex, Team *team) {
    int position = this->positions[posIndex];
    vector<Player *> *posPlayers = &this->players->at(position);
    int posSize = posPlayers->size();

    for (int pidx = 0; pidx < posSize; ++pidx) {
        Player *nextPlayer = posPlayers->at(pidx);
        if (team->hasPlayer(nextPlayer)) {
            continue;
        }

        team->players[posIndex] = nextPlayer;
        if (posIndex == this->numpos-1) {
            this->tc++;
            if (this->tc % 10000000 == 0) {
                printf("Counted %d teams\n", this->tc);
            }

            double score = team->score();
            if (score >= this->max) {
                if (!team->isValid(this->budget, this->maxperteam)) {
                    continue;
                }

                if (score > this->max) {
                    this->maxTeams->clear();
                    this->max = score;
                }

                this->maxTeams->push_back(team->dup());
            }
        } else {
            this->findMaxTeams(posIndex+1, team);
        }
    }
}


int main(int argc, char **argv) {

    int numpos = 3;
    int *pos = (int *)malloc(sizeof(int) * numpos);
    pos[0] = POS_QB;
    pos[1] = POS_RB;
    pos[2] = POS_RB;
    /*
    pos[3] = POS_WR;
    pos[4] = POS_WR;
    pos[5] = POS_WR;
    pos[6] = POS_TE;
    pos[7] = POS_K;
    pos[8] = POS_D;*/

    vector<string> *playingTeams = new vector<string>();
    playingTeams->push_back("NYG");
    playingTeams->push_back("PHI");
    playingTeams->push_back("SF");
    playingTeams->push_back("STL");

    TeamMaker *tm = new TeamMaker(pos, numpos, 60000, 4, playingTeams);
    pair<double, vector<Team *> *> res = tm->findTeams();

    double curmax = res.first;
    vector<Team *> *teams = res.second;

    for (int tt = 0; tt < teams->size(); tt++) {
        Team *team = teams->at(tt);
        printf("Team %d -- Expected Score: %.2f\n", tt+1, team->score());
        for (int pp = 0; pp < team->size; pp++) {
            Player *player = team->players[pp];
            cout << " -- " << player->name 
                 << " (" << player->team << ") "
                 << player->score << " "
                 << "$" << player->cost
                 << endl;
        }
        printf("\n");
    }


    return 0;
}
