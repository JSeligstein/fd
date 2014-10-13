import copy

players = {
    'qb' : [
        ['Kaepernick', 'SF', 17.4, 7800],
        ['Manning', 'NYG', 18.43, 7200],
        ['Foles', 'PHI', 16.20, 8100],
        ['Davis', 'STL', 16.26, 7800],
    ],

    'rb' : [
        ['Gore', 'SF', 11.46, 7200],
        ['Hyde', 'SF', 5.4, 5800],

        ['Jennings', 'NYG', 13.20, 7000],
        ['Williams', 'NYG', 6.7, 6300],

        ['McCoy', 'PHI', 9.04, 8000],
        ['Sproles', 'PHI', 12.20, 6100],
        ['Polk', 'PHI', 10.48, 4800],

        ['Stacy', 'STL', 9.78, 7200],
        ['Cunningham', 'STL', 7, 5600],
    ],

    'wr' : [
        ['Cruz', 'NYG', 9.72, 6800],
        ['Beckham', 'NYG', 12.4, 6000],
        ['Randle', 'NYG', 8.48, 5900],
        ['Parker', 'NYG', 3.64, 4800],

        ['Crabtree', 'SF', 9.9, 6700],
        ['Boldin', 'SF', 8.56, 6300],
        ['Johnson', 'SF', 7.56, 5700],
        ['Lloyd', 'SF', 3.48, 5300],

        ['Maclin', 'PHI', 15.88, 8000],
        ['Matthews', 'PHI', 7.82, 6000],
        ['Cooper', 'PHI', 5.86, 5400],

        ['Quick', 'STL', 15.18, 6300],
        ['Britt', 'STL', 6.83, 5500],
        ['Austin', 'STL', 3.93, 5200],
        ['Pettis', 'STL', 5.2, 5000],
        ['Bailey', 'STL', 6.15, 4700],
    ],

    'te' : [
        ['Donnel', 'NYG', 11.62, 6000],
        ['Fells', 'NYG', 4.96, 5000],
        ['Davis', 'SF', 8.53, 5800],
        ['Carrier', 'SF', 2.5, 4600],
        ['Ertz', 'PHI', 7.58, 5200],
        ['Celek', 'PHI', 1.2, 4500],
        ['Cook', 'STL', 7.9, 5200],
        ['Kendricks', 'STL', 5.2, 4900],
    ],

    'k' : [
        ['Dawson', 'SF', 10.8, 5300],
        ['Parkey', 'PHI', 10.40, 5300],
        ['Brown', 'NYG', 8, 5200],
        ['Zuerlein', 'STL', 8.5, 5000],
    ],

    # http://www.cheatsheetwarroom.com/fantasy-football/nfl/free/rankings/offense/defenses.aspx
    # http://fftoday.com/stats/playerstats.php?Season=2014&GameWeek=&PosID=99
    'd' : [
        ['Eagles', 'PHI', 9, 5300],
        ['49ers', 'SF', 6.2, 5300],
        ['Giants', 'NYG', 6, 4800],
        ['Rams', 'STL', 4.8, 4700],
    ],
}

class TeamMaker:
    IDX_NAME = 0
    IDX_TEAM = 1
    IDX_SCORE = 2
    IDX_COST = 3
    IDX_UNIQ = 4

    players = []
    teams = []
    makeup = []
    maxperteam = 1
    budget = 0

    tc = 0

    def __init__(self, players, teams, makeup, maxperteam, budget):
        self.players = players
        self.teams = teams
        self.makeup = makeup
        self.maxperteam = maxperteam
        self.budget = budget
        self.filterPlayers()

        total = 1
        for pos in makeup:
            total = total * len(self.players[pos])
        print "TeamMaker, rough total combinations: ",total
        print ""

    def printPlayer(self, player):
        return player[self.IDX_NAME]+' ('+player[self.IDX_TEAM]+')'

    def printTeams(self, teams):
        idx = 1
        for team in teams:
            print 'Team', idx, ' - Expected Score:', self.teamScore(team)
            for player in team:
                print ' --', self.printPlayer(player)+':', player[self.IDX_SCORE],'$'+str(player[self.IDX_COST])
            print ""

    def filterPlayers(self):
        newPlayers = {}
        for pos in self.players:
            newPlayers[pos] = []
            for player in self.players[pos]:
                if player[self.IDX_TEAM] in self.teams:
                    player.insert(self.IDX_UNIQ, self.printPlayer(player))
                    newPlayers[pos].append(player)
                else:
                    print "Skipping ", self.printPlayer(player)
        self.players = newPlayers

    def findTeams(self):
        return self.maxTeams(self.makeup, [])

    def teamContainsPlayer(self, team, player):
        uniq = player[self.IDX_UNIQ]
        for tplayer in team:
            if tplayer[self.IDX_UNIQ] == uniq:
                return True
        return False

    def teamScore(self, team):
        score = 0
        for player in team:
            score = score + player[self.IDX_SCORE]
        return score

    def teamCost(self, team):
        cost = 0
        for player in team:
            cost = cost + player[self.IDX_COST]
        return cost

    def isValidTeam(self, team):
        if self.teamCost(team) > self.budget:
            return False

        teamCounts = {}
        for player in team:
            t = player[self.IDX_TEAM]
            if t in teamCounts:
                teamCounts[t] = teamCounts[t] + 1
            else:
                teamCounts[t] = 1

        for t in teamCounts:
            if teamCounts[t] > self.maxperteam:
                return False

        return True

    def maxTeams(self, positions, team):
        pos = positions[0]
        positions = positions[1:]

        curmax = 0
        teams = []

        for player in self.players[pos]:
            if self.teamContainsPlayer(team, player):
                continue

            newTeam = copy.copy(team)
            newTeam.append(player)

            if len(positions) == 0:
                self.tc = self.tc+1
                if self.tc % 1000000 == 0:
                    print "Counted", self.tc, "teams"

                if not self.isValidTeam(newTeam):
                    continue

                score = self.teamScore(newTeam)
                if score == curmax:
                    teams.append(newTeam)
                elif score > curmax:
                    curmax = score
                    teams = [newTeam]
            else:
                [score, maxTeams] = self.maxTeams(positions, newTeam)
                if score == curmax:
                    teams = teams + maxTeams
                elif score > curmax:
                    curmax = score
                    teams = maxTeams

        return [curmax, teams]

tm = TeamMaker(players,
                   ['PHI', 'SF', 'NYG', 'STL'],
                   ['qb', 'rb', 'rb', 'wr', 'wr', 'wr', 'te', 'k', 'd'],
                   4,
                   60000
                   );
[scoremax, teams] = tm.findTeams()
tm.printTeams(teams)

