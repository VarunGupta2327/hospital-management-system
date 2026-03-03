#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATIENTS 100
#define MAX_DOCTORS 50
#define MAX_DEPARTMENTS 5

/* ── Structures ─────────────────────────────────────────────────────────── */

struct Patient {
  int patient_id;
  char name[100];
  int age;
  char medical_condition[200];
  char appointment_date[20];
};

struct Doctor {
  int doctor_id;
  char name[100];
  char department[50];
  struct Patient *patients[MAX_PATIENTS]; /* FIX: bounded by MAX_PATIENTS  */
  int patient_count;
};

/* FIX: Removed unused DoctorNode / department_roots declarations           */

/* ── Global state ───────────────────────────────────────────────────────── */

struct Patient patients[MAX_PATIENTS];
struct Doctor doctors[MAX_DOCTORS];
int patient_count = 0;
int doctor_count = 0;

/* ── Menu ───────────────────────────────────────────────────────────────── */

void displayMenu(void) {
  printf("\n--- Hospital Management System ---\n");
  printf("1. Add Patient\n");
  printf("2. Add Doctor\n");
  printf("3. Assign Doctor to Patient\n");
  printf("4. View Patient Record\n");
  printf("5. View Doctor Assignment\n");
  printf("6. Exit\n");
  printf("Enter your choice: ");
}

/* ── Add Patient ────────────────────────────────────────────────────────── */

void addPatient(void) {
  if (patient_count >= MAX_PATIENTS) {
    printf("Patient limit reached!\n");
    return;
  }

  printf("Enter patient ID: ");
  scanf("%d", &patients[patient_count].patient_id);

  printf("Enter patient name: ");
  getchar(); /* consume leftover newline after scanf */
  fgets(patients[patient_count].name, 100, stdin);
  patients[patient_count].name[strcspn(patients[patient_count].name, "\n")] =
      '\0';

  printf("Enter patient age: ");
  scanf("%d", &patients[patient_count].age);

  printf("Enter medical condition: ");
  getchar(); /* consume leftover newline after scanf */
  fgets(patients[patient_count].medical_condition, 200, stdin);
  patients[patient_count].medical_condition[strcspn(
      patients[patient_count].medical_condition, "\n")] = '\0';

  printf("Enter appointment date (DD-MM-YYYY): ");
  getchar(); /* FIX: consume leftover newline before fgets */
  fgets(patients[patient_count].appointment_date, 20, stdin);
  patients[patient_count].appointment_date[strcspn(
      patients[patient_count].appointment_date, "\n")] = '\0';

  patient_count++;
  printf("Patient added successfully!\n");
}

/* ── Add Doctor ─────────────────────────────────────────────────────────── */

void addDoctor(void) {
  if (doctor_count >= MAX_DOCTORS) {
    printf("Doctor limit reached!\n");
    return;
  }

  printf("Enter doctor ID: ");
  scanf("%d", &doctors[doctor_count].doctor_id);

  printf("Enter doctor name: ");
  getchar(); /* consume leftover newline after scanf */
  fgets(doctors[doctor_count].name, 100, stdin);
  doctors[doctor_count].name[strcspn(doctors[doctor_count].name, "\n")] = '\0';

  printf("Enter department: ");
  /* FIX: added getchar() to consume newline before fgets for department   */
  getchar();
  fgets(doctors[doctor_count].department, 50, stdin);
  doctors[doctor_count]
      .department[strcspn(doctors[doctor_count].department, "\n")] = '\0';

  doctors[doctor_count].patient_count = 0;
  doctor_count++;
  printf("Doctor added successfully!\n");
}

/* ── Assign Doctor to Patient ───────────────────────────────────────────── */

void assignDoctorToPatient(void) {
  int patient_id, doctor_id;
  printf("Enter patient ID: ");
  scanf("%d", &patient_id);
  printf("Enter doctor ID: ");
  scanf("%d", &doctor_id);

  struct Patient *patient = NULL;
  struct Doctor *doctor = NULL;

  int i;
  for (i = 0; i < patient_count; i++) {
    if (patients[i].patient_id == patient_id) {
      patient = &patients[i];
      break;
    }
  }

  for (i = 0; i < doctor_count; i++) {
    if (doctors[i].doctor_id == doctor_id) {
      doctor = &doctors[i];
      break;
    }
  }

  if (patient == NULL || doctor == NULL) {
    printf("Invalid patient or doctor ID!\n");
    return;
  }

  /* FIX: bounds check before assigning to prevent buffer overflow */
  if (doctor->patient_count >= MAX_PATIENTS) {
    printf("This doctor has reached the maximum number of patients!\n");
    return;
  }

  doctor->patients[doctor->patient_count] = patient;
  doctor->patient_count++;
  printf("Doctor assigned to patient successfully!\n");
}

/* ── View Patient Record ────────────────────────────────────────────────── */

void viewPatientRecord(void) {
  int patient_id;
  printf("Enter patient ID: ");
  scanf("%d", &patient_id);

  struct Patient *patient = NULL;
  int i;
  for (i = 0; i < patient_count; i++) {
    if (patients[i].patient_id == patient_id) {
      patient = &patients[i];
      break;
    }
  }

  if (patient == NULL) {
    printf("Patient not found!\n");
    return;
  }

  printf("\nPatient ID: %d\n", patient->patient_id);
  printf("Name: %s\n", patient->name);
  printf("Age: %d\n", patient->age);
  printf("Medical Condition: %s\n", patient->medical_condition);
  printf("Appointment Date: %s\n", patient->appointment_date);
}

/* ── View Doctor Assignment ─────────────────────────────────────────────── */

void viewDoctorAssignment(void) {
  int doctor_id;
  printf("Enter doctor ID: ");
  scanf("%d", &doctor_id);

  struct Doctor *doctor = NULL;
  int i;
  for (i = 0; i < doctor_count; i++) {
    if (doctors[i].doctor_id == doctor_id) {
      doctor = &doctors[i];
      break;
    }
  }

  if (doctor == NULL) {
    printf("Doctor not found!\n");
    return;
  }

  printf("\nDoctor ID: %d\n", doctor->doctor_id);
  printf("Name: %s\n", doctor->name);
  printf("Department: %s\n", doctor->department);
  printf("Assigned Patients:\n");

  if (doctor->patient_count == 0) {
    printf("No patients assigned yet.\n");
  } else {
    for (i = 0; i < doctor->patient_count; i++) {
      printf("  Patient ID: %d, Name: %s\n", doctor->patients[i]->patient_id,
             doctor->patients[i]->name);
    }
  }
}

/* ── Main ───────────────────────────────────────────────────────────────── */

int main(void) {
  int choice;

  while (1) {
    displayMenu();
    scanf("%d", &choice);

    switch (choice) {
    case 1:
      addPatient();
      break;
    case 2:
      addDoctor();
      break;
    case 3:
      assignDoctorToPatient();
      break;
    case 4:
      viewPatientRecord();
      break;
    case 5:
      viewDoctorAssignment();
      break;
    case 6:
      printf("Exiting the system...\n");
      return 0;
    default:
      printf("Invalid choice! Please try again.\n");
    }
  }

  return 0;
}