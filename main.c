#include <stdio.h>
#include <stdbool.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

enum MenuOption {
    SHOW_MAIN_MENU = 0,
    CREATE_REGISTRATION = 1,
    COMMIT_REGISTRATION = 2,
    MODIFY_REGISTRATION = 3,
    DELETE_REGISTRATION = 4,
    LIST_ALL_REGISTRATIONS = 5,
    MODIFY_SPECIFIC_REGISTRATION = 6,
    DELETE_SPECIFIC_REGISTRATION = 7,
    QUIT = 8,
    VACCINATE = 9
};

struct Registration {
    char name[101];
    int birthYear;
    char phoneNumber[16];
    bool isApplyingForPaidVaccination;
    bool isRegistrationCreated;
    bool isVaccinated;
};

void printRegistration(struct Registration *registration) {
    printf("Name: %s\nBirth Year: %d\nPhone Number: %s\nVaccination Type: %s\nVaccinated: %s\n", 
        registration->name,
        registration->birthYear,
        registration->phoneNumber,
        registration->isApplyingForPaidVaccination ? "Paid": "Free",
        registration->isVaccinated ? "Yes": "No");
}

void printMenu(bool isAdmin, struct Registration *registration) {
    printf("------------------------------\n");
    printf("------------------------------\n");

    if (!registration->isRegistrationCreated) {
        printf("[1] Create Registration\n");
    } else {
        printRegistration(registration);

        printf("------------------------------\n");

        printf("[1] Commit Registration\n");
        printf("[2] Modify Registration\n");
        printf("[3] Delete Registration\n");
    }

    if (registration->isRegistrationCreated) {
        printf("[4] List All Registrations\n");
        if (isAdmin) {
            printf("[5] Modify Registration By ID\n");
            printf("[6] Delete Registration By ID\n");
            printf("[7] Quit\n");
            printf("[8] Vaccinate\n");
        } else {
            printf("[5] Quit\n");
        }
    } else {
        printf("[2] List All Registrations\n");
        if (isAdmin) {
            printf("[3] Modify Registration By ID\n");
            printf("[4] Delete Registration By ID\n");
            printf("[5] Quit\n");
            printf("[6] Vaccinate\n");
        } else {
            printf("[3] Quit\n");
        }
    }

    printf("------------------------------\n");
}

bool clearStdinIfNeeded() {
    bool enteredExtraInvalidInput = false;

    char c;
    while((c = getchar()) != '\n' && c != EOF) {
        enteredExtraInvalidInput = true;
    }

    return enteredExtraInvalidInput;
}

int correctInputForIsRegistrationCreated(bool isAdmin, bool isRegistrationCreated, int input) {
    if (input == 0) {
        return input;
    }
    
    if (isRegistrationCreated) {
        if (!isAdmin && input == 5) {
            return 8;
        }

        return ++input;
    }

    if (!isAdmin && input == 3) {
        return 8;
    }

    if (2 <= input) {
        input += 3;
    }

    return input;
}

enum MenuOption getMenuOptionFromInput(bool isAdmin, struct Registration *registration) {
    while (true) {
        if (registration->isRegistrationCreated) {
            if (isAdmin) {
                printf("Please choose from [1..8]: ");
            } else {
                printf("Please choose from [1..5]: ");
            }
        } else {
            if (isAdmin) {
                printf("Please choose from [1..6]: ");
            } else {
                printf("Please choose from [1..3]: ");
            }
        }

        int input = -1;
        scanf("%d", &input);

        if (!clearStdinIfNeeded()
            && (((!isAdmin && registration->isRegistrationCreated && 0 <= input && input <= 5)
            || (!isAdmin && !registration->isRegistrationCreated && 0 <= input && input <= 3)) 
            || ((isAdmin && registration->isRegistrationCreated && 0 <= input && input <= 8)
            || (isAdmin && !registration->isRegistrationCreated && 0 <= input && input <= 6)))) {
            return (enum MenuOption) correctInputForIsRegistrationCreated(
                isAdmin, registration->isRegistrationCreated, input);
        } else {
            system("clear");
            printMenu(isAdmin, registration);
            printf("Your input was invalid.\n");
        }
    }
}

void readNameIntoRegistration(struct Registration *registration) {
    bool isInputValid = false;
    while (!isInputValid) {
        printf("Please enter your name: ");

        fgets(registration->name, 100, stdin);

        if (!clearStdinIfNeeded()) {
            registration->name[strcspn(registration->name, "\n")] = 0;
            isInputValid = true;
        } else {
            printf("Your input was invalid.\n");
        }
    }
}

void readBirthYearIntoRegistration(struct Registration *registration) {
    bool isInputValid = false;
    while (!isInputValid) {
        printf("Please enter your birth year: ");

        int input = -1;
        scanf("%d", &input);

        if (!clearStdinIfNeeded() && 1900 <= input && input <= 2021) {
            registration->birthYear = input;
            isInputValid = true;
        } else {
            printf("Your input was invalid.\n");
        }
    }
}

void readPhoneNumberIntoRegistration(struct Registration *registration) {
    bool isInputValid = false;
    while (!isInputValid) {
        printf("Please enter your phone number: ");

        scanf("%15s",  registration->phoneNumber);

        if (!clearStdinIfNeeded()) {
            isInputValid = true;
        } else {
            printf("Your input was invalid.\n");
        }
    }
}

void readIsApplyingForPaidVaccination(struct Registration *registration) {
    bool isInputValid = false;
    while (!isInputValid) {
        printf("Do you want to apply for paid vaccination? (y/n) ");

        char input;
        scanf("%s", &input);

        if (!clearStdinIfNeeded() && (input == 'y' || input == 'n')) {
            registration->isApplyingForPaidVaccination = ((input == 'y') ? true: false);
            isInputValid = true;
        } else {
            printf("Your input was invalid.\n");
        }
    }
}

void createRegistration(struct Registration *registration) {
    system("clear");
    
    readNameIntoRegistration(registration);
    readBirthYearIntoRegistration(registration);
    readPhoneNumberIntoRegistration(registration);
    readIsApplyingForPaidVaccination(registration);

    registration->isRegistrationCreated = true;
    registration->isVaccinated = false;
}

void modifyRegistration(struct Registration *registration) {
    system("clear");

    bool isInputValid = false;
    while (!isInputValid) {
        printf("Are you sure you want to modify your registration? (y/n) ");

        char input;
        scanf("%s", &input);

        if (!clearStdinIfNeeded() && (input == 'y' || input == 'n')) {
            if (input == 'y') {
                system("clear");
    
                printRegistration(registration);
                printf("------------------------------\n");

                readNameIntoRegistration(registration);
                readBirthYearIntoRegistration(registration);
                readPhoneNumberIntoRegistration(registration);
                readIsApplyingForPaidVaccination(registration);
            }

            isInputValid = true;            
        } else {
            printf("Your input was invalid.\n");
        }
    }
}

void deleteRegistration(struct Registration *registration) {
    system("clear");

    bool isInputValid = false;
    while (!isInputValid) {
        printf("Are you sure you want to delete your registration? (y/n) ");

        char input;
        scanf("%s", &input);

        if (!clearStdinIfNeeded() && (input == 'y' || input == 'n')) {
            registration->isRegistrationCreated = (input == 'y') ? false: true;
            isInputValid = true;
        } else {
            printf("Your input was invalid.\n");
        }
    }
}

void printAllRegistrations() {
    FILE *infile;
    struct Registration registration;

    infile = fopen("registrations.dat", "r");
    if (infile == NULL) {
        fprintf(stderr, "An unexpected error occured while reading registrations "
                        "from \"registration.dat\".\n");
        exit(1);
    }

    int id = 1;
    while (fread(&registration, sizeof(struct Registration), 1, infile)) {
        if(id == 1) {
            printf("------------------------------\n");
        } 
        printf("------------------------------\n");
        printf("ID: %d\n", id++);
        printRegistration(&registration);
    }

    fclose(infile);
}

void commitRegistration(struct Registration *registration) {
  if (registration->isRegistrationCreated) {
    FILE *outfile;

    outfile = fopen("registrations.dat", "a");
    if (outfile == NULL) {
      fprintf(stderr, "An unexpected error occured while saving your "
                      "registrations to \"registration.dat\".\n");
      exit(1);
    }

    fwrite(registration, sizeof(struct Registration), 1, outfile);

    if (&fwrite != 0) {
      printf("Your registration has successfully been commited.\n");
      registration->isRegistrationCreated = false;
    } else {
      fprintf(stderr, "An unexpected error occured while saving your "
                      "registrations to \"registration.dat\".\n");
      fclose(outfile);
      exit(1);
    }

    fclose(outfile);
  }
}

void modifyRegistrationByID(int id) {
    FILE *file;
    struct Registration *registrations = (struct Registration *) malloc(sizeof(struct Registration));

    file = fopen("registrations.dat", "r");
    if (file == NULL) {
        fprintf(stderr, "An unexpected error occured while modfying the registration "
                        "in \"registration.dat\".\n");
        exit(1);
    }

    struct Registration registration;
    int currentID = 1;
    while (fread(&registration, sizeof(struct Registration), 1, file)) {

        registrations[currentID - 1] = registration;

        currentID++;

        struct Registration *tmp = (struct Registration *) realloc(
            registrations, (currentID * sizeof(struct Registration)));

        if (tmp == NULL) {
            fprintf(stderr, "An unexpected error occured while modfying the registration "
                        "in \"registration.dat\".\n");
            exit(1);
        }
    
        registrations = tmp;
    }

    fclose(file);
    if (currentID <= id) {
        free(registrations);

        system("clear");
        fprintf(stderr, "Registration with the id [%d] doesn't exist.\n", id);

        return;
    }

    system("clear");
    
    printRegistration(&registrations[id - 1]);
    printf("------------------------------\n");

    readNameIntoRegistration(&registrations[id - 1]);
    readBirthYearIntoRegistration(&registrations[id - 1]);
    readPhoneNumberIntoRegistration(&registrations[id - 1]);
    readIsApplyingForPaidVaccination(&registrations[id - 1]);

    file = fopen("registrations.dat", "w");
    if (file == NULL) {
        fprintf(stderr, "An unexpected error occured while modfying the registration "
                        "in \"registration.dat\".\n");
        exit(1);
    }

    currentID--;
    while (0 < currentID) {
        fwrite(&registrations[currentID - 1], sizeof(struct Registration), 1, file);
        currentID--;
    }

    if(&fwrite != 0) {
        system("clear");
        printf("The registration with the id [%d] has successfully been modified.\n", id);
    } else {
        fprintf(stderr, "An unexpected error occured while modfying the registration from \"registration.dat\".\n"); 
        fclose(file);
        exit(1);
    } 

    fclose(file);
    free(registrations);
}

void modifyRegistrationByIDConfirmationPrompt(int id) {
    system("clear");

    bool isInputValid = false;
    while (!isInputValid) {
        printf("Are you sure you want to modify the registration with id [%d]? (y/n) ", id);

        char input;
        scanf("%s", &input);

        if (!clearStdinIfNeeded() && (input == 'y' || input == 'n')) {
            if (input == 'y') {
                modifyRegistrationByID(id);
            } else {
                system("clear");
            }

            isInputValid = true;
        } else {
            printf("Your input was invalid.\n");
        }
    }
}

void modifyRegistrationByIDIDPrompt() {
    system("clear");

    bool isInputValid = false;
    while (!isInputValid) {
        printf("What is the id of the registration you want to modify? ");

        int input = -1;
        scanf("%d", &input);

        if (!clearStdinIfNeeded() && 0 < input) {
            modifyRegistrationByIDConfirmationPrompt(input);
            isInputValid = true;
        } else {
            printf("Your input was invalid.\n");
        }
    }
}

void deleteRegistrationByID(int id) {
    FILE *file;
    struct Registration *registrations = (struct Registration *) malloc(sizeof(struct Registration));

    file = fopen("registrations.dat", "r");
    if (file == NULL) {
        fprintf(stderr, "An unexpected error occured while deleting the registration "
                        "from \"registration.dat\".\n");
        exit(1);
    }

    struct Registration registration;
    int currentID = 1;
    int idToExclude = id;
    while (fread(&registration, sizeof(struct Registration), 1, file)) {
        if (currentID != idToExclude) {
            registrations[currentID - 1] = registration;

            currentID++;

            struct Registration *tmp = (struct Registration *) realloc(
                registrations, (currentID * sizeof(struct Registration)));

            if (tmp == NULL) {
                fprintf(stderr, "An unexpected error occured while deleting the registration "
                            "from \"registration.dat\".\n");
                exit(1);
            }
        
            registrations = tmp;
        } else {
            idToExclude = -1;
        }
    }

    fclose(file);
    if (currentID < id) {
        free(registrations);

        system("clear");
        fprintf(stderr, "Registration with the id [%d] doesn't exist.\n", id);
        
        return;
    }

    file = fopen("registrations.dat", "w");
    if (file == NULL) {
        fprintf(stderr, "An unexpected error occured while deleting the registration "
                        "from \"registration.dat\".\n");
        exit(1);
    }

    currentID--;
    while (0 < currentID) {
        fwrite(&registrations[currentID - 1], sizeof(struct Registration), 1, file);
        currentID--;
    }

    if(&fwrite != 0) {
        system("clear");
        printf("The registration with the id [%d] has successfully been deleted.\n", id);
    } else {
        fprintf(stderr, "An unexpected error occured while deleting the registration from \"registration.dat\".\n"); 
        fclose(file);
        exit(1);
    } 

    fclose(file);
    free(registrations);
}

void deleteRegistrationByIDConfirmationPrompt(int id) {
    system("clear");

    bool isInputValid = false;
    while (!isInputValid) {
        printf("Are you sure you want to delete the registration with id [%d]? (y/n) ", id);

        char input;
        scanf("%s", &input);

        if (!clearStdinIfNeeded() && (input == 'y' || input == 'n')) {
            if (input == 'y') {
                deleteRegistrationByID(id);
            } else {
                system("clear");
            }

            isInputValid = true;
        } else {
            printf("Your input was invalid.\n");
        }
    }
}

void deleteRegistrationByIDIDPrompt() {
    system("clear");

    bool isInputValid = false;
    while (!isInputValid) {
        printf("What is the id of the registration you want to delete? ");

        int input = -1;
        scanf("%d", &input);

        if (!clearStdinIfNeeded() && 0 < input) {
            deleteRegistrationByIDConfirmationPrompt(input);
            isInputValid = true;
        } else {
            printf("Your input was invalid.\n");
        }
    }
}

void vaccinate() {
    FILE *file;
    struct Registration *registrations = (struct Registration *) malloc(sizeof(struct Registration));

    file = fopen("registrations.dat", "r");
    if (file == NULL) {
        fprintf(stderr, "An unexpected error occured while loading registrations "
                        "from \"registration.dat\".\n");
        exit(1);
    }

    int numberOfUnvaccinatedPeople = 0;
    struct Registration registration;
    int currentID = 1;
    while (fread(&registration, sizeof(struct Registration), 1, file)) {

        registrations[currentID - 1] = registration;

        currentID++;

        if (registration.isVaccinated == false) {
            numberOfUnvaccinatedPeople++;
        }

        struct Registration *tmp = (struct Registration *) realloc(
            registrations, (currentID * sizeof(struct Registration)));

        if (tmp == NULL) {
            fprintf(stderr, "An unexpected error occured while loading registrations "
                        "from \"registration.dat\".\n");
            exit(1);
        }
    
        registrations = tmp;
    }

    fclose(file);

    if (numberOfUnvaccinatedPeople < 5) {
        printf("");
        fflush(stdout);

        return;
    }

    // Creating pipes
    int fd[4][2];
    for (int i = 0; i < 4; i++) {
        if (pipe(fd[i]) < 0) {
            exit(1);
        }
    }

    // First child
    int pid1 = fork();
    if (pid1 < 0) {
        exit(1);
    }

    if (pid1 == 0 && 4 < numberOfUnvaccinatedPeople) {
        close(fd[0][1]);
        close(fd[1][0]);
        close(fd[2][0]);
        close(fd[2][1]);
        close(fd[3][0]);
        close(fd[3][1]);

        bool isReadyToVaccinate = true;
        if (write(fd[1][1], &isReadyToVaccinate, sizeof(bool)) < 0) {
            printf("Error writing is ready to vaccinate.");
            exit(1);
        }

        struct Registration *fiveUnvaccinatedPeople = (struct Registration *) malloc(sizeof(struct Registration) * 5);
        if (read(fd[0][0], fiveUnvaccinatedPeople, sizeof(struct Registration) * 5) < 0) {
            printf("Error reading five unvaccinated people.\n");
            exit(1);
        }

        srand(getpid() + time(0));
        for (int i = 0; i < 5; i++) {
            int num = rand() % 10 + 1;

            if (num != 1) {
                fiveUnvaccinatedPeople[i].isVaccinated = true;
            }

            printf("------------------------------\n");
            printf("Trying to vaccinate:\n");
            printRegistration(&fiveUnvaccinatedPeople[i]);
        }

        if (write(fd[1][1], fiveUnvaccinatedPeople, sizeof(struct Registration) * 5) < 0) {
            printf("Error writing five modified unvaccinated people.\n");
            exit(1);
        }

        close(fd[0][0]);
        close(fd[1][1]);

        exit(0);
    }

    int pid2 = 1;
    if (9 < numberOfUnvaccinatedPeople) {
        // Second child
        pid2 = fork();
        if (pid2 < 0) {
            exit(1);
        }
    }

    if (pid2 == 0 && 9 < numberOfUnvaccinatedPeople) {
        close(fd[0][0]);
        close(fd[0][1]);
        close(fd[1][0]);
        close(fd[1][1]);
        close(fd[2][1]);
        close(fd[3][0]);

        bool isReadyToVaccinate = true;
        if (write(fd[3][1], &isReadyToVaccinate, sizeof(bool)) < 0) {
            printf("Error writing is ready to vaccinate.");
            exit(1);
        }

        struct Registration *fiveUnvaccinatedPeople = (struct Registration *) malloc(sizeof(struct Registration) * 5);
        if (read(fd[2][0], fiveUnvaccinatedPeople, sizeof(struct Registration) * 5) < 0) {
            printf("Error reading five unvaccinated people.\n");
            exit(1);
        }

        srand(getpid() + time(0));
        for (int i = 0; i < 5; i++) {
            int num = rand() % 10 + 1;

            if (num != 1) {
                fiveUnvaccinatedPeople[i].isVaccinated = true;
            }

            printf("------------------------------\n");
            printf("Trying to vaccinate:\n");
            printRegistration(&fiveUnvaccinatedPeople[i]);
        }

        if (write(fd[3][1], fiveUnvaccinatedPeople, sizeof(struct Registration) * 5) < 0) {
            printf("Error writing five modified unvaccinated people.\n");
            exit(1);
        }

        close(fd[2][0]);
        close(fd[3][1]);

        exit(0);
    }

    if (4 < numberOfUnvaccinatedPeople) {
        // Parent
        close(fd[0][0]);
        close(fd[1][1]);
        close(fd[2][0]);
        close(fd[3][1]);

        bool isReadyToVaccinate = false;
        if (read(fd[1][0], &isReadyToVaccinate, sizeof(bool)) < 0) {
            printf("Error reading is ready to vaccinate.");
            exit(1);
        }
        if (9 < numberOfUnvaccinatedPeople) {
            if (read(fd[3][0], &isReadyToVaccinate, sizeof(bool)) < 0) {
                printf("Error reading is ready to vaccinate.");
                exit(1);
            }
        }

        // Write to child 1
        int i = 0;
        struct Registration *fiveUnvaccinatedPeople = (struct Registration *) malloc(sizeof(struct Registration) * 5);
        for (int j = 0; j < 5 && i < currentID - 1; i++) {
            if (registrations[i].isVaccinated == false) {
                fiveUnvaccinatedPeople[j++] = registrations[i];
                printf("Vaccination Station 1 is waiting for %s; SMS sent to %s\n", registrations[i].name, registrations[i].phoneNumber);
                fflush(stdout);
            }
        }
        if (write(fd[0][1], fiveUnvaccinatedPeople, sizeof(struct Registration) * 5) < 0) {
            printf("Error writing five unvaccinated people.\n");
            exit(1);
        }

        // Write to child 2
        if (9 < numberOfUnvaccinatedPeople) {
            fiveUnvaccinatedPeople = (struct Registration *) malloc(sizeof(struct Registration) * 5);
            for (int j = 0; j < 5 && i < currentID - 1; i++) {
                if (registrations[i].isVaccinated == false) {
                    fiveUnvaccinatedPeople[j++] = registrations[i];
                    printf("Vaccination Station 2 is waiting for %s; SMS sent to %s\n", registrations[i].name, registrations[i].phoneNumber);
                    fflush(stdout);
                }
            }
            if (write(fd[2][1], fiveUnvaccinatedPeople, sizeof(struct Registration) * 5) < 0) {
                printf("Error writing five unvaccinated people.\n");
                exit(1);
            }
        }
        
        // Read from child 1
        if (read(fd[1][0], fiveUnvaccinatedPeople, sizeof(struct Registration) * 5) < 0) {
            printf("Error writing five modified unvaccinated people.\n");
            exit(1);
        }

        i = 0;
        for (int j = 4; j >= 0 && i < currentID - 1; i++) {
            if (registrations[i].isVaccinated == false) {
                registrations[i] = fiveUnvaccinatedPeople[j--];
            }
        }

        // Read from child 2
        if (9 < numberOfUnvaccinatedPeople) {
            if (read(fd[3][0], fiveUnvaccinatedPeople, sizeof(struct Registration) * 5) < 0) {
                printf("Error reading five modified unvaccinated people.\n");
                exit(1);
            }

            for (int j = 4; j >= 0 && i < currentID - 1; i++) {
                if (registrations[i].isVaccinated == false) {
                    registrations[i] = fiveUnvaccinatedPeople[j--];
                }
            }
        }

        close(fd[0][1]);
        close(fd[1][0]);
        close(fd[2][1]);
        close(fd[3][0]);

        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);

        file = fopen("registrations.dat", "w");
        if (file == NULL) {
            fprintf(stderr, "An unexpected error occured while modfying the registration "
                            "in \"registration.dat\".\n");
            exit(1);
        }

        currentID--;
        while (0 < currentID) {
            fwrite(&registrations[currentID - 1], sizeof(struct Registration), 1, file);
            currentID--;
        }

        if(&fwrite == 0) {
            fprintf(stderr, "An unexpected error occured while vaccinating.\n"); 
            fclose(file);
            exit(1);
        } 

        fclose(file);
        free(registrations);
    }
}

void run(bool isAdmin) {
    struct Registration registration;
    registration.isRegistrationCreated = false;

    enum MenuOption selectedMenuOption = SHOW_MAIN_MENU;
    bool isRunning = true;
    while (isRunning) {
        switch(selectedMenuOption) {
            case SHOW_MAIN_MENU:
                system("clear");
                printMenu(isAdmin, &registration);
                selectedMenuOption = getMenuOptionFromInput(isAdmin, &registration);
                break;
            case CREATE_REGISTRATION:
                createRegistration(&registration);
                selectedMenuOption = SHOW_MAIN_MENU;
                break;
            case COMMIT_REGISTRATION:
                system("clear");
                commitRegistration(&registration);
                printMenu(isAdmin, &registration);
                selectedMenuOption = getMenuOptionFromInput(isAdmin, &registration);
                break;
            case MODIFY_REGISTRATION:
                modifyRegistration(&registration);
                selectedMenuOption = SHOW_MAIN_MENU;
                break;
            case DELETE_REGISTRATION:
                deleteRegistration(&registration);
                selectedMenuOption = SHOW_MAIN_MENU;
                break;
            case LIST_ALL_REGISTRATIONS:
                system("clear");
                printAllRegistrations();
                printMenu(isAdmin, &registration);
                selectedMenuOption = getMenuOptionFromInput(isAdmin, &registration);
                break;
            case MODIFY_SPECIFIC_REGISTRATION:
                modifyRegistrationByIDIDPrompt();
                printMenu(isAdmin, &registration);
                selectedMenuOption = getMenuOptionFromInput(isAdmin, &registration);
                break;
            case DELETE_SPECIFIC_REGISTRATION:
                deleteRegistrationByIDIDPrompt();
                printMenu(isAdmin, &registration);
                selectedMenuOption = getMenuOptionFromInput(isAdmin, &registration);
                break;
            case QUIT:
                system("clear");
                isRunning = false;
                break;
            case VACCINATE:
                vaccinate();
                printMenu(isAdmin, &registration);
                selectedMenuOption = getMenuOptionFromInput(isAdmin, &registration);
                break;
        }
    }
}

int main(int argc, char** argv) {
    if (argc == 2 && !strcmp(argv[1], "admin")) {
        run(true);
    } else {
        run(false);
    }

    return 0;
}
