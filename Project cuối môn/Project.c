#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TRIPS 50
#define MAX_TICKETS 100

//1.C?u trúc Trip (Chuy?n xe)
typedef struct {
    char name [50]; //Ten dia diem
    char address[100]; //Dia chi cua dia diem
} Location;

typedef struct {
    char tripID[20];
    Location departure; //Dia diem khoi hanh
    Location destination; //Dia diem den
    char date[20]; //Ngay gio chuyen xe
    int totalSeats; //Tong so ghe 
    int bookedSeats; //So ghe da dat
} Trip;

//2. C?u trúc Transaction (Giao d?ch)
typedef struct {
    char name[50]; //Tên hành khách
    char phone[15]; //S? di?n tho?i
} Passenger;

typedef struct {
    char tiketId[20]; //Mã vé 
    char tripId[20]; //M? chuy?n xe 
    Passenger passenger; //Thông tin hành khách 
    int seatNumber; //S? gh? dã d?t
    double price; //Giá vé 
    int paymentStatus; //Tr?ng thái thanh toán (0: Chua thanh toán || 1: Ðã thanh toán)
    char date[20]; //Ngày gi? d?t vé
} Ticket;


void clear_input();
void input(char dest[], int max_len);
void printMenu();
int check_ID(Trip trip_list[], int num_trip, char *id);
void addBusTrip(Trip trip_List[], int *num_trips);
void update_bus_trip (Trip trip_list[], int num_trips);
void book_ticket(Trip trip_list[], int num_trips, Ticket ticket_list[], int num_tickets);


int main(){
    Trip trip_list[MAX_TRIPS];
    int num_trips = 0;
    int choice;

    do{
        printMenu();

        do{
            printf ("Moi ban nhap vao lua chon (1-8): ");
            if(scanf("%d", &choice) != 1 || choice < 1 || choice > 8){
                printf ("[Loi] Vui long nhap trong khoang 1-8 !!!\n");
                clear_input();
                continue;
            }
            clear_input();
            break;
        } while (true);

        switch(choice){
            case 1:
                addBusTrip(trip_list, &num_trips);
                printf_display_trips(trip_list, num_trips);
                break;
            case 2:
                update_bus_trip(trip_list, num_trips);
                printf_display_trips(trip_list, num_trips);
                break;
            default:
                break;
        } //Cua switch

    } while (true);

    return 0;
}


void clear_input(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//Hàm d? nh?p thông tin
void input(char dest[], int max_len){
    bool valid_input = false;
    if (fgets(dest, max_len, stdin) != NULL){
        dest[strcspn(dest, "\n")] = 0;

        if (strlen(dest) > 0){
                 valid_input = true;
        } else {
            printf("[Loi] Du lieu khong duoc de trong. Vui long nhap lai!!!\n");
        }
    }
}

void printMenu(){
    printf ("\n+------------------------------------------------------------+\n");
    printf ("|                      MENU CHUC NANG                        |\n");
    printf ("+------------------------------------------------------------+\n");
    printf ("|%-60s|\n", "1. Them chuyen xe moi");
    printf ("|%-60s|\n", "2. Cap nhat thong tin chuyen xe");
    printf ("|%-60s|\n", "3. Dat ve");
    printf ("|%-60s|\n", "4. Kiem tra tinh trang ve");
    printf ("|%-60s|\n", "5. Liet ke chuyen xe");
    printf ("|%-60s|\n", "6. Thanh toan ve");
    printf ("|%-60s|\n", "7. Quan ly trang thai ve");
    printf ("|%-60s|\n", "8. Bao cao thong ke & doanh thu");
    printf ("+------------------------------------------------------------+\n");
}

//Gi?i thích: trip_list truy c?p vào danh sách các id dang có, num_trip là s? lu?ng vé hi?n t?i, *id truy c?p vào giá tr? hi?n t?i c?a id 
int check_ID(Trip trip_list[], int num_trip, char *id){ 
    for (int i = 0; i < num_trip; i++){
        if (strcmp(trip_list[i].tripID, id) == 0){
            return i;
        }
    }
    return -1;
}

//1. Hàm thêm làm thay d?i c?u trúc c?a s? lu?ng trong m?ng nên s? d?ng con tr? truy c?p
void addBusTrip(Trip trip_List[], int *num_trips){
    if (*num_trips >= MAX_TRIPS){
        printf ("[Loi] Danh sach chuyen xe da day, khong the them chuyen xe.\n");
        return;
    }

    Trip new_trip;
    char temp_id[20]; //Tao them 1 bien de ghi lai id moi va gan vao Trip

    printf ("\n---THEM CHUYEN XE MOI---\n");

    do{
        printf ("Nhap Ma chuyen xe: ");
        input (temp_id, 20);
        int check = check_ID(trip_List, *num_trips, temp_id);

        if(check != -1){
            printf ("[Loi] Ma chuyen xe %s da ton tai, vui long nhap lai !!!\n", temp_id);
        } else{
            break; //Thoat khi id chua ton tai
        }
    } while (true);
    strcpy(new_trip.tripID, temp_id);

    //Sau khi ID hop le -> tien hanh nhap cac thong tin con lai

    printf ("Nhap Ten dia diem khoi hanh: ");
    input(new_trip.departure.name, 50);

    printf ("Nhap dia chi khoi hanh: ");
    input(new_trip.departure.address, 100);

    printf ("Nhap Ten dia diem den: ");
    input(new_trip.destination.name, 50);

    printf ("Nhap Dia chi den: ");
    input(new_trip.destination.address, 100);

    printf ("Nhap ngay dio chuyen xe (VD: NGAY-THANG-NAM 00:00): ");
    input(new_trip.date, 20);

    do{
        printf ("Nhap tong so ghe: ");
        if (scanf("%d", &new_trip.totalSeats) != 1 || new_trip.totalSeats <= 0){
            printf ("[Loi] Vui long nhap so ghe (> 0) !!!\n");
            clear_input();
            continue;
        }
        clear_input();
        break;
    } while (true);

    new_trip.bookedSeats = 0;
    trip_List[*num_trips] = new_trip;
    (*num_trips)++;

    printf ("[Thanh cong] Da them chuyen xe '%s' (Ma: %s) voi %d ghe.\n", new_trip.departure.name, new_trip.tripID, new_trip.totalSeats);
}   

//2. Hàm c?p nh?t chuy?n xe
void update_bus_trip (Trip trip_list[], int num_trips){
    if (num_trips == 0){
        printf ("[Loi] Danh sach chuyen xe hien tai rong, khong the cap nhat !!!\n");
        return;
    }

    char id_update[20];
    printf ("\n---CAP NHAT THONG TIN CHUYEN XE---\n");
    printf ("Nhap Ma chuyen xe can cap nhat: ");
    input(id_update, 20);

    int index = check_ID(trip_list, num_trips, id_update);
    if(index == -1){
        printf ("[Loi] Khong tim thay chuyen xe voi Ma ID: %s .\n", id_update);
        return;
    }

    Trip *trip = &trip_list[index];
    
    printf ("Nhap Ten dia diem khoi hanh moi: ");
    input(trip->departure.name, 50);

    printf ("Nhap Dia chi khoi hanh moi: ");
    input(trip->departure.address, 100);

    printf ("Nhap Ten dia diem den moi: ");
    input(trip->destination.name, 50);

    printf ("Nhap Dia chi den moi: ");
    input(trip->destination.address, 100);

    printf ("Nhap vao ngay gio chuyen xe moi (VD: NGAY-THANG-NAM 00:00): ");
    input (trip->date, 20);

    int new_totalSeats; //khai bao 1 bien luu gia tri tong so ghe moi
    do{
        printf ("Nhap vao so ghe moi: ");
        if(scanf("%d", &new_totalSeats) != 1 || new_totalSeats < 0){
            printf ("[Loi] Vui long nhap so ghe (> 0) !!!\n");
            clear_input();
            continue;
        }
        clear_input();
        break;
    } while (true);

    trip->totalSeats = new_totalSeats;
    printf ("[Thanh cong] Da cap nhat thong tin chuyen xe thanh cong !!!\n");
}

//Hàm d? in ra danh sách các chuy?n xe 
void printf_display_trips(Trip trip_list[], int num_trips){
    if (num_trips == 0){
        printf ("[Thong bao] Danh sach chuyen xe hien tai rong.\n");
        return; 
    }
    printf ("\n+----------------------------------------------------------------------------------------------+\n"); //95
    printf ("|                                    DANH SACH CHUYEN XE                                       |\n");
    printf ("+----------------------------------------------------------------------------------------------+\n"); 
    printf ("|%-10s| %-15s | %-15s | %-20s | %-10s | %-10s|\n", "ID", "KH.HANH", "DIEM DEN", "THOI GIAN", "DA DAT", "TONG");
    printf ("+----------------------------------------------------------------------------------------------+\n"); 
    for (int i = 0; i < num_trips; i++){
        printf ("|%-10s| %-15s | %-15s | %-20s | %-10d | %-10d|\n",
                trip_list[i].tripID,
                trip_list[i].departure.name,
                trip_list[i].destination.name,
                trip_list[i].date,
                trip_list[i].bookedSeats,
                trip_list[i].totalSeats);
    }
    printf ("+----------------------------------------------------------------------------------------------+\n"); 
}

