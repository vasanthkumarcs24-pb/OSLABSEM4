#include <stdio.h>

void p(int a[], int ps[], int n)
{
    printf("Process No.\tProcess Size\tBlock No.\n");

    for(int i = 0; i < n; i++)
    {
        printf("%d\t\t%d\t\t", i + 1, ps[i]);

        if(a[i] != -1)
            printf("%d\n", a[i] + 1);
        else
            printf("Not Allocated\n");
    }
}

void ff(int b[], int nb, int ps[], int np)
{
    int a[10];

    for(int i = 0; i < np; i++)
        a[i] = -1;

    for(int i = 0; i < np; i++)
    {
        for(int j = 0; j < nb; j++)
        {
            if(b[j] >= ps[i])
            {
                a[i] = j;
                b[j] = 0 ;
                break;
            }
        }
    }

    printf("\n--- First Fit ---\n");
    p(a, ps, np);
}

void bf(int b[], int nb, int ps[], int np)

{
    int a[10];

    for(int i = 0; i < np; i++)
        a[i] = -1;

    for(int i = 0; i < np; i++)
    {
        int bi = -1;

        for(int j = 0; j < nb; j++)
        {
            if(b[j] >= ps[i])
            {
                if(bi == -1 || b[j] < b[bi])
                    bi = j;
            }
        }

        if(bi != -1)
        {
            a[i] = bi;
            b[bi] = 0 ;
        }
    }

    printf("\n--- Best Fit ---\n");
    p(a, ps, np);
}

void wf(int b[], int nb, int ps[], int np)
{
    int a[10];

    for(int i = 0; i < np; i++)
        a[i] = -1;

    for(int i = 0; i < np; i++)
    {
        int wi = -1;

        for(int j = 0; j < nb; j++)
        {
            if(b[j] >= ps[i])
            {
                if(wi == -1 || b[j] > b[wi])
                    wi = j;
            }
        }

        if(wi != -1)
        {
            a[i] = wi;
            b[wi] = 0 ;
        }
    }

    printf("\n--- Worst Fit ---\n");
    p(a, ps, np);
}

int main()
{
    int nb, np;

    printf("Enter number of memory blocks: ");
    scanf("%d", &nb);

    int b[10];

    printf("Enter size of memory blocks:\n");
    for(int i = 0; i < nb; i++)
        scanf("%d", &b[i]);

    printf("Enter number of processes: ");
    scanf("%d", &np);

    int ps[10];

    printf("Enter size of processes:\n");
    for(int i = 0; i < np; i++)
        scanf("%d", &ps[i]);

    int b1[10], b2[10], b3[10];

    for(int i = 0; i < nb; i++)
    {
        b1[i] = b[i];
        b2[i] = b[i];
        b3[i] = b[i];
    }

    ff(b1, nb, ps, np);
    bf(b2, nb, ps, np);
    wf(b3, nb, ps, np);

    return 0;
}
