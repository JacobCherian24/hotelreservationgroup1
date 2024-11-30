#include <gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SMALL_ROOM_COST 5000
#define MEDIUM_ROOM_COST 7500
#define DELUXE_ROOM_COST 10000
#define TAX_RATE 0.18
#define LAUNDRY_COST 50 // Per clothing item
#define ROOM_SERVICE_COST 500 // Flat rate per service

typedef struct {
    int room_number;
    char type[10];
    char occupant_name[50];
    char mobile_number[15];
    int is_occupied;
    int laundry_items;
    int room_service_count;
    int stay_duration;
} Room;

// Define the hotel rooms
Room rooms[25];

// Initialize hotel rooms
void initialize_rooms() {
    for (int i = 0; i < 25; i++) {
        rooms[i].room_number = 101 + i;
        strcpy(rooms[i].occupant_name, "N/A");
        strcpy(rooms[i].mobile_number, "N/A");
        rooms[i].is_occupied = 0;
        rooms[i].laundry_items = 0;
        rooms[i].room_service_count = 0;
        rooms[i].stay_duration = 0;

        if (i < 12)
            strcpy(rooms[i].type, "Small");
        else if (i < 19)
            strcpy(rooms[i].type, "Medium");
        else
            strcpy(rooms[i].type, "Deluxe");
    }
}

// Calculate bill
void calculate_bill(Room *room, char *bill_summary) {
    int room_cost = 0;

    if (strcmp(room->type, "Small") == 0)
        room_cost = SMALL_ROOM_COST * room->stay_duration;
    else if (strcmp(room->type, "Medium") == 0)
        room_cost = MEDIUM_ROOM_COST * room->stay_duration;
    else if (strcmp(room->type, "Deluxe") == 0)
        room_cost = DELUXE_ROOM_COST * room->stay_duration;

    int laundry_cost = room->laundry_items * LAUNDRY_COST;
    int service_cost = room->room_service_count * ROOM_SERVICE_COST;

    int subtotal = room_cost + laundry_cost + service_cost;
    float tax = subtotal * TAX_RATE;
    float total = subtotal + tax;

    snprintf(bill_summary, 1024,
             "Bill Summary for Room %d\n\n"
             "Occupant Name: %s\n"
             "Mobile Number: %s\n\n"
             "Room Cost (for %d nights): %d INR\n"
             "Laundry Cost: %d INR\n"
             "Room Service Cost: %d INR\n"
             "---------------------------\n"
             "Subtotal: %d INR\n"
             "Tax (18%%): %.2f INR\n"
             "---------------------------\n"
             "Total: %.2f INR\n",
             room->room_number, room->occupant_name, room->mobile_number, room->stay_duration,
             room_cost, laundry_cost, service_cost, subtotal, tax, total);
}

// Reserve a room (with additional services)
void reserve_room(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *content_area, *grid;
    GtkWidget *entry_name, *entry_phone, *entry_room, *entry_days, *entry_laundry, *entry_services;
    GtkWidget *confirm_button;

    dialog = gtk_dialog_new_with_buttons("Reserve Room",
                                         GTK_WINDOW(data),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         "_Close", GTK_RESPONSE_CLOSE, NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    // Form inputs
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Name:"), 0, 0, 1, 1);
    entry_name = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_name, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Phone Number:"), 0, 1, 1, 1);
    entry_phone = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_phone, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Room Number (101-125):"), 0, 2, 1, 1);
    entry_room = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_room, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Stay Duration (nights):"), 0, 3, 1, 1);
    entry_days = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_days, 1, 3, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Number of Laundry Items:"), 0, 4, 1, 1);
    entry_laundry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_laundry, 1, 4, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Room Service Requests:"), 0, 5, 1, 1);
    entry_services = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_services, 1, 5, 1, 1);

    confirm_button = gtk_button_new_with_label("Confirm");
    gtk_grid_attach(GTK_GRID(grid), confirm_button, 0, 6, 2, 1);

    gtk_widget_show_all(dialog);

    // Handle reservation confirmation
    g_signal_connect(confirm_button, "clicked", G_CALLBACK((void (*)(GtkWidget *, gpointer)) [](GtkWidget *widget, gpointer dialog_ptr) {
        GtkWidget *dialog = GTK_WIDGET(dialog_ptr);
        GtkWidget *entry_name = g_object_get_data(G_OBJECT(dialog), "entry_name");
        GtkWidget *entry_phone = g_object_get_data(G_OBJECT(dialog), "entry_phone");
        GtkWidget *entry_room = g_object_get_data(G_OBJECT(dialog), "entry_room");
        GtkWidget *entry_days = g_object_get_data(G_OBJECT(dialog), "entry_days");
        GtkWidget *entry_laundry = g_object_get_data(G_OBJECT(dialog), "entry_laundry");
        GtkWidget *entry_services = g_object_get_data(G_OBJECT(dialog), "entry_services");

        const char *name = gtk_entry_get_text(GTK_ENTRY(entry_name));
        const char *phone = gtk_entry_get_text(GTK_ENTRY(entry_phone));
        int room_number = atoi(gtk_entry_get_text(GTK_ENTRY(entry_room)));
        int days = atoi(gtk_entry_get_text(GTK_ENTRY(entry_days)));
        int laundry_items = atoi(gtk_entry_get_text(GTK_ENTRY(entry_laundry)));
        int room_service_count = atoi(gtk_entry_get_text(GTK_ENTRY(entry_services)));

        if (room_number >= 101 && room_number <= 125) {
            int room_index = room_number - 101;
            if (!rooms[room_index].is_occupied) {
                // Update room information
                Room *room = &rooms[room_index];
                strcpy(room->occupant_name, name);
                strcpy(room->mobile_number, phone);
                room->stay_duration = days;
                room->laundry_items = laundry_items;
                room->room_service_count = room_service_count;
                room->is_occupied = 1;

                GtkWidget *success_dialog = gtk_message_dialog_new(
                    GTK_WINDOW(dialog), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE,
                    "Reservation successful for Room %d!", room_number);
                gtk_dialog_run(GTK_DIALOG(success_dialog));
                gtk_widget_destroy(success_dialog);
            } else {
                GtkWidget *error_dialog = gtk_message_dialog_new(
                    GTK_WINDOW(dialog), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
                    "Room %d is already occupied!", room_number);
                gtk_dialog_run(GTK_DIALOG(error_dialog));
                gtk_widget_destroy(error_dialog);
            }
        } else {
            GtkWidget *error_dialog = gtk_message_dialog_new(
                GTK_WINDOW(dialog), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
                "Invalid room number!");
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        }

        gtk_widget_destroy(dialog);
    }), dialog);

    g_object_set_data(G_OBJECT(dialog), "entry_name", entry_name);
    g_object_set_data(G_OBJECT(dialog), "entry_phone", entry_phone);
    g_object_set_data(G_OBJECT(dialog), "entry_room", entry_room);
    g_object_set_data(G_OBJECT(dialog), "entry_days", entry_days);
    g_object_set_data(G_OBJECT(dialog), "entry_laundry", entry_laundry);
    g_object_set_data(G_OBJECT(dialog), "entry_services", entry_services);
}

// View Bill
void view_bill(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *content_area, *entry_room, *confirm_button;
    dialog = gtk_dialog_new_with_buttons("View Bill",
                                         GTK_WINDOW(data),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         "_Close", GTK_RESPONSE_CLOSE, NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    entry_room = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(content_area), gtk_label_new("Enter Room Number:"));
    gtk_container_add(GTK_CONTAINER(content_area), entry_room);

    confirm_button = gtk_button_new_with_label("View Bill");
    gtk_container_add(GTK_CONTAINER(content_area), confirm_button);

    gtk_widget_show_all(dialog);

    // Show bill functionality
    g_signal_connect(confirm_button, "clicked", G_CALLBACK((void (*)(GtkWidget *, gpointer)) [](GtkWidget *widget, gpointer dialog_ptr) {
        GtkWidget *dialog = GTK_WIDGET(dialog_ptr);
        GtkWidget *entry_room = g_object_get_data(G_OBJECT(dialog), "entry_room");

        int room_number = atoi(gtk_entry_get_text(GTK_ENTRY(entry_room)));
        if (room_number >= 101 && room_number <= 125) {
            int room_index = room_number - 101;
            Room *room = &rooms[room_index];

            if (room->is_occupied) {
                char bill[1024];
                calculate_bill(room, bill);
                GtkWidget *bill_dialog = gtk_message_dialog_new(
                    GTK_WINDOW(dialog), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE,
                    "%s", bill);
                gtk_dialog_run(GTK_DIALOG(bill_dialog));
                gtk_widget_destroy(bill_dialog);
            } else {
                GtkWidget *error_dialog = gtk_message_dialog_new(
                    GTK_WINDOW(dialog), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
                    "Room %d is not occupied!", room_number);
                gtk_dialog_run(GTK_DIALOG(error_dialog));
                gtk_widget_destroy(error_dialog);
            }
        } else {
            GtkWidget *error_dialog = gtk_message_dialog_new(
                GTK_WINDOW(dialog), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
                "Invalid room number!");
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        }

        gtk_widget_destroy(dialog);
    }), dialog);

    g_object_set_data(G_OBJECT(dialog), "entry_room", entry_room);
}

int main(int argc, char *argv[]) {
    GtkWidget *window, *grid;
    GtkWidget *button_reserve, *button_bill;

    gtk_init(&argc, &argv);
    initialize_rooms();

    // Create main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Hotel Surfsplash Reservation System");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create grid layout
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Buttons
    button_reserve = gtk_button_new_with_label("Reserve a Room");
    button_bill = gtk_button_new_with_label("View Bill");

    gtk_grid_attach(GTK_GRID(grid), button_reserve, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_bill, 0, 1, 1, 1);

    g_signal_connect(button_reserve, "clicked", G_CALLBACK(reserve_room), window);
    g_signal_connect(button_bill, "clicked", G_CALLBACK(view_bill), window);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

