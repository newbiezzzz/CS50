#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool traceback(int target, int current);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    // Loop thru ranks
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    // Loop thru preferences
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                // preference[0][1] = 3, preference[1][0] = 1
                // winner = 0 , loser = 1
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count = pair_count + 1;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count = pair_count + 1;
            }

            if (pair_count >= (MAX * (MAX - 1) / 2))
            {
                return;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO

    for (int i = 0; i < pair_count; i++)
    {
        int win_votes_i = preferences[pairs[i].winner][pairs[i].loser];
        int lose_votes_i = preferences[pairs[i].loser][pairs[i].winner];
        int strength_i = win_votes_i - lose_votes_i;

        int max_strength_j = strength_i;
        int max_strength_idx = i;
        for (int j = i + 1; j < pair_count; j++)
        {
            int win_votes_j = preferences[pairs[j].winner][pairs[j].loser];
            int lose_votes_j = preferences[pairs[j].loser][pairs[j].winner];
            int strength_j = win_votes_j - lose_votes_j;

            if (strength_j > max_strength_j)
            {
                max_strength_j = strength_j;
                max_strength_idx = j;
            }
        }
        // Selection sort
        pair temp = pairs[i];
        pairs[i] = pairs[max_strength_idx];
        pairs[max_strength_idx] = temp;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO

    for (int i = 0; i < pair_count; i++)
    {
        if (!traceback(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}

// target is winner, current is loser
bool traceback(int target, int current)
{
    if (current == target)
    {
        return true; // Cycle detected
    }

    for (int i = 0; i < candidate_count; i++)
    {
        // if first_loser or source has chain, then traceback again
        if (locked[current][i] == true)
        {
            // This is important for child function to return true
            if (traceback(target, i) == true)
            {
                return true;
            }
        }
    }

    return false; // No cycle detected
}

// Print the winner of the election
void print_winner(void)
{
    // TODO

    // Check col by col
    for (int i = 0; i < candidate_count; i++)
    {
        int flag = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                flag = 1;
            }
        }

        if (flag == 0)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}
