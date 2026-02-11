#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_ROWS 6
#define MAX_COLS 10
#define MAX_SHOWS 4
#define MAX_NAME 50
#define MAX_PHONE 15
#define GREEN "\033[32m"
#define RESET "\033[0m"

// Structure for customer in queue
typedef struct Customer {
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    struct Customer *next;
} Customer;

// Structure for queue
typedef struct Queue {
    Customer *front;
    Customer *rear;
    int count;
} Queue;

// Structure for show
typedef struct {
    int showNumber;
    char movieName[50];
    char time[10];
    int price;
    int seats[MAX_ROWS][MAX_COLS];
    int availableSeats;
} Show;

// Structure for booking
typedef struct {
    char ticketID[20];
    char name[MAX_NAME];
    int showNumber;
    char seats[100];
    int totalAmount;
} Booking;

// Global variables
Queue bookingQueue;
Show shows[MAX_SHOWS];
int ticketCounter = 1;

// Function prototypes
void initializeQueue(Queue *q);
void initializeShows();
int isEmpty(Queue *q);
void enqueue(Queue *q, char name[], char phone[]);
Customer dequeue(Queue *q);
void displayQueue(Queue *q);
void displayMainMenu();
void displayShows();
void processCustomer();
void bookTickets(char customerName[], char customerPhone[]);
void displaySeats(int showNum);
void cancelBooking();
void generateTicketID(char *ticketID);
void clearScreen();
void waitForEnter();

int main() {
    int choice;
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    
    // Initialize
    initializeQueue(&bookingQueue);
    initializeShows();
    srand(time(NULL));
    
    printf("\n========================================\n");
    printf("   MOVIE THEATER BOOKING SYSTEM\n");
    printf("========================================\n");
    waitForEnter();
    
    while(1) {
        clearScreen();
        displayMainMenu();
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); // Clear newline
        
        switch(choice) {
            case 1:
                printf("\n--- Join Booking Queue ---\n");
                printf("Enter your name: ");
                fgets(name, MAX_NAME, stdin);
                name[strcspn(name, "\n")] = 0; // Remove newline
                
                printf("Enter phone number: ");
                fgets(phone, MAX_PHONE, stdin);
                phone[strcspn(phone, "\n")] = 0;
                
                enqueue(&bookingQueue, name, phone);
                printf(GREEN"You have been added to the queue!\n"RESET);
                printf("Your position: %d\n", bookingQueue.count);
                waitForEnter();
                break;
                
            case 2:
                processCustomer();
                break;
                
            case 3:
                displayShows();
                waitForEnter();
                break;
                
            case 4:
                cancelBooking();
                waitForEnter();
                break;
                
            case 5:
                displayQueue(&bookingQueue);
                waitForEnter();
                break;
                
            case 6:
                printf("\nThank you for using our system!\n");
                exit(0);
                
            default:
                printf("\nInvalid choice! Please try again.\n");
                waitForEnter();
        }
    }
    
    return 0;
}

void initializeQueue(Queue *q) {
    q->front = NULL;
    q->rear = NULL;
    q->count = 0;
}

void initializeShows() {
    // Show 1
    strcpy(shows[0].movieName, "The Dark Knight");
    strcpy(shows[0].time, "10:00 AM");
    shows[0].showNumber = 1;
    shows[0].price = 150;
    shows[0].availableSeats = MAX_ROWS * MAX_COLS;
    
    // Show 2
    strcpy(shows[1].movieName, "Inception");
    strcpy(shows[1].time, "1:30 PM");
    shows[1].showNumber = 2;
    shows[1].price = 200;
    shows[1].availableSeats = MAX_ROWS * MAX_COLS;
    
    // Show 3
    strcpy(shows[2].movieName, "Interstellar");
    strcpy(shows[2].time, "5:00 PM");
    shows[2].showNumber = 3;
    shows[2].price = 200;
    shows[2].availableSeats = MAX_ROWS * MAX_COLS;
    
    // Show 4
    strcpy(shows[3].movieName, "Tenet");
    strcpy(shows[3].time, "9:00 PM");
    shows[3].showNumber = 4;
    shows[3].price = 250;
    shows[3].availableSeats = MAX_ROWS * MAX_COLS;
    
    // Initialize all seats as available (0)
    for(int i = 0; i < MAX_SHOWS; i++) {
        for(int j = 0; j < MAX_ROWS; j++) {
            for(int k = 0; k < MAX_COLS; k++) {
                shows[i].seats[j][k] = 0;
            }
        }
    }
}

int isEmpty(Queue *q) {
    return (q->front == NULL);
}

void enqueue(Queue *q, char name[], char phone[]) {
    Customer *newCustomer = (Customer *)malloc(sizeof(Customer));
    strcpy(newCustomer->name, name);
    strcpy(newCustomer->phone, phone);
    newCustomer->next = NULL;
    
    if(isEmpty(q)) {
        q->front = newCustomer;
        q->rear = newCustomer;
    } else {
        q->rear->next = newCustomer;
        q->rear = newCustomer;
    }
    q->count++;
}

Customer dequeue(Queue *q) {
    Customer customer;
    strcpy(customer.name, "");
    strcpy(customer.phone, "");
    
    if(isEmpty(q)) {
        printf("Queue is empty!\n");
        return customer;
    }
    
    Customer *temp = q->front;
    strcpy(customer.name, temp->name);
    strcpy(customer.phone, temp->phone);
    
    q->front = q->front->next;
    if(q->front == NULL) {
        q->rear = NULL;
    }
    
    free(temp);
    q->count--;
    
    return customer;
}

void displayQueue(Queue *q) {
    printf("\n========================================\n");
    printf("        QUEUE STATUS\n");
    printf("========================================\n");
    
    if(isEmpty(q)) {
        printf("Queue is empty - No customers waiting\n");
        return;
    }
    
    printf("Total customers in queue: %d\n\n", q->count);
    printf("Position | Name                | Phone\n");
    printf("---------|---------------------|---------------\n");
    
    Customer *temp = q->front;
    int pos = 1;
    while(temp != NULL) {
        printf("   %-6d| %-20s| %s\n", pos, temp->name, temp->phone);
        temp = temp->next;
        pos++;
    }
    printf("========================================\n");
}

void displayMainMenu() {
    printf("\n========================================\n");
    printf("   MOVIE THEATER BOOKING SYSTEM\n");
    printf("========================================\n");
    printf("Current Queue: %d customers waiting\n", bookingQueue.count);
    if(!isEmpty(&bookingQueue)) {
        printf("Next in line: %s\n", bookingQueue.front->name);
    }
    printf("\nMAIN MENU:\n");
    printf("[1] Join Booking Queue\n");
    printf("[2] Process Next Customer\n");
    printf("[3] View Available Shows\n");
    printf("[4] Cancel Booking\n");
    printf("[5] View Queue Status\n");
    printf("[6] Exit\n");
    printf("========================================\n");
}

void displayShows() {
    printf("\n========================================\n");
    printf("        AVAILABLE SHOWS TODAY\n");
    printf("========================================\n");
    
    for(int i = 0; i < MAX_SHOWS; i++) {
        printf("Show %d: %-20s | %s | rs‚%-4d | Seats: %d/%d\n",
               shows[i].showNumber,
               shows[i].movieName,
               shows[i].time,
               shows[i].price,
               shows[i].availableSeats,
               MAX_ROWS * MAX_COLS);
    }
    printf("========================================\n");
}

void displaySeats(int showNum) {
    int idx = showNum - 1;
    
    printf("\n========================================\n");
    printf("      SEAT LAYOUT - Show %d\n", showNum);
    printf("        %s\n", shows[idx].movieName);
    printf("========================================\n");
    printf("              SCREEN\n\n");
    printf("     ");
    for(int j = 1; j <= MAX_COLS; j++) {
        printf("%2d ", j);
    }
    printf("\n");
    
    char rowLabel = 'A';
    for(int i = 0; i < MAX_ROWS; i++) {
        printf("  %c  ", rowLabel++);
        for(int j = 0; j < MAX_COLS; j++) {
            if(shows[idx].seats[i][j] == 0) {
                printf("O  "); // Available
            } else {
                printf("X   "); // Booked
            }
        }
        printf("\n");
    }
    
    printf("\0 = Available  X  = Booked\n");
    printf("========================================\n");
}

void processCustomer() {
    if(isEmpty(&bookingQueue)) {
        printf("\nNo customers in queue!\n");
        waitForEnter();
        return;
    }
    
    Customer customer = dequeue(&bookingQueue);
    printf("\n========================================\n");
    printf("Processing: %s\n", customer.name);
    printf("Phone: %s\n", customer.phone);
    printf("========================================\n");
    
    bookTickets(customer.name, customer.phone);
}

void bookTickets(char customerName[], char customerPhone[]) {
    int showChoice;
    char seatRow;
    int seatCol;
    int numSeats;
    char confirm;
    
    displayShows();
    
    printf("\nEnter show number to book (0 to cancel): ");
    scanf("%d", &showChoice);
    getchar();
    
    if(showChoice == 0) {
        printf("Booking cancelled.\n");
        waitForEnter();
        return;
    }
    
    if(showChoice < 1 || showChoice > MAX_SHOWS) {
        printf("Invalid show number!\n");
        waitForEnter();
        return;
    }
    
    int idx = showChoice - 1;
    
    displaySeats(showChoice);
    
    printf("\nHow many seats do you want to book? ");
    scanf("%d", &numSeats);
    getchar();
    
    if(numSeats > shows[idx].availableSeats) {
        printf("Not enough seats available!\n");
        waitForEnter();
        return;
    }
    
    int bookedSeats = 0;
    char seatList[100] = "";
    int totalAmount = 0;
    
    printf("\nEnter seats one by one (e.g., A5):\n");
    
    for(int i = 0; i < numSeats; i++) {
        printf("Seat %d: ", i + 1);
        scanf(" %c%d", &seatRow, &seatCol);
        getchar();
        
        seatRow = toupper(seatRow);
        int row = seatRow - 'A';
        int col = seatCol - 1;
        
        if(row < 0 || row >= MAX_ROWS || col < 0 || col >= MAX_COLS) {
            printf("Invalid seat! Try again.\n");
            i--;
            continue;
        }
        
        if(shows[idx].seats[row][col] == 1) {
            printf("Seat already booked! Try again.\n");
            i--;
            continue;
        }
        
        shows[idx].seats[row][col] = 1;
        shows[idx].availableSeats--;
        bookedSeats++;
        
        char temp[10];
        sprintf(temp, "%c%d", seatRow, seatCol);
        if(strlen(seatList) > 0) strcat(seatList, ", ");
        strcat(seatList, temp);
        
        totalAmount += shows[idx].price;
    }
    
    // Generate ticket
    char ticketID[20];
    generateTicketID(ticketID);
    
    printf("\n========================================\n");
    printf("      BOOKING CONFIRMED!\n");
    printf("========================================\n");
    printf("Ticket ID: %s\n", ticketID);
    printf("Name: %s\n", customerName);
    printf("Phone: %s\n", customerPhone);
    printf("Show: %s - %s\n", shows[idx].movieName, shows[idx].time);
    printf("Seats: %s\n", seatList);
    printf("Total Amount: rs‚%d\n", totalAmount);
    printf("========================================\n");
    printf("\nThank you for booking with us!\n");
    
    waitForEnter();
}

void cancelBooking() {
    int showChoice;
    char seatRow;
    int seatCol;
    
    printf("\n========================================\n");
    printf("        CANCEL BOOKING\n");
    printf("========================================\n");
    
    displayShows();
    
    printf("\nEnter show number: ");
    scanf("%d", &showChoice);
    getchar();
    
    if(showChoice < 1 || showChoice > MAX_SHOWS) {
        printf("Invalid show number!\n");
        return;
    }
    
    int idx = showChoice - 1;
    displaySeats(showChoice);
    
    printf("\nEnter seat to cancel (e.g., A5): ");
    scanf(" %c%d", &seatRow, &seatCol);
    getchar();
    
    seatRow = toupper(seatRow);
    int row = seatRow - 'A';
    int col = seatCol - 1;
    
    if(row < 0 || row >= MAX_ROWS || col < 0 || col >= MAX_COLS) {
        printf("Invalid seat!\n");
        return;
    }
    
    if(shows[idx].seats[row][col] == 0) {
        printf("This seat is not booked!\n");
        return;
    }
    
    shows[idx].seats[row][col] = 0;
    shows[idx].availableSeats++;
    
    printf("\nâœ“ Booking cancelled successfully!\n");
    printf("Refund amount: â‚¹%d\n", shows[idx].price);
}

void generateTicketID(char *ticketID) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(ticketID, "TKT%04d%02d%02d%03d", 
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, ticketCounter++);
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void waitForEnter() {
    printf("\nPress ENTER to continue...");
    getchar();
}
