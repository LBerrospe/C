#include <stdio.h>

void selectionSort( int array[], int lengthArray ) {
	int i, j, iMin, temp;
	for (i = 0; i < lengthArray - 1; i++) {
		iMin = i;
		for ( j = i+1; j < lengthArray; j++) {
			if (array[j] < array[iMin]) {
			iMin = j;
			}//if
		}//for
	temp = array[i];
        array[i] = array[iMin];
        array[iMin] = temp;
	}//for
}//selectionSort

void bubbleSort( int array[], int lengthArray ) {
	int i, j, temp;
	for ( i = 1; i < lengthArray; i++) {
		for ( j = 0; j < lengthArray-i; j++) {
		    if ( array[j] > array[j+1]) {
                	temp = array[j];
                	array[j] = array[j+1];
                	array[j+1] = temp;
		    }//if
		}//for
	}//for
}//BubbleSort

void insertionSort( int array[], int lengthArray ) {
    int i,hole,value;
    for ( i = 1; i < lengthArray; i++  ) {
        value = array[i];
        hole = i;
        while (hole > 0 && array[hole-1] > value) {
            array[hole] = array[hole-1];
            hole = hole-1;
        }//while
        array[hole]=value;
    }//for
}//insertionSort

void merge(int arrayLeft[], int arrayLeftLength, int arrayRight[], int arrayRightLength, int array[]) {
    int i=0, j=0, k=0;
    while (i < arrayLeftLength && j < arrayRightLength) {
        if ( arrayLeft[i] <= arrayRight[j]) {
            array[k]=arrayLeft[i];
            i++;
        } else {
            array[k]=arrayRight[j];
            j++;
        }//if{}else{}
        k++;
    }//while
    while (i < arrayLeftLength) {
        array[k]=arrayLeft[i];
        i++;
        k++;
    }//while
    while (j < arrayRightLength) {
        array[k]=arrayRight[j];
        j++;
        k++;
    }//while
}//merge

void mergeSort(int array[], int lengthArray) {
    if (lengthArray < 2){
        return;
    }//if
    int mid = lengthArray/2;
    int arrayLeft[mid];
    int arrayRight[lengthArray-mid];
    int i;
    for (i = 0; i < mid; i++) {
        arrayLeft[i] = array[i];
    }//for
    for (i = mid; i < lengthArray; i++) {
        arrayRight[i-mid]=array[i];
    }//for
    mergeSort(arrayLeft,mid);
    mergeSort(arrayRight,lengthArray-mid);
    merge(arrayLeft, mid, arrayRight, lengthArray-mid, array);
}//mergeSort

int partition(int array[], int start, int end) {
    int i,temp;
    int pivot=array[end-1];
    int partitionIndex=start;
    for (i = start; i < end; i++) {
        if (array[i] <= pivot) {
            temp=array[i];
            array[i]=array[partitionIndex];
            array[partitionIndex]=temp;
            partitionIndex++;
        }//if
    }//for
    temp=array[partitionIndex];
    array[partitionIndex]=array[end-1];
    array[end-1]=temp;
    return partitionIndex;
}//partitionIndex()

void quickSort(int array[], int start, int end) {
    if (start < end) {
        int partitionIndex = partition(array, start, end);
        quickSort(array, start, partitionIndex-1);
        quickSort(array, partitionIndex+1, end);
    }//if
}//quickSort()

int main() {
    int array[]={1,2,4,6,3,5,7,8,2,55,22,23,40,1,2,3,5,54,35,25};
    int lengthArray = sizeof(array)/sizeof(array[0]);
    //bubbleSort(array,lengthArray);
    //selectionSort(array,lengthArray);
    //insertionSort(array,lengthArray);
    //mergeSort(array,lengthArray);
    quickSort(array,0,lengthArray);
    int i;
    for ( i = 0; i < lengthArray; i++) {
        printf("[%i]",array[i]);
    }
    return 0;
}//main
