#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct NodArbore {
    char numeJoc[50];
    int dimJoc;
    struct NodArbore* left;
    struct NodArbore* right;
};

typedef struct NodArbore NodArbore;

NodArbore* creareNod(const char* numeJoc, int dimJoc) {
    NodArbore* nod = (NodArbore*) malloc(sizeof(NodArbore));
    strcpy(nod->numeJoc, numeJoc);
    nod->dimJoc = dimJoc;
    nod->left = nod->right = NULL;
    return nod;
}

NodArbore* inserareBST(NodArbore* root, const char* numeJoc, int dimJoc) {
    if (root == NULL)
        return creareNod(numeJoc, dimJoc);

    int compare = strcmp(numeJoc, root->numeJoc);

    if (compare < 0) {
        root->left = inserareBST(root->left, numeJoc, dimJoc);
    } else if (compare > 0) {
        root->right = inserareBST(root->right, numeJoc, dimJoc);
    }
    return root;
}

void afisareOrdineBST(NodArbore* root) {
    if (root != NULL) {
        afisareOrdineBST(root->left);
        printf("%s    %d GB\n", root->numeJoc, root->dimJoc);
        afisareOrdineBST(root->right);
    }
}

void eliberareMemorie(NodArbore* root) {
    if (root != NULL) {
        eliberareMemorie(root->left);
        eliberareMemorie(root->right);
        free(root);
    }
}

void afisareMeniuPrincipal();
int citireVarianta();
void afisareJocuriInstalate(NodArbore* root);
void afisareJocuriDisponibile(NodArbore* root);

int dimensiuneJocuriInstalate = 0;
const int DIMENSIUNE = 1024;

int main() {
    
    NodArbore* rootBST = NULL;

    while(1) {
        system("cls");
        afisareMeniuPrincipal();

        int choice = citireVarianta();

        system("cls");

        switch (choice) {
        case 1: {
            FILE* fp = fopen("jocinstalate.txt", "r");
            char numeJoc[50];
            int dimJoc;
            while (fscanf(fp, "%49[^\n]\n%d\n", numeJoc, &dimJoc) == 2) {
                rootBST = inserareBST(rootBST, numeJoc, dimJoc);
                dimensiuneJocuriInstalate += dimJoc;
            }
            fclose(fp);

            afisareJocuriInstalate(rootBST);
            break;
        }

        case 2: {
            FILE* fd = fopen("jocuridisponibile.txt", "r");
            char numeJoc[50];
            int dimJoc;
            while(fscanf(fd, "%49[^\n]\n%d\n", numeJoc, &dimJoc) == 2) {
                rootBST = inserareBST(rootBST, numeJoc, dimJoc);
            }
            fclose(fd);

            afisareJocuriDisponibile(rootBST);
            break;
        }

        case 3: {
            eliberareMemorie(rootBST);
            return 0;
        }

        default:
            printf("Optiune invalida!\n");
        }
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

        // Consumă caracterele rămase în buffer-ul de intrare
        while (getchar() != '\n');

    } while (choice < 1 || choice > 3);
    return choice;
}

void afisareJocuriInstalate(NodArbore* root) {
    printf("-= JOCURI INSTALATE =-\n");
    afisareOrdineBST(root);

    printf("--------------------------------------------\n");
    printf("(%d GB  /  %d  GB)\n\n", dimensiuneJocuriInstalate, DIMENSIUNE);
    printf("--------------------------------------------\n");
    system("pause");
}

void afisareJocuriDisponibile(NodArbore* root) {
    printf("-= JOCURI DISPONIBILE =-\n");
    afisareOrdineBST(root);

    printf("--------------------------------------------\n");
    printf("(%d GB  /  %d  GB)\n\n", dimensiuneJocuriInstalate, DIMENSIUNE);
    printf("--------------------------------------------\n");
    system("pause");
}