#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define forr(i, a, b) for (int i = a; i < b; i+=1)
#define forn(i, n) forr(i, 0, n)
#define MAX_CITIES 2134
#define MAX_PEOPLE 100000
#define MAX_STR_LENGTH 200
#define PATH_CITIES "db/codigoLocalidades.txt"
#define PATH_PEOPLE "db/personas.txt"
#define PATH_OUT "out/out.txt"
typedef unsigned long ul;

void ltrim(char *str, const char *seps);
void rtrim(char *str, const char *seps);
void trim(char *str, const char *seps);
void chop(char *str);
char *strChunk(char *str);
void getNRandoms(ul n, ul max, ul r[]);
void swap(ul* a, ul* b);
int partition (ul arr[], int low, int high);
void quickSort(ul arr[], int low, int high);

struct Person
{
    char *name;
    char *lastName;
    int localCode;
    int age;
    char gen;  //own gender
    char igen; // interest gender
};

int main()
{
    FILE *fCities = fopen(PATH_CITIES, "r");
    FILE *fPeople = fopen(PATH_PEOPLE, "r");
    FILE *fOut = fopen(PATH_OUT, "w+");
    
    char GENDERS[] = {'M', 'F'};
    char IGENDERS[] = {'F', 'M', 'A', 'N'};

    if (fCities == NULL)
    {
        perror(strcat("Error opening: ", PATH_CITIES));
        return -1;
    }

    if (fPeople == NULL)
    {
        perror(strcat("Error opening: ", PATH_PEOPLE));
        return -1;
    }

	if (fOut == NULL)
    {
        perror(strcat("Error opening: ", PATH_OUT));
        return -1;
    }
    
    ul n; // unsigned long <=> ul
	do{
		printf("Ingrese la cantidad de personas que desea obtener (max: %d): ", MAX_PEOPLE);
		scanf("%lu", &n);
	}while(n > MAX_PEOPLE);
	
	ul *r = (ul *)malloc(sizeof(ul)*n);
	getNRandoms(n, MAX_PEOPLE, r);
	
    char cities[n][MAX_STR_LENGTH];
    unsigned long lengthCities = 0;
    char line[MAX_STR_LENGTH];
    
    while (fgets(line, MAX_STR_LENGTH - 1, fCities) != NULL && lengthCities < n)
    {	
        trim(line, NULL);
        chop(line);
        strcpy(cities[lengthCities], line);
        lengthCities += 1;
    }

    struct Person people[MAX_PEOPLE];
    int lengthPeople = 0;
    int pre_localCode = 0, pre_gen = 0, pre_igen = 0, pre_age = 0, currentLine = 0;
    while (fgets(line, MAX_STR_LENGTH - 1, fPeople) != NULL && lengthPeople < MAX_PEOPLE)
    {
		currentLine += 1;
		if((currentLine-1) != r[lengthPeople])
			continue;
        trim(line, NULL);
        
        char *name = strChunk(line);
        char *lastName = strChunk(line);
		char *localCode = strChunk(line);
		char *age = strChunk(line);
        char *gen = strChunk(line);
        char *igen =strChunk(line);

        trim(name, NULL);
        trim(lastName, NULL);
		
		//printf("name: %s, lastName: %s, localCode: %s, age: %s,gen: %s, igen: %s\n", name, lastName, localCode, age,gen, igen);


        sscanf(localCode, "%d", &pre_localCode);
        sscanf(age, "%d", &pre_age);
        sscanf(gen, "%d", &pre_gen);
        sscanf(igen, "%d", &pre_igen);
        
        //printf("name: %s, lastName: %s, localCode: %s, gen: %c, igen: %c\n", name, lastName, cities[pre_localCode - 1], GENDERS[pre_gen - 1], IGENDERS[pre_igen - 1]);

		printf("%d", pre_localCode);
        people[lengthPeople].name = name;
        people[lengthPeople].lastName = lastName;
        people[lengthPeople].localCode = pre_localCode - 1;
        people[lengthPeople].age = pre_age;
        people[lengthPeople].gen = GENDERS[pre_gen - 1];
        people[lengthPeople].igen = IGENDERS[pre_igen - 1];
		
        lengthPeople += 1;
    }
	 
    forn(i, n){
		printf("%s", cities[people[r[i]].localCode]);
        fprintf(fOut, "Name: %s, lastName: %s, city: %s, age: %d,gender: %c, igender: %c\n",
        people[r[i]].name, people[r[i]].lastName, cities[people[r[i]].localCode], people[r[i]].age, people[r[i]].gen, people[r[i]].igen);
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

char *strChunk(char *str){
	int n = 0;
    int len = strlen(str);
    while (str[n] != ',' && n < len)
        n++;
    char *output = (char *)malloc(sizeof(char)*(n+1));
    //strncpy(output, str, i); Unespected behavior
    forn(i,n) output[i] = str[i];
    output[n] = '\0';
    chop(str);
    return output;
}

void swap(ul* a, ul* b)  
{  
    ul t = *a;  
    *a = *b;  
    *b = t;  
}  
  
int partition (ul arr[], int low, int high)  
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
  
int search(ul a[], ul n, ul data) {
   forn(i,n)
	   if(a[i] == data)
		return 1;
	return -1;
}

/*
void getNRandoms(ul n, ul max, ul r[]){
	ul new;
	forn(i,n) r[i] = 0;
	forn(i,n){
		do{
			new = rand() % max;
		}while(search(r,n,new) != -1);	
		r[i] = new;
	}
	quickSort(r,0,n-1);
}*/

void getNRandoms(ul n, ul max, ul r[]){
	ul new;
	char p[max];
	forn(i,n) r[i] = 0;
	forn(i,max) p[i] = 0;
	forn(i,n){
		do{
			new = rand() % max;
		}while(search(r,n,new) != -1);	
		r[i] = new;
	}
	quickSort(r,0,n-1);
}

