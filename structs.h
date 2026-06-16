#ifndef STRUCTS_H
#define STRUCTS_H

/* ============================================================
   CONSTANTS
   ============================================================ */

#define MAX_USERS        100
#define MAX_PATIENTS     100
#define MAX_STAFF         50
#define MAX_APPOINTMENTS 200
#define MAX_SCHEDULES    200
#define MAX_DIAGNOSTICS  200
#define MAX_NOTES        200
#define MAX_INVENTORY    200
#define MAX_COMPLAINTS   100
#define MAX_FINANCE      200

#define FILE_USERS        "users.txt"
#define FILE_PATIENTS     "patients.txt"
#define FILE_STAFF        "staff.txt"
#define FILE_APPOINTMENTS "appointments.txt"
#define FILE_SCHEDULES    "schedule.txt"
#define FILE_DIAGNOSTICS  "diagnostics.txt"
#define FILE_NOTES        "nurse_notes.txt"
#define FILE_INVENTORY    "inventory.txt"
#define FILE_COMPLAINTS   "complaints.txt"
#define FILE_FINANCE      "finance.txt"

/* ============================================================
   STRUCTS
   ============================================================ */

/* users.txt
   userID|username|password|role|phone|email
   e.g. D001|director|pass123|director|0123456789|director@hms.com */
struct user {
    char userID[10];
    char username[20];
    char password[20];
    char role[10];      /* "director", "admin", "doctor", "nurse", "patient" */
    char phone[15];
    char email[50];
};

/* patients.txt
   patientID|name|gender|dob|phone|email|address|bloodType
   e.g. P001|Ali Hassan|M|01/01/1990|0123456789|ali@email.com|123 Jalan ABC|O+ */
struct patient {
    char patientID[10];
    char name[50];
    char gender;        /* 'M' or 'F' */
    char dob[12];       /* DD/MM/YYYY */
    char phone[15];
    char email[50];
    char address[100];
    char bloodType[5];  /* e.g. A+, B-, O+ */
};

/* staff.txt
   staffID|name|gender|dob|phone|email|role
   e.g. DR001|Dr Sarah|F|05/05/1985|0198765432|sarah@hms.com|doctor */
struct staff {
    char staffID[10];
    char name[50];
    char gender;        /* 'M' or 'F' */
    char dob[12];       /* DD/MM/YYYY */
    char phone[15];
    char email[50];
    char role[10];      /* "director", "admin", "doctor", "nurse" */
};

/* appointments.txt
   appointmentID|patientID|doctorID|scheduleID|date|time|status
   e.g. APT001|P001|DR001|SCH001|20/06/2026|10:00|pending */
struct appointment {
    char appointmentID[10];
    char patientID[10];
    char doctorID[10];
    char scheduleID[10];
    char date[12];      /* DD/MM/YYYY */
    char time[6];       /* HH:MM */
    char status[15];    /* "pending", "completed", "cancelled" */
};

/* schedule.txt
   scheduleID|doctorID|date|time|status
   e.g. SCH001|DR001|20/06/2026|10:00|available */
struct schedule {
    char scheduleID[10];
    char doctorID[10];
    char date[12];      /* DD/MM/YYYY */
    char time[6];       /* HH:MM */
    char status[15];    /* "available", "booked" */
};

/* diagnostics.txt
   diagnosticID|appointmentID|patientID|doctorID|date|diagnosis|prescription|notes
   e.g. DG001|APT001|P001|DR001|20/06/2026|Fever|Paracetamol 500mg|Rest for 3 days */
struct diagnostic {
    char diagnosticID[10];
    char appointmentID[10];
    char patientID[10];
    char doctorID[10];
    char date[12];          /* DD/MM/YYYY */
    char diagnosis[200];
    char prescription[200];
    char notes[200];
};

/* nurse_notes.txt
   noteID|patientID|nurseID|date|time|temperature|bloodPressure|symptoms|note
   e.g. NN001|P001|N001|20/06/2026|08:00|36.8|120/80|Mild cough|Patient resting well */
struct nurseNote {
    char noteID[10];
    char patientID[10];
    char nurseID[10];
    char date[12];          /* DD/MM/YYYY */
    char time[6];           /* HH:MM */
    float temperature;      /* Celsius */
    char bloodPressure[10]; /* e.g. 120/80 */
    char symptoms[200];
    char note[200];
};

/* inventory.txt
   itemID|itemName|category|quantity|unit|expiryDate
   e.g. INV001|Paracetamol|medication|500|tablets|12/2027 */
struct inventory {
    char itemID[10];
    char itemName[50];
    char category[20];  /* "medication" or "supply" */
    int quantity;
    char unit[15];      /* "tablets", "boxes", "pcs" */
    char expiryDate[12];/* MM/YYYY — for medication; leave blank for supply */
};

/* complaints.txt
   complaintID|patientID|date|category|description|status
   e.g. CPL001|P001|20/06/2026|facility|Broken AC in ward 3|open */
struct complaint {
    char complaintID[10];
    char patientID[10];
    char date[12];      /* DD/MM/YYYY */
    char category[30];  /* e.g. "facility", "service" */
    char description[300];
    char status[15];    /* "open", "resolved" */
};

/* finance.txt
   financeID|date|patientID|appointmentID|amount|paymentMethod|status
   e.g. FIN001|20/06/2026|P001|APT001|150.00|cash|paid */
struct finance {
    char financeID[10];
    char date[12];          /* DD/MM/YYYY */
    char patientID[10];
    char appointmentID[10];
    float amount;
    char paymentMethod[20]; /* "cash", "card", "online" */
    char status[15];        /* "paid", "pending" */
};

#endif /* STRUCTS_H */