#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define forr(i, a, b) for (int i = a; i < b; i++)
#define forn(i, n) forr(i, 0, n)
#define MAX_CITIES 2134
#define MAX_PEOPLE 10000
#define MAX_STR_LENGTH 200
#define PATH_CITIES "db/codigoLocalidades.txt"
#define PATH_PEOPLE "db/personas.txt"

void ltrim(char *str, const char *seps);
void rtrim(char *str, const char *seps);
void trim(char *str, const char *seps);
void chop(char *str);
void strChunk(char *output, char *str);

struct Person
{
    char *name;
    char *lastName;
    char *localCode;
    char gen;  //own gender
    char igen; // interest gender
};

int main()
{
    FILE *fCities = fopen(PATH_CITIES, "r");
    FILE *fPeople = fopen(PATH_PEOPLE, "r");
    char GENDERS[] = {'M', 'F'};
    char IGENDERS[] = {'F', 'M', 'A', 'N'};

    if (fCities == NULL)
    {
        perror(strcat("Error opening: ", PATH_CITIES));
        return -1;
    }

    if (fCities == NULL)
    {
        perror(strcat("Error opening: ", PATH_PEOPLE));
        return -1;
    }

    char cities[MAX_CITIES][MAX_STR_LENGTH];
    int lengthCities = 0;
    char line[MAX_STR_LENGTH];
    while (fgets(line, MAX_STR_LENGTH - 1, fCities) != NULL && lengthCities < MAX_CITIES)
    {
        trim(line, NULL);
        chop(line);
        strcpy(cities[lengthCities], line);
        lengthCities += 1;
    }

    struct Person people[MAX_PEOPLE];
    int lengthPeople = 0;
    char *localCode = "", *gen = "", *igen = "";
    int pre_localCode = 0, pre_gen = 0, pre_igen = 0;
    while (fgets(line, MAX_STR_LENGTH - 1, fPeople) != NULL && lengthPeople < MAX_PEOPLE)
    {
        trim(line, NULL);
        strChunk(people[lengthPeople].name, line);
        strChunk(people[lengthPeople].lastName, line);
        
        trim(people[lengthPeople].name, NULL);
        trim(people[lengthPeople].lastName, NULL);
		
		strChunk(localCode, line);
        strChunk(gen, line);
        strChunk(igen, line);

        sscanf(localCode, "%d", &pre_localCode);
        sscanf(gen, "%d", &pre_gen);
        sscanf(igen, "%d", &pre_igen);

        people[lengthPeople].localCode = cities[pre_localCode - 1];
        people[lengthPeople].gen = GENDERS[pre_gen - 1];
        people[lengthPeople].igen = IGENDERS[pre_igen - 1];

        lengthPeople += 1;
    }

    forn(i, lengthPeople)
    {
        struct Person x = people[i];
        printf("Name: %s, lastName: %s, city: %s, gender: %c, igender: %c\n", x.name, x.lastName, x.localCode, x.gen, x.igen);
    }
    fclose(fCities);
    fclose(fPeople);
    return 0;
}

/************************ STRING UTILITIES ****************************/

void ltrim(char *str, const char *seps)
{
    size_t totrim;
    if (seps == NULL)
    {
        seps = "\t\n\v\f\r ";
    }
    totrim = strspn(str, seps);
    if (totrim > 0)
    {
        size_t len = strlen(str);
        if (totrim == len)
        {
            str[0] = '\0';
        }
        else
        {
            memmove(str, str + totrim, len + 1 - totrim);
        }
    }
}

void rtrim(char *str, const char *seps)
{
    int i;
    if (seps == NULL)
    {
        seps = "\t\n\v\f\r ";
    }
    i = strlen(str) - 1;
    while (i >= 0 && strchr(seps, str[i]) != NULL)
    {
        str[i] = '\0';
        i--;
    }
}

void trim(char *str, const char *seps)
{
    rtrim(str, seps);
    ltrim(str, seps);
}

/************************* AUX. FUNCTIONS *****************************/

void chop(char *str)
{
    int i = 0;
    int len = strlen(str);
    while (str[i] != ',' && i < len)
        i++;
    memmove(str, str + i + 1, len - i + 1);
}

void strChunk(char *output, char *str){
	int i = 0;
    int len = strlen(str);
    while (str[i] != ',' && i < len)
        i++;
    strncpy(output, str, i);
    chop(str);
}