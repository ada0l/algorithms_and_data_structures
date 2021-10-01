#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !_WIN32
#include <unistd.h>
#endif

/*
 * Util
 */
int get_count_of_digits(int number)
{
    int count = (number == 0) ? 1 : 0;
    while (number != 0) {
        count++;
        number /= 10;
    }
    return count;
}

bool is_valid_int(char* str)
{
    int len = strlen(str) - 1;
    if (len == 0) {
        return false;
    }
    for (int i = 0; i < len - 1; ++i) {
        if (!('0' <= str[i] && str[i] <= '9')) {
            return false;
        }
    }
    return true;
}

void poka_durak()
{
    printf("\
   ____       U  ___ u    _  __        _                    \n\
 U|  _\"\\ u     \\/\"_ \\/   |\"|/ /    U  /\"\\  u        \n\
 \\| |_) |/     | | | |   | \' /      \\/ _ \\/             \n\
  |  __/   .-,_| |_| | U/| . \\\\u    / ___ \\              \n\
  |_|       \\_)-\\___/    |_|\\_\\    /_/   \\_\\          \n\
  ||>>_          \\\\    ,-,>> \\\\,-.  \\\\    >>          \n\
 (__)__)        (__)    \\.)   (_/  (__)  (__)              \n\
  ____       _   _     ____         _         _  __         \n\
 |  _\"\\   U |\"|u| | U |  _\"\\ u  U  /\"\\  u    |\"|/ / \n\
/| | | |   \\| |\\| |  \\| |_) |/   \\/ _ \\/     | \' /    \n\
U| |_| |\\   | |_| |   |  _ <     / ___ \\   U/| . \\\\u    \n\
 |____/ u  <<\\___/    |_| \\_\\   /_/   \\_\\    |_|\\_\\  \n\
  |||_    (__) )(     //   \\\\_   \\\\    >>  ,-,>> \\\\,-.\n\
 (__)_)       (__)   (__)  (__) (__)  (__)  \\.)   (_/");
}

/*
 * Random
 */

// returns random number bertween a and b, inclusive
int random_get_num_in_range(int start, int end)
{
    return rand() % (end + 1 - start) + start;
}

char random_get_eng_char()
{
    return (char)random_get_num_in_range((int)'a', (int)'z');
}

/*
 * Customer
 */

typedef struct Customer {
    char name;
    int time;
} Customer;

Customer* customer_new(char name, int time)
{
    Customer* customer = (Customer*)malloc(sizeof(Customer));
    customer->name = name;
    customer->time = time;
    return customer;
}

Customer* customer_new_random(int max_customer_time)
{
    return customer_new(random_get_eng_char(),
        random_get_num_in_range(1, max_customer_time));
}

/*
 * Queue
 */

typedef Customer QueueType;

typedef struct QueueNode {
    QueueType* data;
    struct QueueNode *prev, *next;
} QueueNode;

typedef struct Queue {
    int size;
    struct QueueNode *beg, *end;
} Queue;

QueueNode* queue_node_new(
    QueueType* data, QueueNode* prev, QueueNode* next)
{
    QueueNode* result = (QueueNode*)malloc(sizeof(QueueNode));
    result->data = data;
    result->prev = prev;
    result->next = next;
    return result;
}

void queue_node_customer_print(QueueNode* node)
{
    printf("%c%d", node->data->name, node->data->time);
}

QueueNode* queue_node_get_next(QueueNode* node) { return node->next; }

QueueNode* queue_node_get_prev(QueueNode* node) { return node->prev; }

bool queue_node_is_dereferenable(QueueNode* node)
{
    return node != NULL;
}

bool queue_is_node_dereferenable(Queue* queue, QueueNode* node)
{
    return node != NULL && queue->beg != node && queue->end != node;
}

void queue_node_free(QueueNode* node)
{
    free(node->data);
    free(node);
}

Queue* queue_new()
{
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    // There are always two elements in the queue,
    // which explicitly point to the beginning and
    // the end. It seems to me that this is the best
    // solution that simplifies implementation
    queue->beg = queue_node_new(NULL, NULL, NULL);
    queue->end = queue_node_new(NULL, queue->beg, NULL);
    queue->beg->next = queue->end;
    queue->size = 0;
    return queue;
}

QueueNode* queue_get_begin(Queue* queue) { return queue->beg->next; }

QueueNode* queue_get_end(Queue* queue) { return queue->end->prev; }

size_t queue_get_size(Queue* queue) { return queue->size; }

// the code sucks, but it works and there is no memory leak
void queue_push(Queue* queue, QueueType* new_element)
{
    QueueNode* new_node
        = queue_node_new(new_element, queue->beg, queue->beg->next);
    queue->beg->next->prev = new_node;
    queue->beg->next = new_node;
    ++queue->size;
}

QueueType* queue_top(Queue* queue) { return queue->end->prev->data; }

// the code sucks, but it works and there is no memory leak (x3)
QueueType* queue_pop(Queue* queue)
{
    QueueType* tmp = queue_top(queue);
    QueueNode* node_for_free = queue->end->prev;
    queue->end->prev->prev->next = queue->end;
    queue->end->prev = queue->end->prev->prev;
    --queue->size;
    free(node_for_free);
    return tmp;
}

void queue_for_each(Queue* queue, void (*func)(QueueNode* node))
{
    QueueNode* node = queue->beg->next;
    while (node != queue->end) {
        QueueNode* node_for_next = node->next;
        func(node);
        node = node_for_next;
    }
}

void queue_free_without_content(Queue* queue)
{
    free(queue->beg);
    free(queue->end);
    free(queue);
}

void queue_free(Queue* queue)
{
    queue_for_each(queue, queue_node_free);
    queue_free_without_content(queue);
}

Queue* queue_new_random(int max_next_customers, int max_customer_time)
{
    Queue* queue = queue_new();
    int queue_size = random_get_num_in_range(1, max_next_customers);
    for (int i = 0; i < queue_size; ++i) {
        queue_push(queue, customer_new_random(max_customer_time));
    }
    return queue;
}

void queue_customer_print(Queue* queue)
{
    QueueNode* node = queue_get_begin(queue);
    while (queue_is_node_dereferenable(queue, node)) {
        queue_node_customer_print(node);
        printf(" ");
        node = queue_node_get_next(node);
    }
}

/*
 * Cashbox
 */

typedef struct Cashbox {
    bool is_work;
    int num_of_served_customers;
    Queue* queue;
} Cashbox;

Cashbox* cashbox_new()
{
    Cashbox* cashbox = (Cashbox*)malloc(sizeof(Cashbox));
    cashbox->is_work = false;
    cashbox->num_of_served_customers = 0;
    cashbox->queue = queue_new();
    return cashbox;
}

void cashbox_free(Cashbox* cashbox)
{
    queue_free(cashbox->queue);
    free(cashbox);
}

/*
 * Market
 */

typedef struct Market {
    Cashbox** cashboxes;
    int size;
    int max_queue_size;
    int customers_served;
} Market;

Market* market_new(int size, int max_queue_size)
{
    Market* market = (Market*)malloc(sizeof(Market));
    market->size = size;
    market->max_queue_size = max_queue_size;
    market->customers_served = 0;
    market->cashboxes = (Cashbox**)malloc(sizeof(Cashbox*) * size);
    for (int i = 0; i < size; ++i) {
        market->cashboxes[i] = cashbox_new();
    }
    return market;
}

void market_free(Market* market)
{
    for (int i = 0; i < market->size; ++i) {
        cashbox_free(market->cashboxes[i]);
    }
    free(market->cashboxes);
    free(market);
}

void market_serve_current_customers(Market* market)
{
    for (int i = 0; i < market->size; ++i) {
        Cashbox* current_cashbox = market->cashboxes[i];

        // skip if there are no items in the queue
        if (queue_get_size(current_cashbox->queue) <= 0) {
            continue;
        }

        QueueType* first_customer_in_queue
            = queue_top(current_cashbox->queue);

        if (first_customer_in_queue == NULL) {
            continue;
        }

        --first_customer_in_queue->time;

        // If the customer's time is 0, then it is necessary to delete
        // it.
        if (first_customer_in_queue->time <= 0) {
            free(queue_pop(current_cashbox->queue));
            ++market->customers_served;
        }

        // If there are no buyers left, then you need to
        // close the queue.
        if (queue_get_size(current_cashbox->queue) == 0) {
            current_cashbox->is_work = false;
        }
    }
}

void market_add_new_customer(Market* market, Customer* customers)
{
    Cashbox* priority_cashbox = NULL;

    for (int i = 0; i < market->size; ++i) {
        Cashbox* current_cashbox = market->cashboxes[i];

        // skip if the checkout is already full
        if (market->max_queue_size
            <= queue_get_size(current_cashbox->queue)) {
            continue;
        }

        if (priority_cashbox == NULL
            || priority_cashbox->num_of_served_customers
                < current_cashbox->num_of_served_customers) {
            priority_cashbox = current_cashbox;
        }
    }

    if (priority_cashbox != NULL) {
        if (!priority_cashbox->is_work) {
            priority_cashbox->is_work = true;
        }
        queue_push(priority_cashbox->queue, customers);
        ++priority_cashbox->num_of_served_customers;
    }
}

void market_add_new_customers(Market* market, Queue* queue)
{
    while (queue_get_size(queue) > 0) {
        market_add_new_customer(market, queue_pop(queue));
    }
}

void market_display_numbers_header(Market* market)
{
    for (int i = 0; i < market->size; ++i) {
        printf("\t%d", i + 1);
    }
}

void market_display_count_of_customers_header(Market* market)
{
    for (int i = 0; i < market->size; ++i) {
        printf(
            "\t%d", (int)queue_get_size(market->cashboxes[i]->queue));
    }
}

void market_display_queue_is_work_or_not(Market* market)
{
    for (int i = 0; i < market->size; ++i) {
        if (market->cashboxes[i]->is_work) {
            printf("\t+");
        } else {
            printf("\t-");
        }
    }
}

void market_display_customers_in_order(Market* market)
{
    // prepare
    QueueNode** customer_in_current_position
        = (QueueNode**)malloc(sizeof(QueueNode*) * market->size);
    for (int i = 0; i < market->size; ++i) {
        customer_in_current_position[i]
            = queue_get_end(market->cashboxes[i]->queue);
        Queue* queue = market->cashboxes[i]->queue;
    }

    // display customers in orders
    for (int i = 0; i < market->max_queue_size; ++i) {
        for (int j = 0; j < market->size; ++j) {
            QueueNode* current_customer
                = customer_in_current_position[j];
            Queue* current_queue = market->cashboxes[j]->queue;
            if (queue_is_node_dereferenable(
                    current_queue, current_customer)) {
                printf("\t");
                queue_node_customer_print(current_customer);
                customer_in_current_position[j] = queue_node_get_prev(
                    customer_in_current_position[j]);
            } else {
                printf("\t||");
            }
        }
        printf("\n");
    }

    // free
    free(customer_in_current_position);
}

int market_get_the_number_of_working_cashboxes(Market* market)
{
    int working_cashboxes = 0;
    for (int i = 0; i < market->size; ++i) {
        if (market->cashboxes[i]->is_work) {
            ++working_cashboxes;
        }
    }
    return working_cashboxes;
}

int market_get_the_number_of_customers_in_queue(Market* market)
{
    int customers_in_queue = 0;
    for (int i = 0; i < market->size; ++i) {
        if (market->cashboxes[i]->is_work) {
            customers_in_queue += market->cashboxes[i]->queue->size;
        }
    }
    return customers_in_queue;
}

bool market_can_push_queue(Market* market, Queue* queue)
{
    return market_get_the_number_of_customers_in_queue(market)
        + queue_get_size(queue)
        >= market->max_queue_size * market->size;
}

/*
 * Settings
 */

typedef struct Settings {
    int max_customer_time;
    int max_cashier_queue;
    int max_cashiers;
    int max_next_customers;
    bool error;
} Settings;

Settings* settings_new()
{
    Settings* settings = (Settings*)malloc(sizeof(Settings));
    settings->max_customer_time = settings->max_cashier_queue
        = settings->max_cashiers = settings->max_next_customers = -1;
    settings->error = false;
    return settings;
}

void settings_free(Settings* settings) { free(settings); }

enum SETTINGS_VERDICT {
    SETTINGS_OKAY,
    SETTINGS_NOT_ASSIGNED,
    SETTINGS_BAD_VALUE,
    SETTINGS_CANNOT_READ
};

int settings_set_property(Settings* settings, char* line)
{
    int* property = NULL;
    int offset = 0;
    if (strncmp("MAX_CUSTOMER_TIME=", line, 18) == 0) {
        property = &settings->max_customer_time;
        offset = 18;
    } else if (strncmp("MAX_CASHIER_QUEUE=", line, 18) == 0) {
        property = &settings->max_cashier_queue;
        offset = 18;
    } else if (strncmp("MAX_CASHIERS=", line, 13) == 0) {
        property = &settings->max_cashiers;
        offset = 13;
    } else if (strncmp("MAX_NEXT_CUSTOMERS=", line, 19) == 0) {
        property = &settings->max_next_customers;
        offset = 19;
    }
    if (is_valid_int(line + offset)) {
        int value = atoi(line + offset);
        if (value < 0) {
            return SETTINGS_BAD_VALUE;
        }
        *property = value;
        return SETTINGS_OKAY;
    } else {
        return SETTINGS_BAD_VALUE;
    }
}

int settings_read_from_file(Settings* settings, char* settings_path)
{
    FILE* fin = fopen(settings_path, "r");
    if (fin == NULL) {
        settings->error = true;
        return SETTINGS_CANNOT_READ;
    }
    const size_t len_max = 300;
    char* line = (char*)malloc(len_max);
    size_t len = 0;
    while (fgets(line, len_max, fin) != NULL) {
        if (settings_set_property(settings, line)
            == SETTINGS_BAD_VALUE) {
            settings->error = true;
            fclose(fin);
            free(line);
            return SETTINGS_BAD_VALUE;
        }
    }
    fclose(fin);
    if (line) {
        free(line);
    }
    settings->error = false;
    return SETTINGS_OKAY;
}

bool settings_are_all_properties_assigned(Settings* settings)
{
    return settings->max_customer_time > 0
        && settings->max_cashier_queue > 0
        && settings->max_cashiers > 0
        && settings->max_next_customers > 0;
}

void settings_print(Settings* settings)
{
    printf("MAX_CUSTOMER_TIME=%d\nMAX_CASHIER_QUEUE=%d\nMAX_CASHIERS="
           "%d\nMAX_"
           "NEXT_CUSTOMERS=%d\n",
        settings->max_customer_time, settings->max_cashier_queue,
        settings->max_cashiers, settings->max_next_customers);
}

int main(int argc, char* argv[])
{
    char* settings_path = NULL;

    if (argc >= 2) {
        settings_path = argv[1];
    } else {
        settings_path = "settings.txt";
    }

    Settings* settings = settings_new();
    switch (settings_read_from_file(settings, settings_path)) {
    case SETTINGS_CANNOT_READ:
        printf("I can't read settings file\n");
        break;
    case SETTINGS_BAD_VALUE:
        printf("Some bad value in settings file\n");
        break;
    }

    if (settings->error) {
        settings_free(settings);
        poka_durak();
        return EXIT_FAILURE;
    }

    settings_print(settings);

    if (!settings_are_all_properties_assigned(settings)) {
        printf("Some property is not assigned.\n");
        settings_free(settings);
        poka_durak();
        return EXIT_FAILURE;
    }

    Market* market = market_new(
        settings->max_cashiers, settings->max_cashier_queue);
    market_add_new_customer(
        market, customer_new_random(settings->max_customer_time));

    long long int time = 0;

    while (true) {
        // render display
        //
        // display numbers [1; MAX_CASHIERS]
        market_display_numbers_header(market);
        printf("\n");

        // display count of customer in queue
        market_display_count_of_customers_header(market);
        printf("\n");

        // display queue is work or not
        market_display_queue_is_work_or_not(market);
        printf("\n");

        // prepare
        // display customers in orders
        market_display_customers_in_order(market);
        printf("\n");

        // generate next customers
        Queue* next_customers
            = queue_new_random(settings->max_next_customers,
                settings->max_customer_time);

        // display some info
        printf("Time: %lld\n", time++);
        printf("Next customers: ");
        queue_customer_print(next_customers);
        printf("\n");

        printf("Customers in queue: %d\n",
            market_get_the_number_of_customers_in_queue(market));
        printf("The number of working cashboxes: %d\n",
            market_get_the_number_of_working_cashboxes(market));
        printf("The number of customers serverd: %d\n",
            market->customers_served);
        printf("Allowable queue at the cashboxes: %d\n",
            settings->max_cashier_queue);

        market_serve_current_customers(market);
        printf("\n");

        if (market_can_push_queue(market, next_customers)) {
            printf("GAME OVER\n");
            poka_durak();
            queue_free(next_customers);
            break;
        }
        market_add_new_customers(market, next_customers);

        queue_free_without_content(next_customers);

        fflush(stdout);

#if !_WIN32
        sleep(1);
#endif

#if __unix__ || __linux__ || __APPLE__ || __MACH__
        system("clear");
#elif _WIN32
        system("cls");
#else
        puts("who are u?");
#endif
    }

    market_free(market);
    settings_free(settings);

    return EXIT_SUCCESS;
}
