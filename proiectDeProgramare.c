/*Sistem gestionare jocuri consolă
Aplicația permite vizualizarea unei *liste de jocuri deja instalate* (jocul este descris prin nume și
dimensiune dată în GB). De asemenea, se poate vizualiza o listă cu toate jocurile disponibile
pentru consola respectivă. Utilizatorul poate instala / dezinstala jocuri. Memoria consolei este
de 1TB (adică 1024 GB). Aplicația permite sortarea listei de jocuri deja instalate în funcție de 
nume sau de dimensiune, criteriu ales de utilizator.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

struct Jocuri {
    char numeJoc[50];
    int dimJoc;
}jocuriInstalate[100], jocuriDisponibile[100];
typedef struct Jocuri Jocuri;


void afisareMeniuPrincipal();
int citireVarianta();
void afisareJocuriInstalate();
void afisareJocuriDisponibile();





int dimensiuneJocuriInstalate=0;
const int dimTotalaConsola = 1024;

int main() 
{

    while (true)
    {
        system("cls");
        afisareMeniuPrincipal();

        int choice = citireVarianta();

        system("cls");
        switch (choice)
        {
            case 1:
                FILE* fp = fopen("jocinstalate.txt", "r");
                int i = 0;
                while (!feof(fp)) {
                    fgets(jocuriInstalate[i].numeJoc, 50, fp);
                    jocuriInstalate[i].numeJoc[strcspn(jocuriInstalate[i].numeJoc, "\n")] = '\0';
                    char line[50];
                    fgets(line, 50, fp);
                    if (strlen(line) > 1) {
                        sscanf(line, "%d", &jocuriInstalate[i].dimJoc);
                        i++;
                    }
                }
            fclose(fp);
                int nrJocuriInstalate = i;

                ///////////////////////////////////////////////////////*

                afisareJocuriInstalate(jocuriInstalate, nrJocuriInstalate);

                break;

            case 2:

                printf("-= JOCURI DISPONIBILE =-\n");

                FILE* fd = fopen("jocuridisponibile.txt", "a+");

                i=0;

                while(!feof(fd)) {
                    fgets(jocuriDisponibile[i].numeJoc, 50, fd);
                    jocuriDisponibile[i].numeJoc[strcspn(jocuriDisponibile[i].numeJoc, "\n")] = '\0';
                    char line[50];
                    fgets(line, 50, fd);
                    if (strlen(line) > 1) {
                        sscanf (line, "%d", &jocuriDisponibile[i].dimJoc);
                        i++;
                    }
                }
                fclose(fd);
                int nrJocuriDisponibile = i;
                afisareJocuriDisponibile(jocuriDisponibile, nrJocuriDisponibile);
                break;

            case 3:
                return 0;
            default:
                printf("Optiune invalida!\n");
        }



        //system("pause");
    }


    return 0;
}

void afisareMeniuPrincipal() {
    printf("-= MENIU PRINCIPAL =-\n");
    printf("1. Jocuri instalate\n");
    printf("2. Jocuri disponibile\n");
    printf("3. Exit\n");
}

int citireVarianta() {
    int choice;
    do {
        printf("Introdu varianta dorita: ");
        scanf("%d", &choice);
    } while (choice < 1 || choice > 3);
    return choice;
}


/// case 1
void afisareJocuriInstalate(struct Jocuri jocuriInstalate[100], int nrJocuriInstalate) {
    printf("-= JOCURI INSTALATE =-\n");
    extern int dimensiuneJocuriInstalate;
    dimensiuneJocuriInstalate = 0;
    for (int i = 0; i < nrJocuriInstalate; ++i) {
        dimensiuneJocuriInstalate += jocuriInstalate[i].dimJoc;
        printf("%d. %s   %d GB\n", i+1, jocuriInstalate[i].numeJoc, jocuriInstalate[i].dimJoc);
    }

    printf("...................................\n");
    printf("%d. Dezinstalare joc\n", nrJocuriInstalate+1);
    printf("%d. Sortare jocuri dupa nume\n", nrJocuriInstalate+2);
    printf("%d. Sortare jocuri dupa dimensiune\n", nrJocuriInstalate+3);
    printf("%d. Inapoi\n", nrJocuriInstalate+4);
    printf("\n(%d GB / 1024 GB)\n\n", dimensiuneJocuriInstalate);
    printf("...................................\n");
    int choice;
    do {
        printf("Actiune: ");
        scanf("%d", &choice);
    } while(choice < nrJocuriInstalate+1 || choice > nrJocuriInstalate+4);


///Inapoi la main menu
    if (choice == nrJocuriInstalate+4) {
        system("cls");
        return afisareMeniuPrincipal();
    }

/// Dezinstalare joc
    if (choice == nrJocuriInstalate+1) {
        FILE* fd = fopen("jocuridisponibile.txt", "a+");
        FILE* fp = fopen("jocinstalate.txt", "r");
        FILE* ft = fopen("temp.txt", "w");

        int dez;
        do {
            printf("Introduceti nr. jocului pentru dezinstalare: ");
            scanf("%d", &dez);
        } while (dez < 0 || dez > nrJocuriInstalate );

        fprintf(fd, "\n%s\n", jocuriInstalate[dez-1].numeJoc);
        fprintf(fd, "%d", jocuriInstalate[dez-1].dimJoc);



        char numeJoc[50];
        int dimJoc;
        while (fscanf(fp, "%49[^\n]\n%d\n", numeJoc, &dimJoc) == 2) {
            if (strcmp(numeJoc, jocuriInstalate[dez-1].numeJoc) != 0) {
                size_t len = strlen(numeJoc);
                if (len > 0 && numeJoc[len-1] == '\n') {
                    numeJoc[len-1] = '\0';
                }
                fprintf(ft, "%s\n%d\n", numeJoc, dimJoc);
            }
        }

// Stergerea caracterului newline de la sfârșitul fișierului temp.txt
        fseek(ft, -1, SEEK_END);
        char lastChar;
        do {
            lastChar = fgetc(ft);
            fseek(ft, -2, SEEK_CUR);
        } while (lastChar == '\n');
        ftruncate(fileno(ft), ftell(ft) + 1);


        fclose(fd);
        fclose(fp);
        fclose(ft);

        remove ("jocinstalate.txt");
        rename("temp.txt", "jocinstalate.txt");
        
    
        printf("Jocul a fost dezinstalat cu succes!\n");
        system("pause");
    }
    
    

////////////////////////Sortare joc dupa nume

    if (choice == nrJocuriInstalate+2) {

        int cmpNumeJoc(const void* a, const void* b) { 
            const struct Jocuri* jocA = a;
            const struct Jocuri* jocB = b;
            return strcmp(jocA->numeJoc, jocB->numeJoc);
        }    // utilizăm funcția qsort pentru a sorta jocurile după nume
            qsort(jocuriInstalate, nrJocuriInstalate, sizeof(struct Jocuri), cmpNumeJoc);

            // rescriem lista sortată în fișierul jocinstalate.txt
            FILE* fp = fopen("jocinstalate.txt", "w");
            for (int i = 0; i < nrJocuriInstalate; i++) {
                fprintf(fp, "%s\n", jocuriInstalate[i].numeJoc);
                fprintf(fp, "%d\n", jocuriInstalate[i].dimJoc);

            }

        /// Eliminarea ultimului newline din fisier
            fseek(fp, -1, SEEK_END);
                char lastChar;
                do {
                    lastChar = fgetc(fp);
                    fseek(fp, -2, SEEK_CUR);
                } while (lastChar == '\n');
                ftruncate(fileno(fp), ftell(fp) + 1);

            fclose(fp);
    }


/////////////////////Sortare joc dupa dimensiune

    if (choice == nrJocuriInstalate+3) {
        int cmpDimensiuneJoc(const void* a, const void* b) {
            const struct Jocuri* jocA = a;
            const struct Jocuri* jocB = b;
            return jocB->dimJoc - jocA->dimJoc;
        
    }

        // utilizăm funcția qsort pentru a sorta jocurile după dimensiune
        qsort(jocuriInstalate, nrJocuriInstalate, sizeof(struct Jocuri), cmpDimensiuneJoc);

        // rescriem lista sortată în fișierul jocinstalate.txt
        FILE* fp = fopen("jocinstalate.txt", "w");
        for (int i = 0; i < nrJocuriInstalate; i++) {
            fprintf(fp, "%s\n", jocuriInstalate[i].numeJoc);
            fprintf(fp, "%d\n", jocuriInstalate[i].dimJoc);

        }

    /// Eliminarea ultimului newline din fisier
        fseek(fp, -1, SEEK_END);
            char lastChar;
            do {
                lastChar = fgetc(fp);
                fseek(fp, -2, SEEK_CUR);
            } while (lastChar == '\n');
            ftruncate(fileno(fp), ftell(fp) + 1);

        fclose(fp);
    }
}




/// case 2
void afisareJocuriDisponibile(struct Jocuri jocuriDisponibile[100], int nrJocuriDisponibile) {
    extern int dimensiuneJocuriInstalate;
    for (int i=0; i<nrJocuriDisponibile; ++i) {
        printf("%d. %s   %d GB\n", i+1, jocuriDisponibile[i].numeJoc, jocuriDisponibile[i].dimJoc);
    }

    printf("...................................\n");
    printf("%d. Instalare joc\n", nrJocuriDisponibile+1);
    printf("%d. Inapoi\n", nrJocuriDisponibile+2);
    printf("\n(%d GB / 1024 GB)\n\n", dimensiuneJocuriInstalate);
    printf("...................................\n");

    int choice;
    do {
        printf("Actiune: ");
        scanf("%d", &choice);
    } while(choice < nrJocuriDisponibile+1 || choice > nrJocuriDisponibile+2);

///Inapoi la main menu
    if (choice == nrJocuriDisponibile+2) {
        system("cls");
        return afisareMeniuPrincipal();
    }



    if (choice == nrJocuriDisponibile+1) {
        FILE* fd = fopen("jocuridisponibile.txt", "r");
        FILE* fp = fopen("jocinstalate.txt", "a+");
        FILE* ft2 = fopen("temp2.txt", "w");

        int ins;
        do {
            printf("Introduceti nr. jocului pentru instalare: ");
            scanf("%d", &ins);
            
        } while (ins < 0 || ins > nrJocuriDisponibile );

        if (dimensiuneJocuriInstalate+jocuriDisponibile[ins-1].dimJoc > 1024) {
        printf("Nu exista suficient spatiu de stocare!\n");
        system("pause"); }
        else {
        fprintf(fp, "\n%s\n", jocuriDisponibile[ins-1].numeJoc);
        fprintf(fp, "%d", jocuriDisponibile[ins-1].dimJoc);



        char numeJoc[50];
        int dimJoc;
        while (fscanf(fd, "%49[^\n]\n%d\n", numeJoc, &dimJoc) == 2) {
            if (strcmp(numeJoc, jocuriDisponibile[ins-1].numeJoc) != 0) {
                size_t len = strlen(numeJoc);
                if (len > 0 && numeJoc[len-1] == '\n') {
                    numeJoc[len-1] = '\0';
                }
                fprintf(ft2, "%s\n%d\n", numeJoc, dimJoc);
            }
        }

// Stergerea caracterului newline de la sfârșitul fișierului temp.txt
        fseek(ft2, -1, SEEK_END);
        char lastChar;
        do {
            lastChar = fgetc(ft2);
            fseek(ft2, -2, SEEK_CUR);
        } while (lastChar == '\n');
        ftruncate(fileno(ft2), ftell(ft2) + 1);


        fclose(fd);
        fclose(fp);
        fclose(ft2);

        remove ("jocuridisponibile.txt");
        rename("temp2.txt", "jocuridisponibile.txt");

        printf("Joc instalat cu succes!\n");
        system("pause");
        }
        //system("pause");
        

}
}
