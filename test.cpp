/* 동적할당
c언어는 기본적으로 메모리 할당방식이 두가지로
나뉩니다.
정적할당/ 동적할당입니다.
정적할당은 프로그램이 시작되면 변수의 저장공간을 먼저 할당하고 
프로그램이 종료되면 저장공간을 해제하는 것이고
동적할당은 프로그램이 실행되는 중에 필요한만큼 저장공간을 할당하고
원하는 상황에서 저장공간을 해제하는 것입니다.
동적 할당방법에는 malloc/ calloc/realloc이 있습니다.
malloc 및 calloc은 할당하고자 하는 메모리 크기를 바이트 단위로 할당할 수 있는 함수입니다.
이 두개의 차이는 
malloc 은 할당을 하게되면 값들이 쓰레기값이 들어가있고,
calloc은 값이 0으로 초기화 됩니다.
realloc은 이미 할당되어 있는 메모리의 크기를 바꿀때 사용합니다.
*/
#include<stdio.h>
#include<stdlib.h>
int main()
{
    int num;
    int *arr, *arr2;
    printf("배열 크기 입력:");
    scanf("%d", &num);
 
    arr = (int *)malloc(sizeof(int)*num); //int [10]크기의 배열 동적할당을 합니다.
    arr2 = (int *)calloc(num, sizeof(int)); //int [10]크기의 배열 동적할당을 합니다.
    printf("malloc으로 할당된 크기: %3d\ncalloc으로 할당된 크기: %3d\n\n", _msize(arr) / sizeof(*arr), _msize(arr2) / sizeof(*arr2));// 크기확인을 합니다.
 
    for (int i = 0; i < num; i++)
    {
        printf("malloc 결과: %10d\n", *(arr + i)); // 결과가 쓰레기 값으로 나온것을 알 수 있습니다.
    }
    printf("\n");
 
    for (int i = 0; i < num; i++)
    {
        printf("calloc 결과:%10d\n", *(arr2 + i)); // 결과가 쓰레기 값으로 나온것을 알 수 있습니다.
    }
    printf("\n");
 
    //우리는 동적할당으로 잡은 메모리는 해제 시켜야합니다.
    free(arr);
    free(arr2);
    return 0;
}