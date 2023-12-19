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
void dezinstalareJoc(NodArbore** root, int* dimensiuneJocuriInstalate);
NodArbore* cautareJoc(NodArbore* root, int dezinstalare);
void salvareFisier(const char* numeFisier, NodArbore* root);
void salvareFisierRecursiv(NodArbore* root, FILE* fp);
void afisareOrdineBSTNumerotat(NodArbore* root, int* contor);
int countNodes(NodArbore* root);
void copiereArbore(NodArbore* root, NodArbore** newRoot, int dezinstalare);
void instalareJoc(NodArbore** root, int* dimensiuneJocuriInstalate);
NodArbore* rootJocuriInstalate = NULL;
NodArbore* rootJocuriDisponibile = NULL;
int dimensiuneJocuriInstalate = 0;
int dimensiuneJocuriDisponibile = 0;
const int DIMENSIUNE = 1024;

int main() {
    


    while(1) {
        afisareMeniuPrincipal();

        int choice = citireVarianta();

        system("cls");

        switch (choice) {
        case 1: {
            FILE* fp = fopen("jocinstalate.txt", "r");
            char numeJoc[50];
            int dimJoc;
            while (fscanf(fp, "%49[^\n]\n%d\n", numeJoc, &dimJoc) == 2) {
                rootJocuriInstalate = inserareBST(rootJocuriInstalate, numeJoc, dimJoc);
                dimensiuneJocuriInstalate += dimJoc;
            }
            fclose(fp);

            afisareJocuriInstalate(rootJocuriInstalate);
            break;
        }

        case 2: {
            FILE* fd = fopen("jocuridisponibile.txt", "r");
            char numeJoc[50];
            int dimJoc;
            while(fscanf(fd, "%49[^\n]\n%d\n", numeJoc, &dimJoc) == 2) {
                rootJocuriDisponibile = inserareBST(rootJocuriDisponibile, numeJoc, dimJoc);
                dimensiuneJocuriDisponibile += dimJoc;
            }
            fclose(fd);

            afisareJocuriDisponibile(rootJocuriDisponibile);
            break;
        }

        case 3: {
            eliberareMemorie(rootJocuriInstalate);
            eliberareMemorie(rootJocuriDisponibile);
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

        while (getchar() != '\n');

    } while (choice < 1 || choice > 3);
    return choice;
}

void afisareJocuriInstalate(NodArbore* root) {
    printf("-= JOCURI INSTALATE =-\n");
    int contor = 0;

    afisareOrdineBSTNumerotat(root, &contor);

    printf("--------------------------------------------\n");
    printf("(%d GB  /  %d  GB)\n\n", dimensiuneJocuriInstalate, DIMENSIUNE);
    printf("--------------------------------------------\n");

    dezinstalareJoc(&root, &dimensiuneJocuriInstalate);
}

void afisareOrdineBSTNumerotat(NodArbore* root, int* contor) {
        if (root != NULL) {
            afisareOrdineBSTNumerotat(root->left, contor);
            (*contor)++;
            printf("%d. %s    %d  GB\n", *contor, root->numeJoc, root->dimJoc);
            afisareOrdineBSTNumerotat(root->right, contor);
        }
}

void afisareJocuriDisponibile(NodArbore* root) {
    printf("-= JOCURI DISPONIBILE =-\n");
    int contor = 0;
    afisareOrdineBSTNumerotat(root, &contor);

    printf("--------------------------------------------\n");
    printf("(%d GB  /  %d  GB)\n\n", dimensiuneJocuriInstalate, DIMENSIUNE);
    printf("--------------------------------------------\n");
    instalareJoc(&rootJocuriDisponibile, &dimensiuneJocuriDisponibile);
}

void dezinstalareJoc(NodArbore** root, int* dimensiuneJocuriInstalate) {
    int dezinstalare;
    do {
        printf("Introduceti nr. jocului pentru dezinstalare sau 0 pentru a reveni la meniu principal: ");
        scanf("%d", &dezinstalare);

        if (dezinstalare == 0) {
            return;
        }
    } while (dezinstalare < 0 || dezinstalare > *dimensiuneJocuriInstalate);

    NodArbore* jocDezinstalat = cautareJoc(*root, dezinstalare);
    printf("Jocul %s a fost dezinstalat cu succes!\n", jocDezinstalat->numeJoc);

    FILE* fd = fopen("jocuridisponibile.txt", "a+");
    if (fd == NULL) {
        printf("Eroare la crearea fisierului jocuridisponibile.txt!\n");
        exit(EXIT_FAILURE);
    }

    fprintf(fd, "\n%s\n%d\n", jocDezinstalat->numeJoc, jocDezinstalat->dimJoc);
    fclose(fd);

    *dimensiuneJocuriInstalate -= jocDezinstalat->dimJoc;

    NodArbore* newRoot = NULL;
    copiereArbore(*root, &newRoot, jocDezinstalat->dimJoc);
    eliberareMemorie(*root);
    *root = newRoot;

    FILE* originalFile = fopen("jocinstalate.txt", "w");
    if (originalFile == NULL) {
        printf("Eroare la deschiderea fisierului jocinstalate.txt!\n");
        exit(EXIT_FAILURE);
    }

    salvareFisierRecursiv(newRoot, originalFile);

    fclose(originalFile);

    printf("--------------------------------------------\n");
    printf("(%d GB  /  %d  GB)\n\n", *dimensiuneJocuriInstalate, DIMENSIUNE);
    printf("--------------------------------------------\n");
    system("pause");
}


void instalareJoc(NodArbore** root, int* dimensiuneJocuriDisponibile) {
    int instalare;
    do {
        printf("Introduceti nr. jocului pentru instalare sau 0 pentru a reveni la meniu principal: ");
        scanf("%d", &instalare);

        if (instalare == 0) {
            return;
        }
    } while (instalare < 0 || instalare > *dimensiuneJocuriDisponibile+1);

    NodArbore* jocInstalat = cautareJoc(*root, instalare);
    
    if (jocInstalat == NULL) {
        printf("Jocul nu a fost gasit!\n");
        return;
    }

    printf("Jocul %s a fost instalat cu succes!\n", jocInstalat->numeJoc);

    FILE* fd = fopen("jocinstalate.txt", "a+");
    if (fd == NULL) {
        printf("Eroare la crearea fisierului jocinstalate.txt!\n");
        exit(EXIT_FAILURE);
    }

    fprintf(fd, "\n%s\n%d\n", jocInstalat->numeJoc, jocInstalat->dimJoc);
    fclose(fd);

    *dimensiuneJocuriDisponibile += jocInstalat->dimJoc;

    NodArbore* newRoot = NULL;
    copiereArbore(*root, &newRoot, jocInstalat->dimJoc);
    eliberareMemorie(*root);
    *root = newRoot;

    FILE* originalFile = fopen("jocuridisponibile.txt", "w");
    if (originalFile == NULL) {
        printf("Eroare la deschiderea fisierului jocuridisponibile.txt!\n");
        exit(EXIT_FAILURE);
    }

    salvareFisierRecursiv(newRoot, originalFile);

    fclose(originalFile);

    printf("--------------------------------------------\n");
    printf("(%d GB  /  %d  GB)\n\n", *dimensiuneJocuriDisponibile, DIMENSIUNE);
    printf("--------------------------------------------\n");
    system("pause");
}




void copiereArbore(NodArbore* root, NodArbore** newRoot, int dezinstalare) {
    if (root != NULL) {
        int leftSize = countNodes(root->left);

        if (root->dimJoc != dezinstalare) {
            *newRoot = inserareBST(*newRoot, root->numeJoc, root->dimJoc);
        }

        copiereArbore(root->left, newRoot, dezinstalare);
        copiereArbore(root->right, newRoot, dezinstalare);
    }
}



NodArbore* cautareJoc(NodArbore* root, int dezinstalare) {
    if (root == NULL || dezinstalare == 1) {
        return root;
    }

    int leftSize = 0;
    if (root->left != NULL) {
        leftSize = countNodes(root->left);
    }

    if (dezinstalare <= leftSize + 1) {
        return cautareJoc(root->left, dezinstalare);
    } else {
        return cautareJoc(root->right, dezinstalare - leftSize - 1);
    }
}

int countNodes(NodArbore* root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + countNodes(root->left) + countNodes(root->right);
}


void salvareFisier(const char* numeFisier, NodArbore* root) {
    FILE* fp = fopen(numeFisier, "w");
    if (fp == NULL) {
        printf("Eroare la deschiderea fisierului!\n");
        exit(EXIT_FAILURE);
    }

    char jocDezinstalat[50];

    salvareFisierRecursiv(root, fp);
    fclose(fp);
}

void salvareFisierRecursiv(NodArbore* root, FILE* fp) {
    if (root != NULL) {
        fprintf(fp, "%s\n%d\n", root->numeJoc, root->dimJoc);
        salvareFisierRecursiv(root->left, fp);
        salvareFisierRecursiv(root->right, fp);
    }
}
