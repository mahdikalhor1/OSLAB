#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define CUSTOMERS_NUMBER 5
#define TRANSACTIONS_NUMBER 10

int RESOURCES_NUMBER;
pthread_mutex_t banker;

int *available;
int **maximum;
int **need;
int **allocation;

int is_safe(void);
int request_resources(int customer_num, int request[RESOURCES_NUMBER]);
int release_resources(int customer_num, int request[RESOURCES_NUMBER]);
int loan_permision(int customer_num);
void *customer(void *id);

void cleanup()
{
    pthread_mutex_destroy(&banker);
    free(available);

    for (int i = 0; i < CUSTOMERS_NUMBER; i++)
    {
        free(maximum[i]);
        free(need[i]);
        free(allocation[i]);
    }

    free(maximum);
    free(need);
    free(allocation);
}

int main(int argc, char **argv)
{
    srand(time(0));

    if (pthread_mutex_init(&banker, NULL) != 0)
    {
        perror("Mutex initialization failed");
        return 1;
    }

    RESOURCES_NUMBER = argc - 1;

    if (RESOURCES_NUMBER == 0)
    {
        printf("No resources given!\n");
        cleanup();
        return 1;
    }

    available = malloc(sizeof(int) * RESOURCES_NUMBER);
    maximum = malloc(CUSTOMERS_NUMBER * sizeof(*maximum));
    need = malloc(CUSTOMERS_NUMBER * sizeof(*need));
    allocation = malloc(CUSTOMERS_NUMBER * sizeof(*allocation));

    for (int i = 0; i < CUSTOMERS_NUMBER; i++)
    {
        maximum[i] = malloc(RESOURCES_NUMBER * sizeof(int));
        need[i] = malloc(RESOURCES_NUMBER * sizeof(int));
        allocation[i] = malloc(RESOURCES_NUMBER * sizeof(int));
    }

    for (int i = 0; i < RESOURCES_NUMBER; i++)
    {
        available[i] = strtol(argv[i + 1], NULL, 10);
    }

    for (int i = 0; i < CUSTOMERS_NUMBER; i++)
    {
        for (int j = 0; j < RESOURCES_NUMBER; j++)
        {
            allocation[i][j] = 0;
            maximum[i][j] = 1 + rand() % (available[j]);
            need[i][j] = maximum[i][j];
        }
    }

    pthread_t id[CUSTOMERS_NUMBER];
    int *pid = malloc(sizeof(int) * CUSTOMERS_NUMBER);

    for (int i = 0; i < CUSTOMERS_NUMBER; i++)
    {
        *(pid + i) = i;
        pthread_create(&(id[i]), NULL, &customer, (pid + i));
    }

    for (int i = 0; i < CUSTOMERS_NUMBER; i++)
    {
        pthread_join(id[i], NULL);
    }

    cleanup();
    return 0;
}

void *customer(void *id)
{
    int customer = *(int *)id;
    int transaction_counter = 0;

    while (transaction_counter < TRANSACTIONS_NUMBER)
    {
        if (loan_permision(customer) == 0)
        {
            int release_request[RESOURCES_NUMBER];
            int flag = 0;

            for (int i = 0; i < RESOURCES_NUMBER; i++)
            {
                release_request[i] = (rand() % (allocation[customer][i] + 1));
                if (release_request[i] != 0)
                    flag = 1;
            }

            if (flag == 1)
            {
                release_resources(customer, release_request);
            }
        }
        else
        {
            int request[RESOURCES_NUMBER];
            int request_kind = (rand() % 3);

            if (request_kind > 0)
            {
                int flag = 0;

                for (int i = 0; i < RESOURCES_NUMBER; i++)
                {
                    request[i] = (rand() % (need[customer][i] + 1));
                    if (request[i] != 0)
                        flag = 1;
                }

                if (flag == 1)
                {
                    if (request_resources(customer, request) != 0)
                    {
                        continue;
                    }
                }
            }
        }
        transaction_counter++;
    }

    if (loan_permision(customer) == 0)
    {
        int release_request[RESOURCES_NUMBER];
        int flag = 0;

        for (int i = 0; i < RESOURCES_NUMBER; i++)
        {
            release_request[i] = (rand() % (allocation[customer][i] + 1));
            if (release_request[i] != 0)
                flag = 1;
        }

        if (flag == 1)
            release_resources(customer, release_request);
    }

    printf("**customer%d had %d transactions**\n", customer, TRANSACTIONS_NUMBER);
    return 0;
}

int loan_permision(int customer_num)
{
    for (int i = 0; i < RESOURCES_NUMBER; i++)
    {
        if (need[customer_num][i] != 0)
        {
            return 1;
        }
    }

    return 0;
}

int release_resources(int customer_num, int request[RESOURCES_NUMBER])
{
    pthread_mutex_lock(&banker);

    for (int i = 0; i < RESOURCES_NUMBER; i++)
    {
        available[i] += request[i];
        allocation[customer_num][i] -= request[i];
        need[customer_num][i] += request[i];
    }

    printf("Released  ");

    for (int i = 0; i < RESOURCES_NUMBER; i++)
    {
        printf("%2d ", request[i]);
    }

    printf("from customer %d ", customer_num);
    printf("available: ");

    for (int i = 0; i < RESOURCES_NUMBER; i++)
    {
        printf("%2d ", available[i]);
    }

    printf("\n");

    pthread_mutex_unlock(&banker);
    return 0;
}

int request_resources(int customer_num, int request[RESOURCES_NUMBER])
{
    for (int i = 0; i < RESOURCES_NUMBER; i++)
    {
        if (request[i] > need[customer_num][i])
            return -1;
    }

    pthread_mutex_lock(&banker);

    for (int i = 0; i < RESOURCES_NUMBER; i++)
    {
        if (request[i] > available[i])
        {
            pthread_mutex_unlock(&banker);
            return -1;
        }
    }

    for (int i = 0; i < RESOURCES_NUMBER; i++)
    {
        available[i] -= request[i];
        allocation[customer_num][i] += request[i];
        need[customer_num][i] -= request[i];
    }

    if (!is_safe())
    {
        for (int i = 0; i < RESOURCES_NUMBER; i++)
        {
            available[i] += request[i];
            allocation[customer_num][i] -= request[i];
            need[customer_num][i] += request[i];
        }

        pthread_mutex_unlock(&banker);
        return -1;
    }

    printf("Allocated ");

    for (int i = 0; i < RESOURCES_NUMBER; i++)
    {
        printf("%2d ", request[i]);
    }

    printf("to customer %d  ", customer_num);
    printf(" available: ");

    for (int i = 0; i < RESOURCES_NUMBER; i++)
    {
        printf("%2d ", available[i]);
    }

    printf("\nneed: ");

    for (int i = 0; i < RESOURCES_NUMBER; i++)
    {
        printf("%2d ", need[customer_num][i]);
    }

    printf("\n");

    pthread_mutex_unlock(&banker);
    return 0;
}

int is_safe()
{
    int work[RESOURCES_NUMBER];

    for (int i = 0; i < RESOURCES_NUMBER; i++)
    {
        work[i] = available[i];
    }

    int finish[CUSTOMERS_NUMBER];

    for (int i = 0; i < CUSTOMERS_NUMBER; i++)
    {
        finish[i] = 0;
    }

    for (int j = 0; j < CUSTOMERS_NUMBER; j++)
    {
        if (finish[j] != 0)
        {
            continue;
        }

        int enough_resource = 1;

        for (int i = 0; i < RESOURCES_NUMBER; i++)
        {
            if (need[j][i] > work[i])
            {
                enough_resource = 0;
                break;
            }
        }

        if (enough_resource)
        {
            for (int i = 0; i < RESOURCES_NUMBER; i++)
            {
                work[i] += allocation[j][i];
            }

            finish[j] = 1;
        }
    }

    for (int j = 0; j < CUSTOMERS_NUMBER; j++)
    {
        if (finish[j] == 0)
        {
            return 0;
        }
    }

    return 1;
}