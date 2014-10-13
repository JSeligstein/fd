<?php

date_default_timezone_set('America/Los_Angeles');

$players = array(
    'qb' => array(
        array('Kaepernick', 'SF', 17.4, 7800),
        array('Manning', 'NYG', 18.43, 7200),
        array('Foles', 'PHI', 16.20, 8100),
        array('Davis', 'STL', 16.26, 7800),
    ),

    'rb' => array(
        array('Gore', 'SF', 11.46, 7200),
        array('Hyde', 'SF', 5.4, 5800),

        array('Jennings', 'NYG', 13.20, 7000),
        array('Williams', 'NYG', 6.7, 6300),

        array('McCoy', 'PHI', 9.04, 8000),
        array('Sproles', 'PHI', 12.20, 6100),
        array('Polk', 'PHI', 10.48, 4800),

        array('Stacy', 'STL', 9.78, 7200),
        array('Cunningham', 'STL', 7, 5600),
    ),

    'wr' => array(
        array('Cruz', 'NYG', 9.72, 6800),
        array('Beckham', 'NYG', 12.4, 6000),
        array('Randle', 'NYG', 8.48, 5900),
        array('Parker', 'NYG', 3.64, 4800),

        array('Crabtree', 'SF', 9.9, 6700),
        array('Boldin', 'SF', 8.56, 6300),
        array('Johnson', 'SF', 7.56, 5700),
        array('Lloyd', 'SF', 3.48, 5300),

        array('Maclin', 'PHI', 15.88, 8000),
        array('Matthews', 'PHI', 7.82, 6000),
        array('Cooper', 'PHI', 5.86, 5400),

        array('Quick', 'STL', 15.18, 6300),
        array('Britt', 'STL', 6.83, 5500),
        array('Austin', 'STL', 3.93, 5200),
        array('Pettis', 'STL', 5.2, 5000),
        array('Bailey', 'STL', 6.15, 4700),
    ),

    'te' => array(
        array('Donnel', 'NYG', 11.62, 6000),
        array('Fells', 'NYG', 4.96, 5000),
        array('Davis', 'SF', 8.53, 5800),
        array('Carrier', 'SF', 2.5, 4600),
        array('Ertz', 'PHI', 7.58, 5200),
        array('Celek', 'PHI', 1.2, 4500),
        array('Cook', 'STL', 7.9, 5200),
        array('Kendricks', 'STL', 5.2, 4900),
    ),

    'k' => array(
        array('Dawson', 'SF', 10.8, 5300),
        array('Parkey', 'PHI', 10.40, 5300),
        array('Brown', 'NYG', 8, 5200),
        array('Zuerlein', 'STL', 8.5, 5000),
    ),

    // http://www.cheatsheetwarroom.com/fantasy-football/nfl/free/rankings/offense/defenses.aspx
    // http://fftoday.com/stats/playerstats.php?Season=2014&GameWeek=&PosID=99
    'd' => array(
        array('Eagles', 'PHI', 9, 5300),
        array('49ers', 'SF', 6.2, 5300),
        array('Giants', 'NYG', 6, 4800),
        array('Rams', 'STL', 4.8, 4700),
    ),
);

$tm = new TeamMaker($players,
                    array('PHI', 'SF', 'NYG', 'STL'),
                    array('qb', 'rb', 'rb', 'wr', 'wr', 'wr', 'te', 'k', 'd'),
                    4,
                    60000
                    );
list($max, $teams) = $tm->findTeams();
$tm->printTeams($teams);

class TeamMaker {
    const IDX_NAME = 0;
    const IDX_TEAM = 1;
    const IDX_SCORE = 2;
    const IDX_COST = 3;
    const IDX_UNIQ = 4;

    public $players, $teams, $makeup, $maxperteam, $budget;
    public $tc;

    public function __construct($players, $teams, $makeup, $maxperteam, $budget) {
        $this->players = $players;
        $this->teams = $teams;
        $this->makeup = $makeup;
        $this->maxperteam = $maxperteam;
        $this->budget = $budget;
        $this->filterPlayers();
        
        $total = 1;
        foreach ($makeup as $pos) {
            $total *= count($this->players[$pos]);
        }
        echo "TeamMaker, rough total combinations: ".$total."\n\n";
    }

    public function printPlayer($player) {
        return $player[self::IDX_NAME].' ('.$player[self::IDX_TEAM].')';
    }

    public function printTeams($teams) {
        foreach ($teams as $idx => $team) {
            echo 'Team '.($idx+1).' - Expected Score: '.$this->teamScore($team)."\n";
            foreach ($team as $player) {
                echo ' -- '.$this->printPlayer($player).': '
                     .$player[self::IDX_SCORE].' '
                     .'$'.$player[self::IDX_COST]."\n";
            }
            echo "\n";
        }
    }

    public function filterPlayers() {
        $newPlayers = array();
        $teams = array_flip($this->teams);
        foreach ($this->players as $pos => $players) {
            $newPlayers[$pos] = array();
            foreach ($players as $player) {
                if (isset($teams[$player[self::IDX_TEAM]])) {
                    $player[self::IDX_UNIQ] = $this->printPlayer($player);
                    $newPlayers[$pos][] = $player;
                } else {
                    echo 'Skipping '.$this->printPlayer($player)."\n";
                }
            }
        }
        $this->players = $newPlayers;
    }

    public function findTeams() {
        return $this->maxTeams($this->makeup, array());
    }

    public function teamContainsPlayer($team, $player) {
        $uniq = $player[self::IDX_UNIQ];
        foreach ($team as $tplayer) {
            if ($tplayer[self::IDX_UNIQ] == $uniq) {
                return true;
            }
        }
        return false;
    }

    public function teamScore($team) {
        $score = 0;
        foreach ($team as $player) {
            $score += $player[self::IDX_SCORE];
        }
        return $score;
    }

    public function teamCost($team) {
        $cost = 0;
        foreach ($team as $player) {
            $cost += $player[self::IDX_COST];
        }
        return $cost;
    }

    public function isValidTeam($team) {
        if ($this->teamCost($team) > $this->budget) {
            return false;
        }


        $teams = array();
        foreach ($team as $player) {
            $t = $player[self::IDX_TEAM];
            if (isset($teams[$t])) {
                $teams[$t]++;
            } else {
                $teams[$t] = 1;
            }
        }

        foreach ($teams as $t => $c) {
            if ($c > $this->maxperteam) {
                return false;
            }
        }

        return true;
    }

    public function maxTeams($positions, $team) {
        $pos = array_shift($positions);
        $max = 0;
        $teams = array();
        foreach ($this->players[$pos] as $player) {
            if ($this->teamContainsPlayer($team, $player)) {
                continue;
            }

            $newTeam = $team;
            $newTeam[] = $player;

            if (empty($positions)) {
                $this->tc++;
                if ($this->tc % 1000000 == 0) {
                    echo date('r').' Counted '.$this->tc." teams\n";
                }

                if (!$this->isValidTeam($newTeam)) {
                    continue;
                }

                $score = $this->teamScore($newTeam);
                if ($score == $max) {
                    $teams[] = $newTeam;
                } elseif ($score > $max) {
                    $max = $score;
                    $teams = array($newTeam);
                }
            } else {
                list($score, $max_teams) = $this->maxTeams($positions, $newTeam);
                if ($score == $max) {
                    $teams = array_merge($teams, $max_teams);
                } elseif ($score > $max) {
                    $max = $score;
                    $teams = $max_teams;
                }
            }
        }
        return array($max, $teams);
    }
}
                

