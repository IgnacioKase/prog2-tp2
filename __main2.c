#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define forr(i, a, b) for (int i = a; i < b; i += 1)
#define forn(i, n) forr(i, 0, n)
#define MAX_CITIES 2134
#define MAX_PEOPLE 10000
#define MAX_STR_LENGTH 200
#define PATH_CITIES "db/codigoLocalidades.txt"
#define PATH_PEOPLE "db/personas.txt"
#define PATH_OUT "out/out.txt"
typedef unsigned long ul;

struct Person
{
    char *name;
    char *lastName;
    int localCode;
    int age;
    char gen;  //own gender
    char igen; // interest gender
};

void ltrim(char *str, const char *seps);
void rtrim(char *str, const char *seps);
void trim(char *str, const char *seps);
void chop(char *str);
char *strChunk(char *str);
void getNRandoms(ul max, ul a[], ul r[], ul sizeA, ul sizeR);
void swap(ul *a, ul *b);
int partition(ul arr[], int low, int high);
void quickSort(ul arr[], int low, int high);
void loadPerson(char *line, struct Person *p, char GENDERS[], char IGENDERS[]);
int checkFiles(FILE *files[], int n);

int main(int argc, char *argv[])
{
    FILE *fCities = fopen(PATH_CITIES, "r");
    FILE *fPeople = fopen(PATH_PEOPLE, "r");
    FILE *fOut = fopen(PATH_OUT, "w+");
    FILE *files[] = {fCities, fPeople, fOut};

    char GENDERS[] = {'M', 'F'};
    char IGENDERS[] = {'F', 'M', 'A', 'N'};

    if (checkFiles(files, (int)(sizeof(files) / sizeof(files[0]))) == -1)
        return -1;

    ul requestedP; // unsigned long <=> ul
    do
    {
        printf("Ingrese la cantidad de personas que desea obtener: ");
        scanf("%lu", &requestedP);
    } while (requestedP < 1);

    ul *selected = (ul *)malloc(sizeof(ul) * requestedP);
    forn(i, requestedP) selected[i] = 0;
    struct Person people[requestedP];
    ul max_city = 0;

    ul lengthPeople = 0;
    ul currentLine = 0;
    char line[MAX_STR_LENGTH];
	
    while (fgets(line, MAX_STR_LENGTH - 1, fPeople) != NULL && lengthPeople < requestedP)
    {
        currentLine++;
        if (rand() % 100 > 40) //" '40%' de probabilidad de entrar";
            continue;
		
        loadPerson(line, &people[lengthPeople], GENDERS, IGENDERS);
        
        max_city = people[lengthPeople].localCode > max_city ? people[lengthPeople].localCode : max_city;
		
        selected[lengthPeople] = currentLine;
        lengthPeople += 1;    
    }
	
    long sizeFile = currentLine;
    long differ = (long)requestedP - (long)lengthPeople;
	
    if (differ > 0)
    {
        ul *randoms = (ul *)malloc(sizeof(ul) * differ);
        quickSort(selected, 0, requestedP - 1);
        getNRandoms(sizeFile, randoms, selected, differ, requestedP);

        fclose(fPeople);
        fPeople = fopen(PATH_PEOPLE, "r");

        currentLine = 0;
        ul index = 0;     

        while (fgets(line, MAX_STR_LENGTH - 1, fPeople) != NULL && lengthPeople < requestedP)
        {
            currentLine++;
            if (randoms[index] != currentLine)
                continue;

            loadPerson(line, &people[lengthPeople], GENDERS, IGENDERS);
            max_city = people[lengthPeople].localCode > max_city ? people[lengthPeople].localCode : max_city;

            lengthPeople++;
            index++;
        }
    }
    
	max_city++;
    char cities[max_city][MAX_STR_LENGTH];
    unsigned long lengthCities = 0;

    while (fgets(line, MAX_STR_LENGTH - 1, fCities) != NULL && lengthCities < max_city)
    {
        trim(line, NULL);
        chop(line);
        strcpy(cities[lengthCities], line);
        lengthCities += 1;
    }
   
    forn(i, requestedP)
    {
        fprintf(fOut, "%s,%s,%s,%d,%c,%c\n",
                people[i].name, people[i].lastName, cities[people[i].localCode], people[i].age, people[i].gen, people[i].igen);
    }
    
    fclose(fCities);
    fclose(fPeople);
    fclose(fOut);
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

char *strChunk(char *str)
{
    int n = 0;
    int len = strlen(str);
    while (str[n] != ',' && n < len)
        n++;
    char *output = (char *)malloc(sizeof(char) * (n + 1));
    //strncpy(output, str, i); Unespected behavior
    forn(i, n) output[i] = str[i];
    output[n] = '\0';
    chop(str);
    return output;
}

void swap(ul *a, ul *b)
{
    ul t = *a;
    *a = *b;
    *b = t;
}

int partition(ul arr[], int low, int high)
{
    ul pivot = arr[high];
    ul i = (low - 1);

    for (ul j = low; j <= high - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(ul arr[], int low, int high)
{
    if (low < high)
    {
        ul pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int binary_search(ul a[], ul n, ul data)
{
    ul lowerBound = 0;
    ul upperBound = n - 1;
    ul midPoint = -1;
    ul index = -1;

    while (lowerBound <= upperBound)
    {
        midPoint = lowerBound + (upperBound - lowerBound) / 2;
        if (a[midPoint] == data)
        {
            index = midPoint;
            break;
        }
        else
        {
            if (a[midPoint] < data)
                lowerBound = midPoint + 1;
            else
                upperBound = midPoint - 1;
        }
    }
    return index;
}

void getNRandoms(ul max, ul a[], ul r[], ul sizeA, ul sizeR)
{
    ul new;

    forn(i, sizeA) a[i] = 0;
    forn(i, sizeA)
    {
        do
        {
            new = rand() % max + 1;
        } while (binary_search(r, sizeR, new) != -1);
        a[i] = new;
        r[i] = new;
    }
    quickSort(a, 0, sizeA - 1);
}

void loadPerson(char *line, struct Person *p, char GENDERS[], char IGENDERS[])
{
    ul pre_localCode = 0, pre_gen = 0, pre_igen = 0, pre_age = 0;
    trim(line, NULL);

    char *name = strChunk(line);
    char *lastName = strChunk(line);
    char *localCode = strChunk(line);
    char *age = strChunk(line);
    char *gen = strChunk(line);
    char *igen = strChunk(line);

    trim(name, NULL);
    trim(lastName, NULL);

    //printf("name: %s, lastName: %s, localCode: %s, age: %s,gen: %s, igen: %s\n", name, lastName, localCode, age,gen, igen);

    sscanf(localCode, "%lu", &pre_localCode);
    sscanf(age, "%lu", &pre_age);
    sscanf(gen, "%lu", &pre_gen);
    sscanf(igen, "%lu", &pre_igen);

    //printf("name: %s, lastName: %s, localCode: %s, gen: %c, igen: %c\n", name, lastName, cities[pre_localCode - 1], GENDERS[pre_gen - 1], IGENDERS[pre_igen - 1]);

    p->name = name;
    p->lastName = lastName;
    p->localCode = pre_localCode - 1;
    p->age = pre_age;
    p->gen = GENDERS[pre_gen - 1];
    p->igen = IGENDERS[pre_igen - 1];   
}

int checkFiles(FILE *files[], int n)
{
    forn(i, n)
    {
        if (files[i] == NULL)
        {
            perror("Error opening: a file.");
            return -1;
        }
    }
    return 0;
}
