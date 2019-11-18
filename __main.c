#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    Definiciones para facilidades a la hora
    de programar
*/
#define forr(i, a, b) for (int i = a; i < b; i += 1)
#define forn(i, n) forr(i, 0, n)
#define MAX_STR_LENGTH 200
typedef unsigned long ul;

/*
    Estructura utilizada para representar
    a las personas en el problema
*/
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

/*
    Toma 3 archivos:
        1 - Archivo de ciudades
        2 - Archivo de personas
        3 - Archivo de salida

    Luego pide un numero de personas deseada, digamos x
    y en el archivo de salida escribe x personas tomadas
    aleatoriamente del archivo de personas, con formato

    NOMBRE,APELLIDO,CIUDAD,EDAD,GENERO,GENERO_DE_INTERES

    La ciudad será el nombre que le corresponda al código
    escrito en el archivo de personas. Al igual que el
    género y el género de interes.

    Ejemplos de los archivos de entrada se pueden encontrar
    en la carpeta db
*/
int main(int argc, char *argv[])
{
    //Checkeamos recibir los parametros correctamente
    if (argc != 4)
    {
        printf("---------------------------------------\n\n");
        printf("Error, ingrese el path de los archivos:\n\n");
        printf("1 - Archivo de ciudades\n");
        printf("2 - Archivo de personas\n");
        printf("3 - Archivo de salida\n\n");
        printf("Ejemplo:\n\n");
        printf("$ gcc __main.c\n");
        printf("$ fileName db/codigoLocalidades.txt db/personas.txt out/out.txt\n\n");
        printf("---------------------------------------\n");
        return -1;
    }

    //Abrimos los archivos correspondientes
    char *PATH_CITIES = argv[1];
    char *PATH_PEOPLE = argv[2];
    char *PATH_OUT = argv[3];

    FILE *fCities = fopen(PATH_CITIES, "r");
    FILE *fPeople = fopen(PATH_PEOPLE, "r");
    FILE *fOut = fopen(PATH_OUT, "w+");
    FILE *files[] = {fCities, fPeople, fOut};

    //Si no se puede abrir algún archivo, cortamos la ejecución
    if (checkFiles(files, (int)(sizeof(files) / sizeof(files[0]))) == -1)
        return -1;
    ///////////////////////////////////////////////////

    //Arreglos que nos serviran para covertir los codigos de genero
    //al caracter correspondiente
    char GENDERS[] = {'M', 'F'};
    char IGENDERS[] = {'F', 'M', 'A', 'N'};
    ///////////////////////////////////////////////////

    //Obtenemos la cantidad de personas
    ul requestedP; // unsigned long <=> ul
    do
    {
        printf("Ingrese la cantidad de personas que desea obtener: ");
        scanf("%lu", &requestedP);
    } while (requestedP < 1);
    ///////////////////////////////////////////////////

    ul *selected = (ul *)malloc(sizeof(ul) * requestedP); //Array para saber que personas fueron elegidas
    forn(i, requestedP) selected[i] = 0;

    struct Person people[requestedP];
    ul max_city = 0;
    ul lengthPeople = 0; //Tendrá el tamaño 'util' es decir donde hay personas ya seteadas de @people
    ul currentLine = 0;
    char line[MAX_STR_LENGTH]; //buffer para lecutura

    //Hacemos la primer entrada de las personas, tomando de forma aleatoria
    while (fgets(line, MAX_STR_LENGTH - 1, fPeople) != NULL && lengthPeople < requestedP)
    {
        currentLine++;
        if (rand() % 100 > 40) //" '40%' de probabilidad de entrar";
            continue;

        loadPerson(line, &people[lengthPeople], GENDERS, IGENDERS);

        //guardaremos cual es la ciudad con indice mas alto, para luego
        //leer el archivo de ciudades hasta este punto
        max_city = people[lengthPeople].localCode > max_city ? people[lengthPeople].localCode : max_city;

        selected[lengthPeople] = currentLine;
        lengthPeople += 1;
    }
    ///////////////////////////////////////////////////

    long sizeFile = currentLine; //al recorrer una vez, ya sabemos el tamaño del archivo
    /*
        A este punto, puede que hayamos obtenido todas las personas
        pero también es posible que no, lo sabremos si @differ es positivo
    */
    long differ = (long)requestedP - (long)lengthPeople;

    if (differ > 0)
    {
        /*
            En caso de faltar personas, ahora que sabemos la
            cantidad de personas que tiene el archivo y la cantidad
            de personas que faltan, generaremos un array de indices
            únicos (de forma aleatoria), sobre las personas que no 
            hayan sido seleccionadas,de tamaño exactamente |differ| 
            (las que faltan) para que al recorrer el archivo, 
            tomemos exactamente esta cantidadde personas. 
            De esta forma podemos, recorriendo solo una vez mas,
            tomar de forma aleatoria todas las personas requeridas
        */
        ul *randoms = (ul *)malloc(sizeof(ul) * differ);
        quickSort(selected, 0, requestedP - 1);
        getNRandoms(sizeFile, randoms, selected, differ, requestedP);

        //volvemos al inicio del archivo
        fclose(fPeople);
        fPeople = fopen(PATH_PEOPLE, "r");

        currentLine = 0;
        ul index = 0;

        //Ahora obtenemos las personas faltantes
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
        ///////////////////////////////////////////////////
    }

    //coregimos el indice de las ciudades, para el tamaño del array
    max_city++;
    char cities[max_city][MAX_STR_LENGTH];
    unsigned long lengthCities = 0;

    //Obtenemos las ciudades hasta @max_city y las guardamos en @cities
    while (fgets(line, MAX_STR_LENGTH - 1, fCities) != NULL && lengthCities < max_city)
    {
        trim(line, NULL);
        chop(line);
        strcpy(cities[lengthCities], line);
        lengthCities += 1;
    }
    ///////////////////////////////////////////////////

    //Escribimos la salida en el archivo
    forn(i, requestedP)
    {
        fprintf(fOut, "%s,%s,%s,%d,%c,%c\n",
                people[i].name, people[i].lastName, cities[people[i].localCode], people[i].age, people[i].gen, people[i].igen);
    }
    ///////////////////////////////////////////////////

    //Cerramos los archivos
    fclose(fCities);
    fclose(fPeople);
    fclose(fOut);
    ///////////////////////////////////////////////////
    return 0;
}

/************************ STRING UTILITIES ****************************/

/*
    Toma una string(array de char) y elimina
    los caracteres: en @seps o los siguientes si
    seps es NULL:
    \t\n\v\f\r (incluido espacios en blancos)
    del lado izquierdo de la string
*/
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

/*
    Toma una string(array de char) y elimina
    los caracteres: en @seps o los siguientes si
    seps es NULL:
    \t\n\v\f\r (incluido espacios en blancos)
    del lado derecho de la string
*/
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

/*
    Toma una string(array de char) y elimina
    los caracteres: en @seps o los siguientes si
    seps es NULL:
    \t\n\v\f\r (incluido espacios en blancos)
    del lado derecho e izquierdo de la string
*/
void trim(char *str, const char *seps)
{
    rtrim(str, seps);
    ltrim(str, seps);
}

/************************* AUX. FUNCTIONS *****************************/

/*
    Toma una string(array de char) y elimina
    todos los caracteres hasta la primer coma
    encontrada (de izquierda a derecha) con
    coma incluida, o toda la string en caso de
    no tener una coma
*/
void chop(char *str)
{
    int i = 0;
    int len = strlen(str);
    while (str[i] != ',' && i < len)
        i++;
    memmove(str, str + i + 1, len - i + 1);
}

/*
    Toma una string(array de char) y devuelve
    una substring hasta la primer coma encontrada
    (o toda la string en caso de no tener una)
    y además elimina esta substring (con la coma)
    de la string @str
*/
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

/*
    Toma dos direcciones de memoria de unsigned long
    e intercambia los valores que se encuentran en ellas
*/
void swap(ul *a, ul *b)
{
    ul t = *a;
    *a = *b;
    *b = t;
}

/*
    Toma el ultimo elemento como pivote, lo posiciona 
    en el lugar correcto y ubica todos los elementos
    mas chicos a la izquierda y todos los mas grandes
    a la derecha del pivote. Devuelve la posición del
    pivote
*/
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

/*
    Toma un array de ul, el incio (@low) y el fin (@high)
    y lo ordena de menor a mayor
*/
void quickSort(ul arr[], int low, int high)
{
    if (low < high)
    {
        ul pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

/*
    Toma un array, su tamaño (@n) y un valor a buscar (@data)
    y devuelve el indice donde se encuentra dicho valor
    en caso de no estar en el array, devuelve -1
*/
int binarySearch(ul a[], ul n, ul data)
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

/*
    Escribe en el array @a y en el array @r, numeros aleatorios
    que no se encuentren previamente en @r,
    los numeros aleatorios van de 1 a @max
*/
void getNRandoms(ul max, ul a[], ul r[], ul sizeA, ul sizeR)
{
    ul new;

    forn(i, sizeA) a[i] = 0;
    forn(i, sizeA)
    {
        do
        {
            new = rand() % max + 1;
        } while (binarySearch(r, sizeR, new) != -1);
        a[i] = new;
        r[0] = new;
        quickSort(r, 0, sizeR);
    }
    quickSort(a, 0, sizeA - 1);
}

/*
    Recibe una linea del archivo de entrada
    y carga los datos en @p, siendo los datos
    Nombre
    Apellido
    Ciudad (código numerico)
    Edad
    Género (char correspondiente)
    Género de interés (char correspondiente)
*/
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

    sscanf(localCode, "%lu", &pre_localCode);
    sscanf(age, "%lu", &pre_age);
    sscanf(gen, "%lu", &pre_gen);
    sscanf(igen, "%lu", &pre_igen);

    p->name = name;
    p->lastName = lastName;
    p->localCode = pre_localCode - 1;
    p->age = pre_age;
    p->gen = GENDERS[pre_gen - 1];
    p->igen = IGENDERS[pre_igen - 1];
}

/*
    Recibe un array de FILE* y chequea que no
    sean NULL, si alguno lo es, devuelve -1
    en caso contrario, devuelve 0
*/
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
