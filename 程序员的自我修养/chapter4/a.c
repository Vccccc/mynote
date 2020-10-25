/* a.c */
extern int shared;
extern int swap;

int main()
{
    int a = 100;
    swap(&a, &shared);
}
