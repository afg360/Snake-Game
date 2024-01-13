#ifndef __CONFIGURE_H__
#define __CONFIGURE_H__

//fxns linked to the options menu
//soundfx/volume
//config files for size of grid
//change ctrls?
//high scores?
//eventually, perhaps read char by char instead of line by line
//and regroup the main configs with {}, and dont care about whitespace?

typedef struct high_scores{
    int easy;
    int norm;
    int hard;
} High_Scores;

void reset_buffer(char *buffer, int size);
//returns a string to the path where the .config folder is located
const char *create_config_folder();
//returns high_Scores from the .score file
High_Scores check_score(const char *pathname);
int save_highscores(High_Scores *old, High_Scores *update, const char *pathname);
void print_scores(High_Scores *scores);

#endif