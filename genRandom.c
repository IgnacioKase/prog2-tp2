#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define forr(i, a, b) for (int i = a; i < b; i+=1)
#define forn(i, n) forr(i, 0, n)

typedef unsigned long ul;

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


int main(){
	
	ul sizeFile = 10;
	long differ = 7 - 3;
	ul selected[] = {6,1,3,0,0,0,0};
	ul *randoms = (ul *)malloc(sizeof(ul) * differ);
	quickSort(selected, 0, 7 - 1);
	
	getNRandoms(sizeFile, randoms, selected, differ, 7);

	for (int i = 0; i < differ; i++)
	{
		printf("%d\t%lu\n", i, randoms[i]);
	}
	return 0;
}
