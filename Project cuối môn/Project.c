#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_TRIPS 50
#define MAX_TICKETS 100

//1.Cấu trúc Trip (Chuyến xe)
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

//2. Cấu trúc Transaction (Giao dịch)
typedef struct {
    char name[50]; //Tên hành khách
    char phone[15]; //Số điện thoại
} Passenger;

typedef struct {
    char tiketId[20]; //Mã vé 
    char tripId[20]; //Mẫ chuyến xe 
    Passenger passenger; //Thông tin hành khách 
    int seatNumber; //Số ghế đã đặt
    double price; //Giá vé 
    int paymentStatus; //Trạng thái thanh toán (0: Chưa thanh toán || 1: Đã thanh toán)
    int status;
    char date[20]; //Ngày giờ đặt vé
} Ticket;


void clear_input();
void input(char dest[], int max_len);
void printMenu();
void printf_display_trips(Trip trip_list[], int num_trips);

void addBusTrip(Trip trip_List[], int *num_trips);
int check_ID(Trip trip_list[], int num_trip, char *id);

void update_bus_trip (Trip trip_list[], int num_trips);

void book_ticket(Trip trip_list[], int num_trips, Ticket ticket_list[], int *num_tickets);

void check_ticket (Trip trip_list[], int num_trips, Ticket ticket_list[], int num_tickets);

void list_trips(Trip trip_list[], int num_trips);

void pay_ticket(Ticket ticket_list[], int num_tickets);


int main(){
    Trip trip_list[MAX_TRIPS];
    Ticket ticket_list[MAX_TICKETS];
    int num_trips = 0, num_tickets = 0;
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
            case 3:
                book_ticket(trip_list, num_trips, ticket_list, &num_tickets);
                break;
            case 4:
                check_ticket (trip_list,  num_trips, ticket_list, num_tickets);
                break;
            case 5:
                list_trips(trip_list, num_trips);
                break;
            case 6:
                pay_ticket(ticket_list, num_tickets);
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

//Hàm để nhập thông tin
void input(char dest[], int max_len){
    bool valid_input = false;
    do{
        if (fgets(dest, max_len, stdin) != NULL){
            dest[strcspn(dest, "\n")] = 0;

            if (strlen(dest) > 0){
                    valid_input = true;
            } else {
                printf("[Loi] Du lieu khong duoc de trong. Vui long nhap lai: ");
            }
        } else {
            break;
        }
    } while (!valid_input);
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
    printf ("|%-60s|\n", "9. Thoat chuong trinh");
    printf ("+------------------------------------------------------------+\n");
}

//Giải thích: trip_list truy cập vào danh sách các id đang có, num_trip là số lượng vé hiện tại, *id truy cập vào giá trị hiện tại của id 
int check_ID(Trip trip_list[], int num_trip, char *id){ 
    for (int i = 0; i < num_trip; i++){
        if (strcmp(trip_list[i].tripID, id) == 0){
            return i;
        }
    }
    return -1;
}

int check_ID_ticket(Ticket ticket_list[], int num_tikcets, char *id){ 
    for (int i = 0; i < num_tikcets; i++){
        if (strcmp(ticket_list[i].tiketId, id) == 0){
            return i;
        }
    }
    return -1;
}

//1. Hàm thêm làm thay đổi cấu trúc của số lượng trong mảng nên sử dụng con trỏ truy cập
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

//2. Hàm cập nhật chuyến xe
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

//3. Hàm đặt vé xe
//kiểm tra điều kiện rỗng
void book_ticket(Trip trip_list[], int num_trips, Ticket ticket_list[], int *num_tickets){
    if (num_trips == 0){
        printf ("[Loi] Khong co chuyen xe nao de dat ve !!!\n");
        return;
    } 
    if (*num_tickets >= MAX_TICKETS){
        printf ("[Loi] Da het ve !!!\n");
        return;
    }

    //Đặt vé
    char book_ticket_id[20];
    printf ("\n---DAT VE XE---\n");
    printf ("Moi ban nhap vao ID chuyen xe muon dat ve: ");
    input(book_ticket_id, 20);

    int trips_index_book = check_ID(trip_list, num_trips, book_ticket_id);
    if(trips_index_book == -1){
        printf ("[Loi] Khong tim thay chuyen xe voi Ma ID: %s\n", book_ticket_id);
        return;
    }

    Trip *trip_book = &trip_list[trips_index_book];
    if (trip_book->bookedSeats >= trip_book->totalSeats){
        printf ("[Loi] Chuyen xe %s da het cho, vui long  dat chuyen xe khac !!!\n", book_ticket_id);
        return;
    }
    printf ("[Thong bao] Chuyen xe %s con trong: %d ghe.\n", book_ticket_id, trip_book->totalSeats);

    //Nhặp và kiểm tra thông tin hành khách
    Ticket ticket_book;
    printf ("\n---THONG TIN HANH KHACH---\n");
    printf ("Nhap ten hanh khach: ");
    input(ticket_book.passenger.name, 50);

    printf ("Nhap vao so dien thoai: ");
    bool check_phone = false;
    while (!check_phone){
        input(ticket_book.passenger.phone, 15);
        bool check_number = true;
        for (int i; ticket_book.passenger.phone[i] != '\0'; i++){
            if(ticket_book.passenger.phone[i] < '0' || ticket_book.passenger.phone[i] > '9'){
                check_number = false;
                break;
            }
        }
        if (check_number){
            check_phone = true;
        } else {
            printf ("[Loi] So dien thoai chi duoc chua ky tu so !!!\n");
        }
    }

    //Kiểm tra số ghế muốn đặt
    bool check_seat = false;
    int requested_seat; ///Ghe dat

    while (!check_seat){
        printf ("Nhap vao so ghe muon dat: ");
        if (scanf("%d", &requested_seat) != 1 ){
            printf ("[Loi] Vui long nhap so!!!\n");
            clear_input();
            continue;
        }
        clear_input();

        if(requested_seat <= 0 || requested_seat > trip_book->totalSeats){
            printf ("[Loi] So ghe phai nam trong pham vi 0 - %d\n", trip_book->totalSeats);
        } else {
            //kiểm tra xem ghế đã được đặt hay chưa
            bool book_seat = false;
            for (int i = 0; i < *num_tickets; i++){
                if(strcmp(ticket_list[i].tripId, book_ticket_id) == 0 && ticket_list[i].seatNumber == requested_seat){
                    book_seat = true;
                    break;
                }
            }
            if(book_seat){
                printf ("[Loi] Ghe so %d da co nguoi dat, vui long nhap ghe khac !!!\n", requested_seat);
            } else {
                check_seat = true;
            }
        }
    }
    ticket_book.seatNumber = requested_seat; //gan lai gia tri so ghe dat bang so ghe da dat

    //nhap kiem tra gia ve
    printf ("Nhap gia ve: ");
    while(scanf("%lf", &ticket_book.price) != 1 || ticket_book.price <= 0){
        printf ("[Loi] Vui long nhap gia ve la so duong !!!\n");
        clear_input();
    }
    clear_input();

    //Hoan tat va gan lai cac gia tri da nhap vao
    sprintf (ticket_book.tiketId, "TICKET %d", *num_tickets + 1);
    strcpy (ticket_book.tripId, book_ticket_id);

    ticket_book.paymentStatus = 0; //Thanh toan o case 6
    ticket_book.status = 0; // phuc vu trang thai o case 7

    // Tu dong sinh ngay dat ve (lay thoi gian hien tai)
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(ticket_book.date, sizeof(ticket_book.date), "%Y-%m-%d %H:%M:%S", tm_info);
 
    trip_book->bookedSeats++; //Cap nhat so ghe duoc dat cua chuyen xe.

    ticket_list[*num_tickets] = ticket_book; //luu ve
    (*num_tickets)++;

    printf ("\n---[Thong bao] Dat ve thanh cong !!!---\n");
    printf (" Ma ve: %s\n", ticket_book.tiketId);
    printf (" Ma chuyen xe: %s\n", ticket_book.tripId);
    printf (" Ghe da dat: %d\n", ticket_book.seatNumber);
    printf (" Gia ve: %lf\n", ticket_book.price);
    printf (" Thoi gian dat: %s\n", ticket_book.date);
    printf (" Trang thai: CHUA THANH TOAN.\n");
    printf ("     ---------------------------\n");
}

//4. Kiem tra ve xe
void check_ticket (Trip trip_list[], int num_trips, Ticket ticket_list[], int num_tickets){
    if (num_tickets == 0){
        printf ("[Thong bao] Danh sach rong. Khong the kiem tra.\n");
        return;
    }

    char id_check[20];
    printf ("\n---KIEM TRA TINH TRANG VE XE---\n");

    printf ("Nhap ma ve can kien tra: ");
    input(id_check, 20);

    int ticket_check = check_ID(trip_list, num_tickets, id_check);

    if (ticket_check == -1){
        printf ("[Loi] Khong tim thay ve xe voi Ma ID: %s. Vui long kiem tra lai !!!\n", id_check);
        return;
    }

    Ticket *ticket = &ticket_list[ticket_check];

    //tim thong tin cua chuyen xe
    int trip_index = check_ID(trip_list, num_trips, ticket->tripId);
    Trip *trip = (trip_index != -1) ? &trip_list[trip_index] : NULL;

    printf ("/n---THONG TIN VE XE---/n");
    printf (" Ma ve: %s\n", ticket->tiketId);
    printf (" Ma chuyen xe: %s\n", ticket->tripId); 
    printf (" Ngay dat ve: %s\n", ticket->date);
    printf ("     ---------------\n");

    printf ("/n---THONG TIN HANH KHACH DAT VE---/n");
    printf (" Ten: %s\n", ticket->passenger.name);
    printf (" So dien thoai: %s\n", ticket->passenger.phone);
    printf ("     ---------------------------\n");

    printf ("/n---THONG TIN CHUYEN XE---/n");
    printf (" Khoi hanh: %s (Dia chi: %s)\n", trip->departure.name, trip->departure.address);
    printf (" Diem den: %s (Dia chi: %s)\n", trip->destination.name, trip->destination.address);
    printf ("     -------------------\n");

    printf ("/n---THANH TOAN---/n");
    printf (" So ghe da dat: %d\n", ticket->seatNumber);
    printf (" Gia ve: %lf VND\n", ticket->price);
    printf (" Trang thai thanh toan: %s\n", (ticket->paymentStatus == 1 ? "DA THANH TOAN" : "CHUA THANH TOAN"));
    printf (" Tinh trang ve: %s\n", ticket->status);
    printf ("      ----------\n");
}

//5. Liet ke cac chuyen xe
void list_trips(Trip trip_list[], int num_trips){
    if (num_trips == 0){
        printf ("[Thong bao] khong co chuyen xe nao de hien thi.\n");
        return;
    }
    int page_size = 10; //Neu khong nhap mac dinh la 10
    int page_number = 1;
    int input_buf;

    printf ("\n---LIET KE CAC CHUYEN XE---\n");
    do{
        printf ("Moi nhap vao so dong hien thi 1 trang: ");
        if (scanf("%d", &input_buf) != 1 || input_buf <= 0 || input_buf > 10){
            printf ("[Loi] Vui long nhap vao so nguyen duong 1 - 10 !!!\n");
            clear_input();
            continue;
        } 
        clear_input();
        page_size = input_buf;
        break;
    } while (1);
    
    int total_page = (num_trips + page_size - 1) / page_size;

    do{
        printf ("Nhap vao so trang muon xem: ");
        if (scanf("%d", &page_number) != 1 || page_number <= 0 || page_number > total_page){
            printf ("[Loi] Vui long nhap vao so nguyen duong 1 - %d !!!\n", total_page);
            clear_input();
            continue;
        } 
        clear_input();
        break;
    } while (1);

    int choice = -1;

    do{
        int start_index = (page_number - 1) * page_size;
        int end_index = start_index + page_size;

        if (end_index > num_trips){
            end_index = num_trips;
        }

        printf ("\n+----------------------------------------------------------------------------------------------+\n"); //95
        printf ("|                                    DANH SACH CHUYEN XE                                       |\n");
        printf ("+----------------------------------------------------------------------------------------------+\n"); 
        printf ("|%-10s| %-15s | %-15s | %-20s | %-10s | %-10s|\n", "STT", "ID", "KH.HANH", "DIEM DEN", "THOI GIAN", "DA DAT");
        printf ("+----------------------------------------------------------------------------------------------+\n"); 
        for (int i = start_index; i < end_index; i++){
            printf ("|%-10d| %-15s | %-15s | %-20s | %-10s | %-10d|\n",
                    i + 1,
                    trip_list[i].tripID,
                    trip_list[i].departure.name,
                    trip_list[i].destination.name,
                    trip_list[i].date,
                    trip_list[i].bookedSeats);
        }
        printf ("+----------------------------------------------------------------------------------------------+\n");

        do{
            printf ("[Goi y] De xem trang tiep vui long nhap 1, xem trang truoc nhap 0.\n");
            printf ("Nhap vao lua chon cua ban (0 - Previous || 1 - Next || 2 - Thoat): "); 
            if (scanf("%d", &choice) != 1 || choice < 0 || choice > 2){
                printf("[Loi] Vui long nhap 1 hoac 0 !!!\n");
                clear_input();
                continue;
            }
            clear_input();
            break;
        } while (1);

        switch(choice){
            case 0:
                if(page_number > 1){
                    page_number--; 
                    printf("Quay lai trang %d.\n", page_number);
                } else {
                    printf ("[Loi] Day la trang dau tien !!!\n");
                }
                break;
            case 1:
                if(page_number < total_page){
                    page_number++; 
                    printf("Chuyen den trang %d.\n", page_number);
                } else {
                    printf ("[Loi] Day la trang cuoi cung !!!\n");
                }
                break;
            case 2:
                printf ("[Thong bao] Thoat xem danh sach !!!\n");
                return;
            default:
                break;
        }
    } while (1);
}

//6. Chuc nang thanh toan ve xe
void pay_ticket(Ticket ticket_list[], int num_tickets){
    if (num_tickets == 0){
        printf ("[Thong bao] Danh sach ve xe rong. Khong the thuc hien cac chuc nang thanh toan.\n");
        return;
    }
    char id_pay[20];
    printf ("\n----THANH TOAN VE XE----\n");
    printf ("Nhap Ma ve can thanh toan: ");
    input(id_pay, 20);
    
    int index_pay = check_ID_ticket(ticket_list, num_tickets, id_pay);

    if (index_pay == -1){
        printf ("[Loi] Khong tim thay ve xe voi Ma ID: %s. Vui long kiem tra lai.\n", id_pay);
        return;
    }
    Ticket *ticket = &ticket_list[index_pay];
    if (ticket->paymentStatus == 1){
        printf ("[Loi] Ve %s da duoc thanh toan.\n", id_pay);
        return;
    }
    ticket->paymentStatus = 1; //Dat bang 1 de phuc vu cho case 7 (Danh dau da dat)

    printf ("[Thanh cong] Da thanh toan thanh cong ve %s.\n", id_pay);
    printf ("[Trang thai] DA THANH TOAN !!!\n");
}

//7. Khoa huy ve
void look_cancel_ticket(Trip trip_list[], int num_trips, Ticket ticket_list[], int num_tickets){
    int choice;
    char id_check[20];

    if (num_tickets == 0){
        printf ("[Thong bao] Danh sach ve trong. Khong the thuc hien thao tac !!!\n");
        return;
    }

    printf ("\n----KHOA / HUY VE XE----\n");
    printf ("Nhap vao Ma ve can thao tac: ");
    input(id_check, 20);

    int index_check = check_ID_ticket(ticket_list, num_tickets, id_check);
    if (index_check == -1){
        printf ("[Loi] Khong tim thay Ma ID: %s. Vui long nhap lai !!!\n");
        return;
    }

    Ticket *ticket = &ticket_list[index_check];
    if(ticket->status != 0){
        printf ("[Loi] Ve %s da bi vo hieu hoa. Khong the thao tac tiep !!!\n", id_check);
        return;
    }
    int trip_index = check_ID (trip_list, num_trips, ticket->tripId);
    
    do{
        do{
            printf ("Moi ban nhap vao hanh dong (1 - Khoa ve || 2 - Huy ve || 0 - Thoat)");
            if (scanf("%d", &choice) != 1 || choice < 0 || choice > 2){
                printf ("[Loi] Vui long nhap vao 0 - 1 - 2 !!!\n");;
                clear_input();
                continue;
            }
            clear_input();
            break;
        } while (1);

        switch(choice){
            case 1:
                ticket->status = 1;
                printf ("[Thong bao] Da khoa ve %s thanh cong !!!\n", id_check);
                break;
            case 2:
                if(ticket->paymentStatus == 1){
                    printf ("[Loi] Khong the huy ve %s vi da thanh toan !!!\n", id_check);
                    return;
                }
                //Huy ve
                if (trip_index != -1){
                    //xoa ve va giai phong ve
                    Trip *trip = &trip_list[trip_index];
                    if (trip->bookedSeats > 0){
                        trip->bookedSeats--;
                        printf ("[Thong bao] Da giam so ghe da dat cua chuyen xe %s. Con lai: %d\n", trip->tripID, trip->totalSeats);
                    }
                }
                ticket->status = 2;
                printf ("[Thanh cong] Huy ve %s thanh cong.\n", id_check);
                break;
            case 0:
                printf ("[Thong bao] Thoat chuc nang !!!\n");
                return;
            default:
                break;
        }
    } while (1);
}

//Hàm để in ra danh sách các chuyến xe 
void printf_display_trips(Trip trip_list[], int num_trips){
    if (num_trips == 0){
        printf ("[Thong bao] Danh sach chuyen xe hien tai rong.\n");
        return; 
    }
    printf ("\n+----------------------------------------------------------------------------------------------+\n"); //80
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
