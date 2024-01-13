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
const char *create_config_folder();
High_Scores check_score();
int save_highscores(High_Scores *old, High_Scores *update, const char *pathname);

#endif